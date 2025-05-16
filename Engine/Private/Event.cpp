#include "EnginePCH.h"
#include "Event.h"
#include "Animation.h"
#include "CSound_Manager.h"


CEvent::CEvent(const CEvent& rhs)
	: m_fAnimSpeed(rhs.m_fAnimSpeed)
	, m_tEventtype(rhs.m_tEventtype)
	, m_tSoundData(rhs.m_tSoundData)
{

}

HRESULT CEvent::Initialize_Prototype(const EVENTTYPE& EventType, any EventData, weak_ptr<class CAnimation> _pAnimation)
{
	m_tEventtype = EventType;
	switch (EventType)
	{
	case TYPE_SOUND:
	{
		m_tSoundData = any_cast<SOUNDDATA>(EventData);
		//m_SoundFileName = wstring(m_tSoundData.szName, &m_tSoundData.szName[MAX_PATH]);
		m_Function = [=]() { CSound_Manager::Get_Instance()->PlaySoundW(m_tSoundData.SoundName, m_tSoundData.volume); };
	}
	break;
	case TYPE_ANIMSPEED:
	{
		m_fAnimSpeed = any_cast<_float>(EventData);
		m_Function = [=]() { _pAnimation.lock()->Set_AnimSpeed(m_fAnimSpeed); };
	}
	break;
	
	default:
		break;
	}
	return S_OK;
}


HRESULT CEvent::Initialize(weak_ptr<class CAnimation> _pAnimation)
{
	switch (m_tEventtype)
	{
	case TYPE_SOUND:
	{
		m_Function = [=]() { CSound_Manager::Get_Instance()->PlaySoundW(m_tSoundData.SoundName, m_tSoundData.volume); };
	}
	break;
	case TYPE_ANIMSPEED:
	{
		m_Function = [=]() { _pAnimation.lock()->Set_AnimSpeed(m_fAnimSpeed); };
	}
	break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CEvent::Initialize_FromBinary(std::ifstream& _inFile, weak_ptr<CAnimation> _pAnimation)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_tEventtype), sizeof(EVENTTYPE));
	switch (m_tEventtype)
	{
	case EVENTTYPE::TYPE_SOUND:
	{
		//_inFile.read(reinterpret_cast<_byte*>(&m_tSoundData), sizeof(SOUNDDATA));
		_wchar szName[MAX_PATH];
		_inFile.read(reinterpret_cast<_byte*>(szName), MAX_PATH);
		m_tSoundData.SoundName = szName;

		_inFile.read(reinterpret_cast<_byte*>(&m_tSoundData.volume), sizeof(_float));
		//wstring SoundFileName = wstring(m_tSoundData.szName, &m_tSoundData.szName[MAX_PATH]);
		
		m_Function = [=](){	CSound_Manager::Get_Instance()->PlaySoundW(m_tSoundData.SoundName, m_tSoundData.volume);};
	}
	break;
	case EVENTTYPE::TYPE_ANIMSPEED:
	{
		_inFile.read(reinterpret_cast<_byte*>(&m_fAnimSpeed), sizeof(_float));
		m_Function = [=]() {_pAnimation.lock()->Set_AnimSpeed(m_fAnimSpeed); };
	}
	break;
	default:
		break;
	}

	return S_OK;
}

void CEvent::Act()
{
	if (m_Function == nullptr)
	{
		cout << "Event is Null" << endl;
		return;
	}
	m_Function();
}

shared_ptr<CEvent> CEvent::Create(const EVENTTYPE& EventType, weak_ptr<CAnimation> _pAnimation, any EventData)
{
	shared_ptr<CEvent> pInstance = make_private_shared(CEvent);

	if (FAILED(pInstance->Initialize_Prototype(EventType, EventData, _pAnimation)))
	{
		MSG_RETURN(nullptr, "CChannel::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CEvent> CEvent::Read(std::ifstream& _inFile, weak_ptr<CAnimation> _pAnimation)
{
	shared_ptr<CEvent> pInstance = make_private_shared(CEvent);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile, _pAnimation)))
	{
		MSG_RETURN(nullptr, "CChannel::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CEvent> CEvent::Clone(weak_ptr<class CAnimation> _pAnimation)
{
	shared_ptr<CEvent> pInstance = make_private_shared_copy(CEvent, *this);

	if (FAILED(pInstance->Initialize(_pAnimation)))
	{
		MSG_RETURN(nullptr, "CModel::Clone", "Failed to Initialize");
	}
	return pInstance;
}

void CEvent::Bind_Event(EVENTTYPE EventType, function<void(void)> Function)
{
	if (m_tEventtype == EventType)
		m_Function = Function;
}

void CEvent::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_tEventtype), sizeof(EVENTTYPE));
	switch (m_tEventtype)
	{
	case EVENTTYPE::TYPE_SOUND:
		_outFile.write(reinterpret_cast<const _byte*>(m_tSoundData.SoundName.c_str()), MAX_PATH);
		_outFile.write(reinterpret_cast<const _byte*>(&m_tSoundData.volume), sizeof(_float));
		break;
	case EVENTTYPE::TYPE_ANIMSPEED:
		_outFile.write(reinterpret_cast<const _byte*>(&m_fAnimSpeed), sizeof(_float));
		break;
	default:
		break;
	}
}
