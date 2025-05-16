#include "EnginePCH.h"
#include "CSound_Manager.h"


CSound_Manager::~CSound_Manager()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_RESULT result = FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
}

HRESULT CSound_Manager::Initialize()
{
	FMOD_RESULT fr = FMOD_OK;
	// 사운드를 담당하는 대표객체를 생성하는 함수
	fr = FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	if (fr != FMOD_OK)
		return E_FAIL;

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	fr = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	if (fr != FMOD_OK)
		return E_FAIL;

	LoadSoundFile(); 

#ifdef _DEBUG
	m_vecVolume.resize(CHANNELID::MAXCHANNEL, 0.f);
#endif
	return S_OK;
}

void CSound_Manager::PlaySound(const wstring& pSoundKey, float fVolume)
{
	//map<wstring, FMOD_SOUND*>::iterator iter; 

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	auto iter = m_mapSound.find(pSoundKey);
	
	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE; 

	CHANNELID eID = SOUND1;
	while (isplaying(eID))
	{
		eID = (CHANNELID)(eID + 1);
		if (eID >= MAXCHANNEL)
			return;
	}
	StopSound(eID);
	//wcout << eID <<  pSoundKey << endl;
	
	FMOD_System_PlaySound(m_pSystem, iter->second,nullptr, FALSE, &m_pChannelArr[eID]);
	

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
#ifdef _DEBUG
	m_vecVolume[eID] = fVolume;
	if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[eID], 0.f);
#endif
	//FMOD_Channel_SetPosition(m_pChannelArr[eID],100)
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlaySound_BrainTalk(CHANNELID eId, const wstring& pSoundKey, float fVolume)
{
	auto iter = m_mapSound.find(pSoundKey);

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	CHANNELID eID = eId;
	StopSound(eID);

	if (isplaying(SOUND_BRAINTALK))
	{
		while (true) 
		{
			if (!isplaying(SOUND_BRAINTALK))
				break;
			//Sleep(100); // 재생이 끝날 때까지 대기
		}
	}
	else
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}


	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
#ifdef _DEBUG
	m_vecVolume[eID] = fVolume;
	if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[eID], 0.f);
#endif
	//FMOD_Channel_SetPosition(m_pChannelArr[eID],100)
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlaySoundeffect(const wstring& pSoundKey, CHANNELID eID, float fVolume)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second,nullptr, FALSE, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
#ifdef _DEBUG
	m_vecVolume[eID] = fVolume;
	if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[eID], 0.f);
#endif

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(const wstring& pSoundKey, float fVolume)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return pSoundKey == iter.first;
	});
	
	if (iter == m_mapSound.end())
		return;
	
	if (isplaying(SOUND_BGM))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[SOUND_BGM2]);
		FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM2], FMOD_LOOP_NORMAL);
		FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM2], fVolume);
		FMOD_System_Update(m_pSystem);
		return;
	}
	FMOD_System_PlaySound(m_pSystem, iter->second,nullptr, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
#ifdef _DEBUG
	m_vecVolume[SOUND_BGM] = fVolume;
	if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], 0.f);
#endif
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (eID != CHANNELID::MAXCHANNEL)
	{
		FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
#ifdef _DEBUG
		m_vecVolume[eID] = fVolume;
		if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[eID], 0.f);
#endif
	}
	else
	{
		for (size_t i = 0; i < CHANNELID::MAXCHANNEL; ++i)
		{
			FMOD_Channel_SetVolume(m_pChannelArr[i], 0.f);
#ifdef _DEBUG
			m_vecVolume[i] = fVolume;
			if (m_bMute) FMOD_Channel_SetVolume(m_pChannelArr[i], 0.f);
#endif
		}
	}
	FMOD_System_Update(m_pSystem);
}

bool CSound_Manager::isplaying(CHANNELID eID)
{
	FMOD_BOOL playing = false;

	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &playing);

	return playing;
}

void CSound_Manager::SetPitch(_uint eid, _float fPitch)
{
	FMOD_Channel_SetPitch(m_pChannelArr[eid], fPitch);
}

#ifdef _DEBUG
void CSound_Manager::Mute(_bool _bMute)
{
	m_bMute = _bMute;
	if (m_bMute)
	{
		for (size_t i = 0; i < CHANNELID::MAXCHANNEL; ++i)
		{
			FMOD_Channel_GetVolume(m_pChannelArr[i], &m_vecVolume[i]);
		}
		SetChannelVolume(CHANNELID::MAXCHANNEL, 0.f);
	}
	else
	{
		for (size_t i = 0; i < CHANNELID::MAXCHANNEL; ++i)
		{
			SetChannelVolume(CHANNELID(i), m_vecVolume[i]);
		}
	}
}
#endif

void CSound_Manager::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd; 

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t  handle = _findfirst("Bin/Resources/Sound & movie/BGM/*.*", &fd);

	if (handle == -1)
		return; 

	int iResult = 0; 

	char szCurPath[128] = "Bin/Resources/Sound & movie/BGM/";
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 

		// "../ Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr; 
		
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
			Safe_Delete_Array(pSoundKey);
			printf("(%d) %s\n", eRes, szFullPath);
		}
		else {
			printf("[ERROR] (%d) %s\n", eRes, szFullPath);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
	
	_findclose(handle);
}
