/**
 * This file is part of Special K.
 *
 * Special K is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Special K is distributed in the hope that it will be useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Special K.
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

#ifndef __SK__OSD_H__
#define __SK__OSD_H__

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdint.h>

LPVOID __stdcall SK_GetSharedMemory     (void);
BOOL   __stdcall SK_ReleaseSharedMemory (LPVOID pMemory);

void __stdcall SK_InstallOSD       (void);
BOOL __stdcall SK_DrawOSD          (void);
BOOL __stdcall SK_UpdateOSD        (LPCSTR lpText, LPVOID pMapAddr = nullptr, LPCSTR lpAppName = nullptr);
void __stdcall SK_ReleaseOSD       (void);

void __stdcall SK_SetOSDPos        (int x,   int y,                           LPCSTR lpAppName = nullptr);

// Any value out of range: [0,255] means IGNORE that color
void __stdcall SK_SetOSDColor      (int red, int green, int blue,             LPCSTR lpAppName = nullptr);

void __stdcall SK_SetOSDScale      (DWORD dwScale, bool relative = false,     LPCSTR lpAppName = nullptr);
void __stdcall SK_ResizeOSD        (int scale_incr,                           LPCSTR lpAppName = nullptr);

#endif /* __SK__OSD_H__ */