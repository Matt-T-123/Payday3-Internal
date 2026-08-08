#include "pch.h"
#include "Player.hpp"
#include <vector>
#include <algorithm>

bool Player::SetupMenu()
{
	Localization::AddToLocale("ENG", std::initializer_list<std::pair<size_t, std::string>>{
		{ "PLAYER_BUTTON"Hashed, "Player" },
		{ "PLAYER_TAB1"Hashed, "Local Player" },
		{ "PLAYER_TAB2"Hashed, "Players" },
		{ "PLAYER_TAB3"Hashed, "Weapon Mods" },

		{ "PLAYER_TAB1_LEFT"Hashed, "General" },
		{ "PLAYER_TAB1_RIGHT"Hashed, "Options" },

		{ "PLAYER_TAB2_LEFT"Hashed, "Details" },

		{ "PLAYER_TAB3_LEFT"Hashed, "Mods" },
		{ "PLAYER_TAB3_RIGHT"Hashed, "Options" },

		//Tab 1
		{ "PLAYER_GODMODE_TYPE"Hashed, "Godmode Type" },
		{ "PLAYER_GODMODE"Hashed, "Godmode" },
		{ "PLAYER_INF_STAMINA"Hashed, "Infinite Stamina" },
		{ "PLAYER_INSTA_MELEE"Hashed, "Instant Melee" },
		{ "PLAYER_NO_SCREENSHAKE"Hashed, "No Screenshake" },
		{ "PLAYER_NO_FALLDAMAGE"Hashed, "No Fall Damage" },
		{ "PLAYER_NO_DETECTION"Hashed, "No Detection" },

		//Tab2
		{ "PLAYER_TABLE"Hashed, "Players" },
		{ "PLAYER_TABLE_ROW1"Hashed, "Row 1" },

		//Tab3
		{ "PLAYER_INSTA_RELOAD"Hashed, "Instant Reload" },
		{ "PLAYER_INF_AMMO"Hashed, "Infinite Ammo" },
		{ "PLAYER_NO_RECOIL"Hashed, "No Recoil" },
		{ "PLAYER_NO_SPREAD"Hashed, "No Spread" },
		{ "PLAYER_FIRE_RATE_SLIDER"Hashed, "Fire Rate" },
		{ "PLAYER_FIRE_RATE"Hashed, "Fire Rate" }
	});

	return true;
}

bool Player::Setup()
{
	m_pNoRecoil->SetOnValueChangedCallback([this](const bool, const bool bNewValue) {
		if (!bNewValue)
		{
			noRecoil(false);
		}
	});

	m_pNoSpread->SetOnValueChangedCallback([this](const bool, const bool bNewValue) {
		if (!bNewValue)
		{
			noSpread(false);
		}
	});

	m_pFireRate->SetOnValueChangedCallback([this](const bool, const bool bNewValue) {
		if (!bNewValue)
		{
			fireRate(false);
		}
	});

	return true;
}
void Player::HandleMenu()
{
	static std::once_flag onceflag;

	std::call_once(onceflag, [this]() {
		auto pHeaderGroup = static_cast<HeaderGroup*>(Framework::menu->GetChild("HEADER_GROUP"));
		if (pHeaderGroup)
			pHeaderGroup->AddHeaders(Player::s_iPlayerPageId, { "PLAYER_TAB1"Hashed, "PLAYER_TAB2"Hashed, "PLAYER_TAB3"Hashed });

		m_pTab1Left->SetCallback([]() {
			return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() - 20.0f));
		});
		m_pTab1Right->SetCallback([]() {
			return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() - 20.0f));
		});
		m_pTab2Left->SetCallback([]() {
			return ImVec2((ImGui::GetWindowWidth() - 20.0f), (ImGui::GetWindowHeight() - 20.0f));
		});
		m_pTab3Left->SetCallback([]() {
			return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() - 20.0f));
		});
		m_pTab3Right->SetCallback([]() {
			return ImVec2((ImGui::GetWindowWidth() - 10.0f - 10.0f * 2) / 2, (ImGui::GetWindowHeight() - 20.0f));
		});

		/////////////////Tab 1////////////////////

		// Godmode Toggle
		m_pTab1Left->AddElement(m_pGodMode.get());

		// Infinite Stamina Toggle
		m_pTab1Left->AddElement(m_pInfStamina.get());

		// Instant Melee Toggle
		m_pTab1Left->AddElement(m_pInstaMelee.get());
		m_pInstaMelee->SetOnValueChangedCallback([this](const bool, const bool bNewValue) {
			InstantMelee(bNewValue);
		});

		// No Screenshake Toggle
		m_pTab1Left->AddElement(m_pNoScreenshake.get());

		// No Fall Damage Toggle
		m_pTab1Left->AddElement(m_pNoFallDamage.get());

		// No Detection Toggle
		m_pTab1Left->AddElement(m_pNoDetection.get());

		// Godmode Type
		m_pGodModeType->AddOption("Set");
		m_pGodModeType->AddOption("Block");
		m_pTab1Right->AddElement(m_pGodModeType.get());

		m_pTab1Group->AddElement(m_pTab1Left.get());
		m_pTab1Group->AddElement(m_pTab1Right.get());

		////////////////Tab 2///////////////////

		//Add a ?x? table then populate it with player(s) info
		//Need to figure out then best way to interact with the players in the table, maybe a left click player color highlight and then select an action from below the table ?
		//Edit: Implemented tables, just need to do the populate it with player info and then work on interacting with table rows and selecting actions for the selected player(s).
		//Edit2: I'll have to make the tables look better first befoore populating cus they look a lil ugly rn
		m_pPlayerRow1->AddElement(m_pPlayerName.get());
		m_pPlayerRow1->AddElement(m_pPlayerHealth.get());
		m_pPlayerTable->AddElement(m_pPlayerRow1.get());
		
		m_pTab2Left->AddElement(m_pPlayerTable.get());

		m_pTab2Group->AddElement(m_pTab2Left.get());

		////////////////Tab 3///////////////////

		m_pTab3Left->AddElement(m_pInstaReload.get());
		m_pTab3Left->AddElement(m_pInfAmmo.get());
		m_pTab3Left->AddElement(m_pNoRecoil.get());
		m_pTab3Left->AddElement(m_pNoSpread.get());
		m_pTab3Left->AddElement(m_pFireRate.get());
		m_pTab3Right->AddElement(m_pFireRateSlider.get());

		m_pTab3Group->AddElement(m_pTab3Left.get());
		m_pTab3Group->AddElement(m_pTab3Right.get());

		m_pTab1Page->AddElement(m_pTab1Group.get());
		m_pTab2Page->AddElement(m_pTab2Group.get());
		m_pTab3Page->AddElement(m_pTab3Group.get());

		Framework::menu->GetChild("HEADER_GROUP")->GetChild("BODY")->AddElement(m_pTab1Page.get());
		Framework::menu->GetChild("HEADER_GROUP")->GetChild("BODY")->AddElement(m_pTab2Page.get());
		Framework::menu->GetChild("HEADER_GROUP")->GetChild("BODY")->AddElement(m_pTab3Page.get());
	});
}

void Player::blockDamage(bool bEnabled)
{
	auto* localChar = Unreal::GetLocalCharacter();
	if (!localChar)
	{
		m_bBlockDamageApplied = false;
		return;
	}

	if (bEnabled) //I'm not kidding I had to design the method this way because bCanBeDamaged wasn't disabling for some reason. Digusting but it works.
	{
		if (!m_bBlockDamageApplied)
		{
			m_bOriginalCanBeDamaged = localChar->bCanBeDamaged;
			m_bBlockDamageApplied = false;
		}

		localChar->bCanBeDamaged = false;
		m_bBlockDamageApplied = true;
	}
	else if (m_bBlockDamageApplied)
	{
		localChar->bCanBeDamaged = m_bOriginalCanBeDamaged;
		m_bBlockDamageApplied = false;
	}
}

void Player::InstantMelee(bool bEnabled)
{
	if (auto* localChar = Unreal::GetLocalCharacter())
	{

		auto* montage = localChar->CurrentMeleeMontage;
		if (!montage)
		{
			m_pLastMeleeMontage = nullptr;
			m_bInstantMeleeApplied = false;
			return;
		}

		if (bEnabled)
		{
			if (montage != m_pLastMeleeMontage)
			{
				m_pLastMeleeMontage = montage;
				m_flOriginalMeleeRate = montage->RateScale;
				m_bInstantMeleeApplied = false;
			}

			if (!m_bInstantMeleeApplied || montage->RateScale != 10000.0f)
			{
				montage->RateScale = 10000.0f;

				if (montage->RateScale == 10000.0f)
					m_bInstantMeleeApplied = true;
			}
		}
		else
		{
			if (m_bInstantMeleeApplied)
			{
				montage->RateScale = m_flOriginalMeleeRate;

				m_bInstantMeleeApplied = false;
				m_pLastMeleeMontage = nullptr;
			}
		}
	}
}

void Player::InstantReload(bool bEnabled)
{
	if (auto* localChar = Unreal::GetLocalCharacter())
	{

		auto* montage = localChar->LastLocalReloadMontage;
		if (!montage)
		{
			m_pLastReloadMontage = nullptr;
			m_bInstantReloadApplied = false;
			return;
		}

		if (bEnabled)
		{
			if (montage != m_pLastReloadMontage)
			{
				m_pLastReloadMontage = montage;
				m_flOriginalReloadRate = montage->RateScale;
				m_bInstantReloadApplied = false;
			}

			// Keep trying until ratescale is applied
			if (!m_bInstantReloadApplied || montage->RateScale != 10000.0f)
			{
				montage->RateScale = 10000.0f;

				if (montage->RateScale == 10000.0f)
					m_bInstantReloadApplied = true;
			}
		}
		else
		{
			if (m_bInstantReloadApplied)
			{
				montage->RateScale = m_flOriginalReloadRate;

				m_bInstantReloadApplied = false;
				m_pLastReloadMontage = nullptr;
			}
		}
	}
}

bool BackupWeaponData(){ //copied straight from v1 and modified to work with v2
	SDK::UWorld* pGWorld = SDK::UWorld::GetWorld();
	if (!pGWorld)
		return false;

	auto pGameInstance = reinterpret_cast<SDK::USBZGameInstance*>(pGWorld->OwningGameInstance);
	if (!pGameInstance || !pGameInstance->IsA(SDK::USBZGameInstance::StaticClass()))
		return false;

    auto pDatabase = pGameInstance->GlobalItemDatabase;
    if(!pDatabase)
        return false;

    SDK::USBZWeaponDatabase* aDatabases[3]{
        pDatabase->PrimaryWeapons.Get(),
        pDatabase->SecondaryWeapons.Get(),
        pDatabase->OverkillWeapons.Get()
    };

    if(!aDatabases[0] || !aDatabases[1] || !aDatabases[2])
        return false;

    for(int iDatabase = 0; iDatabase < 3; ++iDatabase){
        for(int i = 0; i < aDatabases[iDatabase]->Weapons.Num(); i++){
            if(!aDatabases[iDatabase]->Weapons.IsValidIndex(i))
                return false;

            auto pWeaponData = reinterpret_cast<SDK::USBZRangedWeaponData*>(aDatabases[iDatabase]->Weapons[i]);
            if(!pWeaponData)
                return false;

            if(!pWeaponData->IsA(SDK::USBZRangedWeaponData::StaticClass()))
                continue;

            if(!pWeaponData->SpreadData || !pWeaponData->RecoilData || !pWeaponData->FireData)
                return false;

            g_mapWeaponDataBackup.try_emplace(std::hash<std::string>{}(pWeaponData->Name.ToString().substr(14)), WeaponDataBackupEntry_t{
                .m_flViewSpeedDeflect = pWeaponData->RecoilData->ViewKick.SpeedDeflect,
				.m_flGunKickBackSpeedDeflect = pWeaponData->RecoilData->GunKickBack.SpeedDeflect,
                .m_flGunSpeedDeflect = pWeaponData->RecoilData->GunKickXY.SpeedDeflect,

                .m_flInnerClusterSpreadMultiplier = pWeaponData->SpreadData->InnerClusterSpreadMultiplier,
                .m_flFireSpreadStart = pWeaponData->SpreadData->FireSpreadStart,
                .m_flFireSpreadMinCap = pWeaponData->SpreadData->FireSpreadMinCap,
                .m_flFireSpreadCap = pWeaponData->SpreadData->FireSpreadCap,
                .m_flFireSpreadIncrease = pWeaponData->SpreadData->FireSpreadIncrease,

                .m_iProjectilesPerFiredRound = pWeaponData->FireData->ProjectilesPerFiredRound,
                .m_flRoundsPerMinute = pWeaponData->FireData->RoundsPerMinute,
                .m_eFireMode = pWeaponData->FireData->FireMode
            });
        }
    }
    return true;
}

WeaponDataBackupEntry_t* Player::GetWeaponBackupData(SDK::USBZRangedWeaponData* pWeaponData)
{
    if (!pWeaponData)
        return nullptr;

    auto* pEquippable = pWeaponData->EquippableClass.Get();
    if (!pEquippable)
        return nullptr;

    std::string sName = pEquippable->Name.ToString().substr(16);
    sName.resize(sName.size() - 2);

    size_t hash = std::hash<std::string>{}(sName);

    auto itrEntry = g_mapWeaponDataBackup.find(hash);
    if (itrEntry == g_mapWeaponDataBackup.end())
    {
        return nullptr;
    }

    return &itrEntry->second;
}

std::vector<SDK::USBZRangedWeaponData*> Player::GetCurrentWeaponData(SDK::ASBZPlayerCharacter* pLocalChar)
{
	std::vector<SDK::USBZRangedWeaponData*> weapons;

	// Get weapon from FPCameraAttachment (currently equipped)
	if (pLocalChar->FPCameraAttachment) {
		auto* equippedData = pLocalChar->FPCameraAttachment->EquippedWeaponData;
		if (equippedData && equippedData->IsA(SDK::USBZRangedWeaponData::StaticClass())) {
			weapons.push_back(reinterpret_cast<SDK::USBZRangedWeaponData*>(equippedData));
		}
	}

	// Get weapons from EquippableArray (all carried weapons)
	for (UC::int32 i = 0; i < pLocalChar->EquippableArray.Num(); ++i) {
		if (!pLocalChar->EquippableArray.IsValidIndex(i))
			continue;

		auto* equippable = pLocalChar->EquippableArray[i];
		if (!equippable)
			continue;

		auto* weaponData = equippable->EquippableConfig.EquippableData;
		if (weaponData && weaponData->IsA(SDK::USBZRangedWeaponData::StaticClass())) {
			auto* rangedData = reinterpret_cast<SDK::USBZRangedWeaponData*>(weaponData);
			if (std::find(weapons.begin(), weapons.end(), rangedData) == weapons.end()) {
				weapons.push_back(rangedData);
			}
		}
	}
	return weapons;
}

void Player::noRecoil(bool bEnabled)
{
	SDK::ASBZPlayerCharacter* localChar = Unreal::GetLocalCharacter();
	if (!localChar || !g_bDidBackupWeaponData)
		return;

	for (auto* weaponData : GetCurrentWeaponData(localChar)) {
		if (!weaponData || !weaponData->RecoilData)
			continue;

		if (bEnabled) {
			weaponData->RecoilData->ViewKick.SpeedDeflect = 0.f;
			weaponData->RecoilData->GunKickBack.SpeedDeflect = 0.f;
			weaponData->RecoilData->GunKickXY.SpeedDeflect = 0.f;
		} else {
			auto* backup = GetWeaponBackupData(weaponData);
			if (backup) {
				weaponData->RecoilData->ViewKick.SpeedDeflect = backup->m_flViewSpeedDeflect;
				weaponData->RecoilData->GunKickBack.SpeedDeflect = backup->m_flGunKickBackSpeedDeflect;
				weaponData->RecoilData->GunKickXY.SpeedDeflect = backup->m_flGunSpeedDeflect;
			}
		}
	}
}

void Player::noSpread(bool bEnabled)
{
	SDK::ASBZPlayerCharacter* localChar = Unreal::GetLocalCharacter();
	if (!localChar || !g_bDidBackupWeaponData)
		return;

	for (auto* weaponData : GetCurrentWeaponData(localChar)) {
		if (!weaponData || !weaponData->SpreadData)
			continue;

		if (bEnabled) {
			weaponData->SpreadData->InnerClusterSpreadMultiplier = 
				weaponData->SpreadData->FireSpreadStart = 
				weaponData->SpreadData->FireSpreadMinCap = 
				weaponData->SpreadData->FireSpreadCap = 
				weaponData->SpreadData->FireSpreadIncrease = 0.f;
		} else {
			auto* backup = GetWeaponBackupData(weaponData);
			if (backup) {
				weaponData->SpreadData->InnerClusterSpreadMultiplier = backup->m_flInnerClusterSpreadMultiplier;
				weaponData->SpreadData->FireSpreadStart = backup->m_flFireSpreadStart;
				weaponData->SpreadData->FireSpreadMinCap = backup->m_flFireSpreadMinCap;
				weaponData->SpreadData->FireSpreadCap = backup->m_flFireSpreadCap;
				weaponData->SpreadData->FireSpreadIncrease = backup->m_flFireSpreadIncrease;
			}
		}
	}
}

void Player::fireRate(bool bEnabled)
{
	SDK::ASBZPlayerCharacter* localChar = Unreal::GetLocalCharacter();
	if (!localChar || !g_bDidBackupWeaponData)
		return;

	for (auto* weaponData : GetCurrentWeaponData(localChar)) {
		if (!weaponData || !weaponData->FireData)
			continue;

		auto* backup = GetWeaponBackupData(weaponData);
		if (bEnabled) {
			weaponData->FireData->RoundsPerMinute = backup->m_flRoundsPerMinute * m_pFireRateSlider->GetValue();
			weaponData->FireData->FireMode = SDK::ESBZFireMode::Auto;
		} else {
			if (backup) {
				weaponData->FireData->RoundsPerMinute = backup->m_flRoundsPerMinute;
				weaponData->FireData->FireMode = backup->m_eFireMode;
			}
		}
	}
}

void Player::Run()
{
	if (!g_bDidBackupWeaponData)
	{
		g_bDidBackupWeaponData = BackupWeaponData();
	}
	/////////////////Tab 1///////////////////

	// Godmode
	if (m_pGodMode->GetValue())
	{
		if (auto* localChar = Unreal::GetLocalCharacter())
		{

			if (m_pGodModeType->GetSelectedIndex() == 0)
			{
				auto* PlayerAttributeSet = localChar->PlayerAttributeSet;
				// Need to change back on disable, too lazy to do rn
				PlayerAttributeSet->Health.CurrentValue = 200.0f;
			}
			else
			{
				blockDamage(true);
			}
		}
	}
	else
	{
		blockDamage(false);
	}

	//Infinite Stamina
	if (m_pInfStamina->GetValue())
	{
		if (auto* localChar = Unreal::GetLocalCharacter())
		{
			auto* PlayerAttributeSet = localChar->PlayerAttributeSet;
			PlayerAttributeSet->Stamina.CurrentValue = 100.0f;
		}
	}

	//Instant Melee
	if (m_pInstaMelee->GetValue())
	{
		InstantMelee(m_pInstaMelee->GetValue());
	}

	//No Screen Shake
	if (m_pNoScreenshake->GetValue())
	{
		auto* camManager = Unreal::GetPlayerCameraManager();
		if (!camManager)
			return;

		camManager->StopAllCameraShakes(m_pNoScreenshake->GetValue());
	}

	//No Fall Damage
	if (m_pNoFallDamage->GetValue())
	{
		if (auto* localChar = Unreal::GetLocalCharacter())
		{
			localChar->FallingStartHeight = localChar->K2_GetActorLocation().Z;
		}
	}

	//No Detection
	if (m_pNoDetection->GetValue())
	{
		if (auto* localChar = Unreal::GetLocalCharacter())
		{
			for (UC::int32 i=0; i<localChar->VisualDetectors.Num(); ++i) {
				auto* det = localChar->VisualDetectors[i];
			if (!det) continue;

				for (UC::int32 e=0; e<det->EnemyDetectionValue.Num(); ++e) {
					auto& d = det->EnemyDetectionValue[e];
					d.Target = nullptr;
				}
				det->bMarkAsCriminalOnSearch = false;
				det->bShouldDisplayDetectionBuildup = false;
			}
		}
	}

	/////////////////Tab 2///////////////////

	//Populate the player table with player(s) info here

	/////////////////Tab 3///////////////////

	//Instant Reload
	if (m_pInstaReload->GetValue())
	{
		InstantReload(m_pInstaReload->GetValue());
	}

	//Infinite Ammo
	if (m_pInfAmmo->GetValue()) //Needs rework to return ammo back to normal on disable
	{
		if(auto* localChar = Unreal::GetLocalCharacter())
		{
			auto* Att = localChar->PlayerAttributeSet;

			Att->PrimaryEquippableAmmoInventory.CurrentValue = 999;
			Att->SecondaryEquippableAmmoInventory.CurrentValue = 999;
			Att->TertiaryEquippableAmmoInventory.CurrentValue = 999;

			Att->PrimaryThrowableAmmoInventory.CurrentValue = 999;
			Att->SecondaryThrowableAmmoInventory.CurrentValue = 999;
			Att->TertiaryThrowableAmmoInventory.CurrentValue = 999;

			Att->PrimaryToolAmmoInventory.CurrentValue = 999;
			Att->SecondaryToolAmmoInventory.CurrentValue = 999;
			Att->TertiaryToolAmmoInventory.CurrentValue = 999;
		}
	}

	//No Recoil
	if (m_pNoRecoil->GetValue())
	{
		noRecoil(true);
	}

	//No Spread
	if (m_pNoSpread->GetValue())
	{
		noSpread(true);
	}

	//Fire Rate
	if (m_pFireRate->GetValue())
	{
		fireRate(true);
	}
}