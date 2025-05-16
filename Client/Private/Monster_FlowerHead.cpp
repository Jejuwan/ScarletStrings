#include "ClientPCH.h"
#include "Monster_FlowerHead.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "PlayerIdleState.h"
#include "FlowerHead_FSM.h"
#include "Bone.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion


CMonster_FlowerHead::CMonster_FlowerHead(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_FlowerHead::CMonster_FlowerHead(const CMonster_FlowerHead& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_FlowerHead::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERHEAD, g_aNull);

	return S_OK;
}

HRESULT CMonster_FlowerHead::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Initialize", "Failed to CGameObject::Initialize");
	}
	m_eMonsterType = FLOWERHEAD;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f };
	cDesc.height = 2.f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	m_tEntityDesc.fHP = 20.f;
	m_tEntityDesc.fMaxHP = 20.f;

	if (pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		_float4 InitPos = any_cast<_float4>(Any);
		InitPos.y += 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
		m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
		InitPos.w = 1.f;
		cDesc.position = { InitPos.x,InitPos.y,InitPos.z };
		m_tEntityDesc.fHP = 20000.f;
		m_tEntityDesc.fMaxHP = 20000.f;
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;


	//m_eCurState = make_shared<CPlayerIdleState>();
	//m_eCurState->Entry(*this);

	/*m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;
	m_pCollider->Initialize(desc);

	pGameInstance->Set_Player(this);*/
	//m_pModel->Set_Animation(3);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster_FlowerHead::SetWeaphoneBoneNameLeft, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CMonster_FlowerHead::SetWeaphoneBoneNameRight, this));

	m_pFSM = CFlowerHead_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 0.2f;
	desc.fCapsuleHeight = 0.5f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	m_pAttackCollider->Initialize(desc, actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 0.7f;
	desc.fCapsuleHeight = 0.9f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);


	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleHeight = 1.f;
	desc.fCapsuleRadius = 1.3f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;
	m_pAttackActorDesc->bActive = false;

	SetWeaphoneBoneNameLeft();

#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	return S_OK;
}

void CMonster_FlowerHead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
}

void CMonster_FlowerHead::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	m_pFSM->Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::OUTLINE);

#pragma region UI
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
	m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

	m_pUI_Hp->Set_MonsterHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_Hp->Set_MonsterNameTag(Get_TypeName());
	m_pUI_Hp->Late_Tick(fTimeDelta);
	if (m_tEntityDesc.fHP < m_tEntityDesc.fMaxHP)
	{
		m_pUI_Hp->Set_IndicatorPosition(m_pCamera_Player->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	}
#pragma endregion
}

HRESULT CMonster_FlowerHead::Render()
{
	/*if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}*/

	if (FAILED(__super::Render(15)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CMonster_FlowerHead::RootMotion(_float fTimeDelta, _bool OneFrame)
{	
	//¸ÁÇÒ ²É´ë°¡¸®»õ±â ÇÇ¹þÀÌ ¿Ö ´Ù¸¥¾Ö¶û Á¤¹Ý´ëÀÎµ¥
	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(XMMatrixRotationX(XMConvertToRadians(90.f)) * m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//cout << vUpdatePos.x<< '\t' << vUpdatePos.y << '\t' << vUpdatePos.z << endl;
	_vector MoveDir = XMVector3TransformNormal((m_vPreRootPos - m_pModel->Get_RootPos()), m);
	//cout << XMVectorGetX(m_pModel->Get_RootPos()) << '\t' << XMVectorGetY(m_pModel->Get_RootPos()) << '\t' << XMVectorGetZ(m_pModel->Get_RootPos()) << endl;
	//cout << XMVectorGetX(MoveDir) << '\t' << XMVectorGetY(MoveDir) << '\t' << XMVectorGetZ(MoveDir) << endl;

	vUpdatePos += MoveDir;

	vUpdatePos.w = 1.f;

	_float4 vPosition = { vUpdatePos.x,vUpdatePos.y,vUpdatePos.z,1.f };
	m_pTransform->Set_State(TRANSFORM::POSITION, XMLoadFloat4(&vPosition));

	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	//cout << vDisp.x<< '\t' << vDisp.y << '\t' << vDisp.z << endl;
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fTime = 0.f;
	}

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));

	m_vPreRootPos = m_pModel->Get_RootPos();
}

HRESULT CMonster_FlowerHead::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(CGameObject::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CMonster_FlowerHead::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerHead::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

HRESULT CMonster_FlowerHead::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Snail::Ready_Behaviors", "Failed to CMonster::Ready_Behaviors");
	}

	return S_OK;
}

shared_ptr<CMonster_FlowerHead> CMonster_FlowerHead::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_FlowerHead> pInstance = make_private_shared(CMonster_FlowerHead, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_FlowerHead::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_FlowerHead::Clone(any pArg)
{
	shared_ptr<CMonster_FlowerHead> pInstance = make_private_shared_copy(CMonster_FlowerHead, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_FlowerHead::Clone", "Failed to Initialize");
	}

	return pInstance;
}
