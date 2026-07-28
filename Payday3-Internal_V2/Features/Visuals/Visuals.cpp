#include "pch.h"
#include "Visuals.hpp"
#include <vector>
#include <string>

#undef min
#undef max

// Calculate screen box from top and bottom points
std::optional<ImVec4> CalculateScreenBoxFromTopBottom(SDK::APlayerController* pPlayerController, SDK::AActor* pActor, 
    SDK::FVector vecTop, SDK::FVector vecBottom)
{
    SDK::FVector2D vec2Top, vec2Bottom;
    if (!pPlayerController->ProjectWorldLocationToScreen(vecBottom, &vec2Bottom, false) ||
        !pPlayerController->ProjectWorldLocationToScreen(vecTop, &vec2Top, false))
        return {};

    float flHeight = std::abs(vec2Top.Y - vec2Bottom.Y);
    float flYawRad = pActor->K2_GetActorRotation().Yaw * (3.14159265358979323846f / 180.0f);
    float flWidthRatio = 0.25f + (std::abs(SDK::FVector(std::cos(flYawRad), std::sin(flYawRad), 0.f).GetNormalized().Dot(
        (pActor->K2_GetActorLocation() - pPlayerController->PlayerCameraManager->GetCameraLocation()).GetNormalized())) * 0.15f);
    
    float flWidth = flHeight * flWidthRatio;
    float flCenter = (vec2Bottom.X + vec2Top.X) / 2.0f;
    return ImVec4{ flCenter - (flWidth / 2.0f), std::min(vec2Bottom.Y, vec2Top.Y), 
                   flCenter + (flWidth / 2.0f), std::max(vec2Bottom.Y, vec2Top.Y) };
}

// Calculate screen box for characters
std::optional<ImVec4> CalculateScreenBoxForCharacter(SDK::USkeletalMeshComponent* pMeshComponent, 
    SDK::APlayerController* pPlayerController, SDK::AActor* pActor)
{
    if (!pMeshComponent || !pPlayerController || !pActor)
        return {};

    return CalculateScreenBoxFromTopBottom(
        pPlayerController, 
        pActor, 
        pMeshComponent->GetSocketLocation(SDK::UKismetStringLibrary::Conv_StringToName(L"HeadEnd")), 
        pMeshComponent->GetSocketLocation(SDK::UKismetStringLibrary::Conv_StringToName(L"Reference"))
    );
}

void Visuals::BuildBoneCache(SDK::USkeletalMeshComponent* mesh, BoneCache& cache)
{
    if (!mesh)
        return;

    const int numBones = mesh->GetNumBones();

    for (int i = 0; i < numBones; ++i)
    {
        const std::string bone = mesh->GetBoneName(i).ToString();

        if (bone == "Hips")
            cache.Hips = i;

        else if (bone == "Spine")
            cache.Spine = i;

        else if (bone == "Spine1")
            cache.Spine1 = i;

        else if (bone == "Spine2")
            cache.Spine2 = i;

        else if (bone == "Spine3")
            cache.Spine3 = i;

        else if (bone == "Neck")
            cache.Neck = i;

        else if (bone == "Head")
            cache.Head = i;

        // Left arm
        else if (bone == "LeftShoulder")
            cache.LeftShoulder = i;

        else if (bone == "LeftArm")
            cache.LeftUpperArm = i;

        else if (bone == "LeftForeArm")
            cache.LeftForeArm = i;

        else if (bone == "LeftHand")
            cache.LeftHand = i;

        // Right arm
        else if (bone == "RightShoulder")
            cache.RightShoulder = i;

        else if (bone == "RightArm")
            cache.RightUpperArm = i;

        else if (bone == "RightForeArm")
            cache.RightForeArm = i;

        else if (bone == "RightHand")
            cache.RightHand = i;

        // Left leg
        else if (bone == "LeftUpLeg")
            cache.LeftUpperLeg = i;

        else if (bone == "LeftLeg")
            cache.LeftLowerLeg = i;

        else if (bone == "LeftFoot")
            cache.LeftFoot = i;

        else if (bone == "LeftToeBase")
            cache.LeftToe = i;

        // Right leg
        else if (bone == "RightUpLeg")
            cache.RightUpperLeg = i;

        else if (bone == "RightLeg")
            cache.RightLowerLeg = i;

        else if (bone == "RightFoot")
            cache.RightFoot = i;

        else if (bone == "RightToeBase")
            cache.RightToe = i;
    }

    cache.Initialized = true;
}

void Visuals::DrawBone(ImDrawList* pDrawList, SDK::APlayerController* pPlayerController, SDK::USkeletalMeshComponent* mesh, int parent, int child, ImU32 color)
{
    if (parent < 0 || child < 0)
        return;

    auto parentWorld = mesh->GetSocketLocation(mesh->GetBoneName(parent));
    auto childWorld = mesh->GetSocketLocation(mesh->GetBoneName(child));
    SDK::FVector2D p0, p1;

    if (pPlayerController->ProjectWorldLocationToScreen(parentWorld, &p0, false) && pPlayerController->ProjectWorldLocationToScreen(childWorld, &p1, false))
    {
        pDrawList->AddLine(ImVec2(p0.X - 1, p0.Y - 1), ImVec2(p1.X - 1, p1.Y - 1), IM_COL32(0, 0, 0, 255), 1.1f);
        pDrawList->AddLine(ImVec2(p0.X, p0.Y), ImVec2(p1.X, p1.Y), color, 1.0f);
    }
}

bool Visuals::SetupMenu()
{
    Localization::AddToLocale("ENG", std::initializer_list<std::pair<size_t, std::string>>{
        { "VISUALS_BUTTON"Hashed, "Visuals" },
        { "VISUALS_TAB1"Hashed, "ESP" },
        { "VISUALS_TAB1_LEFT"Hashed, "ESP" },
        { "VISUALS_TAB1_RIGHT"Hashed, "Colors" },
        { "VISUALS_TAB1_BOTTOM"Hashed, "Filters" },

        { "VISUALS_BOUNDING_BOX"Hashed, "Bounding Box" },
        { "VISUALS_BOUNDING_BOX_COP_COLOR"Hashed, "Bounding Box Cop Color" },
        { "VISUALS_BOUNDING_BOX_CIVILIAN_COLOR"Hashed, "Bounding Box Civilian Color" },

        { "VISUALS_NAME"Hashed, "Name" },
        { "VISUALS_NAME_COP_COLOR"Hashed, "Name Cop Color" },
        { "VISUALS_NAME_CIVILIAN_COLOR"Hashed, "Name Civilian Color" },

        { "VISUALS_DISTANCE"Hashed, "Distance" },
        { "VISUALS_DISTANCE_COP_COLOR"Hashed, "Distance Cop Color" },
        { "VISUALS_DISTANCE_CIVILIAN_COLOR"Hashed, "Distance Civilian Color" },

        { "VISUALS_HEALTH_BAR"Hashed, "Health Bar" },
        { "VISUALS_HEALTH_BAR_COP_COLOR"Hashed, "Health Bar Cop Color" },
        { "VISUALS_HEALTH_BAR_CIVILIAN_COLOR"Hashed, "Health Bar Civilian Color" },

        { "VISUALS_ARMOR_BAR"Hashed, "Armor Bar" },
        { "VISUALS_ARMOR_BAR_COLOR"Hashed, "Armor Bar Color" },

        { "VISUALS_SKELETON"Hashed, "Skeleton" },
        { "VISUALS_SKELETON_COP_COLOR"Hashed, "Skeleton Cop Color" },
        { "VISUALS_SKELETON_CIVILIAN_COLOR"Hashed, "Skeleton Civilian Color" },

        { "VISUALS_HIGHLIGHT"Hashed, "Highlight" },

        { "VISUALS_KEY_ITEM"Hashed, "Key Items" },
        { "VISUALS_KEY_ITEM_COLOR"Hashed, "Key Items Color" },

        { "VISUALS_FILTERS"Hashed, "" },
        { "VISUALS_COP"Hashed, "Cops" },
        { "VISUALS_CIVILIAN"Hashed, "Civilians" },
        { "VISUALS_ITEMS"Hashed, "Items" },
        { "VISUALS_MONEY"Hashed, "Money" }
    });

    return true;
}

void Visuals::UpdateMenuVisibility()
{
    const bool copSelected = m_pFilters->IsSelected(0);
    const bool civilianSelected = m_pFilters->IsSelected(1);

    // Bounding Box
    m_pBoundingBoxCopColor->SetVisible(m_pBoundingBox->GetValue() && copSelected);
    m_pBoundingBoxCivilianColor->SetVisible(m_pBoundingBox->GetValue() && civilianSelected);

    // Name
    m_pNameCopColor->SetVisible(m_pName->GetValue() && copSelected);
    m_pNameCivilianColor->SetVisible(m_pName->GetValue() && civilianSelected);

    // Distance
    m_pDistanceCopColor->SetVisible(m_pDistance->GetValue() && copSelected);
    m_pDistanceCivilianColor->SetVisible(m_pDistance->GetValue() && civilianSelected);

    // Health
    m_pHealthBarCopColor->SetVisible(m_pHealthBar->GetValue() && copSelected);
    m_pHealthBarCivilianColor->SetVisible(m_pHealthBar->GetValue() && civilianSelected);

    // Armor
    m_pArmorBarColor->SetVisible(m_pArmorBar->GetValue());

    // Skeleton
    m_pSkeletonCopColor->SetVisible(m_pSkeleton->GetValue() && copSelected);
    m_pSkeletonCivilianColor->SetVisible(m_pSkeleton->GetValue() && civilianSelected);

    // Key Items
    m_pKeyItemColor->SetVisible(m_pKeyItem->GetValue());
}

void Visuals::HandleMenu()
{
    static std::once_flag onceflag;
    std::call_once(onceflag, [this]() {
        auto pHeaderGroup = static_cast<HeaderGroup*>(Framework::menu->GetChild("HEADER_GROUP"));
        if (pHeaderGroup)
            pHeaderGroup->AddHeaders(Visuals::s_iVisualsPageId, { "VISUALS_TAB1"Hashed });

        m_pTab1Left->SetCallback([]() {
            return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() + 60.0f + 30.0f) / 2);
        });
        m_pTab1Right->SetCallback([]() {
            return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() + 60.0f + 30.0f) / 2);
        });
        m_pTab1Bottom->SetCallback([]() {
            return ImVec2((ImGui::GetWindowWidth() - 10.0f * 2), (ImGui::GetWindowHeight() - 40.0f - 60.0f * 2) / 2);
        });

        // Add elements
        m_pTab1Left->AddElement(m_pBoundingBox.get());
        m_pTab1Right->AddElement(m_pBoundingBoxCopColor.get());
        m_pTab1Right->AddElement(m_pBoundingBoxCivilianColor.get());
        m_pBoundingBoxCopColor->SetValue(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        m_pBoundingBoxCivilianColor->SetValue(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

        m_pTab1Left->AddElement(m_pName.get());
        m_pTab1Right->AddElement(m_pNameCopColor.get());
        m_pTab1Right->AddElement(m_pNameCivilianColor.get());
        m_pNameCopColor->SetValue(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pNameCivilianColor->SetValue(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));

        m_pTab1Left->AddElement(m_pDistance.get());
        m_pTab1Right->AddElement(m_pDistanceCopColor.get());
        m_pTab1Right->AddElement(m_pDistanceCivilianColor.get());
        m_pDistanceCopColor->SetValue(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        m_pDistanceCivilianColor->SetValue(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

        m_pTab1Left->AddElement(m_pHealthBar.get());
        m_pTab1Right->AddElement(m_pHealthBarCopColor.get());
        m_pTab1Right->AddElement(m_pHealthBarCivilianColor.get());
        m_pHealthBarCopColor->SetValue(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        m_pHealthBarCivilianColor->SetValue(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

        m_pTab1Left->AddElement(m_pArmorBar.get());
        m_pTab1Right->AddElement(m_pArmorBarColor.get());
        m_pArmorBarColor->SetValue(ImVec4(0.0f, 0.5f, 1.0f, 1.0f));
        
        m_pTab1Left->AddElement(m_pSkeleton.get());
        m_pTab1Right->AddElement(m_pSkeletonCopColor.get());
        m_pTab1Right->AddElement(m_pSkeletonCivilianColor.get());
        m_pSkeletonCopColor->SetValue(ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        m_pSkeletonCivilianColor->SetValue(ImVec4(0.0f, 1.0f, 1.0f, 1.0f));

        m_pTab1Left->AddElement(m_pHighlight.get());

        m_pTab1Left->AddElement(m_pKeyItem.get());
        m_pTab1Right->AddElement(m_pKeyItemColor.get());
        m_pKeyItemColor->SetValue(ImVec4(0.0f, 1.0f, 1.0f, 1.0f));

        // Filters
        m_pFilters->AddOption("COP", [](bool bEnabled) {
            //Utils::LogDebug("Cops selected");
        });
        m_pFilters->AddOption("CIVILIAN", [](bool bEnabled) {
            //Utils::LogDebug("Civilians selected");
        });
        m_pFilters->AddOption("ITEMS", [](bool bEnabled) {
            //Utils::LogDebug("Items selected");
        });
        m_pFilters->AddOption("MONEY", [](bool bEnabled) {
            //Utils::LogDebug("Money selected");
        });

        m_pTab1Bottom->AddElement(m_pFilters.get());

        m_pTab1Group->AddElement(m_pTab1Left.get());
        m_pTab1Group->AddElement(m_pTab1Right.get());
        m_pTab1Group->AddElement(m_pTab1Bottom.get());
        m_pTab1Page->AddElement(m_pTab1Group.get());

        Framework::menu->GetChild("HEADER_GROUP")->GetChild("BODY")->AddElement(m_pTab1Page.get());
    });

    UpdateMenuVisibility();
}

//Before you ask, yes, this function is a fucking mess. I will clean it up another time, but for now it works so boohoo cry about it.
//Another thing you may ask is "why is it all done in render() and not in both render() and run() ?" The answer is simple: different timings, render() runs every frame, run() runs 10 times per second. -
//If we split it between the 2 then ESP gets delayed and looks god awful compared to render() which runs every frame.
void Visuals::Render()
{
    // All must be assigned here to ensure they are updated in case of user changes
    const bool drawBox      = m_pBoundingBox->GetValue();
    const bool drawName     = m_pName->GetValue();
    const bool drawDistance = m_pDistance->GetValue();
    const bool drawHealthBar = m_pHealthBar->GetValue();
    const bool drawArmorBar = m_pArmorBar->GetValue();
    const bool drawSkeleton = m_pSkeleton->GetValue();
    const bool drawHighlight = m_pHighlight->GetValue();

    const ImU32 boxCopColor = ImGui::ColorConvertFloat4ToU32(m_pBoundingBoxCopColor->GetValue());
    const ImU32 boxCivilianColor = ImGui::ColorConvertFloat4ToU32(m_pBoundingBoxCivilianColor->GetValue());

    const ImU32 nameCopColor = ImGui::ColorConvertFloat4ToU32(m_pNameCopColor->GetValue());
    const ImU32 nameCivilianColor = ImGui::ColorConvertFloat4ToU32(m_pNameCivilianColor->GetValue());

    const ImU32 distanceCopColor = ImGui::ColorConvertFloat4ToU32(m_pDistanceCopColor->GetValue());
    const ImU32 distanceCivilianColor = ImGui::ColorConvertFloat4ToU32(m_pDistanceCivilianColor->GetValue());

    const ImU32 healthBarCopColor = ImGui::ColorConvertFloat4ToU32(m_pHealthBarCopColor->GetValue());
    const ImU32 healthBarCivilianColor = ImGui::ColorConvertFloat4ToU32(m_pHealthBarCivilianColor->GetValue());

    const ImU32 armorBarColor = ImGui::ColorConvertFloat4ToU32(m_pArmorBarColor->GetValue());

    const ImU32 skeletonCopColor = ImGui::ColorConvertFloat4ToU32(m_pSkeletonCopColor->GetValue());
    const ImU32 skeletonCivilianColor = ImGui::ColorConvertFloat4ToU32(m_pSkeletonCivilianColor->GetValue());

    // Clear or else it'll look like you got concussed with a cod nade. Trust me...
    m_vESPData.clear();
    m_vESPData.reserve(256); // Reserving space for 256 objects to avoid frequent reallocations, but may need to increase in the future if more objects are present than expected.

    if (!drawBox && !drawName && !drawDistance && !drawHealthBar && !drawArmorBar && !drawSkeleton && !drawHighlight)
        return;

    SDK::UWorld* pGWorld = SDK::UWorld::GetWorld();
    if (!pGWorld || !pGWorld->PersistentLevel)
        return;

    SDK::APlayerController* pPlayerController = SDK::UGameplayStatics::GetPlayerController(pGWorld, 0);
    if (!pPlayerController || !pPlayerController->PlayerCameraManager)
        return;

    SDK::APlayerCameraManager* pCameraManager = pPlayerController->PlayerCameraManager;
    if (!pCameraManager)
        return;
    SDK::FVector vecCameraLocation = pCameraManager->GetCameraLocation();

    // Get the local player
    auto pLocalPlayer = pPlayerController->AcknowledgedPawn;
    if (!pLocalPlayer)
        return;

    // Get world runtime for actors
    SDK::USBZWorldRuntime* pWorldRuntime = SDK::USBZWorldRuntime::Get(pGWorld);
    if (!pWorldRuntime)
        return;

    // Get filter states
    bool bShowCops = m_pFilters->IsSelected(0);
    bool bShowCivilians = m_pFilters->IsSelected(1);
    bool bShowItems = m_pFilters->IsSelected(2);
    bool bShowMoney = m_pFilters->IsSelected(3);

    UC::TArray<SDK::UObject*>& actors = pWorldRuntime->AllPawns->Objects;
    
    for (int i = 0; i < actors.Num(); ++i) {
        if (!actors.IsValidIndex(i))
            break;

        auto pActor = reinterpret_cast<SDK::AActor*>(actors[i]);
        if (!pActor)
            continue;

        // Skip local player
        if (pActor == pLocalPlayer)
            continue;

        // Check if character is alive
        auto pCharacter = reinterpret_cast<SDK::ASBZCharacter*>(pActor);
        if (!pCharacter)
            continue;
        
        if (!pCharacter->bIsAlive)
            continue;

        EnemyType type = EnemyType::None;

        auto it = m_ClassCache.find(pActor->Class);

        if (it != m_ClassCache.end())
        {
            type = it->second;
        }
        else
        {
            std::string className = pActor->Class->Name.ToString();

            //Utils::LogDebug("Class name:" + className);

            std::transform(
                className.begin(),
                className.end(),
                className.begin(),
                [](unsigned char c)
                {
                    return static_cast<char>(std::tolower(c));
                });

            for (const auto& entry : g_EnemyLookup)
            {
                if (className.find(entry.Keyword) != std::string::npos)
                {
                    type = entry.Type;
                    break;
                }
            }

            m_ClassCache.try_emplace(pActor->Class, type);
        }

        const EnemyInfo& info = g_EnemyInfo[static_cast<size_t>(type)];

        bool bIsCop       = info.Category == EnemyCategory::Cop;
        bool bIsCivilian  = info.Category == EnemyCategory::Civilian;

        const char* sName = info.Name;

        if (!bIsCop && !bIsCivilian)
            continue;

        // Apply filters
        if (bIsCop && !bShowCops)
            continue;
        if (bIsCivilian && !bShowCivilians)
            continue;

        // Get skeletal mesh
        SDK::USkeletalMeshComponent* pMesh = pCharacter->Mesh;
        if (!pMesh)
            continue;

        // Calculate screen box
        auto optScreenBox = CalculateScreenBoxForCharacter(pMesh, pPlayerController, pActor);
        if (!optScreenBox.has_value())
            continue;

        ImVec4 vec4ScreenBox = optScreenBox.value();

        // Calculate distance
        float flDistance = (pActor->K2_GetActorLocation() - vecCameraLocation).Magnitude() / 100.0f;

        auto* AbilitySystem = pCharacter->AbilitySystem; // offset 0x0A48
        if (!AbilitySystem) return;

        float flHealth;
        float flHealthMax;
        float flArmor;
        float flArmorMax;

        // Iterate SpawnedAttributes (TArray at 0x0150)
        UC::TArray<SDK::UAttributeSet*>* spawnedAttrs = (UC::TArray<SDK::UAttributeSet*>*)((uintptr_t)AbilitySystem + 0x0150);
        for (int i = 0; i < spawnedAttrs->Num(); i++) {
            auto* attrSet = (*spawnedAttrs)[i];
            if (attrSet && attrSet->IsA(SDK::USBZPawnAttributeSet::StaticClass())) {
                auto* pawnAttrs = static_cast<SDK::USBZPawnAttributeSet*>(attrSet);
                flHealth = pawnAttrs->Health.CurrentValue;   // 0x0090 + 0x0C
                flHealthMax = pawnAttrs->HealthMax.CurrentValue; // 0x00A0 + 0x0C
                flArmor = pawnAttrs->Armor.CurrentValue;     // 0x00B0 + 0x0C
                flArmorMax = pawnAttrs->ArmorMax.CurrentValue; // 0x00C0 + 0x0C
                break;
            }
        }

        m_vESPData.push_back({
            vec4ScreenBox,
            std::string(sName),
            pMesh,
            pCharacter,
            flHealth,
            flHealthMax,
            flArmor,
            flArmorMax,
            flDistance,
            bIsCop,
            bIsCivilian
        });
    }

    if (m_vESPData.empty())
        return;

    ImDrawList* pDrawList = ImGui::GetBackgroundDrawList();
    if (!pDrawList)
        return;

    for (const auto& esp : m_vESPData)
    {
        // Bounding box
        if (drawBox)
        {
            const ImU32 color = esp.IsCop ? boxCopColor : boxCivilianColor;

            pDrawList->AddRect(
                ImVec2(esp.Box.x - 1.0f, esp.Box.y - 1.0f),
                ImVec2(esp.Box.z + 1.0f, esp.Box.w + 1.0f),
                IM_COL32(0, 0, 0, 255),
                0.0f,
                0,
                1.6f
            );

            pDrawList->AddRect(
                ImVec2(esp.Box.x, esp.Box.y),
                ImVec2(esp.Box.z, esp.Box.w),
                color,
                0.0f,
                0,
                1.5f
            );
        }

        if (drawName || drawDistance)
        {
            std::string nameText;
            std::string distanceText;

            if (drawName)
            {
                const char* name = esp.Name.c_str();
                if (name && name[0])
                {
                    // Validate the string
                    bool valid = true;
                    size_t len = strlen(name);
                    if (len > 0 && len < 256)
                    {
                        for (size_t i = 0; i < len; ++i)
                        {
                            if (name[i] < 32 || name[i] > 126)
                            {
                                valid = false;
                                break;
                            }
                        }
                        if (valid)
                            nameText = name;
                        else
                            nameText = "Invalid";
                    }
                    else
                    {
                        nameText = "Unknown";
                    }
                }
                else
                {
                    nameText = "Unknown";
                }
            }

            if (drawDistance)
            {
                if (esp.Distance >= 0 && esp.Distance < 10000)
                {
                    char distanceBuffer[32];
                    snprintf(distanceBuffer, sizeof(distanceBuffer), "[%.0fm]", esp.Distance);
                    distanceText = distanceBuffer;
                }
                else
                {
                    distanceText = "[???m]";
                }
            }

            if (nameText.empty() && distanceText.empty())
                continue;

            // Calculate text sizes
            ImVec2 nameSize = ImGui::CalcTextSize(nameText.c_str());
            ImVec2 distanceSize = ImGui::CalcTextSize(distanceText.c_str());

            // Validate sizes
            if (!std::isfinite(nameSize.x) || !std::isfinite(nameSize.y) ||
                !std::isfinite(distanceSize.x) || !std::isfinite(distanceSize.y))
                continue;

            float totalWidth = nameSize.x;
            if (!nameText.empty() && !distanceText.empty())
                totalWidth += ImGui::CalcTextSize(" ").x;
            totalWidth += distanceSize.x;

            float boxWidth = esp.Box.z - esp.Box.x;
            float boxHeight = esp.Box.w - esp.Box.y;

            if (boxWidth <= 0 || boxHeight <= 0)
                continue;

            ImVec2 textPos(
                (boxWidth - totalWidth) / 2.f + esp.Box.x,
                esp.Box.y - nameSize.y - 4.f
            );

            if (!std::isfinite(textPos.x) || !std::isfinite(textPos.y))
                continue;

            // Name
            if (!nameText.empty())
            {
                const ImU32 color = esp.IsCop ? nameCopColor : nameCivilianColor;

                pDrawList->AddText(ImVec2(textPos.x - 1, textPos.y - 1), IM_COL32(0, 0, 0, 255), nameText.c_str());

                pDrawList->AddText(textPos, color, nameText.c_str());
                textPos.x += nameSize.x;
            }

            // Space between name and distance
            if (!nameText.empty() && !distanceText.empty())
                textPos.x += ImGui::CalcTextSize(" ").x;

            // Distance
            if (!distanceText.empty())
            {
                const ImU32 color = esp.IsCop ? distanceCopColor : distanceCivilianColor;

                pDrawList->AddText(ImVec2(textPos.x - 1, textPos.y - 1), IM_COL32(0, 0, 0, 255), distanceText.c_str());

                pDrawList->AddText(textPos, color, distanceText.c_str());
            }
        }

        if(drawHealthBar)
        {
            const float barWidth = 4.0f;
            const float barHeight = esp.Box.w - esp.Box.y;

            if (barHeight > 0)
            {
                float healthPercent = esp.HealthMax > 0 ? esp.Health / esp.HealthMax : 0.0f;

                float healthBarHeight = barHeight * healthPercent;

                const ImU32 healthColor = esp.IsCop ? healthBarCopColor : healthBarCivilianColor;

                pDrawList->AddRectFilled(
                    ImVec2(esp.Box.x - barWidth - 3, esp.Box.w - barHeight),
                    ImVec2(esp.Box.x - 1, esp.Box.w),
                    IM_COL32(0, 0, 0, 255)
                );

                // Draw health bar
                pDrawList->AddRectFilled(
                    ImVec2(esp.Box.x - barWidth - 2, esp.Box.w - healthBarHeight),
                    ImVec2(esp.Box.x - 2, esp.Box.w),
                    healthColor
                );
            }
        }

        if(drawArmorBar)
        {
            const float barWidth = 4.0f;
            const float barHeight = esp.Box.w - esp.Box.y;

            if (barHeight > 0)
            {
                float armorPercent = esp.ArmorMax > 0 ? esp.Armor / esp.ArmorMax : 0.0f;

                float armorBarHeight = barHeight * armorPercent;

                pDrawList->AddRectFilled(
                    ImVec2(esp.Box.x - barWidth * 2 - 5, esp.Box.w - armorBarHeight),
                    ImVec2(esp.Box.x - barWidth - 3, esp.Box.w),
                    IM_COL32(0, 0, 0, 255)
                );

                // Draw armor bar
                pDrawList->AddRectFilled(
                    ImVec2(esp.Box.x - barWidth * 2 - 4, esp.Box.w - armorBarHeight),
                    ImVec2(esp.Box.x - barWidth - 4, esp.Box.w),
                    armorBarColor
                );
            }
        }

        // Yes it looks ugly, too bad. Dynamically obtaining & drawing the skeleton is a performance hit, manual is better.
        if(drawSkeleton) 
        {
            BoneCache& cache = m_BoneCache[esp.Mesh];
            ImU32 color = esp.IsCop ? skeletonCopColor : skeletonCivilianColor;

            if (!cache.Initialized)
            {
                BuildBoneCache(esp.Mesh, cache);
            }
            // Spine
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Head, cache.Neck, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Neck, cache.Spine3, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Spine3, cache.Hips, color);

            // Left arm
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Spine3, cache.LeftShoulder, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftShoulder, cache.LeftUpperArm, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftUpperArm, cache.LeftForeArm, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftForeArm, cache.LeftHand, color);

            // Right arm
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Spine3, cache.RightShoulder, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightShoulder, cache.RightUpperArm, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightUpperArm, cache.RightForeArm, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightForeArm, cache.RightHand, color);

            // Left leg
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Hips, cache.LeftUpperLeg, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftUpperLeg, cache.LeftLowerLeg, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftLowerLeg, cache.LeftFoot, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.LeftFoot, cache.LeftToe, color);

            // Right leg
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.Hips, cache.RightUpperLeg, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightUpperLeg, cache.RightLowerLeg, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightLowerLeg, cache.RightFoot, color);
            DrawBone(pDrawList, pPlayerController, esp.Mesh, cache.RightFoot, cache.RightToe, color);
        }

        if(drawHighlight) //Anyone know a good way to disable this shit ? Setting it false doesn't even disable it.
        {
            esp.Character->Multicast_SetMarked(true);
        }
    }
}

void Visuals::Run()
{
    // // ----- ITEMS & LOOT ESP -----
    // if (bShowItems || bShowMoney) {
    //     UC::TArray<SDK::ULevel*> vecLevels = pGWorld->Levels;
    //     for (SDK::ULevel* pLevel : vecLevels) {
    //         if (!pLevel || !pLevel->Actors)
    //             continue;

    //         for (SDK::AActor* pActor : pLevel->Actors) {
    //             if (!pActor)
    //                 continue;

    //             std::string className = pActor->Class->Name.ToString();
                
    //             // Check for money/loot
    //             bool bIsMoney = className.find("BP_BaseValuableBag") != std::string::npos;
                
    //             // Check for key items
    //             bool bIsKeyItem = false;
    //             std::string sItemName = "";
                
    //             if (className.find("BP_Keycard") != std::string::npos ||
    //                 className.find("BP_RFID") != std::string::npos ||
    //                 className.find("BP_QRPhone") != std::string::npos ||
    //                 className.find("BP_FOR_USBDrive") != std::string::npos) {
    //                 bIsKeyItem = true;
                    
    //                 if (className.find("Keycard") != std::string::npos)
    //                     sItemName = "Keycard";
    //                 else if (className.find("RFID") != std::string::npos)
    //                     sItemName = "RFID Tag";
    //                 else if (className.find("QRPhone") != std::string::npos)
    //                     sItemName = "Phone";
    //                 else if (className.find("USBDrive") != std::string::npos)
    //                     sItemName = "USB Drive";
    //                 else
    //                     sItemName = "Item";
    //             }

    //             if (!bIsMoney && !bIsKeyItem)
    //                 continue;

    //             // Apply filters
    //             if (bIsMoney && !bShowMoney)
    //                 continue;
    //             if (!bIsMoney && !bShowItems)
    //                 continue;

    //             SDK::FVector2D vec2ScreenLocation;
    //             if (!pPlayerController->ProjectWorldLocationToScreen(
    //                 pActor->K2_GetActorLocation(), &vec2ScreenLocation, false))
    //                 continue;

    //             // Draw item
    //             if (bIsMoney) {
    //                 pDrawList->AddText(
    //                     ImVec2(vec2ScreenLocation.X - 10, vec2ScreenLocation.Y - 10),
    //                     IM_COL32(255, 215, 0, 255),
    //                     "$"
    //                 );
    //             } else if (bIsKeyItem && m_pKeyItem->GetValue()) {
    //                 ImU32 colorU32 = IM_COL32(
    //                     (int)(m_pKeyItemColor->GetValue().x * 255),
    //                     (int)(m_pKeyItemColor->GetValue().y * 255),
    //                     (int)(m_pKeyItemColor->GetValue().z * 255),
    //                     (int)(m_pKeyItemColor->GetValue().w * 255)
    //                 );
                    
    //                 // Draw glow
    //                 for (int i = 3; i > 0; i--) {
    //                     pDrawList->AddCircle(
    //                         ImVec2(vec2ScreenLocation.X, vec2ScreenLocation.Y),
    //                         15.0f + (i * 3.0f),
    //                         IM_COL32(
    //                             (int)(m_pKeyItemColor->GetValue().x * 255),
    //                             (int)(m_pKeyItemColor->GetValue().y * 255),
    //                             (int)(m_pKeyItemColor->GetValue().z * 255),
    //                             (int)(m_pKeyItemColor->GetValue().w * 50 * i / 3)
    //                         ),
    //                         12,
    //                         1.0f
    //                     );
    //                 }
                    
    //                 pDrawList->AddText(
    //                     ImVec2(vec2ScreenLocation.X - 20, vec2ScreenLocation.Y + 20),
    //                     colorU32,
    //                     sItemName.c_str()
    //                 );
    //             }
    //         }
    //     }
    // }
}