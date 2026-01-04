#pragma once
#include "imgui.h"
#include "imgui_internal.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <d3d11.h>
#include <tchar.h>

class Overlay {
public:
	static void CreateOverlay(HWND hOwner, float fontSize);
	// Forward declarations of helper functions
	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
public:
	// for use inside of your main loop to render.
	static bool StartRender();
	static void EndRender();
public:
	static void* fnOwnerWndProc;
	static void HookOwnerWndProc();
	static void(&DetourOwnerWndProc)();
	static bool IndirectOwnerWndProc(DWORD_PTR Rsp);
	static LRESULT hkOwnerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIntercept);
public:
	// winapi window requirements
	static ImFont* IconFont;
	static ImFont* NormalFont;
private:
	static HWND hOwner;
	static HWND hwnd;
	// Data
	static ID3D11Device* g_pd3dDevice;
	static ID3D11DeviceContext* g_pd3dDeviceContext;
	static IDXGISwapChain* g_pSwapChain;
	static bool                     g_SwapChainOccluded;
	static UINT                     g_ResizeWidth, g_ResizeHeight;
	static ID3D11RenderTargetView* g_mainRenderTargetView;
};