#include "hooks.h"

#include "../render/render.h"
#include <detours/detours.h>

#ifdef _DEBUG
#include "../../util/Console.h"
#include <string>
#endif

#define HOOK(ppPointer, pDetour) { if (!CreateHook({ &(void*&)ppPointer, pDetour })) {\
		return false; } }

bool hooks::init() {
	// Init Detours
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());


	// Hooks ( All Offsets should already be valididated by the signature parser )
	// Render functions
	HOOK(Render::oPresent, Render::hkPresent);
	HOOK(Render::oResizeBuffers, Render::hkResizeBuffers);
	HOOK(Render::oCreateSwapChain, Render::hkCreateSwapChain);
	HOOK(Render::oTranslateMessages, Render::hkTranslateMessages);

	// Your additional hooks

	//

	// Commit
	DetourTransactionCommit();

	return true;
}

// Destroy all hooks
void hooks::destroy() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	for (auto& hook : m_vHooks) {
		DetourDetach(hook.ppPointer, hook.pDetour);
	}

	DetourTransactionCommit();

	// Clear hooks
	m_vHooks.clear();
}


// Add Hooks to list
bool hooks::CreateHook(Hook hook) {
	long res = DetourAttach(hook.ppPointer, hook.pDetour);
	if (res != NO_ERROR) {
#ifdef _DEBUG
		console::printError(("Failed to attach hook, error code: " + std::to_string(res)).c_str());
#endif
		return false;
	}
	m_vHooks.push_back(hook);

	return true;
}