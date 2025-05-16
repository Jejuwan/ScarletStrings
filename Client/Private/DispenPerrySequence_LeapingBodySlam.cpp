#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"
#include "Animation.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Map_Special_Dynamic.h"
#include "InteractiveObjectFinder.h"
#include "Psychokinesis.h"
#include "Effect_Manager.h"
#include "Bone.h"
HRESULT CDispenPerrySequence_LeapingBodySlam::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "LeapingBodySlam";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySequence_LeapingBodySlam::Initialize", "failed to Get_System: Target:Transform");
	}

	Add_Child(CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::ATK_LEAP_START), 3.f, false, g_fDefaultInterpolationDuration, false));
//	Add_Child(CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
//		IDX(ANIMATION::DISPEN_PERRY::ATK_LEAP_LAND), 3.f, false, g_fDefaultInterpolationDuration, false));
//	Add_Child(CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
//		IDX(ANIMATION::DISPEN_PERRY::ATK_LEAP_LANDLOOP), 3.f, false, g_fDefaultInterpolationDuration, false));
	Add_Child(CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 10.f,
		CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::ATK_LEAP_JITABATA), 3.f, false, g_fDefaultInterpolationDuration, true)));

	return S_OK;
}

void CDispenPerrySequence_LeapingBodySlam::Activate()
{
	__super::Activate();

	m_bIsJumping = false;
	m_pTransform->LookAt_Interpolation(m_pTargetTransform, true, false, 2.f, 1.f);
	m_pAnimator->Reset_Animation(ANIMATION::DISPEN_PERRY::ATK_LEAP_START);

	m_fLockOnAccTime = 0.f;
}

STATUS CDispenPerrySequence_LeapingBodySlam::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Physics_PhysX(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);
	//cout << "BADUNG" << endl;
	if (0 == m_iChildIndex)
	{
		if (m_pOwner.lock()->Is_PxColDown())
		{
			if (m_bIsJumping)
			{
				(*m_itCurrentChild)->Terminate();
				++m_itCurrentChild;
				++m_iChildIndex;
			}
		}
		else
		{
			if (!m_bIsJumping)
			{
				m_pPhysics->Force(TRANSFORM::LOOK, ATK_LEAP_RANGE_FORCE);
				m_bIsJumping = true;
			}
		}
	}
	else if (1 == m_iChildIndex)
	{
		m_fLockOnAccTime += _fTimeDelta;
		if (m_fLockOnAccTime >= m_fLockOnDelay)
		{
			CGameInstance::Get_Instance()->Find_Layer(SCENE::MIZUHA, LAYER_MAP_SPECIALDYNAMIC)->Iterate_Objects(
				[&](shared_ptr<CGameObject> pObject)->_bool
				{
					if (SPECIAL_THROW::HEAVY_EQUIPMENT == static_pointer_cast<CMap_Special_Dynamic>(pObject)->Get_Type())
					{
						CGameInstance::Get_Instance()->Set_SpecialOnly(true);
						dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOn(true);
						dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_Target(pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM), -XM_PIDIV4 * 0.6f);

						return false;
					}
					return true;
				}
			);

			m_fLockOnAccTime = 0.f;
		}
	}

	shared_ptr<CMap_Special_Dynamic> pSpecial = static_pointer_cast<CMap_Special_Dynamic>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Get_RecentThrowdObject());
	if (pSpecial && pSpecial->Get_Type() == SPECIAL_THROW::HEAVY_EQUIPMENT)
	{
		m_eStatus = STATUS::FAILURE;
	}


	const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("LeftLeg")->Get_CombinedTransformationPointer();
	const _float4x4* mat2 = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("RightLeg")->Get_CombinedTransformationPointer();

	_float4x4 LeftLegMat = XMLoadFloat4x4(mat) * XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * m_pTransform->Get_Matrix();
	_float4x4 LeftLegLook = XMLoadFloat4x4(mat) * XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * m_pTransform->Get_Matrix();
	_float4x4 RightLegMat = XMLoadFloat4x4(mat2) * XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * m_pTransform->Get_Matrix();
	_float4x4 RightLegLook = XMLoadFloat4x4(mat2) * XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * m_pTransform->Get_Matrix();

	_float3 vDispenRight = m_pTransform->Get_State(TRANSFORM::RIGHT) * 5.f;
	_float3 vDispenLook = m_pTransform->Get_State(TRANSFORM::LOOK) * 5.f;

	LeftLegMat._41 -= vDispenRight.x;
	LeftLegMat._42 += 1.f;
	LeftLegMat._43 -= vDispenRight.z;

	LeftLegLook._41 += vDispenLook.x - vDispenRight.x;
	LeftLegLook._42 += vDispenLook.y;
	LeftLegLook._43 += vDispenLook.z - vDispenRight.z;

	RightLegMat._41 += vDispenRight.x;
	RightLegMat._42 += 1.f;
	RightLegMat._43 += vDispenRight.z;

	RightLegLook._41 += vDispenLook.x + vDispenRight.x;
	RightLegLook._42 += vDispenLook.y;
	RightLegLook._43 += vDispenLook.z + vDispenRight.z;

	m_fEffect_Timer += _fTimeDelta;
	m_fEffect_Timer2 += _fTimeDelta;

	if (m_fEffect_Timer >= 0.5f)
	{
		CEffect_Manager::Get_Instance()->Fetch(VFX::DISPEN_SMOKE, LeftLegMat);
		CEffect_Manager::Get_Instance()->Fetch(VFX::DISPEN_SMOKE, LeftLegLook);
		m_fEffect_Timer = 0.f;
	}

	if (m_fEffect_Timer2 >= 0.7f)
	{
		CEffect_Manager::Get_Instance()->Fetch(VFX::DISPEN_SMOKE, RightLegMat);
		CEffect_Manager::Get_Instance()->Fetch(VFX::DISPEN_SMOKE, RightLegLook);
		m_fEffect_Timer2 = 0.f;
	}

	return Return_Invoke();
}

void CDispenPerrySequence_LeapingBodySlam::Terminate()
{
	m_bIsJumping = false;

	__super::Terminate();

	dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOn(false);
	dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_Target();
	dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOn(true);
	dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOnTransform(m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM));

	static _float fTime = 0.f;;
	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[&](_float fTimeDelta)->_bool
		{
 			fTime += fTimeDelta;
			if (fTime > 0.2f)
			{
				fTime = 0.f;
				dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOn(false);
				dynamic_pointer_cast<CPlayer>(Function::Find_Player())->Get_Camera()->Set_LockOnTransform(nullptr);
				return false;
			}

			return true;
		}
	);

	CGameInstance::Get_Instance()->Set_SpecialOnly(false);
}

#if ACTIVATE_IMGUI
void CDispenPerrySequence_LeapingBodySlam::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerrySequence_LeapingBodySlam> CDispenPerrySequence_LeapingBodySlam::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySequence_LeapingBodySlam> pInstance = make_private_shared(CDispenPerrySequence_LeapingBodySlam);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySequence_LeapingBodySlam::Create", "Failed to Initialize");
	}

	return pInstance;
}
