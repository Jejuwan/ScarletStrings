#include "ClientPCH.h"
#include "Loader.h"
#include "Thread_Loader.h"
#include "Scene.h"

#pragma region Scene Loader Header
#if ACTIVATE_TOOL
#include "Loader_Tool.h"
#include "Loader_MapTool.h"
#endif
#pragma endregion
#include "Loader_Test.h"
#include "Loader_Mizuha.h"
#include "Loader_RealTest.h"
#include "Loader_Suoh.h"
#include "Loader_Suoh_Battle.h"
#include "Loader_MindRoom.h"
#include "Loader_Hideout.h"
#include "Loader_Title.h"
#pragma region Cutscene
#include "Loader_Cutscene_Mizuha.h"
#pragma endregion

CLoader::CLoader(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
	, m_eLoadScene(_eLoadScene)
{
}

CLoader::~CLoader()
{
	Free();
}

HRESULT CLoader::Initialize()
{
	InitializeCriticalSection(&m_tCriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Loader_Main, this, 0, nullptr);
	if (0 == m_hThread)
	{
		MSG_RETURN(E_FAIL, "CLoader::Initialize", "Failed to _beginthreadex");
	}

	return S_OK;
}

HRESULT CLoader::Loading()
{
	HRESULT hr = { 0 };

	EnterCriticalSection(&m_tCriticalSection);

	switch (m_eLoadScene)
	{
#if ACTIVATE_TOOL
	case SCENE::TOOL:
		hr = Load_ToolScene();
		break;

	case SCENE::MAPTOOL:
		hr = Load_MapToolScene();
		break;
#endif
	case SCENE::TEST:
		hr = Load_TestScene();
		break;
	case SCENE::REALTEST:
		hr = Load_RealTestScene();
		break;
	case SCENE::MIZUHA:
		hr = Load_MizuhaScene();
		break;
	case SCENE::SUOH:
		hr = Load_SuohScene();
		break;
	case SCENE::SUOH_BATTLE:
		hr = Load_SuohBattleScene();
		break;
	case SCENE::MINDROOM:
		hr = Load_MindRoomScene();
		break;
	case SCENE::HIDEOUT:
		hr = Load_HideoutScene();
		break;
	case SCENE::TITLE:
		hr = Load_TitleScene();
		break;
#pragma region Cutscene
	case SCENE::CUTSCENE_MIZUHA:
		hr = Load_Cutscene_Mizuha();
		break;
#pragma endregion
	}

	LeaveCriticalSection(&m_tCriticalSection);

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CLoader:Loading", "Failed to Load");
	}
	else
	{
		m_bIsFinish = true;
	}

	return S_OK;
}

shared_ptr<CLoader> CLoader::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
{
	shared_ptr<CLoader> pInstance = make_private_shared(CLoader, _pDevice, _pContext, _eLoadScene);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CLoader::Create", "Failed to Initialize");
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_tCriticalSection);
}