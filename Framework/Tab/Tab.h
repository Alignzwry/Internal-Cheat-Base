#pragma once
#include "../Widgets/Widgets.h"
#include <D3D11.h>

class Tab {
public:
	Tab(std::string label, ID3D11ShaderResourceView** icon);
	~Tab() = default;

	float Draw(float y, bool selected, bool hovered);
	void addCheckbox(std::string label, bool* value, int child = -1, int* Hotkey = nullptr);
	void addSlider(std::string label, float* value, float min = 0.f, float max = 1.f, std::string format = "%.3f");
	void addColorPicker(std::string label, ImColor* value, float size = 200.f, bool hasAlpha = 1, bool* value2 = nullptr);
	void addDropDown(std::string label, int* selected, std::vector<std::string>* options);
	void addButton(std::string label, std::string label2, ButtonFunction function);
	void addSeperator();
	bool checkHotkeys(int key);

	bool Input(UINT msg, WPARAM wParam, LPARAM lParam, int id);

	void reset();

public:
	float animation = 0.f;

private:
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
	float lastY = 0.f, animation_hover = 0.f;
	ID3D11ShaderResourceView** pIcon;
	// Scrolling
	float scroll = 0.f;
	float des_scroll = 0.f;
	float speed = 0.f;
};