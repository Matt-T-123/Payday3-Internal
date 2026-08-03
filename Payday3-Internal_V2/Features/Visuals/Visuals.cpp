#include "pch.h"
#include "Visuals.hpp"
#include "VisualsHelpers.hpp"
#include <vector>
#include <string>

#undef min
#undef max

namespace
{
    std::string ToLowerCopy(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(std::tolower(c));
            });
        return value;
    }

    VisualsTypes::EnemyType ResolveEnemyType(std::unordered_map<SDK::UClass*, VisualsTypes::EnemyType>& cache, SDK::AActor* actor)
    {
        auto it = cache.find(actor->Class);
        if (it != cache.end())
            return it->second;

        VisualsTypes::EnemyType type = VisualsTypes::EnemyType::None;
        std::string className = ToLowerCopy(actor->Class->Name.ToString());

        for (const auto& entry : VisualsTypes::g_EnemyLookup)
        {
            if (className.find(entry.Keyword) != std::string::npos)
            {
                type = entry.Type;
                break;
            }
        }

        cache.try_emplace(actor->Class, type);
        return type;
    }

    VisualsTypes::ItemType ResolveItemType(std::unordered_map<SDK::UClass*, VisualsTypes::ItemType>& cache, SDK::AActor* actor)
    {
        auto it = cache.find(actor->Class);
        if (it != cache.end())
            return it->second;

        VisualsTypes::ItemType type = VisualsTypes::ItemType::None;

        for (SDK::UStruct* pStruct = actor->Class; pStruct; pStruct = static_cast<SDK::UStruct*>(pStruct->SuperStruct))
        {
            std::string name = ToLowerCopy(pStruct->Name.ToString());

            for (const auto& entry : VisualsTypes::g_ItemLookup)
            {
                if (name.find(entry.Keyword) != std::string::npos)
                {
                    type = entry.Type;
                    break;
                }
            }

            if (type != VisualsTypes::ItemType::None)
                break;
        }

        cache.emplace(actor->Class, type);
        return type;
    }

    std::string FormatEntityName(const std::string& name)
    {
        if (name.empty())
            return "Unknown";

        const char* rawName = name.c_str();
        size_t length = strlen(rawName);
        if (length == 0 || length >= 256)
            return "Unknown";

        for (size_t i = 0; i < length; ++i)
        {
            if (rawName[i] < 32 || rawName[i] > 126)
                return "Invalid";
        }

        return name;
    }

    std::string FormatDistanceText(float distance)
    {
        if (distance >= 0 && distance < 10000)
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "[%.0fm]", distance);
            return buffer;
        }

        return "[???m]";
    }

    bool TryReadPawnStats(SDK::ASBZCharacter* character, float& health, float& healthMax, float& armor, float& armorMax)
    {
        auto* abilitySystem = character->AbilitySystem;
        if (!abilitySystem)
            return false;

        UC::TArray<SDK::UAttributeSet*>* spawnedAttrs = (UC::TArray<SDK::UAttributeSet*>*)((uintptr_t)abilitySystem + 0x0150);
        if (!spawnedAttrs)
            return false;

        for (int i = 0; i < spawnedAttrs->Num(); ++i)
        {
            auto* attrSet = (*spawnedAttrs)[i];
            if (attrSet && attrSet->IsA(SDK::USBZPawnAttributeSet::StaticClass()))
            {
                auto* pawnAttrs = static_cast<SDK::USBZPawnAttributeSet*>(attrSet);
                health = pawnAttrs->Health.CurrentValue;
                healthMax = pawnAttrs->HealthMax.CurrentValue;
                armor = pawnAttrs->Armor.CurrentValue;
                armorMax = pawnAttrs->ArmorMax.CurrentValue;
                return true;
            }
        }

        return false;
    }

}

VisualsTypes::EnemyType Visuals::GetEnemyType(SDK::AActor* actor)
{
    return ResolveEnemyType(m_ClassCache, actor);
}

VisualsTypes::ItemType Visuals::GetItemType(SDK::AActor* actor)
{
    return ResolveItemType(m_ItemTypeCache, actor);
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

        { "VISUALS_ITEM"Hashed, "Items" },
        { "VISUALS_ITEM_CASH_COLOR"Hashed, "Cash Color" },
        { "VISUALS_ITEM_DEPOSITBOX_COLOR"Hashed, "Deposit Box Color" },
        { "VISUALS_ITEM_KEYCARD_COLOR"Hashed, "Keycard Color" },

        { "VISUALS_FILTERS"Hashed, "" },
        { "VISUALS_ITEM_FILTERS"Hashed, "" }
    });

    return true;
}

void Visuals::UpdateMenuVisibility()
{
    const bool copSelected = m_pFilters->IsSelected(0);
    const bool civilianSelected = m_pFilters->IsSelected(1);

    const bool cashSelected = m_pItemFilters->IsSelected(0);
    const bool depositBoxSelected = m_pItemFilters->IsSelected(1);
    const bool keycardSelected = m_pItemFilters->IsSelected(2);

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
    m_pArmorBarColor->SetVisible(m_pArmorBar->GetValue() && copSelected);

    // Skeleton
    m_pSkeletonCopColor->SetVisible(m_pSkeleton->GetValue() && copSelected);
    m_pSkeletonCivilianColor->SetVisible(m_pSkeleton->GetValue() && civilianSelected);

    // Key Items
    m_pItemCashColor->SetVisible(m_pItem->GetValue() && cashSelected);
    m_pItemDepositBoxColor->SetVisible(m_pItem->GetValue() && depositBoxSelected);
    m_pItemKeycardColor->SetVisible(m_pItem->GetValue() && keycardSelected);
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

        m_pTab1Left->AddElement(m_pItem.get());
        m_pTab1Right->AddElement(m_pItemCashColor.get());
        m_pTab1Right->AddElement(m_pItemDepositBoxColor.get());
        m_pTab1Right->AddElement(m_pItemKeycardColor.get());
        m_pItemCashColor->SetValue(ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
        m_pItemDepositBoxColor->SetValue(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        m_pItemKeycardColor->SetValue(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));

        // Filters
        m_pFilters->AddOption("COP", [](bool bEnabled) {
            //Utils::LogDebug("Cops selected");
        });
        m_pFilters->AddOption("CIVILIAN", [](bool bEnabled) {
            //Utils::LogDebug("Civilians selected");
        });

        // Item Filters
        m_pItemFilters->AddOption("CASH", [](bool bEnabled) {
            //Utils::LogDebug("CASH selected");
        });
        m_pItemFilters->AddOption("DEPOSIT BOX", [](bool bEnabled) {
            //Utils::LogDebug("DEPOSIT BOX selected");
        });
        m_pItemFilters->AddOption("KEYCARDS", [](bool bEnabled) {
            //Utils::LogDebug("KEYCARDS selected");
        });

        m_pTab1Bottom->AddElement(m_pFilters.get());
        m_pTab1Bottom->AddElement(m_pItemFilters.get());

        m_pTab1Group->AddElement(m_pTab1Left.get());
        m_pTab1Group->AddElement(m_pTab1Right.get());
        m_pTab1Group->AddElement(m_pTab1Bottom.get());
        m_pTab1Page->AddElement(m_pTab1Group.get());

        Framework::menu->GetChild("HEADER_GROUP")->GetChild("BODY")->AddElement(m_pTab1Page.get());
    });

    UpdateMenuVisibility();
}

//Something you may ask is "why is it all done in render() and not in both render() and run() ?" The answer is simple: different timings, render() runs every frame, run() runs 10 times per second. -
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
    const bool drawItems = m_pItem->GetValue();

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

    const ImU32 itemCashColor = ImGui::ColorConvertFloat4ToU32(m_pItemCashColor->GetValue());
    const ImU32 itemDepositBoxColor = ImGui::ColorConvertFloat4ToU32(m_pItemDepositBoxColor->GetValue());
    const ImU32 itemKeycardColor = ImGui::ColorConvertFloat4ToU32(m_pItemKeycardColor->GetValue());

    // Clear or else it'll look like you got concussed with a cod nade. Trust me...
    m_vESPData.clear();
    m_vESPData.reserve(256); // Reserving space for 256 objects to avoid frequent reallocations, but may need to increase in the future if more objects are present than expected.

    m_vItemData.clear();
    //m_vItemData.reserve(256);

    if (!drawBox && !drawName && !drawDistance && !drawHealthBar && !drawArmorBar && !drawSkeleton && !drawHighlight && !drawItems)
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

    // Get key item filter states
    bool bShowCash = m_pItemFilters->IsSelected(0);
    bool bShowDepositBox = m_pItemFilters->IsSelected(1);
    bool bShowKeycards = m_pItemFilters->IsSelected(2);

    UC::TArray<SDK::UObject*>& actors = pWorldRuntime->AllPawns->Objects;
    UC::TArray<SDK::ULevel*> vecLevels = pGWorld->Levels;
    
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

        VisualsTypes::EnemyType type = GetEnemyType(pActor);

        const VisualsTypes::EnemyInfo& info = VisualsTypes::g_EnemyInfo[static_cast<size_t>(type)];

        bool bIsCop       = info.Category == VisualsTypes::EnemyCategory::Cop;
        bool bIsCivilian  = info.Category == VisualsTypes::EnemyCategory::Civilian;

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
        auto optScreenBox = VisualsHelpers::CalculateScreenBoxForCharacter(pMesh, pPlayerController, pActor);
        if (!optScreenBox.has_value())
            continue;

        ImVec4 vec4ScreenBox = optScreenBox.value();

        // Calculate distance
        float flDistance = (pActor->K2_GetActorLocation() - vecCameraLocation).Magnitude() / 100.0f;

        float flHealth = 0.0f;
        float flHealthMax = 0.0f;
        float flArmor = 0.0f;
        float flArmorMax = 0.0f;
        TryReadPawnStats(pCharacter, flHealth, flHealthMax, flArmor, flArmorMax);

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

    for (SDK::ULevel *pLevel : vecLevels)
    {
        if (!pLevel || !pLevel->Actors)
            continue;

        for (SDK::AActor* pActor : pLevel->Actors)
        {
            if (!pActor)
                continue;

            VisualsTypes::ItemType type = GetItemType(pActor); //Yes it's laggy at the start, it's supposed to be that way. 
                                                 //Caching as things are discovered gives lag spikes so instead I'm just caching everything at the start to have one big lag spike instead of multiple ones.

            if (type == VisualsTypes::ItemType::None)
                continue;

            if (!((type == VisualsTypes::ItemType::Cash && bShowCash) || (type == VisualsTypes::ItemType::DepositBox && bShowDepositBox) || (type == VisualsTypes::ItemType::Keycard && bShowKeycards)))
                continue;

            SDK::FVector ActorLocation = pActor->K2_GetActorLocation();

            SDK::FVector2D ScreenLocation;

            m_vItemData.push_back({
                ScreenLocation,
                ActorLocation,
                std::string(pActor->GetName()),
                type
            });
        }
    }

    if (m_vESPData.empty() && m_vItemData.empty())
        return;

    ImDrawList* pDrawList = ImGui::GetBackgroundDrawList();
    if (!pDrawList)
        return;

    for (const auto& entity : m_vESPData)
    {
        // Bounding box
        if (drawBox)
        {
            const ImU32 color = entity.IsCop ? boxCopColor : boxCivilianColor;

            pDrawList->AddRect(
                ImVec2(entity.Box.x - 1.0f, entity.Box.y - 1.0f),
                ImVec2(entity.Box.z + 1.0f, entity.Box.w + 1.0f),
                IM_COL32(0, 0, 0, 255),
                0.0f,
                0,
                1.6f
            );

            pDrawList->AddRect(
                ImVec2(entity.Box.x, entity.Box.y),
                ImVec2(entity.Box.z, entity.Box.w),
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
                const char* name = entity.Name.c_str();
                if (name && name[0])
                {
                    nameText = FormatEntityName(entity.Name);
                }
                else
                {
                    nameText = FormatEntityName(entity.Name);
                }
            }

            if (drawDistance)
            {
                distanceText = FormatDistanceText(entity.Distance);
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

            float boxWidth = entity.Box.z - entity.Box.x;
            float boxHeight = entity.Box.w - entity.Box.y;

            if (boxWidth <= 0 || boxHeight <= 0)
                continue;

            ImVec2 textPos(
                (boxWidth - totalWidth) / 2.f + entity.Box.x,
                entity.Box.y - nameSize.y - 4.f
            );

            if (!std::isfinite(textPos.x) || !std::isfinite(textPos.y))
                continue;

            // Name
            if (!nameText.empty())
            {
                const ImU32 color = entity.IsCop ? nameCopColor : nameCivilianColor;

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
                const ImU32 color = entity.IsCop ? distanceCopColor : distanceCivilianColor;

                pDrawList->AddText(ImVec2(textPos.x - 1, textPos.y - 1), IM_COL32(0, 0, 0, 255), distanceText.c_str());

                pDrawList->AddText(textPos, color, distanceText.c_str());
            }
        }

        if(drawHealthBar)
        {
            const float barWidth = 4.0f;
            const float barHeight = entity.Box.w - entity.Box.y;

            if (barHeight > 0)
            {
                float healthPercent = entity.HealthMax > 0 ? entity.Health / entity.HealthMax : 0.0f;

                float healthBarHeight = barHeight * healthPercent;

                const ImU32 healthColor = entity.IsCop ? healthBarCopColor : healthBarCivilianColor;

                pDrawList->AddRectFilled(
                    ImVec2(entity.Box.x - barWidth - 3, entity.Box.w - barHeight),
                    ImVec2(entity.Box.x - 1, entity.Box.w),
                    IM_COL32(0, 0, 0, 255)
                );

                // Draw health bar
                pDrawList->AddRectFilled(
                    ImVec2(entity.Box.x - barWidth - 2, entity.Box.w - healthBarHeight),
                    ImVec2(entity.Box.x - 2, entity.Box.w),
                    healthColor
                );
            }
        }

        if(drawArmorBar)
        {
            const float barWidth = 4.0f;
            const float barHeight = entity.Box.w - entity.Box.y;

            if (barHeight > 0)
            {
                float armorPercent = entity.ArmorMax > 0 ? entity.Armor / entity.ArmorMax : 0.0f;

                float armorBarHeight = barHeight * armorPercent;

                pDrawList->AddRectFilled(
                    ImVec2(entity.Box.x - barWidth * 2 - 5, entity.Box.w - armorBarHeight),
                    ImVec2(entity.Box.x - barWidth - 3, entity.Box.w),
                    IM_COL32(0, 0, 0, 255)
                );

                // Draw armor bar
                pDrawList->AddRectFilled(
                    ImVec2(entity.Box.x - barWidth * 2 - 4, entity.Box.w - armorBarHeight),
                    ImVec2(entity.Box.x - barWidth - 4, entity.Box.w),
                    armorBarColor
                );
            }
        }

        // Yes it looks ugly, too bad. Dynamically obtaining & drawing the skeleton is a performance hit, manual is better.
        if(drawSkeleton) 
        {
            VisualsTypes::BoneCache& cache = m_BoneCache[entity.Mesh];
            ImU32 color = entity.IsCop ? skeletonCopColor : skeletonCivilianColor;

            if (!cache.Initialized)
            {
                VisualsHelpers::BuildBoneCache(entity.Mesh, cache);
            }
            // Spine
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Head, cache.Neck, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Neck, cache.Spine3, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Spine3, cache.Hips, color);

            // Left arm
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Spine3, cache.LeftShoulder, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftShoulder, cache.LeftUpperArm, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftUpperArm, cache.LeftForeArm, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftForeArm, cache.LeftHand, color);

            // Right arm
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Spine3, cache.RightShoulder, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightShoulder, cache.RightUpperArm, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightUpperArm, cache.RightForeArm, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightForeArm, cache.RightHand, color);

            // Left leg
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Hips, cache.LeftUpperLeg, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftUpperLeg, cache.LeftLowerLeg, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftLowerLeg, cache.LeftFoot, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.LeftFoot, cache.LeftToe, color);

            // Right leg
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.Hips, cache.RightUpperLeg, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightUpperLeg, cache.RightLowerLeg, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightLowerLeg, cache.RightFoot, color);
            VisualsHelpers::DrawBone(pDrawList, pPlayerController, entity.Mesh, cache.RightFoot, cache.RightToe, color);
        }

        if(drawHighlight) //Anyone know a good way to disable this shit ? Setting it false doesn't even disable it.
        {
            entity.Character->Multicast_SetMarked(true);
        }
    }

    for (auto& item : m_vItemData)
    {
        if (drawItems)
        {
            if (!pPlayerController->ProjectWorldLocationToScreen(item.WorldLocation, &item.ScreenLocation, false))
                    continue;
                    
            ImVec2 vecTextSize = ImGui::CalcTextSize(item.Name.c_str());

            ImU32 itemColor = VisualsTypes::ItemType::Cash == item.Type ? itemCashColor :
                              VisualsTypes::ItemType::DepositBox == item.Type ? itemDepositBoxColor :
                              VisualsTypes::ItemType::Keycard == item.Type ? itemKeycardColor : IM_COL32(255, 255, 255, 255);

            pDrawList->AddText(
                ImVec2(item.ScreenLocation.X - vecTextSize.x / 2 - 1, item.ScreenLocation.Y - vecTextSize.y / 2 - 1),
                IM_COL32(0, 0, 0, 255),
                item.Name.c_str()
            );

            pDrawList->AddText(
                ImVec2(item.ScreenLocation.X - vecTextSize.x / 2, item.ScreenLocation.Y - vecTextSize.y / 2),
                itemColor,
                item.Name.c_str()
            );
        }
    }
}

void Visuals::Run()
{
}