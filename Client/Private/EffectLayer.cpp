#include "ClientPCH.h"
#include "EffectLayer.h"
#include "Effect_Manager.h"

CEffectLayer::CEffectLayer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MULTIVFX _eMultiVFX)
	: CGameObject(_pDevice, _pContext)
	, m_eMultiVFX(_eMultiVFX)
{
}

CEffectLayer::CEffectLayer(const CEffectLayer& _rhs)
	: CGameObject(_rhs)
	, m_fTimeAcc				(0.f)
	, m_fRecentProcessEventTime	(0.f)
	, m_mmapEffectEvents		(_rhs.m_mmapEffectEvents)
	, m_lstEffectArgs			(_rhs.m_lstEffectArgs)
	, m_itArgs					(_rhs.m_itArgs)
	, m_eMultiVFX				(_rhs.m_eMultiVFX)
{
}

void CEffectLayer::Tick(_float _fTimeDelta)
{
	m_fTimeAcc += _fTimeDelta;

	auto iter = m_mmapEffectEvents.lower_bound(m_fRecentProcessEventTime);
	while (iter != m_mmapEffectEvents.end() && iter->first <= m_fTimeAcc)
	{
		if (!iter->second.second)
		{
			CEffect_Manager::Get_Instance()->Fetch(iter->second.first, *m_itArgs);
			iter->second.second = true;
			m_fRecentProcessEventTime = iter->first;
			++m_itArgs;
		}
		++iter;
	}
}

HRESULT CEffectLayer::Render()
{
	return S_OK;
}

HRESULT CEffectLayer::Fetch(any _ilArgs)
{
	m_fTimeAcc		= 0.f;

	m_lstEffectArgs	= any_cast<initializer_list<any>>(_ilArgs);
	m_itArgs		= m_lstEffectArgs.begin();

	for (auto& bCall : m_mmapEffectEvents)
	{
		bCall.second.second = false;
	}

	return S_OK;
}

_bool CEffectLayer::Return()
{
	return false;
}

HRESULT CEffectLayer::Release()
{
	m_lstEffectArgs.clear();
	m_itArgs = m_lstEffectArgs.end();

	return S_OK;
}

HRESULT CEffectLayer::Append_Effect(_float _fTime, VFX _eVFX)
{
    m_mmapEffectEvents.emplace(_fTime, make_pair(_eVFX, false));
	
	return S_OK;
}

shared_ptr<CEffectLayer> CEffectLayer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MULTIVFX _eMultiVFX)
{
	return make_private_shared(CEffectLayer, _pDevice, _pContext, _eMultiVFX);
}

shared_ptr<CGameObject> CEffectLayer::Clone(any)
{
	return make_private_shared_copy(CEffectLayer, *this);
}
