#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CTimer final
{
private:
	explicit CTimer() DEFAULT;
	virtual ~CTimer() DEFAULT;

public:
	HRESULT			Initialize();
	void			Tick();

public:
	_float			Get_TimeDelta() const	{ return m_fTimeDelta * m_fTimeScale; }
	
	void			Set_TimeScale(_float _fTimeScale) { m_fTimeScale = _fTimeScale; }
	_float			Get_TimeScale() { return m_fTimeScale; }

	_bool			Check(_float fFPS);

private:
	LARGE_INTEGER	m_cpuTick{};
	LARGE_INTEGER	m_orgTime{};
	LARGE_INTEGER	m_curTime{};
	LARGE_INTEGER	m_oldTime{};

	_float			m_fTimeDelta	= 0.f;
	_float			m_fTimeAcc		= 0.f;

	_float			m_fTimeScale = { 1.f };

public:
	static shared_ptr<CTimer> Create();
};

END
