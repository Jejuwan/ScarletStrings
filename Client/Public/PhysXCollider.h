#pragma once
#include "physx/PxPhysicsAPI.h"
#include "Client_Define.h"

using namespace physx;

BEGIN(Client)
class CPhysXCollider
{
public:
	enum class TYPE { BOX, SPHERE, CAPSULE };
	typedef struct tagPhysXColliderDesc
	{
		TYPE eType;
		float3  vHalfExtents;
		_bool bSetLocalPose = false;
		_float3 vLocalPose;
		float fRadius;
		float fCapsuleRadius, fCapsuleHeight;
	}PHYSXCOLLIDERDESC;
public:
	CPhysXCollider();
	~CPhysXCollider() DEFAULT;

private:
	PxShape* m_pShape = { nullptr };
	PxRigidStatic* m_pActor = { nullptr };
	
	_bool	m_bActive = { false };
	//collider is currently colliding
	_bool	m_bHit = { false };
	ACTORDESC m_ActorDesc;
public:
	void Initialize(PHYSXCOLLIDERDESC desc, ACTORDESC actorDesc);
	PxShape* GetShape() { return m_pShape; }
	PxRigidStatic* GetActor() { return m_pActor; }
	void SetActive(_bool b);
	_bool GetActive();
	void SetHit(_bool b) { m_bHit = true; }
	_bool GetHit() { return m_bHit; }
	ACTORDESC& GetActorDesc() { return m_ActorDesc; }
	void UpdateActorDesc() { m_pActor->userData = &m_ActorDesc; }

};
END
