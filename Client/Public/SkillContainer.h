#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Client)

class CSkillContainer final : public ISystem
{
private:
	explicit CSkillContainer() DEFAULT;
	virtual ~CSkillContainer() DEFAULT;

public:
	virtual void							Tick(_float fTimeDelta) override;

public:
	HRESULT									Register_Skill(const wstring& wstrTag, shared_ptr<class CSkill>);
	HRESULT									Register_Skill(const wstring& wstrTag, _uint iMaxStock, _float fCoolTime, _float fImmediateCoolTime = 0.f);

	shared_ptr<class CSkill>				Get_Skill(const wstring& wstrTag) const;

	const _bool								Is_Available(const wstring& wstrTag) const;
	const _bool								Call(const wstring& wstrTag) const;

public:
#if ACTIVATE_IMGUI
	void									DebugLog();
#endif

private:
	map<wstring, shared_ptr<class CSkill>>	m_mapSkill;

public:
	static shared_ptr<CSkillContainer>		Create();
};

END
