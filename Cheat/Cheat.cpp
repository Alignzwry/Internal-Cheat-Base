#include "Cheat.h"
#include "Features/Features.h"

void Cheat::update() noexcept {
    // Clear aimbot
    Features::Aimbot::begin();

    for (int i = 0; i < 64; i++) {
        // Get target
        void* target = 0;
        if (!target)
            continue;

        // Aimbot
        Features::Aimbot::validateTarget(target);

        // Esp
        Features::Esp::update(target);
    }

    // Finish
    Features::Aimbot::end();
}

#include "Init/Init.h"
void Cheat::Data::Unload() {
    // Destroy Hooks
    base::destroy();

    // Free up
    CloseHandle(hModule);
    FreeLibrary(hModule);
}