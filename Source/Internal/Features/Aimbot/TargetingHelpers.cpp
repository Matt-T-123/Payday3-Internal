#include "pch.h"
#include "../Shared/Helpers.hpp"
#include "TargetingHelpers.hpp"

namespace TargetingHelpers
{
	std::optional<TargetInfo> SelectBestTarget(SDK::UWorld* pWorld, SDK::APlayerController* pPlayerController,
		bool Cops, bool Civs, float maxScreenDistance, bool visibleCheck, const wchar_t* targetPart)
	{
		if (!pWorld || !pPlayerController || !pPlayerController->PlayerCameraManager)
			return {};

		SDK::USBZWorldRuntime* pWorldRuntime = SDK::USBZWorldRuntime::Get(pWorld);
		if (!pWorldRuntime || !pWorldRuntime->AllPawns)
			return {};

		int viewportWidth = 0;
		int viewportHeight = 0;
		pPlayerController->GetViewportSize(&viewportWidth, &viewportHeight);
		if (viewportWidth <= 0 || viewportHeight <= 0)
			return {};

		const float centerX = viewportWidth * 0.5f;
		const float centerY = viewportHeight * 0.5f;

		auto pLocalPlayer = pPlayerController->AcknowledgedPawn;
		UC::TArray<SDK::UObject*>& actors = pWorldRuntime->AllPawns->Objects;
		std::unordered_map<SDK::UClass*, Types::EnemyType> classCache;

		std::optional<TargetInfo> bestTarget;
		float bestDistance = maxScreenDistance;

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

			if (visibleCheck && !pPlayerController->LineOfSightTo(pCharacter, pPlayerController->PlayerCameraManager->GetCameraLocation(), false))
				continue;

			Types::EnemyType type = Helpers::ResolveEnemyType(classCache, pActor);
			const Types::EnemyInfo& info = Types::g_EnemyInfo[static_cast<size_t>(type)];
			const bool isCop = info.Category == Types::EnemyCategory::Cop;
			const bool isCivilian = info.Category == Types::EnemyCategory::Civilian;

			if (!isCop && !isCivilian)
				continue;
			if (isCop && !Cops)
				continue;
			if (isCivilian && !Civs)
				continue;

			SDK::FVector aimWorldLocation = pCharacter->Mesh->GetSocketLocation(SDK::UKismetStringLibrary::Conv_StringToName(targetPart));
			SDK::FVector2D aimScreenLocation;
			if (!pPlayerController->ProjectWorldLocationToScreen(aimWorldLocation, &aimScreenLocation, false))
				continue;

			const float screenDistance = std::hypot(aimScreenLocation.X - centerX, aimScreenLocation.Y - centerY);
			if (screenDistance > bestDistance)
				continue;

			TargetInfo candidate;
			candidate.Character = pCharacter;
			candidate.Mesh = pCharacter->Mesh;
			candidate.AimWorldLocation = aimWorldLocation;
			candidate.AimScreenLocation = aimScreenLocation;
			candidate.ScreenDistance = screenDistance;
			candidate.WorldDistance = (pActor->K2_GetActorLocation() - pPlayerController->PlayerCameraManager->GetCameraLocation()).Magnitude();
			candidate.IsCop = isCop;
			candidate.IsCivilian = isCivilian;

			bestDistance = screenDistance;
			bestTarget = candidate;
		}

		return bestTarget;
	}
}
