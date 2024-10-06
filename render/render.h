#pragma once
#include <Windows.h>
#include <D3D11.h>
#include <stdint.h>

typedef HRESULT(__fastcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__fastcall* ResizeBuffers) (IDXGISwapChain* pSwapChain, uint32_t nBufferCount, uint32_t nWidth, uint32_t nHeight, DXGI_FORMAT newFormat, uint32_t nFlags);
typedef HRESULT(__fastcall* CreateSwapChain) (IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
typedef __int64(__fastcall* TranslateMessages)(void* a1, void* a2, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnda, void* a8, void* a9, void* a10, void* a11, int a12, void* a13, void* a14);

namespace Render {
	inline Present oPresent = nullptr;
	inline ResizeBuffers oResizeBuffers = nullptr;
	inline CreateSwapChain oCreateSwapChain = nullptr;
	inline TranslateMessages oTranslateMessages = nullptr;
	inline HWND Window = 0;

	HRESULT __fastcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	HRESULT __fastcall hkResizeBuffers(IDXGISwapChain* pSwapChain, uint32_t nBufferCount, uint32_t nWidth, uint32_t nHeight, DXGI_FORMAT newFormat, uint32_t nFlags);
	HRESULT __fastcall hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
	__int64 __fastcall hkTranslateMessages(void* a1, void* a2, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnda, void* a8, void* a9, void* a10, void* a11, int a12, void* a13, void* a14);
}