#pragma once
#include "../Localization.hpp"

Locale_t localeEnglish{
	.sKey = "English",
	.ullKeyHash = "ENG"Hashed,
	.hMenuFont = &TahomaFont,
	.hFeatureFont = &TahomaFontFeature,
	.umLocalizedStrings = std::unordered_map<size_t, std::string>({
		{ "SIDEBAR"Hashed, "Sidebar" },
		{ "PLAYER_SEPERATOR"Hashed, "Player" },
		{ "MISC_SEPERATOR"Hashed, "Misc" },
		{ "CONFIG_BUTTON"Hashed, "Config" },

		//-----------------------------------------------------------------

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
		{ "PLAYER_FIRE_RATE"Hashed, "Fire Rate" },

		//-----------------------------------------------------------------

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
        { "VISUALS_ITEM_FILTERS"Hashed, "" },

		//------------------------------------------------------------

		{ "AIMBOT_BUTTON"Hashed, "Aimbot" },
		{ "AIMBOT_TAB1"Hashed, "Main" },
		{ "AIMBOT_TAB1_LEFT"Hashed, "Aimbot" },
		{ "AIMBOT_TAB1_RIGHT"Hashed, "Options" },

		{ "AIMBOT_ENABLED"Hashed, "Aimbot" },
		{ "AIMBOT_VISIBLE_CHECK"Hashed, "Visible Check" },
		{ "AIMBOT_FOV_ENABLED"Hashed, "FOV Circle" },

		{ "AIMBOT_HOTKEY"Hashed, "Aimbot Hotkey" },
		{ "AIMBOT_TYPE"Hashed, "Aimbot Type" },
		{ "AIMBOT_TARGET"Hashed, "Target" },
		{ "AIMBOT_FOV_COLOR"Hashed, "FOV Color" },
		{ "AIMBOT_FOV"Hashed, "FOV" },
		{ "AIMBOT_SMOOTHING"Hashed, "Smoothing" },

		//------------------------------------------------------------

		{ "UI"Hashed, "UI" },
		{ "UI_BUTTON"Hashed, "UI" },
		{ "UI_MAIN"Hashed, "Main" },
		{ "UI_UNLOAD_BUTTON"Hashed, "Unload" },
		{ "UI_CONSOLE_SHOW"Hashed, "Show Console" },
		{ "UI_CONSOLE_HIDE"Hashed, "Hide Console" },
		{ "UI_LANGUAGE"Hashed, "Language" },

		//------------------------------------------------------------

		{ "CHEAT"Hashed, "Cheat"},
		{ "SAVE_CONFIG"Hashed, "Save Config" },
		{ "LOAD_CONFIG"Hashed, "Load Config" },
	}),
};
