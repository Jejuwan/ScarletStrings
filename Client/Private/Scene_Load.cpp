#include "ClientPCH.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Scene_Load.h"
#include "Scene_Test.h"
#include "Scene_Mizuha.h"
#include "Scene_Tool.h"
#include "Scene_MapTool.h"
#include "Scene_RealTest.h"
#include "Scene_Suoh.h"
#include "Scene_Suoh_Battle.h"
#include "Scene_MindRoom.h"
#include "Scene_Hideout.h"
#include "Scene_Title.h"

#include "ImGui_Manager.h"

#pragma region Cutscene
#include "Cutscene_Mizuha.h"
#pragma endregion

#include "LoadingFrame.h"
#include "LoadingLine.h"
#include "LoadingTip.h"
#include "LoadingCharacter.h"

CScene_Load::CScene_Load(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
	: CScene(_pDevice, _pContext, SCENE::LOADING)
	, m_eLoadScene(_eLoadScene)
{
}

HRESULT CScene_Load::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Show_Cursor(false);
	pGameInstance->Fix_Cursor(true);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eLoadScene);

	if (nullptr == m_pLoader)
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed To CLoader::Create");
	}

	pGameInstance->Enable_Lensflare(false);

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed to Ready_BackGround");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed to Ready_Character");
	}

	return S_OK;
}

void CScene_Load::Tick(_float _fTimeDelta)
{
}

void CScene_Load::Late_Tick(_float _fTimeDelta)
{
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText().c_str());

	if (m_pLoader->Is_Finished()/*false*/)
	{
		shared_ptr<CScene> pScene;

		switch (m_eLoadScene)
		{
//		case SCENE::MENU:
//			pScene = CScene_Menu::Create(m_pDevice, m_pContext);
//			break;
//
#if ACTIVATE_TOOL
		case SCENE::TOOL:
			pScene = CScene_Tool::Create(m_pDevice, m_pContext);
			break;

		case SCENE::MAPTOOL:
			pScene = CScene_MapTool::Create(m_pDevice, m_pContext);
			break;
#endif
		case SCENE::TEST:
			pScene = CScene_Test::Create(m_pDevice, m_pContext);
			break;

		case SCENE::REALTEST:
			pScene = CScene_RealTest::Create(m_pDevice, m_pContext);
			break;

		case SCENE::MIZUHA:
			pScene = CScene_Mizuha::Create(m_pDevice, m_pContext);
			break;

		case SCENE::CUTSCENE_MIZUHA:
			pScene = CCutscene_Mizuha::Create(m_pDevice, m_pContext);
			break;

		case SCENE::SUOH:
			pScene = CScene_Suoh::Create(m_pDevice, m_pContext);
			break;

		case SCENE::SUOH_BATTLE:
			pScene = CScene_Suoh_Battle::Create(m_pDevice, m_pContext);
			break;

		case SCENE::MINDROOM:
			pScene = CScene_MindRoom::Create(m_pDevice, m_pContext);
			break;

		case SCENE::HIDEOUT:
			pScene = CScene_Hideout::Create(m_pDevice, m_pContext);
			break;

		case SCENE::TITLE:
			pScene = CScene_Title::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pScene)
		{
			MSG_RETURN(, "CScene_Load::Late_Tick", "Failed To CScene::Create");
		}

		if (FAILED(CGameInstance::Get_Instance()->Open_Scene(m_eLoadScene, pScene, false)))
		{
			MSG_RETURN(, "CScene_Load::Late_Tick", "Failed To Open_Scene");
		}
	}
}

HRESULT CScene_Load::Render()
{
	return S_OK;
}

HRESULT CScene_Load::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::LOADING, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}
	if (FAILED(pLayer->Add(CLoadingFrame::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	if (FAILED(pLayer->Add(CLoadingLine::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	if (FAILED(pLayer->Add(CLoadingTip::Create(m_pDevice, m_pContext, m_eLoadScene))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CScene_Load::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::LOADING, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to Add_Layer: LAYER_CHARACTER");
	}
	
	CLoadingCharacter::LOADINGCHARACTERDESC tCharacterDesc;
	ZeroMemory(&tCharacterDesc, sizeof(CLoadingCharacter::LOADINGCHARACTERDESC));
	tCharacterDesc.wszModelPath = TEXT("Bin/Resources/Model/Arashi/Arashi.mdl");
	tCharacterDesc.wszLineNoiseTexturePath = TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds");
	tCharacterDesc.matTransform = XMMatrixScaling(150.f, 150.f, 1.f) 
		* XMMatrixRotationY(XMConvertToRadians(-180.f)) * XMMatrixRotationZ(XMConvertToRadians(3.f))
		* XMMatrixTranslation(90.f, -235.f, 0.5f);
	tCharacterDesc.iWalkAnimIndex = _uint(ANIMATION::ARASHI::WALK);
	if (FAILED(pLayer->Add(CLoadingCharacter::Create(m_pDevice, m_pContext, tCharacterDesc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to CObjectLayer::Add");
	}

	ZeroMemory(&tCharacterDesc, sizeof(CLoadingCharacter::LOADINGCHARACTERDESC));
	tCharacterDesc.wszModelPath = TEXT("Bin/Resources/Model/Kyoka/Kyoka_Default_Blend.mdl");
	tCharacterDesc.wszLineNoiseTexturePath = TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds");
	tCharacterDesc.matTransform = XMMatrixScaling(150.f, 150.f, 1.f)
		* XMMatrixRotationY(XMConvertToRadians(-180.f)) * XMMatrixRotationZ(XMConvertToRadians(3.f))
		* XMMatrixTranslation(166.f, -235.f, 0.7f);
	tCharacterDesc.iWalkAnimIndex = _uint(ANIMATION::KYOKA::WALK);
	if (FAILED(pLayer->Add(CLoadingCharacter::Create(m_pDevice, m_pContext, tCharacterDesc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to CObjectLayer::Add");
	}

	ZeroMemory(&tCharacterDesc, sizeof(CLoadingCharacter::LOADINGCHARACTERDESC));
	tCharacterDesc.wszModelPath = TEXT("Bin/Resources/Model/Kasane/Kasane.mdl");
	tCharacterDesc.wszLineNoiseTexturePath = TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds");
	tCharacterDesc.matTransform = XMMatrixScaling(150.f, 150.f, 1.f)
		* XMMatrixRotationY(XMConvertToRadians(-180.f)) * XMMatrixRotationZ(XMConvertToRadians(3.f))
		* XMMatrixTranslation(330.f, -246.f, 0.5f);
	tCharacterDesc.iWalkAnimIndex = _uint(ANIMATION::KASANE_CUTSCENE::WALK);
	if (FAILED(pLayer->Add(CLoadingCharacter::Create(m_pDevice, m_pContext, tCharacterDesc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to CObjectLayer::Add");
	}

	ZeroMemory(&tCharacterDesc, sizeof(CLoadingCharacter::LOADINGCHARACTERDESC));
	tCharacterDesc.wszModelPath = TEXT("Bin/Resources/Model/Shiden/Shiden.mdl");
	tCharacterDesc.wszLineNoiseTexturePath = TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds");
	tCharacterDesc.matTransform = XMMatrixScaling(150.f, 150.f, 1.f)
		* XMMatrixRotationY(XMConvertToRadians(-180.f)) * XMMatrixRotationZ(XMConvertToRadians(3.f))
		* XMMatrixTranslation(419.f, -247.f, 0.7f);
	tCharacterDesc.iWalkAnimIndex = _uint(ANIMATION::KASANE_CUTSCENE::WALK);
	if (FAILED(pLayer->Add(CLoadingCharacter::Create(m_pDevice, m_pContext, tCharacterDesc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to CObjectLayer::Add");
	}

	ZeroMemory(&tCharacterDesc, sizeof(CLoadingCharacter::LOADINGCHARACTERDESC));
	tCharacterDesc.wszModelPath = TEXT("Bin/Resources/Model/Kagero/Kagero.mdl");
	tCharacterDesc.wszLineNoiseTexturePath = TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds");
	tCharacterDesc.matTransform = XMMatrixScaling(150.f, 150.f, 1.f)
		* XMMatrixRotationY(XMConvertToRadians(-180.f)) * XMMatrixRotationZ(XMConvertToRadians(3.f))
		* XMMatrixTranslation(500.f, -251.f, 0.7f);
	tCharacterDesc.iWalkAnimIndex = _uint(ANIMATION::KAGERO::WALK);
	if (FAILED(pLayer->Add(CLoadingCharacter::Create(m_pDevice, m_pContext, tCharacterDesc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Ready_Character", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

shared_ptr<CScene_Load> CScene_Load::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE eLoadScene)
{
	shared_ptr<CScene_Load> pInstance = make_private_shared(CScene_Load, _pDevice, _pContext, eLoadScene);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Load", "Failed To CInitialize");
	}

	return pInstance;
}
