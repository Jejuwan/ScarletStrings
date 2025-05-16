#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerrySelector_SetField::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Selector";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_SetField::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_SetField::Initialize", "failed to Get_System: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_SetField::Initialize", "failed to Get_System: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_SetField::Initialize", "failed to Get_System: Target:Transform");
	}

	Add_Child(CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::RUN_F_START), 3.f, false, g_fDefaultInterpolationDuration, false,
		{ IDX(ANIMATION::DISPEN_PERRY::RUN_F_START), IDX(ANIMATION::DISPEN_PERRY::RUN_F_ROOP) },
		CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
			CBehaviorTreeLeaf_Animation::Create(m_pBlackBoard, "Reference", "Reference", true,
				IDX(ANIMATION::DISPEN_PERRY::RUN_F_ROOP), 3.f, false, g_fDefaultInterpolationDuration, false))));

	return S_OK;
}

void CDispenPerrySelector_SetField::Activate()
{
	__super::Activate();

	if (_float3(m_pTransform->Get_State(TRANSFORM::POSITION) - DP_FIELDCENTER).length() < DP_FIELDRANGE)
	{
		m_eStatus = STATUS::FAILURE;
	}
//	else
//	{
//		m_pTransform->LookAt(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
//	}
}

STATUS CDispenPerrySelector_SetField::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pTransform->LookAt_Interpolation(m_pTargetTransform, true, false, 32.f, 1.f);

	if (STATUS::FAILURE != m_eStatus)
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CDispenPerrySelector_SetField::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_SetField::DebugLog()
{
}
#endif

shared_ptr<CDispenPerrySelector_SetField> CDispenPerrySelector_SetField::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_SetField> pInstance = make_private_shared(CDispenPerrySelector_SetField);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_SetField::Create", "Failed to Initialize");
	}

	return pInstance;
}
