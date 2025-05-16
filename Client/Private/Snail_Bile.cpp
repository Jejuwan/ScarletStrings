#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_Bile.h"
#include "Monster_Snail.h"

HRESULT CSnail_Bile::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Bile::Initialize", "Failed to __super::Initialize");
	}
	
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_Bile::Initialize", "Failed to Get: Owner:Animator");
	}

	//m_pNotice = m_pBlackBoard->Get_Anything<_bool*>(TEXT("Owner:Notice")).value_or(nullptr);
	//if (nullptr == m_pNotice)
	//{
	//	MSG_RETURN(E_FAIL, "CSnail_Bile::Initialize", "Failed to Get: Owner:Notice");
	//}
	m_pOwner = m_pBlackBoard->Get_System<CMonster_Snail>(TEXT("Owner"));
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFly_LeafNode_Demaged_Normal_Back::Initialize", "Failed to Get: Owner");
	}
	return S_OK;
}

void CSnail_Bile::Activate()
{
	//*m_pNotice = true;

	__super::Activate();
	cout << "Snail Bile State" << endl;
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::SHOOT, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CSnail_Bile::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);
	m_pOwner.lock()->Update_Motion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::SHOOT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::SHOOT))
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

	m_pOwner.lock()->No_RootMotion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);
	return Return_Invoke();
}

void CSnail_Bile::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Bile> CSnail_Bile::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Bile> pInstance = make_private_shared(CSnail_Bile);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Bile::Create", "Failed to Initialize");
	}

	return pInstance;
}
