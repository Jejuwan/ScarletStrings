#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_Inflate.h"
#include "Monster_Snail.h"

HRESULT CSnail_Inflate::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Inflate::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_Inflate::Initialize", "Failed to Get: Owner:Animator");
	}

	//m_pNotice = m_pBlackBoard->Get_Anything<_bool*>(TEXT("Owner:Notice")).value_or(nullptr);
	//if (nullptr == m_pNotice)
	//{
	//	MSG_RETURN(E_FAIL, "CSnail_Inflate::Initialize", "Failed to Get: Owner:Notice");
	//}
	m_pOwner = m_pBlackBoard->Get_System<CMonster_Snail>(TEXT("Owner"));
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFly_LeafNode_Demaged_Normal_Back::Initialize", "Failed to Get: Owner");
	}
	return S_OK;
}

void CSnail_Inflate::Activate()
{
	//*m_pNotice = true;

	__super::Activate();
	cout << "Snail Inflate State" << endl;
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::OIL_WALK_START, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CSnail_Inflate::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);
	m_pOwner.lock()->Update_Motion(_fTimeDelta);

	m_pOwner.lock()->No_RootMotion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::SNAIL::OIL_WALK_START))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::SNAIL::OIL_WALK_START))
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

void CSnail_Inflate::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Inflate> CSnail_Inflate::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Inflate> pInstance = make_private_shared(CSnail_Inflate);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Inflate::Create", "Failed to Initialize");
	}

	return pInstance;
}
