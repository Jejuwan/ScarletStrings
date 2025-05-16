#include "ClientPCH.h"
#include "ScreenRippleEffect.h"

#include "GameInstance.h"
#include "Pipeline.h"

#include "SceneCapture.h"

CScreenRippleEffect::CScreenRippleEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScreenEffect(_pDevice, _pContext)
{
}

CScreenRippleEffect::CScreenRippleEffect(const CScreenRippleEffect& _rhs)
	: CScreenEffect(_rhs)
{
}

HRESULT CScreenRippleEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenRippleEffect::Initialize(any _tRippleDesc)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	RIPPLEDESC tRippleDesc = any_cast<RIPPLEDESC>(_tRippleDesc);
	m_fRippleSpeed = tRippleDesc.fRippleSpeed;
	m_fRippleStrength = tRippleDesc.fRippleStrength;

	return S_OK;
}

void CScreenRippleEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CScreenRippleEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CScreenRippleEffect::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Render", "Failed to CScreenEffect::Render");
	}

	if (FAILED(m_pShader->Bind_Float("g_fRippleSpeed", m_fRippleSpeed)))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Render", "Failed to Bind_Float: Ripple Speed");
	}
	if (FAILED(m_pShader->Bind_Float("g_fRippleStrength", m_fRippleStrength)))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Render", "Failed to Bind_Float: Ripple Strength");
	}

	if (FAILED(m_pVIBuffer_Rect->Render(m_pShader, 10)))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CScreenRippleEffect::Set_Enable(_bool bEnable)
{
	__super::Set_Enable(bEnable);
}

HRESULT CScreenRippleEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CScreenRippleEffect> CScreenRippleEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any tRippleDesc)
{
	shared_ptr<CScreenRippleEffect> pInstance = make_private_shared(CScreenRippleEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(tRippleDesc)))
	{
		MSG_RETURN(nullptr, "CScreenRippleEffect::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CScreenRippleEffect::Clone(any)
{
	shared_ptr<CScreenRippleEffect> pInstance = make_private_shared_copy(CScreenRippleEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScreenRippleEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
