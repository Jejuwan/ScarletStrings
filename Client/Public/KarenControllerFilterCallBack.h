#pragma once

#include "physx/PxQueryFiltering.h"

class CKarenControllerFilterCallBack : public PxQueryFilterCallback
{
public:
	CKarenControllerFilterCallBack() {};
	virtual ~CKarenControllerFilterCallBack() {}

public:
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags);
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor);

public:
	void				Set_Enable(_bool bEnable) { m_bEnable = bEnable; }

private:
	_bool				m_bEnable = { true };
};

