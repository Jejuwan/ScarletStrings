#include "ClientPCH.h"
#include "Monster_DispenPerry.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "UI_Manager.h"
#include "DispenPerry_BehaviorTree.h"
#include "WaterTank.h"
#include "Player.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Bone.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "UI_Indicator_Name_Boss.h"
#include "UI_Indicator_Boss_Hp.h"
#include "Camera_Player.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_ToolTip_Tutorial_Tips.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#pragma endregion

#ifdef _DEBUG
#define TEST_CODE
#endif
#define ENABLE_BEHAVIORTREE	true
#define REVERSE_ROOTMOTION	true

CMonster_DispenPerry::CMonster_DispenPerry(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_DispenPerry::CMonster_DispenPerry(const CMonster_DispenPerry& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_DispenPerry::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR) | BIT(BEHAVIOR::PHYSICS);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_DISPENPERRY, g_aNull);
//	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_BUFFALO, g_aNull);

	m_tEntityDesc.fMaxHP = m_tEntityDesc.fHP = 1000.f;
	
	m_tEntityDesc.fAtk = 0.f;

	m_tEntityDesc.fForwardSpeed = 60.f;
	m_tEntityDesc.fLeftSpeed = 60.f;
	m_tEntityDesc.fBackwardSpeed = 60.f;
	m_tEntityDesc.fRightSpeed = 60.f;
	m_tEntityDesc.fSpritPower = 1.f;
	m_tEntityDesc.fJumpPower = 60.f;

	m_tEntityDesc.vMaxSpeed = _float3(200.f, 200.f, 200.f);
	m_tEntityDesc.vResist = _float3(0.02f, 0.2f, 0.02f);

	return S_OK;
}

HRESULT CMonster_DispenPerry::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Initialize", "Failed to CMonster::Initialize");
	}
	
	m_pWaterTank = CWaterTank::Create(dynamic_pointer_cast<CMonster_DispenPerry>(shared_from_gameobject()), WATERTANK_MAXHP, WATERTANK_HEALSPD);
	m_eMonsterType = DISPENPERRY;

	Set_DamageLevel(80.f, DP_STATE::DAMAGE_I);
	Set_DamageLevel(60.f, DP_STATE::DAMAGE_M);
	Set_DamageLevel(40.f, DP_STATE::WEAK);
	Set_DamageLevel(20.f, DP_STATE::DAMAGE_M);

	m_pWaterTank->Set_DamageLevel(90.f, DP_STATE::DAMAGE_I);
	m_pWaterTank->Set_DamageLevel(80.f, DP_STATE::DAMAGE_I);
	m_pWaterTank->Set_DamageLevel(70.f, DP_STATE::DAMAGE_M);
	m_pWaterTank->Set_DamageLevel(60.f, DP_STATE::DAMAGE_I);
	m_pWaterTank->Set_DamageLevel(50.f, DP_STATE::DAMAGE_I);
	m_pWaterTank->Set_DamageLevel(40.f, DP_STATE::DAMAGE_M);
	m_pWaterTank->Set_DamageLevel(30.f, DP_STATE::WEAK);
	m_pWaterTank->Set_DamageLevel(20.f, DP_STATE::DAMAGE_I);
	m_pWaterTank->Set_DamageLevel(10.f, DP_STATE::DAMAGE_M);

	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::DISPEN_PERRY::WAIT1);

	_float4 InitPos = any_cast<_float4>(Any);
	InitPos.y += 1.f;

	m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));

	InitPos.w = 1.f;
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { InitPos.x,InitPos.y,InitPos.z };
	cDesc.height = 3.f;
	cDesc.radius = 7.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.5f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1,	std::bind(&CMonster_DispenPerry::SetWeapone1ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2,	std::bind(&CMonster_DispenPerry::SetWeapone1ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3,	std::bind(&CMonster_DispenPerry::SetWeapone2ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4,	std::bind(&CMonster_DispenPerry::SetWeapone2ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5,	std::bind(&CMonster_DispenPerry::SetWeapone3ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION6,	std::bind(&CMonster_DispenPerry::SetWeapone3ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION7,	std::bind(&CMonster_DispenPerry::SetWeapone4ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION8,	std::bind(&CMonster_DispenPerry::SetWeapone4ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION9,	std::bind(&CMonster_DispenPerry::SetWeapone5ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION10,	std::bind(&CMonster_DispenPerry::SetWeapone5ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION11,	std::bind(&CMonster_DispenPerry::SetWeapone6ActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION12,	std::bind(&CMonster_DispenPerry::SetWeapone6ActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION13,	std::bind(&CMonster_DispenPerry::SetWeaponeAllActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION14,	std::bind(&CMonster_DispenPerry::SetWeaponeAllActiveOff, this));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	for (size_t i = 0; i < 6; i++)
	{
		m_pAttack1Collider[i] = make_shared<CPhysXCollider>();
		CPhysXCollider::PHYSXCOLLIDERDESC desc;
		desc.eType = CPhysXCollider::TYPE::SPHERE;
		desc.fRadius = 2.f;

		ACTORDESC actorDesc;
		actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
		actorDesc.stAttackDesc.iDamage = 10;
		m_pAttack1Collider[i]->Initialize(desc, actorDesc);//atk
		m_pAttack1ActorDesc[i] = (ACTORDESC*)m_pAttack1Collider[i]->GetActor()->userData;
		m_pAttack1ActorDesc[i]->bActive = false;
	}

	m_pBodyCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 4.f;
	desc.fCapsuleHeight = 6.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, actorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;

	m_strAttack1BoneName[0] = "LeftHandHelp";
	m_strAttack1BoneName[1] = "LeftCenterHandHelp";
	m_strAttack1BoneName[2] = "LeftBackHandHelp";
	m_strAttack1BoneName[3] = "RightHandHelp";
	m_strAttack1BoneName[4] = "RightCenterHandHelp";
	m_strAttack1BoneName[5] = "RightBackHandHelp";

	m_strCenterBoneName = "Spine";
	m_strWeakBoneName = "Water";
#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Boss_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP));
	m_pUI_Hp_Small = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
	m_pUI_Name = dynamic_pointer_cast<CUI_Indicator_Name_Boss>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS));
	m_pUI_Hp->Set_BossHp_Initialize(this->m_tEntityDesc.fHP, this->m_tEntityDesc.fMaxHP);
	m_pUI_Name->Start_Animation_Trigger();
	m_pUI_Hp->Render_Start();
	m_bOnce1 = true;
	m_bOnce2 = true;
#pragma endregion

	m_bOriginDOFEnable = pGameInstance->Enable_DOF();
	m_fOriginDOFDepthStart = pGameInstance->Get_DOF_DepthStart();
	m_fOriginDOFDepthRange = pGameInstance->Get_DOF_DepthRange();

	m_pModel->Hide_Mesh(3);

	return S_OK;
}

_bool CMonster_DispenPerry::Detect_Ground(_float4& vGroundPosition)
{
	auto pScene = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene();
	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);

	const PxU32 iBufferSize = 16;
	PxRaycastHit thitBuffer[iBufferSize];
	PxRaycastBuffer RayCallback(thitBuffer, iBufferSize);

	PxVec3 origin = PxVec3(vPosition.x, vPosition.y + 0.4f, vPosition.z);
	PxVec3 unitDir = PxVec3(0.f, -1.f, 0.f);
	PxReal maxDistance = 1000.f;

	_bool bHit = pScene->raycast(origin, unitDir, maxDistance, RayCallback/*, PxHitFlag::Enum::eDEFAULT, FilterData*/);

	_float fMinDist = FLT_MAX;
	_bool bHitGround = false;
	PxVec3 vNearlestGround;
	if (bHit)
	{
		PxU32 nbHits = RayCallback.nbTouches;
		for (PxU32 i = 0; i < nbHits; ++i)
		{
			const PxRaycastHit& touch = RayCallback.touches[i];
			ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
			if (nullptr == pActorDesc)
			{
				bHitGround = true;

				PxF32 fDist = touch.distance;
				if (fMinDist > fDist)
				{
					fMinDist = fDist;
					vNearlestGround = touch.position;
				}
			}
		}
	}

	//if (bHitGround)
	//{
	vGroundPosition = _float4(vNearlestGround.x, vNearlestGround.y, vNearlestGround.z, 1.f);
	//}

	return bHitGround;
}

void CMonster_DispenPerry::Tick(_float _fTimeDelta)
{
	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Dispenparry Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();

	if (m_bCutscene)
	{
		if (false == m_bLanding)
		{
			CUI_Manager::Get_Instance()->Set_UI_All_RenderStopTrue();
			CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
			CGameInstance::Get_Instance()->PlayBGM(L"BGM_The_Other_Appeared.mp3", 0.5f);

			auto pGameInstance = CGameInstance::Get_Instance();
			pGameInstance->Enable_DOF(true);
			pGameInstance->Set_DOF_DepthStart(300.f);
			pGameInstance->Set_DOF_DepthRange(500.f);

			_float3 vPrevPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
			m_pTransform->Translate(XMVectorSet(0.f, -30.f, 0.f, 0.f) * _fTimeDelta);

			_vector vNestPosition = m_pTransform->Get_State(TRANSFORM::POSITION);

			_float3 vDisp;
			XMStoreFloat3(&vDisp, vNestPosition - vPrevPosition);
			PxControllerCollisionFlags eCollisionFlags = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, _fTimeDelta, m_ControllerFilters);
			if (eCollisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN)
			{
				m_pModel->Set_Animation(IDX(ANIMATION::DISPEN_PERRY::TURN_UP_LANDING), 0.8f, false, 0.2f, false);
				m_bLanding = true;

				shared_ptr<CCamera_Player> pCamera = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera();
				pCamera->Set_Shake(1.5f, 4.f);
			}

			PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));
		}
		else
		{
			m_pModel->Tick_Animation(_fTimeDelta);

			auto pGameInstance = CGameInstance::Get_Instance();
			if (m_pModel->Is_AnimationFinished(IDX(ANIMATION::DISPEN_PERRY::TURN_UP_LANDING)))
			{
				m_pModel->Reset_Animation(IDX(ANIMATION::DISPEN_PERRY::TURN_UP_LANDING));
				m_pModel->Set_Animation(IDX(ANIMATION::DISPEN_PERRY::WAIT1), 2.f, false, 0.2f, true);

				shared_ptr<CCamera_Player> pCamera = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera();
				pCamera->Lock(true);

				auto pCamTransform = dynamic_pointer_cast<CTransform>(pCamera->Get_Component(COMPONENT::TRANSFORM));
				pCamTransform->LookAt_Interpolation(m_pTransform->Get_State(TRANSFORM::POSITION));
				pCamTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(-249.923f, -30.592f, 10.506f, 1.f));

				pGameInstance->Enable_DOF(true);
				pGameInstance->Set_DOF_DepthStart(73.f);
				pGameInstance->Set_DOF_DepthRange(16.f);
			}

			m_fCutsceneAccTime += _fTimeDelta;
			if (m_fCutsceneAccTime >= m_fCutsceneTimeThreshold)
			{
				shared_ptr<CCamera_Player> pCamera = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera();
				pCamera->Lock(false);

				m_bCutscene = false;


				auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
				pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE);

				auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
				pToolTip->Start_Tip_Gimmick_DumpTruck();

				CGameInstance::Get_Instance()->Enable_DOF(false);

				CUI_Manager::Get_Instance()->Set_UI_All_RenderStopFalse();
			}
		}

		return;
	}

	if (false == m_pNormalActorDesc->bThrowHit)
	{
		if (m_pNormalActorDesc->bThrowHit = m_pBigActorDesc->bThrowHit)
			m_pNormalActorDesc->stHitDesc = m_pBigActorDesc->stHitDesc;
	}

	Check_Hit();

	__super::Tick(_fTimeDelta);

	for (size_t i = 0; i < 6; i++)
	{
		_matrix BoneMatrix = m_pModel->Get_Bone(m_strAttack1BoneName[i].c_str())->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		_matrix PivotMatrix = m_pModel->Get_Pivot();

		_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
			BoneMatrix
			* PivotMatrix
			* m_pTransform->Get_Matrix();

		PxTransform p;

		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

		m_pAttack1ActorDesc[i]->mOwnerMat = m_pTransform->Get_Matrix();
		m_pAttack1Collider[i]->GetActor()->setGlobalPose(p);
		m_pAttack1Collider[i]->GetActorDesc().vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_pAttack1Collider[i]->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
		m_pAttack1Collider[i]->UpdateActorDesc(); 
	}

	_matrix BoneMatrix = m_pModel->Get_Bone("Water")->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	PxTransform p;

	p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
	_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pBodyCollider->GetActor()->setGlobalPose(p);


	m_pWaterTank->Tick(_fTimeDelta);
	_float4 Test = {};
	(!Detect_Ground(Test));
	{
		_float Dist = XMVectorGetX(XMVector3Length((m_pTransform->Get_State(TRANSFORM::POSITION) - Test)));
		if (Dist >= 1.f)
			m_pTransform->Translate(XMVectorSet(0.f, -Dist, 0.f, 0.f));
	}
	
#ifdef TEST_CODE
//	if (CGameInstance::Get_Instance()->Key_Down(VK_F9))
//	{
//	//	Get_Component<CTransform>(COMPONENT::TRANSFORM)->
//	//		LookTo_Interpolation(
//	//			Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION)
//	//			- Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION), true, false, 10.f);
//	//	Get_Component<CTransform>(COMPONENT::TRANSFORM)->LookAt(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
//	//	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Translate(
//	//		(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION)
//	//			- Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION).normalize()));
//	}

#if !ENABLE_BEHAVIORTREE
	{
		auto anima = ANIMATION::DISPEN_PERRY::WALK_F_ROOP;
	//	auto animb = ANIMATION::DISPEN_PERRY::WALK_B_ROOP;
		auto animb = ANIMATION::DISPEN_PERRY::WALK_F_ROOP;
		static bool a(true);
		// ���⼭ ����
		if (CGameInstance::Get_Instance()->Key_Down('J'))
		{
			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(anima);
			m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
			m_pAnimator->Play_Animation(anima, 3.f, false, 0.1f, false);
			InitRootMotion();
		}
		// ���̳� �ڷΰ��Ⱑ �������� �ִϸ��̼��� ��ȯ
		if (Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(a ? anima : animb))
		{
			// ���� ����
			a = !a;
			// ��Ʈ��� ����
			MotionReset();
			m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
			// �ִϸ��̼� ���
			m_pAnimator->Play_Animation(a ? anima : animb, 3.f, false, 0.1f, false);
			// ��Ʈ��� �õ�
			InitRootMotion();
		}
		// ���� �����ΰ��� �ڷΰ��Ⱑ ������̶�� ������Ʈ
		if (Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(anima)
		||	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(animb))
		{
			Update_Motion(_fTimeDelta);
			RootMotion(_fTimeDelta);
		}
	}
#endif
	if (CGameInstance::Get_Instance()->Key_Down('N'))
	{
		Damage(DP_PARTS::BODY, 5.f);
	}
	if (CGameInstance::Get_Instance()->Key_Down('M'))
	{
		Damage(DP_PARTS::WATERTANK, 5.f);
	}
#endif

#pragma region UI
	m_pUI_Name->Tick(_fTimeDelta);
	m_pUI_Hp->Tick(_fTimeDelta);
	m_pUI_Hp_Small->Tick(_fTimeDelta);
#pragma endregion
}

void CMonster_DispenPerry::Late_Tick(_float _fTimeDelta)
{
	if (m_bCutscene)
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);

		return;
	}

	__super::Late_Tick(_fTimeDelta);

	m_pWaterTank->Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//	Add_RenderObject(RENDER_GROUP::BLOOM);

#pragma region UI
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
	m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));


	m_pUI_Name->Late_Tick(_fTimeDelta);
	m_pUI_Hp->Set_BossName(Get_TypeName());
	m_pUI_Hp->Set_BossHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_Hp->Late_Tick(_fTimeDelta);

	m_pUI_Hp_Small->Set_MonsterHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_Hp_Small->Set_MonsterNameTag(Get_TypeName());
	m_pUI_Hp_Small->Late_Tick(_fTimeDelta);
	if (m_tEntityDesc.fHP < m_tEntityDesc.fMaxHP)
	{
		m_pUI_Hp_Small->Set_IndicatorPosition(m_pCamera_Player->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	}
	
	_float fHpRatio = this->m_tEntityDesc.fHP / this->m_tEntityDesc.fMaxHP;

	if (0.5f >= fHpRatio && true == m_bOnce1)
	{
		auto pUI_Manager = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
		pUI_Manager->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE_HALF);
		m_bOnce1 = false;
	}

	if (0.f >= fHpRatio && true == m_bOnce2)
	{
		auto pUI_Manager = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
		pUI_Manager->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_BOSSSTAGE_END);
		m_bOnce2 = false;
	}

#pragma endregion

//	static bool b = true;
//	if(b)
//	{
//		b = !b;
//		m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::TURN_L_ROOP, 1.f, false, 0.2f, true);
//	}
//	_matrix m = m_pModel->Get_Bone("Reference")->Get_CombinedTransformation() * m_pTransform->Get_Matrix();
//	auto a = Function::RelativeAngle(m.r[2], m.r[3], CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION));
//	ImGui::Begin("asdfafd");
//	ImGui::InputFloat("ASdf", &a);
//	ImGui::End();

//	if (CGameInstance::Get_Instance()->Key_Hold(VK_OEM_PLUS))
//	{
//		m_pTransform->LookAt_Interpolation(CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM), true, false, 32.f, 1.f);
//	}

//	if (CGameInstance::Get_Instance()->Exist_Event(m_pTransform))
//	{
//		cout << "asdf" << endl;
//	}
}

HRESULT CMonster_DispenPerry::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(29)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

//HRESULT CMonster_DispenPerry::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
//{
//	return S_OK;
//}

HRESULT CMonster_DispenPerry::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(CGameObject::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_DispenPerry::SetWeaponeAllActiveOn()
{
	for (size_t i = 0; i < 6; i++)
		m_pAttack1ActorDesc[i]->bActive = true;

}

void CMonster_DispenPerry::SetWeaponeAllActiveOff()
{
	for (size_t i = 0; i < 6; i++)
		m_pAttack1ActorDesc[i]->bActive = false;
}

HRESULT CMonster_DispenPerry::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Ready_Components", "Failed to Get_Component: SHADER");
	}
	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);
	m_pTransform->Set_Scale(_float3(1.5f, 1.5f, 1.5f));

	return S_OK;
}

HRESULT CMonster_DispenPerry::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_DispenPerry::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to Get_Behavior: PHYSICS");
	}

#if ENABLE_BEHAVIORTREE
	Add_Behavior(BEHAVIOR::CUSTOM, CDispenPerry_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc));
#endif

	Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Set_Gravity(false);

	return S_OK;
}

void CMonster_DispenPerry::RootMotion(_float _fTimeDelta, _bool _bOneFrame)
{
#if !REVERSE_ROOTMOTION
	__super::RootMotion(_fTimeDelta, _bOneFrame);
#else
	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(XMMatrixRotationX(XMConvertToRadians(90.f)) * m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_vector MoveDir = XMVector3TransformNormal((m_vPreRootPos - m_pModel->Get_RootPos()), m);

//a	m_mRootPrev = m_mRootCur;
//a	m_mRootCur = m * m_pModel->Get_RootMatrix();
//a	_vector q = QuaternionBetweenAxis(m_mRootCur.row(2), m_mRootPrev.row(2));
//a	m_pTransform->Rotate(q);

	vUpdatePos.x += XMVectorGetX(MoveDir);
	vUpdatePos.y -= XMVectorGetY(MoveDir);
	vUpdatePos.z += XMVectorGetZ(MoveDir);
	vUpdatePos.w = 1.f;

	_float4 vPosition = { vUpdatePos.x,vUpdatePos.y,vUpdatePos.z,1.f };
	m_pTransform->Set_State(TRANSFORM::POSITION, XMLoadFloat4(&vPosition));

	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, _fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fTime = 0.f;
	}

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));

	m_vPreRootPos = m_pModel->Get_RootPos();
#endif
}

const _float CMonster_DispenPerry::Get_HP(DP_PARTS _eParts) const
{
	switch (_eParts)
	{
	case DP_PARTS::BODY:
		return m_tEntityDesc.fHP;
	case DP_PARTS::WATERTANK:
		return m_pWaterTank->Get_HP();
	}

	return 0.f;
}

const DP_STATE CMonster_DispenPerry::Damage(DP_PARTS _eParts, _float _fAtk)
{
	switch (_eParts)
	{
	case DP_PARTS::BODY:
		m_pairPartState = make_pair(_eParts, Damage(_fAtk));
		break;
	case DP_PARTS::WATERTANK:
		m_pairPartState = make_pair(_eParts, m_pWaterTank->Damage(_fAtk));
		break;
	}

	return m_pairPartState.second;
}

void CMonster_DispenPerry::WaterShot(_bool _bMultiWaterShot)
{
	m_pWaterTank->Fire_WaterDrop(_bMultiWaterShot);
}

const DP_STATE CMonster_DispenPerry::Damage(_float _fAtk)
{
	m_tEntityDesc.fHP = std::clamp(m_tEntityDesc.fHP - _fAtk, 0.f, m_tEntityDesc.fHP);

	if (m_tEntityDesc.fHP <= 0.f)
	{
	}
	
	auto iter = m_mapDamageLevel.lower_bound(m_tEntityDesc.fHP);
	if (iter != m_itDamageLevel)
	{
		m_itDamageLevel = iter;
		m_eState = m_itDamageLevel->second;
	}
	else
	{
		m_eState = DP_STATE::NOEVENT;
	}

	return m_eState;
}

void CMonster_DispenPerry::Check_Hit()
{
	shared_ptr<CTransform> pTarget = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector Direction = pTarget->Get_State(TRANSFORM::POSITION) - XMVectorSetY(m_pTransform->Get_State(TRANSFORM::POSITION), pTarget->Get_State(TRANSFORM::POSITION).y);

	_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	_float fDegree = XMConvertToDegrees(Radian);

	if (m_ActorDesc.bHit)
	{
		if (m_tEntityDesc.fHP <= 0)
		{
			Delete_Behavior(BEHAVIOR::CUSTOM);
			m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::DOWN_SHOCK);
			return;
		}

		Damage(DP_PARTS::BODY, m_ActorDesc.stHitDesc.iDamage);

		if (!m_ActorDesc.bThrowHit)
		{
			int test = 0;
		}
		return;
	}
	if (m_ActorDesc.bThrowHit)
	{
		if (m_tEntityDesc.fHP <= 0)
		{
			Delete_Behavior(BEHAVIOR::CUSTOM);
			m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::DOWN_SHOCK);
			return;
		}
		SetWeaponeAllActiveOff();
		auto Kynesis = dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Kinesis();
		auto Interactive = Kynesis->Get_RecentThrowdObject();
		if (Interactive == nullptr)
			return;

		shared_ptr<CTransform> InterActiveTransform = Interactive->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		_vector Direction = InterActiveTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
		_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pTransform->Get_State(TRANSFORM::LOOK)), XMVector3Normalize(Direction))));
		_vector Cross = XMVector3Cross(m_pTransform->Get_State(TRANSFORM::LOOK), Direction);
	
		if (XMVectorGetY(Cross) <= 0.f)
			Radian *= -1;
		_float InteractiveDegree = XMConvertToDegrees(Radian);
	
		Damage(DP_PARTS::BODY, m_ActorDesc.stHitDesc.iDamage);
		Kynesis->Set_RecentThrowObject(nullptr);
	
		return;
	}
}

void CMonster_DispenPerry::Set_DamageLevel(_float _fHp, DP_STATE _eState)
{
	m_mapDamageLevel.emplace(_fHp, _eState);
	m_itDamageLevel = m_mapDamageLevel.end();
}

shared_ptr<CMonster_DispenPerry> CMonster_DispenPerry::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_DispenPerry> pInstance = make_private_shared(CMonster_DispenPerry, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_DispenPerry::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_DispenPerry::Clone(any pArg)
{
	shared_ptr<CMonster_DispenPerry> pInstance = make_private_shared_copy(CMonster_DispenPerry, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_DispenPerry::Clone", "Failed to Initialize");
	}

	return pInstance;
}
