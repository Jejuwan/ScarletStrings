#include "EnginePCH.h"
#include "Object_Manager.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Scene_Manager.h"
#include "GameInstance.h"

HRESULT CObject_Manager::Reserve_Manager(const SCENE _eSceneMax)
{
	m_arrPrototypes	= Function::CreateDynamicArray<Prototypes>(IDX(_eSceneMax), false);
	m_arrLayers		= Function::CreateDynamicArray<Layers>(IDX(_eSceneMax), false);
	m_arrPools		= Function::CreateDynamicArray<Pools>(IDX(_eSceneMax), false);

	m_eSceneMax		= _eSceneMax;

	return S_OK;
}

void CObject_Manager::Tick(_float _fTimeDelta)
{
	if (m_bOnLoading)
	{
		for (auto& iter : m_arrLayers[IDX(m_eLoadingScene)])
		{
			iter.second->Tick(_fTimeDelta);
		}
		for (auto& iter : m_arrPools[IDX(m_eLoadingScene)])
		{
			iter.second->Tick(_fTimeDelta);
		}

		return;
	}

	for (auto& iter : m_arrLayers[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Tick(_fTimeDelta);
	}
	for (auto& iter : m_arrPools[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Tick(_fTimeDelta);
	}

	if (m_bHitLag)
	{
		m_fLagTime -= _fTimeDelta;
		if (m_fLagTime < 0.f)
		{
			CGameInstance::Get_Instance()->Set_PhysXTimeScale(1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), TEXT("Layer:Player"), 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), TEXT("Layer:Monster"), 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), TEXT("Layer:Map_Road"), 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), TEXT("Layer:Map_Dynamic"), 1.f);
			m_fEffectTimeScale = 1.f;
			m_bHitLag = false;
			m_fLagTime = 0.1f;
		}
	}
}

void CObject_Manager::Late_Tick(_float _fTimeDelta)
{
	if (m_bOnLoading)
	{
		for (auto& iter : m_arrLayers[IDX(m_eLoadingScene)])
		{
			iter.second->Late_Tick(_fTimeDelta);
		}
		for (auto& iter : m_arrPools[IDX(m_eLoadingScene)])
		{
			iter.second->Late_Tick(_fTimeDelta);
		}

		return;
	}

	for (auto& iter : m_arrLayers[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
	for (auto& iter : m_arrPools[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
}

shared_ptr<CObjectLayer> CObject_Manager::Find_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Layer", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrLayers[IDX(_eScene)];
	auto iter = Prototype.find(_wstrLayerTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}

shared_ptr<CObjectPool> CObject_Manager::Find_Pool(const SCENE _eScene, const wstring& _wstrPoolTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Pool", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrPools[IDX(_eScene)];
	auto iter = Prototype.find(_wstrPoolTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}

HRESULT CObject_Manager::Set_TimeScale(const SCENE _eScene, const wstring& wstrLayerTag, _float fTimeScale)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Set_TimeScale", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrLayers[IDX(_eScene)];
	auto iter = Prototype.find(wstrLayerTag);
	if (iter == Prototype.end())
	{
		return E_FAIL;
	}

	iter->second->Set_TimeScale(fTimeScale);

	return S_OK;
}

_float CObject_Manager::Get_TimeScale(const SCENE _eScene, const wstring& wstrLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(0.f, "CObject_Manager::Get_TimeScale", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrLayers[IDX(_eScene)];
	auto iter = Prototype.find(wstrLayerTag);
	if (iter == Prototype.end())
	{
		return 0.f;
	}

	return iter->second->Get_TimeScale();
}

HRESULT CObject_Manager::Add_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<CGameObject> _pPrototype)
{
	if (nullptr != Find_Prototype(_eScene, _wstrPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Add_Prototype", "Already Exists: CGameObject");
	}

	m_arrPrototypes[IDX(_eScene)].emplace(_wstrPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CObjectLayer> CObject_Manager::Add_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Invalid Range: SCENE");
	}

	if (nullptr != Find_Layer(_eScene, _wstrLayerTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Already Exists: CObjectLayer");
	}

	shared_ptr<CObjectLayer> pLayer = CObjectLayer::Create(_eScene);
	if (nullptr == pLayer)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Failed to Create");
	}

	m_arrLayers[IDX(_eScene)].emplace(_wstrLayerTag, pLayer);

	return pLayer;
}

shared_ptr<CObjectPool> CObject_Manager::Add_Pool(const SCENE _eScene, const wstring& _wstrPoolTag, const wstring& _wstrPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Invalid Range: SCENE");
	}

	if (nullptr != Find_Pool(_eScene, _wstrPoolTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Already Exists: CObjectPool");
	}

	shared_ptr<CObjectPool> pPool = CObjectPool::Create(_eScene, _iPoolSize, _wstrPrototypeTag, _arg);
	if (nullptr == pPool)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Failed to Create");
	}

	m_arrPools[IDX(_eScene)].emplace(_wstrPoolTag, pPool);

	return pPool;
}

shared_ptr<CObjectPool> CObject_Manager::Add_Pool(const SCENE _eScene, const wstring& _wstrPoolTag, shared_ptr<CGameObject> _pPrototype, _uint _iPoolSize, any _arg)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Invalid Range: SCENE");
	}

	if (nullptr != Find_Pool(_eScene, _wstrPoolTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Already Exists: CObjectPool");
	}

	shared_ptr<CObjectPool> pPool = CObjectPool::Create(_eScene, _iPoolSize, _pPrototype, _arg);
	if (nullptr == pPool)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Failed to Create");
	}

	m_arrPools[IDX(_eScene)].emplace(_wstrPoolTag, pPool);

	return pPool;
}

shared_ptr<CGameObject> CObject_Manager::Clone_GameObject(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	shared_ptr<CGameObject> pGameObject = Find_Prototype(_eScene, _wstrPrototypeTag);
	if (nullptr == pGameObject)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Clone_GameObject", "Failed to Find_Prototype");
	}

	return pGameObject->Clone(_arg);
}

HRESULT CObject_Manager::Clear_Scene_Object(const SCENE _eScene)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Clear_Scene_Object", "Invalid Range: SCENE");
	}

	m_arrPrototypes[IDX(_eScene)].clear();
	m_arrLayers[IDX(_eScene)].clear();
	m_arrPools[IDX(_eScene)].clear();

	return S_OK;
}

void CObject_Manager::Iterate_Layers(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<CObjectLayer>>)> _funcCallback)
{
	for (auto& iter : m_arrLayers[IDX(_eScene)])
	{
		if (!_funcCallback(iter))
		{
			return;
		}
	}
}

void CObject_Manager::Iterate_Pools(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<CObjectPool>>)> _funcCallback)
{
	for (auto& iter : m_arrPools[IDX(_eScene)])
	{
		if (!_funcCallback(iter))
		{
			return;
		}
	}
}


shared_ptr<CGameObject> CObject_Manager::Find_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Prototype", "Invalid Range: SCENE");
	}

	auto& Prototypes = m_arrPrototypes[IDX(_eScene)];
	auto iter = Prototypes.find(_wstrPrototypeTag);
	if (iter == Prototypes.end())
	{
		return nullptr;
	}

	return iter->second;
}
