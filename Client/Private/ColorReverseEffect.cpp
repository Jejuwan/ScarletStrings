#include "ClientPCH.h"
#include "ColorReverseEffect.h"

#include "GameInstance.h"

CColorReverseEffect::CColorReverseEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScreenEffect(_pDevice, _pContext)
{
}

CColorReverseEffect::CColorReverseEffect(const CColorReverseEffect& _rhs)
	: CScreenEffect(_rhs)
{
}

HRESULT CColorReverseEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CColorReverseEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CColorReverseEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CColorReverseEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CColorReverseEffect::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Render", "Failed to CScreenEffect::Render");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 12)))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CColorReverseEffect::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
}

HRESULT CColorReverseEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CColorReverseEffect::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CColorReverseEffect> CColorReverseEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CColorReverseEffect> pInstance = make_private_shared(CColorReverseEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CColorReverseEffect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CColorReverseEffect::Clone(any)
{
	shared_ptr<CColorReverseEffect> pInstance = make_private_shared_copy(CColorReverseEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CColorReverseEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
