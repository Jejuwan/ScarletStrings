#include "ClientPCH.h"
#include "Monster_Projectile.h"
#include "Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Model.h"
#include "Monster.h"

CMonster_Projectile::CMonster_Projectile(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster_Projectile::CMonster_Projectile(const CMonster_Projectile& _rhs)
	:CGameObject(_rhs)
{
}

CMonster_Projectile::~CMonster_Projectile()

{
}

HRESULT CMonster_Projectile::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
    m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
    m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_PROJECTILE, g_aNull);

    return S_OK;
}

HRESULT CMonster_Projectile::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pArg.has_value())
		m_ObjDesc = any_cast<TESTOBJDESC>(pArg);

	m_pTransform->Set_Scale({ 3.f,3.f,3.f,0.f });
	m_pTransform->Set_State(TRANSFORM::POSITION, 
		{ m_ObjDesc.vSpawnPos.x,m_ObjDesc.vSpawnPos.y,m_ObjDesc.vSpawnPos.z,1.f});
	m_pTransform->LookAtDir({ m_ObjDesc.vSpawnLookDir.x,m_ObjDesc.vSpawnLookDir.y,m_ObjDesc.vSpawnLookDir.z,0.f });

	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());

	m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 0.3f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.iType = (_uint)CMonster::MONSTERTYPE::WATER_PROJECTILE;
	m_pCollider->Initialize(desc, actorDesc);

	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 1.f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;

	m_pBigAttackCollider->Initialize(desc, actorDesc);
	m_pTransform->LookAt(XMVectorSet(0.f, -1.f, 0.f, 0.f));

	m_pCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
	return S_OK;
}

HRESULT CMonster_Projectile::ShootAgain(any pArg)
{
	if (pArg.has_value())
		m_ObjDesc = any_cast<TESTOBJDESC>(pArg);
	else
		return E_FAIL;

	m_pTransform->Set_Scale({ 3.f,3.f,3.f,0.f });
	m_pTransform->Set_State(TRANSFORM::POSITION,
		{ m_ObjDesc.vSpawnPos.x,m_ObjDesc.vSpawnPos.y,m_ObjDesc.vSpawnPos.z,1.f });
	m_pTransform->LookAtDir({ m_ObjDesc.vSpawnLookDir.x,m_ObjDesc.vSpawnLookDir.y,m_ObjDesc.vSpawnLookDir.z,0.f });

	return S_OK;
}

void CMonster_Projectile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_fTime += fTimeDelta;
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 10.f);
	//m_pTransform->Translate(_float4( 0,-1,0,0 )*m_fTime * m_fTime);
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pCollider->GetActor()->setGlobalPose(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	m_pBigAttackCollider->GetActor()->setGlobalPose(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	m_pCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
}

void CMonster_Projectile::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::OUTLINE);
}

HRESULT CMonster_Projectile::Render()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Projectile::Render_OutLine()
{
	return S_OK;
}

HRESULT CMonster_Projectile::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Projectile::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CMonster_Projectile> CMonster_Projectile::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Projectile> pInstance = make_private_shared(CMonster_Projectile, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Projectile::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Projectile::Clone(any pArg)
{
	shared_ptr<CMonster_Projectile> pInstance = make_private_shared_copy(CMonster_Projectile, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Projectile::Clone", "Failed to Initialize");
	}

	return pInstance;
}
