#include "pch.h"
#include "Visuals/Visuals.hpp"
#include "Helpers.hpp"
#include "Visuals/VisualsHelpers.hpp"

namespace
{
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

Types::EnemyType Visuals::GetEnemyType(SDK::AActor* actor)
{
    return Helpers::ResolveEnemyType(m_ClassCache, actor);
}

Types::ItemType Visuals::GetItemType(SDK::AActor* actor)
{
    return Helpers::ResolveItemType(m_ItemTypeCache, actor);
}

void Visuals::CollectFrameData(SDK::UWorld* pGWorld, SDK::APlayerController* pPlayerController, SDK::APlayerCameraManager* pCameraManager, SDK::AActor* pLocalPlayer, const FrameSettings& settings)
{
	m_vESPData.clear();
	m_vESPData.reserve(256);
	m_vItemData.clear();

	const bool needsEnemyBoxData = settings.DrawBox || settings.DrawName || settings.DrawDistance || settings.DrawHealthBar || settings.DrawArmorBar;
	const bool needsEnemyStats = settings.DrawHealthBar || settings.DrawArmorBar;
	const bool needsItems = settings.DrawItems;

	SDK::FVector vecCameraLocation = pCameraManager->GetCameraLocation();
	SDK::USBZWorldRuntime* pWorldRuntime = SDK::USBZWorldRuntime::Get(pGWorld);
	if (!pWorldRuntime)
		return;

	UC::TArray<SDK::UObject*>& actors = pWorldRuntime->AllPawns->Objects;
	UC::TArray<SDK::ULevel*> vecLevels = pGWorld->Levels;

	for (int i = 0; i < actors.Num(); ++i)
	{
		if (!actors.IsValidIndex(i))
			break;

		auto pActor = reinterpret_cast<SDK::AActor*>(actors[i]);
		if (!pActor || pActor == pLocalPlayer)
			continue;

		auto pCharacter = reinterpret_cast<SDK::ASBZCharacter*>(pActor);
		if (!pCharacter || !pCharacter->bIsAlive || !pCharacter->Mesh)
			continue;

		Types::EnemyType type = GetEnemyType(pActor);
		const Types::EnemyInfo& info = Types::g_EnemyInfo[static_cast<size_t>(type)];
		const bool bIsCop = info.Category == Types::EnemyCategory::Cop;
		const bool bIsCivilian = info.Category == Types::EnemyCategory::Civilian;

		if (!bIsCop && !bIsCivilian)
			continue;
		if (bIsCop && !settings.ShowCops)
			continue;
		if (bIsCivilian && !settings.ShowCivilians)
			continue;

		ImVec4 screenBox{};
		if (needsEnemyBoxData)
		{
			auto optScreenBox = VisualsHelpers::CalculateScreenBoxForCharacter(pCharacter->Mesh, pPlayerController, pActor);
			if (!optScreenBox.has_value())
				continue;

			screenBox = optScreenBox.value();
		}

		float flHealth = 0.0f;
		float flHealthMax = 0.0f;
		float flArmor = 0.0f;
		float flArmorMax = 0.0f;
		if (needsEnemyStats)
			TryReadPawnStats(pCharacter, flHealth, flHealthMax, flArmor, flArmorMax);

		float flDistance = 0.0f;
		if (settings.DrawDistance)
			flDistance = (pActor->K2_GetActorLocation() - vecCameraLocation).Magnitude() / 100.0f;

		m_vESPData.push_back({
			screenBox,
			std::string(info.Name),
			pCharacter->Mesh,
			pCharacter,
			flHealth,
			flHealthMax,
			flArmor,
			flArmorMax,
			flDistance,
			bIsCop,
			bIsCivilian
		});

		if (settings.DrawSkeleton)
		{
			auto [cacheIt, inserted] = m_BoneCache.try_emplace(pCharacter->Mesh);
			if (!cacheIt->second.Initialized)
				VisualsHelpers::BuildBoneCache(pCharacter->Mesh, cacheIt->second);
		}
	}

	if (!needsItems)
		return;

	for (SDK::ULevel* pLevel : vecLevels)
	{
		if (!pLevel || !pLevel->Actors)
			continue;

		for (SDK::AActor* pActor : pLevel->Actors)
		{
			if (!pActor)
				continue;

			Types::ItemType type = GetItemType(pActor);
			if (type == Types::ItemType::None)
				continue;

			if (!((type == Types::ItemType::Cash && settings.ShowCash) || (type == Types::ItemType::DepositBox && settings.ShowDepositBox) || (type == Types::ItemType::Keycard && settings.ShowKeycards)))
				continue;

			m_vItemData.push_back({
				SDK::FVector2D{},
				pActor->K2_GetActorLocation(),
				std::string(pActor->GetName()),
				type
			});
		}
	}
}