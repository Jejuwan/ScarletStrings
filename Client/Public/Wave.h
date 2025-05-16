#pragma once
#include "Client_Define.h"

BEGIN(Client)

class CWave :public std::enable_shared_from_this<CWave>
{
private :
	CWave() DEFAULT;
	virtual ~CWave() DEFAULT;

public :
	void AddWave(_uint WaveIndex, const wstring& PrototypeTag, _float4 InitPos);
	void Tick();

	void Subscribe_WaveInitCallback(_uint WaveIndex, function<void()>);

private :
	_uint											m_CurruntWave = { 0 };
	list<list<pair<wstring, _float4>>>				m_Waves;
	map<_uint, function<void()>>					m_mapWaveCallback;

private:
	_bool											m_bSoundPlay_Once = { false };

public :
	static shared_ptr<CWave>		Create();
};

END