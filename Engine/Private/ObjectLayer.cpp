#include "EnginePCH.h"
#include "ObjectLayer.h"
#include "GameObject.h"
#include "GameInstance.h"

CObjectLayer::CObjectLayer(const SCENE _eScene)
	: m_eScene(_eScene)
{
}

void CObjectLayer::Tick(_float _fTimeDelta)
{
	if (false == m_bEnable)
		return;

	_float fScaledTimeDelta = _fTimeDelta * m_fTimeScale;
	for (auto& pObject : m_usetObject)
	{
		if (pObject->Get_Enable())
			pObject->Tick(fScaledTimeDelta);
	}
}

void CObjectLayer::Late_Tick(_float _fTimeDelta)
{
	if (false == m_bEnable)
		return;

	_float fScaledTimeDelta = _fTimeDelta * m_fTimeScale;
	for (auto& pObject : m_usetObject)
	{
		if (pObject->Get_Enable())
			pObject->Late_Tick(fScaledTimeDelta);
	}
	for (auto iter = m_usetObject.begin(); iter != m_usetObject.end();)
	{
		if ((*iter)->Return())
		{
			(*iter)->Release();
			iter = m_usetObject.erase(iter);
		}
		else
			++iter;
	}
}

HRESULT CObjectLayer::Add(shared_ptr<class CGameObject> _pObject)
{
	if (nullptr == _pObject)
	{
		MSG_RETURN(E_FAIL, "CObjectLayer::Add", "Null Exception");
	}

	_pObject->CheckOut();

	m_usetObject.emplace(_pObject);

	return S_OK;
}

HRESULT CObjectLayer::Delete(shared_ptr<class CGameObject> _pObject)
{
	auto iter = m_usetObject.find(_pObject);
	if (iter == m_usetObject.end())
	{
		MSG_RETURN(E_FAIL, "CObjectLayer::Delete", "Failed to find");
	}

	m_usetObject.erase(iter);

	return S_OK;
}

void CObjectLayer::Iterate_Objects(function<_bool(shared_ptr<class CGameObject>)> _funcCallback)
{
	for (auto iter : m_usetObject)
	{
		if (!_funcCallback(iter))
		{
			return;
		}
	}
}

shared_ptr<class CGameObject> CObjectLayer::Get_GameObject(_uint idx)
{
	_uint curIdx = 0;
	for (ObjectIterator it = Begin(); it != End(); ++it)
	{
		if (curIdx == idx)
			return *it;
		curIdx++;
	}
	return nullptr;
}

_bool CObjectLayer::Contains(shared_ptr<CGameObject> _pObject)
{
	return m_usetObject.find(_pObject) != m_usetObject.end();
}

shared_ptr<CObjectLayer> CObjectLayer::Create(const SCENE _eScene)
{
	return make_private_shared(CObjectLayer, _eScene);
}
