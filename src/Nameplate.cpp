/*
This file is part of Nameplate.
Copyright (C) 2024 BunnyBox Productions

Nameplate is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Nameplate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Nameplate.
If not, see <https://www.gnu.org/licenses/>.
*/

#include <windows.h>
#include <strsafe.h>
#include <inttypes.h>

#include <stdlib.h>
#include <ctype.h>

#include "Nameplate.h"

#include "bytes/bytes.h"

import findsignature;

// No copyright is claimed on the following "fair use for interoperability" section.
/* Begin "fair use for interoperability" section */
#define GETFRAMEDELTA "8B0D????????D94128D81D????????DFE0F6C4057A07D905????????C3"
#define INITAPPLICATIONDEFAULTS "B90002000033C0890D????????890D????????B901000000A3????????C705????????80020000C705????????E0010000"
#define GETPLAYERENTITY "66A1????????6685C0740E0FBFC08B0485????????85C0750233C0C3"

#define DRAWNAME_PATCH0_LOCATION "E8????????25FFFF000089442410DB442410D80D????????D88C24E0060000D95C244CE8????????8BBC24DC06000025FFFF00008944241083C9FFDB44241033C0F2AED80D????????D88C24E0060000F7D14983F924894C2418D95C24507E09B924000000894C2418"
#define DRAWNAME_PATCH1_LOCATION "E8????????25FFFF000089442410DB442410D80D????????D88C24D4060000D95C2438E8????????25FFFF00008BAC24D0060000894424108BFDDB44241083C9FF33C0F2AED80D????????D88C24D4060000F7D14983F924"
#define DRAWNAME2_PATCH_LOCATION "6683B9B20000000075178B44240C8B4C2408"
#define DRAWNAME3_PATCH_LOCATION "A0????????84C0745E8B4E7033C0A0????????83F803774FFF2485????????F74178000000FF753F8B168BCEFF92CC0000005F5E81C418010000C38B517881E2000000FF81FA00000001751B8B8120010000C1E80DA801EBCD8B8920010000C1E90DF6C10175C1"
#define DRAWNAME3_V2_PATCH_LOCATION "A0????????84C0747A8B7E7033C0A0????????83F804776BFF2485????????F74778000000FF755B8B168BCEFF92CC0000005F5E81C418010000C38B4F7881E1000000FF81F90000000175378B9720010000C1EA0DF6C201EBCC8B8720010000C1E80DA801EB1AF74778000000FF7513E8????????85C0740A8B48788B47783BC875A5"

#define DRAWDAMAGE_PATCH_LOCATION "A1????????8B88C80D0000E8????????8B5008A1????????8954240CDB44240CD84C2408D80D????????D95C24208B88C80D0000E8????????8B400C8B0D????????8944240C8BF1DB44240C8BD681E2FFFFFF00D84C240881FA80808000D80D????????D95C2424750E8B879401000081E6000000FF0BF0"
/* End "fair use for interoperability" section */

static constexpr auto getFrameDeltaSig = MakeSig<GETFRAMEDELTA>();
static constexpr auto initApplicationDefaultsSig = MakeSig<INITAPPLICATIONDEFAULTS>();
static constexpr auto getPlayerEntitySig = MakeSig<GETPLAYERENTITY>();

static constexpr auto drawNamePatch0LocationSig = MakeSig<DRAWNAME_PATCH0_LOCATION>();
static constexpr auto drawNamePatch1LocationSig = MakeSig<DRAWNAME_PATCH1_LOCATION>();
static constexpr auto drawName2PatchLocationSig = MakeSig<DRAWNAME2_PATCH_LOCATION>();
static constexpr auto drawName3PatchLocationSig = MakeSig<DRAWNAME3_PATCH_LOCATION>();
static constexpr auto drawName3V2PatchLocationSig = MakeSig<DRAWNAME3_V2_PATCH_LOCATION>();
static constexpr auto drawDamagePatchLocationSig = MakeSig<DRAWDAMAGE_PATCH_LOCATION>();

static constexpr size_t DRAWNAME_PATCH0_SIZE = __builtin_strlen(DRAWNAME_PATCH0_LOCATION) / 2;
static constexpr size_t DRAWNAME_PATCH1_SIZE = __builtin_strlen(DRAWNAME_PATCH1_LOCATION) / 2;
static constexpr size_t DRAWNAME2_PATCH_SIZE = __builtin_strlen(DRAWNAME2_PATCH_LOCATION) / 2;
static constexpr size_t DRAWNAME3_PATCH_SIZE = __builtin_strlen(DRAWNAME3_PATCH_LOCATION) / 2;
static constexpr size_t DRAWNAME3_V2_PATCH_SIZE = __builtin_strlen(DRAWNAME3_V2_PATCH_LOCATION) / 2;
static constexpr size_t DRAWDAMAGE_PATCH_SIZE = __builtin_strlen(DRAWDAMAGE_PATCH_LOCATION) / 2;

// Wish I could just #embed these...
// See src/assembly for source code.
static constexpr uint8_t DrawName_Patch0[DRAWNAME_PATCH0_SIZE] = {
  0x8d, 0x4c, 0x24, 0x4c, 0x8d, 0x94, 0x24, 0xe0, 0x06, 0x00, 0x00, 0xb8,
  0x78, 0x56, 0x34, 0x12, 0xff, 0xd0, 0x8b, 0xbc, 0x24, 0xdc, 0x06, 0x00,
  0x00, 0x83, 0xc9, 0xff, 0x31, 0xc0, 0xf2, 0xae, 0xf7, 0xd1, 0x49, 0x83,
  0xf9, 0x24, 0x7e, 0x05, 0xb9, 0x24, 0x00, 0x00, 0x00, 0x89, 0x4c, 0x24,
  0x18, 0xeb, 0x36, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};
#define DRAWNAME_PATCH0_ADDRESS_LOCATION 12

static constexpr uint8_t DrawName_Patch1[DRAWNAME_PATCH1_SIZE] = {
  0x8d, 0x4c, 0x24, 0x38, 0x8d, 0x94, 0x24, 0xd4, 0x06, 0x00, 0x00, 0xb8,
  0x78, 0x56, 0x34, 0x12, 0xff, 0xd0, 0xd9, 0x44, 0x24, 0x3c, 0x8b, 0xac,
  0x24, 0xd0, 0x06, 0x00, 0x00, 0x83, 0xc9, 0xff, 0x89, 0xef, 0x31, 0xc0,
  0xf2, 0xae, 0xf7, 0xd1, 0x49, 0x83, 0xf9, 0x24, 0xeb, 0x2a, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90
};
#define DRAWNAME_PATCH1_ADDRESS_LOCATION 12

// No custom assembly here-- just patching a jnz to a jmp
#define DRAWNAME2_PATCH_ADDRESS_LOCATION 8

static constexpr uint8_t DrawName3_Patch[DRAWNAME3_PATCH_SIZE] = {
  0x89, 0xf1, 0xb8, 0x78, 0x56, 0x34, 0x12, 0xff, 0xd0, 0x84, 0xc0, 0x75,
  0x5a, 0x74, 0x19, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x8b, 0x16, 0x89, 0xf1, 0xff, 0x92, 0xcc, 0x00,
  0x00, 0x00, 0x5f, 0x5e, 0x81, 0xc4, 0x18, 0x01, 0x00, 0x00, 0xc3, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};
#define DRAWNAME3_PATCH_ADDRESS_LOCATION 3

static constexpr uint8_t DrawName3_V2_Patch[DRAWNAME3_V2_PATCH_SIZE] = {
  0x89, 0xf1, 0xb8, 0x78, 0x56, 0x34, 0x12, 0xff, 0xd0, 0x84, 0xc0, 0x75,
  0x76, 0x74, 0x19, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x8b, 0x16, 0x89, 0xf1, 0xff, 0x92, 0xcc, 0x00,
  0x00, 0x00, 0x5f, 0x5e, 0x81, 0xc4, 0x18, 0x01, 0x00, 0x00, 0xc3, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};
#define DRAWNAME3_V2_PATCH_ADDRESS_LOCATION 3

static constexpr uint8_t DrawDamage_Patch[DRAWDAMAGE_PATCH_SIZE] = {
  0x8d, 0x4c, 0x24, 0x20, 0x8d, 0x54, 0x24, 0x08, 0xb8, 0x78, 0x56, 0x34,
  0x12, 0xff, 0xd0, 0x8b, 0x0d, 0x78, 0x56, 0x34, 0x12, 0x89, 0xca, 0x89,
  0xce, 0x81, 0xe2, 0xff, 0xff, 0xff, 0x00, 0x81, 0xfa, 0x80, 0x80, 0x80,
  0x00, 0x75, 0x51, 0x8b, 0x87, 0x94, 0x01, 0x00, 0x00, 0x81, 0xe6, 0x00,
  0x00, 0x00, 0xff, 0x09, 0xc6, 0xeb, 0x41, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

#define DRAWDAMAGE_PATCH_CALL_ADDRESS_LOCATION 0x09
#define DRAWDAMAGE_PATCH_COLOR_ADDRESS_LOCATION 0x11
#define DRAWDAMAGE_PATCH_OLD_COLOR_ADDRESS_LOCATION 0x3E

// "CGame" - various application-level rendering-related things.
typedef struct CGame {
	uint8_t unknown0[0x18];
	uint16_t BackgroundWidth;
	uint16_t BackgroundHeight;
	uint8_t unknown1[0x33C - 0x1C];
} CGame;

typedef struct Entity {
	uint8_t unknown0[0x74];
	uint32_t EntityIndex;
	uint32_t EntityId;
	uint8_t unknown1[0x120 - 0x7C];
	uint32_t Flags0;
	// etc.
} Entity;

typedef struct XiModel {
	uint8_t unknown0[0x70];
	Entity* pEntity;
	// etc.
} XiModel;

static struct {
	CGame** pgGame;
	uint32_t* pWindowWidth;
	uint32_t* pWindowHeight;
	uint16_t* pPlayerEntityIndex;
	bool* pNamesOff;

	uintptr_t DrawNamePatch0Addr;
	uint8_t DrawNamePatch0Orig[DRAWNAME_PATCH0_SIZE];

	uintptr_t DrawNamePatch1Addr;
	uint8_t DrawNamePatch1Orig[DRAWNAME_PATCH1_SIZE];

	uintptr_t DrawName2PatchAddr;
	uint8_t DrawName2PatchOrig[DRAWNAME2_PATCH_SIZE];

	uintptr_t DrawName3PatchAddr;
	uint8_t DrawName3PatchOrig[DRAWNAME3_PATCH_SIZE];

	uintptr_t DrawName3V2PatchAddr;
	uint8_t DrawName3V2PatchOrig[DRAWNAME3_V2_PATCH_SIZE];

	uintptr_t DrawDamagePatchAddr;
	uint8_t DrawDamagePatchOrig[DRAWDAMAGE_PATCH_SIZE];
} Globals;

static struct {
	int FontSize;
	int DamageFontSize;
	int HideStars;
	NAME_MODE NameMode;
} AsmSettings;

static void __fastcall GetNameplateScale(float* wh, const float* pS) {
	if (Globals.pgGame != nullptr) {
		auto gGame = *Globals.pgGame;

		double bkgW = gGame->BackgroundWidth;
		double bkgH = gGame->BackgroundHeight;
		double winW = *Globals.pWindowWidth;
		double winH = *Globals.pWindowHeight;

		double wRatio = bkgW / winW;
		double hRatio = bkgH / winH;

		double s = *pS;

		double FontScale = (double) AsmSettings.FontSize / 11.0;

		wh[0] = static_cast<float>(FontScale * wRatio * s);
		wh[1] = static_cast<float>(FontScale * hRatio * s);
	}
}

static void __fastcall GetDamageNumberScale(float* wh, const float* pS) {
	if (Globals.pgGame != nullptr) {
		auto gGame = *Globals.pgGame;

		double bkgW = gGame->BackgroundWidth;
		double bkgH = gGame->BackgroundHeight;
		double winW = *Globals.pWindowWidth;
		double winH = *Globals.pWindowHeight;

		double wRatio = bkgW / winW;
		double hRatio = bkgH / winH;

		double s = *pS;

		// Using the same scale factor seems to look correct.
		double FontScale = (double) AsmSettings.DamageFontSize / 11.0;

		wh[0] = static_cast<float>(FontScale * wRatio * s);
		wh[1] = static_cast<float>(FontScale * hRatio * s);
	}
}

static bool __fastcall ShouldDrawNameplate(const XiModel* pModel) {
	if (*Globals.pNamesOff) {
		return false;
	}

	if (pModel == nullptr) {
		return false;
	}
	const Entity* pEntity = pModel->pEntity;
	if (pEntity == nullptr) {
		return false;
	}

	// client version (not accessable in retail client)
	// note - (Entity->Flags0 >> 13) & 1 is attackable entity
	// mode 0 - off
	// mode 1 - pc only
	// mode 2 - mob (incl. attackable pc) and npc
	// mode 3 - non-attackable entities only

	if (Globals.pPlayerEntityIndex != nullptr) {
		switch (AsmSettings.NameMode) {
			case NAME_MODE::ALL:
			default:
				return true;

			case NAME_MODE::NONE:
				return false;

			case NAME_MODE::HIDE_SELF:
				return (pEntity->EntityIndex != *Globals.pPlayerEntityIndex);

			case NAME_MODE::HIDE_PCSELF:
				if (pEntity->EntityIndex == *Globals.pPlayerEntityIndex) {
					return false;
				}
				[[fallthrough]];
			case NAME_MODE::HIDE_PC:
				if ((pEntity->EntityId & 0xFF000000) != 0) {
					return true;
				}
				return (((pEntity->Flags0 >> 13) & 1) != 0);

			case NAME_MODE::HIDE_NPCSELF:
				if (pEntity->EntityIndex == *Globals.pPlayerEntityIndex) {
					return false;
				}
				[[fallthrough]];
			case NAME_MODE::HIDE_NPC:
				return ((pEntity->EntityId & 0xFF000000) == 0);
		}
	}

	return true;
}

void NameplateSettings::Flush() {
	AsmSettings.FontSize = FontSize;
	AsmSettings.DamageFontSize = DamageFontSize;
	AsmSettings.HideStars = HideStars;
	AsmSettings.NameMode = NameMode;
}

void NameplateSettings::Normalize() {
	if (FontSize < 1) {
		FontSize = 1;
	}
	if (FontSize > 256) {
		FontSize = 256;
	}

	if (DamageFontSize < 1) {
		DamageFontSize = 1;
	}
	if (DamageFontSize > 256) {
		DamageFontSize = 256;
	}

	HideStars = (HideStars != 0) ? 1 : 0;

	if (NameMode < NAME_MODE::MIN) {
		NameMode = NAME_MODE::ALL;
	}
	if (NameMode > NAME_MODE::MAX) {
		NameMode = NAME_MODE::ALL;
	}
}

int NameplateSettings::Init() {

	FontSize = 22;
	DamageFontSize = 22;
	HideStars = 0;
	NameMode = NAME_MODE::ALL;

	return 0;
}

int NameplateSettings::Load(const wchar_t* basePath) {
	wchar_t path[UNICODE_MAX_PATH];
	StringCchPrintfW(path, UNICODE_MAX_PATH, L"%s\\nameplate\\defaults.ini", basePath);

	FontSize = GetPrivateProfileIntW(L"Settings", L"FontSizeInPx", 22, path);
	DamageFontSize = GetPrivateProfileIntW(L"Settings", L"DamageFontSizeInPx", 22, path);
	HideStars = GetPrivateProfileIntW(L"Settings", L"HideStars", 0, path);
	NameMode = static_cast<NAME_MODE>(GetPrivateProfileIntW(L"Settings", L"NameMode", static_cast<int>(NAME_MODE::ALL), path));

	Normalize();
	Flush();

	return 0;
}

int NameplateSettings::Save(const wchar_t* basePath) {
	wchar_t path[UNICODE_MAX_PATH];

	StringCchPrintfW(path, UNICODE_MAX_PATH, L"%s\\nameplate", basePath);
	CreateDirectoryW(path, NULL);

	StringCchPrintfW(path, UNICODE_MAX_PATH, L"%s\\nameplate\\defaults.ini", basePath);

	wchar_t buf[256];

	StringCbPrintfW(buf, sizeof(buf), L"%d", FontSize);
	WritePrivateProfileStringW(L"Settings", L"FontSizeInPx", buf, path);

	StringCbPrintfW(buf, sizeof(buf), L"%d", DamageFontSize);
	WritePrivateProfileStringW(L"Settings", L"DamageFontSizeInPx", buf, path);

	StringCbPrintfW(buf, sizeof(buf), L"%d", HideStars);
	WritePrivateProfileStringW(L"Settings", L"HideStars", buf, path);

	StringCbPrintfW(buf, sizeof(buf), L"%d", static_cast<int>(NameMode));
	WritePrivateProfileStringW(L"Settings", L"NameMode", buf, path);

	return 0;
}

static bool is_end(char c) {
	return (c == '\0');
}

static bool is_end(const char* cmd) {
	return is_end(*cmd);
}

static bool isspace_or_end(char c) {
	return isspace(c) || (c == '\0');
}

static bool isspace_or_end(const char* cmd) {
	return isspace_or_end(*cmd);
}

static void eat_space(const char*& cmd) {
	while (isspace(*cmd)) {
		cmd++;
	}
}

static bool eat_space_and_is_end(const char*& cmd) {
	eat_space(cmd);
	return is_end(cmd);
}

static bool token_match_and_advance(const char*& cmd, const char* token, size_t token_length) {
	if (_strnicmp(cmd, token, token_length) == 0) {
		if (isspace_or_end(cmd[token_length])) {
			cmd += token_length;
			return true;
		}
	}

	return false;
}

static bool eat_space_token_match_and_advance(const char*& cmd, const char* token, size_t token_length) {
	eat_space(cmd);

	if (_strnicmp(cmd, token, token_length) == 0) {
		if (isspace_or_end(cmd[token_length])) {
			cmd += token_length;
			return true;
		}
	}

	return false;
}

bool Nameplate::ParseCommand(const char* cmd, bool includesName) {
	if (includesName) {
		if (token_match_and_advance(cmd, "/nameplate", __builtin_strlen("/nameplate"))) {
			// skip
		} else if (token_match_and_advance(cmd, "//nameplate", __builtin_strlen("//nameplate"))) {
			// skip
		} else {
			return false;
		}
	}

	// help
	// load
	// save
	// fontsizeinpx <num>
	// damagefontsizeinpx <num>
	// hidestars
	// showstars
	// mode all
	// mode none
	// mode hideself
	// mode hidepc

	int ret = 0;

	if (eat_space_token_match_and_advance(cmd, "help", __builtin_strlen("help"))) {
		ShowMessage(MESSAGE::LONG_HELP);
	} else if (eat_space_token_match_and_advance(cmd, "load", __builtin_strlen("load"))) {
		if (eat_space_and_is_end(cmd)) {
			ret = LoadSettings();
			if (ret < 0) {
				ShowMessage(MESSAGE::LOAD_ERROR, ret);
			} else {
				Update();
			}
		} else {
			ShowMessage(MESSAGE::LOAD_COMMAND_ERROR);
		}
	} else if (eat_space_token_match_and_advance(cmd, "save", __builtin_strlen("save"))) {
		if (eat_space_and_is_end(cmd)) {
			ret = SaveSettings();
			if (ret < 0) {
				ShowMessage(MESSAGE::SAVE_ERROR, ret);
			}
		} else {
			ShowMessage(MESSAGE::SAVE_COMMAND_ERROR);
		}
	} else if (eat_space_token_match_and_advance(cmd, "fontsizeinpx", __builtin_strlen("fontsizeinpx"))
			|| eat_space_token_match_and_advance(cmd, "fontsize", __builtin_strlen("fontsize"))) {
		errno = 0;

		char* cmd_end = const_cast<char*>(cmd);

		unsigned long val = strtoul(cmd, &cmd_end, 10);

		cmd = cmd_end;

		if (val == 0 || val > 256 || errno != 0 || !eat_space_and_is_end(cmd)) {
			ShowMessage(MESSAGE::FONT_SIZE_COMMAND_ERROR);
		} else {
			Settings.SetFontSize(static_cast<int>(val));
			Settings.Flush();
			// update not necessary in this path
		}
	} else if (eat_space_token_match_and_advance(cmd, "damagefontsizeinpx", __builtin_strlen("damagefontsizeinpx"))
			|| eat_space_token_match_and_advance(cmd, "damagefontsize", __builtin_strlen("damagefontsize"))
			|| eat_space_token_match_and_advance(cmd, "dmgfontsize", __builtin_strlen("dmgfontsize"))) {
		errno = 0;

		char* cmd_end = const_cast<char*>(cmd);

		unsigned long val = strtoul(cmd, &cmd_end, 10);

		cmd = cmd_end;

		if (val == 0 || val > 256 || errno != 0 || !eat_space_and_is_end(cmd)) {
			ShowMessage(MESSAGE::DAMAGE_FONT_SIZE_COMMAND_ERROR);
		} else {
			Settings.SetDamageFontSize(static_cast<int>(val));
			Settings.Flush();
			// update not necessary in this path
		}
	} else if (eat_space_token_match_and_advance(cmd, "showstars", __builtin_strlen("showstars"))) {
		if (eat_space_and_is_end(cmd)) {
			Settings.SetHideStars(0);
			Settings.Flush();
			Update();
		} else {
			ShowMessage(MESSAGE::SHOW_STARS_COMMAND_ERROR);
		}
	} else if (eat_space_token_match_and_advance(cmd, "hidestars", __builtin_strlen("hidestars"))) {
		if (eat_space_and_is_end(cmd)) {
			Settings.SetHideStars(1);
			Settings.Flush();
			Update();
		} else {
			ShowMessage(MESSAGE::HIDE_STARS_COMMAND_ERROR);
		}
	} else if (eat_space_token_match_and_advance(cmd, "mode", __builtin_strlen("mode"))) {
		NAME_MODE NameMode = NAME_MODE::ALL;
		bool cmdError = false;

		if (eat_space_token_match_and_advance(cmd, "all", __builtin_strlen("all"))) {
			NameMode = NAME_MODE::ALL;
		} else if (eat_space_token_match_and_advance(cmd, "none", __builtin_strlen("none"))) {
			NameMode = NAME_MODE::NONE;
		} else if (eat_space_token_match_and_advance(cmd, "hideself", __builtin_strlen("hideself"))) {
			NameMode = NAME_MODE::HIDE_SELF;
		} else if (eat_space_token_match_and_advance(cmd, "hidepc", __builtin_strlen("hidepc"))) {
			NameMode = NAME_MODE::HIDE_PC;
		} else if (eat_space_token_match_and_advance(cmd, "hidepcself", __builtin_strlen("hidepcself"))) {
			NameMode = NAME_MODE::HIDE_PCSELF;
		} else if (eat_space_token_match_and_advance(cmd, "hidenpc", __builtin_strlen("hidenpc"))) {
			NameMode = NAME_MODE::HIDE_NPC;
		} else if (eat_space_token_match_and_advance(cmd, "hidenpcself", __builtin_strlen("hidenpcself"))) {
			NameMode = NAME_MODE::HIDE_NPCSELF;
		} else {
			cmdError = true;
		}

		if (!cmdError && eat_space_and_is_end(cmd)) {
			Settings.SetNameMode(NameMode);
			Settings.Flush();
			// update not necessary in this path
		} else {
			ShowMessage(MESSAGE::MODE_COMMAND_ERROR);
		}
	} else {
		ShowMessage(MESSAGE::SHORT_HELP);
	}

	return true;
}

void Nameplate::Debug(const wchar_t* text) {
	char buf[1024];
	StringCbPrintfA(buf, sizeof(buf), "%ls", text);
	Debug(buf);
}

int Nameplate::Init() {
	ZeroMemory(&Globals, sizeof(Globals));

	// Configuration setup
	Settings.Init();
	LoadSettings();

	// GetFrameDelta
	uintptr_t pGetFrameDelta = (uintptr_t) getFrameDeltaSig.Find();
	if (pGetFrameDelta == 0) {
		return -4;
	}

	// offset into instruction accessing gGame
	uintptr_t pppObj = pGetFrameDelta + 2;

	// pointer to gGame
	uintptr_t ppObj = lsb32_i((uint8_t*) pppObj, 0);
	if (ppObj == 0) {
		return -5;
	}

	// note - some of these tests no longer make any sense, as the safe reads were removed

	// InitApplicationDefaults (for lack of a better name)
	uintptr_t pInitApplicationDefaults = (uintptr_t) initApplicationDefaultsSig.Find();
	if (pInitApplicationDefaults == 0) {
		return -6;
	}

	uintptr_t ppWindowWidth = pInitApplicationDefaults + 31;
	uintptr_t ppWindowHeight = pInitApplicationDefaults + 41;

	// pWindowWidth
	uintptr_t pWindowWidth = lsb32_i((uint8_t*) ppWindowWidth, 0);
	if (pWindowWidth == 0) {
		return -7;
	}

	// pWindowHeight
	uintptr_t pWindowHeight = lsb32_i((uint8_t*) ppWindowHeight, 0);
	if (pWindowHeight == 0) {
		return -8;
	}

	// GetPlayerEntity
	uintptr_t pGetPlayerEntity = (uintptr_t) getPlayerEntitySig.Find();
	if (pGetPlayerEntity == 0) {
		return -15;
	}

	uintptr_t ppPlayerEntityIndex = pGetPlayerEntity + 2;
	uintptr_t pPlayerEntityIndex = lsb32_i((uint8_t*) ppPlayerEntityIndex, 0);
	if (pPlayerEntityIndex == 0) {
		return -16;
	}

	// DrawName, patch location 0
	uintptr_t pDrawNamePatch0 = (uintptr_t) drawNamePatch0LocationSig.Find();
	if (pDrawNamePatch0 == 0) {
		return -9;
	}

	// DrawName, patch location 1 (useless mastery stars)
	uintptr_t pDrawNamePatch1 = (uintptr_t) drawNamePatch1LocationSig.Find();
	if (pDrawNamePatch1 == 0) {
		return -10;
	}

	// DrawName2 (mastery stars)
	uintptr_t pDrawName2Patch = (uintptr_t) drawName2PatchLocationSig.Find();
	if (pDrawName2Patch == 0) {
		return -11;
	}

	// DrawName3 (display mode)
	// either use the original or the new version, depending on what's found
	uintptr_t pDrawName3Patch = (uintptr_t) drawName3PatchLocationSig.Find();
	uintptr_t pDrawName3V2Patch = (uintptr_t) drawName3V2PatchLocationSig.Find();
	if (pDrawName3Patch == 0 && pDrawName3V2Patch == 0) {
		return -14;
	}

	uintptr_t ppNamesOff = (pDrawName3Patch != 0)
		? pDrawName3Patch + 1
		: pDrawName3V2Patch + 1;
	uintptr_t pNamesOff = lsb32_i((uint8_t*) ppNamesOff, 0);
	if (pNamesOff == 0) {
		return -17;
	}

	// DrawDamage
	uintptr_t pDrawDamagePatch = (uintptr_t) drawDamagePatchLocationSig.Find();
	if (pDrawDamagePatch == 0) {
		return -18;
	}

	uint8_t* pLoc;

	CopyMemory(&Globals.DrawNamePatch0Orig[0], reinterpret_cast<const void*> (pDrawNamePatch0), DRAWNAME_PATCH0_SIZE);
	CopyMemory(reinterpret_cast<void*> (pDrawNamePatch0), &DrawName_Patch0[0], DRAWNAME_PATCH0_SIZE);

	pLoc = reinterpret_cast<uint8_t*> (pDrawNamePatch0 + DRAWNAME_PATCH0_ADDRESS_LOCATION);
	pLoc[0] = (((uintptr_t) GetNameplateScale) >> 0) & 0xFF;
	pLoc[1] = (((uintptr_t) GetNameplateScale) >> 8) & 0xFF;
	pLoc[2] = (((uintptr_t) GetNameplateScale) >> 16) & 0xFF;
	pLoc[3] = (((uintptr_t) GetNameplateScale) >> 24) & 0xFF;

	CopyMemory(&Globals.DrawNamePatch1Orig[0], reinterpret_cast<const void*> (pDrawNamePatch1), DRAWNAME_PATCH1_SIZE);
	CopyMemory(reinterpret_cast<void*> (pDrawNamePatch1), &DrawName_Patch1[0], DRAWNAME_PATCH1_SIZE);

	pLoc = reinterpret_cast<uint8_t*> (pDrawNamePatch1 + DRAWNAME_PATCH1_ADDRESS_LOCATION);
	pLoc[0] = (((uintptr_t) GetNameplateScale) >> 0) & 0xFF;
	pLoc[1] = (((uintptr_t) GetNameplateScale) >> 8) & 0xFF;
	pLoc[2] = (((uintptr_t) GetNameplateScale) >> 16) & 0xFF;
	pLoc[3] = (((uintptr_t) GetNameplateScale) >> 24) & 0xFF;

	CopyMemory(&Globals.DrawName2PatchOrig[0], reinterpret_cast<const void*> (pDrawName2Patch), DRAWNAME2_PATCH_SIZE);

	pLoc = reinterpret_cast<uint8_t*> (pDrawName2Patch + DRAWNAME2_PATCH_ADDRESS_LOCATION);

	// Stick a JMP there.
	pLoc[0] = (AsmSettings.HideStars != 0) ? 0xEB : 0x75;

	if (pDrawName3Patch != 0) {
		CopyMemory(&Globals.DrawName3PatchOrig[0], reinterpret_cast<const void*> (pDrawName3Patch), DRAWNAME3_PATCH_SIZE);
		CopyMemory(reinterpret_cast<void*> (pDrawName3Patch), &DrawName3_Patch, DRAWNAME3_PATCH_SIZE);

		pLoc = reinterpret_cast<uint8_t*> (pDrawName3Patch + DRAWNAME3_PATCH_ADDRESS_LOCATION);
		pLoc[0] = (((uintptr_t) ShouldDrawNameplate) >> 0) & 0xFF;
		pLoc[1] = (((uintptr_t) ShouldDrawNameplate) >> 8) & 0xFF;
		pLoc[2] = (((uintptr_t) ShouldDrawNameplate) >> 16) & 0xFF;
		pLoc[3] = (((uintptr_t) ShouldDrawNameplate) >> 24) & 0xFF;
	}

	if (pDrawName3V2Patch != 0) {
		CopyMemory(&Globals.DrawName3V2PatchOrig[0], reinterpret_cast<const void*> (pDrawName3V2Patch), DRAWNAME3_V2_PATCH_SIZE);
		CopyMemory(reinterpret_cast<void*> (pDrawName3V2Patch), &DrawName3_V2_Patch, DRAWNAME3_V2_PATCH_SIZE);

		pLoc = reinterpret_cast<uint8_t*> (pDrawName3V2Patch + DRAWNAME3_V2_PATCH_ADDRESS_LOCATION);
		pLoc[0] = (((uintptr_t) ShouldDrawNameplate) >> 0) & 0xFF;
		pLoc[1] = (((uintptr_t) ShouldDrawNameplate) >> 8) & 0xFF;
		pLoc[2] = (((uintptr_t) ShouldDrawNameplate) >> 16) & 0xFF;
		pLoc[3] = (((uintptr_t) ShouldDrawNameplate) >> 24) & 0xFF;
	}

	CopyMemory(&Globals.DrawDamagePatchOrig[0], reinterpret_cast<const void*> (pDrawDamagePatch), DRAWDAMAGE_PATCH_SIZE);
	CopyMemory(reinterpret_cast<void*> (pDrawDamagePatch), &DrawDamage_Patch, DRAWDAMAGE_PATCH_SIZE);

	pLoc = reinterpret_cast<uint8_t*> (pDrawDamagePatch + DRAWDAMAGE_PATCH_CALL_ADDRESS_LOCATION);
	pLoc[0] = (((uintptr_t) GetDamageNumberScale) >> 0) & 0xFF;
	pLoc[1] = (((uintptr_t) GetDamageNumberScale) >> 8) & 0xFF;
	pLoc[2] = (((uintptr_t) GetDamageNumberScale) >> 16) & 0xFF;
	pLoc[3] = (((uintptr_t) GetDamageNumberScale) >> 24) & 0xFF;

	pLoc = reinterpret_cast<uint8_t*> (pDrawDamagePatch + DRAWDAMAGE_PATCH_COLOR_ADDRESS_LOCATION);
	const uint8_t* pOldLoc = &Globals.DrawDamagePatchOrig[DRAWDAMAGE_PATCH_OLD_COLOR_ADDRESS_LOCATION];
	pLoc[0] = pOldLoc[0];
	pLoc[1] = pOldLoc[1];
	pLoc[2] = pOldLoc[2];
	pLoc[3] = pOldLoc[3];

	Globals.pgGame = reinterpret_cast<CGame**> (ppObj);
	Globals.pWindowWidth = reinterpret_cast<uint32_t*> (pWindowWidth);
	Globals.pWindowHeight = reinterpret_cast<uint32_t*> (pWindowHeight);
	Globals.pPlayerEntityIndex = reinterpret_cast<uint16_t*> (pPlayerEntityIndex);
	Globals.pNamesOff = reinterpret_cast<bool*> (pNamesOff);

	Globals.DrawNamePatch0Addr = pDrawNamePatch0;
	Globals.DrawNamePatch1Addr = pDrawNamePatch1;
	Globals.DrawName2PatchAddr = pDrawName2Patch;
	Globals.DrawName3PatchAddr = pDrawName3Patch;
	Globals.DrawName3V2PatchAddr = pDrawName3V2Patch;
	Globals.DrawDamagePatchAddr = pDrawDamagePatch;

	return 0;
}

int Nameplate::Update() {
	if (Globals.DrawName2PatchAddr != 0) {
		uint8_t* pLoc;

		uintptr_t pDrawName2Patch = Globals.DrawName2PatchAddr;
		pLoc = reinterpret_cast<uint8_t*> (pDrawName2Patch + DRAWNAME2_PATCH_ADDRESS_LOCATION);

		// Stick a JMP there.
		pLoc[0] = (AsmSettings.HideStars != 0) ? 0xEB : 0x75;
	}

	return 0;
}

int Nameplate::LoadSettings() {

	wchar_t basePath[UNICODE_MAX_PATH];
	bool pathOk = this->GetConfigPath(basePath);

	int ret;

	if (pathOk) {
		ret = Settings.Load(basePath);
		if (ret < 0) {
			return ret;
		}
		ret = Settings.Save(basePath);
		if (ret < 0) {
			return ret;
		}
	} else {
		return -12;
	}

	return 0;
}

int Nameplate::SaveSettings() {
	wchar_t basePath[UNICODE_MAX_PATH];
	bool pathOk = this->GetConfigPath(basePath);

	int ret;

	if (pathOk) {
		ret = Settings.Save(basePath);
		if (ret < 0) {
			return ret;
		}
	} else {
		return -13;
	}

	return 0;
}

int Nameplate::Deinit() {

	if (Globals.DrawDamagePatchAddr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawDamagePatchAddr), &Globals.DrawDamagePatchOrig[0], DRAWDAMAGE_PATCH_SIZE);
	}

	if (Globals.DrawName3V2PatchAddr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawName3V2PatchAddr), &Globals.DrawName3V2PatchOrig[0], DRAWNAME3_V2_PATCH_SIZE);
	}

	if (Globals.DrawName3PatchAddr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawName3PatchAddr), &Globals.DrawName3PatchOrig[0], DRAWNAME3_PATCH_SIZE);
	}

	if (Globals.DrawName2PatchAddr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawName2PatchAddr), &Globals.DrawName2PatchOrig[0], DRAWNAME2_PATCH_SIZE);
	}

	if (Globals.DrawNamePatch1Addr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawNamePatch1Addr), &Globals.DrawNamePatch1Orig[0], DRAWNAME_PATCH1_SIZE);
	}

	if (Globals.DrawNamePatch0Addr != 0) {
		CopyMemory(reinterpret_cast<void*> (Globals.DrawNamePatch0Addr), &Globals.DrawNamePatch0Orig[0], DRAWNAME_PATCH0_SIZE);
	}

	ZeroMemory(&Globals, sizeof(Globals));

    return 0;
}
