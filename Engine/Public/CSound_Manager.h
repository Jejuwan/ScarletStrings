#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CSound_Manager :public CSingleton<CSound_Manager>
{
public :
	enum CHANNELID {
		SOUND_BGM,
		SOUND_BGM2,
		SOUND_BRAINTALK,
		SOUND_CHARGE,
		SOUND1,
		SOUND2,
		SOUND3,
		SOUND4,
		SOUND5,
		SOUND6,
		SOUND7,
		SOUND8,
		SOUND9,
		SOUND10,
		SOUND11,
		SOUND12,
		SOUND13,
		SOUND14,
		SOUND15,
		SOUND16,
		SOUND17,
		SOUND18,
		SOUND20,
		SOUND21,
		SOUND22,
		SOUND23,
		SOUND24,
		SOUND25,
		SOUND26,
		SOUND27,
		SOUND28,
		SOUND29,
		MAXCHANNEL
	};


private:
	explicit CSound_Manager() DEFAULT;
	virtual ~CSound_Manager();

public:
	HRESULT Initialize(); 

public:
	void PlaySound(const wstring& pSoundKey, float fVolume);
	void PlaySound_BrainTalk(CHANNELID eId ,const wstring& pSoundKey, float fVolume);
	void PlaySoundeffect(const wstring& pSoundKey, CHANNELID eID,  float fVolume);
	void PlayBGM(const wstring& pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	bool isplaying(CHANNELID eid);
	void SetPitch(_uint eid, _float fPitch);

#ifdef _DEBUG
	void Mute(_bool bMute = true);
#endif

private:
	void LoadSoundFile(); 

	// 사운드 리소스 정보를 갖는 객체 
	map<wstring, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

#ifdef _DEBUG
	_bool	m_bMute = false;
	vector<_float>	m_vecVolume;
#endif

	friend CSingleton<CSound_Manager>;
};

END