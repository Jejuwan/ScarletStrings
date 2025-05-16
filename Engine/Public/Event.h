#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEvent
{
public :
	struct SOUNDDATA {
		wstring SoundName;
		//_char szName[MAX_PATH] = "";
		_float volume = { 1.f };
	};
private :
	explicit CEvent() DEFAULT;
	explicit CEvent(const CEvent& rhs);
	virtual ~CEvent() DEFAULT;

	HRESULT Initialize_Prototype(const EVENTTYPE& EventType, any EventData, weak_ptr<class CAnimation> _pAnimation);
	HRESULT Initialize(weak_ptr<class CAnimation> _pAnimation);
	HRESULT Initialize_FromBinary(std::ifstream&, weak_ptr<class CAnimation> _pAnimation);

public :
	void Act();

private :
	EVENTTYPE					m_tEventtype = { TYPE_END };
	//pair<weak_ptr<void>, function<void(const wstring&, _float)>> m_Function;
	SOUNDDATA					m_tSoundData;
	_float						m_fAnimSpeed = { 0 };
	function<void(void)>		m_Function;

	void*						m_FunctionAny = { nullptr };

public :
	EVENTTYPE					Get_Type() { return m_tEventtype; }
	SOUNDDATA					Get_SoundData() { return m_tSoundData; }
	_float						Get_AnimSpeed() { return m_fAnimSpeed; }
	void						Bind_Event(EVENTTYPE EventType, function<void(void)> Function);


public :
	static shared_ptr<CEvent> 	Create(const EVENTTYPE& EventType, weak_ptr<class CAnimation> _pAnimation, any EventData = nullptr);
	static shared_ptr<CEvent> 	Read(std::ifstream&, weak_ptr<class CAnimation> _pAnimation);
	shared_ptr<CEvent>			Clone(weak_ptr<class CAnimation> _pAnimation);

	void						Export(std::ofstream&);
};

END