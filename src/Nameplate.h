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

#pragma once

#include <stdint.h>

#define UNICODE_MAX_PATH 32768

enum class NAME_MODE {
    ALL,
    NONE,
    HIDE_SELF,
    HIDE_PC,
    HIDE_PCSELF,
    HIDE_NPC,
    HIDE_NPCSELF,

    MIN = ALL,
    MAX = HIDE_NPCSELF,
};

enum class MESSAGE : uint32_t {
    PLUGIN_LOAD_ERROR,
    PLUGIN_UNLOAD_ERROR,

    SHORT_HELP,
    LONG_HELP,
    LOAD_ERROR,
    LOAD_COMMAND_ERROR,
    SAVE_ERROR,
    SAVE_COMMAND_ERROR,
    FONT_SIZE_COMMAND_ERROR,
    DAMAGE_FONT_SIZE_COMMAND_ERROR,
    SHOW_STARS_COMMAND_ERROR,
    HIDE_STARS_COMMAND_ERROR,
    MODE_COMMAND_ERROR,

    MESSAGE_MAX,
};

class NameplateSettings {
    friend class Nameplate;
    public:
        NameplateSettings() :
            FontSize(0),
            DamageFontSize(0),
            HideStars(0),
            NameMode(NAME_MODE::ALL) {}

        int Init();
        int Load(const wchar_t* basePath);
        int Save(const wchar_t* basePath);

        void SetFontSize(int fontSize) {
            FontSize = fontSize;
        }
        void SetDamageFontSize(int fontSize) {
            DamageFontSize = fontSize;
        }
        void SetHideStars(int hideStars) {
            HideStars = hideStars;
        }
        void SetNameMode(NAME_MODE nameMode) {
            NameMode = nameMode;
        }

    private:
        void Flush();
        void Normalize();

        int FontSize;
        int DamageFontSize;
        int HideStars;
        NAME_MODE NameMode;
};

class Nameplate {
    public:
        virtual void Debug(const char* text) = 0;
        virtual void Debug(const wchar_t* text);

        virtual void ShowMessage(MESSAGE message, int param = 0) = 0;
        virtual bool GetConfigPath(wchar_t path[UNICODE_MAX_PATH]) = 0;

        int Init();
        int Deinit();

        int LoadSettings();
        int SaveSettings();
        int Update();

        bool ParseCommand(const char* cmd, bool includesName);

    private:
        NameplateSettings Settings;
};
