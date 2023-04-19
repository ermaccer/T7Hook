#include "gui_impl.h"
#include "log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "font.h"
#include "..\plugin\Menu.h"
#include "..\plugin\Settings.h"
#include "notifications.h"
#include "..\helper\eKeyboardMan.h"

bool  GUIImplementation::ms_bInit;
bool  GUIImplementation::ms_bFailed;
bool  GUIImplementation::ms_bShouldReloadFonts;;
HWND  GUIImplementation::ms_hWindow;
ID3D11RenderTargetView* GUIImplementation::ms_pRenderTarget;
WNDPROC GUIImplementation::ms_pWndProc;

void GUIImplementation::Init()
{
	eLog::Message(__FUNCTION__, "INFO: Init");
	ms_bInit = false;
	ms_bFailed = false;
	ms_hWindow = 0;
	ms_pRenderTarget = nullptr;
	ms_bShouldReloadFonts = false;
	ms_pWndProc = 0;
}

bool GUIImplementation::ImGui_Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWindow)
{
	if (!ImGui::CreateContext())
	{
		eLog::Message(__FUNCTION__, "Failed to create ImGui context!");
		return false;
	}

	ImGui::GetIO().ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	if (!ImGui_ImplWin32_Init(ms_hWindow))
	{
		eLog::Message(__FUNCTION__, "Failed to init Win32 Backend!");
		return false;
	}

	if (!ImGui_ImplDX11_Init(pDevice, pContext))
	{
		eLog::Message(__FUNCTION__, "Failed to init DX11 Backend!");
		return false;
	}

	ms_pWndProc = (WNDPROC)SetWindowLongPtr(ms_hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

	ImGui_SetStyle();
	eLog::Message(__FUNCTION__, "INFO: Init OK");
	return true;
}

void GUIImplementation::ImGui_SetStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 6.0f;
	style->ItemSpacing = ImVec2(7, 5.5);
	style->FrameRounding = 2.0f;
	style->FramePadding = ImVec2(6, 4.25);
	
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		ImVec4 col = style->Colors[i];

		if (i == ImGuiCol_Text || i == ImGuiCol_TextDisabled ||
			i == ImGuiCol_WindowBg || i == ImGuiCol_MenuBarBg) continue;

		float avg = (col.x + col.y + col.z) / 3;
		style->Colors[i] = { avg, avg,avg , col.w };
	}

	ImGui_ReloadFont();
}

void GUIImplementation::ImGui_ReloadFont()
{
	float fontSize = 16.0f;
	float fMenuScale = SettingsMgr->fMenuScale;
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiIO io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromMemoryCompressedTTF(Font_compressed_data, Font_compressed_size, fontSize * fMenuScale);
	io.Fonts->Build();
	ImGui_ImplDX11_InvalidateDeviceObjects();

	if (ms_bShouldReloadFonts)
		ms_bShouldReloadFonts = false;
}

void GUIImplementation::OnPresent(IDXGISwapChain* pSwapChain)
{
	if (ms_bFailed)
		return;

	if (!ms_bInit)
		OnPresent_GUIStart(pSwapChain);


	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

	HRESULT hResult = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
	if (FAILED(hResult)) return;

	pDevice->GetImmediateContext(&pContext);

	ImGui_Process(pContext);
}

void GUIImplementation::OnPresent_GUIStart(IDXGISwapChain* pSwapChain)
{
	if (ms_bInit)
		return;

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11Texture2D* pBackBuffer = nullptr;

	HRESULT hResult = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
	if (FAILED(hResult))
	{
		ms_bFailed = true;
		eLog::Message(__FUNCTION__, "ERROR: Failed to obtain D3D11 device! Error code: 0x%X", hResult);
		return;
	}

	pDevice->GetImmediateContext(&pContext);
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	pSwapChain->GetDesc(&swapChainDescription);

	ms_hWindow = swapChainDescription.OutputWindow;

	if (!ms_hWindow)
	{
		ms_bFailed = true;
		eLog::Message(__FUNCTION__, "ERROR: Failed to obtain D3D11 device window!");
		return;
	}

	hResult = pSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	if (FAILED(hResult))
	{
		ms_bFailed = true;
		eLog::Message(__FUNCTION__, "ERROR: Failed to obtain D3D11 device backbuffer! Error code: 0x%X", hResult);
		return;
	}

	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &ms_pRenderTarget);
	pBackBuffer->Release();

	if (ImGui_Init(pDevice, pContext, ms_hWindow))
	{
		ms_bInit = true;
		ms_bFailed = false;
		eLog::Message(__FUNCTION__, "INFO: Init OK");
	}

}

void GUIImplementation::ImGui_Process(ID3D11DeviceContext* pContext)
{
	if (!ms_bInit)
		return;

	if (!pContext)
		return;

	if (ms_bShouldReloadFonts)
		ImGui_ReloadFont();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = false;

	static bool ms_bFirstDraw = true;

	if (ms_bFirstDraw)
	{
		Notifications->SetNotificationTime(7500);
		Notifications->PushNotification("T7Hook %s is running! Press %s to open the menu. Build date: %s\n", T7HOOK_VERSION, eKeyboardMan::KeyToString(SettingsMgr->iMenuOpenKey), __DATE__);
		ms_bFirstDraw = false;
	}
	Notifications->Draw();
	TheMenu->Draw();

	ImGui::EndFrame();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &ms_pRenderTarget, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void GUIImplementation::OnBeforeResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

	HRESULT hResult = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
	if (FAILED(hResult))
		return;

	assert(pDevice);

	pDevice->GetImmediateContext(&pContext);

	if (ms_pRenderTarget)
	{
		pContext->OMSetRenderTargets(0, nullptr, nullptr);
		ms_pRenderTarget->Release();
	}

}

void GUIImplementation::OnAfterResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11Texture2D* pBackBuffer = nullptr;

	HRESULT hResult = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
	if (FAILED(hResult))
		return;

	assert(pDevice);
	pDevice->GetImmediateContext(&pContext);

	hResult = pSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	if (FAILED(hResult))
		return;

	assert(pBackBuffer);

	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &ms_pRenderTarget);
	pBackBuffer->Release();

	D3D11_VIEWPORT viewport;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;



	pContext->OMSetRenderTargets(1, &ms_pRenderTarget, NULL);
	pContext->RSSetViewports(1, &viewport);

}

float GUIImplementation::GetDeltaTime()
{
	float delta = 1.0f / 60.0f;
	if (ms_bInit)
		delta = 1.0f / ImGui::GetIO().Framerate;

	return delta;
}

void GUIImplementation::RequestFontReload()
{
	ms_bShouldReloadFonts = true;
}

LRESULT WINAPI GUIImplementation::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		break;
	case WM_SETFOCUS:
		break;
	case WM_KEYDOWN:
		if (wParam == SettingsMgr->iMenuOpenKey)
			TheMenu->OnActivate();
		if (wParam == SettingsMgr->iToggleHUDKey)
			TheMenu->OnToggleHUD();
		if (wParam == SettingsMgr->iToggleSlowMoKey)
			TheMenu->OnToggleSlowMotion();
		break;
	default:
		break;
	}
	if (TheMenu->m_bIsActive)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}


	return CallWindowProc(ms_pWndProc, hWnd, uMsg, wParam, lParam);
}
