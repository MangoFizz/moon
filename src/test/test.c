// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>

WINAPI BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH: {
            MessageBoxA(NULL, "Loaded", "Moon test", MB_OK);
            break;
        }
        case DLL_PROCESS_DETACH: {
            break;
        }
   }

   return TRUE;
}
