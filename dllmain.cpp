#include <Windows.h>
#include "Cheat/Cheat.h"
#include "Cheat/Init/Init.h"
#include <thread>

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        // Allocate console
#ifdef _DEBUG
        console::create();
#endif

        DisableThreadLibraryCalls(hMod);

        // Init Cheat
        if (!base::init()) {
#ifdef _DEBUG
            console::printError("Failed to Initialize!");
#endif
            system("pause");
            goto detach;
        }

#ifdef _DEBUG
        console::printHeader("Cheat Initialized!");
#endif

    } // DLL_PROCESS_ATTACH


    else if (dwReason == DLL_PROCESS_DETACH) {
    detach:
        // Clear and close console
#ifdef _DEBUG
        console::destroy();
#endif

        // Detach
        Cheat::Data::Unload();
    } // DLL_PROCESS_DETACH

    return TRUE;
}