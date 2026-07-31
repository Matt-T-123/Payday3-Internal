#pragma once
#include "pch.h"

class Visuals : public BaseFeature
{
private:
	inline static uint8_t s_iVisualsPageId = ElementBase::AddPage("VISUALS_BUTTON"Hashed, ICON_FA_EYE);

	std::unique_ptr<RadioButtonIcon> m_pMenuButton = std::make_unique<RadioButtonIcon>(
		std::string("VISUALS_BUTTON"),
		"VISUALS_BUTTON"Hashed,
		ElementBase::Style_t({ .vec2Size = ImVec2(-0.1f, 0) }),
		ICON_FA_EYE,
		s_iVisualsPageId);

	std::unique_ptr<Page> m_pTab1Page = std::make_unique<Page>(
		"VISUALS_TAB1_PAGE",
		ElementBase::Style_t(),
		s_iVisualsPageId,
		0);

	std::unique_ptr<Group> m_pTab1Group = std::make_unique<Group>("VISUALS_TAB1_GROUP", ElementBase::Style_t{});

	std::unique_ptr<GroupChild> m_pTab1Left = std::make_unique<GroupChild>(
		"VISUALS_TAB1_LEFT",
		"VISUALS_TAB1_LEFT"Hashed,
		ElementBase::Style_t(),
		ImGuiChildFlags_Border);

	std::unique_ptr<GroupChild> m_pTab1Right = std::make_unique<GroupChild>(
		"VISUALS_TAB1_RIGHT",
		"VISUALS_TAB1_RIGHT"Hashed,
		ElementBase::Style_t{ .eSameLine = ElementBase::ESameLine::Same },
		ImGuiChildFlags_Border);
	
	std::unique_ptr<GroupChild> m_pTab1Bottom = std::make_unique<GroupChild>(
		"VISUALS_TAB1_BOTTOM",
		"VISUALS_TAB1_BOTTOM"Hashed,
		ElementBase::Style_t(),
		ImGuiChildFlags_Border);

	std::unique_ptr<Checkbox> m_pBoundingBox = std::make_unique<Checkbox>("VISUALS_BOUNDING_BOX", "VISUALS_BOUNDING_BOX"Hashed);
	std::unique_ptr<ColorPicker> m_pBoundingBoxCopColor = std::make_unique<ColorPicker>("VISUALS_BOUNDING_BOX_COP_COLOR", "VISUALS_BOUNDING_BOX_COP_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pBoundingBoxCivilianColor = std::make_unique<ColorPicker>("VISUALS_BOUNDING_BOX_CIVILIAN_COLOR", "VISUALS_BOUNDING_BOX_CIVILIAN_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pName = std::make_unique<Checkbox>("VISUALS_NAME", "VISUALS_NAME"Hashed);
	std::unique_ptr<ColorPicker> m_pNameCopColor = std::make_unique<ColorPicker>("VISUALS_NAME_COP_COLOR", "VISUALS_NAME_COP_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pNameCivilianColor = std::make_unique<ColorPicker>("VISUALS_NAME_CIVILIAN_COLOR", "VISUALS_NAME_CIVILIAN_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pDistance = std::make_unique<Checkbox>("VISUALS_DISTANCE", "VISUALS_DISTANCE"Hashed);
	std::unique_ptr<ColorPicker> m_pDistanceCopColor = std::make_unique<ColorPicker>("VISUALS_DISTANCE_COP_COLOR", "VISUALS_DISTANCE_COP_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pDistanceCivilianColor = std::make_unique<ColorPicker>("VISUALS_DISTANCE_CIVILIAN_COLOR", "VISUALS_DISTANCE_CIVILIAN_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pHealthBar = std::make_unique<Checkbox>("VISUALS_HEALTH_BAR", "VISUALS_HEALTH_BAR"Hashed);
	std::unique_ptr<ColorPicker> m_pHealthBarCopColor = std::make_unique<ColorPicker>("VISUALS_HEALTH_BAR_COP_COLOR", "VISUALS_HEALTH_BAR_COP_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pHealthBarCivilianColor = std::make_unique<ColorPicker>("VISUALS_HEALTH_BAR_CIVILIAN_COLOR", "VISUALS_HEALTH_BAR_CIVILIAN_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pArmorBar = std::make_unique<Checkbox>("VISUALS_ARMOR_BAR", "VISUALS_ARMOR_BAR"Hashed);
	std::unique_ptr<ColorPicker> m_pArmorBarColor = std::make_unique<ColorPicker>("VISUALS_ARMOR_BAR_COLOR", "VISUALS_ARMOR_BAR_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pSkeleton = std::make_unique<Checkbox>("VISUALS_SKELETON", "VISUALS_SKELETON"Hashed);
	std::unique_ptr<ColorPicker> m_pSkeletonCopColor = std::make_unique<ColorPicker>("VISUALS_SKELETON_COP_COLOR", "VISUALS_SKELETON_COP_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pSkeletonCivilianColor = std::make_unique<ColorPicker>("VISUALS_SKELETON_CIVILIAN_COLOR", "VISUALS_SKELETON_CIVILIAN_COLOR"Hashed);

	std::unique_ptr<Checkbox> m_pHighlight = std::make_unique<Checkbox>("VISUALS_HIGHLIGHT", "VISUALS_HIGHLIGHT"Hashed);
	
	std::unique_ptr<Checkbox> m_pItem = std::make_unique<Checkbox>("VISUALS_ITEM", "VISUALS_ITEM"Hashed);
	std::unique_ptr<ColorPicker> m_pItemCashColor = std::make_unique<ColorPicker>("VISUALS_ITEM_CASH_COLOR", "VISUALS_ITEM_CASH_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pItemDepositBoxColor = std::make_unique<ColorPicker>("VISUALS_ITEM_DEPOSITBOX_COLOR", "VISUALS_ITEM_DEPOSITBOX_COLOR"Hashed);
	std::unique_ptr<ColorPicker> m_pItemKeycardColor = std::make_unique<ColorPicker>("VISUALS_ITEM_KEYCARD_COLOR", "VISUALS_ITEM_KEYCARD_COLOR"Hashed);

	std::unique_ptr<MultiSelectCombo> m_pFilters = std::make_unique<MultiSelectCombo>("VISUALS_FILTERS", "VISUALS_FILTERS"Hashed, ElementBase::Style_t{ .vec2Size = ImVec2(-0.1f, 0) });
	std::unique_ptr<MultiSelectCombo> m_pItemFilters = std::make_unique<MultiSelectCombo>("VISUALS_ITEM_FILTERS", "VISUALS_ITEM_FILTERS"Hashed, ElementBase::Style_t{ .vec2Size = ImVec2(-0.1f, 0) });

	struct ESPData
	{
		ImVec4 Box;
		std::string Name;
		SDK::USkeletalMeshComponent* Mesh;
		SDK::ASBZCharacter* Character;
		float Health;
		float HealthMax;
		float Armor;
		float ArmorMax;
		float Distance;
		bool IsCop;
		bool IsCivilian;
	};
	std::vector<ESPData> m_vESPData;

	enum class EnemyCategory // This is only used for the mutliselect combo
	{
		None,
		Cop,
		Civilian
	};

	struct EnemyInfo
	{
		const char* Name;
		EnemyCategory Category;
	};

	enum class EnemyType // The order between this and g_EnemyInfo must match
	{
		None,
		Security,
		ArmedCop,
		Shield,
		Dozer,
		Cloaker,
		Sniper,
		Taser,
		Tower,
		Shotgun,
		AR,
		SMG,
		Grenadier,
		Civilian,
		SentryGun
	};

	struct EnemyLookup
	{
		std::string_view Keyword;
		EnemyType Type;
	};

	static constexpr EnemyLookup g_EnemyLookup[] =
	{
		{ "security",   EnemyType::Security },
		{ "armedcop",   EnemyType::ArmedCop },
		{ "shield",     EnemyType::Shield },
		{ "dozer",      EnemyType::Dozer },
		{ "cloaker",    EnemyType::Cloaker },
		{ "sniper",     EnemyType::Sniper },
		{ "taser",      EnemyType::Taser },
		{ "tower",      EnemyType::Tower },
		{ "shotgun",    EnemyType::Shotgun },
		{ "grenadier",  EnemyType::Grenadier },
		{ "civilian",   EnemyType::Civilian },
		{ "smg",        EnemyType::SMG },
		{ "ar",         EnemyType::AR }
	};

	static constexpr EnemyInfo g_EnemyInfo[] =
	{
		{ "Unknown",    EnemyCategory::None },
		{ "Security",   EnemyCategory::Cop },
		{ "Armed Cop",  EnemyCategory::Cop },
		{ "Shield",     EnemyCategory::Cop },
		{ "Dozer",      EnemyCategory::Cop },
		{ "Cloaker",    EnemyCategory::Cop },
		{ "Sniper",     EnemyCategory::Cop },
		{ "Taser",      EnemyCategory::Cop },
		{ "Tower",      EnemyCategory::Cop },
		{ "Shotgun",    EnemyCategory::Cop },
		{ "AR",         EnemyCategory::Cop },
		{ "SMG",        EnemyCategory::Cop },
		{ "Grenadier",  EnemyCategory::Cop },
		{ "Civilian",   EnemyCategory::Civilian },
		{ "Sentry Gun", EnemyCategory::Cop }
	};

	struct BoneCache
	{
		bool Initialized = false;

		// Core body
		int Hips = -1;
		int Spine = -1;
		int Spine1 = -1;
		int Spine2 = -1;
		int Spine3 = -1;
		int Neck = -1;
		int Head = -1;

		// Left arm
		int LeftShoulder = -1;
		int LeftUpperArm = -1;
		int LeftForeArm = -1;
		int LeftHand = -1;

		// Right arm
		int RightShoulder = -1;
		int RightUpperArm = -1;
		int RightForeArm = -1;
		int RightHand = -1;

		// Left leg
		int LeftUpperLeg = -1;
		int LeftLowerLeg = -1;
		int LeftFoot = -1;
		int LeftToe = -1;

		// Right leg
		int RightUpperLeg = -1;
		int RightLowerLeg = -1;
		int RightFoot = -1;
		int RightToe = -1;
	};

	std::unordered_map<SDK::USkeletalMeshComponent*, BoneCache> m_BoneCache;
	std::unordered_map<SDK::UClass*, EnemyType> m_ClassCache;

	enum class ItemCategory // This is only used for the mutliselect combo
	{
		None,
		Cash,
		DepositBox,
		Keycard
	};

	enum class ItemType
	{
		None,
		Cash,
		DepositBox,
		Keycard
	};

	struct ItemLookup
	{
		std::string_view Keyword;
		ItemType Type;
	};

	static constexpr ItemLookup g_ItemLookup[] =
	{
		{ "instantloot_money_",     ItemType::Cash },
		{ "depositbox", 			ItemType::DepositBox },
		{ "keycard",        		ItemType::Keycard }
	};

	struct ItemData
	{
		SDK::FVector2D ScreenLocation;
		SDK::FVector WorldLocation;
		std::string Name;
		ItemType Type;
	};
	std::vector<ItemData> m_vItemData;

	std::unordered_map<SDK::UClass*, ItemType> m_ItemTypeCache;

public:
	bool SetupMenu();
	void UpdateMenuVisibility();
	void HandleMenu();
	void BuildBoneCache(SDK::USkeletalMeshComponent* mesh, BoneCache& cache);
	ItemType GetItemType(SDK::AActor* actor);
	void DrawBone(ImDrawList* pDrawList, SDK::APlayerController* pPlayerController, SDK::USkeletalMeshComponent* mesh, int parent, int child, ImU32 color);
	void Render();
	void Run();
	RadioButtonIcon* GetMenuButton() const { return m_pMenuButton.get(); }
	std::string GetName() { return "Visuals"; };
};

inline std::unique_ptr<Visuals> pVisuals = std::make_unique<Visuals>();