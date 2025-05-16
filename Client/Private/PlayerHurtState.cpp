#include "ClientPCH.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerHurtState.h"
#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "PhysXEngine.h"
#include "Effect_Manager.h"
#include "ScreenEffectManager.h"

CPlayerHurtState::CPlayerHurtState()
{

}

CPlayerHurtState::CPlayerHurtState(PLAYER_HURTTYPE _type)
{
	m_eType = _type;
}

shared_ptr<CPlayerState> CPlayerHurtState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	switch (m_eType)
	{
	case PLAYER_HURTTYPE::AIR:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::WAKE_UP_F) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_LANDING))
		{
			return make_shared<CPlayerIdleState>();
		}
		break;
	default:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eHurtAnim))
		{
			return make_shared<CPlayerIdleState>();
		}
		break;
	}

	return nullptr;
}

void CPlayerHurtState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::HURT;


	if (!_player.Get_PlayerDesc().bBrainField)
		_player.Get_Kinesis()->DestroyObject();
	_player.Get_Kinesis()->Reset();
	if (!_player.Get_PlayerDesc().bBrainField)
		_player.Get_Kinesis()->DuplicateReset();

#pragma region 추가할 이펙트!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/* Snail 발사체에게 피격받는 이펙트, TWINCE에게 피격받는 이펙트 */
		/*_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
		Pos.y += 1.f;

		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED_PARTICLE, Pos);*/


		/* FLOWERCHEST에게 피격받는 이펙트 */
	_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	Pos.y += 1.f;
		switch (_player.Get_ActorDesc().stHitDesc.iType)
	{
	case (_uint)CMonster::MONSTERTYPE::FLOWERCHEST:
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLY0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLOWERCHEST, Pos);
		break;
	case (_uint)CMonster::MONSTERTYPE::FLY:
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLY0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLY1, Pos);
		break;
	case (_uint)CMonster::MONSTERTYPE::BUFFALO:
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO1, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO2, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO3, Pos);
		break;
	case (_uint)CMonster::MONSTERTYPE::WATER_PROJECTILE:
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED1, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED2, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::WATER_ATTACKED_PARTICLE, Pos);
		break;
	//default:
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLY0, Pos);
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_FLY1, Pos);
	//	break;
	}



	/* BUFFALO에게 피격받는 이펙트 */
		/*_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
		Pos.y += 1.f;

		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO0, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO1, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO2, Pos);
		CEffect_Manager::Get_Instance()->Fetch(VFX::ATTACKED_BY_BUFFALO3, Pos);*/
#pragma endregion

	switch (m_eType)
	{
	case PLAYER_HURTTYPE::NORMAL:
		Normal_Entry(_player);
		break;
	case PLAYER_HURTTYPE::STRONG:
		Strong_Entry(_player);
		break;
	case PLAYER_HURTTYPE::AIR:
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Spine"));
		Air_Entry(_player);
		break;
	default:
		Default_Entry(_player);
		break;
	}
	_player.Get_PlayerDesc().iHp -= _player.Get_ActorDesc().stHitDesc.iDamage;
	if (_player.Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
		_player.SAS_Exit();

	auto pGameInstance = CGameInstance::Get_Instance();
	_player.Get_PlayerDesc().bJustDodge = false;
	_player.Get_PlayerDesc().fJustDodgeTime = _player.Get_PlayerDesc().fJustDodgeMaxTime;
	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 1.f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 1.f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 1.f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 1.f);
	pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 1.f);

	CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::DAMAGE_OVERAY);

	if(m_eType != PLAYER_HURTTYPE::AIR)
	{
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Damaged00.wav"),TEXT("Voice_Kasane_Damaged01.wav") ,TEXT("Voice_Kasane_Damaged02.wav"),TEXT("Voice_Kasane_Damaged03.wav") }), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attacked_Down.wav"), g_fPlayerSound);
	}
	else
	{
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_KnockDown00.wav"),TEXT("Voice_Kasane_KnockDown01.wav") }), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attacked_Down.wav"), g_fPlayerSound);
	}
}

void CPlayerHurtState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::DAMAGE_AIR))
	{
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * -fTimeDelta * 3.f);
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::DAMAGE_AIR) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BLOW_LANDING))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BLOW_LANDING, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BLOW_LANDING))
	{
		if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bBreakFall = true;
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BLOW_LANDING))
	{
		if (m_bBreakFall)
		{
			if (!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BREAKFALL_F))
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BREAKFALL_F, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
			}
		}
		else
		{
			if (!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::WAKE_UP_F))
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::WAKE_UP_F, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_KnockDownWakeUp00.wav"),TEXT("Voice_Kasane_KnockDownWakeUp01.wav") }), g_fPlayerSound);
			}
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BREAKFALL_F))
	{
		m_pTransform->Translate({ 0,fTimeDelta * 10.f ,0,0 });
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BREAKFALL_F) &&
		!m_bInAir)
	{
		if (!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_LANDING))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}

	if (m_eType == PLAYER_HURTTYPE::NORMAL)
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
	else if (m_eType == PLAYER_HURTTYPE::STRONG)
		__super::Late_Update_Root(_player, fTimeDelta, 2.f);
	else if (m_eType == PLAYER_HURTTYPE::AIR)
		__super::Late_Update(_player, fTimeDelta);
}

void CPlayerHurtState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	_player.Get_PlayerDesc().bInvincibility = false;
}

void CPlayerHurtState::Default_Entry(CPlayer& _player)
{

	_vector vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
	_vector vTargetLook = XMVector3Normalize(_float3(_player.Get_ActorDesc().vTargetPos.x, _player.Get_ActorDesc().vTargetPos.y, _player.Get_ActorDesc().vTargetPos.z) - m_pTransform->Get_State(TRANSFORM::POSITION));
	_float fAngle = XMVectorGetX(XMVector3Dot(vPlayerLook, vTargetLook));

	m_pTransform->LookAt(_float3(_player.Get_ActorDesc().vTargetPos));

	const PxU32 bufferSize = 32;        // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here
	PxExtendedVec3 vPos = _player.Get_Controller()->getPosition();
	bool hasHit = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
		PxVec3(vPos.x, vPos.y, vPos.z),
		PxVec3(0, -1, 0),
		100.f,
		buf);
	PxF32 fDist = -1.f;
	if (hasHit) {
		PxU32 nbHits = buf.nbTouches;
		for (PxU32 i = 0; i < nbHits; ++i) {
			const PxRaycastHit& touch = buf.touches[i];
			ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
			if (nullptr == pActorDesc)
			{
				fDist = touch.distance;
			}
		}
	}
	if (fDist < 2.5f)
	{
		Normal_Entry(_player);
	}
	else
	{
		Air_Entry(_player);
	}
}

void CPlayerHurtState::Normal_Entry(CPlayer& _player)
{
	_vector vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
	_vector vTargetLook = XMVector3Normalize(_float3(_player.Get_ActorDesc().vTargetPos.x, _player.Get_ActorDesc().vTargetPos.y, _player.Get_ActorDesc().vTargetPos.z) - m_pTransform->Get_State(TRANSFORM::POSITION));
	_float fAngle = XMVectorGetX(XMVector3Dot(vPlayerLook, vTargetLook));

	m_pTransform->LookAt(_float3(_player.Get_ActorDesc().vTargetPos));


	if (XMVectorGetX(XMVector3Dot(vPlayerRight, vTargetLook)) > 0)
	{
		if (fAngle >= cosf(XMConvertToRadians(45.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_FL;
		}
		else if (fAngle > cosf(XMConvertToRadians(135.f)) && fAngle < cosf(XMConvertToRadians(45.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_R;
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-90.f));
		}
		else if (fAngle >= cosf(XMConvertToRadians(180.f)) && fAngle < cosf(XMConvertToRadians(135.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_B;
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(180.f));
		}
	}
	else
	{
		if (fAngle >= cosf(XMConvertToRadians(45.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_FL;
		}
		else if (fAngle > cosf(XMConvertToRadians(135.f)) && fAngle < cosf(XMConvertToRadians(45.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_L;
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(90.f));
		}
		else if (fAngle >= cosf(XMConvertToRadians(180.f)) && fAngle < cosf(XMConvertToRadians(135.f)))
		{
			m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_B;
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(180.f));
		}
	}
	m_eType = PLAYER_HURTTYPE::NORMAL;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eHurtAnim, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);

}

void CPlayerHurtState::Strong_Entry(CPlayer& _player)
{
	//m_pModel->SetRootBoneIndex((_uint)BONE::SPINE, (_uint)BONE::SPINE);
	_vector vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
	_vector vTargetLook = XMVector3Normalize(_float3(_player.Get_ActorDesc().vTargetPos.x, _player.Get_ActorDesc().vTargetPos.y, _player.Get_ActorDesc().vTargetPos.z) - m_pTransform->Get_State(TRANSFORM::POSITION));
	_float fAngle = XMVectorGetX(XMVector3Dot(vPlayerLook, vTargetLook));

	m_pTransform->LookAt(_float3(_player.Get_ActorDesc().vTargetPos));

	if (fAngle > 0)
	{
		m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_F_L;
	}
	else
	{
		m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(180.f));
		m_eHurtAnim = ANIMATION::PLAYER::DAMAGE_B_L;
	}
	m_eType = PLAYER_HURTTYPE::STRONG;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eHurtAnim, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
}

void CPlayerHurtState::Air_Entry(CPlayer& _player)
{
	m_eType = PLAYER_HURTTYPE::AIR;
	_player.Get_PlayerDesc().bInvincibility = true;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::DAMAGE_AIR, g_fPlayerAnimationSpeed * 3.f, false, g_fDefaultInterpolationDuration, false);
}

