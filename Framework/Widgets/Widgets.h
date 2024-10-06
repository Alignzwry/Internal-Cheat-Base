#pragma once
#include <Windows.h>
#include <string>
#include <imgui/imgui.h>
#include <vector>

class Slider {
public:
	Slider(std::string label, float* value, float min = 0.f, float max = 1.f, std::string format = "%.3f");

	float Draw(float x, float y, bool hovered);

	bool click(float x, float y);
	bool hover(float x, float y, bool moving);

public:
	float animation = 0.f, animation_hover = 0.f;
	const float default_height = 12.f;
	float default_width = 200.f; // change if in child window

private:
	std::string pLabel, pFormat;
	float* pValue;
	float textWidth = 0.f, labelTextWidth = 0.f;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
	float min, max;
	bool isInit = false;
};

class Checkbox {
public:
	Checkbox(std::string label, bool* value, int child_id = -1, int* Hotkey = 0);

	float Draw(float x, float y, bool hovered, bool hovered2 = 0, bool typing = 0, bool childHover = 0);

	int click(float x, float y);
	int hover(float x, float y);
	void setKey(int key);
	bool checkKey(int key) {
		if (pHotkey == nullptr) return false;
		if (*pHotkey != key) return false;
		*this->pValue = !*this->pValue;
		return true;
	}

public:
	float animation = 0.f, animation_hover = 0.f, animation_hover2 = 0.f, animation_hoverchild = 0.f;
	bool needsUpdate = true;
	bool pHasChild = false;
	int* pHotkey = 0;
	const float default_height = 24.f;
	int child_id;

private:
	std::string pLabel, pHotkeyLabel;
	bool* pValue;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
	ImVec2 HotkeyLabelSize = ImVec2(0, 0);
};

class ColorPicker {
public:
	ColorPicker(std::string label, ImColor* value, float size, bool hasAlpha = 1, bool* value2 = nullptr);

	float Draw(float x, float y, bool hovered, bool opened);
	void DrawFull();

	bool click(float x, float y, int* moving, int* openedMenu, int id);
	bool hover(float x, float y, bool moving);

	void loadFromColor(ImColor _color);
	void loadFromHSV();

public:
	float animation = 0.f, animation2 = 0.f;
	const float default_height = 30.f;

private:
	std::string pLabel;
	ImColor* pValue;
	ImColor hue;
	bool hasAlpha;
	float lastX = 0.f, lastY = 0.f, lastX2 = 0.f, lastY2 = 0.f, lastW = 0.f, lastH = 0.f;
	float h = 0.f, s = 0.f, v = 0.f, a = 1.f, size = 200.f;
	int movingType = 0;
	bool hasCheckbox = 0;
	bool* pValue2 = nullptr;
};

class Dropdown {
public:
	Dropdown(std::string label, int* selected, std::vector<std::string>* options);

	float Draw(float x, float y, bool hovered, bool opened);

	void DrawFull();

	bool click(float x, float y, int* openedMenu, int id);
	bool hover(float x, float y);

public:
	float animation = 0.f, animation_hover = 0.f, animation_selected = 0.f;
	const float default_height = 30.f;

private:
	std::string pLabel;
	int* pSelected;
	std::vector<std::string>* pOptions;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
};

typedef void(__fastcall* ButtonFunction)();

class Button {
public:
	Button(std::string label, std::string label2, ButtonFunction function);

	float Draw(float x, float y, bool hovered, bool pressed);

	bool click(float x, float y, int* pressed, int id);
	bool hover(float x, float y);

	float animation = 0.f, animation_hover = 0.f;
	const float default_height = 30.f;

private:
	std::string pLabel;
	std::string pLabel2;
	ButtonFunction pFunction;
	float textWidth = 0.f;
	float lastX = 0.f, lastY = 0.f, lastW = 0.f, lastH = 0.f;
	bool isInit = false;
};

class Seperator {
public:
	Seperator() = default;

	float Draw(float x, float y);

public:
	const float default_height = 2.f;
};