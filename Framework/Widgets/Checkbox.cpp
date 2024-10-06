#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;

Checkbox::Checkbox(std::string label, bool* value, int childId, int* Hotkey) {
	this->pLabel = label;
	this->pValue = value;
	this->pHotkey = Hotkey;
	this->pHasChild = childId != -1;
	this->child_id = childId;
}

float Checkbox::Draw(float x, float y, bool hovered, bool hovered2, bool typing, bool childHover) {
	// Positions
	lastY = y;

	lastW = 42.f * pData.pWScale;
	lastH = default_height * pData.pHScale;

	// Title
	pImDrawList->AddText(ImVec2(x, y + (lastH - pData.textSize) / 2), convColorTab(pTheme.text), pLabel.c_str());

	// Animation
	animation = Lerp(animation, *pValue ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	animation_hover = Lerp(animation_hover, hovered ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	animation_hover2 = Lerp(animation_hover2, hovered2 ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	// Continue positions
	x = pData.xpos + pData.width - lastW - pData.borderSpace;

	lastX = x;

	// Checkbox
	pImDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + lastW, y + lastH), convColorTab(interpolateColors(pTheme.accent2, pTheme.accent, animation)), lastH / 2);

	pImDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + lastW, y + lastH), convColorTab(interpolateColors(pTheme.hover2, pTheme.hover, animation), animation_hover), lastH / 2);

	float xpos = x + (lastH - 4.f) / 2 + ((lastW - 4.f) - (lastH - 4.f)) * animation;

	float offset = (abs(animation - 0.5f) - 0.5f) * 2.f;

	float radius = (lastH - 4.f) / 2 + offset;

	pImDrawList->AddCircleFilled(ImVec2(xpos + 2.f, y + 2.f + radius - offset), radius, convColorTab(pTheme.highlight));

	pImDrawList->AddShadowCircle(ImVec2(xpos + 2.f, y + 2.f + radius - offset), radius, convColorTab(pTheme.highlight), 15.f, ImVec2(0, 0));

	// Child
	if (pHasChild) {
		animation_hoverchild = Lerp(animation_hoverchild, childHover ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

		pImDrawList->AddImage(Menu::ChildIcon, ImVec2(x - 15.f - lastH + 4.f, y + 2.f), ImVec2(x - 15.f, y + lastH - 2.f), ImVec2(0, 0), ImVec2(1, 1), convColorTab(pTheme.icon, 0.5f + animation_hoverchild * 0.5f));
	}

	// Hotkey
	if (pHotkey != nullptr) {
		pImDrawList->AddRectFilled(ImVec2(x - 30.f - lastW * 1.5f - (pHasChild ? lastH + 4.f : 0.f), y + 2.f), ImVec2(x - 30.f - (pHasChild ? lastH + 4.f : 0.f), y + lastH - 2.f), convColorTab(pTheme.accent2), lastH / 2);
		pImDrawList->AddRectFilled(ImVec2(x - 30.f - lastW * 1.5f - (pHasChild ? lastH + 4.f : 0.f), y + 2.f), ImVec2(x - 30.f - (pHasChild ? lastH + 4.f : 0.f), y + lastH - 2.f), convColorTab(pTheme.hover2, animation_hover2), lastH / 2);

		pImDrawList->AddRect(ImVec2(x - 30.f - lastW * 1.5f - (pHasChild ? lastH + 4.f : 0.f), y + 2.f), ImVec2(x - 30.f - (pHasChild ? lastH + 4.f : 0.f), y + lastH - 2.f), convColorTab(ImColor(1.f, 1.f, 1.f, 0.45f)), lastH / 2);

		// Update text
		if (needsUpdate) {
			needsUpdate = false;
			if (typing) {
				this->pHotkeyLabel = "...";
			}
			else if (*pHotkey == -1)
				this->pHotkeyLabel = "None";
			else {
				UINT scanCode = MapVirtualKey(*pHotkey, MAPVK_VK_TO_VSC);
				char name[10];
				GetKeyNameTextA((scanCode << 16), name, 10);
				this->pHotkeyLabel = name;
			}
			this->HotkeyLabelSize = ImGui::CalcTextSize(pHotkeyLabel.c_str());
		}

		float size = HotkeyLabelSize.x;
		if(HotkeyLabelSize.x > lastW * 1.5f - 4.f)
			size = lastW * 1.5f - 4.f;

		pImDrawList->PushClipRect(ImVec2(x - 29.f - lastW * 1.5f, y), ImVec2(x - 31.f, y + lastH), true);

		pImDrawList->AddText(ImVec2(x - 30.f - lastW * 1.5f + lastW * 0.75f - size / 2, y + 2.f + (lastH - 4.f - HotkeyLabelSize.y) / 2), convColorTab(pTheme.text), pHotkeyLabel.c_str());

		pImDrawList->PopClipRect();
	}

	return lastH;
}

int Checkbox::click(float x, float y) {
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		*pValue = !*pValue;
		return 1;
	}
	if (pHotkey != nullptr && x >= lastX - 30.f - lastW * 1.5f - (pHasChild ? lastH + 4.f : 0.f) && y >= lastY + 2.f && x <= lastX - 30.f - (pHasChild ? lastH + 4.f : 0.f) && y <= lastY + lastH - 2.f)
		return 2;
	if (pHasChild && x >= lastX - 15.f - lastH + 4.f && x <= lastX - 15.f && y >= lastY + 2.f && y <= lastY + lastH - 2.f)
		return 3;
	return 0;
}

void Checkbox::setKey(int key) {
	if (this->pHotkey == nullptr) return;
	*this->pHotkey = key;
	this->needsUpdate = true;
}

int Checkbox::hover(float x, float y) {
	if (x >= lastX && x <= lastX + lastW && y >= lastY && y <= lastY + lastH) {
		return 1;
	}
	if (pHotkey != nullptr && x >= lastX - 30.f - lastW * 1.5f - (pHasChild ? lastH + 4.f : 0.f) && y >= lastY + 2.f && x <= lastX - 30.f - (pHasChild ? lastH + 4.f : 0.f) && y <= lastY + lastH - 2.f)
		return 2;
	if (pHasChild && x >= lastX - 15.f - lastH + 4.f && x <= lastX - 15.f && y >= lastY + 2.f && y <= lastY + lastH - 2.f)
		return 3;
	return false;
}