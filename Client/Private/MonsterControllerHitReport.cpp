#include "ClientPCH.h"
#include "MonsterControllerHitReport.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerState.h"

void CMonsterControllerHitReport::onShapeHit(const PxControllerShapeHit& hit)
{
	ACTORDESC* pActorDesc = (ACTORDESC*)hit.actor->userData;
	if (nullptr != pActorDesc)
	{
		if (pActorDesc->strTag == ACTORTAG::INTERACTIVE)
		{
			int a = 5;
		}
	}

}

void CMonsterControllerHitReport::onControllerHit(const PxControllersHit& hit)
{
}

void CMonsterControllerHitReport::onObstacleHit(const PxControllerObstacleHit& hit)
{
}
