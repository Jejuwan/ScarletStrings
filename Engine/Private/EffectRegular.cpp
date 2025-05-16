#include "EnginePCH.h"
#include "EffectRegular.h"
#include "Component_Manager.h"

CEffectRegular::CEffectRegular(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, EFFECT _eEffect)
	: CEffect(_pDevice, _pContext, _eEffect)
{
}

CEffectRegular::CEffectRegular(const CEffectRegular& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CEffectRegular::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Initialize_Prototype", "Failed to CEffect::Initialize_Prototype");
	}

	return S_OK;
}

HRESULT CEffectRegular::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Initialize_Prototype", "Failed to CEffect::Initialize");
	}

	return S_OK;
}

void CEffectRegular::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CEffectRegular::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CEffectRegular::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Fetch", "Failed to CEffect::Fetch");
	}

	if (!_vPosition3.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Fetch", "Invalid Argument");
	}
	else
	{
		if (_vPosition3.type() == typeid(_float3))
		{
			_float3 vPosition = any_cast<_float3>(_vPosition3);
			m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
		}
		if (_vPosition3.type() == typeid(_float4x4))
		{
			m_pTransform->Set_Matrix(any_cast<_float4x4>(_vPosition3));
		}
	}

	return S_OK;
}

HRESULT CEffectRegular::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Ready_Components", "Failed to CEffect::Ready_Components");
	}
	
	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Ready_Components", "Nullptr Exception: m_pShader");
	}

	m_pVIBuffer = Get_Component<CVIBuffer>(COMPONENT::VIBUFFER);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Ready_Components", "Nullptr Exception: m_pVIBuffer");
	}

	return S_OK;
}

HRESULT CEffectRegular::Render(_uint _iPassIndex, _bool bOrthographic)
{
	if (FAILED(__super::Render(_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular::Render", "Failed to CEffect::Render");
	}

	return S_OK;
}
