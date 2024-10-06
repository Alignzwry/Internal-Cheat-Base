#pragma once
#include "Widgets.h"

class ChildWindow {
public:
	ChildWindow(std::string label);
	~ChildWindow() = default;

	void Draw(float x, float y, bool opened);

	bool Input(UINT msg, WPARAM wParam, LPARAM lParam);

	void addCheckbox(std::string label, bool* value, int* Hotkey = nullptr);
	void addSlider(std::string label, float* value, float min = 0.f, float max = 1.f, std::string format = "%.3f");
	void addColorPicker(std::string label, ImColor* value, float size = 200.f, bool hasAlpha = 1, bool* value2 = nullptr);
	void addDropDown(std::string label, int* selected, std::vector<std::string>* options);
	void addButton(std::string label, std::string label2, ButtonFunction function);
	void addSeperator();
	void calcSize();

public:
	float animation = 0.f;

private:
	// Contens
	std::vector<Button> m_pButtons;
	std::vector<Checkbox> m_pCheckboxes;
	std::vector<Slider> m_pSliders;
	std::vector<ColorPicker> m_pColorPickers;
	std::vector<Dropdown> m_pDropdowns;
	std::vector<Seperator> m_pSeperators;
	std::vector<int> m_pIDs;

	int hovered = -1;
	int moving = -1;
	int openedMenu = -1;
	int typing = -1;

	std::string pLabel;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
	float currentHeight = 54.f;
};