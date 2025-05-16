#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

CDispenPerryLeaf_Walk::CDispenPerryLeaf_Walk(DIRECTION _eDirection)
	: m_eDirection(_eDirection)
{
}

HRESULT CDispenPerryLeaf_Walk::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Walk";
#endif

	switch (m_eDirection)
	{
	case DIRECTION::LEFT:
		m_eAnimation = ANIMATION::DISPEN_PERRY::WALK_L_ROOP;
		break;
	case DIRECTION::RIGHT:
		m_eAnimation = ANIMATION::DISPEN_PERRY::WALK_R_ROOP;
		break;

	default:
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Invalid Direction");
	}

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_Anything: Owner");
	}
	
	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_EntityDesc: Owner:EntityDesc");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Target:Transform");
	}

	return S_OK;
}

void CDispenPerryLeaf_Walk::Activate()
{
	__super::Activate();

//	m_eDirection = static_cast<DIRECTION>(Function::Probability({ 0.f, 1.f, 0.f, 1.f }));
//
//	switch (m_eDirection)
//	{
//	case LEFT:
//		m_eStart	= ANIMATION::DISPEN_PERRY::WALK_L_START;
//		m_eRoop		= ANIMATION::DISPEN_PERRY::WALK_L_ROOP;
//		break;
//	case RIGHT:
//		m_eStart	= ANIMATION::DISPEN_PERRY::WALK_R_START;
//		m_eRoop		= ANIMATION::DISPEN_PERRY::WALK_R_ROOP;
//		break;
//	}

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(m_eAnimation, 3.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_Walk::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	m_pTransform->LookAt_Interpolation(m_pTargetTransform, true, false, 32.f, 1.f);

	if (m_pAnimator->Is_Playing(m_eAnimation))
	{
		if (m_pAnimator->Is_Finished(m_eAnimation))
		{
			m_eStatus = STATUS::SUCCESS;
		}
		else
		{
			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke(); 
}

void CDispenPerryLeaf_Walk::Terminate()
{
	m_pOwner.lock()->MotionReset();

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_Walk::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerryLeaf_Walk> CDispenPerryLeaf_Walk::Create(shared_ptr<CBlackBoard> _pBlackBoard, DIRECTION _eDirection)
{
	shared_ptr<CDispenPerryLeaf_Walk> pInstance = make_private_shared(CDispenPerryLeaf_Walk, _eDirection);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_Walk::Create", "Failed to Initialize");
	}

	return pInstance;
}
