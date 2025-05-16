#include "EnginePCH.h"
//#include "SimulationFilterCallBack.h"
//#include "Engine_Define.h"
//
//CSimulationFilterCallBack::CSimulationFilterCallBack()
//{
//}
//
//PxFilterFlags	pairFound(PxU64 /*pairID*/,
//	PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/, const PxActor* a0, const PxShape* s0,
//	PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/, const PxActor* a1, const PxShape* s1,
//	PxPairFlags& pairFlags)
//{
//	ACTORDESC* desc1 = (ACTORDESC*)(a0->userData);
//	ACTORDESC* desc2 = (ACTORDESC*)(a1->userData);
//
//	if(desc1->strTag == ACTORTAG::INTERACTIVE &&
//		desc2->strTag == ACTORTAG::PLAYER)
//	{
//		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
//	}
//	else
//		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//
//	return PxFilterFlags();
//}
//
//
//void	pairLost(PxU64 /*pairID*/,
//	PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/,
//	PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/,
//	bool /*objectRemoved*/)
//{
//
//}
//
//bool	statusChange(PxU64& /*pairID*/, PxPairFlags& /*pairFlags*/, PxFilterFlags& /*filterFlags*/)
//{
//	return false;
//}
//
