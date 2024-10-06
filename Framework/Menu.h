#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <tchar.h>
#include <vector>

#include "Widgets/Widgets.h"
#include "Widgets/ChildWindow.h"

#define Menu_ratio 0.6486486486
#define Menu_min_width 640

// Widget types
enum WidgetType {
	WIDGET_CHECKBOX = 0,
	WIDGET_SLIDER = 100,
	WIDGET_COLORPICKER = 200,
	WIDGET_DROWPDOWN = 300,
	WIDGET_SEPERATOR = 400,
	WIDGET_BUTTON = 500,
	WIDGET_CHILD = 600
};

// Everything d3d11 related
class D3D11 {
public:
	D3D11() = default;
	~D3D11() = default;

	bool Init(HWND hWnd);
	void Destroy();

	bool CreateRenderTarget();
	void CleanupRenderTarget();

public:
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	bool g_SwapChainOccluded = false;
	UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
};

// Colors for the menu
class Theme {
public:
	ImColor background = ImColor(0.13f, 0.13f, 0.13f);
	ImColor background2 = ImColor(0.06f, 0.06f, 0.06f);
	ImColor accent = ImColor(0xB5, 0xB2, 0xFF, 80);
	ImColor accent2 = ImColor(0.25f, 0.25f, 0.25f);
	ImColor text = ImColor(0.97f, 0.9f, 1.f);
	ImColor icon = ImColor(0.97f, 0.9f, 1.f);
	ImColor hover = ImColor(0xBF, 0xC1, 0xFF, 130);
	ImColor hover2 = ImColor(1.f, 1.f, 1.f, 0.3f);
	ImColor highlight = ImColor(0xB5, 0xBE, 0xFF, 255);
};

class Data {
public:
	float pWScale = 1.1f; // The x scale of the menu
	float pHScale = 1.1f; // The y scale of the menu
	float pSpacing = 20.f; // The spacing between widgets

	float pAnimationSpeed = 8.f; // The global animation speed

	float xpos = 100.f; // The x position of the menu
	float ypos = 100.f; // The y position of the menu
	float width = 800.f; // The width of the menu
	float height = 519.f; // The height of the menu
	float sideBarWidth = 200.f; // The width of the side bar
	float borderSpace = 32.f; // The border space of the menu

	float textSize = 18.f; // The text size of the menu

	HWND Window = NULL;


	// Child window
	void addChild(std::string label);
	std::vector<ChildWindow> m_pChildWindows{};
	int openedMenu = -1;

	struct temp {
		ImVec2 pos;
		int id, index;
	};
	std::vector<temp> childWindows;
};

#include "Tab/Tab.h"
namespace Menu {
	void onRender();

	bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	inline bool isOpen = false;
	inline bool isInit = false;

	inline D3D11 pD3D11;

	inline std::vector<Tab> m_pTabs;
	inline int selectedTab = 0;
	inline int hoveredTab = 0;
	inline Data pData;

	inline float pAnimation = 0.f;

	inline bool closed = false;

	inline bool moving = 0;
	inline float moving_offx = 0.f;
	inline float moving_offy = 0.f;

	//fps
	inline float frameCount = 0;
	inline int frameCount2 = 0;
	inline int currFrames = 0;
	inline DWORD lastTime = 0;

	// For Child window
	inline ID3D11ShaderResourceView* ChildIcon = nullptr;

	Theme pTheme;
}



/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////           Functions           ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


__forceinline float Lerp(float a, float b, float t) {
	float prevValue = a;
	float ret = (a + (b - a) * t);
	if (prevValue <= b && ret > b) ret = b;
	else if (prevValue > b && ret < b) ret = b;
	return ret;
}

__forceinline ImColor interpolateColors(ImColor col1, ImColor col2, float t) {
	float r = col1.Value.x + (col2.Value.x - col1.Value.x) * t;
	float g = col1.Value.y + (col2.Value.y - col1.Value.y) * t;
	float b = col1.Value.z + (col2.Value.z - col1.Value.z) * t;
	float a = col1.Value.w + (col2.Value.w - col1.Value.w) * t;
	return ImColor(r, g, b, a);
}

__forceinline ImU32 convColor(ImColor col) {
	col.Value.w *= Menu::pAnimation;
	return ImGui::ColorConvertFloat4ToU32(col);
}

__forceinline ImU32 convColor(ImColor col, float w) {
	col.Value.w *= Menu::pAnimation * w;
	return ImGui::ColorConvertFloat4ToU32(col);
}


__forceinline ImU32 convColorTab(ImColor col) {
	col.Value.w *= Menu::pAnimation * Menu::m_pTabs[Menu::selectedTab].animation;
	return ImGui::ColorConvertFloat4ToU32(col);
}

__forceinline ImU32 convColorTab(ImColor col, float w) {
	col.Value.w *= Menu::pAnimation * w * Menu::m_pTabs[Menu::selectedTab].animation;
	return ImGui::ColorConvertFloat4ToU32(col);
}