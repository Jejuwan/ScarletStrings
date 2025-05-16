#include "ClientPCH.h"
#include "Teammate_Weapon.h"

#include "GameInstance.h"

#include "PhysXCollider.h"

CTeammate_Weapon::CTeammate_Weapon(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CTeammate_Weapon::CTeammate_Weapon(const CTeammate_Weapon& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTeammate_Weapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTeammate_Weapon::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTeammate_Weapon::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 1.f;

	ACTORDESC attackDesc;
	attackDesc.strTag = ACTORTAG::TEAMMATE_ATTACK;
	attackDesc.stAttackDesc.iDamage = 1.f;
	attackDesc.bActive = true;
	m_pCollider->Initialize(desc, attackDesc);
	m_pCollider->SetActive(false);

	return S_OK;
}

void CTeammate_Weapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CTeammate_Weapon::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	PxTransform p;
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION) + (m_pTransform->Get_State(TRANSFORM::LOOK) * 0.5f);
	p.p = PxVec3(vPos.x, vPos.y, vPos.z);
	_float4 vQuat = XMQuaternionRotationMatrix(m_pTransform->Get_Matrix());
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pCollider->GetActor()->setGlobalPose(p);
	m_pCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
	m_pCollider->GetActorDesc().mEffectMat = m_pTransform->Get_Matrix();

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CTeammate_Weapon::Render()
{
	return S_OK;
}

void CTeammate_Weapon::Activate_Collider(_bool bActivate)
{
	m_pCollider->SetActive(bActivate);
	m_pCollider->GetActorDesc().bActive = bActivate;
}

HRESULT CTeammate_Weapon::Ready_Components()
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
