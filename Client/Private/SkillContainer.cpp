#include "ClientPCH.h"
#include "SkillContainer.h"
#include "Skill.h"

HRESULT CSkillContainer::Register_Skill(const wstring& _wstrTag, shared_ptr<CSkill> _pSkill)
{
	if (m_mapSkill.end() != m_mapSkill.find(_wstrTag))
	{
		MSG_RETURN(E_FAIL, "CSkillContainer::Register_Skill", "Invalid Tag");
	}

	if (nullptr == _pSkill)
	{
		MSG_RETURN(E_FAIL, "CSkillContainer::Register_Skill", "Invalid Skill");
	}

	m_mapSkill.emplace(_wstrTag, _pSkill);

	return S_OK;
}

HRESULT CSkillContainer::Register_Skill(const wstring& _wstrTag, _uint _iMaxStock, _float _fCoolTime, _float _fImmediateCoolTime)
{
	if (m_mapSkill.end() != m_mapSkill.find(_wstrTag))
	{
		MSG_RETURN(E_FAIL, "CSkillContainer::Register_Skill", "Invalid Tag");
	}

	m_mapSkill.emplace(_wstrTag, CSkill::Create(_iMaxStock, _fCoolTime, _fImmediateCoolTime));

	return S_OK;
}

shared_ptr<CSkill> CSkillContainer::Get_Skill(const wstring& _wstrTag) const
{
	if (m_mapSkill.end() == m_mapSkill.find(_wstrTag))
	{
		MSG_RETURN(nullptr, "CSkillContainer::Get_Skill", "Invalid Tag");
	}

	return m_mapSkill.at(_wstrTag);
}

void CSkillContainer::Tick(_float _fTimeDelta)
{
	for (auto& pSkill : m_mapSkill)
	{
		pSkill.second->Tick(_fTimeDelta);
	}
}

const _bool CSkillContainer::Is_Available(const wstring& _wstrTag) const
{
	if (m_mapSkill.end() == m_mapSkill.find(_wstrTag))
	{
		MSG_RETURN(false, "CSkillContainer::Is_Available", "Invalid Tag");
	}

	return m_mapSkill.at(_wstrTag)->Is_Available();
}

const _bool CSkillContainer::Call(const wstring& _wstrTag) const
{
	if (m_mapSkill.end() == m_mapSkill.find(_wstrTag))
	{
		MSG_RETURN(false, "CSkillContainer::Is_Available", "Invalid Tag");
	}
	
	return m_mapSkill.at(_wstrTag)->Call();
}

#if ACTIVATE_IMGUI
void CSkillContainer::DebugLog()
{
	for (auto& pSkill : m_mapSkill)
	{
		if (pSkill.second->Is_Available())
		{
			ImGui::Text(Function::ConvertString(pSkill.first).c_str());
		}
		else
		{
			ImGui::TextDisabled(Function::ConvertString(pSkill.first).c_str());
		}
		pSkill.second->DebugLog();
	}
}
#endif

shared_ptr<CSkillContainer> CSkillContainer::Create()
{
	return make_private_shared(CSkillContainer);
}
