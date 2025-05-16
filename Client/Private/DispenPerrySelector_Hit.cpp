#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerrySelector_Hit::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Hit";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Hit::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Hit::Initialize", "Failed to Get_WeakSystem: Owner");
	}

	m_pResetTree = m_pBlackBoard->Get_Anything<_bool*>(TEXT("Reset")).value_or(nullptr);
	if (nullptr == m_pResetTree)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Hit::Initialize", "Failed to Get:Reset");
	}
	m_pState = m_pBlackBoard->Get_Anything<DP_STATE*>(TEXT("Owner:State")).value_or(nullptr);
	if (nullptr == m_pState)
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Hit::Initialize", "Failed to Get: Owner:State");
	}

	Add_Child(CDispenPerryLeaf_Damage::Create(m_pBlackBoard));
	Add_Child(CDispenPerrySelector_Weak::Create(m_pBlackBoard));

	return S_OK;
}

void CDispenPerrySelector_Hit::Activate()
{
	__super::Activate();
}

STATUS CDispenPerrySelector_Hit::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (DP_STATE::NOEVENT == *m_pState)
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		*m_pResetTree = true;
		m_eStatus = __super::Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CDispenPerrySelector_Hit::Terminate()
{
	__super::Terminate();

	*m_pState = DP_STATE::NOEVENT;
}

#if ACTIVATE_IMGUI
void CDispenPerrySelector_Hit::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerrySelector_Hit> CDispenPerrySelector_Hit::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerrySelector_Hit> pInstance = make_private_shared(CDispenPerrySelector_Hit);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerrySelector_Hit::Create", "Failed to Initialize");
	}

	return pInstance;
}
