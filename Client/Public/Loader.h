#pragma once
#include "Client_Define.h"

BEGIN(Client)

class CLoader final
{
private:
	explicit CLoader(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE);
	virtual ~CLoader();

public:
	HRESULT						Initialize();
	HRESULT						Loading();

public:
	_bool						Is_Finished() const { return m_bIsFinish; };
	wstring						Get_LoadingText() const { return m_wstrLoading; };

private:
#ifdef ACTIVATE_TOOL
	HRESULT						Load_ToolScene();
	HRESULT						Load_MapToolScene();
#endif

	HRESULT						Load_TestScene();
	HRESULT						Load_RealTestScene();
	HRESULT						Load_MizuhaScene();
	HRESULT						Load_SuohScene();
	HRESULT						Load_SuohBattleScene();
	HRESULT						Load_MindRoomScene();
	HRESULT						Load_HideoutScene();
	HRESULT						Load_TitleScene();
	
#pragma region Cutscene
	HRESULT						Load_Cutscene_Mizuha();
#pragma endregion

private:
	const SCENE					m_eLoadScene;

	_bool						m_bIsFinish	= false;
	wstring						m_wstrLoading;

	HANDLE						m_hThread	= nullptr;
	CRITICAL_SECTION			m_tCriticalSection{};

	ComPtr<ID3D11Device>		m_pDevice;
	ComPtr<ID3D11DeviceContext>	m_pContext;

public:
	static shared_ptr<CLoader>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE);
	void						Free();
};


END
