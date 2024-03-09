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

#include <inttypes.h>

#define WINDOZER_INTERFACE_VERSION 0x40002

typedef uint8_t undefined;

class input_manager;
class TextHandler;
class PrimitiveHandler;
class PacketStreamHandler;
class FFXI;

struct MMFSettingsHandler {
    char PolPath[16384];
    char WindowerPath[16384];
    uint8_t field2_0x8000[8192];

    uint8_t field3[0x6000];

    float LauncherVersion;
    undefined field24580_0x10004;
    undefined field24581_0x10005;
    undefined field24582_0x10006;
    undefined field24583_0x10007;
    int XRes;
    int YRes;
    int UiXRes;
    int UiYRes;
    int WindowXPos;
    int WindowYPos;
    undefined field24590_0x10020;
    bool field24591_0x10021;
    bool IsResizeable;
    undefined field24593_0x10023;
    uint8_t field24594_0x10024[16];
    undefined field24595_0x10034;
    undefined field24596_0x10035;
    undefined field24597_0x10036;
    undefined field24598_0x10037;
    undefined field24599_0x10038;
    undefined field24600_0x10039;
    undefined field24601_0x1003a;
    undefined field24602_0x1003b;
    undefined field24603_0x1003c;
    undefined field24604_0x1003d;
    undefined field24605_0x1003e;
    undefined field24606_0x1003f;
    undefined field24607_0x10040;
    undefined field24608_0x10041;
    undefined field24609_0x10042;
    undefined field24610_0x10043;
    undefined field24611_0x10044;
    undefined field24612_0x10045;
    bool field24613_0x10046;
    undefined field24614_0x10047;
};

class Console {
    public:
        virtual bool __stdcall _00();
        virtual void __stdcall OpenConsole(bool);
        virtual bool __stdcall IsVisible();
        virtual void __stdcall SetPosition(float, float);
        virtual void __stdcall Write(const char *);
        virtual void __stdcall Clear();
        virtual void __stdcall SendCommand(bool, const char *, bool);
        virtual void __stdcall ExecScript(bool, const char *);
};

class PluginManager {
    public:
        virtual float __stdcall GetVersion();
        virtual MMFSettingsHandler* __stdcall GetMMFSettingsHandler(MMFSettingsHandler *);
        virtual void * __stdcall GetHWND();
        virtual void * __stdcall GetDirect3D8Device();
        virtual input_manager* __stdcall GetInputManager();
        virtual Console* __stdcall GetConsole();
        virtual TextHandler* __stdcall GetTextHandler();
        virtual PrimitiveHandler* __stdcall GetPrimitiveHandler();
        virtual PacketStreamHandler* __stdcall GetPacketStreamHandler();
        virtual void * __stdcall _09_Return_Zero();
        virtual FFXI * __stdcall GetFFXI();
        virtual PluginManager* __thiscall Dtor(uint8_t);
};

struct PluginMetadata {
    uint8_t Unknown0[0x100];
    char Author[0x100];
    uint8_t Unknown1[0x108];
    char Name[0x20];
};

class PluginBase {
    public:
        virtual void __stdcall Load(PluginManager* _manager) { pPluginManager = _manager; }
        virtual void __stdcall Unload() {}
        virtual void __stdcall Dealloc() {
            // if (this != nullptr) {
                this->Dtor(1);
            // }
        }
        virtual PluginMetadata* __stdcall GetMetadata(PluginMetadata* metadata) = 0;
        //{
        //    strncpy(metadata->Author, "Windozer");
        //    strncpy(metadata->Name, "pluginbase");
        //    return metadata;
        //}

        virtual void __stdcall PluginCommand(const char*, const char*) {}
        virtual bool __stdcall UnhandledCommand(const char*) { return false; }
        virtual void __stdcall PreReset() {}
        virtual void __stdcall PostReset() {}
        virtual void __stdcall PreRender() {}
        virtual void __stdcall PostRender() {}
        virtual void __stdcall IncomingText(void *, void *, void *) {}
        virtual void __stdcall OutgoingText(void *, void *, void *) {}
        virtual bool __stdcall IncomingChunk(void *, void *, void *, bool modified) { return modified; }
        virtual bool __stdcall OutgoingChunk(void *, void *, void *, bool modified) { return modified; }
        virtual void __stdcall _14(void *) {} // todo
        virtual bool __stdcall IgnoreUnload() { return false; }
        virtual void __stdcall Reset() {} // todo
        virtual bool __stdcall Mouse(void *, void *, void *, void *, bool modified) { return modified; }
        virtual bool __stdcall Keyboard(void *, void *, bool modified) { return modified; }
        virtual void __stdcall AddItem(void *, void *, void *, void *, void *) {} // todo
        virtual void __stdcall RemoveItem(void *, void *, void *, void *, void *) {} // todo
        virtual PluginBase* __thiscall Dtor(uint8_t) { return this; }

        PluginManager* GetPluginManager() const {
            return pPluginManager;
        }

    protected:
        PluginManager* pPluginManager;
};

#ifdef __cplusplus
extern "C" {
#endif

#define DllExport /*__declspec(dllexport)*/

DllExport uint32_t GetInterfaceVersion();
DllExport PluginBase* CreateInstance();

#ifdef __cplusplus
};
#endif
