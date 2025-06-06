#include "ClientPCH.h"
#include "ImGui_Manager.h"

#if ACTIVATE_IMGUI

CImGui_Manager::~CImGui_Manager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

HRESULT CImGui_Manager::Initialize(_In_ HWND _hWnd, ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	_bool bSuccess = true;

	bSuccess &= IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	bSuccess &= ImGui_ImplWin32_Init(_hWnd);
	bSuccess &= ImGui_ImplDX11_Init(_pDevice.Get(), _pContext.Get());
	ImGui::StyleColorsDark();

	return bSuccess ? S_OK : E_FAIL;
}

void CImGui_Manager::Tick()
{
	m_bEnable = m_bReserveEnable;

	if (m_bEnable)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
}

void CImGui_Manager::Render()
{
	if (m_bEnable)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

#endif
