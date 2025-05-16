#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_BehaviorTree.h"
#include "Snail_Dead.h"
#include "Monster_Snail.h"

HRESULT CSnail_Dead::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Dead::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_Dead::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CSnail_Dead::Initialize", "Failed to Get: Target:Transform");
	}
	m_pOwner = m_pBlackBoard->Get_System<CMonster_Snail>(TEXT("Owner"));
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFly_LeafNode_Demaged_Normal_Back::Initialize", "Failed to Get: Owner");
	}
	return S_OK;
}

void CSnail_Dead::Activate()
{
	__super::Activate();
	cout << "Snail Dead State" << endl;
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::DEAD_DOWN, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CSnail_Dead::Invoke(_float _fTimeDelta)
{
	if (m_pEntityDesc->fHP >= 0.f)
	{
		return m_eStatus = STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);

	m_pOwner.lock()->No_RootMotion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);
	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::DEAD_DOWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::DEAD_DOWN))
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

void CSnail_Dead::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Dead> CSnail_Dead::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Dead> pInstance = make_private_shared(CSnail_Dead);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Dead::Create", "Failed to Initialize");
	}

	return pInstance;
}
