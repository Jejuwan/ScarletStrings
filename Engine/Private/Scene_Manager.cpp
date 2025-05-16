#include "EnginePCH.h"
#include "Scene_Manager.h"
#include "Scene.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"
#include "Grid_Manager.h"
#include "Light_Manager.h"
#include "PhysXEngine.h"

void CScene_Manager::Tick(_float _fTimeDelta)
{
	if (nullptr != m_pScene)
	{
		m_pScene->Tick(_fTimeDelta);
	}
}

void CScene_Manager::Late_Tick(_float _fTimeDelta)
{
	if (nullptr != m_pScene)
	{
		m_pScene->Late_Tick(_fTimeDelta);
	}
}

#ifdef _DEBUG
void CScene_Manager::Debug()
{
	if (nullptr != m_pScene)
	{
		m_pScene->Debug();
	}
}
#endif

HRESULT CScene_Manager::Initialize(_In_ const SCENE _eStatic, _In_ const SCENE _eMax)
{
	m_eSceneStatic	= _eStatic;
	m_eSceneMax		= _eMax;

	return S_OK;
}

HRESULT CScene_Manager::Open_Scene(SCENE _eScene, shared_ptr<class CScene> _pScene, _bool bOnLoading)
{
	HRESULT hr = S_OK;
	
	if (FAILED(CObject_Manager::Get_Instance()->Clear_Scene_Object(m_eScene)))
	{
		hr = S_FALSE;
		MSG_BOX("CScene_Manager::Open_Scene", "Failed: Clear_Scene_Object");
	}
	if (FAILED(CGrid_Manager::Get_Instance()->Reset_Grids(m_eScene)))
	{
		hr = S_FALSE;
		MSG_BOX("CScene_Manager::Open_Scene", "Failed: Clear_Scene_Object");
	}
	if (bOnLoading && FAILED(CLight_Manager::Get_Instance()->Clear_Lights(m_eScene)))
	{
		hr = S_FALSE;
		MSG_BOX("CScene_Manager::Open_Scene", "Failed: Clear_Lights");
	}

	CObject_Manager::Get_Instance()->Loading(bOnLoading, _eScene);

	m_eScene = _eScene;
	m_pScene = _pScene;

	return hr;
}
