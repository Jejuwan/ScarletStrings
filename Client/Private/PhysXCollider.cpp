#include "ClientPCH.h"
#include "PhysXCollider.h"
#include "GameInstance.h"
#include "PhysXEngine.h"

CPhysXCollider::CPhysXCollider()
{
}

void CPhysXCollider::Initialize(PHYSXCOLLIDERDESC desc,ACTORDESC actorDesc)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	
	switch (desc.eType)
	{
	case TYPE::BOX:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(
			PxBoxGeometry((PxReal)desc.vHalfExtents.x, (PxReal)desc.vHalfExtents.y, (PxReal)desc.vHalfExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		break;
	case TYPE::SPHERE:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(
			PxSphereGeometry((PxReal)desc.fRadius), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		break;
	case TYPE::CAPSULE:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(
			PxCapsuleGeometry((PxReal)desc.fCapsuleRadius, (PxReal)desc.fCapsuleHeight), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		break;
	}
	if (desc.bSetLocalPose)
	{
		PxTransform p;
		p.p = PxVec3(desc.vLocalPose.x, desc.vLocalPose.y, desc.vLocalPose.z);
		m_pShape->setLocalPose(p);
	}
	m_pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	PxTransform localTm(0, 0, 0);
	m_pActor = PxCreateStatic(*pGameInstance->Get_PhysXEngine()->GetPhysics(), localTm, *m_pShape);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	m_ActorDesc = actorDesc;
	m_pActor->userData = &m_ActorDesc;
}

void CPhysXCollider::SetActive(_bool b)
{
	ACTORDESC* pActorDesc = (ACTORDESC*)(m_pActor->userData);
	pActorDesc->bActive = b;
	m_pActor->userData = pActorDesc;
}

_bool CPhysXCollider::GetActive()
{
	ACTORDESC* pActorDesc = (ACTORDESC*)(m_pActor->userData);
	if (nullptr == pActorDesc)
		return false;

	return pActorDesc->bActive;
}
