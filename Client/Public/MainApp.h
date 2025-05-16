#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
class CModelRenderer;
END

BEGIN(Client)

class CMainApp final : public CSingleton<CMainApp>
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT								Initialize();
	void								Tick(_float fTimeDelta);
	HRESULT								Render();

#pragma region Windows Management

	LRESULT								WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma endregion
#pragma region Timer Management

	void								Tick_Timer();
	_bool								Check_Timer(const _float fFPS);
	_float								Get_TimeDelta(const _float fFPS) const;

#pragma endregion

private:
	HRESULT								Default_Settings();
	HRESULT								Ready_Component_Prototype();
#if ACTIVATE_IMGUI
	void								ImGui_GameStatus(_float fTimeDelta, _bool* pOpen);
#endif

private:
	shared_ptr<CRenderer>				m_pMainRenderer;
	shared_ptr<CGameInstance>			m_pGameInstance;

	shared_ptr<CModelRenderer>			m_pMainModelRenderer;
#if ACTIVATE_IMGUI
	shared_ptr<class CImGui_Manager>	m_pImGui_Manager;
#endif

	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

	class PxSimulationEventCallback* pSimulationEventCallback = { nullptr };

private:
	_uint					m_iRenderCnt = { 0 };

private:
	_float					m_fTimeAcc = { 0.f };

private:
	wstring					m_strFPS;

	friend CSingleton<CMainApp>;
};

END
