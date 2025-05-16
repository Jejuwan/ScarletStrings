#pragma once
#include "Client_Define.h"
#include "physx/PxPhysicsAPI.h"
#include "System.h"

using namespace physx;

class CSimulationEventCallBack :
    public PxSimulationEventCallback,public std::enable_shared_from_this<CSimulationEventCallBack>
{
public:
	CSimulationEventCallBack();
	~CSimulationEventCallBack() = default;

	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count);
	virtual void onWake(PxActor** actors, PxU32 count) ;
	virtual void onSleep(PxActor** actors, PxU32 count);
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) ;

private:
	void HitMonster(ACTORDESC* trigger, ACTORDESC* other);
	void KickMonster(ACTORDESC* trigger, ACTORDESC* other);
	void HitPlayer(ACTORDESC* trigger, ACTORDESC* other);
	void HitMonsterByThrow(ACTORDESC* trigger, ACTORDESC* other);
	void HitMonsterBySpecialThrow(ACTORDESC* trigger, ACTORDESC* other);

	void TeammateAttackMonster(ACTORDESC* trigger, ACTORDESC* other);

};

