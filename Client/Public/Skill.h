#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Client)

class CSkill final : public ISystem
{
private:
	explicit CSkill(_uint, _float, _float);
	virtual ~CSkill() DEFAULT;

public:
	virtual void				Tick(_float fTimeDelta) override;

public:
	const _bool					Is_Available() const			{ return m_bAvailable; }

	_bool						Call();
	void						Exit();

public:
#if ACTIVATE_IMGUI
	void						DebugLog();
#endif

private:
	const _uint					m_iMaxStock						= 0u;
	const _float				m_fCoolTime						= 0.f;
	const _float				m_fImmediateCoolTime			= 0.f;

	_uint						m_iCurrentStock					= 0u;
	_float						m_fCurrentCoolTime				= 0.f;
	_float						m_fCurrentImmediateCoolTime		= 0.f;

	_bool						m_bAvailable					= false;
	_bool						m_bExit							= true;

public:
	static shared_ptr<CSkill>	Create(_uint iMaxStock, _float fCoolTime, _float fImmediateCoolTime = 0.f);
};

END
