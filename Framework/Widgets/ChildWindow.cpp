#include "Widgets.h"
#include "../Menu.h"
#include <global.h>

using namespace Menu;

ChildWindow::ChildWindow(std::string label) {
	this->pLabel = std::string(label);
}


// Add Checkboxes
void ChildWindow::addCheckbox(std::string label, bool* value, int* Hotkey) {
	this->m_pCheckboxes.push_back(Checkbox(label, value, -1, Hotkey));
	this->m_pIDs.push_back(this->m_pCheckboxes.size() - 1 + WIDGET_CHECKBOX);
}

// Add Sliders
void ChildWindow::addSlider(std::string label, float* value, float min, float max, std::string format) {
	this->m_pSliders.push_back(Slider(label, value, min, max, format));
	this->m_pSliders.back().default_width = 140.f; // Resize for child window
	this->m_pIDs.push_back(this->m_pSliders.size() - 1 + WIDGET_SLIDER);
}

// Add ColorPickers
void ChildWindow::addColorPicker(std::string label, ImColor* value, float size, bool hasAlpha, bool* value2) {
	this->m_pColorPickers.push_back(ColorPicker(label, value, size, hasAlpha, value2));
	this->m_pIDs.push_back(this->m_pColorPickers.size() - 1 + WIDGET_COLORPICKER);
}

// Add Dropdowns
void ChildWindow::addDropDown(std::string label, int* selected, std::vector<std::string>* options) {
	this->m_pDropdowns.push_back(Dropdown(label, selected, options));
	this->m_pIDs.push_back(this->m_pDropdowns.size() - 1 + WIDGET_DROWPDOWN);
}

// Add Buttons
void ChildWindow::addButton(std::string label, std::string label2, ButtonFunction function) {
	this->m_pButtons.push_back(Button(label, label2, function));
	this->m_pIDs.push_back(this->m_pButtons.size() - 1 + WIDGET_BUTTON);
}

// Add Seperators
void ChildWindow::addSeperator() {
	this->m_pSeperators.push_back(Seperator());
	this->m_pIDs.push_back(this->m_pSeperators.size() - 1 + WIDGET_SEPERATOR);
}

void ChildWindow::calcSize() {
	currentHeight = 54.f;

	for (int i : this->m_pIDs) {
		if (i >= WIDGET_BUTTON) {
			currentHeight += m_pButtons[i - WIDGET_BUTTON].default_height + pData.pSpacing;
		}
		else if (i >= WIDGET_SEPERATOR) {
			currentHeight += m_pSeperators[i - WIDGET_SEPERATOR].default_height + pData.pSpacing;
		}
		else if (i >= WIDGET_DROWPDOWN) {
			currentHeight += m_pDropdowns[i - WIDGET_DROWPDOWN].default_height + pData.pSpacing;
		}
		else if (i >= WIDGET_COLORPICKER) {
			currentHeight += m_pColorPickers[i - WIDGET_COLORPICKER].default_height + pData.pSpacing;
		}
		else if (i >= WIDGET_SLIDER) {
			currentHeight += m_pSliders[i - WIDGET_SLIDER].default_height + pData.pSpacing;
		}
		else if (i >= WIDGET_CHECKBOX) {
			currentHeight += m_pCheckboxes[i - WIDGET_CHECKBOX].default_height + pData.pSpacing;
		}
	}

	currentHeight -= pData.pSpacing;
}

#include <global.h>
void ChildWindow::Draw(float _x, float _y, bool opened) {
	lastY = _y;
	lastX = _x;
	lastW = 340.f * pData.pWScale;
	lastH = currentHeight * pData.pHScale;

	if (lastX + lastW > vecDisplaySize.x) {
		lastX = _x - lastW;
	}
	if (lastY + lastH > vecDisplaySize.y) {
		lastY = _y - lastH;
	}

	// Animation
	animation = Lerp(animation, opened ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

	// dim menu
	pImDrawList->AddRectFilled(ImVec2(pData.xpos, pData.ypos), ImVec2(pData.xpos + pData.width, pData.ypos + pData.height), convColor(ImColor(0.f, 0.f, 0.f, 0.5f * animation)), 8.f);

	// Change Menu position and backup
	float backup_x = Menu::pData.xpos;
	float backup_y = Menu::pData.ypos;
	float backup_w = Menu::pData.width;
	float backup_h = Menu::pData.height;
	float backup_animation = Menu::pAnimation;
	float backup_borderspace = Menu::pData.borderSpace;
	Menu::pData.xpos = lastX;
	Menu::pData.ypos = lastY;
	Menu::pData.width = lastW;
	Menu::pData.height = lastH;
	Menu::pAnimation *= animation;
	Menu::pData.borderSpace = 15.f;

	// Background
	pImDrawList->AddRectFilled(ImVec2(lastX, lastY), ImVec2(lastX + lastW, lastY + lastH), convColor(pTheme.background2), 8.f);
	pImDrawList->AddRectFilled(ImVec2(lastX + 8.f, lastY + 37.f), ImVec2(lastX + lastW - 8.f, lastY + lastH - 8.f), convColor(pTheme.background), 8.f);

	// Header
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	pImDrawList->AddText(ImVec2(lastX + 8.f, lastY + 3.f), convColor(pTheme.text), this->pLabel.c_str());

	ImGui::PopFont();

	// Contents
	pImDrawList->PushClipRect(ImVec2(lastX + 8.f, lastY + 37.f), ImVec2(lastX + lastW - 8.f, lastY + lastH - 8.f));

	// Init
	float x = lastX + 15.f;
	float y = lastY + 42.f;

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
			y += Menu::pData.pSpacing + this->m_pCheckboxes[i - WIDGET_CHECKBOX].Draw(x, y, this->hovered == i, this->hovered - 1000 == i, this->typing == i);
		}
	}

	pImDrawList->PopClipRect();

	pImDrawList->AddRect(ImVec2(lastX, lastY), ImVec2(lastX + lastW, lastY + lastH), convColor(ImColor(1.f, 1.f, 1.f, 0.45f)), 8.f);

	// Draw Pop ups
	for (int i = 0; i < this->m_pColorPickers.size(); i++) {
		this->m_pColorPickers[i].DrawFull();
	}

	for (int i = 0; i < this->m_pDropdowns.size(); i++) {
		this->m_pDropdowns[i].DrawFull();
	}

	Menu::pData.xpos = backup_x;
	Menu::pData.ypos = backup_y;
	Menu::pData.width = backup_w;
	Menu::pData.height = backup_h;
	Menu::pAnimation = backup_animation;
	Menu::pData.borderSpace = backup_borderspace;
}


bool ChildWindow::Input(UINT msg, WPARAM wParam, LPARAM lParam) {
	// Get mouse position
	float x = (float)LOWORD(lParam);
	float y = (float)HIWORD(lParam);

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

		if (x < lastX || x > lastX + lastW || y < lastY || y > lastY + lastH)
			Menu::pData.openedMenu = -1; // Close

		return true;
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

		return true;
	case WM_LBUTTONUP:
		if (this->moving != -1) {
			this->moving = -1;
			return true;
		}
		return true;
	case WM_MOUSEWHEEL:
		return true;
	case WM_KEYDOWN:
		// Check for hotkeys on checkboxes
		if (this->typing != -1) {
			for (int i = 0; i < this->m_pCheckboxes.size(); i++) {
				if (i + WIDGET_CHECKBOX == this->typing) {
					if (wParam == VK_BACK) {
						this->m_pCheckboxes[i].setKey(-1);
					}
					else if (wParam != VK_ESCAPE) {
						this->m_pCheckboxes[i].setKey(wParam);
					}
					else
						this->m_pCheckboxes[i].needsUpdate = true;
					this->typing = -1;
					break;
				}
			}
		}

		return true;
	}

	return false;
}