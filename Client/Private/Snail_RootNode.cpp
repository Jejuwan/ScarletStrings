#include "ClientPCH.h"
#include "GameInstance.h"
#include "Snail_BehaviorTree.h"
#include "Snail_RootNode.h"
#include "Snail_Selector.h"

HRESULT CSnail_RootNode::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CSnail_RootNode::Initialize", "Failed to __super::Initialize");
	}

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CSnail_RootNode::Initialize", "Failed to Ready_Skills");
	}

	//m_pBlackBoard->Add_Anything(TEXT("Owner:Notice"), &m_bNotice);

	m_pSelector = CSnail_Selector::Create(m_pBlackBoard);
	if (nullptr == m_pSelector)
	{
		MSG_RETURN(E_FAIL, "CSnail_RootNode::Initialize", "Failed to CSnail_Selector::Create");
	}
	

	return S_OK;
}

void CSnail_RootNode::Activate()
{
	__super::Activate();
}

STATUS CSnail_RootNode::Invoke(_float _fTimeDelta)
{
	//if (m_bExit)
	//{
	//	return STATUS::FAILURE;
	//}

	Begin_Invoke(_fTimeDelta);

	for (auto& iter : m_mapSkills)
	{
		if (iter.second.iStock < iter.second.iMaxStock)
		{
			iter.second.fCurrentCoolTime += _fTimeDelta;
			if (iter.second.fCurrentCoolTime >= iter.second.fCoolTime)
			{
				iter.second.fCurrentCoolTime = 0.f;
				++iter.second.iStock;
			}
		}
	}

	//static _bool bExit = false;
	//ImGui::Begin("Snail");
	//if (ImGui::Checkbox("Exit", &bExit))
	//{
	//	m_bExitTrigger = bExit;
	//}
	//ImGui::End();

	//if (!m_bExitTrigger)
	//{
		m_eStatus = m_pSelector->Invoke(_fTimeDelta);
	//}
	//else
	//{
	//	// Handle
	//	switch (m_pDeath->Invoke(_fTimeDelta))
	//	{
	//	case STATUS::SUCCESS:
	//		m_bExit = true;
	//		break;
	//	case STATUS::RUNNING:
	//	case STATUS::FAILURE:
	//	case STATUS::MAX:
	//		break;
	//	}
	//}

	return Return_Invoke();
}

void CSnail_RootNode::Terminate()
{
	__super::Terminate();
}

void CSnail_RootNode::Exit_Tree()
{
	//m_bExitTrigger = true;
}

HRESULT CSnail_RootNode::Ready_Skills()
{
	SKILLDESC tSkill{};

	tSkill.iMaxStock		= 1;	// Skill Max Stock
	tSkill.fCoolTime		= 2.f;	
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime	= 0.f;
	tSkill.fCoefficient		= 0.f;	// Atk
	m_mapSkills.emplace(TEXT("Bile"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:Bile"), &m_mapSkills[TEXT("Bile")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 2.f;	
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime	= 0.f;
	tSkill.fCoefficient		= 0.f;
	m_mapSkills.emplace(TEXT("Inflate"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:Inflate"), &m_mapSkills[TEXT("Inflate")]);

	return S_OK;
}

shared_ptr<CSnail_RootNode> CSnail_RootNode::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CSnail_RootNode> pInstance = make_private_shared(CSnail_RootNode);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CSnail_RootNode::Create", "Failed to Initialize");
	}

	return pInstance;
}
