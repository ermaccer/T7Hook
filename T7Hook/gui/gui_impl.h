#pragma once
#include <d3d11.h>

class GUIImplementation {
public:
	static bool							ms_bInit;
	static bool							ms_bFailed;
	static bool							ms_bShouldReloadFonts;
	static HWND							ms_hWindow;
	static WNDPROC						ms_pWndProc;
	static ID3D11RenderTargetView*		ms_pRenderTarget;


	static void Init();
	static bool ImGui_Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWindow);
	static void ImGui_SetStyle();
	static void	ImGui_ReloadFont();

	static void OnPresent(IDXGISwapChain* pSwapChain);
	static void OnPresent_GUIStart(IDXGISwapChain* pSwapChain);
	static void ImGui_Process(ID3D11DeviceContext* pContext);

	static void OnBeforeResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	static void OnAfterResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

	static float GetDeltaTime();
	static void  RequestFontReload();

	static LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);