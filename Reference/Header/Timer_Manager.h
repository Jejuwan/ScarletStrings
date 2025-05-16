#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CTimer_Manager final : public CSingleton<CTimer_Manager>
{
private:
	explicit CTimer_Manager() DEFAULT;
	virtual ~CTimer_Manager() DEFAULT;

public:
	void									Tick();

public:
	shared_ptr<class CTimer>				Find_Timer(const _float fFPS) const;
	_float									Get_TimeDelta(const _float fFPS) const;

	HRESULT									Add_Timer(const _float fFPS);
	HRESULT									Delete_Timer(const _float fFPS);

	_bool									Check_Timer(const _float fFPS);

public:
	HRESULT									Set_Default_Timer(const _float fFPS);

	HRESULT									Set_TimeScale(const _float fFPS, _float fTimeScale);
	HRESULT									Set_TimeScale(_float fTimeScale);

	_float									Get_TimeScale(const _float fFPS);

private:
	shared_ptr<class CTimer>				m_pDefaultTimer;

private:
	map<_float, shared_ptr<class CTimer>>	m_mapTimer;
	friend CSingleton<CTimer_Manager>;
};

END
