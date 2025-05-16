#include "ClientPCH.h"
#include "SimulationEventCallBack.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "Camera_Player.h"
#include "Monster.h"
#include "Monster_Projectile.h"
#include "PhysXCollider.h"
#include "ObjectLayer.h"
#include "PlayerState.h"
#include "PlayerHurtState.h"
#include "Psychokinesis.h"
#include "UI_Manager.h"

CSimulationEventCallBack::CSimulationEventCallBack()
{
}

void CSimulationEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	int a = 6;
}

void CSimulationEventCallBack::onWake(PxActor** actors, PxU32 count)
{
	int a = 6;
}

void CSimulationEventCallBack::onSleep(PxActor** actors, PxU32 count)
{
	int a = 6;
}

void CSimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{

	int a = 5;
}

void CSimulationEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//ignore pairs when shapes have been deleted
	if (pairs->flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
		PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		return;

	CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	for (_int i = 0; i < static_cast<_int>(count); i++)
	{
		const PxTriggerPair& pair = pairs[i];
		
		ACTORDESC* triggerDesc = (ACTORDESC*)(pair.triggerActor->userData);
		ACTORDESC* otherDesc = (ACTORDESC*)(pair.otherActor->userData);
		if (nullptr == triggerDesc || nullptr == otherDesc)
			return;

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGBODY &&
			otherDesc->strTag == ACTORTAG::INTERACTIVE &&
			otherDesc->bThrow == true &&
			otherDesc->bActive == true &&
			triggerDesc->bActive == true &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//monster hit by throwing object
			triggerDesc->bThrowHit = true;
			triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			otherDesc->bThrowHit = true;
			pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			HitMonsterByThrow(triggerDesc, otherDesc);
			cout << "Interactive" << endl;
		}

		if (triggerDesc->strTag == ACTORTAG::KAREN_BIGBODY &&
			otherDesc->strTag == ACTORTAG::INTERACTIVE &&
			otherDesc->bActive == true &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//monster hit by throwing object
			triggerDesc->bThrowHit = true;
			triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			otherDesc->bThrowHit = true;
			pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			HitMonsterByThrow(triggerDesc, otherDesc);
			cout << "KAREN_THROWHIT" << endl;
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGBODY &&
			otherDesc->strTag == ACTORTAG::INTERACTIVE_SPECIAL &&
			otherDesc->bThrow == true &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//monster hit by throwing object
			otherDesc->bThrowHit = true;
			otherDesc->bThrow = false;


			triggerDesc->stHitDesc.strAttackTag = otherDesc->strTag;
			triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			triggerDesc->stHitDesc.AttackColliderActive = &otherDesc->bActive;
			triggerDesc->bThrowHit = true;
			pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			triggerDesc->stHitDesc.vAttackPose = otherDesc->vOwnerPose;
			cout << "INTERACTIVE_SPECIAL" << endl;
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGBODY &&
			otherDesc->strTag == ACTORTAG::FALLEN_OBJECT &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND &&
			triggerDesc->bActive)
		{
			//monster hit by throwing object
			otherDesc->bThrowHit = true;
			//otherDesc->bActive = false;
			//otherDesc->bThrow = false;
			triggerDesc->stHitDesc.strAttackTag = otherDesc->strTag;
			triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			triggerDesc->stHitDesc.AttackColliderActive = &otherDesc->bActive;
			triggerDesc->bThrowHit = true;
	
			//pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			triggerDesc->stHitDesc.vAttackPose = otherDesc->vOwnerPose;
			cout << "FALLEN_OBJECT_HIT" << endl;
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGBODY &&
			otherDesc->strTag == ACTORTAG::FALLEN_OBJECT &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//monster hit by throwing object
		/*	otherDesc->bThrowHit = false;
			triggerDesc->stHitDesc.AttackColliderActive = nullptr;*/
			cout << "FALLEN_OBJECT_LOST" << endl;
		}

		if (triggerDesc->strTag == ACTORTAG::INTERACTIVE_BEAM &&
			otherDesc->strTag == ACTORTAG::MONSTER &&
			triggerDesc->bThrow == true &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerDesc->bThrowHit = true;
			triggerDesc->bThrow = false;
			otherDesc->stHitDesc.strAttackTag = triggerDesc->strTag;
			otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
			otherDesc->bThrowHit = true;
			pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			otherDesc->stHitDesc.vAttackPose = triggerDesc->vOwnerPose;
			HitMonsterBySpecialThrow(triggerDesc, otherDesc);
		}

		if (triggerDesc->strTag == ACTORTAG::INTERACTIVE_BUS &&
			otherDesc->strTag == ACTORTAG::MONSTER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerDesc->bThrowHit = true;
			triggerDesc->bThrow = false;
			otherDesc->stHitDesc.strAttackTag = triggerDesc->strTag;
			otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
			otherDesc->stHitDesc.strAttackTag = triggerDesc->strTag;
			otherDesc->bThrowHit = true;
			pPlayer->Get_Kinesis()->Set_ThrowHit(true);
			otherDesc->stHitDesc.vAttackPose = triggerDesc->vOwnerPose;
			HitMonsterBySpecialThrow(triggerDesc, otherDesc);
		}

#pragma region Player->Monster
		if ((triggerDesc->strTag == ACTORTAG::PLAYER_ATTACK ||
			triggerDesc->strTag == ACTORTAG::PLAYER_BIGATTACK) &&
			triggerDesc->bActive && 
			(otherDesc->strTag == ACTORTAG::MONSTER ||
				otherDesc->strTag == ACTORTAG::FALLEN_OBJECT) &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerDesc->bHit = true;
			otherDesc->bHit = true;
			otherDesc->stHitDesc.AttackColliderActive = &triggerDesc->bActive;
			//otherDesc->stHitDesc.mEffectMat = triggerDesc->mEffectMat;
			otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
			otherDesc->mEffectMat = triggerDesc->mOwnerMat;
			pair.otherActor->userData = otherDesc;
			HitMonster(triggerDesc, otherDesc);
			cout << "HITMONSTER" << endl;
		}

		if ((triggerDesc->strTag == ACTORTAG::PLAYER_ATTACK ||
			triggerDesc->strTag == ACTORTAG::PLAYER_BIGATTACK) &&
			(otherDesc->strTag == ACTORTAG::FLY) &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerDesc->bHit = true;
			otherDesc->bHit = true;
			otherDesc->stHitDesc.AttackColliderActive = &triggerDesc->bActive;
			//otherDesc->stHitDesc.mEffectMat = triggerDesc->mEffectMat;
			otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
			otherDesc->mEffectMat = triggerDesc->mOwnerMat;
			pair.otherActor->userData = otherDesc;
			HitMonster(triggerDesc, otherDesc);
		}


		if ((otherDesc->strTag == ACTORTAG::PLAYER_ATTACK) &&
			triggerDesc->strTag == ACTORTAG::MONSTER_DODGESENCE &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			auto State = pPlayer->Get_PlayerState()->Get_CurrentState();
			if (pPlayer->Get_PlayerState()->Get_CurrentState() >= PLAYER_STATE::ATTACK && pPlayer->Get_PlayerState()->Get_CurrentState() <= PLAYER_STATE::SWING_A3)
			{
				triggerDesc->bCapture = true;
				triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			}
		}
		if ((otherDesc->strTag == ACTORTAG::PLAYER_ATTACK ) &&
			triggerDesc->strTag == ACTORTAG::MONSTER_DODGESENCE &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (pPlayer->Get_PlayerState()->Get_CurrentState() >= PLAYER_STATE::ATTACK && pPlayer->Get_PlayerState()->Get_CurrentState() <= PLAYER_STATE::SWING_A3)
			{
				triggerDesc->bCapture = true;
				triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			}
		}
		if (otherDesc->strTag == ACTORTAG::INTERACTIVE &&
			triggerDesc->strTag == ACTORTAG::MONSTER_DODGESENCE &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//cout << "PLAYERATTACK" << endl;
			if (otherDesc->bThrow)
			{
				//HitMonster(triggerDesc, otherDesc);
				//cout << "dodge" << endl;
				triggerDesc->bCapture = true;
				triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
				otherDesc->bHit = true;
			}
		}
		if (otherDesc->strTag == ACTORTAG::TEAMMATE_ATTACK &&
			triggerDesc->strTag == ACTORTAG::MONSTER_DODGESENCE &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//cout << "dodge" << endl;
			triggerDesc->bCapture = true;
			triggerDesc->stHitDesc.iDamage = otherDesc->stAttackDesc.iDamage;
			otherDesc->bHit = true;
		}

		if (triggerDesc->strTag == ACTORTAG::PLAYER_KICK &&
			otherDesc->strTag == ACTORTAG::MONSTER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_UPPER)
			{
				otherDesc->bAirBone = true;
				otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
				pair.otherActor->userData = otherDesc;
				KickMonster(triggerDesc, otherDesc);
			}
		}

		if (triggerDesc->strTag == ACTORTAG::PLAYER_ATTACK &&
			otherDesc->strTag == ACTORTAG::MONSTER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pair.otherActor->userData = otherDesc;
		}
#pragma endregion


#pragma region Monster->Player
		if (triggerDesc->strTag == ACTORTAG::MONSTER_ATTACK &&
			otherDesc->strTag == ACTORTAG::PLAYER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			otherDesc->bHit = true;
			triggerDesc->bHit = true;
			//PxTransform triggerPose = static_cast<PxRigidStatic*>(pair.triggerActor)->getGlobalPose();
			otherDesc->vTargetPos = _float3(triggerDesc->mOwnerMat.r[3]); 
			otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
			otherDesc->stHitDesc.AttackColliderActive = &triggerDesc->bActive;
			otherDesc->stHitDesc.eHurtType = triggerDesc->stAttackDesc.eHurtType;
			otherDesc->stHitDesc.iType = triggerDesc->stAttackDesc.iType;
			pair.otherActor->userData = otherDesc;
			HitPlayer(triggerDesc, otherDesc);
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_ATTACK &&
			otherDesc->strTag == ACTORTAG::PLAYER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			otherDesc->bHit = false;
			pair.otherActor->userData = otherDesc;
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGATTACK &&
			triggerDesc->bActive && 
			otherDesc->strTag == ACTORTAG::PLAYER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			otherDesc->bBigHit = true;
			otherDesc->stHitDesc.AttackColliderActive = &triggerDesc->bActive;
		}

		if (triggerDesc->strTag == ACTORTAG::MONSTER_BIGATTACK &&
			otherDesc->strTag == ACTORTAG::PLAYER &&
			pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			otherDesc->bBigHit = false;
		}
#pragma endregion		

#pragma region Teammate->Monster
			if (triggerDesc->strTag == ACTORTAG::TEAMMATE_ATTACK &&
				otherDesc->strTag == ACTORTAG::MONSTER &&
				pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND &&
				triggerDesc->bActive)
			{
				TeammateAttackMonster(triggerDesc, otherDesc);
				triggerDesc->bHit = true;
				otherDesc->bHit = true;
				otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
				otherDesc->stHitDesc.strAttackTag = triggerDesc->strTag;
				pair.otherActor->userData = otherDesc;
			}

			if (triggerDesc->strTag == ACTORTAG::TEAMMATE_ATTACK &&
				otherDesc->strTag == ACTORTAG::MONSTER &&
				pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				otherDesc->bHit = false;
				pair.otherActor->userData = otherDesc;
			}
#pragma endregion
#pragma region Monster->Teammate
			if (triggerDesc->strTag == ACTORTAG::MONSTER_ATTACK &&
				otherDesc->strTag == ACTORTAG::TEAMMATE &&
				pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				triggerDesc->bHit = true;
				otherDesc->bHit = true;
				otherDesc->stHitDesc.iDamage = triggerDesc->stAttackDesc.iDamage;
				otherDesc->stHitDesc.AttackColliderActive = &triggerDesc->bActive;
				otherDesc->stHitDesc.eHurtType = triggerDesc->stAttackDesc.eHurtType;
				pair.otherActor->userData = otherDesc;

				//cout << "Teammate Damaged" << endl;
			}

			if (triggerDesc->strTag == ACTORTAG::MONSTER_ATTACK &&
				otherDesc->strTag == ACTORTAG::TEAMMATE &&
				pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				otherDesc->bHit = false;
				pair.otherActor->userData = otherDesc;
			}
#pragma endregion


			if (triggerDesc->strTag == ACTORTAG::EVENT &&
				otherDesc->strTag == ACTORTAG::PLAYER &&
				pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				triggerDesc->bHit = true;
				pair.otherActor->userData = otherDesc;
			}
	}
}

void CSimulationEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	int a = 6;
}

void CSimulationEventCallBack::HitMonster(ACTORDESC* trigger, ACTORDESC* other)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	pPlayer->Get_PlayerState()->Execute_Lag();
	pPlayer->Get_Camera()->Set_Shake();

	shared_ptr<CTransform> pPlayerTramsform =
		CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	PLAYER_STATE Player_State = pPlayer->Get_PlayerState()->Get_CurrentState();

	_float fCrit = Function::RandomFloat(0.f, 1.f);

	if (pPlayer->Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC)
	{
		if (pPlayer->Get_ElecEffectEnable())
		{
			_float4x4 scaleMat = trigger->mEffectMat;
			_float4x4 mat = XMMatrixScaling(2.f, 2.f, 2.f) * trigger->mEffectMat;
			_float4x4 mat2 = XMMatrixScaling(3.f, 3.f, 3.f) * trigger->mEffectMat;
			_float4x4 mat_half = XMMatrixScaling(0.5f, 0.5f, 0.5f) * trigger->mEffectMat;
			_float4x4 mat_Particle = XMMatrixScaling(5.f, 5.f, 5.f) * trigger->mEffectMat;
			_float3 vTriggerPos = trigger->vTargetPos;
			_float3 vPlayerPos = pPlayerTramsform->Get_State(TRANSFORM::POSITION);
			_float3 vPlayerLook = pPlayerTramsform->Get_State(TRANSFORM::LOOK);
			_float3 vPlayerRight = pPlayerTramsform->Get_State(TRANSFORM::RIGHT);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<_float> RandomValue(0.f, 3.f);

			_float4x4 RandomMat;

			if (RandomValue(gen) < 1.f)
			{
				RandomMat = mat * XMMatrixRotationX(XMConvertToRadians(90.f));
			}
			else if (RandomValue(gen) > 1.f && RandomValue(gen) <= 2.f)
			{
				RandomMat = mat * XMMatrixRotationY(XMConvertToRadians(90.f));
			}
			else if (RandomValue(gen) > 2.f)
			{
				RandomMat = mat * XMMatrixRotationZ(XMConvertToRadians(90.f));
			}

			vTriggerPos.y += 1.25f;
			scaleMat._42 += 1.25f;
			mat._42 += 1.25f;
			mat2._42 += 1.25f;
			if (Player_State == PLAYER_STATE::ATTACK1)
			{

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC,
					initializer_list<any>{ mat, mat, mat_Particle });

			}
			else if (Player_State == PLAYER_STATE::ATTACK2)
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_SECOND_ELEC,
					initializer_list<any>{mat, mat, mat, mat});

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK3)
			{
				_float3 PlayerleftFoward = vPlayerPos + vPlayerLook - vPlayerRight;
				PlayerleftFoward.y += 1.25f;
				_float3 PlayerRightFoward = vPlayerPos + vPlayerLook + vPlayerRight;
				PlayerRightFoward.y += 1.25f;

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_SECOND_ELEC,
					initializer_list<any>{mat, mat, mat, mat});

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_THIRD_ELEC_1,
					initializer_list<any>{PlayerleftFoward, PlayerleftFoward, PlayerleftFoward,
					PlayerleftFoward, PlayerleftFoward, PlayerleftFoward, PlayerleftFoward });

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_THIRD_ELEC_2,
					initializer_list<any>{PlayerRightFoward, PlayerRightFoward, PlayerRightFoward,
					PlayerRightFoward, PlayerRightFoward, PlayerRightFoward, PlayerRightFoward });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK4)
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_BACKSTEP,
					initializer_list<any>{mat, mat, mat, mat, mat, 
					mat, mat, mat, mat, mat,
					scaleMat, scaleMat, scaleMat, scaleMat, scaleMat, 
					mat, mat, mat, mat, mat,
					mat });
				CEffect_Manager::Get_Instance()->Fetch(VFX::ELEC_ATTACK_SECOND_DISTORTION, mat2);
			}
			else if (Player_State == PLAYER_STATE::ATTACK_BACKSTEP)
			{
				_float4x4 BackStepmat = trigger->mEffectMat;
				_float4x4 BackStepmat2 = XMMatrixScaling(2.f, 2.f, 2.f) * trigger->mEffectMat;
				BackStepmat._42 += 1.f;
				BackStepmat2._42 += 1.f;

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_BACKSTEP,
					initializer_list<any>{BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2,
					BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2,
					BackStepmat, BackStepmat, BackStepmat, BackStepmat, BackStepmat,
					BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2, BackStepmat2,
					BackStepmat2 });
			}
			else if (Player_State == PLAYER_STATE::ATTACK_DASH)
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_DASH,
					initializer_list<any>{mat, mat, mat, mat, mat, 
					mat2, mat2, mat2, mat2, mat2, 
					mat });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::JUMP_ATTACK1||
				Player_State == PLAYER_STATE::JUMP_ATTACK2)
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_SECOND_ELEC,
					initializer_list<any>{mat, mat, mat, mat});

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::JUMP_ATTACK3)
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_SECOND_ELEC,
					initializer_list<any>{mat, mat, mat, mat});

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK_FOLLOW1) /* 던진 후 후속타1 */
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC,
					initializer_list<any>{mat, mat, mat, mat, mat, mat_half, mat_Particle });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK_FOLLOW2) /* 던진 후 후속타1 */
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_DASH,
					initializer_list<any>{mat, mat, mat, mat, mat,
					mat2, mat2, mat2, mat2, mat2,
					mat_Particle });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK_AIRDOWN) /* 공중 아래 */
			{
	
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_DASH,   
					initializer_list<any>{mat, mat, mat, mat, mat,
					mat2, mat2, mat2, mat2, mat2,
					mat });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}
			else if (Player_State == PLAYER_STATE::ATTACK_JUSTDODGE) /* 회피 공격 */
			{
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_ELEC_DASH,
					initializer_list<any>{mat, mat, mat, mat, mat,
					mat2, mat2, mat2, mat2, mat2,
					mat_Particle });

				CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_INITIALIZE_ELEC, RandomMat);
			}

			pPlayer->Set_ElecEffectEnable(false);
		}
		if(rand()%2==0)
			CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("SFX_Kasane_Attack_Electric00.wav"),TEXT("SFX_Kasane_Attack_Electric01.wav"),TEXT("SFX_Kasane_Attack_Electric02.wav") }), g_fPlayerSound);
	}
	else
	{
		if (rand() % 2 == 0)
			CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("SFX_Kasane_Attack_Hit00.wav"),TEXT("SFX_Kasane_Attack_Hit01.wav"),TEXT("SFX_Kasane_Attack_Hit01.wav") }), g_fPlayerSound);
		if (fCrit > 0.5f) //normal
		{
			_float4x4 mat = /*XMMatrixScaling(1.f, 1.f, 1.f) **/ trigger->mEffectMat;
			_float3 vPlayerPos = static_pointer_cast<CTransform>(pPlayer->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
		//	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
		//		initializer_list<any>{mat, _float3(0, 0, 0), _float3(0, 0, 0), _float3(0, 0, 0), _float3(0, 0, 0)});
			VFXMGR->Fetch(VFX::HIT_FLUIDMESH, mat);
		}
		else
		{
			other->stHitDesc.iDamage *= 2;
			_float4x4 mat = XMMatrixScaling(0.5f, 0.5f, 0.5f) * trigger->mEffectMat;
			_float4x4 scaleMat = trigger->mEffectMat;
			_float3 vPlayerPos = static_pointer_cast<CTransform>(pPlayer->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
				initializer_list<any>{_float4x4(), scaleMat, scaleMat, scaleMat, scaleMat});
		}

		//crit
		if (fCrit < pPlayer->Get_PlayerDesc().fCritRate)
			other->stHitDesc.iDamage *= 2;

		if (pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_BACKSTEP)
			pPlayer->Get_PlayerDesc().fKinesisGauge += pPlayer->Get_PlayerDesc().fKinesisBigIncrease;
		else
			pPlayer->Get_PlayerDesc().fKinesisGauge += pPlayer->Get_PlayerDesc().fKinesisNormalIncrease;

		if (pPlayer->Get_PlayerDesc().fKinesisGauge > pPlayer->Get_PlayerDesc().fMaxKinesisGauge)
		{
			pPlayer->Get_PlayerDesc().fKinesisGauge = pPlayer->Get_PlayerDesc().fMaxKinesisGauge;
		}

		pPlayer->Get_PlayerDesc().fDriveGauge += pPlayer->Get_PlayerDesc().fDriveGaugeIncrease;
		if (pPlayer->Get_PlayerDesc().fDriveGauge > 100.f)
		{
			pPlayer->Get_PlayerDesc().fDriveGauge = 100.f;
		}

	
	}

#pragma region DamageFont
	CUI_Manager::Get_Instance()->Set_DamageType(other->stHitDesc.iDamage); // 보통일때 1, 치명타일때 2

	_matrix vDamagePosition = XMMatrixIdentity();
	vDamagePosition.r[3] = _float4(trigger->vOwnerPose.x, trigger->vOwnerPose.y, trigger->vOwnerPose.z, 1.f);

	CUI_Manager::Get_Instance()->Set_DamagePosition(vDamagePosition);

	//if (!(CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)/* && !(CGameInstance::Get_Instance()->Current_Scene() == SCENE::MINDROOM)*/)
	CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT)->Pop();
#pragma endregion
}

void CSimulationEventCallBack::KickMonster(ACTORDESC* trigger, ACTORDESC* other)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	pPlayer->Get_PlayerState()->Set_MonsterKick(true);
	pPlayer->Get_PlayerState()->Set_LagIntervalTime(0.2f);
	pPlayer->Get_Camera()->Set_Shake();
	PLAYER_STATE Player_State = pPlayer->Get_PlayerState()->Get_CurrentState();
	_float3 vPlayerPos = static_pointer_cast<CTransform>(pPlayer->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

	if (pPlayer->Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC)
	{
		if (pPlayer->Get_ElecEffectEnable())
		{
			_float4x4 scaleMat = trigger->mEffectMat;
			_float4x4 mat = XMMatrixScaling(2.f, 2.f, 2.f) * trigger->mEffectMat;
			_float4x4 mat2 = XMMatrixScaling(3.f, 3.f, 3.f) * trigger->mEffectMat;
			_float4x4 mat_half = XMMatrixScaling(0.5f, 0.5f, 0.5f) * trigger->mEffectMat;
			_float4x4 mat_Particle = XMMatrixScaling(5.f, 5.f, 5.f) * trigger->mEffectMat;

			if (Player_State == PLAYER_STATE::ATTACK_UPPER)
			{
				_float3 vTriggerPos = other->vOwnerPose;
				vTriggerPos.y += 5.f;


				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_SECOND_ELEC,
					initializer_list<any>{vTriggerPos, vTriggerPos, vTriggerPos, vTriggerPos});
			}
		}
		pPlayer->Set_ElecEffectEnable(false);

	}
	CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("SFX_Kasane_Attack_Hit00.wav"),TEXT("SFX_Kasane_Attack_Hit01.wav"),TEXT("SFX_Kasane_Attack_Hit02.wav") }), g_fPlayerSound);
#pragma region DamageFont
	CUI_Manager::Get_Instance()->Set_DamageType(other->stHitDesc.iDamage);
	
	_matrix vDamagePosition = XMMatrixIdentity();
	vDamagePosition.r[3] = _float4(trigger->vOwnerPose.x, trigger->vOwnerPose.y, trigger->vOwnerPose.z, 1.f);

	CUI_Manager::Get_Instance()->Set_DamagePosition(vDamagePosition);

	CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT)->Pop();
#pragma endregion
}

void CSimulationEventCallBack::HitPlayer(ACTORDESC* trigger, ACTORDESC* other)
{
}

void CSimulationEventCallBack::HitMonsterByThrow(ACTORDESC* trigger, ACTORDESC* other)
{
	//trigger : monster ,  other : dynamic
	auto pGameInstance = CGameInstance::Get_Instance();
	CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	_float4x4 mat = XMMatrixScaling(2.f,2.f,2.f)*other->mEffectMat;
	_float3 vPlayerPos = static_pointer_cast<CTransform>(pPlayer->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
	_float3 vTriggerPos = trigger->vOwnerPose;
	vTriggerPos.y += 3.f;

	if (pPlayer->Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC)
	{

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::PSY_ELEC,
			initializer_list<any>{ vTriggerPos, vTriggerPos, vTriggerPos, vTriggerPos });
	}
	else
	{
		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
			initializer_list<any>{vTriggerPos, vTriggerPos, vTriggerPos, vTriggerPos, vTriggerPos });
	}

#pragma region DamageFont
	CUI_Manager::Get_Instance()->Set_DamageType(trigger->stHitDesc.iDamage); // 보통일때 1, 치명타일때 2

 	_matrix vDamagePosition = XMMatrixIdentity();
	vDamagePosition.r[3] = _float4(trigger->vOwnerPose.x, trigger->vOwnerPose.y, trigger->vOwnerPose.z, 1.f);

	CUI_Manager::Get_Instance()->Set_DamagePosition(vDamagePosition);

	if(!(CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST))
		CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT)->Pop();

	pPlayer->Get_PlayerDesc().fDriveGauge += pPlayer->Get_PlayerDesc().fDriveGaugeIncrease;
	if (pPlayer->Get_PlayerDesc().fDriveGauge > 100.f)
	{
		pPlayer->Get_PlayerDesc().fDriveGauge = 100.f;
	}
#pragma endregion
}

void CSimulationEventCallBack::HitMonsterBySpecialThrow(ACTORDESC* trigger, ACTORDESC* other)
{
	//trigger : special dynamic ,  other : monster
#pragma region DamageFont
	CUI_Manager::Get_Instance()->Set_DamageType(other->stHitDesc.iDamage); // 보통일때 1, 치명타일때 2


	_matrix vDamagePosition = XMMatrixIdentity();
	vDamagePosition.r[3] = _float4(other->vOwnerPose.x, other->vOwnerPose.y, other->vOwnerPose.z, 1.f);

	CUI_Manager::Get_Instance()->Set_DamagePosition(vDamagePosition);

	if (!(CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST))
		CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT)->Pop();
#pragma endregion
}

void CSimulationEventCallBack::TeammateAttackMonster(ACTORDESC* trigger, ACTORDESC* other)
{
	_float4x4 mat = XMMatrixScaling(4.f, 4.f, 4.f) * trigger->mEffectMat;
//	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
//		initializer_list<any>{mat, _float3(0, 0, 0), _float3(0, 0, 0), _float3(0, 0, 0), _float3(0, 0, 0)});
	VFXMGR->Fetch(VFX::HIT_FLUIDMESH, mat);
}
