#include "ClientPCH.h"
#include "PlayerAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerThrowState.h"
#include "PlayerDodgeState.h"
#include "PlayerSpecialThrowState.h"
#include "Map_Special_Dynamic.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "PlayerWeapon.h"
#include "PhysXCollider.h"
#include "Effect_Manager.h"
#include "Map_MindRoom_Dynamic.h"
#include "Bone.h"

CPlayerAttackState::CPlayerAttackState(ATK_TYPE type)
{
	m_eType = type;
}

shared_ptr<CPlayerState> CPlayerAttackState::Transition(CPlayer& _player)
{

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_START) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR1_WP) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR2_WP) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR3_WP))
	{
		if (pGameInstance->Key_Down('C'))
			return make_shared<CPlayerAttackState>(ATK_TYPE::AIRDOWN);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eEndAnim) &&
		m_pModel->Get_Proportion() > m_fProportion)
	{
		if (pGameInstance->Key_Hold('W'))
		{
			if (pGameInstance->Key_Hold(VK_SHIFT))
				return make_shared<CPlayerDodgeState>(PLAYER_DIR::F);
			return make_shared<CPlayerWalkState>(PLAYER_DIR::F, true);
		}
		else if (pGameInstance->Key_Hold('A'))
		{
			if (pGameInstance->Key_Hold(VK_SHIFT))
				return make_shared<CPlayerDodgeState>(PLAYER_DIR::L);
			return make_shared<CPlayerWalkState>(PLAYER_DIR::L, true);
		}
		else if (pGameInstance->Key_Hold('S'))
		{
			if (pGameInstance->Key_Hold(VK_SHIFT))
				return make_shared<CPlayerDodgeState>(PLAYER_DIR::B);
			return make_shared<CPlayerWalkState>(PLAYER_DIR::B, true);
		}
		else if (pGameInstance->Key_Hold('D'))
		{
			if (pGameInstance->Key_Hold(VK_SHIFT))
				return make_shared<CPlayerDodgeState>(PLAYER_DIR::R);
			return make_shared<CPlayerWalkState>(PLAYER_DIR::R, true);
		}

		if (pGameInstance->Key_Hold('C') && (ANIMATION::PLAYER::ATK_BACKSTEP != m_eEndAnim && ANIMATION::PLAYER::ATK_BACKSTEP_ELECTRIC != m_eEndAnim))
		{
			return make_shared<CPlayerAttackState>(ATK_TYPE::BACKSTEP);
		}

		//if (pGameInstance->Key_Hold(VK_RBUTTON) && nullptr != _player.Get_Kinesis()->Get_CapturedObject() &&
		//	!_player.Get_Kinesis()->Get_SpecialThrow() &&_player.Get_PlayerDesc().fKinesisGauge > 20.f)
		//	return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START);

		if (nullptr != _player.Get_Kinesis()->Get_CapturedObject())
		{
			if (shared_ptr<CMap_Special_Dynamic> pSpecial = dynamic_pointer_cast<CMap_Special_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
			{
				if (pGameInstance->Key_Hold('G'))
					return make_shared<CPlayerSpecialThrowState>(pSpecial->Get_Type());
			}
			else if (nullptr != dynamic_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()) && _player.Get_PlayerDesc().fKinesisGauge > 20.f)
			{
				if (pGameInstance->Key_Hold(VK_RBUTTON) && _player.Get_PlayerDesc().fKinesisGauge > 20.f)
					return make_shared<CPlayerThrowState>(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START);
				if (pGameInstance->Key_Hold(VK_RBUTTON) && _player.Get_PlayerDesc().fKinesisGauge <= 20.f)
				{
					return make_shared<CPlayerSpecialThrowState>(pSpecial->Get_Type());
					_player.Get_Kinesis()->Set_CapturedObject(nullptr);
				}
			}
		}
	}

	if ((_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_START) && m_pModel->Get_Proportion() > 0.5f) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER) ||
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_FALL))
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
			return make_shared<CPlayerAttackState>(ATK_TYPE::JUMP);
	}	
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eEndAnim))
	{
		return make_shared<CPlayerIdleState>();
	}

	return nullptr;
}

void CPlayerAttackState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::ATTACK;
	for (int i = 0; i < _player.Get_Weapons().size(); i++)
	{
		_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
	}
	switch (m_eType)
	{
	case ATK_TYPE::NORMAL:
		m_eAnim = ANIMATION::PLAYER::ATK1;
		m_eEndAnim = m_eAnim;
		m_fProportion = 0.3f;
		break;
	case ATK_TYPE::BACKSTEP:
		if (_player.Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC)
		{
			m_eAnim = ANIMATION::PLAYER::ATK_BACKSTEP_ELECTRIC;
		}
		else
			m_eAnim = ANIMATION::PLAYER::ATK_BACKSTEP;
		m_eCurrentState = PLAYER_STATE::ATTACK_BACKSTEP;
		m_eEndAnim = m_eAnim;
		m_fProportion = 0.6f;
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_BackDash01.wav"),TEXT("Voice_Kasane_BackDash02.wav") }), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal01.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::UPPER:
		m_eCurrentState = PLAYER_STATE::ATTACK_UPPER;
		m_eAnim = ANIMATION::PLAYER::ATK_UPPER_START;
		m_eEndAnim = ANIMATION::PLAYER::ATK_UPPER_LANDING;
		m_fProportion = 0.5f;
		m_bJumpRise = true;
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_SomerSaultKick00.wav"),TEXT("Voice_Kasane_SomerSaultKick01.wav"),TEXT("Voice_Kasane_SomerSaultKick02.wav") }), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_SomersaultKick.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::DASH:
		m_eAnim = ANIMATION::PLAYER::ATK_DASH_START;
		m_eEndAnim = ANIMATION::PLAYER::ATK_DASH_END;
		m_eCurrentState = PLAYER_STATE::ATTACK_DASH;
		m_fProportion = 0.5f;
		_player.Get_Kinesis()->Set_ThrowHit(false);
		_player.Set_WeaponsActive(false);
		_player.Set_BigWeaponsActive(true);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_DashAttack.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::JUMP:
		m_eCurrentState = PLAYER_STATE::JUMP_ATTACK1;
		m_eAnim = ANIMATION::PLAYER::ATK_AIR1_WP;
		m_eEndAnim = ANIMATION::PLAYER::ATK_UPPER_LANDING;
		m_fProportion = 0.0f;
		m_bGravity = false;
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_JumpAttack1_00.wav"),TEXT("Voice_Kasane_JumpAttack1_01.wav"),TEXT("Voice_Kasane_JumpAttack1_02.wav") }), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal00.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::CHARGE:
	{
		m_eAnim = ANIMATION::PLAYER::ATK_CHARGE_START;
		m_eEndAnim = ANIMATION::PLAYER::ATK_CHARGE_ATK_END;
		m_fProportion = 0.5f;
		_float4x4 mOriginal	= m_pTransform->Get_Matrix();
		_float4x4 mEff1		= m_pModel->Get_Bone("Eff01")->Get_CombinedTransformation() * m_pModel->Get_Pivot() * m_pTransform->Get_Matrix();
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * 4.f + _float3(0.f, 1.f, 0.f));
		_float4x4 mBoom		= m_pTransform->Get_Matrix();
		shared_ptr<CGameObject> pPlayer(Function::Find_Player());
		VFXMGR->Fetch(MULTIVFX::STEALTHCHARGE, initializer_list<any>{
			make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01"),
			m_pTransform->Get_State(TRANSFORM::POSITION), make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01"), mBoom, make_pair(pPlayer, "Eff01"), make_pair(pPlayer, "Eff01") });
		m_pTransform->Set_Matrix(mOriginal);
		_player.Enable_ChargeVFX();
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Stealth_ChargeAttack_Detail_ChargeUp.wav"), g_fPlayerSound);
	}
	break;
	case ATK_TYPE::FOLLOW:
		if (_player.Get_Kinesis()->Get_Combo() == 0)
		{
			m_eAnim = ANIMATION::PLAYER::ATK_DASH2_START;
			m_eEndAnim = ANIMATION::PLAYER::ATK_DASH2_END;
			_player.Set_ElecEffectEnable(true);
			m_eCurrentState = PLAYER_STATE::ATTACK_FOLLOW1;
		}
		else if (_player.Get_Kinesis()->Get_Combo() == 1)
		{
			m_eAnim = ANIMATION::PLAYER::ATK_DASH3_START;
			m_eEndAnim = ANIMATION::PLAYER::ATK_DASH3_END;
			_player.Set_ElecEffectEnable(true);
			m_eCurrentState = PLAYER_STATE::ATTACK_FOLLOW2;
			_player.Set_WeaponsActive(false);
			_player.Set_BigWeaponsActive(true);
		}
		else
		{
			m_eAnim = ANIMATION::PLAYER::ATK_DASH2_START;
			m_eEndAnim = ANIMATION::PLAYER::ATK_DASH2_END;
			_player.Get_Kinesis()->Set_Combo(0);
		}
		m_fProportion = 0.5f;
		_player.Get_Kinesis()->Set_ThrowHit(false);
		_player.Get_Kinesis()->Set_Combo(_player.Get_Kinesis()->Get_Combo() + 1);
		break;
	case ATK_TYPE::AIRDASH:
		m_eAnim = ANIMATION::PLAYER::ATK_AIR_DASH_END;
		m_eEndAnim = ANIMATION::PLAYER::JUMP_LANDING;
		m_fProportion = 0.5f;
		m_bGravity = false;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal02.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::AIRDOWN:
		m_eAnim = ANIMATION::PLAYER::ATK_AIR_HOLD_START;
		m_eEndAnim = ANIMATION::PLAYER::ATK_AIR_HOLD_END;
		m_fProportion = 0.5f;
		m_bGravity = false;
		_player.Set_ElecEffectEnable(true);
		m_eCurrentState = PLAYER_STATE::ATTACK_AIRDOWN;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_AirDown.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::ONECLICKCHARGE:
		m_eAnim = ANIMATION::PLAYER::ATK_NORMAL_CHARGE_START;
		m_eEndAnim = ANIMATION::PLAYER::ATK_CHARGE;
		m_fProportion = 0.5f;
		_player.Set_WeaponsActive(false);
		_player.Set_BigWeaponsActive(true);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Stealth_ChargeAttack_Detail_ChargeUp.wav"), g_fPlayerSound);
		break;
	case ATK_TYPE::JUSTDODGE:
		if (_player.Get_Camera()->Get_LockOn())
			if (_player.Get_Camera()->Get_LockOnTransform())
				m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
		if (!m_bInAir)
		{
			m_eAnim = ANIMATION::PLAYER::ATK_JUSTDODGE_ATTACK;
			m_eEndAnim = ANIMATION::PLAYER::ATK_JUSTDODGE_ATTACK;
		}
		else
		{
			m_eAnim = ANIMATION::PLAYER::ATK_AIR_JUSTDODGE;
			m_eEndAnim = ANIMATION::PLAYER::ATK_AIR_JUSTDODGE_LANDING;
		}
		m_fProportion = 0.5f;
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Spine"));
		_player.Set_ElecEffectEnable(true);
		m_eCurrentState = PLAYER_STATE::ATTACK_JUSTDODGE;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_JustDodgeAttack.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_CounterAttack0_0.wav"),TEXT("Voice_Kasane_CounterAttack0_1.wav") }), g_fPlayerSound);
		break;
	case ATK_TYPE::BRAINFIELD:
		m_eAnim = ANIMATION::PLAYER::SWING_A1;
		m_eEndAnim = ANIMATION::PLAYER::SWING_A1;
		m_fProportion = 0.7f;
		_player.Set_WeaponsActive(false);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Attack.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Attack_BrainField0.wav"), g_fPlayerSound);
		break;
	}
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	m_bElectric = _player.Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC ? true : false;
	_player.Enable_WeaponVFX(m_bElectric);

	if (_player.Get_Camera()->Get_LockOn())
		if (_player.Get_Camera()->Get_LockOnTransform())
			m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());

}

void CPlayerAttackState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	switch (m_eType)
	{
	case ATK_TYPE::NORMAL:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK1))
		{
			if (!m_bExecuted[51])
			{
				m_bExecuted[51] = true;
				_player.Set_ElecEffectEnable(true);
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Attack1_00.wav"),TEXT("Voice_Kasane_Attack1_01.wav"),TEXT("Voice_Kasane_Attack1_02.wav") }), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal00.wav"), g_fPlayerSound);
			}
			m_eCurrentState = PLAYER_STATE::ATTACK1;

			if (m_pModel->Get_Proportion() > 0.2f)
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;
				}
				if (pGameInstance->Key_Down(VK_LBUTTON))
				{
					if (_player.Get_Camera()->Get_LockOn())
						if (_player.Get_Camera()->Get_LockOnTransform())
							m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK2, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					_player.Enable_WeaponVFX(m_bElectric);
					m_bHitLagEnable = true;
					m_eEndAnim = ANIMATION::PLAYER::ATK2;
				}

				_player.Set_WeaponEmissive(m_bElectric ? _float4(3.72f, 3.23f, 0.72f, 1.f) : _float4(3.72f, 0.23f, 3.72f, 1.f));
			}
			else
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
				}
			}
			if (m_fTrackPos < 20.f)
			{
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 5.f);
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK2))
		{
			if (!m_bExecuted[52])
			{
				m_bExecuted[52] = true;
				_player.Set_ElecEffectEnable(true);
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Attack2_00.wav"),TEXT("Voice_Kasane_Attack2_01.wav"),TEXT("Voice_Kasane_Attack2_02.wav") }), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal01.wav"), g_fPlayerSound);
			}
			m_eCurrentState = PLAYER_STATE::ATTACK2;

			if (m_pModel->Get_Proportion() > 0.2f)
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;
				}
				if (pGameInstance->Key_Down(VK_LBUTTON))
				{
					if (_player.Get_Camera()->Get_LockOn())
						m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK3, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					_player.Enable_WeaponVFX(m_bElectric);
					m_eEndAnim = ANIMATION::PLAYER::ATK3;
					m_fProportion = 0.5f;
				}

				_player.Set_WeaponEmissive(m_bElectric ? _float4(3.72f, 3.23f, 0.72f, 1.f) : _float4(3.72f, 0.23f, 3.72f, 1.f));
			}
			else
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
				}
			}
			if (m_fTrackPos > 5.f && m_fTrackPos < 20.f)
			{
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 5.f);
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK3))
		{
			if (!m_bExecuted[53])
			{
				m_bExecuted[53] = true;
				_player.Set_ElecEffectEnable(true);
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Attack3_00.wav"),TEXT("Voice_Kasane_Attack3_01.wav"),TEXT("Voice_Kasane_Attack3_02.wav") }), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal02.wav"), g_fPlayerSound);
			}
			m_eCurrentState = PLAYER_STATE::ATTACK3;

			if (m_fTrackPos > 10.f && m_fTrackPos < 25.f)
			{
				m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 5.f);
			}
			if (m_pModel->Get_Proportion() > 0.1f && !m_bExecuted[81])
			{
				m_bExecuted[81] = true;
				m_bHitLagEnable = true;
			}
			if (m_pModel->Get_Proportion() < 0.6f)
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
				}
			}
			else
			{
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;
				}

				_player.Set_WeaponEmissive(_float4(0.f, 0.f, 0.f, 1.f));
			}

			if (m_pModel->Get_Proportion() > 0.4f)
			{
				if (_player.Get_PlayerDesc().AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::FORTH] && pGameInstance->Key_Down(VK_LBUTTON))
				{
					if (_player.Get_Camera()->Get_LockOn())
						m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK4, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					m_eEndAnim = ANIMATION::PLAYER::ATK4;
					m_fProportion = 0.5f;
				}
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK4))
		{
			if (!m_bExecuted[58])
			{
				m_bExecuted[58] = true;
				_player.Set_ElecEffectEnable(true);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack4.wav"), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_Attack4_00.wav.wav"),TEXT("Voice_Kasane_Attack4_01.wav.wav"),TEXT("Voice_Kasane_Attack4_02.wav") }), g_fPlayerSound);
			}

			m_eCurrentState = PLAYER_STATE::ATTACK4;
					if (m_pModel->Get_Proportion() > 0.1f && !m_bExecuted[81])
					{
						m_bExecuted[81] = true;
						m_bHitLagEnable = true;
					}
			if (m_pModel->Get_Proportion() < 0.4f)
			{
				if (m_pModel->Get_Proportion() > 0.2f && !m_bExecuted[11])
				{
					m_bExecuted[11] = true;
					_player.Enable_WeaponVFX(m_bElectric);

					_player.Set_WeaponEmissive(m_bElectric ? _float4(3.72f, 3.23f, 0.72f, 1.f) : _float4(3.72f, 0.23f, 3.72f, 1.f));
				}
				if (m_pModel->Get_Proportion() > 0.3f && !m_bExecuted[12])
				{
					m_bExecuted[12] = true;
					_player.Enable_WeaponVFX(m_bElectric);
				}
				if (m_pModel->Get_Proportion() > 0.05f && m_pModel->Get_Proportion() < 0.2f)
				{
					_player.Get_Camera()->Set_Fixed(true);
					_float3 vDir = m_pCameraTransform->Get_State(TRANSFORM::LOOK);
					m_pCameraTransform->Translate(XMVector3Normalize(vDir)* fTimeDelta * 5.f);
				}
				else
					_player.Get_Camera()->Set_Fixed(false);
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
				}
			}
			else
			{
				_player.Get_Camera()->Set_Fixed(false);
				for (int i = 0; i < _player.Get_Weapons().size(); i++)
				{
					_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;

				}

				_player.Set_WeaponEmissive(_float4(0.f, 0.f, 0.f, 1.f));
			}
		}


		if (!_player.Get_PlayerDesc().bBrainField)
		{
			if (m_eCurrentState == PLAYER_STATE::ATTACK1
				|| m_eCurrentState == PLAYER_STATE::ATTACK2
				|| m_eCurrentState == PLAYER_STATE::ATTACK3
				|| m_eCurrentState == PLAYER_STATE::ATTACK4
				|| m_eCurrentState == PLAYER_STATE::ATTACK_DASH
				|| m_eCurrentState == PLAYER_STATE::ATTACK_BACKSTEP
				|| m_eCurrentState == PLAYER_STATE::ATTACK_UPPER
				|| m_eCurrentState == PLAYER_STATE::JUMP_ATTACK1
				|| m_eCurrentState == PLAYER_STATE::JUMP_ATTACK2
				|| m_eCurrentState == PLAYER_STATE::JUMP_ATTACK3
				|| m_eCurrentState == PLAYER_STATE::ATTACK_BACKSTEP
				|| m_eCurrentState == PLAYER_STATE::ATTACK_FOLLOW1
				|| m_eCurrentState == PLAYER_STATE::ATTACK_FOLLOW2
				|| m_eCurrentState == PLAYER_STATE::ATTACK_AIRDOWN
				|| m_eCurrentState == PLAYER_STATE::ATTACK_JUSTDODGE)
			{
				m_fEffect_Timer += fTimeDelta;

				if (m_fEffect_Timer >= 0.75f)
				{

					shared_ptr pPlayerTransform = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM));

					_float3 Pos = pPlayerTransform->Get_State(TRANSFORM::POSITION);
					Pos.y += 2.f;

					const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

					_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTransform->Get_Matrix();

					mat2._42 += 0.25f;

					CEffect_Manager::Get_Instance()->Fetch(VFX::PSY_BRAINITER, mat2);

					m_fEffect_Timer = 0.f;
				}
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::BACKSTEP:
		if (!m_bExecuted[54])
		{
			m_bExecuted[54] = true;
			_player.Set_ElecEffectEnable(true);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_BACKSTEP_ELECTRIC))
		{
			if (m_pModel->Get_Proportion() < 0.2f)
				m_pTransform->Translate(XMVectorLerp(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 7.f, _float3(0, 0, 0), 0.1f));
			if (m_pModel->Get_Proportion() > 0.25 && !m_bExecuted[50])
			{
				m_bExecuted[50] = true;
				_player.Enable_WeaponVFX(m_bElectric);
			}
			__super::Late_Update(_player, fTimeDelta);
		}
		else if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_BACKSTEP))
			__super::Late_Update_Root(_player, fTimeDelta, 1.f);
		break;
	case ATK_TYPE::UPPER:
		if (!m_bExecuted[55])
		{
			m_bExecuted[55] = true;
			_player.Set_ElecEffectEnable(true);
		}
		if (m_bExecuted[2] && !m_bInAir && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_LANDING))
		{
			m_bJumpRise = false;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_UPPER_LANDING, _player.Get_PlayerDesc().fAnimSpeed * 2.f, false, g_fDefaultInterpolationDuration, false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_UPPER_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_UPPER, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			_player.Enable_WeaponVFX(m_bElectric);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_UPPER) && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_UPPER_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_START) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER_FALL))
		{
			m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fUpperAtkHeight, 0, 0));
			m_bExecuted[2] = true;
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_UPPER) && m_pModel->Get_Proportion() > 0.6f)
		{
			if (pGameInstance->Key_Down(VK_LBUTTON))
			{
				m_eType = ATK_TYPE::JUMP;
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR1_WP, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				_player.Enable_WeaponVFX(m_bElectric);
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::DASH:
		if (!m_bExecuted[55])
		{
			m_bExecuted[55] = true;
			_player.Set_ElecEffectEnable(true);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_DASH_START))
		{

		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_DASH_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_DASH_END, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
		break;
	case ATK_TYPE::JUMP:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR1_WP) &&
			m_pModel->Get_Proportion() > 0.3f && !m_bExecuted[0])
		{
			if (pGameInstance->Key_Down(VK_LBUTTON))
			{
				m_bGravity = false;
				m_fGravityTime = 0.f;
				m_vPreRootPos = _float4(0, 0, 0, 1);
				m_bExecuted[0] = true;
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR2_WP, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				_player.Enable_WeaponVFX(m_bElectric);
				m_bHitLagEnable = true;
				m_eCurrentState = PLAYER_STATE::JUMP_ATTACK2;
				if (!m_bExecuted[56])
				{
					m_bExecuted[56] = true;
					_player.Set_ElecEffectEnable(true);
				}
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_JumpAttack2_00.wav"),TEXT("Voice_Kasane_JumpAttack2_01.wav"),TEXT("Voice_Kasane_JumpAttack2_02.wav") }), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal01.wav"), g_fPlayerSound);
			}
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR2_WP) &&
			m_pModel->Get_Proportion() > 0.25f && !m_bExecuted[1])
		{
			if (pGameInstance->Key_Down(VK_LBUTTON))
			{
				m_bGravity = false;
				m_fGravityTime = 0.f;
				m_vPreRootPos = _float4(0, 0, 0, 1);
				m_bExecuted[1] = true;
				if (m_eCurSAS == PLAYER_SAS::ELECTRIC)
				{
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR3_WP_ELECTRIC, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					m_eCurrentState = PLAYER_STATE::JUMP_ATTACK3;
					if (!m_bExecuted[57])
					{
						m_bExecuted[57] = true;
						_player.Set_ElecEffectEnable(true);
					}
				}
				else
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR3_WP, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				_player.Enable_WeaponVFX(m_bElectric);
				m_bHitLagEnable = true;
				CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_JumpAttack1_00.wav"),TEXT("Voice_Kasane_JumpAttack1_01.wav"),TEXT("Voice_Kasane_JumpAttack1_02.wav") }), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Attack_Normal02.wav"), g_fPlayerSound);
			}
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR1_WP) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR2_WP) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR3_WP) ||
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR3_WP_ELECTRIC))
		{
			//m_pTransform->Translate(_float4(0, fTimeDelta * _player.Get_PlayerDesc().fJumpAtkHeight, 0, 0));

			_float fProportion;
			switch (m_pModel->Get_CurrentAnimIndex())
			{
			case (_uint)ANIMATION::PLAYER::ATK_AIR1_WP:
				fProportion = 0.5f;
				break;
			case (_uint)ANIMATION::PLAYER::ATK_AIR2_WP:
				fProportion = 0.4f;
				break;
			case (_uint)ANIMATION::PLAYER::ATK_AIR3_WP:
				fProportion = 0.35f;
				break;
			case (_uint)ANIMATION::PLAYER::ATK_AIR3_WP_ELECTRIC:
				fProportion = 0.35f;
				break;
			}
			if (m_pModel->Get_Proportion() > fProportion)
			{
				m_bGravity = true;
				if (!m_bExecuted[77])
				{
					m_bExecuted[77] = true;
					m_fGravityTime = 0.3f;
				}
			}
			
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR1_WP) ||
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR2_WP) ||
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR3_WP) ||
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR3_WP_ELECTRIC))
			{
				m_bGravity = true;
				m_fGravityTime = 0.3f;
			}
		}
		if ((m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN || m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_SIDES) && !m_bExecuted[3])
		{
			m_bExecuted[3] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_UPPER_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		__super::Late_Update(_player, fTimeDelta);
			break;
	case ATK_TYPE::CHARGE:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_CHARGE_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_CHARGE_LOOP, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
//			_player.Enable_WeaponVFX(m_bElectric);
			for (int i = 0; i < _player.Get_Weapons().size(); i++)
			{
				_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = false;
			}
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Stealth_ChargeAttack_Detail_ChargeUp_Loop.wav"), g_fPlayerSound);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_CHARGE_LOOP))
		{
			m_fChargeTime += fTimeDelta;

			_player.Blink_Transparency(fTimeDelta);

			if (m_fChargeTime > 4.35f)
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_CHARGE_ATK_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				m_eEndAnim = ANIMATION::PLAYER::ATK_CHARGE_ATK_END;
//				_player.Enable_WeaponVFX(m_bElectric);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Stealth_ChargeAttack_Detail_ChargeUp_Attack.wav"), g_fPlayerSound);
			}

		}
		if (!pGameInstance->Key_Hold('Z'))
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_CHARGE_LOOP) && !m_bExecuted[1])
			{
				m_bExecuted[1] = true;

				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_CHARGE_CANCEL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				m_eEndAnim = ANIMATION::PLAYER::ATK_CHARGE_CANCEL;
			}
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_CHARGE_ATK_START) && !m_bExecuted[2])
		{
			m_bExecuted[2] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_CHARGE_ATK_END, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
//			_player.Enable_WeaponVFX(m_bElectric);

			for (int i = 0; i < _player.Get_Weapons().size(); i++)
			{
				_player.Get_Weapons()[i]->GetCollider()->GetActorDesc().bActive = true;
			}
			_player.Set_Transparency(false);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_CHARGE_ATK_START))
		{
			if (m_pModel->Get_Proportion() < 0.2f)
				m_pTransform->Translate(XMVectorLerp(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 50.f, _float3(0, 0, 0), 0.1f));
			else
				m_pTransform->Translate(XMVectorLerp(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 10.f, _float3(0, 0, 0), 0.1f));
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::FOLLOW:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(m_eEndAnim) &&
			m_pModel->Get_Proportion() > 0.15f && !m_bExecuted[25])
		{
			m_bExecuted[25] = true;
			_player.Enable_WeaponVFX(m_bElectric);
			if(m_eAnim == ANIMATION::PLAYER::ATK_DASH2_START)
			{
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Attack_FollowUp_Hit_2.wav"), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_Attack_FollowUp_Hit_2.wav"), g_fPlayerSound);
			}
			else if(m_eAnim == ANIMATION::PLAYER::ATK_DASH3_START)
			{
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Attack_FollowUp_Hit_1.wav"), g_fPlayerSound);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_Attack_FollowUp_Hit_1.wav"), g_fPlayerSound);
			}
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eAnim) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Enable_WeaponVFX(m_bElectric);
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eEndAnim, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		__super::Late_Update_Root(_player, fTimeDelta, 2.f);
		break;
	case ATK_TYPE::AIRDASH:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR_DASH_END))
		{
			if (m_pModel->Get_Proportion() > 0.5f && !m_bExecuted[0])
			{
				m_bExecuted[0] = true;
				m_bGravity = true;
				m_fGravityTime = 0.f;
			}
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR_DASH_END))
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
			}
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_FALL) && !m_bInAir)
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::AIRDOWN:
		m_eCurrentState = PLAYER_STATE::ATTACK_AIRDOWN;
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR_HOLD_START))
		{
			if (m_pModel->Get_Proportion() > 0.1f && !m_bExecuted[11])
			{
				m_bExecuted[11] = true;
				m_bGravity = true;
				m_fGravityTime = 0.f;
			}
			if (m_pModel->Get_Proportion() > 0.4f && !m_bExecuted[12])
			{
				m_bExecuted[12] = true;
				_player.Enable_WeaponVFX(m_bElectric);

			}
			if (!m_bInAir && m_pModel->Get_Proportion() > 0.5f)
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR_HOLD_END, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
			}
		}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::ONECLICKCHARGE:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_START) && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_LOOP, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, true);
			_player.Enable_WeaponVFX(m_bElectric);
		}
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_LOOP))
		{
			m_fChargeTime += fTimeDelta;

			if (m_fChargeTime > 1.f)
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_CHARGE, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
				m_eEndAnim = ANIMATION::PLAYER::ATK_CHARGE;
				_player.Enable_WeaponVFX(m_bElectric);

				if (!m_bExecuted[60])
				{
					m_bExecuted[60] = true;
					_player.Set_ElecEffectEnable(true);
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_DashAttack.wav"), g_fPlayerSound);
				}

				m_eCurrentState = PLAYER_STATE::ATTACK_CHARGE;
			}
		}
		if (!pGameInstance->Key_Hold('Z'))
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_LOOP) && !m_bExecuted[1])
			{
				m_bExecuted[1] = true;
				
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_CANCEL, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					m_eEndAnim = ANIMATION::PLAYER::ATK_NORMAL_CHARGE_CANCEL;
			}
		}
		//if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_NORMAL_CHARGE_START))
		//{
		//	if(!m_bExecuted[2] && m_pModel->Get_Proportion() > 0.3f)
		//	{
		//		m_bExecuted[2] = true;
		//		cout << "DDONG1" << endl;
		//		_player.Enable_WeaponVFX(m_bElectric);
		//	}
		//	if (!m_bExecuted[3] && m_pModel->Get_Proportion() > 0.6f)
		//	{
		//		m_bExecuted[3] = true;
		//		cout << "DDONG2" << endl;
		//		_player.Enable_WeaponVFX(m_bElectric);
		//	}
		//}
		__super::Late_Update(_player, fTimeDelta);
		break;
	case ATK_TYPE::JUSTDODGE:
		if (m_bInAir)
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::ATK_AIR_JUSTDODGE) &&
				!m_bExecuted[0])
			{
				m_bExecuted[0] = true;
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR_JUSTDODGE_FALL, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
			}
		}
		else
		{
			if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::ATK_AIR_JUSTDODGE_FALL))
			{
				_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR_JUSTDODGE_LANDING, _player.Get_PlayerDesc().fAnimSpeed, false, 0.f, false);
			}
		}
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
		break;
	case ATK_TYPE::BRAINFIELD:
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_A1))
		{
			m_eCurrentState = PLAYER_STATE::SWING_A1;
			if (m_pModel->Get_Proportion() < 0.5f)
			{
				if(static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = true;
			}
			else
			{
				if (static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = false;
			}
			if (m_pModel->Get_Proportion() > 0.5f)
			{
				if (pGameInstance->Key_Down(VK_LBUTTON))
				{
					if (_player.Get_Camera()->Get_LockOn())
						if (_player.Get_Camera()->Get_LockOnTransform())
							m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::SWING_A2, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					if(dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
						static_pointer_cast<CMap_MindRoom_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Set_LagEnable(true);
					m_eEndAnim = ANIMATION::PLAYER::SWING_A2;
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Attack.wav"), g_fPlayerSound);
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Attack_BrainField1.wav"), g_fPlayerSound);
				}
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_A2))
		{
			if (m_pModel->Get_Proportion() < 0.5f && m_pModel->Get_Proportion() > 0.4f)
			{
				if (static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = true;
			}
			else
			{
				if (static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = false;
			}
			if (m_pModel->Get_Proportion() < 0.15f)
			{
				_player.Get_Kinesis()->Change_BF_Attack(2, fTimeDelta);
			}
			if (m_pModel->Get_Proportion() > 0.6f)
			{
				if (pGameInstance->Key_Down(VK_LBUTTON))
				{
					if (_player.Get_Camera()->Get_LockOn())
						m_pTransform->LookAt_Interpolation(_player.Get_Camera()->Get_LockOnTransform());
					_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::SWING_A3, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
					_player.Enable_WeaponVFX(m_bElectric);
					m_eEndAnim = ANIMATION::PLAYER::SWING_A3;
					m_fProportion = 0.9f;
					if (dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
						static_pointer_cast<CMap_MindRoom_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Set_LagEnable(true);
					_player.Get_Kinesis()->Change_BF_Attack(3, fTimeDelta);
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Attack.wav"), g_fPlayerSound);
					CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Attack_BrainField2.wav"), g_fPlayerSound);
				}
			}
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_A3))
		{
			if (m_pModel->Get_Proportion() < 0.5f && m_pModel->Get_Proportion() > 0.3f)
			{
				if (static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = true;
			}
			else
			{
				if (static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
					static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_ActorDesc().bActive = false;
			}
			if (m_pModel->Get_Proportion() < 0.15f)
			{
				_player.Get_Kinesis()->Change_BF_Attack(3, fTimeDelta);
			}
			m_eCurrentState = PLAYER_STATE::SWING_A3;
			if (m_pModel->Get_Proportion() > 0.1f && !m_bExecuted[81])
			{
				m_bExecuted[81] = true;
				m_bHitLagEnable = true;
			}
		}
		__super::Late_Update_Root(_player, fTimeDelta, 1.f);
		break;
	}


}

void CPlayerAttackState::Exit(CPlayer& _player)
{
	__super::Exit(_player);

		m_bGravity = true;
		m_fGravityTime = 0.f;

	_player.Set_BigWeaponsActive(false);
	_player.Get_Camera()->Set_Fixed(false);

	_player.Set_WeaponEmissive(_float4(0.f, 0.f, 0.f, 1.f));

	if (m_eType == ATK_TYPE::BRAINFIELD)
	{
		CGameInstance::Get_Instance()->Erase_InteractiveObject(_player.Get_Kinesis()->Get_CapturedObject());
		_player.Get_Kinesis()->DestroyObject();
		_player.Get_Kinesis()->Reset();
	}

}

//void CPlayerAttackState::AirDownAttack(CPlayer& _player)
//{
//	auto pGameInstance = CGameInstance::Get_Instance();
//	if (pGameInstance->Key_Down('C'))
//	{
//		m_eAnim = ANIMATION::PLAYER::ATK_AIR_HOLD_START;
//		m_eEndAnim = ANIMATION::PLAYER::ATK_AIR_HOLD_END;
//		m_fProportion = 0.5f;
//		m_bGravity = false;
//		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::ATK_AIR_HOLD_START, _player.Get_PlayerDesc().fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
//	}
//}
