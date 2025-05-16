#pragma once
#include "physx/PxQueryFiltering.h"
class CPlayerControllerFilterCallBack :
	public PxQueryFilterCallback
{
public:
	CPlayerControllerFilterCallBack() {};
	virtual ~CPlayerControllerFilterCallBack() {}
	/**
	\brief This filter callback is executed before the exact intersection test if PxQueryFlag::ePREFILTER flag was set.

	\param[in] filterData		custom filter data specified as the query's filterData.data parameter.
	\param[in] shape			A shape that has not yet passed the exact intersection test.
	\param[in] actor			The shape's actor.
	\param[in,out] queryFlags	scene query flags from the query's function call (only flags from PxHitFlag::eMODIFIABLE_FLAGS bitmask can be modified)
	\return the updated type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags);

	/**
	\brief This filter callback is executed if the exact intersection test returned true and PxQueryFlag::ePOSTFILTER flag was set.

	\param[in] filterData	custom filter data of the query
	\param[in] hit			Scene query hit information. faceIndex member is not valid for overlap queries. For sweep and raycast queries the hit information can be cast to #PxSweepHit and #PxRaycastHit respectively.
	\param[in] shape		Hit shape
	\param[in] actor		Hit actor
	\return the updated hit type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor);

	/**
	\brief virtual destructor
	*/
};

