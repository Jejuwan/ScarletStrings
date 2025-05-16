#include "ClientPCH.h"
#include "Client_Define.h"
#include "MonsterControllerFilterCallBack.h"

PxQueryHitType::Enum CMonsterControllerFilterCallBack::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
		return PxQueryHitType::eNONE;

	ACTORDESC* desc = (ACTORDESC*)actor->userData;
	if (nullptr != desc)
	{
		if (desc->strTag == ACTORTAG::INTERACTIVE_SPECIAL)
			return PxQueryHitType::eNONE;
		if (desc->bCapture)
			return PxQueryHitType::eNONE;
		if (desc->bThrowHit)
			return PxQueryHitType::eNONE;
	}


	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum CMonsterControllerFilterCallBack::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	return PxQueryHitType::eNONE;
}
