#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_Repeat.h"

CBehaviorTreeDecorator_Repeat::CBehaviorTreeDecorator_Repeat(shared_ptr<CNode> _pNode, _float _fTime)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_fTime(_fTime)
{
}

CBehaviorTreeDecorator_Repeat::CBehaviorTreeDecorator_Repeat(shared_ptr<CNode> _pNode, _uint _iCount)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_iRepeat(_iCount)
{
}

HRESULT CBehaviorTreeDecorator_Repeat::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Repeat";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Repeat::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_Repeat::Activate()
{
	__super::Activate();

	m_iCount = 0u;
}

STATUS CBehaviorTreeDecorator_Repeat::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_fTime)
	{
		if (m_fTimeAcc < m_fTime)
		{
			m_eStatus = m_pNode->Invoke(_fTimeDelta);
			if (!Is_Running())
			{
				m_eStatus = m_pNode->Invoke(0.f);
			}
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}
	else if (m_iRepeat)
	{
		if (m_iRepeat < m_iCount)
		{ 
			m_eStatus = m_pNode->Invoke(_fTimeDelta);
			if (STATUS::SUCCESS == m_eStatus)
			{
				++m_iCount;
			}
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_Repeat::Terminate()
{
	if (m_pNode->Is_Running())
	{
		m_pNode->Terminate();
	}

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_Repeat::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CBehaviorTreeDecorator_Repeat> CBehaviorTreeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fTime, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_Repeat> pInstance = make_private_shared(CBehaviorTreeDecorator_Repeat, _pNode, _fTime);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehaviorTreeDecorator_Repeat> CBehaviorTreeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, _uint _iCount, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_Repeat> pInstance = make_private_shared(CBehaviorTreeDecorator_Repeat, _pNode, _iCount);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}
