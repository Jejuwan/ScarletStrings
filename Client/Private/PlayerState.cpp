#include "ClientPCH.h"
#include "PlayerState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "PhysXCollider.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Bone.h"
#include "Psychokinesis.h"
#include "Effect_Manager.h"

CPlayerState::CPlayerState()
{
}

shared_ptr<CPlayerState> CPlayerState::Transition(CPlayer& _player)
{

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Hold('A'))
		return make_shared<CPlayerWalkState>(PLAYER_DIR::L);
	if (pGameInstance->Key_Hold('D'))
		return make_shared<CPlayerWalkState>(PLAYER_DIR::R);
	if (pGameInstance->Key_Hold('W'))
		return make_shared<CPlayerWalkState>(PLAYER_DIR::F);
	if (pGameInstance->Key_Hold('S'))
		return make_shared<CPlayerWalkState>(PLAYER_DIR::B);

	if (pGameInstance->Key_Down(VK_LBUTTON))
	{
		if(!_player.Get_Kinesis()->Get_ThrowHit())
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::NORMAL);
		else
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::DASH);
	}

	if (pGameInstance->Key_Down('C'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::BACKSTEP);

	if (pGameInstance->Key_Down('F'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::UPPER);

	if (pGameInstance->Key_Hold(VK_RBUTTON) && nullptr != _player.Get_Kinesis()->Get_CapturedObject())
	{
		return make_shared<CPlayerThrowState>();
	}

	return nullptr;
}

void CPlayerState::Entry(CPlayer& _player)
{
	m_pTransform = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
	m_pCameraTransform = static_pointer_cast<CTransform>(_player.Get_Camera()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
	m_vInitPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pModel->Set_NextAnimIndex((_uint)ANIMATION::PLAYER::IDLE);
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Lock(false);
	m_pModel->Set_InterpolateTime(0.2f);
	m_bHitLagEnable = true;

	m_eCurSAS = _player.Get_PlayerDesc().eSAS;

	for (int i = 0; i < _player.Get_Weapons().size(); i++)
	{
		_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;
	}
}

void CPlayerState::Update(CPlayer& _player, _float fTimeDelta)
{
	m_vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_fTrackPos = m_pModel->Get_TrackPosition();
	m_fDuration = m_pModel->Get_Duration();
	
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_fTime += fTimeDelta;

	if (m_bGravity)
	{
		m_fGravityTime += fTimeDelta;
		m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_fGravityTime * m_fGravityTime *_player.Get_PlayerDesc().fGravity * fTimeDelta);
	}
	else
		m_fGravityTime = 0.f;

	if (m_bMonsterKick && !m_bMonsterKickExecuted)
	{
		m_fCurrentLagIntervalTime += fTimeDelta;
		Execute_Lag_Interval();
	}
}

void CPlayerState::Late_Update(CPlayer& _player, _float fTimeDelta)
{
	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	m_CollisionFlag = _player.Get_Controller()->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, _player.Get_ControllerFilters());
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN || m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_SIDES)
	{
		if(!m_bJumpRise)
			m_fGravityTime = 0.f;
		if(!m_bForceInAir)
			m_bInAir = false;
	}
	else
	{
		if (!m_bForceInAir)
			m_bInAir = true;
	}

	PxExtendedVec3 vControllerPos = _player.Get_Controller()->getFootPosition() + PxExtendedVec3(0, 0.f, 0);

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));
}

 void CPlayerState::Late_Update_Root(CPlayer& _player, _float fTimeDelta, _float fAmount)
{
	 _vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	 _vector v = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
	 _matrix m = XMMatrixRotationQuaternion(v);

	 _float4 vUpdatePos;
	 vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION) + XMVector3TransformNormal((m_pModel->Get_RootPos() - m_vPreRootPos) * fAmount, m);
	 vUpdatePos.w = 1.f;

	 _float4 vPosition = { vUpdatePos.x,vUpdatePos.y,vUpdatePos.z,1.f };
	 m_pTransform->Set_State(TRANSFORM::POSITION, XMLoadFloat4(&vPosition));

	 _vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	 _float3 vDisp;
	 XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	 m_CollisionFlag = _player.Get_Controller()->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, _player.Get_ControllerFilters());
	 if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN || m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_SIDES)
	 {
		 if (!m_bJumpRise)
			 m_fGravityTime = 0.f;
		 if (!m_bForceInAir)
			 m_bInAir = false;
	 }
	 else
	 {
		 if (!m_bForceInAir)
			 m_bInAir = true;
	 }

	 PxExtendedVec3 vControllerPos = _player.Get_Controller()->getFootPosition() + PxExtendedVec3(0, 0.f, 0);

	 m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));

	 m_vPreRootPos = m_pModel->Get_RootPos();
 }

void CPlayerState::Exit(CPlayer& _player)
{
	m_vPreRootPos = _float4(0, 0, 0, 1);
	m_pModel->Set_InterpolateTime(0.2f);
	m_bForceInAir = false;
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"),m_pModel->Get_BoneIndex("Eff01"));
}

void CPlayerState::Execute_Lag()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if(m_bHitLagEnable)
	{
		m_bHitLagEnable = false;
		_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
		//CGameInstance::Get_Instance()->Set_LagTime(fDefaultTimeScale *0.1f);
		pGameInstance->Set_HitLag(true);
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_ROAD, fDefaultTimeScale * 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
		CGameInstance::Get_Instance()->Set_Effect_TimeScale(fDefaultTimeScale*0.3f);
	}
}

void CPlayerState::Execute_Lag_Interval()
{
	if (m_fCurrentLagIntervalTime > m_fLagIntervalTime)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		m_bMonsterKickExecuted = true;
		CGameInstance::Get_Instance()->Set_LagTime(0.3f);
		CGameInstance::Get_Instance()->Set_HitLag(true);
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER, 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER, 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_ROAD, 0.1f);
		CGameInstance::Get_Instance()->Set_TimeScale(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC, 0.1f);
		_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
		CGameInstance::Get_Instance()->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
	}
}
