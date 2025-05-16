#include "ClientPCH.h"
#include "GameInstance.h"
#include "DebugNode.h"

#ifdef _DEBUG

void CDebugNode::Activate()
{
	__super::Activate();

	m_bEventCall = true;
	m_bOpenItem = true;
}

void CDebugNode::Terminate()
{
	m_bEventCall = true;
	m_bOpenItem = false;

	__super::Terminate();
}

void CDebugNode::Debug(any _arg)
{
#if ACTIVATE_IMGUI

	if (_arg.has_value() && _arg.type() == typeid(_bool))
	{
		m_bOpenMode = any_cast<_bool>(_arg);
	}
	if (!m_bOpenMode)
	{
		ImGui::SetNextItemOpen(m_bOpenItem);
		if (m_bEventCall)
		{
			ImGui::SetNextItemOpen(m_bOpenItem);
			m_bEventCall = false;
		}
	}
	else
	{
		ImGui::SetNextItemOpen(m_bOpenMode);
	}

	if (Is_Running()) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

	if (ImGui::TreeNode(string("Leaf: " + m_strNodeName).c_str()))
	{
		if (Is_Running()) ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		DebugLog();
		ImGui::PopStyleColor();

		ImGui::TreePop();
	}
	else if (Is_Running()) ImGui::PopStyleColor();

#endif
}

void CDebugNode::DebugLog()
{
}

#endif
