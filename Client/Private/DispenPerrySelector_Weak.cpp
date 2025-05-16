#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerrySelector_Weak.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerrySelector_Weak::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Selector";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Weak::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Weak::Initialize", "Failed to Owner");
	}

	Add_Child(CDispenPerryLeaf_Weak::Create(m_pBlackBoard));

	Add_Child(CBehaviorTreeDecorator_AnimationEnter::Create(m_pBlackBoard, "Reference", "Reference", true,
		IDX(ANIMATION::DISPEN_PERRY::DAMAGE_WEAKBREAK), 3.f, false, g_fDefaultInterpolationDuration, false, {},
		CBehaviorTreeDecorator_AnimationExit::Create(m_pBlackBoard, "Reference", "Reference", true,
			IDX(ANIMATION::DISPEN_PERRY::GETUP), 3.f, false, g_fDefaultInterpolationDuration, false,
			CBehaviorTreeDecorator_Repeat::Create(m_pBlackBoard, 10.f,
				CDispenPerryLeaf_WeakDown::Create(m_pBlackBoard)))));

	return S_OK;
}

void CDispenPerrySelector_Weak::Activate()
{
	__super::Activate();

	switch (m_pOwner.lock()->Get_RecentEvent().second)
	{
	case DP_STATE::WEAK:
		m_bSuccess = true;
		break;

	default:
		m_bSuccess = false;
		break;
	}
}

STATUS CDispenPerrySelector_Weak::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_bSuccess)
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CDispenPerrySelector_Weak::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_Weak::DebugLog()
{
}
#endif

shared_ptr<CDispenPerrySelector_Weak> CDispenPerrySelector_Weak::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_Weak> pInstance = make_private_shared(CDispenPerrySelector_Weak);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_Weak::Create", "Failed to Initialize");
	}

	return pInstance;
}
