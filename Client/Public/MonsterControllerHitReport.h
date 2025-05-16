#pragma once
#include "physx/PxPhysicsAPI.h"
class CMonsterControllerHitReport :
	public PxUserControllerHitReport
{
	virtual void onShapeHit(const PxControllerShapeHit& hit);
	virtual void onControllerHit(const PxControllersHit& hit);
	virtual void onObstacleHit(const PxControllerObstacleHit& hit);
};

