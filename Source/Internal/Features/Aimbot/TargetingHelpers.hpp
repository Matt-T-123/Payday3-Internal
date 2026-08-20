#pragma once
#include "pch.h"
#include "../Visuals/Types.hpp"

namespace TargetingHelpers
{
	struct TargetInfo
	{
		SDK::ASBZCharacter* Character = nullptr;
		SDK::USkeletalMeshComponent* Mesh = nullptr;
		SDK::FVector AimWorldLocation;
		SDK::FVector2D AimScreenLocation;
		float ScreenDistance = 0.0f;
		float WorldDistance = 0.0f;
		bool IsCop = false;
		bool IsCivilian = false;
	};

	std::optional<TargetInfo> SelectBestTarget(SDK::UWorld* pWorld, SDK::APlayerController* pPlayerController,
		bool Cops, bool Civs, float maxScreenDistance, bool visibleCheck, const wchar_t* targetPart);
}
