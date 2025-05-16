#include "ClientPCH.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "PlayerDodgeState.h"
#include "PlayerExecuteState.h"
#include "PlayerSpecialThrowState.h"
#include "PlayerDriveModeState.h"
#include "PlayerHealState.h"
#include "PlayerBrainFieldStartState.h"
#include "PlayerBrainFieldExitState.h"
#include "PlayerBrainFieldCutsceneState.h"
#include "PlayerBrainFieldCutsceneFinalState.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "PhysXCollider.h"
#include "Model.h"
#include "Animator.h"
#include "Psychokinesis.h"
#include "Map_Special_Dynamic.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Monster.h"
#include "Effect_Manager.h"
#include "PlayerBrainFieldExitState.h"

CPlayerIdleState::CPlayerIdleState()
{
}

shared_ptr<CPlayerState> CPlayerIdleState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Hold('W'))
	{
		if (pGameInstance->Key_Hold('A'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::FL);
		}
		if (pGameInstance->Key_Hold('D'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::FR);
		}
		return make_shared<CPlayerWalkState>(PLAYER_DIR::F);
	}
	if (pGameInstance->Key_Hold('S'))
	{
		if (pGameInstance->Key_Hold('A'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::BL);
		}
		if (pGameInstance->Key_Hold('D'))
		{
			return make_shared<CPlayerWalkState>(PLAYER_DIR::BR);
		}
		return make_shared<CPlayerWalkState>(PLAYER_DIR::B);
	}
	if (pGameInstance->Key_Hold('A'))
	{
		return make_shared<CPlayerWalkState>(PLAYER_DIR::L);
	}
	if (pGameInstance->Key_Hold('D'))
	{
		return make_shared<CPlayerWalkState>(PLAYER_DIR::R);
	}

	if (pGameInstance->Key_Down(VK_LBUTTON))
	{
		if (_player.Get_PlayerDesc().bBrainField)
		{
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::BRAINFIELD);
		}
		else
		{
			if (!_player.Get_Kinesis()->Get_ThrowHit())
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::NORMAL);
			else
				return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::FOLLOW);
		}
	}

	if (pGameInstance->Key_Down('C'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::BACKSTEP);

	if (pGameInstance->Key_Down('F'))
		return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::UPPER);

	if (nullptr != _player.Get_Kinesis()->Get_CapturedObject())
	{
		if(shared_ptr<CMap_Special_Dynamic> pSpecial = dynamic_pointer_cast<CMap_Special_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
		{
			if(pGameInstance->Key_Hold('G'))
				return make_shared<CPlayerSpecialThrowState>(pSpecial->Get_Type());
		}
		else if (nullptr != dynamic_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject()))
		{
			if(_player.Get_PlayerDesc().fKinesisGauge > 20.f)
			{
				if (pGameInstance->Key_Hold(VK_RBUTTON))
					return make_shared<CPlayerThrowState>();
			}
			else if (pGameInstance->Key_Hold(VK_RBUTTON))
			{
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("UI_SFX_Kasane_No_PsycoGauge.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);
				_player.Get_Kinesis()->Set_CapturedObject(nullptr);
			}
		}                                                                                                                                  
	}

	if (pGameInstance->Key_Down(VK_SPACE))
		return make_shared<CPlayerJumpState>(CPlayerJumpState::JUMP_TYPE::INPLACE);
	weak_ptr<CMonster> PickedMonster = _player.Get_Camera()->Get_LockedMonster();// ->Get_BcChance()
	if (PickedMonster.lock() != nullptr)
	{
		if (pGameInstance->Key_Down('G') && PickedMonster.lock()->Get_BcChance() && _player.Get_Camera()->Get_LockOnTransform() != nullptr)
			return make_shared<CPlayerExecuteState>();
	}

	if (pGameInstance->Key_Down('Z'))
	{
		if (_player.Get_PlayerDesc().eSAS == PLAYER_SAS::STEALTH)
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::CHARGE);
		else if (true == _player.Get_PlayerDesc().AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::ONECLICKCHARGE])
			return make_shared<CPlayerAttackState>(CPlayerAttackState::ATK_TYPE::ONECLICKCHARGE);
	}

	if (pGameInstance->Key_Down('R') && !_player.Get_PlayerDesc().bDriveMode)
		return make_shared<CPlayerDriveModeState>();

	if (pGameInstance->Key_Down('T'))
	{
		shared_ptr<CTransform> pPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

		_float3 PlayerPos = pPlayerTramsform->Get_State(TRANSFORM::POSITION);
		_float3 PlayerUp = pPlayerTramsform->Get_State(TRANSFORM::UP);

		_float3 Pos = PlayerPos;
		_float3 Pos2 = PlayerPos;
		Pos.y += 2.5f;
		Pos2.y += 1.25f;

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::ITEM_USE, 
			initializer_list<any>{Pos, Pos, Pos2, Pos2, Pos2 });

		return make_shared<CPlayerHealState>();
	}

	if(pGameInstance->Current_Scene() == SCENE::MINDROOM)
	{
		if (!_player.Get_PlayerDesc().bBrainField && pGameInstance->Key_Down('B'))
			return make_shared<CPlayerBrainFieldStartState>();
		else if (_player.Get_PlayerDesc().bBrainField && pGameInstance->Key_Down('B'))
			_player.Exit_BrainField();
	}

#ifdef _DEBUG

	if (pGameInstance->Key_Down('N'))
		return make_shared<CPlayerBrainFieldCutsceneState>();

	if (pGameInstance->Key_Down('M'))
		return make_shared<CPlayerBrainFieldCutsceneFinalState>();

	if (pGameInstance->Key_Down('L'))
		return make_shared<CPlayerBrainFieldExitState>();
#endif

	return nullptr;
}

void CPlayerIdleState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::IDLE;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::IDLE,1.f,false,0.f,false);
}

void CPlayerIdleState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerIdleState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
}
