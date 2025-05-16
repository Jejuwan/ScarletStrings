#include "ClientPCH.h"
#include "Wave.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Scene_Manager.h"
#include "UI_Psyco_Wave.h"
#include "UI_Indicator_Script.h"

void CWave::AddWave(_uint WaveIndex, const wstring& PrototypeTag, _float4 InitPos)
{
	while (WaveIndex >= m_Waves.size())
	{
		list<pair<wstring, _float4>> Wave;
		m_Waves.push_back(Wave);
	}
	auto& iter = m_Waves.back();
	
	iter.push_back({ PrototypeTag,InitPos });
}

void CWave::Tick()
{
	auto Scene = CGameInstance::Get_Instance()->Current_Scene();
	auto Layer = CGameInstance::Get_Instance()->Find_Layer(Scene, LAYER_MONSTER);
	if (nullptr == Layer)
		return;
	auto Object = Layer->Begin();
	if (Layer->End() != Layer->Begin())
		return;
	auto Wave = m_Waves.begin();
	if (m_CurruntWave >= m_Waves.size())
	{
		if (SCENE::TEST == CGameInstance::Get_Instance()->Current_Scene() && false == m_bSoundPlay_Once)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"Tutorial_finish.wav", 1.f);
			auto UI_Manager = CUI_Manager::Get_Instance()->Get_Script();
			UI_Manager->Set_ScriptRenderStart();
			//auto UI_Manager = CUI_Manager::Get_Instance()->Get_Psyco_Wave();
			//UI_Manager->Set_Psyco_Wave();
			m_bSoundPlay_Once = true;
		}
		
		return;
	}
	for (size_t i = 0; i < m_CurruntWave; ++i)
		++Wave;
	for (auto iter : *Wave)
	{
		Layer->Add(CGameInstance::Get_Instance()->Find_Pool(Scene, iter.first)->Pop(iter.second, true));
	}

	// Callback
	auto Iter = m_mapWaveCallback.find(m_CurruntWave);
	if (Iter != m_mapWaveCallback.end())
	{
		auto fpWaveCallback = Iter->second;
		if (fpWaveCallback)
		{
			fpWaveCallback();
			fpWaveCallback = nullptr;
		}
	}

	++m_CurruntWave;
}

void CWave::Subscribe_WaveInitCallback(_uint WaveIndex, function<void()> fpWaveEndCallback)
{
	m_mapWaveCallback[WaveIndex] = fpWaveEndCallback;
}

shared_ptr<CWave> CWave::Create()
{
	return make_private_shared(CWave);
}
