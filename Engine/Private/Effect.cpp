#include "EnginePCH.h"
#include "Effect.h"

CEffect::CEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, EFFECT _eEffect)
	: CGameObject	(_pDevice, _pContext)
	, m_eEffect		(_eEffect)
{
}

CEffect::CEffect(const CEffect& _rhs)
	: CGameObject	(_rhs)
	, m_eEffect		(_rhs.m_eEffect)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Initialize_Prototype", "Failed to CGameObject::Initialize_Prototype");
	}

	return S_OK;
}

HRESULT CEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CEffect::Tick(_float _fTimeDelta)
{
	m_fTimeAcc += _fTimeDelta;

	__super::Tick(_fTimeDelta);
}

void CEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CEffect::Fetch(any)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Fetch", "Failed to CGameObject::Fetch");
	}

	m_bRelease = false;
	m_fTimeAcc = 0.f;

	return S_OK;
}
