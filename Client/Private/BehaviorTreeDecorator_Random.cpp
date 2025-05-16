#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_Random.h"

CBehaviorTreeDecorator_Random::CBehaviorTreeDecorator_Random(initializer_list<shared_ptr<CNode>> _ilNodes, initializer_list<_float> _ilProbabilities)
#ifdef _DEBUG
	: CDebugDecorator(nullptr)
#else
	: CDecorator(nullptr)
#endif
	, m_vecNodes		(_ilNodes)
	, m_vecProbabilities(_ilProbabilities)
{
}

HRESULT CBehaviorTreeDecorator_Random::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Random";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_Random::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_Random::Activate()
{
	__super::Activate();

	m_pNode = m_vecNodes[Function::Probability(initializer_list<_float>(m_vecProbabilities.data(), m_vecProbabilities.data() + m_vecProbabilities.size()))];
	while (STATUS::RUNNING != m_pNode->Invoke(0.f))
	{
		if (m_usetSearchedNodes.size() == m_vecNodes.size())
		{
			m_pNode = nullptr;
			return;
		}
		m_usetSearchedNodes.emplace(m_pNode);
		m_pNode = m_vecNodes[Function::Probability(initializer_list<_float>(m_vecProbabilities.data(), m_vecProbabilities.data() + m_vecProbabilities.size()))];
	}
}

STATUS CBehaviorTreeDecorator_Random::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (nullptr == m_pNode)
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}


	return Return_Invoke();
}

void CBehaviorTreeDecorator_Random::Terminate()
{
	m_pNode = nullptr;
	m_usetSearchedNodes.clear();

	__super::Terminate();
}

#if _DEBUG
void CBehaviorTreeDecorator_Random::Refresh_Node(shared_ptr<CNode> _pRootNode, shared_ptr<CNode> _pParentNode)
{
	CNode::Refresh_Node(_pRootNode, _pParentNode);
	for (auto& pNode : m_vecNodes)
	{
		pNode->Refresh_Node(_pRootNode, _pParentNode);
	}
}
void CBehaviorTreeDecorator_Random::Debug(any _arg)
{
#if ACTIVATE_IMGUI

	if (_arg.has_value() && _arg.type() == typeid(_bool))
	{
		m_bOpenMode = any_cast<_bool>(_arg);
	}
	if (!m_bOpenMode)
	{
		ImGui::SetNextItemOpen(m_bOpenItem);
		m_bEventCall = false;
	}
	else
	{
		ImGui::SetNextItemOpen(m_bOpenMode);
	}

	if (Is_Running()) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

	if (ImGui::TreeNode(string("Decorator: " + m_strNodeName).c_str()))
	{
		if (Is_Running()) ImGui::PopStyleColor();

		DebugLog();
		for (auto& pChild : m_vecNodes)
		{
			pChild->Debug(_arg);
		}

		ImGui::TreePop();
	}
	else if (Is_Running()) ImGui::PopStyleColor();
}
void CBehaviorTreeDecorator_Random::DebugLog()
{
}
#endif
#endif

shared_ptr<CBehaviorTreeDecorator_Random> CBehaviorTreeDecorator_Random::Create(shared_ptr<CBlackBoard> _pBlackBoard, initializer_list<_float> _ilProbabilities, initializer_list<shared_ptr<CNode>> _ilNodes)
{
	shared_ptr<CBehaviorTreeDecorator_Random> pInstance = make_private_shared(CBehaviorTreeDecorator_Random, _ilNodes, _ilProbabilities);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_Random::Create", "Failed to Initialize");
	}

	return pInstance;
}
