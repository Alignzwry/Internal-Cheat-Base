#include "Menu.h"
#include "../Icons.h"
#include <D3DX11tex.h>

bool D3D11::Init(HWND hWnd) {
    //ImGui

    ImGui::CreateContext();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImGuiIO& io = ImGui::GetIO();
    ImFontAtlas* fontAtlas = new ImFontAtlas();
    ImFontConfig arialConfig;
    arialConfig.FontDataOwnedByAtlas = false;

    //all glyths
    static const ImWchar ranges2[] =
    {
        0x0020, 0xFFEE,
        0,
    };

    fontAtlas->AddFontFromMemoryTTF((void*)_acRonysiswadiArchitect4_qZmp2, sizeof(_acRonysiswadiArchitect4_qZmp2), Menu::pData.textSize, &arialConfig, &ranges2[0]);

    fontAtlas->AddFontFromMemoryTTF((void*)_acRonysiswadiArchitect4_qZmp2, sizeof(_acRonysiswadiArchitect4_qZmp2), 30.f, &arialConfig, &ranges2[0]);

    io.Fonts = fontAtlas;

    // Child Icon
    D3DX11_IMAGE_LOAD_INFO info;
    ID3DX11ThreadPump* pump{ nullptr };
    D3DX11CreateShaderResourceViewFromMemory(Menu::pD3D11.g_pd3dDevice, _acchild, sizeof(_acchild), &info, pump, &Menu::ChildIcon, 0);

    return Menu::ChildIcon != nullptr;
}

void D3D11::Destroy() {
    this->CleanupRenderTarget();

    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool D3D11::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    HRESULT res = g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (res != S_OK)
        return false;

    res = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    if (res != S_OK)
        return false;

    pBackBuffer->Release();
    return g_mainRenderTargetView != nullptr;
}

void D3D11::CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}