#pragma once

#include "Client_Define.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Client)

class CKarenCCTFilterCallback : public PxControllerFilterCallback
{
public:
	CKarenCCTFilterCallback() {};
	virtual ~CKarenCCTFilterCallback() {}

	/**
	\brief Filtering method for CCT-vs-CCT.

	\param[in] a	First CCT
	\param[in] b	Second CCT
	\return true to keep the pair, false to filter it out
	*/
	virtual bool filter(const PxController& a, const PxController& b);
};

END