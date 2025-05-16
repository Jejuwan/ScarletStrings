#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"
#include "SkillContainer.h"
#include "Skill.h"

HRESULT CDispenPerrySelector_Phase01::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Phase01";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to __super::Initialize");
	}

	m_pSkillContainer = m_pBlackBoard->Get_System<CSkillContainer>(TEXT("Owner:SkillContainer"));
	if (nullptr == m_pSkillContainer)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get: Owner:SkillContainer");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get_System: Owner:Transform");
	}
	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get_System: Target:Transform");
	}

	m_pDPPhase = m_pBlackBoard->Get_Anything<DP_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pDPPhase)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get: Owner:Phase");
	}

	Add_Child(CDispenPerrySelector_SetField::Create(m_pBlackBoard));

	Add_Child(CBehaviorTreeDecorator_RelativeAngle::Create(m_pBlackBoard, m_pTransform, m_pTargetTransform, XM_PIDIV2,
		CDispenPerryLeaf_Turn::Create(m_pBlackBoard, DIRECTION::DIR_END)));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: LeapingBodySlam")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_LEAP_RANGE_MIN, ATK_LEAP_RANGE_MAX,
			CBehaviorTreeDecorator_AnimationExit::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::GETUP), 3.f, false, g_fDefaultInterpolationDuration, false,
				CDispenPerrySelector_LeapingBodySlam::Create(m_pBlackBoard)))));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: FrontArmSwipe")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_ARMSWIPE_RANGE_MIN, ATK_ARMSWIPE_RANGE_MAX,
			CDispenPerryLeaf_FrontArmSwipe::Create(m_pBlackBoard))));

	Add_Child(CBehaviorTreeDecorator_Skill::Create(m_pBlackBoard, m_pSkillContainer->Get_Skill(TEXT("Atk: WaterShot")), true,
		CBehaviorTreeDecorator_Range::Create(m_pBlackBoard, ATK_WATERSHOT_RANGE_MIN, ATK_WATERSHOT_RANGE_MAX,
			CDispenPerryLeaf_WaterShot::Create(m_pBlackBoard))));

	Add_Child(CBehaviorTreeDecorator_Random::Create(m_pBlackBoard, { 0.5f, 0.5f }, {
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CBehaviorTreeDecorator_AnimationPrevent::Create(m_pBlackBoard,
			{ IDX(ANIMATION::DISPEN_PERRY::WALK_R_START), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), 3.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::LEFT)))),
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CBehaviorTreeDecorator_AnimationPrevent::Create(m_pBlackBoard,
			{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP) },
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_R_START), 3.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::RIGHT)))) }));
/*
	Add_Child(
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), 2.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::LEFT))));

	Add_Child(
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CBehaviorTreeDecorator_AnimationPrevent::Create(m_pBlackBoard,
			{ IDX(ANIMATION::DISPEN_PERRY::WALK_R_START), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
				CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::WALK_L_START), 2.f, false, g_fDefaultInterpolationDuration, false,
				{ IDX(ANIMATION::DISPEN_PERRY::WALK_L_ROOP), IDX(ANIMATION::DISPEN_PERRY::WALK_R_ROOP) },
					CDispenPerryLeaf_Walk::Create(m_pBlackBoard, DIRECTION::LEFT)))));
*/
	return S_OK;
}

void CDispenPerrySelector_Phase01::Activate()
{
	__super::Activate();
}

STATUS CDispenPerrySelector_Phase01::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (DP_PHASE::PHASE1 != *m_pDPPhase)
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CDispenPerrySelector_Phase01::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_Phase01::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerrySelector_Phase01> CDispenPerrySelector_Phase01::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_Phase01> pInstance = make_private_shared(CDispenPerrySelector_Phase01);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_Phase01::Create", "Failed to Initialize");
	}

	return pInstance;
}
