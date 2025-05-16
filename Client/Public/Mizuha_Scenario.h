#pragma once

#include "Client_Define.h"

#include "FSM.h"
#include "StateTemplate.h"

BEGIN(Engine)
class CRenderer;
class CCamera;
END

BEGIN(Client)

class CMizuha_Scenario final : public CFSM
{
private:
	typedef struct tagKeyframeDesc
	{
		_float fStartTime;
		wstring wstrSwitchType;
		wstring wstrCameraTag;

		_uint iChannelIndex;
		wstring wstrChannelAnimTag;
	} KEYFRAME;
	typedef struct tagDialogDesc
	{
		wstring wstrCharacterName;
		wstring wstrName;
		wstring wstrDialog0;
		wstring wstrDialog1;
	} DIALOG;
private:
	CMizuha_Scenario();
	virtual ~CMizuha_Scenario() DEFAULT;

public:
	HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);

public:
	// Initial State ===================================================
	virtual void StateInitial(_float fTimeDelta) override;

private:
	// Cinematic State ===================================================
	virtual void BeginCinematicState();
	virtual void ExecuteCinematicState(_float fTimeDelta);
	virtual void EndCinematicState();

	void OnCinematicFinished(const wstring&);

	// Scenario State ===================================================
	virtual void BeginScenarioState();
	virtual void ExecuteScenarioState(_float fTimeDelta);
	virtual void EndScenarioState();

	void OnSceneCaptured();

private:
	virtual void ExecutePersistent(_float fTimeDelta) override;

private:
	// Cinematic State ===================================================

	// Scenario State ===================================================
	_uint													m_iScenarioIndex = { 0 };
	vector<pair<KEYFRAME, DIALOG>>							m_ScenarioDialog;

	_float													m_fSceneChangeDelay = { 9.f };
	_float													m_fSceneChangeAccTime = { 0.f };

	wstring													m_wstrInitCameraAnimTag;

	// Persistent State ===================================================
	_float													m_fAccTime = { 0.f };

private:
	void Stop_Mouth();
	void Move_Mouth(shared_ptr<CGameObject> pCharacter);
	void Stop_Record();
	void Play_CinematicCamera(const wstring& wstrCameraTag);

private:
	void Add_Characters();
	void Add_Cutscene_Buffers();
	
private:
	void Read_Scenario(const wstring& wstrFilePath);
	_matrix Read_Transform(std::wifstream& File);

private:
	shared_ptr<CStateTemplate<CFSM>> m_StateInitial;

private:
	shared_ptr<CStateTemplate<CMizuha_Scenario>>			m_pCinematicState;
	shared_ptr<CStateTemplate<CMizuha_Scenario>>			m_pScenarioState;

	_bool													m_bScenarioEnd = { false };

private:
	unordered_map<wstring, std::shared_ptr<CGameObject>>	m_mapCharacters;
	shared_ptr<class CCutsceneBuffer>						m_CutsceneBuffers[IDX(CUTSCENE_CHANNEL::MAX)];

private:
	shared_ptr<CCamera>										m_pSubCamera;
	shared_ptr<CRenderer>									m_pRenderer;

private:
	ComPtr<ID3D11Device>									m_pDevice;
	ComPtr<ID3D11DeviceContext>								m_pContext;

public:
	static shared_ptr<CMizuha_Scenario> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
};

END