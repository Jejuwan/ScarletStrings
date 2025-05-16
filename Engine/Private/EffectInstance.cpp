#include "EnginePCH.h"
#include "EffectInstance.h"
#include "Component_Manager.h"

CEffectInstance::CEffectInstance(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, EFFECT _eEffect)
	: CEffect(_pDevice, _pContext, _eEffect)
{
}

CEffectInstance::CEffectInstance(const CEffectInstance& _rhs)
	: CEffect				(_rhs)
	, m_iActivateInstances	(_rhs.m_iActivateInstances)
{
}

HRESULT CEffectInstance::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Initialize_Prototype", "Failed to CEffect::Initialize_Prototype");
	}
	
	return S_OK;
}

HRESULT CEffectInstance::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Initialize_Prototype", "Failed to CEffect::Initialize");
	}

	return S_OK;
}

void CEffectInstance::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (m_pVIBufferInstance)
	{
		m_pVIBufferInstance->Update([&](void* _pData, _uint _iNumInstance) { Update_Instance(_pData, _iNumInstance, _fTimeDelta); });
	}
}

void CEffectInstance::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CEffectInstance::Fetch(any _arg)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Fetch", "Failed to CEffect::Fetch");
	}

	if (m_pVIBufferInstance)
	{
		m_pVIBufferInstance->Update([&](void* _pData, _uint _iNumInstance) { Fetch_Instance(_pData, _iNumInstance, _arg); });
	}

	return S_OK;
}

HRESULT CEffectInstance::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Ready_Components", "Nullptr Exception: m_pShader");
	}

	m_pVIBufferInstance = Get_Component<CVIBufferInstance>(COMPONENT::VIBUFFER);
//	if (nullptr == m_pVIBufferInstance)
//	{
//		MSG_RETURN(E_FAIL, "CEffectInstance::Ready_Components", "Nullptr Exception: m_pVIBufferInstance");
//	}

	return S_OK;
}

HRESULT CEffectInstance::Render(_uint _iPassIndex, _bool bOrthographic)
{
	if (FAILED(__super::Render(_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Render", "Failed to CEffect::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (FAILED(__super::Add_Component(_eComponent, _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance::Add_Component", "Failed to CEffect::Add_Component");
	}

	switch (_eComponent)
	{
	case COMPONENT::VIBUFFER_INSTANCE_MESH:
	case COMPONENT::VIBUFFER_INSTANCE_MODEL:
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_pVIBufferInstance	= Get_Component<CVIBufferInstance>(_eComponent);
		m_iActivateInstances = m_pVIBufferInstance->Get_NumInstances();
		break;
	}

	return S_OK;
}
