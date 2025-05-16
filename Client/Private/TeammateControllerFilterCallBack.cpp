#include "ClientPCH.h"
#include "Client_Define.h"
#include "TeammateControllerFilterCallBack.h"

#include "GameInstance.h"

PxQueryHitType::Enum CTeammateControllerFilterCallBack::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	if (false == m_bEnable)
	{
		return PxQueryHitType::eNONE;
	}

	ACTORDESC* desc = (ACTORDESC*)actor->userData;
	if (nullptr != desc)
	{
		if (desc->strTag == ACTORTAG::INTERACTIVE_SPECIAL)
			return PxQueryHitType::eNONE;
	}

	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum CTeammateControllerFilterCallBack::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
{
	return PxQueryHitType::eNONE;
}
