#include "ClientPCH.h"
#include "Client_Define.h"
#include "PlayerControllerFilterCallBack.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerState.h"

PxQueryHitType::Enum CPlayerControllerFilterCallBack::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
		return PxQueryHitType::eNONE;

	ACTORDESC* desc = (ACTORDESC*)actor->userData;
	if(nullptr!=desc)
	{
		//if (desc->strTag == ACTORTAG::INTERACTIVE/* && (desc->bThrow || desc->bCapture)*/)
		//{
		//	return PxQueryHitType::eNONE;
		//}
		if (desc->strTag == ACTORTAG::FALLEN_OBJECT/* && (desc->bThrow || desc->bCapture)*/)
		{
			return PxQueryHitType::eNONE;
		}

		if (desc->strTag == ACTORTAG::MONSTER)
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
			if (pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_UPPER ||
				pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP || 
				pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK1 ||
				pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK2 ||
				pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK3)
			{
				return PxQueryHitType::eNONE;
			}
		}

		if (desc->strTag == ACTORTAG::FALLEN_OBJECT)
		{
			//CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
			//if (pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK1 ||
			//	pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK2 ||
			//	pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK3 ||
			//	pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK4 ||
			//	pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_BACKSTEP ||
			//	pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_DASH)
			{
				return PxQueryHitType::eNONE;
			}
		}
	}


	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum CPlayerControllerFilterCallBack::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	return PxQueryHitType::eNONE;
}
