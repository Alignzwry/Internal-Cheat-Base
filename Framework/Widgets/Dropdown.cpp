#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;


Dropdown::Dropdown(std::string label, int* selected, std::vector<std::string>* options) {
	this->pSelected = selected;
	this->pOptions = options;
	this->pLabel = label;
}

float Dropdown::Draw(float x, float y, bool hovered, bool opened) {
	// Positions
	lastY = y;

	lastW = 80.f * pData.pWScale;
	lastH = 30.f * pData.pHScale;
	lastX = pData.xpos + pData.width - pData.borderSpace;

	// Draw
	pImDrawList->AddRectFilled(ImVec2(lastX - lastW, y), ImVec2(lastX, y + lastH), convColorTab(pTheme.accent2), 5.f);
	pImDrawList->AddRect(ImVec2(lastX - lastW, y), ImVec2(lastX, y + lastH), convColorTab(ImColor(1.f, 1.f, 1.f, 0.45f)), 5.f);

	// Text
	pImDrawList->AddText(ImVec2(x, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), pLabel.c_str());

	ImVec2 textSize = ImGui::CalcTextSize(pOptions->at(*pSelected).c_str());

	pImDrawList->AddText(ImVec2(lastX - lastW + (lastW - textSize.x) / 2, y + (lastH - textSize.y) / 2), convColorTab(pTheme.text), pOptions->at(*pSelected).c_str());

	animation = Lerp(animation, opened ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	return lastH;
}

// Draw pop up
void Dropdown::DrawFull() {
	float rh = 24.f * pData.pHScale;
	float size = rh * pOptions->size() * animation;

	// Pop up
	pImDrawList->AddRectFilled(ImVec2(lastX - lastW, lastY + lastH), ImVec2(lastX, lastY + lastH + size), convColorTab(pTheme.accent2, animation), 8.f);

	pImDrawList->PushClipRect(ImVec2(lastX - lastW, lastY + lastH - 1.f), ImVec2(lastX, lastY + lastH + size + 1.f));

	// Items
	for (int i = 0; i < ceil(size / rh); i++) {
		if(i == pOptions->size()) break;
		ImVec2 textSize = ImGui::CalcTextSize(pOptions->at(i).c_str());
		pImDrawList->AddText(ImVec2(lastX - lastW + (lastW - textSize.x) / 2, lastY + lastH + rh * i + (rh - pData.textSize) / 2), convColorTab(pTheme.text, animation), pOptions->at(i).c_str());
	}

	// Animation
	animation_selected = Lerp(animation_selected, *pSelected * rh, pData.pAnimationSpeed * flDeltaTime);

	// Outline
	pImDrawList->AddRectFilled(ImVec2(lastX - lastW, lastY + lastH + animation_selected), ImVec2(lastX, lastY + lastH + rh + animation_selected), convColorTab(ImColor(1.f, 1.f, 1.f, 0.45f), animation), 8.f);

	pImDrawList->PopClipRect();
}

bool Dropdown::click(float x, float y, int* openedMenu, int id) {
	if (id == *openedMenu) {
		float size = 24.f * pData.pHScale * pOptions->size() * animation;

		if (x < lastX - lastW || x > lastX || y < lastY + lastH || y > lastY + lastH + size) {
			*openedMenu = -1;
			return true;
		}

		*pSelected = (y - lastY - lastH) / (24.f * pData.pHScale);
		return true;
	}

	if(x >= lastX - lastW && x <= lastX && y >= lastY && y <= lastY + lastH) {
		*openedMenu = id;
		return true;
	}

	return false;
}

bool Dropdown::hover(float x, float y) {
	return false;
}