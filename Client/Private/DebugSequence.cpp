#include "ClientPCH.h"
#include "GameInstance.h"
#include "DebugSequence.h"

#ifdef _DEBUG

void CDebugSequence::Activate()
{
	__super::Activate();

	m_bEventCall = true;
	m_bOpenItem = true;
}

void CDebugSequence::Terminate()
{
	m_bEventCall = true;
	m_bOpenItem = false;

	__super::Terminate();
}

void CDebugSequence::Debug(any _arg)
{
#if ACTIVATE_IMGUI
	
	if (_arg.has_value() && _arg.type() == typeid(_bool))
	{
		m_bOpenMode = any_cast<_bool>(_arg);
	}
	if (!m_bOpenMode)
	{
		ImGui::SetNextItemOpen(m_bOpenItem);
	}
	else
	{
		ImGui::SetNextItemOpen(m_bOpenMode);
	}

	if (Is_Running()) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

	if (ImGui::TreeNode(string("Sequence: " + m_strNodeName).c_str()))
	{
		if (Is_Running()) ImGui::PopStyleColor();

		DebugLog();
		for (auto& pChild : m_lstChildren)
		{
			pChild->Debug(_arg);
		}

		ImGui::TreePop();
	}
	else if (Is_Running()) ImGui::PopStyleColor();
#endif
}

void CDebugSequence::DebugLog()
{
}

#endif
