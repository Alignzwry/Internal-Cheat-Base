#include "Aimbot.h"

///////////////////////////////////////////////////////////
////////////////////////    Aimbot    /////////////////////
///////////////////////////////////////////////////////////

bool hasTarget = false;
float closest = 0.f;

void Features::Aimbot::begin() noexcept {
	// Run aimbot (setting angles)
	closest = fov;
	hasTarget = false;
}

void Features::Aimbot::validateTarget(void* target) noexcept {
	// Check if target is valid and closer to currently closest
}


void Features::Aimbot::end() noexcept {
	if(!hasTarget || !enabled) return;

	// Set angles
}