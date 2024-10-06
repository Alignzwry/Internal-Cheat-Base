#include "Init.h"
#include "../util/Memory.h"
#include "../../render/render.h"
#include <detours/detours.h>

#include "hooks/hooks.h"
#include "Signatures/Signatures.h"

#ifdef _DEBUG
#include "../util/Console.h"
#endif

bool base::init() {
#ifdef _DEBUG
	console::printHeader("Loading Signatures");
#endif
	// Initialize Signatures
	if (!signatures::init()) return false;

#ifdef _DEBUG
	console::printHeader("Loading Hooks");
#endif
	// Initialize Hooks
	if (!hooks::init()) return false;

	return true;
}

void base::destroy() {
	// Uninitialize Hooks
	hooks::destroy();
}