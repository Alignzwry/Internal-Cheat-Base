#include "Menu.h"
#include <global.h>

void Menu::onRender() {
    if (!isInit) return;

    // Init
    if (pData.xpos > vecDisplaySize.x - pData.width) pData.xpos = vecDisplaySize.x - pData.width;
    if (pData.ypos > vecDisplaySize.y - pData.height) pData.ypos = vecDisplaySize.y - pData.height;
    if (pData.xpos < 0.f) pData.xpos = 0.f;
    if (pData.ypos < 0.f) pData.ypos = 0.f;

    pData.childWindows.clear();

    // Fps
    frameCount2++;
    frameCount += ImGui::GetIO().Framerate;

    if (dCurrTime - lastTime > 500) {
        currFrames = (int)round(frameCount / frameCount2);
        frameCount2 = frameCount = 0;
        lastTime = dCurrTime;
    }

    pImDrawList->AddText(ImVec2(5, 5), pTheme.text, (std::to_string(currFrames) + " Fps").c_str());

    // Menu
    pAnimation = Lerp(pAnimation, isOpen ? 1.f : 0.f, pData.pAnimationSpeed * flDeltaTime);

    if (pAnimation < 0.004f) return;

    pImDrawList->AddShadowRect(ImVec2(pData.xpos, pData.ypos), ImVec2(pData.xpos + pData.width, pData.ypos + pData.height), convColor(pTheme.background2), 40.f, ImVec2(0, 0), ImDrawFlags_RoundCornersAll, 8.f);

    pImDrawList->AddRectFilled(ImVec2(pData.xpos, pData.ypos), ImVec2(pData.xpos + pData.width, pData.ypos + pData.height), convColor(pTheme.background2), 8.f);

    pImDrawList->AddRect(ImVec2(pData.xpos, pData.ypos), ImVec2(pData.xpos + pData.width, pData.ypos + pData.height), convColor(ImColor(1.f, 1.f, 1.f, 0.45f)), 8.f);

    // Tabs
    pImDrawList->AddRectFilled(ImVec2(Menu::pData.xpos + 8.f, Menu::pData.ypos + 40.f), ImVec2(Menu::pData.xpos + pData.sideBarWidth - 8.f, Menu::pData.ypos + Menu::pData.height - 8.f), convColor(pTheme.background), 8.f);

    pImDrawList->AddRectFilled(ImVec2(Menu::pData.xpos + pData.sideBarWidth, Menu::pData.ypos + 40.f), ImVec2(Menu::pData.xpos + Menu::pData.width - 8.f, Menu::pData.ypos + Menu::pData.height - 8.f), convColor(pTheme.background), 8.f);

    float y = pData.ypos + 70.f;

    // Draw tabs
    for (int i = 0; i < m_pTabs.size(); i++) {
        y += m_pTabs[i].Draw(y, selectedTab == i, hoveredTab == i) + 8.f;
    }

    // Draw child windows on top
    for (int i = 0; i < pData.childWindows.size(); i++) {
        pData.m_pChildWindows[pData.childWindows[i].index].Draw(pData.childWindows[i].pos.x, pData.childWindows[i].pos.y, pData.openedMenu == pData.childWindows[i].id);
    }
}

bool Menu::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_SIZE || msg == WM_SIZING || msg == WM_CLOSE || msg == WM_QUIT) return false;

    if (msg == WM_KEYDOWN && wParam == VK_INSERT) {
        isOpen = !isOpen;
        if (isOpen && pAnimation < 0.01f) {
            // Open menu
        }
        else {
            // Close menu
        }
        return true;
    }

    if (!isOpen) {
        if (msg == WM_KEYDOWN) {
            for(int i = 0; i < m_pTabs.size(); i++) {
                if (m_pTabs[i].checkHotkeys(wParam)) return true;
			}
        }
        return false;
    }

    // Child windows Input
    if (pData.openedMenu != -1) {
        for (int i = 0; i < pData.childWindows.size(); i++) {
            if (pData.childWindows[i].id != pData.openedMenu) continue;

			if (pData.m_pChildWindows[pData.childWindows[i].index].Input(msg, wParam, lParam)) return true;
		}

        return false;
    }

    // Tabs Input
    int lastSelectedTab = selectedTab;
    for (int i = 0; i < m_pTabs.size(); i++) {
        if (m_pTabs[i].Input(msg, wParam, lParam, i)) {
            if (msg == WM_LBUTTONDOWN && lastSelectedTab != selectedTab) {
                m_pTabs[lastSelectedTab].reset();
            }
            return true;
        }
    }

    // Tabs did not use any input
    float x = (float)LOWORD(lParam);
    float y = (float)HIWORD(lParam);

    switch (msg)
    {
    case WM_SETCURSOR:
        return true;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return true;
        break;
    case WM_LBUTTONDOWN:
        if (y < 0.f) return false;
        if (pData.xpos <= x && x <= pData.xpos + pData.width && pData.ypos <= y && y <= pData.ypos + pData.height) {
            moving_offx = x - pData.xpos;
            moving_offy = y - pData.ypos;
            moving = 1;
            return true;
        }
        return true;
    case WM_LBUTTONUP:
        if (moving == 1) {
            moving = 0;
            return 1;
        }
        return true;
    case WM_MOUSEMOVE:
        if (moving) {
            pData.xpos = x - moving_offx;
            pData.ypos = y - moving_offy;
            if (pData.xpos > vecDisplaySize.x - pData.width) pData.xpos = vecDisplaySize.x - pData.width;
            if (pData.ypos > vecDisplaySize.y - pData.height) pData.ypos = vecDisplaySize.y - pData.height;
            if (pData.xpos < 0.f) pData.xpos = 0.f;
            if (pData.ypos < 0.f) pData.ypos = 0.f;
            return true;
        }
        return true;
    }
    return false;
}

void Data::addChild(std::string label) {
    this->m_pChildWindows.push_back(ChildWindow(label));
}