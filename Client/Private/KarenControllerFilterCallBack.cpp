#include "ClientPCH.h"
#include "Client_Define.h"
#include "KarenControllerFilterCallBack.h"

#include "GameInstance.h"

PxQueryHitType::Enum CKarenControllerFilterCallBack::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	ACTORDESC* desc = (ACTORDESC*)actor->userData;
	if (nullptr != desc)
	{
		if (desc->strTag == ACTORTAG::FALLEN_OBJECT)
			return PxQueryHitType::eNONE;
		//if (desc->strTag == ACTORTAG::PLAYER)
		//	return PxQueryHitType::eNONE;
	}

	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum CKarenControllerFilterCallBack::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	return PxQueryHitType::eNONE;
}
