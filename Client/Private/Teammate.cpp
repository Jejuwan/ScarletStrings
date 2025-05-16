#include "ClientPCH.h"
#include "Teammate.h"

#include "GameInstance.h"
#include "PhysXEngine.h"

#include "TeammateControllerFilterCallBack.h"
#include "TeammageCCTFilterCallback.h"

#include "Player.h"

CTeammate::CTeammate(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CTeammate::CTeammate(const CTeammate& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTeammate::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTeammate::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Initialize", "Failed to CGameObject::Initialize");
	}

	// Init Conteroller
	{
		m_ControllerFilterCallBack = make_shared<CTeammateControllerFilterCallBack>();
		m_CCTFilterCallBack = make_shared<CTeammageCCTFilterCallback>();

		m_ControllerFilters.mFilterCallback = m_ControllerFilterCallBack.get();
		m_ControllerFilters.mCCTFilterCallback = m_CCTFilterCallBack.get();
		m_ControllerFilters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
		m_ControllerFilters.mFilterData = NULL;
	}

	return S_OK;
}

void CTeammate::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());
	PLAYER_SAS eCurrentSAS = pPlayer->Get_PlayerDesc().eSAS;
	if (eCurrentSAS == PLAYER_SAS::STEALTH)
	{
		m_bTransparency = true;
	}

	if (m_bTransparency && eCurrentSAS != PLAYER_SAS::STEALTH)
	{
		m_bTransparency = false;
	}
}

void CTeammate::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CTeammate::Render()
{
	return S_OK;
}

HRESULT CTeammate::Render_ShadowDepth(shared_ptr<class CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CTeammate::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

void CTeammate::Set_Animation(_uint iAnimationIndex, _float fPlaySpeed, _bool bReverse, _float fInterpolationDuration, _bool bLoop)
{
	m_pModel->Set_Animation(iAnimationIndex, fPlaySpeed, bReverse, fInterpolationDuration, bLoop);
	//m_pModel->Set_NextAnimIndex
}

_uint CTeammate::Get_CurrentAnimation()
{
	return m_pModel->Get_CurrentAnimIndex();
}

void CTeammate::Enable_Controller(_bool bEnable)
{
	dynamic_pointer_cast<CTeammateControllerFilterCallBack>(m_ControllerFilterCallBack)->Set_Enable(bEnable);
}

void CTeammate::Active_Weapon(_bool bActive)
{
}

_bool CTeammate::Detect_Obstacle(PxRaycastBuffer& hit, _float fRayDistance)
{
	auto pScene = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene();
	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float3 vLook = XMVector3Normalize(m_pTransform->Get_State(TRANSFORM::LOOK));

	PxVec3 origin = PxVec3(vPosition.x, vPosition.y + 0.4f, vPosition.z);
	PxVec3 unitDir = PxVec3(vLook.x, vLook.y, vLook.z);
	PxReal maxDistance = fRayDistance;

	PxQueryFilterData FilterData;
	FilterData.flags = PxQueryFlag::Enum::eSTATIC;

	return pScene->raycast(origin, unitDir, maxDistance, hit, PxHitFlag::Enum::eDEFAULT, FilterData);
}

_bool CTeammate::Detect_Ground(_float4& vGroundPosition)
{
	auto pScene = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene();
	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);

	const PxU32 iBufferSize = 16;
	PxRaycastHit thitBuffer[iBufferSize];
	PxRaycastBuffer RayCallback(thitBuffer, iBufferSize);

	PxVec3 origin = PxVec3(vPosition.x, vPosition.y + 0.4f, vPosition.z);
	PxVec3 unitDir = PxVec3(0.f, -1.f, 0.f);
	PxReal maxDistance = 1000.f;

	_bool bHit = pScene->raycast(origin, unitDir, maxDistance, RayCallback/*, PxHitFlag::Enum::eDEFAULT, FilterData*/);
	
	_float fMinDist = FLT_MAX;
	_bool bHitGround = false;
	PxVec3 vNearlestGround;
	if (bHit)
	{
		PxU32 nbHits = RayCallback.nbTouches;
		for (PxU32 i = 0; i < nbHits; ++i)
		{
			const PxRaycastHit& touch = RayCallback.touches[i];
			ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
			if (nullptr == pActorDesc)
			{
				bHitGround = true;

				PxF32 fDist = touch.distance;
				if (fMinDist > fDist)
				{
					fMinDist = fDist;
					vNearlestGround = touch.position;
				}
			}
		}
	}

	//if (bHitGround)
	//{
		vGroundPosition = _float4(vNearlestGround.x, vNearlestGround.y, vNearlestGround.z, 1.f);
	//}

	return bHitGround;
}

void CTeammate::Init_RootMotion()
{
	m_vPreRootPos = m_pModel->Get_RootPos();
}

void CTeammate::Update_Motion(_float fTimeDelta)
{
	m_vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);

	if (m_bGravityEnable)
	{
		m_fGravityVelocity += fTimeDelta;
		m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_fGravityVelocity * m_fGravityVelocity * 10.f * fTimeDelta);
	}
}

void CTeammate::RootMotion(_float fTimeDelta, _bool bCheckGround)
{
	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_vector MoveDir = XMVector3TransformNormal((m_pModel->Get_RootPos() - m_vPreRootPos), m);

	vUpdatePos += MoveDir;

	vUpdatePos.w = 1.f;

	m_pTransform->Set_State(TRANSFORM::POSITION, vUpdatePos);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vUpdatePos - m_vPrePos);
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_ControllerFilters);
	m_ActorDesc.eCollisionFlag = m_CollisionFlag;

	if (bCheckGround && m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fGravityVelocity = 0.f;
	}

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));

	m_vPreRootPos = m_pModel->Get_RootPos();
}

void CTeammate::Motion(_float fTimeDelta)
{
	_vector vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	_float3 vDisp;
	XMStoreFloat3(&vDisp, vPos - m_vPrePos);
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_ControllerFilters);
	if (m_CollisionFlag == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_fGravityVelocity = 0.f;
	}

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + PxExtendedVec3(0, 0.f, 0);

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));
}

void CTeammate::Reset_Motion()
{
	m_vResetPos = m_pModel->Get_RootAddPos();
	m_vResetPos.w = 1.f;

	XMStoreFloat4(&m_vPreRootPos, m_vResetPos);
}
