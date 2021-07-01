// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <dinput.h>
#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <cstring>

namespace fs = std::filesystem;

/** Modules handles */
static HINSTANCE moon = NULL;
static HINSTANCE dinput8 = NULL;
static std::map<std::string, HINSTANCE> mods;

static void load_dinput8() noexcept;
static void load_mods() noexcept;
static void unload_mods() noexcept;

WINAPI BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH: {
            // Set instance pointer
            moon = hinstDLL;

            // Load system DInput8.dll
            load_dinput8();

            // Load DLLs from mods directory
            load_mods();

            break;
        }
        case DLL_PROCESS_DETACH: {
            if(dinput8 != NULL) {
                unload_mods();
                FreeLibrary(dinput8);
            }
            
            break;
        }
   }

   return TRUE;
}

static void load_dinput8() noexcept {
    // Get system DInput8.dll path
    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat(path, "\\DInput8.dll");

    // Load it!
    dinput8 = LoadLibraryA(path);

    // Check if DInput8.dll was loaded successfully
    if(!dinput8) {
        MessageBoxA(NULL, "Cannot load system DInput8.dll", "Moon", MB_OK);
        std::exit(1);
    }
}

static void load_mods() noexcept {
    // Get mods directory
    char dir[MAX_PATH];
    GetModuleFileName(moon, dir, MAX_PATH);
    auto mods_path = fs::path(dir).parent_path() / "mods";

    if(!fs::exists(mods_path)) {
        return;
    }

    // Load everything in the mods folder
    for(auto &dll : fs::directory_iterator(mods_path)) {
        auto dll_path = dll.path();
        if(dll_path.extension() == ".dll") {
            std::string filename = dll_path.filename().string();
            auto *handle = LoadLibraryW(dll_path.c_str());
            if(handle) {
                mods.insert({filename, handle});
                std::cout << "[MOON] Loaded " << filename << std::endl;
            }
            else {
                std::cerr << "[MOON] Failed to load " << filename << std::endl;
            }
        }
    }

    std::cout << "[Moon] Loaded " << std::to_string(mods.size()) << " mods" << std::endl;
}

static void unload_mods() noexcept {
    auto it = mods.begin();
    while(it != mods.end()) {
        auto name = it->first;
        auto *handle = it->second;
        if(handle) {
            FreeLibrary(handle);
            std::cout << "[MOON] Unloaded " << name << std::endl;
        }
        it = mods.erase(it);
    }
}

extern "C" {
    // Call a given DInput8.dll function
    #define CALLPROC(function, ...) reinterpret_cast<decltype(function) *>(GetProcAddress(dinput8, #function))(__VA_ARGS__)

    WINAPI HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, IID const &riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) {
        return CALLPROC(DirectInput8Create, hinst, dwVersion, riidltf, ppvOut, punkOuter);
    }

    WINAPI HRESULT DllCanUnloadNow() {
        return CALLPROC(DllCanUnloadNow);
    }

    WINAPI HRESULT DllGetClassObject(IID const &rclsid, IID const &riid, LPVOID *ppv) {
        return CALLPROC(DllGetClassObject, rclsid, riid, ppv);
    }

    WINAPI HRESULT DllRegisterServer() {
        return CALLPROC(DllRegisterServer);
    }

    WINAPI HRESULT DllUnregisterServer() {
        return CALLPROC(DllUnregisterServer);
    }

    WINAPI HRESULT GetdfDIJoystick() {
        return CALLPROC(GetdfDIJoystick);
    }
}
