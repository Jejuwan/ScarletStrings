#include "ClientPCH.h"
#include "Cutscene_Mizuha.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"

#include "Mizuha_Scenario.h"

#include "CameraKeyframeGenerator.h"
#include "ObjectKeyframeGenerator.h"

#include "UI_Manager.h"

#include "Scene_Load.h"

#include "Map_ToolManager.h"
#include "MenuCharacter.h"

#include "Light.h"

CCutscene_Mizuha::CCutscene_Mizuha(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::CUTSCENE_MIZUHA)
{
}

HRESULT CCutscene_Mizuha::Initialize()
{
	//CGameInstance::Get_Instance()->Show_Cursor(false);
	//CGameInstance::Get_Instance()->Fix_Cursor(true);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif


	auto pGameInstance = CGameInstance::Get_Instance();
	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.3f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.25f);

	// Neon
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.145f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 1.095f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 1.455f);

	// LUT Filter
	pGameInstance->Enable_LUTFilter(true);
	pGameInstance->Set_LUTIndex(4);

	// FXAA
	CGameInstance::Get_Instance()->Enable_FXAA(true);

	// SSAO
	pGameInstance->Enable_SSAO(true);

	// Lensflare
	pGameInstance->Enable_Lensflare(true);
	pGameInstance->Set_SunPosition(_float3(2000.f, 10.f, 3000.f));
	pGameInstance->Set_SunColor(_float3(3.810, 0.993f, 0.786f));
	pGameInstance->Set_LensflareStrength(1.517f);

	// DOF
	pGameInstance->Enable_DOF(false);
	pGameInstance->Set_DOF_DepthStart(800.f);
	pGameInstance->Set_DOF_DepthRange(1000.f);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_Camera");
	}
	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_BackGround");
	}
	if (FAILED(Ready_TestObject()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_TestObject");
	}
	if (FAILED(Ready_TestUI()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_TestUI");
	}

	if (FAILED(Ready_Map()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_Map");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_Lights");
	}
#ifdef _DEBUG
	//if (FAILED(Ready_DebugObjects()))
	//{
	//	MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Initialize", "Failed to Ready_DebugObjects");
	//}
#endif

	m_pScenario = CMizuha_Scenario::Create(m_pDevice, m_pContext);

	return S_OK;
}

void CCutscene_Mizuha::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();
#endif
#endif

	m_pScenario->Tick(fTimeDelta);
}

void CCutscene_Mizuha::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCutscene_Mizuha::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CCutscene_Mizuha::Debug()
{
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("RenderTarget"))
			{
				if (ImGui::BeginMenu("MATERIAL"))
				{
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_DIFFUSE).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_AMBIENT).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_SPECULAR).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_EMISSIVE).Get(), ImVec2(200, 200));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("NONBLEND"))
				{
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NORMAL).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_OUTLINE).Get(), ImVec2(200, 200));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("LIGHT"))
				{
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_DIFFUSE).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_AMBIENT).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_SPECULAR).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_SHADOWDEPTH).Get(), ImVec2(200, 200));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("MASK"))
				{
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS).Get(), ImVec2(200, 200));
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MASK).Get(), ImVec2(200, 200));
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();



		}
	}
}

HRESULT CCutscene_Mizuha::Ready_DebugObjects()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_DEBUG_OBJECT);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_Camera", "Failed to Add_Layer: LAYER_DEBUG_OBJECT");
	}

	shared_ptr<CCameraKeyframeGenerator> pCameraTool =
		CGameInstance::Get_Instance()->Clone_GameObject<CCameraKeyframeGenerator>(SCENE::CUTSCENE_MIZUHA,
			PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR);
	if (FAILED(pLayer->Add(pCameraTool)))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_DebugObjects", "Failed to CObjectLayer::Add");
	}

	shared_ptr<CObjectKeyframeGenerator> pObjectTool =
		CGameInstance::Get_Instance()->Clone_GameObject<CObjectKeyframeGenerator>(SCENE::CUTSCENE_MIZUHA,
			PROTOTYPE_GAMEOBJECT_OBJECT_KEYFRAME_GENERATOR);
	if (FAILED(pLayer->Add(pObjectTool)))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_DebugObjects", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}
#endif

HRESULT CCutscene_Mizuha::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Main> pMainCam = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN);
	if (FAILED(pLayer->Add(pMainCam)))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_Camera", "Failed to CObjectLayer::Add");
	}


#ifdef _DEBUG
	pMainCam->Set_DebugMode();
#endif

	return S_OK;
}

HRESULT CCutscene_Mizuha::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/LC12_Cube.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CLOUD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CCutscene_Mizuha::Ready_TestObject()
{

	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_HARUKA))))
	//{
	//	MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_TestObject", "Failed to CObjectLayer::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI))))
	//{
	//	MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_TestObject", "Failed to CObjectLayer::Add");
	//}


	return S_OK;
}

HRESULT CCutscene_Mizuha::Ready_TestUI()
{
	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_UI_3D);
	if (nullptr == pLayerUI3D)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Add_Layer: LAYER_UI_3D");
	}

	{
		CMenuCharacter::MENUCHARACTERDESC tMenuCharacterDesc;
		tMenuCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_KASANE;
		tMenuCharacterDesc.matTransform = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, -1.f, 2.5f);
		tMenuCharacterDesc.vMtrlDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		tMenuCharacterDesc.iAnimIndex = IDX(ANIMATION::KASANE_CUTSCENE::WAIT_02);
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	{
		CMenuCharacter::MENUCHARACTERDESC tMenuCharacterDesc;
		tMenuCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_SHIDEN;
		tMenuCharacterDesc.matTransform = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, -1.f, 2.5f);
		tMenuCharacterDesc.vMtrlDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		tMenuCharacterDesc.iAnimIndex = IDX(ANIMATION::SHIDEN::WAIT_02);
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 8, 0 });
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	{
		CMenuCharacter::MENUCHARACTERDESC tMenuCharacterDesc;
		tMenuCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_ARASHI;
		tMenuCharacterDesc.matTransform = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, -1.f, 2.5f);
		tMenuCharacterDesc.vMtrlDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		tMenuCharacterDesc.iAnimIndex = IDX(ANIMATION::ARASHI::WAIT_02);
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	{
		CMenuCharacter::MENUCHARACTERDESC tMenuCharacterDesc;
		tMenuCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_KYOKA;
		tMenuCharacterDesc.matTransform = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, -1.f, 2.5f);
		tMenuCharacterDesc.vMtrlDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		tMenuCharacterDesc.iAnimIndex = IDX(ANIMATION::KYOKA::WAIT_02);
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	{
		CMenuCharacter::MENUCHARACTERDESC tMenuCharacterDesc;
		tMenuCharacterDesc.wszModelTag = PROTOTYPE_COMPONENT_MODEL_KAGERO;
		tMenuCharacterDesc.matTransform = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, -1.f, 2.5f);
		tMenuCharacterDesc.vMtrlDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		tMenuCharacterDesc.iAnimIndex = IDX(ANIMATION::KAGERO::WAIT02);
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 0, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 2, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 3, 17 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 1, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 6, 0 });
		tMenuCharacterDesc.vecMeshPassIndexPair.push_back({ 7, 0 });
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}


	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_TestUI", "Failed to Add_Layer: LAYER_UI");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_CUTSCENE_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	return S_OK;
}

HRESULT CCutscene_Mizuha::Ready_Lights()
{
	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * 0.25f;
	LightDesc.vDirection = _float3(5.f, -1, -20.f);
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * 0.25f;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::CUTSCENE_MIZUHA, LightDesc, nullptr);

	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(500.f, 500.f);
	LightDesc.vPosition = _float3(-175.f, 165.f, -90.f);
	LightDesc.vDirection = _float3(-0.75f, -1.f, -0.75f);
	LightDesc.fRange = 200.f;
	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::CUTSCENE_MIZUHA, LightDesc, nullptr);
	
	pShadow->Set_ShadowDesc(SHADOWDESC{ 1.f, -0.5f, 0.f, 0.05f, _int2(8, 8) });

	return S_OK;
}

HRESULT CCutscene_Mizuha::Ready_Map()
{
	m_pRoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_ROAD);
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_STATIC);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_DYNAMIC);
	m_pBuildingLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_BUILDING);
	m_pSpecialDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_SPECIALDYNAMIC);
	m_pETCLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::CUTSCENE_MIZUHA, LAYER_MAP_ETC);

	Load_MizuhaMapFile(TEXT("Common_BaseWall"), 0);
#pragma region Road
	Load_MizuhaMapFile(TEXT("Area_MiniMapA"), 0);
	Load_MizuhaMapFile(TEXT("Dirt_Tile"), 0);
	Load_MizuhaMapFile(TEXT("SandyOil_Tile"), 0);
	Load_MizuhaMapFile(TEXT("Circle_Tile"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraA"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraB"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraC"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraD"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraE"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraF"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraG"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraH"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraI"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraJ"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraK"), 0);
	Load_MizuhaMapFile(TEXT("ConcretePiraL"), 0);
	Load_MizuhaMapFile(TEXT("FootholdD"), 0);
	Load_MizuhaMapFile(TEXT("FootholdE"), 0);
	Load_MizuhaMapFile(TEXT("FootholdF"), 0);
	Load_MizuhaMapFile(TEXT("FootholdShapeA"), 0);
	Load_MizuhaMapFile(TEXT("FootholdShapeB"), 0);
	Load_MizuhaMapFile(TEXT("FootholdG"), 0);
	Load_MizuhaMapFile(TEXT("Dirt_Ground"), 0);
	Load_MizuhaMapFile(TEXT("BasementA"), 0);
	Load_MizuhaMapFile(TEXT("BasementB"), 0);
	Load_MizuhaMapFile(TEXT("BasementC"), 0);
	Load_MizuhaMapFile(TEXT("BasementD"), 0);
	Load_MizuhaMapFile(TEXT("BasementE"), 0);
	Load_MizuhaMapFile(TEXT("ConstructionBuildingC"), 0);
	Load_MizuhaMapFile(TEXT("Bridge_EnkeiB"), 0);
	Load_MizuhaMapFile(TEXT("Basement_BendA"), 0);
	Load_MizuhaMapFile(TEXT("BossWallA"), 0);
	Load_MizuhaMapFile(TEXT("BossWallB"), 0);
	Load_MizuhaMapFile(TEXT("BossWallC"), 0);
	Load_MizuhaMapFile(TEXT("BossWallD"), 0);
	Load_MizuhaMapFile(TEXT("BossWallE"), 0);
	Load_MizuhaMapFile(TEXT("BossWallE"), 0);
	Load_MizuhaMapFile(TEXT("BossWallF"), 0);
	Load_MizuhaMapFile(TEXT("BossWallG"), 0);
	Load_MizuhaMapFile(TEXT("BossWallH"), 0);
	Load_MizuhaMapFile(TEXT("BossWallI"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterA"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterB"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterC"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterD"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterF"), 0);
	Load_MizuhaMapFile(TEXT("BossBanisterE"), 0);
	Load_MizuhaMapFile(TEXT("Bridge_EnkeiC"), 0);

#pragma endregion

#pragma region Static
	Load_MizuhaMapFile(TEXT("FenceA"), 1);
	Load_MizuhaMapFile(TEXT("FenceB"), 1);
	Load_MizuhaMapFile(TEXT("FenceC"), 1);
	Load_MizuhaMapFile(TEXT("RealSteelWallF"), 1);
	Load_MizuhaMapFile(TEXT("RealSteelWallA"), 1);
	Load_MizuhaMapFile(TEXT("RealSteelWallB"), 1);
	Load_MizuhaMapFile(TEXT("RealSteelWallC"), 1);
	Load_MizuhaMapFile(TEXT("SteelWall5A"), 1);
	Load_MizuhaMapFile(TEXT("SteelWall5B"), 1);
	Load_MizuhaMapFile(TEXT("SteelWall5C"), 1);
	Load_MizuhaMapFile(TEXT("HeavyEquipmentA"), 1);
	Load_MizuhaMapFile(TEXT("HeavyEquipmentC"), 1);
	Load_MizuhaMapFile(TEXT("ConstructionGateA"), 1);
	Load_MizuhaMapFile(TEXT("HeavyEquipmentE"), 1);
	Load_MizuhaMapFile(TEXT("SteelWall4A"), 1);
	Load_MizuhaMapFile(TEXT("ConstructionSlopeA"), 1);
	Load_MizuhaMapFile(TEXT("PrefabA"), 1);
	Load_MizuhaMapFile(TEXT("PrefabB"), 1);
	Load_MizuhaMapFile(TEXT("RockA"), 1);
	Load_MizuhaMapFile(TEXT("RockB"), 1);
	Load_MizuhaMapFile(TEXT("RockC"), 1);
	Load_MizuhaMapFile(TEXT("RubbleA"), 1);
	Load_MizuhaMapFile(TEXT("RubbleB"), 1);
	Load_MizuhaMapFile(TEXT("FencingA"), 1);
	Load_MizuhaMapFile(TEXT("FencingB"), 1);
	Load_MizuhaMapFile(TEXT("FencingC"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorA"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorB"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorC"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorD"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorE"), 1);
	Load_MizuhaMapFile(TEXT("OutsideFloorF"), 1);
	Load_MizuhaMapFile(TEXT("NDF0"), 1);
	Load_MizuhaMapFile(TEXT("PlyWoodA"), 1);
	Load_MizuhaMapFile(TEXT("PlyWoodB"), 1);
	Load_MizuhaMapFile(TEXT("BioToilet"), 1);
	Load_MizuhaMapFile(TEXT("RubbleC"), 1);
	Load_MizuhaMapFile(TEXT("RubbleD"), 1);
	Load_MizuhaMapFile(TEXT("ConcreteWallA"), 1);
	Load_MizuhaMapFile(TEXT("SteelPoleA"), 1);
	Load_MizuhaMapFile(TEXT("TowerCrane_Base"), 1);
	Load_MizuhaMapFile(TEXT("FencingD"), 1);
	Load_MizuhaMapFile(TEXT("FencingE"), 1);
	Load_MizuhaMapFile(TEXT("RoadBlockB"), 1);
#pragma endregion

#pragma region Dynamic
	Load_MizuhaMapFile(TEXT("BarrelA"), 2);
	Load_MizuhaMapFile(TEXT("BarrelB"), 2);
	Load_MizuhaMapFile(TEXT("Dumpster"), 2);
	Load_MizuhaMapFile(TEXT("BoxA"), 2);
	Load_MizuhaMapFile(TEXT("BoxB"), 2);
	Load_MizuhaMapFile(TEXT("ConcreteBagA"), 2);
	Load_MizuhaMapFile(TEXT("ConcreteBagB"), 2);
	Load_MizuhaMapFile(TEXT("ConcretePipeA"), 2);
	Load_MizuhaMapFile(TEXT("ConcretePipeB"), 2);
	Load_MizuhaMapFile(TEXT("ConcretePipeC"), 2);
	Load_MizuhaMapFile(TEXT("ConcretePipeD"), 2);
	Load_MizuhaMapFile(TEXT("SpoolA"), 2);
	Load_MizuhaMapFile(TEXT("HeavyEquipmentH"), 2);
	Load_MizuhaMapFile(TEXT("Brick_Palette"), 2);
	Load_MizuhaMapFile(TEXT("CableReel"), 2);
	Load_MizuhaMapFile(TEXT("IceBox"), 2);
#pragma endregion

#pragma region Special_Dynamic
	Load_MizuhaMapFile(TEXT("Special_BeamA"), 3);
	Load_MizuhaMapFile(TEXT("HeavyEquipmentG"), 3);
#pragma endregion

#pragma region Building
	Load_MizuhaMapFile(TEXT("RealSteelWallD"), 4);
	Load_MizuhaMapFile(TEXT("RealSteelWallE"), 4);
	Load_MizuhaMapFile(TEXT("SteelWall4ANon"), 4);
	Load_MizuhaMapFile(TEXT("SteelWall3A"), 4);
	Load_MizuhaMapFile(TEXT("SteelWall3B"), 4);
	Load_MizuhaMapFile(TEXT("CityBuilding_PartsA"), 4);
	Load_MizuhaMapFile(TEXT("CityBuildingA"), 4);
	Load_MizuhaMapFile(TEXT("ConstructionBuildingA"), 4);
	Load_MizuhaMapFile(TEXT("ConstructionBuildingB"), 4);
	Load_MizuhaMapFile(TEXT("Crane_EnkeiA"), 4);
	Load_MizuhaMapFile(TEXT("DistantBuildingA"), 4);
	Load_MizuhaMapFile(TEXT("Ironpillar"), 4);
	Load_MizuhaMapFile(TEXT("TarpA"), 4);
	Load_MizuhaMapFile(TEXT("TarpB"), 4);
	Load_MizuhaMapFile(TEXT("TarpC"), 4);
	Load_MizuhaMapFile(TEXT("PlyWoodC"), 4);
	Load_MizuhaMapFile(TEXT("PlyWoodD"), 4);
	Load_MizuhaMapFile(TEXT("Ground_CoalA"), 4);
	Load_MizuhaMapFile(TEXT("CityBuilding_Tower"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingA"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingB"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingC"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingD"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingE"), 4);
	Load_MizuhaMapFile(TEXT("MizuhaBuildingF"), 4);
#pragma endregion
#pragma region ETC
	Load_MizuhaMapFile(TEXT("Ground_CarC"), 5);
	Load_MizuhaMapFile(TEXT("Ground_Grass"), 5);
	Load_MizuhaMapFile(TEXT("RoadBlockA"), 5);
	Load_MizuhaMapFile(TEXT("Ground_ConeA"), 5);
	Load_MizuhaMapFile(TEXT("Ground_ConeB"), 5);
	Load_MizuhaMapFile(TEXT("TanA"), 5);
	Load_MizuhaMapFile(TEXT("TanB"), 5);
	Load_MizuhaMapFile(TEXT("TanC"), 5);
	Load_MizuhaMapFile(TEXT("TanD"), 5);
	Load_MizuhaMapFile(TEXT("TanE"), 5);
#pragma endregion

#pragma region LightObject
	//Load_MizuhaMapFile(TEXT("SpotlightA"), 4);
	//Load_MizuhaMapFile(TEXT("FloodlightB"), 4);
#pragma endregion

#pragma region Neon
	Load_MizuhaMapFile(TEXT("NeonCornerA"), 4);
	Load_MizuhaMapFile(TEXT("NeonCornerB"), 4);
	Load_MizuhaMapFile(TEXT("NeonCornerC"), 4);
	Load_MizuhaMapFile(TEXT("NeonCornerD"), 4);
	Load_MizuhaMapFile(TEXT("NeonCrashA"), 4);
	Load_MizuhaMapFile(TEXT("NeonCrashB"), 4);
	Load_MizuhaMapFile(TEXT("NeonLineA"), 4);
	Load_MizuhaMapFile(TEXT("NeonLineB"), 4);
	Load_MizuhaMapFile(TEXT("NeonRollA"), 4);
	Load_MizuhaMapFile(TEXT("NeonSwayA"), 4);
	Load_MizuhaMapFile(TEXT("NeonSwayB"), 4);
	Load_MizuhaMapFile(TEXT("NeonSwayC"), 4);
#pragma endregion
	return S_OK;
}

HRESULT CCutscene_Mizuha::Load_MizuhaMapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("Bin/Resources/Map/Stage1/dat/Stage1_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	size_t      dataLength;
	wstring     Name;
	_float3		Scale = { 0.f,0.f,0.f };
	_float3     Rotation = { 0.f,0.f,0.f };
	_float3		Tanslation = { 0.f,0.f,0.f };
	_float4x4	Mat;

	while (true)
	{
		if (!ReadFile(hFile, &dataLength, sizeof(size_t), &dwByte, nullptr))
			break;
		if (dataLength == 0)
			break;
		if (0 == dwByte)
			break;
		Name.resize(dataLength);
		if (!ReadFile(hFile, &Name[0], sizeof(wchar_t) * dataLength, &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Scale, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Rotation, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Tanslation, sizeof(_float3), &dwByte, nullptr))
			break;
		if (!ReadFile(hFile, &Mat, sizeof(_float4x4), &dwByte, nullptr))
			break;

		wstring targetString = strObjName;

		if (Name.find(targetString) != wstring::npos)
		{
			wstring extractedString = Name.substr(0, targetString.length());
			m_strObjName = extractedString;
		}
		else
			m_strObjName = TEXT("");

		if (m_strObjName == strObjName)
		{
			m_strPrototypeName = CMap_ToolManager::Get_Instance()->MizuhaMap_Name(m_strObjName);

			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObjTuple_Road0
				= { SCENE::TEST, m_strObjName, Scale,Rotation,Tanslation, Mat, m_strObjName };

			if (iIndex == 0)
			{
				if (FAILED(m_pRoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 3)
			{
				if (FAILED(m_pSpecialDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}

			else if (iIndex == 4)
			{
				if (FAILED(m_pBuildingLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 5)
			{
				if (FAILED(m_pETCLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::CUTSCENE_MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

void CCutscene_Mizuha::Map_Name()
{
	if (m_strObjName == TEXT("Common_BaseWall"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_COMMON_BASEWALL;
#pragma region Road
	if (m_strObjName == TEXT("Area_MiniMap"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAP;
	else if (m_strObjName == TEXT("Dirt_Tile"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_Dirt_Tile;
	else if (m_strObjName == TEXT("SandyOil_Tile"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SandyOil_Tile;
	else if (m_strObjName == TEXT("Circle_Tile"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CIRCLE_TILE;
	else if (m_strObjName == TEXT("Area_MiniMapA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_AREA_MINIMAPA;
	else if (m_strObjName == TEXT("ConcretePiraA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAA;
	else if (m_strObjName == TEXT("ConcretePiraB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAB;
	else if (m_strObjName == TEXT("ConcretePiraC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAC;
	else if (m_strObjName == TEXT("ConcretePiraD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAD;
	else if (m_strObjName == TEXT("ConcretePiraE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAE;
	else if (m_strObjName == TEXT("ConcretePiraF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAF;
	else if (m_strObjName == TEXT("ConcretePiraG"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAG;
	else if (m_strObjName == TEXT("ConcretePiraH"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAH;
	else if (m_strObjName == TEXT("ConcretePiraI"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAI;
	else if (m_strObjName == TEXT("ConcretePiraJ"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAJ;
	else if (m_strObjName == TEXT("ConcretePiraK"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAK;
	else if (m_strObjName == TEXT("ConcretePiraL"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIRAL;
	else if (m_strObjName == TEXT("FootholdD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDD;
	else if (m_strObjName == TEXT("FootholdE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDE;
	else if (m_strObjName == TEXT("FootholdF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDF;
	else if (m_strObjName == TEXT("FootholdShapeA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEA;
	else if (m_strObjName == TEXT("FootholdShapeB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDSHAPEB;
	else if (m_strObjName == TEXT("FootholdG"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FOOTHOLDG;
	else if (m_strObjName == TEXT("Dirt_Ground"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DIRT_GROUND;
	else if (m_strObjName == TEXT("BasementA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTA;
	else if (m_strObjName == TEXT("BasementB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTB;
	else if (m_strObjName == TEXT("BasementC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTC;
	else if (m_strObjName == TEXT("BasementD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTD;
	else if (m_strObjName == TEXT("BasementE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTE;
	else if (m_strObjName == TEXT("ConstructionBuildingC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGC;
	else if (m_strObjName == TEXT("Bridge_EnkeiB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIB;
	else if (m_strObjName == TEXT("Basement_BendA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BASEMENTBENDA;
	else if (m_strObjName == TEXT("BossWallA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLA;
	else if (m_strObjName == TEXT("BossWallB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLB;
	else if (m_strObjName == TEXT("BossWallC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLC;
	else if (m_strObjName == TEXT("BossWallD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLD;
	else if (m_strObjName == TEXT("BossWallE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLE;
	else if (m_strObjName == TEXT("BossWallF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLF;
	else if (m_strObjName == TEXT("BossWallG"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLG;
	else if (m_strObjName == TEXT("BossWallH"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLH;
	else if (m_strObjName == TEXT("BossWallI"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSWALLI;
	else if (m_strObjName == TEXT("BossBanisterA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERA;
	else if (m_strObjName == TEXT("BossBanisterB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERB;
	else if (m_strObjName == TEXT("BossBanisterC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERC;
	else if (m_strObjName == TEXT("BossBanisterD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERD;
	else if (m_strObjName == TEXT("BossBanisterE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERE;
	else if (m_strObjName == TEXT("BossBanisterF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOSSBANISTERF;
	else if (m_strObjName == TEXT("Bridge_EnkeiC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRIDGE_ENKEIC;
#pragma endregion

#pragma region Static
	else if (m_strObjName == TEXT("FenceA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEA;
	else if (m_strObjName == TEXT("FenceB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEB;
	else if (m_strObjName == TEXT("FenceC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCEC;
	else if (m_strObjName == TEXT("RealSteelWallF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLF;
	else if (m_strObjName == TEXT("RealSteelWallA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLA;
	else if (m_strObjName == TEXT("RealSteelWallB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLB;
	else if (m_strObjName == TEXT("RealSteelWallC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLC;
	else if (m_strObjName == TEXT("SteelWall5B"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5B;
	else if (m_strObjName == TEXT("SteelWall5A"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5A;
	else if (m_strObjName == TEXT("SteelWall5C"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL5C;
	else if (m_strObjName == TEXT("HeavyEquipmentA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTA;
	else if (m_strObjName == TEXT("HeavyEquipmentC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTC;
	else if (m_strObjName == TEXT("ConstructionGateA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONGATEA;
	else if (m_strObjName == TEXT("HeavyEquipmentE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTE;
	else if (m_strObjName == TEXT("SteelWall4A"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4A;
	else if (m_strObjName == TEXT("ConstructionSlopeA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONSLOPEA;
	else if (m_strObjName == TEXT("PrefabA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PREFABA;
	else if (m_strObjName == TEXT("PrefabB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PREFABB;
	else if (m_strObjName == TEXT("RockA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKA;
	else if (m_strObjName == TEXT("RockB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKB;
	else if (m_strObjName == TEXT("RockC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROCKC;
	else if (m_strObjName == TEXT("RubbleA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEA;
	else if (m_strObjName == TEXT("RubbleB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEB;
	else if (m_strObjName == TEXT("FencingA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGA;
	else if (m_strObjName == TEXT("FencingB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGB;
	else if (m_strObjName == TEXT("FencingC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGC;
	else if (m_strObjName == TEXT("OutsideFloorA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORA;
	else if (m_strObjName == TEXT("OutsideFloorB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORB;
	else if (m_strObjName == TEXT("OutsideFloorC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORC;
	else if (m_strObjName == TEXT("OutsideFloorD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORD;
	else if (m_strObjName == TEXT("OutsideFloorE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORE;
	else if (m_strObjName == TEXT("OutsideFloorF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_OUTSIDEFLOORF;
	else if (m_strObjName == TEXT("NDF0"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NDF0;
	else if (m_strObjName == TEXT("PlyWoodA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODA;
	else if (m_strObjName == TEXT("PlyWoodB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODB;
	else if (m_strObjName == TEXT("BioToilet"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BIOTOILET;
	else if (m_strObjName == TEXT("RubbleC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLEC;
	else if (m_strObjName == TEXT("RubbleD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_RUBBLED;
	else if (m_strObjName == TEXT("ConcreteWallA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEWALLA;
	else if (m_strObjName == TEXT("SteelPoleA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELPOLEA;
	else if (m_strObjName == TEXT("TowerCrane_Base"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TOWERCRANE_BASE;
	else if (m_strObjName == TEXT("FencingD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGD;
	else if (m_strObjName == TEXT("FencingE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_FENCINGE;
	else if (m_strObjName == TEXT("RoadBlockB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKB;
#pragma endregion

}

void CCutscene_Mizuha::Map_Name_Second()
{
#pragma region Dynamic
	if (m_strObjName == TEXT("BarrelA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BARRELA;
	else if (m_strObjName == TEXT("BarrelB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BARRELB;
	else if (m_strObjName == TEXT("Dumpster"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER;
	else if (m_strObjName == TEXT("BoxA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOXA;
	else if (m_strObjName == TEXT("BoxB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BOXB;
	else if (m_strObjName == TEXT("ConcreteBagA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA;
	else if (m_strObjName == TEXT("ConcreteBagB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB;
	else if (m_strObjName == TEXT("ConcretePipeA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA;
	else if (m_strObjName == TEXT("ConcretePipeB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB;
	else if (m_strObjName == TEXT("ConcretePipeC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC;
	else if (m_strObjName == TEXT("ConcretePipeD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED;
	else if (m_strObjName == TEXT("SpoolA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPOOLA;
	else if (m_strObjName == TEXT("HeavyEquipmentH"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTH;
	else if (m_strObjName == TEXT("Brick_Palette"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE;
	else if (m_strObjName == TEXT("CableReel"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL;
	else if (m_strObjName == TEXT("IceBox"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ICEBOX;
#pragma endregion

#pragma region SpecialDynamic
	else if (m_strObjName == TEXT("Special_BeamA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA;
	else if (m_strObjName == TEXT("HeavyEquipmentG"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_HEAVYEQUIPMENTG;
#pragma endregion

#pragma region Building
	else if (m_strObjName == TEXT("RealSteelWallD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLD;
	else if (m_strObjName == TEXT("RealSteelWallE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_REALSTEELWALLE;
	else if (m_strObjName == TEXT("SteelWall4ANon"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL4ANON;
	else if (m_strObjName == TEXT("SteelWall3A"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3A;
	else if (m_strObjName == TEXT("SteelWall3B"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_STEELWALL3B;
	else if (m_strObjName == TEXT("CityBuilding_PartsA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_PARTSA;
	else if (m_strObjName == TEXT("CityBuildingA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDINGA;
	else if (m_strObjName == TEXT("ConstructionBuildingA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGA;
	else if (m_strObjName == TEXT("ConstructionBuildingB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CONSTRUCTIONBUILDINGB;
	else if (m_strObjName == TEXT("Crane_EnkeiA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CRANE_ENKEIA;
	else if (m_strObjName == TEXT("DistantBuildingA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_DISTANTBUILDINGA;
	else if (m_strObjName == TEXT("Ironpillar"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_IRONPILLAR;
	else if (m_strObjName == TEXT("TarpA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPA;
	else if (m_strObjName == TEXT("TarpB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPB;
	else if (m_strObjName == TEXT("TarpC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TARPC;
	else if (m_strObjName == TEXT("PlyWoodC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODC;
	else if (m_strObjName == TEXT("PlyWoodD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_PLYWOODD;
	else if (m_strObjName == TEXT("Ground_CoalA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_COALA;
	else if (m_strObjName == TEXT("CityBuilding_Tower"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_CITYBUILDING_TOWER;
	else if (m_strObjName == TEXT("MizuhaBuildingA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGA;
	else if (m_strObjName == TEXT("MizuhaBuildingB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGB;
	else if (m_strObjName == TEXT("MizuhaBuildingC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGC;
	else if (m_strObjName == TEXT("MizuhaBuildingD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGD;
	else if (m_strObjName == TEXT("MizuhaBuildingE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGE;
	else if (m_strObjName == TEXT("MizuhaBuildingF"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_MIZUHABUILDINGF;
#pragma endregion

#pragma region ETC
	else if (m_strObjName == TEXT("Ground_CarC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CARC;

	else if (m_strObjName == TEXT("Ground_Grass"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_GRASS;
	else if (m_strObjName == TEXT("RoadBlockA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_ROADBLOCKA;
	else if (m_strObjName == TEXT("Ground_ConeA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEA;
	else if (m_strObjName == TEXT("Ground_ConeB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_GROUND_CONEB;
	else if (m_strObjName == TEXT("TanA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANA;
	else if (m_strObjName == TEXT("TanB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANB;
	else if (m_strObjName == TEXT("TanC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANC;
	else if (m_strObjName == TEXT("TanD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TAND;
	else if (m_strObjName == TEXT("TanE"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_TANE;
#pragma endregion

#pragma region Neon
	else if (m_strObjName == TEXT("NeonCornerA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERA;
	else if (m_strObjName == TEXT("NeonCornerB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERB;
	else if (m_strObjName == TEXT("NeonCornerC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERC;
	else if (m_strObjName == TEXT("NeonCornerD"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCORNERD;
	else if (m_strObjName == TEXT("NeonCrashA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHA;
	else if (m_strObjName == TEXT("NeonCrashB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONCRASHB;
	else if (m_strObjName == TEXT("NeonLineA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONLINEA;
	else if (m_strObjName == TEXT("NeonLineB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONLINEB;
	else if (m_strObjName == TEXT("NeonRollA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONROLLA;
	else if (m_strObjName == TEXT("NeonSwayA"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYA;
	else if (m_strObjName == TEXT("NeonSwayB"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYB;
	else if (m_strObjName == TEXT("NeonSwayC"))
		m_strPrototypeName = PROTOTYPE_GAMEOBJECT_MAP_NEONSWAYC;
#pragma endregion
}

shared_ptr<CCutscene_Mizuha> CCutscene_Mizuha::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCutscene_Mizuha> pInstance = make_private_shared(CCutscene_Mizuha, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Load", "Failed To CInitialize");
	}

	return pInstance;
}