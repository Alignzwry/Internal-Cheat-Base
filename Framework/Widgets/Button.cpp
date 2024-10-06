#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;

Button::Button(std::string label, std::string label2, ButtonFunction function) {
	this->pLabel = label;
	this->pLabel2 = label2;
	this->pFunction = function;
}

float Button::Draw(float x, float y, bool hovered, bool pressed) {
	// Init textsize
	if (!this->isInit) {
		this->textWidth = ImGui::CalcTextSize(this->pLabel2.c_str()).x;
		this->isInit = true;
	}

	// Positions
	lastY = y;

	lastW = 80.f * pData.pWScale;
	lastH = 30.f * pData.pHScale;

	// Title
	pImDrawList->AddText(ImVec2(x, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), pLabel.c_str());

	// Continue positions
	x = pData.xpos + pData.width - lastW - pData.borderSpace;

	lastX = x;

	// Animation
	animation = Lerp(animation, hovered ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	animation_hover = Lerp(animation_hover, (pressed ? 0 : hovered) ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	// Draw button
	float scale = animation_hover * (3.f * pData.pWScale);
	pImDrawList->AddRect(ImVec2(x - scale, y - scale), ImVec2(x + lastW + scale, y + lastH + scale), convColorTab(pTheme.highlight, animation_hover * 0.5f + 0.5f), lastH / 2 + scale, 0, 0.5f + animation_hover);

	// Button text
	pImDrawList->AddText(ImVec2(x + (lastW - textWidth) / 2, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), pLabel2.c_str());

	return lastH;
}

bool Button::click(float x, float y, int* pressed, int id) {
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		*pressed = id;
		pFunction();
		return true;
	}

	return false;
}

bool Button::hover(float x, float y) {
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		return true;
	}

	return false;
}