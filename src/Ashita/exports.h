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

class IPlugin;

#ifdef __cplusplus
extern "C" {
#endif

#define DllExport /*__declspec(dllexport)*/

DllExport double __stdcall expGetInterfaceVersion(void);

DllExport IPlugin* __stdcall expCreatePlugin(const char* args);

#ifdef __cplusplus
};
#endif
