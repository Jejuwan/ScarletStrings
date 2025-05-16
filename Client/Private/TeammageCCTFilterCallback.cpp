#include "ClientPCH.h"
#include "TeammageCCTFilterCallback.h"

bool CTeammageCCTFilterCallback::filter(const PxController& a, const PxController& b)
{
	ACTORDESC* pDesc1 = (ACTORDESC*)a.getActor()->userData;
	ACTORDESC* pDesc2 = (ACTORDESC*)b.getActor()->userData;

	//if (pDesc1)
	//{
	//	if (pDesc1->strTag == ACTORTAG::TEAMMATE && pDesc2->strTag == ACTORTAG::TEAMMATE
	//		&& (pDesc1->eCollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN
	//		|| pDesc2->eCollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN))
	//	{
	//		return false;
	//	}
	//}

	return true;
}
