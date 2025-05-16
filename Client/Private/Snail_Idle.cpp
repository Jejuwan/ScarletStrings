#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_Idle.h"
#include "Monster_Snail.h"

HRESULT CSnail_Idle::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Idle::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_Idle::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_LeafNode_Attack::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_LeafNode_Attack::Initialize", "Failed to Get: Target:Transform");
	}

	//m_pNotice = m_pBlackBoard->Get_Anything<_bool*>(TEXT("Owner:Notice")).value_or(nullptr);
	//if (nullptr == m_pNotice)
	//{
	//	MSG_RETURN(E_FAIL, "CSnail_Idle::Initialize", "Failed to Get: Owner:Notice");
	//}
	m_pOwner = m_pBlackBoard->Get_System<CMonster_Snail>(TEXT("Owner"));
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CSnail_LeafNode_Demaged_Normal_Back::Initialize", "Failed to Get: Owner");
	}

	return S_OK;
}

void CSnail_Idle::Activate()
{
	__super::Activate();
	cout << "Snail Idle State" << endl;
	m_pAnimator->Play_Animation(Function::Random({ ANIMATION::SNAIL::IDLE1, ANIMATION::SNAIL::IDLE2 }));
}

STATUS CSnail_Idle::Invoke(_float _fTimeDelta)
{
	//if (true == *m_pNotice)
	//{
	//	return m_eStatus = STATUS::FAILURE;
	//}
	if (m_pMonsterDesc->Notification)
		return m_eStatus = STATUS::FAILURE;

	Begin_Invoke(_fTimeDelta);
	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	if(Function::Distance(m_pTransform,m_pTargetTransform) <= 10.f)
	{
		m_pMonsterDesc->Notification = true;
		return m_eStatus = STATUS::FAILURE;
	}
	m_pOwner.lock()->No_RootMotion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);
	return Return_Invoke();
}

void CSnail_Idle::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Idle> CSnail_Idle::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Idle> pInstance = make_private_shared(CSnail_Idle);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Idle::Create", "Failed to Initialize");
	}

	return pInstance;
}
