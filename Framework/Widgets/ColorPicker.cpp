#include "Widgets.h"
#include "../Menu.h"
#include <Windows.h>
#include <global.h>

using namespace Menu;

ColorPicker::ColorPicker(std::string label, ImColor* value, float size, bool hasAlpha, bool* value2) {
	this->pLabel = label;
	this->pValue = value;
	this->size = size;
	this->hasAlpha = hasAlpha;
	this->pValue2 = value2;
	this->hasCheckbox = value2 != nullptr;
	loadFromColor(*value);
}


float ColorPicker::Draw(float x, float y, bool hovered, bool opened) {
	// Update color
	loadFromHSV();

	// Positions
	lastY = y;

	lastW = 74.f * pData.pWScale;
	lastH = 30.f * pData.pHScale;

	lastX2 = pData.xpos + pData.width - pData.borderSpace;
	lastY2 = y;

	// Animation
	animation = Lerp(animation, opened ? 1.0f : 0.0f, pData.pAnimationSpeed * flDeltaTime);

	// Main
	pImDrawList->AddRectFilled(ImVec2(lastX2 - lastW, y), ImVec2(lastX2, y + lastH), convColorTab(ImColor(pValue->Value.x, pValue->Value.y, pValue->Value.z)), 5.f);
	pImDrawList->AddShadowRect(ImVec2(lastX2 - lastW, y), ImVec2(lastX2, y + lastH), convColorTab(ImColor(pValue->Value.x, pValue->Value.y, pValue->Value.z), 0.8f), 15.f, ImVec2(0,0), ImDrawFlags_RoundCornersAll, 5.f);
	pImDrawList->AddRect(ImVec2(lastX2 - lastW, y), ImVec2(lastX2, y + lastH), convColorTab(ImColor(1.f, 1.f, 1.f, 0.45f)), 5.f);

	// Checkbox
	if (hasCheckbox) {
		animation2 = Lerp(animation2, *pValue2 ? 1.0f : 0.0f, pData.pAnimationSpeed * flDeltaTime);

		float offset = (lastH - 20.f * pData.pHScale) / 2;

		ImVec2 posMin = ImVec2(lastX2 - lastW - lastH + offset - 8.f, y + offset);
		ImVec2 posMax = ImVec2(lastX2 - lastW - offset - 8.f, y + lastH - offset);
		ImVec2 posMin2 = posMin;
		ImVec2 posMax2 = posMax;

		pImDrawList->AddRectFilled(posMin, posMax, convColorTab(pTheme.accent2), 3.f);

		float off = 10.f - 10.f * pData.pHScale * animation2;
		posMin.x += off;
		posMax.x -= off;
		posMin.y += off;
		posMax.y -= off;
		pImDrawList->AddRectFilled(posMin, posMax, convColorTab(pTheme.accent, animation2), 3.f);

		pImDrawList->AddRect(posMin2, posMax2, convColorTab(ImColor(1.f, 1.f, 1.f, 0.45f)), 3.f);
	}

	// Text
	char* buffer = new char[this->pLabel.length() + 7];
	std::sprintf(buffer, std::string(this->pLabel + " #%02X%02X%02X").c_str(),
		(int)(pValue->Value.x * 255),
		(int)(pValue->Value.y * 255),
		(int)(pValue->Value.z * 255)
	);

	pImDrawList->AddText(ImVec2(x, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), buffer);

	// Continue positions
	lastY += lastH;
	lastX = lastX2 - size - (hasAlpha ? 50.f : 25.f);

	return lastH;
}

void ColorPicker::DrawFull() {
	if (animation * pAnimation < 0.004f) return;

	///alpha

	if (hasAlpha) {
		bool _toggle = 0;
		for (int i = 0; i < size; i += 10) {
			float height = size - i >= 10 ? 10.f : size - i;
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(lastX + size + 30.f, lastY + 5 + i), ImVec2(lastX + size + 40.f, lastY + 5 + i + height), _toggle ? convColorTab(ImColor(0.5f, 0.5f, 0.5f), animation) : convColorTab(ImColor(0.9f, 0.9f, 0.9f), animation));
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(lastX + size + 40.f, lastY + 5 + i), ImVec2(lastX + size + 50.f, lastY + 5 + i + height), _toggle ? convColorTab(ImColor(0.9f, 0.9f, 0.9f), animation) : convColorTab(ImColor(0.5f, 0.5f, 0.5f), animation));
			_toggle = !_toggle;
		}

		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(lastX + size + 30.f, lastY + 5), ImVec2(lastX + size + 50.f, lastY + 5 + size), convColorTab(ImColor(pValue->Value.x, pValue->Value.y, pValue->Value.z, animation)), convColorTab(ImColor(pValue->Value.x, pValue->Value.y, pValue->Value.z, animation)), 0, 0);

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(lastX + size + 30.f, lastY + 5 + (1.f - a) * size), ImVec2(lastX + size + 50.f, lastY + 5 + (1.f - a) * size), convColorTab(ImColor(1.f, 1.f, 1.f, 0.7f), animation), 2.f);
	}

	//brightness

	ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(lastX, lastY + 5), ImVec2(lastX + size, lastY + 5 + size), convColorTab(ImColor(1.f, 1.f, 1.f), animation), convColorTab(ImColor(hue.Value.x, hue.Value.y, hue.Value.z), animation), convColorTab(ImColor(hue.Value.x, hue.Value.y, hue.Value.z), animation), convColorTab(ImColor(1.f, 1.f, 1.f), animation));
	ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(lastX, lastY + 5), ImVec2(lastX + size, lastY + 5 + size), 0, 0, convColorTab(ImColor(0.f, 0.f, 0.f), animation), convColorTab(ImColor(0.f, 0.f, 0.f), animation));

	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(lastX + s * size, lastY + 5 + (1.f - v) * size), 4.f, convColorTab(ImColor(0.1f, 0.1f, 0.1f, 0.8f), animation));

	const ImU32 colors[7] = { convColorTab(ImColor(1.f, 0.f, 0.f), animation), convColorTab(ImColor(1.f, 1.f, 0.f), animation), convColorTab(ImColor(0.f, 1.f, 0.f), animation), convColorTab(ImColor(0.f, 1.f, 1.f), animation), convColorTab(ImColor(0.f,0.f, 1.f), animation), convColorTab(ImColor(1.f, 0.f, 1.f), animation), convColorTab(ImColor(1.f, 0.f, 0.f), animation) };

	for (int i = 0; i < 6; ++i) {
		ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(lastX + size + 5.f, lastY + 5 + i * (size / 6.f)), ImVec2(lastX + size + 25.f, lastY + 5.f + (i + 1) * (size / 6.f)), colors[i], colors[i], colors[i + 1], colors[i + 1]);
	}

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(lastX + size + 5, lastY + 5 + h * size), ImVec2(lastX + size + 25.f, lastY + 5 + h * size), convColorTab(ImColor(1.f, 1.f, 1.f, 0.7f), animation), 2.f);

}

bool ColorPicker::click(float x, float y, int* moving, int* openedMenu, int id) {
	if (*openedMenu == id) {
		if (x < lastX || x > lastX + size + (hasAlpha ? 50.f : 25.f) || y < lastY + 5.f || y > lastY + size + 5.f) {
			*openedMenu = -1;
			return true;
		}

		movingType = x <= lastX + size ? 0 : (x <= lastX + size + 25.f && x >= lastX + size + 5.f ? 1 : x >= lastX + size + 30.f ? 2 : -1);

		if (movingType != -1) {
			this->hover(x, y, true);
			*moving = id;
		}
		return true;
	}

	if(x >= lastX2 - lastW && x <= lastX2 && y >= lastY - lastH && y <= lastY) {
		*openedMenu = id;
		return true;
	}

	float offset = (lastH - 20.f * pData.pHScale) / 2;

	if(x >= lastX2 - lastW - lastH + offset - 8.f && x <= lastX2 - lastW - offset - 8.f && y >= lastY - lastH + offset && y <= lastY - offset) {
		if(hasCheckbox) *pValue2 = !*pValue2;
		return true;
	}
	return false;
}


bool ColorPicker::hover(float x, float y, bool moving) {
	if (moving) {
		if (movingType == 0) {
			float _s = (x - lastX) / size;
			float _v = 1.f - (y - lastY - 5.f) / size;
			if(_v > 1.f) _v = 1.f;
			if(_v < 0.f) _v = 0.f;
			this->v = _v;
			if(_s > 1.f) _s = 1.f;
			if(_s < 0.f) _s = 0.f;
			this->s = _s;
		}
		else if (movingType == 1) {
			float h = (y - lastY - 5.f) / size;
			if(h > 1.f) h = 1.f;
			if(h < 0.f) h = 0.f;
			this->h = h;
		}
		else if (movingType == 2) {
			float a = 1.f - (y - lastY - 5.f) / size;
			if(a > 1.f) a = 1.f;
			if(a < 0.f) a = 0.f;
			this->a = a;
		}
		return true;
	}
	return false;
}



void ColorPicker::loadFromHSV() {
	ImGui::ColorConvertHSVtoRGB(h, s, v, pValue->Value.x, pValue->Value.y, pValue->Value.z);
	pValue->Value.w = a;

	float r = 0.f, g = 0.f, b = 0.f;

	int i = static_cast<int>(h * 6);

	float f = h * 6 - i;
	float p = 0.0f;
	float q = 1.0f - f;
	float t = f;

	switch (i % 6) {
	case 0: r = 1.0f, g = t, b = p; break;
	case 1: r = q, g = 1.0f, b = p; break;
	case 2: r = p, g = 1.0f, b = t; break;
	case 3: r = p, g = q, b = 1.0f; break;
	case 4: r = t, g = p, b = 1.0f; break;
	case 5: r = 1.0f, g = p, b = q; break;
	}

	hue = ImColor(r, g, b);
}

#include <iostream>

void ColorPicker::loadFromColor(ImColor _color) {
	this->a = _color.Value.w;
	ImGui::ColorConvertRGBtoHSV(_color.Value.x, _color.Value.y, _color.Value.z, h, s, v);
	loadFromHSV();
}