#include "ClientPCH.h"
#include "CutsceneTentacle.h"

#include "GameInstance.h"
#include "Player.h"

CCutsceneTentacle::CCutsceneTentacle(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCutsceneTentacle::CCutsceneTentacle(const CCutsceneTentacle& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCutsceneTentacle::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_CUTSCENE_TENTACLE, g_aNull);

	return S_OK;
}

HRESULT CCutsceneTentacle::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CPlayerTentacle::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CCutsceneTentacle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);
}

void CCutsceneTentacle::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NEON);
}

HRESULT CCutsceneTentacle::Render()
{
	return S_OK;
}

HRESULT CCutsceneTentacle::Render_Neon()
{
	if (FAILED(m_pShader->Bind_RawValue("g_vStripeColor", &m_vStripeColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Render");
	}

	if (FAILED(__super::Render(22)))
	{
		MSG_RETURN(E_FAIL, "CCutsceneTentacle::Render_Neon", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CCutsceneTentacle::Render_Screen_Effect()
{
	return S_OK;
}

HRESULT CCutsceneTentacle::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CCutsceneTentacle> CCutsceneTentacle::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCutsceneTentacle> pInstance = make_private_shared(CCutsceneTentacle, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CCutsceneTentacle::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCutsceneTentacle::Clone(any)
{
	shared_ptr<CCutsceneTentacle> pInstance = make_private_shared_copy(CCutsceneTentacle, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCutsceneTentacle::Clone", "Failed to Initialize");
	}

	return pInstance;
}
