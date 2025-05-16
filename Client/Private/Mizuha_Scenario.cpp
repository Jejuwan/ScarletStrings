#include "ClientPCH.h"
#include "Mizuha_Scenario.h"

#include "GameInstance.h"
#include "Pipeline.h"

#include "Camera.h"
#include "CinematicCamera.h"
#include "CutsceneCamera.h"
#include "CutsceneBuffer.h"

#include "FSM.h"

#include "SceneCapture.h"
#include "Renderer.h"

#include "Kyoka.h"
#include "Naomi.h"

#include "ImGui_Manager.h"

#include "Scene_Load.h"
#include "Map_Dynamic.h"

CMizuha_Scenario::CMizuha_Scenario()
{
}

HRESULT CMizuha_Scenario::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CMizuha_Scenario> pInstance = static_pointer_cast<CMizuha_Scenario>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pCinematicState = make_shared<CStateTemplate<CMizuha_Scenario>>();
	m_pCinematicState->Set(pInstance, &CMizuha_Scenario::BeginCinematicState, &CMizuha_Scenario::ExecuteCinematicState, &CMizuha_Scenario::EndCinematicState);
	
	m_pScenarioState = make_shared<CStateTemplate<CMizuha_Scenario>>();
	m_pScenarioState->Set(pInstance, &CMizuha_Scenario::BeginScenarioState, &CMizuha_Scenario::ExecuteScenarioState, &CMizuha_Scenario::EndScenarioState);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pRenderer = dynamic_pointer_cast<CRenderer>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));

	Add_Characters();
	Add_Cutscene_Buffers();

	Read_Scenario(TEXT("Bin/Resources/Cutscene/Scene_Kazuha.csv"));

	

	return S_OK;
}

// Initial State ===================================================

void CMizuha_Scenario::StateInitial(_float fTimeDelta)
{
	GotoState(m_pCinematicState);
}

// Cinematic State ===================================================
void CMizuha_Scenario::BeginCinematicState()
{
	auto pPipeLine = CPipeLine::Get_Instance();
	auto pCineCamera = dynamic_pointer_cast<CCinematicCamera>(pPipeLine->Get_Camera());

	pCineCamera->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_Mizuha_Intro.cin"), TEXT("Cutscene_Intro"));
	pCineCamera->Play_CinematicCamera(TEXT("Cutscene_Intro"));

	pCineCamera->Subscribe_Cinematic_Finish_Callback(std::bind(&CMizuha_Scenario::OnCinematicFinished, this, std::placeholders::_1));

	CGameInstance::Get_Instance()->PlayBGM(TEXT("Mizuha_01.wav"), 1.f);
}

void CMizuha_Scenario::ExecuteCinematicState(_float fTimeDelta)
{

}

void CMizuha_Scenario::EndCinematicState()
{

}

void CMizuha_Scenario::OnCinematicFinished(const wstring& _wstrCinematicTag)
{
	if (false == IsState(m_pScenarioState))
		GotoState(m_pScenarioState);
}

// Scenario State ===================================================
void CMizuha_Scenario::BeginScenarioState()
{
	auto& KeyDialogPair = m_ScenarioDialog[m_iScenarioIndex];
	KEYFRAME& tKeyframe = KeyDialogPair.first;
	DIALOG& tDialog = KeyDialogPair.second;

	auto pSceneCapture = CSceneCapture::Get_Instance();
	pSceneCapture->Ready(m_pRenderer);

	pSceneCapture->Subscribe_Capture_Success_Callback(std::bind(&CMizuha_Scenario::OnSceneCaptured, this));
	pSceneCapture->Capture(CUTSCENE_CHANNEL(tKeyframe.iChannelIndex));

	auto pMainCutscene = m_CutsceneBuffers[tKeyframe.iChannelIndex];
	pMainCutscene->Set_Enable(true);
	pMainCutscene->Play_Animation(tKeyframe.wstrChannelAnimTag);

	m_pRenderer->Set_CutsceneMode(true);

	++m_iScenarioIndex;
}

void CMizuha_Scenario::ExecuteScenarioState(_float fTimeDelta)
{
	{
		static _bool bPause = false;
		if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
		{
			bPause = !bPause;
		}

		if (bPause)
			return;

		static _float fAccTime = 0.f;
		fAccTime += fTimeDelta;
#if ACTIVATE_IMGUI
		ImGui::Begin("Test AccTime");

		ImGui::Text("AccTime: %f", fAccTime);

		ImGui::End();
#endif
	}

	if (m_ScenarioDialog.size() < m_iScenarioIndex + 1)
	{
		m_fSceneChangeAccTime += fTimeDelta;
		if (m_fSceneChangeAccTime >= m_fSceneChangeDelay)
		{
			m_bScenarioEnd = true;

			CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
			CGameInstance::Get_Instance()->ClearPhysX();
			CGameInstance::Get_Instance()->Play_Dialogue(0, TEXT(""), TEXT(""), TEXT(""), 0.f);
		}
		return;
	}

	auto& KeyDialogPair = m_ScenarioDialog[m_iScenarioIndex];
	KEYFRAME& tKeyframe = KeyDialogPair.first;
	DIALOG& tDialog = KeyDialogPair.second;

	if (m_fAccTime >= tKeyframe.fStartTime)
	{
		Stop_Mouth();
		Stop_Record();

		if (tKeyframe.wstrSwitchType == TEXT("Disappear_Left"))
		{
			for (_uint i = 0; i < (_uint)CUTSCENE_CHANNEL::MAX; ++i)
			{
				if (m_CutsceneBuffers[i]->Get_Enable())
					m_CutsceneBuffers[i]->Disappear(_float3(-5.f, 0.f, -1.5f), 1.f);
			}

			Move_Mouth(m_mapCharacters[tDialog.wstrCharacterName]);
		}

		Play_CinematicCamera(tKeyframe.wstrCameraTag);

		auto pSceneCapture = CSceneCapture::Get_Instance();
		pSceneCapture->Record((CUTSCENE_CHANNEL)tKeyframe.iChannelIndex);

		m_CutsceneBuffers[tKeyframe.iChannelIndex]->Set_Enable(true);
		m_CutsceneBuffers[tKeyframe.iChannelIndex]->Play_Animation(tKeyframe.wstrChannelAnimTag
			, [&](const wstring& animTag) {
				
			});

		Move_Mouth(m_mapCharacters[tDialog.wstrCharacterName]);

		auto pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Play_Dialogue(m_iScenarioIndex, tDialog.wstrName, tDialog.wstrDialog0, tDialog.wstrDialog1, 100.f);

		++m_iScenarioIndex;
	}
}

void CMizuha_Scenario::EndScenarioState()
{

}

void CMizuha_Scenario::OnSceneCaptured()
{
	auto pBufferTransform =
		dynamic_pointer_cast<CTransform>(m_CutsceneBuffers[IDX(CUTSCENE_CHANNEL::CHANNEL_MAIN)]->Get_Component(COMPONENT::TRANSFORM));
}

void CMizuha_Scenario::ExecutePersistent(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;

#ifdef _DEBUG
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		if (ImGui::BeginMainMenuBar())
		{
		if (ImGui::BeginMenu("RenderTarget"))
		{
		auto pSceneCapture = CSceneCapture::Get_Instance();
		if (ImGui::BeginMenu("CUTSCENE BUFFER"))
		{
		ImGui::Image(pSceneCapture->Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL::CHANNEL_MAIN).Get(), ImVec2(200, 200));
		ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("CUTSCENE"))
		{
		ImGui::Text("AccTime: %f", m_fAccTime);
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_CUTSCENE_MAIN).Get(), ImVec2(200 * 1.8, 200));
		ImGui::EndMenu();
		}
		ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		}
	}
#endif

	if (m_bScenarioEnd || CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		m_pRenderer->Set_CutsceneMode(false);
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Play_Dialogue(0, TEXT(""), TEXT(""), TEXT(""), 0.f);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::MIZUHA));
	
	}
}

void CMizuha_Scenario::Stop_Mouth()
{
	for (auto& pCharacterPair : m_mapCharacters)
	{
		shared_ptr<CCharacter> pCharacter = dynamic_pointer_cast<CCharacter>(pCharacter);
		if (pCharacter)
			pCharacter->Stop_Mouth();
	}
}

void CMizuha_Scenario::Move_Mouth(shared_ptr<CGameObject> pCharacter)
{
	auto pTarget = dynamic_pointer_cast<CCharacter>(pCharacter);
	if (pTarget)
		pTarget->Move_Mouth();
}

void CMizuha_Scenario::Stop_Record()
{
	auto pSceneCapture = CSceneCapture::Get_Instance();
	pSceneCapture->Stop_Record();
}

void CMizuha_Scenario::Play_CinematicCamera(const wstring& wstrCameraTag)
{
	auto pPipeLine = CPipeLine::Get_Instance();
	auto pCineCamera = dynamic_pointer_cast<CCinematicCamera>(pPipeLine->Get_Camera());
	pCineCamera->Pause_CinematicCamera();
	pCineCamera->Play_CinematicCamera(wstrCameraTag);
}

void CMizuha_Scenario::Add_Characters()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, false);
	m_mapCharacters.emplace(TEXT("KYOKA"), pKyoka);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}

	auto pNaomi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_NAOMI);
	m_mapCharacters.emplace(TEXT("NAOMI"), pNaomi);
	if (FAILED(pLayer->Add(pNaomi)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}

	auto pKasane = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE, false);
	m_mapCharacters.emplace(TEXT("KASANE"), pKasane);
	if (FAILED(pLayer->Add(pKasane)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}

	auto pGemma = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_GEMMA);
	m_mapCharacters.emplace(TEXT("GEMMA"), pGemma);
	if (FAILED(pLayer->Add(pGemma)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, false);
	m_mapCharacters.emplace(TEXT("SHIDEN"), pShiden);
	if (FAILED(pLayer->Add(pShiden)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}

	auto pHaruka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_HARUKA);
	m_mapCharacters.emplace(TEXT("HARUKA"), pHaruka);
	if (FAILED(pLayer->Add(pHaruka)))
	{
		assert(false && "CCutscene_Mizuha::Ready_TestObject : Failed to CObjectLayer::Add");
	}
}

void CMizuha_Scenario::Add_Cutscene_Buffers()
{
	m_pSubCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCutsceneCamera>(SCENE::CUTSCENE_MIZUHA,
		PROTOTYPE_GAMEOBJECT_CUTSCENE_CAMERA);

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_CUTSCENE_BUFFER);
	assert(pLayer && "CMizuha_Scenario::Add_Cutscene_Buffers, Failed to Add_Layer: LAYER_STATIC");
	
	for (_uint i = 0; i < IDX(CUTSCENE_CHANNEL::MAX); ++i)
	{
		m_CutsceneBuffers[i] = dynamic_pointer_cast<CCutsceneBuffer>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
			PROTOTYPE_GAMEOBJECT_CUTSCENE_BUFFER, (CUTSCENE_CHANNEL)i));
		pLayer->Add(m_CutsceneBuffers[i]);

		m_CutsceneBuffers[i]->Bind_Camera(m_pSubCamera, XMMatrixIdentity());
	}
}

void CMizuha_Scenario::Read_Scenario(const wstring& wstrFilePath)
{
	std::wifstream FileIn(wstrFilePath);
	FileIn.imbue(std::locale("kor"));
	FileIn >> std::noskipws;

	std::wstring wstrLine;
	std::wistringstream wLineStream;
	wLineStream.imbue(std::locale("kor"));

	// Read Camera State
	{
		auto pPipeLine = CPipeLine::Get_Instance();
		auto pCineCamera = dynamic_pointer_cast<CCinematicCamera>(pPipeLine->Get_Camera());

		auto pTransform = dynamic_pointer_cast<CTransform>(pCineCamera->Get_Component(COMPONENT::TRANSFORM));
		std::getline(FileIn, wstrLine, L'\n');
		pTransform->Set_Matrix(Read_Transform(FileIn));


		std::getline(FileIn, wstrLine, L'\n');
		wLineStream.clear();
		wLineStream.str(wstrLine);

		_uint iNumCameraAnimation = 0;
		wLineStream >> iNumCameraAnimation;

		wstring wstrFilePath;
		wstring wstrCameraTag;
		for (_uint i = 0; i < iNumCameraAnimation; ++i)
		{
			std::getline(FileIn, wstrFilePath, L'\n');
			std::getline(FileIn, wstrCameraTag, L'\n');
			
			pCineCamera->Append_Cinematic(wstrFilePath, wstrCameraTag);
		}

		std::getline(FileIn, wstrCameraTag, L'\n');
		m_wstrInitCameraAnimTag = wstrCameraTag;
	}

	// Read Buffer State
	{
		std::getline(FileIn, wstrLine, L'\n');
		wLineStream.clear();
		wLineStream.str(wstrLine);

		_uint iNumBufferAnimation = 0;
		wLineStream >> iNumBufferAnimation;

		wstring wstrFilePath;
		wstring wstrChannelAnimTag;
		for (_uint i = 0; i < iNumBufferAnimation; ++i)
		{
			std::getline(FileIn, wstrLine, L'\n');
			wLineStream.clear();
			wLineStream.str(wstrLine);

			_uint iChannelIndex;
			wLineStream >> iChannelIndex;

			std::getline(FileIn, wstrFilePath, L'\n');
			std::getline(FileIn, wstrChannelAnimTag, L'\n');

			m_CutsceneBuffers[iChannelIndex]->Append_Keyframe(wstrFilePath, wstrChannelAnimTag);
		}
	}

	// Read Characters
	{
		std::getline(FileIn, wstrLine, L'\n');
		wLineStream.clear();
		wLineStream.str(wstrLine);

		_uint iNumCharacters = 0;
		wLineStream >> iNumCharacters;

		for (_uint i = 0; i < iNumCharacters; ++i)
		{
			std::getline(FileIn, wstrLine, L'\n');
			wLineStream.clear();
			wLineStream.str(wstrLine);

			auto& pCharacter = m_mapCharacters[wstrLine];
			auto pTransform = dynamic_pointer_cast<CTransform>(pCharacter->Get_Component(COMPONENT::TRANSFORM));

			pTransform->Set_Matrix(Read_Transform(FileIn));
		}
	}

	// Read Dialog
	{
		std::getline(FileIn, wstrLine, L'\n');
		wLineStream.clear();
		wLineStream.str(wstrLine);

		_uint iNumDialog = 0;
		wLineStream >> iNumDialog;

		for (_uint i = 0; i < iNumDialog; ++i)
		{
			KEYFRAME tKeyframe;
			DIALOG tDialog;

			std::getline(FileIn, wstrLine, L'\n');
			wLineStream.clear();
			wLineStream.str(wstrLine);

			wLineStream >> tKeyframe.fStartTime;

			std::getline(FileIn, tDialog.wstrCharacterName, L'\n');
			std::getline(FileIn, tDialog.wstrName, L'/');
			std::getline(FileIn, tDialog.wstrDialog0, L'/');
			std::getline(FileIn, tDialog.wstrDialog1, L'\n');

			std::getline(FileIn, tKeyframe.wstrCameraTag, L'\n');
			std::getline(FileIn, tKeyframe.wstrSwitchType, L'\n');

			std::getline(FileIn, wstrLine, L'\n');
			wLineStream.clear();
			wLineStream.str(wstrLine);

			wLineStream >> tKeyframe.iChannelIndex;

			std::getline(FileIn, tKeyframe.wstrChannelAnimTag, L'\n');

			m_ScenarioDialog.emplace_back(tKeyframe, tDialog);
		}
	}

	FileIn.close();
}

_matrix CMizuha_Scenario::Read_Transform(std::wifstream& File)
{
	std::wstring wstrLine;
	std::wstring wstrElements;
	std::wistringstream wLineStream;

	_float3 vPosition, vRotation, vScaling;

	std::getline(File, wstrLine, L'\n');
	wLineStream.clear();
	wLineStream.str(wstrLine);
	wLineStream >> vPosition.x >> vPosition.y >> vPosition.z;

	std::getline(File, wstrLine, L'\n');
	wLineStream.clear();
	wLineStream.str(wstrLine);
	wLineStream >> vRotation.x >> vRotation.y >> vRotation.z;

	std::getline(File, wstrLine, L'\n');
	wLineStream.clear();
	wLineStream.str(wstrLine);
	wLineStream >> vScaling.x >> vScaling.y >> vScaling.z;

	return XMMatrixAffineTransformation(vScaling, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
		_float3(XMConvertToRadians(vRotation.x), XMConvertToRadians(vRotation.y), XMConvertToRadians(vRotation.z))), vPosition);

}

shared_ptr<CMizuha_Scenario> CMizuha_Scenario::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CMizuha_Scenario> pInstance = make_private_shared(CMizuha_Scenario);

	if (FAILED(pInstance->Initialize(pDevice, pContext)))
	{
		MSG_RETURN(nullptr, "CNaomi::Create", "Failed to Initialize");
	}

	return pInstance;
}
