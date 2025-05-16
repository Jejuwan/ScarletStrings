#include "ClientPCH.h"
#include "Monster_Karen_Ice.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Karen_FSM.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Bone.h"

#pragma region DECAL
#include "IceDecal.h"
#pragma endregion

CMonster_Karen_Ice::CMonster_Karen_Ice(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster_Karen_Ice::CMonster_Karen_Ice(const CMonster_Karen_Ice& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMonster_Karen_Ice::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_KAREN_ICE, g_aNull);

	return S_OK;
}

HRESULT CMonster_Karen_Ice::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Initialize", "Failed to CMonster::Ready_Behaviors");
	}
	//m_tEntityDesc.fHP = m_tEntityDesc.fMaxHP = m_tEntityDesc.fKnuckBackGauge = 20;

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::BOX;
	desc.vHalfExtents = {10.f,3.f,10.f};

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.eHurtType = PLAYER_HURTTYPE::AIR;
	m_pAttackCollider->Initialize(desc, actorDesc);

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, -10.f, 0.f, 1.f));

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//m_pTransform->Set_Scale(XMVectorSet(1.f, 2.f, 1.f, 0.f));
	//m_pTransform->Set_Scale(XMVectorSet(4.f, 1.f, 4.f, 0.f));

	return S_OK;
}

void CMonster_Karen_Ice::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	m_pIceDecal->Tick(fTimeDelta);
	_float3 Position = m_pTransform->Get_State(TRANSFORM::POSITION);
	if (Position.y >= 0.f)
		m_bUpword = false;
	if (m_bUpword)
		m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * fTimeDelta * 25);
	else if (Position.y >= -3.f)
		m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * fTimeDelta * 25);
	PxTransform Transform = m_pAttackCollider->GetActor()->getGlobalPose();
	Transform.p = { Position.x,Position.y,Position.z };
	m_pAttackCollider->GetActor()->setGlobalPose(Transform);
}

void CMonster_Karen_Ice::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	_float3 Position = m_pTransform->Get_State(TRANSFORM::POSITION);
	if (Position.y >= -3.f)
		Add_RenderObject(RENDER_GROUP::NONBLEND);

	m_pIceDecal->Late_Tick(fTimeDelta);

	// TODO: Test Code
	//if (CGameInstance::Get_Instance()->Key_Down('I'))
	//{
	//	// Fetch Decal
	//	ICEDECALDESC tIceDecalDesc;
	//	tIceDecalDesc.fDuration = 5.f;
	//	tIceDecalDesc.fTimeScale = 5.f;
	//	tIceDecalDesc.fTilingFactor = 13.f;
	//	tIceDecalDesc.vDecalColor = _float4(0.1f, 0.4f, 1.3f, 1.f);
	//	tIceDecalDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	//	tIceDecalDesc.vScale = _float3(45.f, 45.f, 1.f);
	//	m_pIceDecal->Fetch(tIceDecalDesc);
	//}
	
	m_pAttackCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
}

HRESULT CMonster_Karen_Ice::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Render", "Failed to CGameObject::Render");
	}

	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to CGameObject::Bind_Transform");
	}

	return S_OK;
}

HRESULT CMonster_Karen_Ice::Fetch(any Any)
{
	m_bUpword = true;
	/*m_pTargetTransform = any_cast<shared_ptr<CTransform>>(Any);
	_float4 InitPos = { m_pTargetTransform->Get_State(TRANSFORM::POSITION) ,1.f };
	InitPos.y = -5.f;
	InitPos.w = 0.f;*/
	//__super::Fetch(InitPos);
	m_vPosition.y = -5.f;
	m_pTransform->Set_State(TRANSFORM::POSITION, m_vPosition);
	

	return S_OK;
}

HRESULT CMonster_Karen_Ice::Fetch_Decal(any Any)
{
	shared_ptr<CTransform> pTargetTransform = any_cast<shared_ptr<CTransform>>(Any);
	_float4 InitPos = { pTargetTransform->Get_State(TRANSFORM::POSITION),1.f };
	InitPos.y = 0.5f;

	// Fetch Decal
	ICEDECALDESC tIceDecalDesc;
	tIceDecalDesc.fDuration = 5.f;
	tIceDecalDesc.fTimeScale = 2.5f;
	tIceDecalDesc.fTilingFactor = 13.f;
	tIceDecalDesc.vDecalColor = _float4(0.1f, 0.4f, 1.3f, 1.f);
	tIceDecalDesc.vPosition = InitPos;
	tIceDecalDesc.vScale = _float3(30.f, 30.f, 1.f);
	m_pIceDecal->Fetch(tIceDecalDesc);
	m_vPosition = InitPos;

	return S_OK;
}

void CMonster_Karen_Ice::Start()
{
	

}

HRESULT CMonster_Karen_Ice::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pIceDecal = pGameInstance->Clone_GameObject<CIceDecal>(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_DECAL_ICE);
	if (nullptr == m_pIceDecal)
	{
		MSG_RETURN(E_FAIL, "CMonster_Karen_Ice::Ready_Components", "Failed to Clone_GameObject: IceDecal");
	}

	return S_OK;
}

HRESULT CMonster_Karen_Ice::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	//m_pTree = CKaren_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc,&//m_tMonsterDesc);
	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, m_pTree);

	return S_OK;
}

shared_ptr<CMonster_Karen_Ice> CMonster_Karen_Ice::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Karen_Ice> pInstance = make_private_shared(CMonster_Karen_Ice, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Karen_Ice::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Karen_Ice::Clone(any pArg)
{
	shared_ptr<CMonster_Karen_Ice> pInstance = make_private_shared_copy(CMonster_Karen_Ice, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Karen_Ice::Clone", "Failed to Initialize");
	}

	return pInstance;
}
