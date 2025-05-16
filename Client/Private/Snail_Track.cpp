#include "ClientPCH.h"
#include "GameInstance.h"
#include "Monster_Snail.h"
#include "Snail_Track.h"

HRESULT CSnail_Track::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Track::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_Track::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CSnail_Track::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CSnail_Track::Initialize", "Failed to Get: Target:Transform");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CSnail_Track::Initialize", "Failed to Get: Target:Transform");
	}
	m_pOwner = m_pBlackBoard->Get_System<CMonster_Snail>(TEXT("Owner"));
	if (nullptr == m_pOwner.lock())
	{
		MSG_RETURN(E_FAIL, "CFly_LeafNode_Demaged_Normal_Back::Initialize", "Failed to Get: Owner");
	}


	return S_OK;
}

void CSnail_Track::Activate()
{
	__super::Activate();
	cout << "Snail Track State" << endl;
	m_pAnimator->Play_Animation(ANIMATION::SNAIL::WALK_FRONT);
}

STATUS CSnail_Track::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);


	m_pTransform->LookAt(m_pTargetTransform);
	m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * _fTimeDelta);


	m_pOwner.lock()->No_RootMotion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	return Return_Invoke();
}

void CSnail_Track::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Track> CSnail_Track::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Track> pInstance = make_private_shared(CSnail_Track);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Track::Create", "Failed to Initialize");
	}

	return pInstance;
}
