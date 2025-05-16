#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerrySelector_LeapingBodySlam::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Selector";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_LeapingBodySlam::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_LeapingBodySlam::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_LeapingBodySlam::Initialize", "failed to Get_System: Owner:Animator");
	}

	Add_Child(CDispenPerrySequence_LeapingBodySlam::Create(m_pBlackBoard));
	Add_Child(CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::DOWN_START), 3.f, false, g_fDefaultInterpolationDuration, false, {}, 
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 5.f,
			CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", false, IDX(ANIMATION::DISPEN_PERRY::DOWN), 3.f, false, g_fDefaultInterpolationDuration, true))));

	return S_OK;
}

void CDispenPerrySelector_LeapingBodySlam::Activate()
{
	__super::Activate();
}

STATUS CDispenPerrySelector_LeapingBodySlam::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CDispenPerrySelector_LeapingBodySlam::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_LeapingBodySlam::DebugLog()
{
}
#endif

shared_ptr<CDispenPerrySelector_LeapingBodySlam> CDispenPerrySelector_LeapingBodySlam::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_LeapingBodySlam> pInstance = make_private_shared(CDispenPerrySelector_LeapingBodySlam);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_LeapingBodySlam::Create", "Failed to Initialize");
	}

	return pInstance;
}
