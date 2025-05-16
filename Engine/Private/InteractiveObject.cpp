#include "EnginePCH.h"
#include "InteractiveObject.h"

#include "GameInstance.h"
#include "Light.h"
#include "Light_Manager.h"

CInteractiveObject::CInteractiveObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CInteractiveObject::CInteractiveObject(const CInteractiveObject& _rhs)
	: CGameObject(_rhs),
	m_fPickWorldDistance(_rhs.m_fPickWorldDistance),
	m_bCanPick(_rhs.m_bCanPick)
{
}

HRESULT CInteractiveObject::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CInteractiveObject::Initialize", "Failed to CInteractiveObject::Initialize");
	}

	return S_OK;
}

void CInteractiveObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CInteractiveObject::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CInteractiveObject::Render()
{
	return S_OK;
}

HRESULT CInteractiveObject::Render(_uint iPassIndex, _bool bOrthographic)
{
	if (FAILED(__super::Render(iPassIndex, bOrthographic)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CInteractiveObject::Create_Light(shared_ptr<class CTransform> pTransform)
{
	LIGHTDESC tLightDesc;
	tLightDesc.eLightType = LIGHTTYPE::POINT;
	tLightDesc.fRange = 5.f;
	tLightDesc.vDiffuse = _float4(0.721f, 0.239f, 0.729f, 1.f);
	tLightDesc.vAmbient = _float4(0.721f, 0.239f, 0.729f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	m_pLight = CLight::Create(tLightDesc, pTransform);
}

void CInteractiveObject::Interactive_PhychoKinesis(_bool bInteractive)
{
	if (bInteractive && false == m_bEnableLight)
	{
		m_bEnableLight = true;
		CLight_Manager::Get_Instance()->Add_Light(CGameInstance::Get_Instance()->Current_Scene(), m_pLight);
	}
	else if (false == bInteractive && m_bEnableLight)
	{
		m_bEnableLight = false;
		CLight_Manager::Get_Instance()->Erase_Light(m_pLight);
	}
}

HRESULT CInteractiveObject::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CGameObject> CInteractiveObject::Clone(any)
{
	shared_ptr<CInteractiveObject> pInstance = make_private_shared_copy(CInteractiveObject, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CInteractiveObject::Clone", "Failed to Initialize");
	}

	return pInstance;
}
