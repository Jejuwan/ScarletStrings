#include "ClientPCH.h"
#include "Skill.h"

CSkill::CSkill(_uint _iMaxStock, _float _fCoolTime, _float _fImmediateCoolTime)
	: m_iMaxStock			(_iMaxStock)
	, m_fCoolTime			(_fCoolTime)
	, m_fImmediateCoolTime	(_fImmediateCoolTime)
{
}

void CSkill::Tick(_float _fTimeDelta)
{
	if (m_iCurrentStock < m_iMaxStock)
	{
		if (m_fCurrentCoolTime < m_fCoolTime)
		{
			m_fCurrentCoolTime += _fTimeDelta;
		}
		else
		{
			m_fCurrentCoolTime = 0.f;
			++m_iCurrentStock;
		}
	}

	if (m_bExit)
	{
		if (m_fCurrentImmediateCoolTime < m_fImmediateCoolTime)
		{
			m_fCurrentImmediateCoolTime += _fTimeDelta;
		}
		else
		{
			m_fCurrentImmediateCoolTime = 0.f;
			m_bExit = false;
		}
	}

	m_bAvailable = !m_bExit && m_iCurrentStock;
}

_bool CSkill::Call()
{
	Tick(0.f);
	return m_bAvailable;
}

void CSkill::Exit()
{
	--m_iCurrentStock;
	m_bExit = true;
	Tick(0.f);
}

#if ACTIVATE_IMGUI
void CSkill::DebugLog()
{
	if (!m_bAvailable)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
	}
	ImGui::BulletText("Stock:				%d / %d", m_iCurrentStock, m_iMaxStock);
	ImGui::BulletText("CoolTime:			%f / %f", m_fCurrentCoolTime, m_fCoolTime);
	ImGui::BulletText("ImmediateCoolTime:	%f / %f", m_fCurrentImmediateCoolTime, m_fImmediateCoolTime);
	if (!m_bAvailable)
	{
		ImGui::PopStyleColor();
	}
}
#endif

shared_ptr<CSkill> CSkill::Create(_uint _iMaxStock, _float _fCoolTime, _float _fImmediateCoolTime)
{
	return make_private_shared(CSkill, _iMaxStock, _fCoolTime, _fImmediateCoolTime);
}
