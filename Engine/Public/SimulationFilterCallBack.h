//#pragma once
//#include "physx/PxPhysicsAPI.h"
//
//using namespace physx;
//
//class CSimulationFilterCallBack :
//    public PxSimulationFilterCallback
//{
//public:
//	CSimulationFilterCallBack();
//	~CSimulationFilterCallBack() = default;
//	virtual		PxFilterFlags	pairFound(PxU64 /*pairID*/,
//		PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/, const PxActor* a0, const PxShape* s0,
//		PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/, const PxActor* a1, const PxShape* s1,
//		PxPairFlags& pairFlags)	PX_OVERRIDE;
//
//	virtual		void	pairLost(PxU64 /*pairID*/,
//		PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/,
//		PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/,
//		bool /*objectRemoved*/)	PX_OVERRIDE;
//
//	virtual		bool	statusChange(PxU64& /*pairID*/, PxPairFlags& /*pairFlags*/, PxFilterFlags& /*filterFlags*/)	PX_OVERRIDE;
//};
//
