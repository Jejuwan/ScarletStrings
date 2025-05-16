#include "ClientPCH.h"
#include "PlayerControllerHitReport.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerState.h"

void CPlayerControllerHitReport::onShapeHit(const PxControllerShapeHit& hit)
{
	ACTORDESC* pActorDesc = (ACTORDESC*)hit.actor->userData;
	if (nullptr==pActorDesc)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
		if (pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP &&
			pPlayer->Get_PlayerState()->Get_InAir())
		{
			pPlayer->Get_PlayerState()->Set_ForceInAir(true);
		}
	}

}

void CPlayerControllerHitReport::onControllerHit(const PxControllersHit& hit)
{
	ACTORDESC* pDesc = (ACTORDESC*)hit.other->getActor()->userData;
	if (nullptr != pDesc)
	{
		if (pDesc->strTag == ACTORTAG::MONSTER)
		{
			
		}
	}
}

void CPlayerControllerHitReport::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

