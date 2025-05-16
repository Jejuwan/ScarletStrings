#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_BehaviorTree.h"
#include "Snail_Selector.h"

HRESULT CSnail_Selector::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_Selector::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CDecorator_Repeat::Create(m_pBlackBoard, 2.f, 
			CSnail_Idle::Create(m_pBlackBoard)));

	Add_Child(
		CDecorator_Distance::Create(m_pBlackBoard, 10.f, 5.f,
			CSnail_Inflate::Create(m_pBlackBoard)));

	Add_Child(
		CDecorator_Distance::Create(m_pBlackBoard, 20.f, 10.f,
			CSnail_Bile::Create(m_pBlackBoard)));

	Add_Child(
		CDecorator_Repeat::Create(m_pBlackBoard, 1.f, 
			CSnail_Track::Create(m_pBlackBoard)));

	return S_OK;
}

void CSnail_Selector::Activate()
{
	__super::Activate();
}

STATUS CSnail_Selector::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CSnail_Selector::Terminate()
{
	__super::Terminate();
}

shared_ptr<CSnail_Selector> CSnail_Selector::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_Selector> pInstance = make_private_shared(CSnail_Selector);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_Selector::Create", "Failed to Initialize");
	}

	return pInstance;
}