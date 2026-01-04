#include "pch.h"
#include "Overlay.h"
#include <dwmapi.h>
#include "fonticon.h"
#include "widgets.h"
#include "gui.h"
HWND Overlay::hOwner = nullptr;
HWND Overlay::hwnd = nullptr;
ImFont* Overlay::IconFont = nullptr;
ImFont* Overlay::NormalFont = nullptr;
ID3D11Device* Overlay::g_pd3dDevice = nullptr;
ID3D11DeviceContext* Overlay::g_pd3dDeviceContext = nullptr;
IDXGISwapChain* Overlay::g_pSwapChain = nullptr;
bool Overlay::g_SwapChainOccluded = false;
UINT Overlay::g_ResizeWidth = 0, Overlay::g_ResizeHeight = 0;
ID3D11RenderTargetView* Overlay::g_mainRenderTargetView = nullptr;
void Overlay::CreateOverlay(HWND hOwner, float fontSize) {
	WINDOWINFO pwi = { 0 };
	pwi.cbSize = 60;
	GetWindowInfo(hOwner, &pwi);
	HWND hwnd = ::CreateWindowExW(WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE, obf(L"STATIC"), nullptr, WS_VISIBLE | WS_POPUP, pwi.rcClient.left, pwi.rcClient.top, pwi.rcClient.right - pwi.rcClient.left, pwi.rcClient.bottom - pwi.rcClient.top, nullptr, nullptr, nullptr, NULL);
	if (hwnd == nullptr) {
		MessageBoxW(nullptr, obf(L"Crashed while creating the rendering overlay."), nullptr, MB_SYSTEMMODAL | MB_ICONERROR);
		TerminateProcess(GetCurrentProcess(), 0);
		return;
	}
	//这是消息回调函数
	Overlay::hOwner = hOwner;
	Overlay::hwnd = hwnd;
	HookOwnerWndProc();
	SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)Overlay::fnOwnerWndProc);
	// 添加穿透属性
	SetWindowLongA(hwnd, -20, WS_EX_TRANSPARENT | WS_EX_LAYERED | GetWindowLongA(hwnd, -20));
	//设置透明风格
	SetLayeredWindowAttributes(hwnd, 0, 255, 2);
	//显示窗口
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	//设施透明
	MARGINS margin = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margin);
	//设置
	::EnableWindow(hwnd, TRUE);
	::SetWindowLongPtrA(hwnd, GWLP_HWNDPARENT, (LONG_PTR)hOwner);
	::EnableWindow(hwnd, FALSE);
	//初始化window
	if (!CreateDeviceD3D(hwnd)) {
		CleanupDeviceD3D();
		DestroyWindow(hwnd);
		MessageBoxW(nullptr, obf(L"D3D创建错误"), nullptr, MB_SYSTEMMODAL | MB_ICONERROR);
		ExitProcess(0);
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.IniFilename = nullptr;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends
	//ImGui::StyleColorsDark();
	// Make process DPI aware and obtain main monitor scale
	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	//// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	//style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


	std::string windowsDirectory;
	windowsDirectory.resize(MAX_PATH);
	GetWindowsDirectoryA(windowsDirectory.data(), UINT(windowsDirectory.size()));
	windowsDirectory = windowsDirectory.c_str();  // 去除多余的空字符
	//获取字体路径
	std::string FontNormalDirectory;
	std::string FontNormalNames[] = { "msyhbd.ttc","msyhbd.ttf","simsunb.ttc","simsunb.ttf","Dengb.ttc","Dengb.ttf" };
	for (size_t i = 0; i < FontNormalNames->size(); i++)
	{
		if (FileExistsA(windowsDirectory + "\\Fonts\\" + FontNormalNames[i])) {
			FontNormalDirectory = windowsDirectory + "\\Fonts\\" + FontNormalNames[i];
			break;
		}
	}
	if (FontNormalDirectory.size() == 0) {
		MessageBoxA(nullptr, obf("Loadding Font Failed!!!"), obf("Font Error"), 0);
		TerminateProcess(GetCurrentProcess(), 0);
	}
	// 配置字体加载参数
	NormalFont = io.Fonts->AddFontFromFileTTF(FontNormalDirectory.c_str(), SCALE(16.0f), nullptr, io.Fonts->GetGlyphRangesChineseFull());
	IconFont = io.Fonts->AddFontFromMemoryCompressedBase85TTF(font_icon_compressed_data_base85, SCALE(32.0f));

}
// Helper functions
bool Overlay::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HMODULE libD3D11 = GetModuleHandleA(obf("d3d11.dll"));
	if (libD3D11 == nullptr) {
		return false;
	}
	void* D3D11CreateDeviceAndSwapChain = (void*)GetProcAddress(libD3D11, obf("D3D11CreateDeviceAndSwapChain"));
	if (D3D11CreateDeviceAndSwapChain == NULL) {
		return false;
	}
	auto fnD3D11CreateDeviceAndSwapChain = reinterpret_cast<long(__stdcall*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**)>(D3D11CreateDeviceAndSwapChain);

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = fnD3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = fnD3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}
void Overlay::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}
void Overlay::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void Overlay::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
//Render
bool Overlay::StartRender() {
	if (hwnd == nullptr)return false;
	static DWORD64 continueTickCount = 0;
	if (continueTickCount) {
		if (GetTickCount64() + 10 > continueTickCount) {
			continueTickCount = 0;
		}
		return false;
	}
	// Handle window being minimized or screen locked
	if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	{
		continueTickCount = GetTickCount64();
		return false;
	}
	g_SwapChainOccluded = false;
	// Handle window resize (we don't resize directly in the WM_SIZE handler)
	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return true;
}
void Overlay::EndRender() {
	// Rendering
	ImGui::Render();
	ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present
	//HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}
DECLARE_NAMESPACE_BOOL_DETOUR(Overlay, OwnerWndProc)
void* Overlay::fnOwnerWndProc = nullptr;
void Overlay::HookOwnerWndProc() {

	if (fnOwnerWndProc == nullptr) {
		Overlay::fnOwnerWndProc = (void*)SetWindowLongPtrW(Overlay::hOwner, GWLP_WNDPROC, (LONG_PTR)&DetourOwnerWndProc);
	}

}
bool Overlay::IndirectOwnerWndProc(DWORD_PTR rsp) {

	HWND hwnd = *(HWND*)(rsp - 4 * 8);
	UINT uMsg = *(UINT*)(rsp - 5 * 8);
	WPARAM wParam = *(WPARAM*)(rsp - 9 * 8);
	LPARAM lParam = *(LPARAM*)(rsp - 10 * 8);
	LRESULT& lResult = *(LRESULT*)(rsp - 2 * 8);
	bool bIntercept = false;
	lResult = hkOwnerWndProc(hwnd, uMsg, wParam, lParam, bIntercept);

	return bIntercept;
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Overlay::hkOwnerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bIntercept) {

	bIntercept = false;
	if (Overlay::hOwner && hwnd == Overlay::hOwner) {
		if ((uMsg == WM_SIZE && wParam != SIZE_MINIMIZED) || uMsg == WM_MOVE) {
			WINDOWINFO pwi = { 0 };
			pwi.cbSize = 60;
			GetWindowInfo(hwnd, &pwi);
			if (Overlay::hwnd) {
				MoveWindow(Overlay::hwnd, pwi.rcClient.left, pwi.rcClient.top, pwi.rcClient.right - pwi.rcClient.left, pwi.rcClient.bottom - pwi.rcClient.top, true);
			}
			if (uMsg == WM_SIZE && wParam != SIZE_MINIMIZED) {
				g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
				g_ResizeHeight = (UINT)HIWORD(lParam);
			}
		}
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return 0;
	}
	//我的窗口
	else if (Overlay::hwnd && hwnd == Overlay::hwnd) {
		bIntercept = true;
		return ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}