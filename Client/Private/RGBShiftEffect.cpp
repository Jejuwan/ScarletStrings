#include "ClientPCH.h"
#include "RGBShiftEffect.h"

#include "GameInstance.h"

CRGBShiftEffect::CRGBShiftEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScreenEffect(_pDevice, _pContext)
{
}

CRGBShiftEffect::CRGBShiftEffect(const CRGBShiftEffect& _rhs)
	: CScreenEffect(_rhs)
{
}

HRESULT CRGBShiftEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRGBShiftEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRGBShiftEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CRGBShiftEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CRGBShiftEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CRGBShiftEffect::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CRGBShiftEffect::Render", "Failed to CScreenEffect::Render");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 11)))
	{
		MSG_RETURN(E_FAIL, "CCCTV::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CRGBShiftEffect::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
}

HRESULT CRGBShiftEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CRGBShiftEffect> CRGBShiftEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRGBShiftEffect> pInstance = make_private_shared(CRGBShiftEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRGBShiftEffect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRGBShiftEffect::Clone(any)
{
	shared_ptr<CRGBShiftEffect> pInstance = make_private_shared_copy(CRGBShiftEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRGBShiftEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
