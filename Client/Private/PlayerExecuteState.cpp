#include "ClientPCH.h"
#include "PlayerExecuteState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"
#include "Model.h"
#include "Animator.h"
#include "Bone.h"
#include "Psychokinesis.h"
#include "Camera_Player.h"
#include "UI_Manager.h"
#include "Monster_Karen.h"
#include "Effect_Manager.h"

CPlayerExecuteState::CPlayerExecuteState()
{
}

shared_ptr<CPlayerState> CPlayerExecuteState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	//if (pGameInstance->Key_Down('G'))
	//	return make_shared<CPlayerExecuteState>();

	if (m_pModel->Get_InterpolateEnd())
	{
		switch (m_eAnim)
		{
		case ANIMATION::PLAYER::BRAINCRUSH_700:
			m_pTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(180.f));
			break;
		}
		return make_shared<CPlayerIdleState>();
	}

	if (m_fEndTime > 2.f)
		return make_shared<CPlayerIdleState>();

	return nullptr;
}

void CPlayerExecuteState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	CUI_Manager::Get_Instance()->Set_InstKillCutScene(true);

	m_eCurrentState = PLAYER_STATE::EXECUTE;
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("CameraPos"));
	m_bGravity = false;
	_player.Get_Camera()->Set_Fixed(true);
	vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
	m_vInitPlayerPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_vInitPlayerPos.y += 1.4f;

	switch(_player.Get_FinishMonster()->Get_Type())
	{
	case CMonster::MONSTERTYPE::SNAIL:
		m_eAnim = ANIMATION::PLAYER::BRAINCRUSH_600;
		break;
	case CMonster::MONSTERTYPE::FLY:
		m_eAnim = ANIMATION::PLAYER::BRAINCRUSH_700;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Fly1.wav"), g_fPlayerSound);
		break;
	case CMonster::MONSTERTYPE::Karen:
		m_eAnim = ANIMATION::PLAYER::CAPTURE_LOOP;
		break;
	default:
		m_eAnim = ANIMATION::PLAYER::BRAINCRUSH_COMMON;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_InstantKill3.wav"), g_fPlayerSound);
		break;
	}
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINCRUSH_OPEN, g_fPlayerAnimationSpeed, false, 0.f, false);
	m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::LOOK) * 1.05f 
		+ m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.042f + _float4(0, 1.25f, 0, 0));
	m_pCameraTransform->LookTo(-m_pTransform->Get_State(TRANSFORM::LOOK));
	m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
	_player.Get_Camera()->Adjust_FOV(XMConvertToRadians(10.f));
	_player.Get_PlayerDesc().bInvincibility = true;
	m_pInitLockedOnTransform = _player.Get_Camera()->Get_LockOnTransform();
	
}

void CPlayerExecuteState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	auto pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BRAINCRUSH_OPEN)&&!m_bExecuted[98])
	{
		m_bExecuted[98] = true;
		_player.Get_Camera()->Adjust_FOV(XMConvertToRadians(60.f));
		m_pTransform->Translate({0, -1.4f, 0, 0});
		if(m_eAnim == ANIMATION::PLAYER::BRAINCRUSH_700)
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, 2.f, false, 0.f, false);
		else
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, 3.f, false, 0.f, false);

		//_PLAYER.GETFINISHMONTSER()
	}
	switch (m_eAnim)
	{
#pragma region 600
	case ANIMATION::PLAYER::BRAINCRUSH_600:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			_float3 vCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
			_float3 vPlayerUp = m_pTransform->Get_State(TRANSFORM::UP);
			_float3 vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);
			if (!m_bExecuted[99])
			{
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_InstantKill4.wav"), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Snail.wav"), g_fPlayerSound);
				m_bExecuted[99] = true;
				_player.Get_FinishMonster()->FSM_GotoFinish();
				if (m_pInitLockedOnTransform != nullptr)
				{
					m_pTransform->LookAt(m_pInitLockedOnTransform->Get_State(TRANSFORM::POSITION));
					m_pTransform->Set_State(TRANSFORM::POSITION, m_pInitLockedOnTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 8.f);
				}
				m_pCameraTransform->LookTo(m_pTransform->Get_State(TRANSFORM::LOOK));
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
					- m_pTransform->Get_State(TRANSFORM::LOOK) * 2.5f +
					/*+m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.3f +*/_float4(0, 3.f, 0, 0));
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(10.f));
			}

			if (m_bExecuted[99] && m_fTrackPos <= 120.f)
			{
				m_fInterpolateTime[11] += fTimeDelta;
				_float3 vPos = XMVectorLerp(m_pCameraTransform->Get_State(TRANSFORM::POSITION), m_pCameraTransform->Get_State(TRANSFORM::POSITION) - vPlayerUp * 0.005f + vPlayerLook * 0.005f, m_fInterpolateTime[11]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, -fTimeDelta * 0.01f);
			}
			else if (m_fTrackPos > 120.f && m_fTrackPos < 170.f)
			{
				m_fInterpolateTime[0] += fTimeDelta;
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos + vPlayerRight * 0.02f, m_fInterpolateTime[0]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate({ 0,1,0,0 }, -fTimeDelta * 0.1f);
			}
			else if (m_fTrackPos > 170.f && m_fTrackPos < 220.f)
			{
				m_fInterpolateTime[1] += fTimeDelta;
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos - vPlayerRight * 0.035f, m_fInterpolateTime[1]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate({ 0,1,0,0 }, fTimeDelta * 0.1f);
			}
			else if (m_fTrackPos > 220.f && m_fTrackPos < 265.f)
			{
				if (!m_bExecuted[77])
				{
					m_bExecuted[77] = true;
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Normal.wav"), g_fPlayerSound);
				}
				m_fInterpolateTime[2] += fTimeDelta;
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos + vPlayerUp * 0.01f, m_fInterpolateTime[2]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, -fTimeDelta * 0.3f);
			}
			else if (m_fTrackPos > 265.f && m_fTrackPos < 275.f)
			{              
				//m_pCameraTransform->Rotate(TRANSFORM::RIGHT, -fTimeDelta * 1.5f);
			}
			else if (m_fTrackPos > 285.f && !m_bExecuted[1])
			{
				m_bExecuted[1] = true;
				_player.Get_Camera()->Set_Fixed(false);
				m_bGravity = true;
				//m_pCameraTransform->LookTo_Interpolation(m_pTransform->Get_State(TRANSFORM::LOOK));
			}
		}
		break;
#pragma endregion
#pragma region 700
	case ANIMATION::PLAYER::BRAINCRUSH_700:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			_float3 vCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
			_float3 vPlayerUp = m_pTransform->Get_State(TRANSFORM::UP);
			_float3 vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);

			if (!m_bExecuted[99])
			{
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_InstantKill_Fly0.wav"), g_fPlayerSound);
				m_bExecuted[99] = true;
				_player.Get_FinishMonster()->FSM_GotoFinish();
				if (m_pInitLockedOnTransform != nullptr)
				{
					shared_ptr<CTransform> pEnemyTransform = static_pointer_cast<CTransform>(_player.Get_FinishMonster()->Get_Component(COMPONENT::TRANSFORM));
					_float3 vEnemyPos = pEnemyTransform->Get_State(TRANSFORM::POSITION);
					_float3 vPlayerPos2 = m_pTransform->Get_State(TRANSFORM::POSITION);
					vEnemyPos.y = vPlayerPos2.y;
					_float3 vDir = vEnemyPos - vPlayerPos2;
					_vector nor = XMVector3Normalize(vDir);
					m_pTransform->Set_State(TRANSFORM::POSITION, vEnemyPos - nor * 5.5f);

					vEnemyPos.y += 2.5f;
					pEnemyTransform->Set_State(TRANSFORM::POSITION, vEnemyPos);

					m_pTransform->LookAt(pEnemyTransform);
					pEnemyTransform->LookAt(m_pTransform);

					m_pCameraTransform->LookTo(m_pTransform->Get_State(TRANSFORM::LOOK));
					m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
						- m_pTransform->Get_State(TRANSFORM::LOOK) * 0.4f +
						+m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.6f + _float4(0, 1.7f, 0, 0));

					m_pCameraTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(-30.f));
				}
			}
			if (m_fTrackPos > 30.f && m_fTrackPos < 80.f)
			{
				m_fInterpolateTime[13] += fTimeDelta;
				_float3 vPos = XMVectorLerp(m_pCameraTransform->Get_State(TRANSFORM::POSITION), m_pCameraTransform->Get_State(TRANSFORM::POSITION) - vPlayerLook * 0.005f, m_fInterpolateTime[13]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
			}
			else if (m_fTrackPos >= 80.f && m_fTrackPos < 100.f)
			{
				if (!m_bExecuted[0])
				{
					m_bExecuted[0] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
				}
				m_fInterpolateTime[0] += fTimeDelta;
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos - vPlayerRight * 2.f, m_fInterpolateTime[0]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(m_pTransform->Get_State(TRANSFORM::LOOK), -fTimeDelta * 0.2f);
			}
			if (m_fTrackPos >= 100.f && m_fTrackPos < 120.f)
			{
				if (!m_bExecuted[1])
				{
					m_bExecuted[1] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
				}
				m_fInterpolateTime[1] += fTimeDelta;
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos + vPlayerRight * 3.f, m_fInterpolateTime[1]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(m_pTransform->Get_State(TRANSFORM::LOOK), fTimeDelta * 0.3f);
			}
			else if (m_fTrackPos >= 120.f && m_fTrackPos < 180.f)
			{
				if (!m_bExecuted[2])
				{
					m_bExecuted[2] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
					//CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_InstantKill2.wav"), g_fPlayerSound);
				}
				m_fInterpolateTime[2] += fTimeDelta * .5f;
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos + vPlayerLook * 0.1f, m_fInterpolateTime[2]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
			}
			else if (m_fTrackPos >= 220.f && m_fTrackPos < 230.f)
			{
				if (!m_bExecuted[11])
				{
					m_bExecuted[11] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Normal.wav"), g_fPlayerSound);
				}
				m_fInterpolateTime[11] += fTimeDelta * 20.f;
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos + vPlayerRight * 0.1f + vPlayerUp * 0.1f, m_fInterpolateTime[11]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->LookAt_Interpolation(vPlayerPos + vPlayerUp * 3.f, false);
			}
			else if (m_fTrackPos >= 230.f && m_fTrackPos < 241.f)
			{
				if (!m_bExecuted[3])
				{
					m_bExecuted[3] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
				}
				m_fInterpolateTime[3] += fTimeDelta * 20.f;
				_float fDefaultTimeScale = CGameInstance::Get_Instance()->Get_TimeScale(DEFAULT_FPS);
				if (m_fTrackPos >= 230.f && m_fTrackPos < 240.f)
				{
					CGameInstance::Get_Instance()->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
					pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
					pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.0f);
				}
				else
				{
					CGameInstance::Get_Instance()->Set_PhysXTimeScale(fDefaultTimeScale);
					pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
					pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
				}
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos + vPlayerLook * 0.1f, m_fInterpolateTime[3]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->LookAt_Interpolation(vPlayerPos + vPlayerUp * 3.f, false);
			}
			else if (m_fTrackPos >= 241.f && m_fTrackPos < 400.f)
			{
				if (!m_bExecuted[4])
				{
					m_bExecuted[4] = true;
					vInitCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
				}
				m_fInterpolateTime[4] += fTimeDelta;
				_float3 vLockOnPos = m_pInitLockedOnTransform->Get_State(TRANSFORM::POSITION);
				_float3 vLockOnDir = m_pInitLockedOnTransform->Get_State(TRANSFORM::RIGHT);
				_float3 vPos = XMVectorLerp(vInitCamPos, vInitCamPos + vPlayerLook * 1.f, m_fInterpolateTime[4]);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->LookAt_Interpolation(vLockOnPos + vLockOnDir * 3.f);
				m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-m_fInterpolateTime[4] * 100.f));

				if (m_fTrackPos > 300.f && !m_bExecuted[5])
				{
					m_bExecuted[5] = true;
					m_bGravity = true;
					m_fGravityTime = 0.f;
				}
			}
		}
		break;
#pragma endregion
#pragma region COMMON
	case ANIMATION::PLAYER::BRAINCRUSH_COMMON:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			if (!m_bExecuted[99])
			{
				m_bExecuted[99] = true;
				_player.Get_FinishMonster()->FSM_GotoFinish();
				if (m_pInitLockedOnTransform != nullptr)
				{
					m_pTransform->LookAt(m_pInitLockedOnTransform->Get_State(TRANSFORM::POSITION));
					m_pTransform->Set_State(TRANSFORM::POSITION, m_pInitLockedOnTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 2.f);
				}
				m_pCameraTransform->LookTo(m_pTransform->Get_State(TRANSFORM::LOOK));
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
					- m_pTransform->Get_State(TRANSFORM::LOOK) * 1.5f + _float4(0, 1.f, 0, 0));
			//	Here
			//	VFXMGR->Fetch(MULTIVFX::EXECUTE_COMMON, initializer_list<any>
			//	{});
				
				pair<shared_ptr<CGameObject>, const _char*> argPlayer = make_pair(Function::Find_Player(), "LeftWeapon");
				pair<shared_ptr<CGameObject>, const _char*> argTarget = make_pair(static_pointer_cast<CGameObject>(_player.Get_FinishMonster()), "Target");
				VFXMGR->Fetch(MULTIVFX::EXECUTE_COMMON, initializer_list<any>
				{ argTarget, argPlayer, argPlayer, argPlayer, argTarget, argTarget, m_pTransform->Get_Matrix(), m_pTransform->Get_Matrix(), m_pTransform->Get_Matrix(),
				argTarget, argPlayer, argTarget, argTarget, argTarget, argTarget, argTarget, argTarget, argTarget, argTarget });
			}
			_float3 vCamPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerRight = m_pTransform->Get_State(TRANSFORM::RIGHT);
			_float3 vPlayerUp = m_pTransform->Get_State(TRANSFORM::UP);
			_float3 vPlayerLook = m_pTransform->Get_State(TRANSFORM::LOOK);

			if (m_fTrackPos < 30.f)
			{
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos + vPlayerUp * 0.0075f - vPlayerLook * 0.01f + vPlayerRight * 0.005f, m_fTrackPos / 30.f);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, -fTimeDelta * 1.f);
			}
			else if (m_fTrackPos >= 30.f && m_fTrackPos < 100.f)
			{
				if(m_fTrackPos > 70.f && !m_bExecuted[77])
				{
					m_bExecuted[77] = true;
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_InstantKill_Normal.wav"), g_fPlayerSound);
				}
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos - vPlayerRight * 0.2f - vPlayerUp * 0.0025f - vPlayerLook * 0.025f, (m_fTrackPos - 30.f)/70.f);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate({ 0,1,0,0 }, fTimeDelta * 0.15f);
			}
			else if (m_fTrackPos >= 100.f && m_fTrackPos < 120.f)
			{
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos - vPlayerUp * 0.1f + vPlayerRight * 0.15f + vPlayerLook * 0.05f, (m_fTrackPos - 100.f) / 20.f);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);
				m_pCameraTransform->Rotate(TRANSFORM::UP, fTimeDelta * 0.5f);
			}
			else if (m_fTrackPos >= 120.f)
			{ 
				_float3 vPos = XMVectorLerp(vCamPos, vCamPos - vPlayerLook * 0.0005f, (m_fTrackPos - 120.f) / 60.f);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, vPos);

			}
		}
		break;
#pragma endregion
#pragma region KAREN
	case ANIMATION::PLAYER::CAPTURE_LOOP:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			shared_ptr<CMonster_Karen> pKaren = nullptr;
			shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
			pMonLayer->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->bool
				{
					if (nullptr == dynamic_pointer_cast<CMonster_Karen>(_pObject))
						return true;

					pKaren = dynamic_pointer_cast<CMonster_Karen>(_pObject);

					return true;
				});

			shared_ptr<CTransform> pKarenTransform = static_pointer_cast<CTransform>(pKaren->Get_Component(COMPONENT::TRANSFORM));
			//m_pTransform->LookAt(static_pointer_cast<CTransform>(pKaren->Get_Component(
			//	COMPONENT::TRANSFORM)));
			//pKarenTransform->LookAt(m_pTransform);
			//m_pCameraTransform->LookAt(pKarenTransform);
			//m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(30.f));

			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_LOOP))
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP, 2.5f, false, 0.2f, false);
			}

			m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 0.7f + m_pTransform->Get_State(TRANSFORM::UP) * 1.2f + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.f);
			m_pCameraTransform->LookAt(pKarenTransform);
			m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-25.f));
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP))
		{
			m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::LOOK) * 4.5f + m_pTransform->Get_State(TRANSFORM::UP) + m_pTransform->Get_State(TRANSFORM::RIGHT) * 5.f);
			m_pCameraTransform->LookAt(m_pTransform);
			m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(24.f));
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP))
		{
			m_fEndTime += fTimeDelta;
		}

		break;
#pragma endregion
	}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BRAINCRUSH_OPEN))
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_vInitPlayerPos);
			__super::Late_Update(_player, fTimeDelta);
		}
		else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eAnim))
		{
			__super::Late_Update_Root(_player, fTimeDelta, 1.f);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eAnim))
		{
			switch (m_eAnim)
			{
			case ANIMATION::PLAYER::BRAINCRUSH_600:
				m_pModel->Set_InterpolateTime(0.5f);
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Interpolate_Animation(fTimeDelta);
				break;
			case ANIMATION::PLAYER::BRAINCRUSH_700:
				if (!m_bExecuted[90])
				{
					m_bExecuted[90] = true;
					m_pModel->Set_InterpolateEnd(true);
				}
				break;
			default:
				m_pModel->Set_InterpolateTime(0.5f);
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Interpolate_Animation(fTimeDelta);
				break;
			}
		}
		
}

void CPlayerExecuteState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	_player.Get_Camera()->Set_Fixed(false);
	_player.Get_PlayerDesc().bInvincibility = false;
	CUI_Manager::Get_Instance()->Set_InstKillCutScene(false);

	auto pCamTransform = _player.Get_Camera()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pCamTransform->Set_Matrix(m_pTransform->Get_Matrix());


}
