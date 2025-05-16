#include "ClientPCH.h"
#include "Arashi_Weapon.h"

#include "GameInstance.h"

#include "PhysXCollider.h"

CArashi_Weapon::CArashi_Weapon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CTeammate_Weapon(pDevice, pContext)
{
}

CArashi_Weapon::CArashi_Weapon(const CArashi_Weapon& rhs)
	: CTeammate_Weapon(rhs)
{
}

HRESULT CArashi_Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CArashi_Weapon::Initialize_Prototype", "Failed to CTeammate_Weapon::Initialize_Prototype");
	}

	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_ARASHI_WEAPON, g_aNull);

	return S_OK;
}

HRESULT CArashi_Weapon::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CArashi_Weapon::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CArashi_Weapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CArashi_Weapon::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CArashi_Weapon::Render()
{
	if (FAILED(CGameObject::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Weapon::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CArashi_Weapon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CArashi_Weapon::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CArashi_Weapon> CArashi_Weapon::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CArashi_Weapon> pInstance = make_private_shared(CArashi_Weapon, pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CArashi_Weapon::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CArashi_Weapon::Clone(any)
{
	shared_ptr<CArashi_Weapon> pInstance = make_private_shared_copy(CArashi_Weapon, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CArashi_Weapon::Clone", "Failed to Initialize");
	}

	return pInstance;
}
