#include <D3DX11tex.h>

#include "render.h"
#include "../Icons.h"
#include "../Cheat/Features/Features.h"
#include "../Framework/Menu.h"

#ifdef _DEBUG
#include "../Cheat/util/Console.h"
#else
#include <iostream> // for "buttonfunction"
#endif

// delete this
bool value = 0;
float value2 = 1.f;
bool value3 = 0;
float value4 = 1.f;
ImColor pColor = ImColor(255, 255, 255, 255);
ID3D11ShaderResourceView* icon;
std::vector<std::string> options = { "option 1", "option 2", "option 3" };
int selectedOption = 0;

void buttonfunction() {
    std::cout << "hello world" << std::endl;
}
//

void InitMenu() {
    if (Menu::isInit) return; // Dont init twice

    // Init imgui with d3d11
    Menu::pD3D11.Init(Render::Window);

    // Create icon for tab
    D3DX11_IMAGE_LOAD_INFO info;
    ID3DX11ThreadPump* pump{ nullptr };
    D3DX11CreateShaderResourceViewFromMemory(Menu::pD3D11.g_pd3dDevice, _accombat, sizeof(_accombat), &info, pump, &icon, 0);

    // -- First tab (Example) --
    Menu::m_pTabs.push_back(Tab("Example", 0));

    Menu::pData.addChild("Child Window");

    Menu::pData.m_pChildWindows.back().addSlider("Value", &value2, 0, 300.f, "%.1f");

    Menu::pData.m_pChildWindows.back().addCheckbox("Checkbox", &value);

    Menu::m_pTabs.back().addCheckbox("Example with Child", &Features::Aimbot::enabled, Menu::pData.m_pChildWindows.size() - 1);

    Menu::m_pTabs.back().addCheckbox("Checkbox", &value);

    Menu::m_pTabs.back().addColorPicker("Color Picker", &pColor);

    Menu::m_pTabs.back().addSeperator();

    Menu::m_pTabs.back().addDropDown("Dropdown", &selectedOption, &options);

    Menu::m_pTabs.back().addButton("Button1", "click", buttonfunction);

    Menu::m_pTabs.back().addSeperator();

    Menu::m_pTabs.back().addSlider("x scale", &Menu::pData.pWScale, 0.7f, 1.6f, "%.2f");

    Menu::m_pTabs.back().addSlider("y scale", &Menu::pData.pHScale, 0.7f, 1.6f, "%.2f");

    Menu::m_pTabs.back().addSlider("spacing", &Menu::pData.pSpacing, 0.f, 50.f, "%.2f");

    // -- Second tab (Combat) --
    Menu::m_pTabs.push_back(Tab("Combat", &icon));

    //
    Menu::pData.addChild("Aimbot (legit)");

    Menu::pData.m_pChildWindows.back().addSlider("Fov", &Features::Aimbot::fov, 0, 300.f, "%.1f");

    Menu::pData.m_pChildWindows.back().addSlider("Smoothing", &Features::Aimbot::smoothing, 0.f, 20.f, "%.2f");

    Menu::m_pTabs.back().addCheckbox("Aimbot (legit)", &Features::Aimbot::enabled, Menu::pData.m_pChildWindows.size() - 1);
    // Aimbot


    // -- Third tab (Visuals) --
    Menu::m_pTabs.push_back(Tab("Visuals", &icon));

    Menu::m_pTabs.back().addCheckbox("Esp", &Features::Esp::enabled);


    // IMPORTANT!!!
    // Init child windows
    for (int i = 0; i < Menu::pData.m_pChildWindows.size(); i++) {
        Menu::pData.m_pChildWindows[i].calcSize();
    }

    // Is initialized
    Menu::isInit = true;
    return;
}

bool Init() {
    // Init d3d11
    if (FAILED(Menu::pD3D11.g_pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Menu::pD3D11.g_pd3dDevice))) return false;

    // Context
    Menu::pD3D11.g_pd3dDevice->GetImmediateContext(&Menu::pD3D11.g_pd3dDeviceContext);
    DXGI_SWAP_CHAIN_DESC sd;
    Menu::pD3D11.g_pSwapChain->GetDesc(&sd);

    // Get Window
    Render::Window = sd.OutputWindow;
    Menu::pData.Window = sd.OutputWindow;

    // Create Render Target
    ID3D11Texture2D* pBackBuffer = nullptr;
    if (FAILED(Menu::pD3D11.g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)) ||
        FAILED(Menu::pD3D11.g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &Menu::pD3D11.g_mainRenderTargetView))
        ) {
        pBackBuffer->Release();
        return false;
    }

    pBackBuffer->Release();
    return true;
}


#include "../Cheat/Cheat.h" // Cheat::update();
#include <global.h>
HRESULT __fastcall Render::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    // Init after resize or on first run
    if (Menu::pD3D11.g_mainRenderTargetView == nullptr)
    {
        Menu::pD3D11.g_pSwapChain = pSwapChain;
        if (!Init())
            return oPresent(pSwapChain, SyncInterval, Flags);
    }
    Menu::pD3D11.g_pd3dDeviceContext->OMSetRenderTargets(1, &Menu::pD3D11.g_mainRenderTargetView, nullptr);

    // Init Menu only once
    if (!Menu::isInit) InitMenu();

    // Start frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    pImDrawList = ImGui::GetBackgroundDrawList();
    vecDisplaySize = ImGui::GetIO().DisplaySize;
    flDeltaTime = ImGui::GetIO().DeltaTime;
    dCurrTime = GetTickCount();

    // Update cheat
    Cheat::update();

    // Render menu
    Menu::onRender();

    // Render
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT __fastcall Render::hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) {
    // Clear main render target
    if (Menu::pD3D11.g_mainRenderTargetView != nullptr)
    {
        Menu::pD3D11.g_mainRenderTargetView->Release();
        Menu::pD3D11.g_mainRenderTargetView = nullptr;
    }

    return oCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
}

HRESULT __fastcall Render::hkResizeBuffers(IDXGISwapChain* pSwapChain, uint32_t nBufferCount, uint32_t nWidth, uint32_t nHeight, DXGI_FORMAT newFormat, uint32_t nFlags) {
    // Init
    Menu::pD3D11.g_pSwapChain = pSwapChain;
    HRESULT hr = oResizeBuffers(pSwapChain, nBufferCount, nWidth, nHeight, newFormat, nFlags);
    Init();
    return hr;
}

__int64 __fastcall Render::hkTranslateMessages(void* a1, void* a2, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnda, void* a8, void* a9, void* a10, void* a11, int a12, void* a13, void* a14) {
    // Handle messages
    if (!Menu::WndProc(hWnd, uMsg, wParam, lParam)) {
        return oTranslateMessages(a1, a2, hWnd, uMsg, wParam, lParam, hWnda, a8, a9, a10, a11, a12, a13, a14);
    }

    return 0;
}