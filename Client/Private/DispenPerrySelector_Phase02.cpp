#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "SkillContainer.h"
#include "Skill.h"

HRESULT CDispenPerrySelector_Phase02::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Phase02";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase02::Initialize", "Failed to __super::Initialize");
	}

	m_pSkillContainer = m_pBlackBoard->Get_System<CSkillContainer>(TEXT("Owner:SkillContainer"));
	if (nullptr == m_pSkillContainer)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase02::Initialize", "Failed to Get: Owner:SkillContainer");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase02::Initialize", "Failed to Get_System: Owner:Transform");
	}
	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase02::Initialize", "Failed to Get_System: Target:Transform");
	}

	m_pDPPhase = m_pBlackBoard->Get_Anything<DP_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pDPPhase)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase02::Initialize", "Failed to Get: Owner:Phase");
	}

	Add_Child(CDispenPerrySelector_SetField::Create(m_pBlackBoard));

	Add_Child(CBehaviorTreeDecorator_RelativeAngle::Create(m_pBlackBoard, m_pTransform, m_pTargetTransform, XM_PIDIV2,
		CDispenPerryLeaf_Turn::Create(m_pBlackBoard, DIRECTION::DIR_END)));

	Add_Child(CBehaviorTreeDecorator_RunOnce::Create(m_pBlackBoard,
		CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::THREAT), 3.f, false, g_fDefaultInterpolationDuration, false)));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: LeapingBodySlam")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_LEAP_RANGE_MIN, ATK_LEAP_RANGE_MAX,
			CBehaviorTreeDecorator_AnimationExit::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::GETUP), 3.f, false, g_fDefaultInterpolationDuration, false,
				CDispenPerrySelector_LeapingBodySlam::Create(m_pBlackBoard)))));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: SpinningArmSwipe")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_ARMSWIPE_RANGE_MIN, ATK_ARMSWIPE_RANGE_MAX,
			CDispenPerryLeaf_FrontArmSwipe::Create(m_pBlackBoard))));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: MultiWaterShot")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_WATERSHOT_RANGE_MIN, ATK_WATERSHOT_RANGE_MAX,
			CDispenPerryLeaf_MultiWaterShot::Create(m_pBlackBoard))));

	Add_Child(CBehaviorTreeDecorator_Random::Create(m_pBlackBoard, { 0.5f, 0.5f }, {
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CBehaviorTreeDecorator_AnimationPrevent::Create(m_pBlackBoard,
			{ IDX(ANIMATION::DISPEN_PERRY::WALK_R_START), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), 2.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::LEFT)))),
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CBehaviorTreeDecorator_AnimationPrevent::Create(m_pBlackBoard,
			{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP) },
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_R_START), 2.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::RIGHT)))) }));

	return S_OK;
}

void CDispenPerrySelector_Phase02::Activate()
{
	__super::Activate();
}

STATUS CDispenPerrySelector_Phase02::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (DP_PHASE::PHASE2 != *m_pDPPhase)
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CDispenPerrySelector_Phase02::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_Phase02::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerrySelector_Phase02> CDispenPerrySelector_Phase02::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_Phase02> pInstance = make_private_shared(CDispenPerrySelector_Phase02);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_Phase02::Create", "Failed to Initialize");
	}

	return pInstance;
}
