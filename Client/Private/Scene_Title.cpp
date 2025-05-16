#include "ClientPCH.h"
#include "Scene_Title.h"

#include "GameInstance.h"
#include "Scene_Load.h"

#include "TitleCharacter.h"
#include "Camera_Main.h"
#include "UI_Manager.h"

CScene_Title::CScene_Title(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TITLE)
{
}

HRESULT CScene_Title::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Show_Cursor(false);
	pGameInstance->Fix_Cursor(true);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTitleScene::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CTitleScene::Initialize", "Failed to Apply_PostProcessing");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Initialize", "Failed to Ready_BackGround");
	}
	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed to Ready_Character");
	}
	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed to Ready_Camera");
	}
	if (FAILED(Add_Light()))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed to Add_Light");
	}

	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_SCARLET NEXUS.mp3"), 1.f);

	return S_OK;
}

void CScene_Title::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();
#endif
}

void CScene_Title::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::TEST));

		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Title_05.Selected.wav"), 1.f);
	}
}

HRESULT CScene_Title::Render()
{
	return S_OK;
}

HRESULT CScene_Title::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.7f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.5f);

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.111f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 2.315f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 0.202f);

	// SSAO
	pGameInstance->Enable_SSAO(true);

	// LUT Filter
	pGameInstance->Enable_LUTFilter(true);
	pGameInstance->Set_LUTIndex(4);

	// Lensflare
	pGameInstance->Enable_Lensflare(false);
	pGameInstance->Set_SunPosition(_float3(2000.f, 10.f, 3000.f));
	pGameInstance->Set_SunColor(_float3(1.4f, 1.2f, 1.0f));
	pGameInstance->Set_LensflareStrength(2.0f);

	// DOF
	pGameInstance->Enable_DOF(false);
	pGameInstance->Set_DOF_DepthStart(160.f);
	pGameInstance->Set_DOF_DepthRange(200.f);

	pGameInstance->Enable_FXAA(true);

	return S_OK;
}

HRESULT CScene_Title::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TITLE, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Main> pCutsceneCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE);
	if (FAILED(pLayer->Add(pCutsceneCamera)))
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_Camera", "Failed to CObjectLayer::Add");
	}

	pCutsceneCamera->Lock_MouseInput(true);

#ifdef _DEBUG
	pCutsceneCamera->Set_DebugMode(true);
#endif

	return S_OK;
}

HRESULT CScene_Title::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TITLE, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_LOGO))))
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_ICON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CScene_Title::Ready_Character()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CObjectLayer> pLayer = pGameInstance->Add_Layer(SCENE::TITLE, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Title::Ready_Character", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	// Kasane
	{
		CTitleCharacter::TITLECHARACTERDESC tTitleCharacterDesc;
		tTitleCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_KASANE;
		tTitleCharacterDesc.matTransform = XMMatrixScaling(0.8f, 0.8f, 0.8f)
			* XMMatrixRotationY(XMConvertToRadians(132.5f)) * XMMatrixTranslation(0.42f, -0.9f, 2.775f);
		tTitleCharacterDesc.vMtrlDiffuse = _float4(0.8f, 0.7f, 0.7f, 1.f);
		tTitleCharacterDesc.iAnimIndex = 1;
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 0 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		if (FAILED(pLayer->Add(pGameInstance->Clone_GameObject(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_CHARACTER, tTitleCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Title::Ready_Character", "Failed to CObjectLayer::Add");
		}
	}
	
	// Yuito
	{
		CTitleCharacter::TITLECHARACTERDESC tTitleCharacterDesc;
		tTitleCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_YUITO;
		tTitleCharacterDesc.matTransform = XMMatrixScaling(0.8f, 0.8f, 0.8f)
			* XMMatrixRotationY(XMConvertToRadians(-166.5f)) * XMMatrixTranslation(-0.42f, -1.215, 2.875f);
		tTitleCharacterDesc.vMtrlDiffuse = _float4(0.8f, 0.7f, 0.7f, 1.f);
		tTitleCharacterDesc.iAnimIndex = 0;
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 0 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 4, 17 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		tTitleCharacterDesc.vecMeshPassIndexPair.push_back({ 8, 0 });
		if (FAILED(pLayer->Add(pGameInstance->Clone_GameObject(SCENE::TITLE, PROTOTYPE_GAMEOBJECT_TITLE_CHARACTER, tTitleCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Title::Ready_Character", "Failed to CObjectLayer::Add");
		}
	}

	return S_OK;
}

HRESULT CScene_Title::Add_Light()
{
	LIGHTDESC LightDesc;
	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .5;
	LightDesc.vDirection = (_float3(1.f, -1.f, -0.6f)).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .5;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::TITLE, LightDesc, nullptr);

	return S_OK;
}

shared_ptr<CScene_Title> CScene_Title::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Title> pInstance = make_private_shared(CScene_Title, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Title", "Failed To CInitialize");
	}

	return pInstance;
}
