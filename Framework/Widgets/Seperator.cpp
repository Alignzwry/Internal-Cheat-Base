#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;

float Seperator::Draw(float x, float y) {
	float pos2 = Menu::pData.xpos + Menu::pData.width - Menu::pData.borderSpace - 15.f;

	ImColor on = convColorTab(pTheme.accent2);
	ImColor off = convColorTab(pTheme.background);

	pImDrawList->AddRectFilledMultiColor(ImVec2(x + 15.f, y), ImVec2(x + 15 + (pos2 - x - 15.f) / 2, y + 2.f), off, on, on, off);

	pImDrawList->AddRectFilledMultiColor(ImVec2(x + 15.f + (pos2 - x - 15.f) / 2, y), ImVec2(pos2, y + 2.f), on, off, off, on);

	return 2.f;
}