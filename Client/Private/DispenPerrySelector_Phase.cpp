#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerrySelector_Phase::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Phase";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase::Initialize", "Failed to Get_EntityDesc: Owner:EntityDesc");
	}

	m_pResetTree = m_pBlackBoard->Get_Anything<_bool*>(TEXT("Reset")).value_or(nullptr);
	if (nullptr == m_pResetTree)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get:Reset");
	}
	m_pState = m_pBlackBoard->Get_Anything<DP_STATE*>(TEXT("Owner:State")).value_or(nullptr);
	if (nullptr == m_pState)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Phase01::Initialize", "Failed to Get: Owner:State");
	}

	m_pBlackBoard->Add_Anything(TEXT("Owner:Phase"), &m_ePhase);

	Add_Child(CDispenPerrySelector_Phase01::Create(m_pBlackBoard));
	Add_Child(CDispenPerrySelector_Phase02::Create(m_pBlackBoard));

	return S_OK;
}

void CDispenPerrySelector_Phase::Activate()
{
	__super::Activate();
}

STATUS CDispenPerrySelector_Phase::Invoke(_float _fTimeDelta)
{
	if (*m_pResetTree)
	{
		Terminate();
		*m_pResetTree = false;
	}

	Begin_Invoke(_fTimeDelta);

	if (DP_STATE::NOEVENT != *m_pState)
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		if (m_pEntityDesc->fHP / m_pEntityDesc->fMaxHP >= 0.6f)
		{
			m_ePhase = DP_PHASE::PHASE1;
		}
		else
		{
			m_ePhase = DP_PHASE::PHASE2;
		}

		m_eStatus = __super::Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CDispenPerrySelector_Phase::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_Phase::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerrySelector_Phase> CDispenPerrySelector_Phase::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_Phase> pInstance = make_private_shared(CDispenPerrySelector_Phase);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_Phase::Create", "Failed to Initialize");
	}

	return pInstance;
}
