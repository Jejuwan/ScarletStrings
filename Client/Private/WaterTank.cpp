#include "ClientPCH.h"
#include "WaterTank.h"
#include "Monster_DispenPerry.h"
#include "GameInstance.h"
#include "ObjectPool.h"
#include "Bone.h"
#include "PhysXEngine.h"

CWaterTank::CWaterTank(_float _fMaxHP, _float _fHeal)
	: m_fHealSpeed(_fHeal)
{
	m_tEntityDesc.fMaxHP = _fMaxHP;
}

HRESULT CWaterTank::Initialize(shared_ptr<CMonster_DispenPerry> _pOwner)
{
	m_pWaterPool			= CObjectPool::Create(CGameInstance::Get_Instance()->Current_Scene(), 10u,
		CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP));

	m_pOwner				= _pOwner;
	m_tEntityDesc.fHP		= m_tEntityDesc.fMaxHP;

	m_mPivot				= _pOwner->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();
	m_mBone					= _pOwner->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Water1")->Get_CombinedTransformationPointer();
	m_pParentTransform		= _pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	m_pTargetTransform		= Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pDumpTransform		= CTransform::Create();

	m_pairRotationRadians	= make_pair(XMMatrixRotationY(XMConvertToRadians(-30.f)), XMMatrixRotationY(XMConvertToRadians(+30.f)));

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,0.f,0.f };
	cDesc.height = 0.00001f;
	cDesc.radius = 3.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	ACTORDESC ActorDesc;
	m_pController = pGameInstance->CreateController(&cDesc);
	ActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	ActorDesc.bThrowHit = false;
	m_pController->getActor()->userData = &ActorDesc;
	m_pActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	return S_OK;
}

void CWaterTank::Tick(_float fTimeDelta)
{
//	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_PERIOD))
//	{
//		Fire_WaterDrop(TRUE);
//	}

	_matrix BoneMatrix = XMLoadFloat4x4(m_mBone);
	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* m_mPivot
		* m_pParentTransform->Get_Matrix();

	if (m_pController)
	{
		PxTransform p;
		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pController->getActor()->setGlobalPose(p);
	}

	m_pWaterPool->Tick(fTimeDelta);

	if (m_bBroken)
	{
		if (m_tEntityDesc.fHP < m_tEntityDesc.fMaxHP)
		{
			m_tEntityDesc.fHP = std::clamp(m_tEntityDesc.fHP + m_fHealSpeed * fTimeDelta, 0.f, m_tEntityDesc.fMaxHP);
		}
		else
		{
			m_bBroken = false;
		}
	}
}

void CWaterTank::Late_Tick(_float fTimeDelta)
{
	m_pWaterPool->Late_Tick(fTimeDelta);
}

void CWaterTank::Set_DamageLevel(_float _fHp, DP_STATE _eState)
{
	m_mapDamageLevel.emplace(_fHp, _eState);
	m_itDamageLevel = m_mapDamageLevel.end();
}

DP_STATE CWaterTank::Damage(_float _fAtk)
{
	m_tEntityDesc.fHP = std::clamp(m_tEntityDesc.fHP - _fAtk, 0.f, m_tEntityDesc.fHP);

	if (m_tEntityDesc.fHP <= 0.f)
	{
		m_bBroken = true;
	}

	auto iter = m_mapDamageLevel.lower_bound(m_tEntityDesc.fHP);
	if (iter != m_itDamageLevel)
	{
		m_itDamageLevel = iter;
		m_eDamageState = m_itDamageLevel->second;
	}
	else
	{
		m_eDamageState = DP_STATE::NOEVENT;
	}

	m_pOwner.lock()->Set_State(m_eDamageState);

	return m_eDamageState;
}

void CWaterTank::Fire_WaterDrop(_bool _bMultiWaterShot)
{
/*
	m_pDumpTransform->Set_State(TRANSFORM::POSITION, (XMLoadFloat4x4(m_mBone) * m_mPivot * m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix()).r[3]);
//	m_pDumpTransform->Set_Matrix(m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix());
//	m_pDumpTransform->Translate(_float3(0.f, 2.f, 0.f));
//	m_pDumpTransform->Translate(m_pDumpTransform->Get_State(TRANSFORM::LOOK) * 0.2f);
	m_pDumpTransform->Set_Scale(_float3(0.5f, 0.5f, 0.75f));
	m_pDumpTransform->LookAt(m_pTargetTransform, true, false);
	m_mShellRadians = XMMatrixRotationAxis(m_pDumpTransform->Get_State(TRANSFORM::RIGHT), XMConvertToRadians(15.f));
	_float4x4 mWorld = m_mShellRadians * m_pDumpTransform->Get_Matrix();
	m_pWaterPool->Pop(m_pDumpTransform->Get_Matrix());
	if (_bMultiWaterShot)
	{
		_float4x4 mLeft		= m_pairRotationRadians.first * mWorld;
		_float4x4 mRight	= m_pairRotationRadians.second * mWorld;
		m_pWaterPool->Pop(mLeft);
		m_pWaterPool->Pop(mRight);
	}
*/
	m_pDumpTransform->Set_State(TRANSFORM::POSITION, (XMLoadFloat4x4(m_mBone) * m_mPivot * m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix()).r[3]);
	m_pDumpTransform->Set_Scale(_float3(0.75f, 0.75f, 1.f));
	m_pDumpTransform->LookAt(m_pTargetTransform, false, false);

	_float3 vPosition	= m_pDumpTransform->Get_State(TRANSFORM::POSITION);
	_float3 vRight		= m_pDumpTransform->Get_State(TRANSFORM::RIGHT);
	_vector vQuaternion	= QuaternionBetweenAxis(vRight, _float3(vRight.x, 0.f, vRight.z));

	m_pDumpTransform->Multiply(XMMatrixRotationQuaternion(vQuaternion));
	m_pDumpTransform->Set_State(TRANSFORM::POSITION, vPosition);

	_float4x4 m = m_pDumpTransform->Get_Matrix();
	m_pWaterPool->Pop(m);
	if (_bMultiWaterShot)
	{
		m_pDumpTransform->Rotate(TRANSFORM::UP, XM_PIDIV4);
		m_pWaterPool->Pop(m_pDumpTransform->Get_Matrix());
		m_pDumpTransform->Set_Matrix(m);
		m_pDumpTransform->Rotate(TRANSFORM::UP, -XM_PIDIV4);
		m_pWaterPool->Pop(m_pDumpTransform->Get_Matrix());
	}
}

shared_ptr<CWaterTank> CWaterTank::Create(shared_ptr<CMonster_DispenPerry> _pOwner, _float _fMaxHP, _float _fHeal)
{
	shared_ptr<CWaterTank> pInstance = make_private_shared(CWaterTank, _fMaxHP, _fHeal);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CWaterTank::Create", "Failed to Initialize");
	}

	return pInstance;
}
