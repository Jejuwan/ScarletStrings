#include "ClientPCH.h"
#include "Monster.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Player.h"
#include "Camera_Player.h"
#include "PlayerIdleState.h"
#include "Effect_Manager.h"
#include "Bone.h"
#include "Monster_Projectile.h"
#include "Monster_Weak.h"
#include "MonsterControllerFilterCallBack.h"
#include "Effect_Manager.h"
#include "MonsterControllerHitReport.h"
#include "Part.h"

CMonster::CMonster(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster::CMonster(const CMonster& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_TUTOBOSS, g_aNull);

	return S_OK;
}

HRESULT CMonster::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	m_pTexDissolve = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_043.dds"), 1, true);

	m_ControllerFilterCallBack = make_shared<CMonsterControllerFilterCallBack>();
	m_ControllerFilters.mFilterCallback = m_ControllerFilterCallBack.get();
	m_ControllerFilters.mCCTFilterCallback = NULL;
	m_ControllerFilters.mFilterFlags = PxQueryFlag::eSTATIC | /*PxQueryFlag::eDYNAMIC |*/ PxQueryFlag::ePREFILTER;
	m_ControllerFilters.mFilterData = NULL;

	m_pMonsterControllerHitReport = make_shared<CMonsterControllerHitReport>();
	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pNormalActorDesc->vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pBigActorDesc->vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);

	if (m_pNormalActorDesc->stHitDesc.strAttackTag != ACTORTAG::INTERACTIVE_BEAM &&
		m_pNormalActorDesc->stHitDesc.strAttackTag != ACTORTAG::INTERACTIVE_BUS)
	{
		if (m_pNormalActorDesc->bThrowHit = m_pBigActorDesc->bThrowHit)
			m_pNormalActorDesc->stHitDesc = m_pBigActorDesc->stHitDesc;
	}

	if(m_pBigAttackActorDesc)
		m_pBigAttackActorDesc->bActive = m_pAttackActorDesc->bActive;

	if (m_pBigActorDesc->bHit)
	{
		m_pNormalActorDesc->bCapture = true;
	}

	if (m_pNormalActorDesc->bHit)
	{
		if (m_eMonsterType != MONSTERTYPE::Karen)
		{
			auto pGameInstance = CGameInstance::Get_Instance();
			auto pPool = pGameInstance->Find_Pool(pGameInstance->Current_Scene(), POOLOBJECT_DECAL_HITCRACK);

			_float4x4 matHitCrackDecal = m_pTransform->Get_Matrix();

			DECALDESC tDecalDesc;
			tDecalDesc.pTargetTransform = m_pTransform;
			tDecalDesc.matHitTransform = m_pNormalActorDesc->mEffectMat;
			tDecalDesc.fLifeTime = 0.5f;

			pPool->Pop(tDecalDesc);
		}
	}

	if (m_pFSM)
	{
		m_pFSM->Tick(fTimeDelta);
	}
	m_tEntityDesc.fJumpPower = 0.f;
	_matrix BoneMatrix = m_pModel->Get_Bone(m_strCenterBoneName.c_str())->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	if (m_pController)
	{
		PxTransform p;
		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pController->getActor()->setGlobalPose(p);

		if (m_eMonsterType == MONSTERTYPE::STATUE)
		{
			BoneMatrix = m_pModel->Get_Bone(m_strWeakBoneName.c_str())->Get_CombinedTransformation();
			BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
			BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
			BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		}
		_float4x4 WeakMat = XMMatrixScaling(100.f, 100.f, 100.f) *
			BoneMatrix
			* PivotMatrix
			* m_pTransform->Get_Matrix();
		p.p = PxVec3(WeakMat._41, WeakMat._42, WeakMat._43);
		vQuat = XMQuaternionRotationMatrix(WeakMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pBodyCollider->GetActor()->setGlobalPose(p);

		m_pNormalActorDesc->bHit = false;
		m_pNormalActorDesc->bAirBone = false;
		if(m_eMonsterType != MONSTERTYPE::STATUE)
		{
			m_pNormalActorDesc->bThrowHit = false;
			m_pBigActorDesc->bThrowHit = false;
		}
	}

	if(m_pAttackCollider)
	{
		PivotMatrix = m_pModel->Get_Pivot();

		BoneMatrix = m_pModel->Get_Bone(m_strAttackBoneName.c_str())->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
			BoneMatrix
			* PivotMatrix
			* m_mAttackColliderPivot
			* m_pTransform->Get_Matrix();


		PxTransform p;
		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pAttackCollider->GetActor()->setGlobalPose(p);
		m_pAttackCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
		m_pAttackCollider->UpdateActorDesc();

		if(m_pBigAttackCollider)
		{
			m_pBigAttackCollider->GetActor()->setGlobalPose(p);
			m_pBigAttackCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
		}
	}
	if (m_pWeak)
		m_pWeak->Tick(fTimeDelta);

#ifdef _DEBUG
	//if (CGameInstance::Get_Instance()->Key_Down(VK_END)) Dissolve();
#if ACTIVATE_IMGUI
//	ImGui::Begin("Dissolve Test");
//	if (ImGui::Button("Apply")) Dissolve();
//	ImGui::End();
#endif
#endif

	if (m_bDissolve)
	{
		m_fDissolveThreshold += fTimeDelta / m_fDissolveDuration;
	}

	if (m_bDeadSlow)
	{
		m_fDeadTime -= fTimeDelta;
		if (m_fDeadTime < 0.f)
		{
			CGameInstance::Get_Instance()->Set_PhysXTimeScale(1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER, 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER, 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_ROAD, 1.f);
			CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC, 1.f);
			CGameInstance::Get_Instance()->Set_Effect_TimeScale(1.f);
		}
	}
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	if (m_pFSM)
	{
		m_pFSM->Late_Tick(fTimeDelta);
	}
	if (m_bIsWeak)
		m_pWeak->Late_Tick(fTimeDelta);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Render_OutLine()
{
	return S_OK;
}

HRESULT CMonster::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMonster::Fetch(any Any)
{
	if (!Any.has_value())
		return E_FAIL;

	_float4 InitPos = any_cast<_float4>(Any);
	InitPos.y += 1.f;
	
	m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
	InitPos.w = 1.f;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
	m_pController->setFootPosition(PxExtendedVec3(XMVectorGetX(InitPos), XMVectorGetY(InitPos), XMVectorGetZ(InitPos)));

	_float3 vFetch = _float3(InitPos) + _float3(0.f, 1.f, 0.f) + m_pTransform->Get_State(TRANSFORM::LOOK).normalize() * 0.5f;
	VFXMGR->Fetch(MULTIVFX::SPAWN, initializer_list<any>{_float3(InitPos), _float3(InitPos)});

	return S_OK;
}

_bool CMonster::Return()
{
	return m_bDead;
}

void CMonster::InitRootMotion()
{
	m_vPreRootPos = m_pModel->Get_RootPos();
}

void CMonster::Update_Motion(_float fTimeDelta)
{
	if (m_bBulldozerHit)
		return;

	m_vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	
	m_fTime += fTimeDelta;

	m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_fTime * m_fTime * 10.f * fTimeDelta);
}

void CMonster::AirBone(_float fTimeDelta) 
{
	//m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_fTime * m_fTime * 10.f * fTimeDelta);
	m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_fTime * m_fTime * 10.f * fTimeDelta * fTimeDelta);
	m_fTime = 0;
}

void CMonster::No_RootMotion(_float fTimeDelta)
{
	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	
	m_CollisionFlag = Get_Controller()->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fTime = 0.f;
	}

	PxExtendedVec3 vControllerPos = Get_Controller()->getFootPosition() + m_vControllerPivot;

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));
}

void CMonster::RootMotion(_float fTimeDelta, _bool OneFrame)
{
	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//cout << vUpdatePos.x<< '\t' << vUpdatePos.y << '\t' << vUpdatePos.z << endl; 
	_vector MoveDir = XMVector3TransformNormal((m_pModel->Get_RootPos() - m_vPreRootPos), m);
	//cout << XMVectorGetX(m_pModel->Get_RootPos()) << '\t' << XMVectorGetY(m_pModel->Get_RootPos()) << '\t' << XMVectorGetZ(m_pModel->Get_RootPos()) << endl;
	//cout << XMVectorGetX(MoveDir) << '\t' << XMVectorGetY(MoveDir) << '\t' << XMVectorGetZ(MoveDir) << endl;
	
	vUpdatePos += MoveDir;

	vUpdatePos.w = 1.f;

	m_pTransform->Set_State(TRANSFORM::POSITION, vUpdatePos);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vUpdatePos - m_vPrePos);
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

void CMonster::MotionReset()
{
	m_vResetPos = m_pModel->Get_RootAddPos();
	m_vResetPos.w = 1.f;
	XMStoreFloat4(&m_vPreRootPos, m_vResetPos);
}

void CMonster::ReleaseController()
{
	if(m_pController)
	{
		m_pController->release();
		m_pController = nullptr;
	}

	if (m_pBodyCollider)
	{
		m_pBodyCollider->GetActor()->release();
		m_pBodyCollider = nullptr;
	}

	if (m_pAttackCollider)
	{
		m_pAttackCollider->GetActor()->release();
		m_pAttackCollider = nullptr;
	}

	if (m_pBigAttackCollider)
	{
		m_pBigAttackCollider->GetActor()->release();
		m_pBigAttackCollider = nullptr;
	}
}

void CMonster::ShootBulletToPlayer()
{
	TESTOBJDESC ObjDesc;
	//_matrix mSpawnPos = m_pModel->Get_Bone("Tail6")->Get_CombinedTransformation();
	//mSpawnPos.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//mSpawnPos *= m_pTransform->Get_Matrix();
	//XMStoreFloat3(&ObjDesc.vSpawnPos, mSpawnPos.r[3]);
	//ObjDesc.vSpawnPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//_vector TargetPos = dynamic_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
	//TargetPos = XMVectorSetY(TargetPos, XMVectorGetY(TargetPos) + 1.f) - XMLoadFloat3(&ObjDesc.vSpawnPos);
	//XMStoreFloat3(&ObjDesc.vSpawnLookDir, TargetPos);
	_float4x4 Mat = m_pTransform->Get_Matrix();
	m_pProjectile->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER_PROJECTILE)->Add(m_pProjectile);
}

void CMonster::Dissolve_Weak()
{
	dynamic_pointer_cast<CPart>(m_pWeak)->Dissolve();
}

_float CMonster::Height()
{
	const PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	PxExtendedVec3 vPos = m_pController->getPosition();

	bool hasHit = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
		PxVec3(vPos.x, vPos.y, vPos.z),
		PxVec3(0, -1, 0),
		300.f,
		buf);

	_float fMinDist = 10.f;

	if (hasHit) {
		PxU32 nbHits = buf.nbTouches;
		for (PxU32 i = 0; i < nbHits; ++i) {
			const PxRaycastHit& touch = buf.touches[i];
			ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
			if (nullptr == pActorDesc)
			{
				PxF32 fDist = touch.distance;
				if (fMinDist > fDist)
					fMinDist = fDist;
			}
		}
		if (fMinDist > 1.f)
		{
			return fMinDist;
		}
		else return 0.f;
	}
	else return 0.f;
}

void CMonster::Update_Physics_PhysX(_float _fTimeDelta)
{
	if (!m_pPhysics)
	{
		return;
	}

	_float3 vVelocity = m_pPhysics->Get_Velocity();
	m_CollisionFlag = m_pController->move(PxVec3(vVelocity.x, vVelocity.y, vVelocity.z), 0.0f, _fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fTime = 0.f;
	}
}

void CMonster::DeadSlow()
{
	m_bDeadSlow = true;
	CGameInstance::Get_Instance()->Set_PhysXTimeScale(0.1f);
	CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER, 0.5f);
	CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER, 0.5f);
	CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_ROAD, 0.5f);
	CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC, 0.5f);
	CGameInstance::Get_Instance()->Set_Effect_TimeScale(0.3f);
}

HRESULT CMonster::Render(_uint iPassIndex)
{
	if (!m_bDissolve)
	{
		if (FAILED(__super::Render(iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(m_pShader->Bind_Float("g_fThreshold", m_fDissolveThreshold)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_Float");
		}

		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDISSOLVE, m_pTexDissolve->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CShader::Bind_ShaderResourceView");
		}

		if (FAILED(CGameObject::Render(5)))
		{
			MSG_RETURN(E_FAIL, "CMonster_Snail::Render", "Failed to CGameObject::Render");
		}
	}

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Components", "Failed to CGameObject::Ready_Components");
	}
	return S_OK;
}

HRESULT CMonster::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_pAnimator = Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to Get_Behavior: ANIMATOR");
	}
	
	//m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	//if (nullptr == m_pPhysics)
	//{
	//	MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to Get_Behavior: PHYSICS");
	//}

	return S_OK;
}

void CMonster::FSM_GotoFinish()
{
	m_pFSM->GotoFinish();
}

void CMonster::Dissolve()
{
	m_bDissolve = true;
	m_fDissolveThreshold = 0.f;
	//if(nullptr!= m_pWeak)
	//	m_pWeak->
	CEffect_Manager::Get_Instance()->Fetch(VFX::FADE_PARTICLE, shared_from_gameobject());
}
