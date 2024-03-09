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

#include "Windozer.h"

#include "../Nameplate.h"


class WindozerNameplate : public PluginBase, private Nameplate {
    public:
        virtual void __stdcall Load(PluginManager* _manager) override {
            pPluginManager = _manager;

            int ret = this->Init();

			if (ret < 0) {
		        ShowMessage(MESSAGE::PLUGIN_LOAD_ERROR, ret);
			}
        }

        virtual void __stdcall Unload() override {
            int ret = this->Deinit();

			if (ret < 0) {
				ShowMessage(MESSAGE::PLUGIN_UNLOAD_ERROR, ret);
			}
        }
        virtual PluginMetadata* __stdcall GetMetadata(PluginMetadata* metadata) override;

        virtual void __stdcall PluginCommand(const char* cmd, const char* name) override;

		virtual void Debug(const char* text) override {
			pPluginManager->GetConsole()->Write(text);
		}

		virtual bool GetConfigPath(wchar_t path[UNICODE_MAX_PATH]) override {
			MMFSettingsHandler settings;
			pPluginManager->GetMMFSettingsHandler(&settings);

			StringCchPrintfW(path, UNICODE_MAX_PATH, L"%s\\plugins\\settings", settings.WindowerPath);
			return true;
		}

		virtual void ShowMessage(MESSAGE message, int param) override;
};

PluginMetadata* __stdcall WindozerNameplate::GetMetadata(PluginMetadata* metadata) {
    StringCbCopyA(metadata->Author, sizeof(metadata->Author), "BunnyBox Productions");
    StringCbCopyA(metadata->Name, sizeof(metadata->Name), "Nameplate");
    return metadata;
}

void __stdcall WindozerNameplate::PluginCommand(const char* cmd, [[maybe_unused]] const char* name) {
	ParseCommand(cmd, false);
}

typedef void (*ShowMessageCallback)(Console*, MESSAGE, int);

static void WindozerPluginLoadError(Console* console, [[maybe_unused]] MESSAGE message, int param) {
	char buf[1000];

	StringCbPrintfA(buf, sizeof(buf), "\\cs(255,192,255)[Nameplate] \\cs(255,128,128)Plugin load failed with error code \\cs(255,128,128)%d\\cs(255,128,255)!\\cr", param);
    console->Write(buf);
}

static void WindozerPluginUnloadError(Console* console, [[maybe_unused]] MESSAGE message, int param) {
	char buf[1000];

	StringCbPrintfA(buf, sizeof(buf), "\\cs(255,192,255)[Nameplate] \\cs(255,128,128)Plugin unload failed with error code \\cs(255,128,128)%d\\cs(255,128,255)!\\cr", param);
    console->Write(buf);
}

static void WindozerShortHelp(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,128,128)Unknown command, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerLongHelp(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
    console->Write("\\cs(255,192,255)Nameplate v0.5\\cr");
    console->Write("\\cs(255,192,255)https://www.github.com/Shirk/Nameplate\\cr");
    console->Write("\\cs(255,255,192)Usage:\\cr");
    console->Write("\\cs(255,255,192)//nameplate help - \\cs(255,255,255)You're reading it!\\cr");
    console->Write("\\cs(255,255,192)//nameplate load - \\cs(255,255,255)Load configuration from\\cs(255,255,192) config\\nameplate\\defaults.ini\\cr");
    console->Write("\\cs(255,255,192)//nameplate save - \\cs(255,255,255)Save current configuration to\\cs(255,255,192) config\\nameplate\\defaults.ini\\cr");
    console->Write("\\cs(255,255,192)//nameplate fontsize - \\cs(255,225,192) <number>\t\\cs(255,255,255)Set the nameplate font size to <number> pixels\\cr");
    console->Write("\\cs(255,255,192)//nameplate damagefontsize - \\cs(255,225,192) <number>\t\\cs(255,255,255)Set the damage font size to <number> pixels\\cr");
    console->Write("\\cs(255,255,192)//nameplate hidestars - \\cs(255,255,255)Hide all Job Mastery stars\\cr");
    console->Write("\\cs(255,255,192)//nameplate showstars - \\cs(255,255,255)Re-enable displaying Job Mastery stars\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode all - \\cs(255,255,255)Show all nameplates\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode none - \\cs(255,255,255)Hide all nameplates\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode hideself - \\cs(255,255,255)Hide your own nameplate\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode hidepc - \\cs(255,255,255)Hide all player nameplates, except when charmed\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode hidepcself - \\cs(255,255,255)Hide all player nameplates, except when charmed, but also keep yours always hidden\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode hidenpc - \\cs(255,255,255)Hide all non-player nameplates\\cr");
    console->Write("\\cs(255,255,192)//nameplate mode hidenpcself - \\cs(255,255,255)Hide all non-player nameplates, but also keep yours always hidden\\cr");
}

static void WindozerLoadError(Console* console, [[maybe_unused]] MESSAGE message, int param) {
	char buf[1000];

	StringCbPrintfA(buf, sizeof(buf), "\\cs(255,192,255)[Nameplate] \\cs(255,128,128)Loading \\cs(255,255,192)plugins\\settings\\nameplate\\defaults.ini \\cs(255,128,128)failed with error code \\cs(255,128,128)%d\\cs(255,128,255)!\\cr", param);
    console->Write(buf);
}

static void WindozerSaveError(Console* console, [[maybe_unused]] MESSAGE message, int param) {
	char buf[1000];

	StringCbPrintfA(buf, sizeof(buf), "\\cs(255,192,255)[Nameplate] \\cs(255,128,128)Save \\cs(255,255,192)plugins\\settings\\nameplate\\defaults.ini \\cs(255,128,128)failed with error code \\cs(255,128,128)%d\\cs(255,128,255)!\\cr", param);
    console->Write(buf);
}

static void WindozerLoadCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate load \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");

}

static void WindozerSaveCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate save \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerFontSizeCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate fontsize \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerDamageFontSizeCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate damagefontsize \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerShowStarsCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate showstars \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerHideStarsCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate hidestars \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static void WindozerModeCommandError(Console* console, [[maybe_unused]] MESSAGE message, [[maybe_unused]] int param) {
	console->Write("\\cs(255,192,255)[Nameplate] \\cs(255,255,192)//nameplate mode \\cs(255,128,128)command error, please consult \\cs(255,255,192)//nameplate help \\cs(255,128,128) for more information.\\cr");
}

static constexpr ShowMessageCallback WindozerMessages[/*MESSAGE::MESSAGE_MAX*/] = {
    /*[MESSAGE::PLUGIN_LOAD_ERROR]              = */ WindozerPluginLoadError,
    /*[MESSAGE::PLUGIN_UNLOAD_ERROR]            = */ WindozerPluginUnloadError,
    /*[MESSAGE::SHORT_HELP]                     = */ WindozerShortHelp,
    /*[MESSAGE::LONG_HELP]                      = */ WindozerLongHelp,
    /*[MESSAGE::LOAD_ERROR]                     = */ WindozerLoadError,
    /*[MESSAGE::LOAD_COMMAND_ERROR]             = */ WindozerLoadCommandError,
    /*[MESSAGE::SAVE_ERROR]                     = */ WindozerSaveError,
    /*[MESSAGE::SAVE_COMMAND_ERROR]             = */ WindozerSaveCommandError,
    /*[MESSAGE::FONT_SIZE_COMMAND_ERROR]        = */ WindozerFontSizeCommandError,
    /*[MESSAGE::DAMAGE_FONT_SIZE_COMMAND_ERROR] = */ WindozerDamageFontSizeCommandError,
    /*[MESSAGE::SHOW_STARS_COMMAND_ERROR]       = */ WindozerShowStarsCommandError,
    /*[MESSAGE::HIDE_STARS_COMMAND_ERROR]       = */ WindozerHideStarsCommandError,
    /*[MESSAGE::MODE_COMMAND_ERROR]             = */ WindozerModeCommandError,
};

void WindozerNameplate::ShowMessage(MESSAGE message, int param) {
    if ((uint32_t) message < (uint32_t) MESSAGE::MESSAGE_MAX && WindozerMessages[(uint32_t) message] != nullptr) {
        WindozerMessages[(uint32_t) message](pPluginManager->GetConsole(), message, param);
    } else {
		char buf[1000];
		StringCbPrintfA(buf, sizeof(buf), "[Nameplate] Unknown message %u (%d)\\cr", (uint32_t) message, param);
        pPluginManager->GetConsole()->Write(buf);
    }
}

uint32_t GetInterfaceVersion() {
    return WINDOZER_INTERFACE_VERSION;
}

PluginBase* CreateInstance() {
    return new WindozerNameplate();
}
