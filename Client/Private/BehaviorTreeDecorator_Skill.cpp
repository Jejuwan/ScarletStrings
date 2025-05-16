#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_Skill.h"
#include "Skill.h"

CBehaviorTreeDecorator_Skill::CBehaviorTreeDecorator_Skill(shared_ptr<CNode> _pNode, shared_ptr<CSkill> _pSkill, _bool _bManualStockManage)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_pSkill				(_pSkill)
	, m_bManageAfterExit	(_bManualStockManage)
{
}

HRESULT CBehaviorTreeDecorator_Skill::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Skill";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Skill::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_Skill::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_Skill::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pNode->Is_Running())
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		if (m_pSkill->Is_Available())
		{
			if (STATUS::RUNNING == m_pNode->Invoke(_fTimeDelta))
			{
				m_pSkill->Call();
				if (m_bManageAfterExit)
				{
					m_pSkill->Exit();
				}

				m_eStatus = STATUS::RUNNING;
			}
			else
			{
				m_eStatus = STATUS::FAILURE;
			}
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_Skill::Terminate()
{
	if (m_pNode->Is_Running())
	{
		if (m_bManageAfterExit)
		{
			m_pSkill->Exit();
		}

		m_pNode->Terminate();
	}

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_Skill::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_Skill> CBehaviorTreeDecorator_Skill::Create(shared_ptr<CBlackBoard> _pBlackBoard, shared_ptr<CSkill> _pSkill, _bool _bManualStockManage, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_Skill> pInstance = make_private_shared(CBehaviorTreeDecorator_Skill, _pNode, _pSkill, _bManualStockManage);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Skill::Create", "Failed to Initialize");
	}

	return pInstance;
}
