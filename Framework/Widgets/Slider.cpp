#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;

Slider::Slider(std::string label, float* value, float min, float max, std::string format) {
	this->pLabel = label;
	this->pValue = value;
	this->pFormat = format;
	this->min = min;
	this->max = max;
}

float Slider::Draw(float x, float y, bool hovered) {
	if (!isInit) {
		isInit = true;
		char buf[14];
		sprintf_s(buf, pFormat.c_str(), max);
		textWidth = ImGui::CalcTextSize(buf).x;
		labelTextWidth = ImGui::CalcTextSize(pLabel.c_str()).x;
	}

	// Positions
	lastY = y;

	lastW = default_width * pData.pWScale;
	lastH = default_height * pData.pHScale;

	// Title
	pImDrawList->AddText(ImVec2(x, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), pLabel.c_str());

	// Animations
	animation = Lerp(animation, (*pValue - min) / (max - min), pData.pAnimationSpeed * flDeltaTime);
	if(animation > 1.f) animation = 1.f; // Sanity check
	if(animation < 0.f) animation = 0.f;

	animation_hover = Lerp(animation_hover, hovered ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	// Text
	char buf[14];
	sprintf_s(buf, pFormat.c_str(), *pValue);

	float textWidth2 = ImGui::CalcTextSize(buf).x;

	pImDrawList->AddText(ImVec2(pData.xpos + pData.width - pData.borderSpace - textWidth2, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), buf);

	// Slider
	lastX = pData.xpos + pData.width - pData.borderSpace - (textWidth > 65.f ? textWidth + 10.f : 65.f) - lastW;

	if (x + labelTextWidth + 5.f > lastX) {
		lastW -= (x + labelTextWidth + 5.f - lastX);
		lastX = x + labelTextWidth + 5.f;
	}

	pImDrawList->AddRectFilled(ImVec2(lastX, y + lastH / 2 - 2.f), ImVec2(lastX + lastW, y + lastH / 2 + 2.f), convColorTab(pTheme.accent2), 2.f);

	pImDrawList->AddRectFilled(ImVec2(lastX, y + lastH / 2 - 2.f), ImVec2(lastX + lastW * animation, y + lastH / 2 + 2.f), convColorTab(interpolateColors(pTheme.highlight, pTheme.accent, animation_hover)), 2.f);

	pImDrawList->AddCircleFilled(ImVec2(lastX + lastW * animation, y + lastH / 2), (lastH / 2) * animation_hover, convColorTab(pTheme.highlight, animation_hover));

	if((lastH / 2) * animation_hover > 2.f)
		pImDrawList->AddShadowCircle(ImVec2(lastX + lastW * animation, y + lastH / 2), (lastH / 2) * animation_hover, convColorTab(pTheme.highlight, animation_hover * 0.75f), 15.f, ImVec2(0,0));

	return lastH;
}

bool Slider::click(float x, float y) {
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		*pValue = (x - lastX) / lastW * (max - min) + min;
		return true;
	}
	return false;
}

bool Slider::hover(float x, float y, bool moving) {
	if (moving) {
		*pValue = (x - lastX) / lastW * (max - min) + min;
		if (*pValue < min) {
			*pValue = min;
		}
		if (*pValue > max) {
			*pValue = max;
		}
		return true;
	}
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		return true;
	}
	return false;
}