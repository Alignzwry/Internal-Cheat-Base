#include "../Menu.h"
#include <iostream>
#include <global.h>

using namespace Menu;

// Add Checkboxes
void Tab::addCheckbox(std::string label, bool* value, int child, int* Hotkey) {
	this->m_pCheckboxes.push_back(Checkbox(label, value, child, Hotkey));
	this->m_pIDs.push_back(this->m_pCheckboxes.size() - 1 + WIDGET_CHECKBOX);
}

// Add Sliders
void Tab::addSlider(std::string label, float* value, float min, float max, std::string format) {
	this->m_pSliders.push_back(Slider(label, value, min, max, format));
	this->m_pIDs.push_back(this->m_pSliders.size() - 1 + WIDGET_SLIDER);
}

// Add ColorPickers
void Tab::addColorPicker(std::string label, ImColor* value, float size, bool hasAlpha, bool* value2) {
	this->m_pColorPickers.push_back(ColorPicker(label, value, size, hasAlpha, value2));
	this->m_pIDs.push_back(this->m_pColorPickers.size() - 1 + WIDGET_COLORPICKER);
}

// Add Dropdowns
void Tab::addDropDown(std::string label, int* selected, std::vector<std::string>* options) {
	this->m_pDropdowns.push_back(Dropdown(label, selected, options));
	this->m_pIDs.push_back(this->m_pDropdowns.size() - 1 + WIDGET_DROWPDOWN);
}

// Add Buttons
void Tab::addButton(std::string label, std::string label2, ButtonFunction function) {
	this->m_pButtons.push_back(Button(label, label2, function));
	this->m_pIDs.push_back(this->m_pButtons.size() - 1 + WIDGET_BUTTON);
}

// Add Seperators
void Tab::addSeperator() {
	this->m_pSeperators.push_back(Seperator());
	this->m_pIDs.push_back(this->m_pSeperators.size() - 1 + WIDGET_SEPERATOR);
}

float Tab::Draw(float _y, bool selected, bool hovered) {
	lastY = _y;

	// Animation
	animation = Lerp(animation, selected ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);
	if (animation > 1.f) animation = 1.f;
	if (animation < 0.f) animation = 0.f;

	// Hover Animation
	animation_hover = Lerp(animation_hover, hovered ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);
	if (animation_hover > 1.f) animation_hover = 1.f;
	if (animation_hover < 0.f) animation_hover = 0.f;

	// SideBar
	pImDrawList->AddRectFilled(ImVec2(Menu::pData.xpos + 16.f, _y), ImVec2(Menu::pData.xpos + pData.sideBarWidth - 16.f, _y + 64.f * pData.pHScale), convColor(ImColor(1.f, 1.f, 1.f, 0.0666666f * animation)), 6.f);
	
	pImDrawList->AddRectFilled(ImVec2(Menu::pData.xpos + 16.f, _y), ImVec2(Menu::pData.xpos + pData.sideBarWidth - 16.f, _y + 64.f * pData.pHScale), convColor(ImColor(1.f, 1.f, 1.f, 0.0352941f * animation_hover)), 6.f);

	if(pIcon != nullptr)
		pImDrawList->AddImageRounded(*pIcon, ImVec2(Menu::pData.xpos + 24.f, _y + 8.f), ImVec2(Menu::pData.xpos + 24.f + 48.f * pData.pHScale, _y + 8.f + 48.f * pData.pHScale), ImVec2(0, 0), ImVec2(1, 1), convColor(pTheme.icon), 6.f);
	
	static ImVec2 labelTextSize = ImVec2(0, 0);
	static float lastHScale = 0.f;
	if (lastHScale != pData.pHScale) {
		labelTextSize = ImGui::CalcTextSize(pLabel.c_str());
		lastHScale = pData.pHScale;
	}

	pImDrawList->AddText(ImVec2(Menu::pData.xpos + 36.f + 48.f * pData.pHScale, _y + 32.f * pData.pHScale - labelTextSize.y / 2), convColor(pTheme.text), pLabel.c_str());

	///////////////////////////////////////

	if (!selected)
		return 64.f * pData.pHScale;

	float x = Menu::pData.xpos + pData.sideBarWidth + 32.f;
	float y = Menu::pData.ypos + 70.f;

	// Header
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	pImDrawList->AddText(ImVec2(x, y), convColor(pTheme.text), pLabel.c_str());

	ImGui::PopFont();

	y += 50.f - scroll;

	// Contents
	pImDrawList->PushClipRect(ImVec2(x, Menu::pData.ypos + 100.f), ImVec2(Menu::pData.xpos + Menu::pData.width, Menu::pData.ypos + Menu::pData.height - 8.f), true);

	for (int i : this->m_pIDs) {
		if (i >= WIDGET_BUTTON) {
			y += Menu::pData.pSpacing + this->m_pButtons[i - WIDGET_BUTTON].Draw(x, y, this->hovered == i, this->moving == i);
		}
		else if (i >= WIDGET_SEPERATOR) {
			y += Menu::pData.pSpacing + this->m_pSeperators[i - WIDGET_SEPERATOR].Draw(x, y);
		}
		else if (i >= WIDGET_DROWPDOWN) {
			y += Menu::pData.pSpacing + this->m_pDropdowns[i - WIDGET_DROWPDOWN].Draw(x, y, this->hovered == i, this->openedMenu == i);
		}
		else if (i >= WIDGET_COLORPICKER) {
			y += Menu::pData.pSpacing + this->m_pColorPickers[i - WIDGET_COLORPICKER].Draw(x, y, this->hovered == i, this->openedMenu == i);
		}
		else if (i >= WIDGET_SLIDER) {
			y += Menu::pData.pSpacing + this->m_pSliders[i - WIDGET_SLIDER].Draw(x, y, this->hovered == i);
		}
		else if (i >= WIDGET_CHECKBOX) {
			if (this->m_pCheckboxes[i - WIDGET_CHECKBOX].pHasChild) {
				float height = this->m_pCheckboxes[i - WIDGET_CHECKBOX].Draw(x, y, this->hovered == i, this->hovered - 1000 == i, this->typing == i, this->hovered - 2000 == i);
				Menu::pData.childWindows.push_back({
					ImVec2((Menu::pData.xpos + Menu::pData.width - Menu::pData.borderSpace - 42.f * pData.pWScale - 15.f - (this->m_pCheckboxes[i - WIDGET_CHECKBOX].default_height * pData.pHScale) / 2.f)
					, y + height / 2.f),
					i - WIDGET_CHECKBOX + WIDGET_CHILD,
					this->m_pCheckboxes[i - WIDGET_CHECKBOX].child_id,
					});
				y += Menu::pData.pSpacing + height;
			}
			else {
				y += Menu::pData.pSpacing + this->m_pCheckboxes[i - WIDGET_CHECKBOX].Draw(x, y, this->hovered == i, this->hovered - 1000 == i, this->typing == i);
			}
		}
	}

	pImDrawList->PopClipRect();

	// Scroll
	des_scroll -= speed * flDeltaTime * 30.f;
	speed = Lerp(speed, 0.f, pData.pAnimationSpeed * flDeltaTime * 2.f);

	if (des_scroll < 0.f) des_scroll = Lerp(des_scroll, 0.f, pData.pAnimationSpeed * flDeltaTime * 2.f);

	y -= -scroll + 120.f + Menu::pData.ypos + 60.f;
	if (y < 0.f) y = 0.f;
	if (des_scroll > y)
		des_scroll = Lerp(des_scroll, y, pData.pAnimationSpeed * flDeltaTime * 2.f);

	scroll = Lerp(scroll, des_scroll, pData.pAnimationSpeed * flDeltaTime * 2.f);


	// Draw Pop ups
	for (int i = 0; i < this->m_pColorPickers.size(); i++) {
		this->m_pColorPickers[i].DrawFull();
	}

	for (int i = 0; i < this->m_pDropdowns.size(); i++) {
		this->m_pDropdowns[i].DrawFull();
	}

	return 64.f * pData.pHScale;
}

bool Tab::Input(UINT msg, WPARAM wParam, LPARAM lParam, int id) {
	// Get mouse position
	float x = (float)LOWORD(lParam);
	float y = (float)HIWORD(lParam);

	// Check if the mouse is over the tab
	if (msg == WM_MOUSEMOVE) {
		if (moving == -1 && x >= Menu::pData.xpos + 16.f && x <= Menu::pData.xpos + pData.sideBarWidth - 16.f && y >= lastY && y <= lastY + 64.f * pData.pHScale) {
			Menu::hoveredTab = id;
			return true;
		}
		Menu::hoveredTab = -1;
	}
	else if (msg == WM_LBUTTONDOWN) {
		if (x >= Menu::pData.xpos + 16.f && x <= Menu::pData.xpos + pData.sideBarWidth - 16.f && y >= lastY && y <= lastY + 64.f * pData.pHScale) {
			Menu::selectedTab = id;
			return true;
		}
	}

	if (id != Menu::selectedTab) {
		return false;
	}

	///////////////////////////////////

	switch (msg) {
	case WM_LBUTTONDOWN:
		// Check pop ups
		// Drop downs
		if (openedMenu >= WIDGET_DROWPDOWN) {
			for (int i = 0; i < this->m_pDropdowns.size(); i++) {
				if (this->m_pDropdowns[i].click(x, y, &openedMenu, i + WIDGET_DROWPDOWN)) {
					return true;
				}
			}
			for (int i = 0; i < this->m_pColorPickers.size(); i++) {
				if (this->m_pColorPickers[i].click(x, y, &moving, &openedMenu, i + WIDGET_COLORPICKER)) {
					return true;
				}
			}
		}
		// Color pickers
		else {
			for (int i = 0; i < this->m_pColorPickers.size(); i++) {
				if (this->m_pColorPickers[i].click(x, y, &moving, &openedMenu, i + WIDGET_COLORPICKER)) {
					return true;
				}
			}
			for (int i = 0; i < this->m_pDropdowns.size(); i++) {
				if (this->m_pDropdowns[i].click(x, y, &openedMenu, i + WIDGET_DROWPDOWN)) {
					return true;
				}
			}
		}

		// Checkboxes
		for (int i = 0; i < this->m_pCheckboxes.size(); i++) {
			int temp = this->m_pCheckboxes[i].click(x, y);
			if (temp == 0) continue;

			if (temp == 2) {
				this->typing = (this->typing != i + WIDGET_CHECKBOX) ? i + WIDGET_CHECKBOX : -1;
				this->m_pCheckboxes[i].needsUpdate = true;
			}

			if (temp == 3) {
				Menu::pData.openedMenu = i + WIDGET_CHILD;
				return true;
			}

			return true;
		}

		// Sliders
		for (int i = 0; i < this->m_pSliders.size(); i++) {
			if (this->m_pSliders[i].click(x, y)) {
				this->moving = i + WIDGET_SLIDER;
				return true;
			}
		}

		// Buttons
		for (int i = 0; i < this->m_pButtons.size(); i++) {
			if (this->m_pButtons[i].click(x, y, &this->moving, i + WIDGET_BUTTON)) {
				this->moving = i + WIDGET_BUTTON;
				return true;
			}
		}
		break;
	case WM_MOUSEMOVE:
		// Check if moving something
		if (this->moving != -1) {
			for (int i = 0; i < this->m_pColorPickers.size(); i++) {
				if ((this->moving - WIDGET_COLORPICKER) != i) continue;
				this->m_pColorPickers[i].hover(x, y, (this->moving - WIDGET_COLORPICKER) == i);
				return true;
			}

			for (int i = 0; i < this->m_pSliders.size(); i++) {
				if ((this->moving - WIDGET_SLIDER) != i) continue;
				this->m_pSliders[i].hover(x, y, (this->moving - WIDGET_SLIDER) == i);
				return true;
			}

			for (int i = 0; i < this->m_pButtons.size(); i++) {
				if ((this->moving - WIDGET_BUTTON) == i) return true;
			}

			// In case of wrong number
			this->moving = -1;
			return true;
		}

		// Color pickers
		for (int i = 0; i < this->m_pColorPickers.size(); i++) {
			if (this->m_pColorPickers[i].hover(x, y, (this->moving - WIDGET_COLORPICKER) == i)) {
				return true;
			}
			if (this->openedMenu == i + WIDGET_COLORPICKER) {
				return true;
			}
		}

		// Sliders
		for (int i = 0; i < this->m_pSliders.size(); i++) {
			if (this->m_pSliders[i].hover(x, y, (this->moving - WIDGET_SLIDER) == i)) {
				this->hovered = i + WIDGET_SLIDER;
				return true;
			}
		}

		// Drop downs
		for (int i = 0; i < this->m_pDropdowns.size(); i++) {
			if (this->m_pDropdowns[i].hover(x, y)) {
				return true;
			}
			if (this->openedMenu == i + WIDGET_DROWPDOWN) {
				return true;
			}
		}

		// Buttons
		for (int i = 0; i < this->m_pButtons.size(); i++) {
			if (this->m_pButtons[i].hover(x, y)) {
				this->hovered = i + WIDGET_BUTTON;
				return true;
			}
		}

		// Checkboxes
		for (int i = 0; i < this->m_pCheckboxes.size(); i++) {
			if (int temp = this->m_pCheckboxes[i].hover(x, y); temp != 0) {
				this->hovered = i + WIDGET_CHECKBOX + (temp - 1) * 1000;
				return true;
			}
		}

		this->hovered = -1;
		break;
	case WM_LBUTTONUP:
		if (this->moving != -1) {
			this->moving = -1;
			return true;
		}
		break;
	case WM_MOUSEWHEEL:
		POINT point;
		point.x = x;
		point.y = y;

		// Scroll tab
		ScreenToClient(Menu::pData.Window, &point);
		if (point.x >= Menu::pData.xpos + Menu::pData.sideBarWidth && point.x <= Menu::pData.xpos + Menu::pData.width && point.y >= Menu::pData.ypos + 100.f && point.y <= Menu::pData.ypos + Menu::pData.height) {
			if (this->openedMenu == -1) {
				speed += GET_WHEEL_DELTA_WPARAM(wParam);
				constexpr float MAX_SCROLL_SPEED = 140.f;
				if (speed > MAX_SCROLL_SPEED) speed = MAX_SCROLL_SPEED;
				else if (speed < -MAX_SCROLL_SPEED) speed = -MAX_SCROLL_SPEED;
			}
		}
		return true;
	case WM_KEYDOWN:
		// Check for hotkeys on checkboxes
		if (this->typing != -1) {
			for(int i = 0; i < this->m_pCheckboxes.size(); i++) {
				if(i + WIDGET_CHECKBOX == this->typing) {
					if (wParam == VK_BACK) {
						this->m_pCheckboxes[i].setKey(-1);
					}
					else if (wParam != VK_ESCAPE) {
						this->m_pCheckboxes[i].setKey(wParam);
					}
					else
						this->m_pCheckboxes[i].needsUpdate = true;
					this->typing = -1;
					return true;
				}
			}
		}
	}

	return false;
}

Tab::Tab(std::string label, ID3D11ShaderResourceView** icon) {
	this->pLabel = label;
	this->pIcon = icon;
}

void Tab::reset() {
	// Reset almost all the animations when the tab is closed
	for (int i = 0; i < this->m_pCheckboxes.size(); i++) {
		this->m_pCheckboxes[i].animation = 0.f;
		this->m_pCheckboxes[i].animation_hover = 0.f;
	}

	for (int i = 0; i < this->m_pSliders.size(); i++) {
		this->m_pSliders[i].animation = 0.f;
		this->m_pSliders[i].animation_hover = 0.f;
	}

	for (int i = 0; i < this->m_pColorPickers.size(); i++) {
		this->m_pColorPickers[i].animation = 0.f;
	}

	for (int i = 0; i < this->m_pDropdowns.size(); i++) {
		this->m_pDropdowns[i].animation = 0.f;
		this->m_pDropdowns[i].animation_hover = 0.f;
		this->m_pDropdowns[i].animation_selected = 0.f;
	}

	// Scroll back to top
	this->des_scroll = this->scroll = 0.f;
}


bool Tab::checkHotkeys(int key) {
	for(int i = 0; i < this->m_pCheckboxes.size(); i++) {
		if (this->m_pCheckboxes[i].checkKey(key)) return true;
	}
	return false;
}