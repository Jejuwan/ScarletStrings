#include "ClientPCH.h"
#include "Scene_MapTool.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
//#include "Camera_Main.h"
#include "GlobalGizmo.h"
#include "Map_Tool.h"
#include "Camera_MapTool.h"
#include "Map_Road.h"
#include "BattleFieldGenerator.h"
#include "UI_Manager.h"
#include "EventColliderGenerator.h"

#if ACTIVATE_TOOL
CScene_MapTool::CScene_MapTool(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CScene(pDevice, pContext, SCENE::MAPTOOL)
{
}

HRESULT CScene_MapTool::Initialize()
{
	CImGui_Manager::Get_Instance()->Enable();
	CGameInstance::Get_Instance()->Show_Cursor(true);
	CGameInstance::Get_Instance()->Fix_Cursor(false);

	CGameInstance::Get_Instance()->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
	CGameInstance::Get_Instance()->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.5f);
	CGameInstance::Get_Instance()->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 2.228f);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_MapTool::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CScene_MapTool::Initialize", "Failed to Ready_Components");
	}

	if (FAILED(Ready_GameObjects()))
	{
		MSG_RETURN(E_FAIL, "CScene_MapTool::Initialize", "Failed to Ready_GameObjects");
	}
	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_MapTool::Initialize", "Failed to Ready_Lights");
	}

	m_pMap_Tool = CMap_Tool::Create(m_pDevice, m_pContext);

#ifdef _DEBUG
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, TEXT("GENERATOR"));
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: GENERATOR");
	}

	auto pBattleFieldGenerator = CBattleFieldGenerator::Create(m_pDevice, m_pContext);
	shared_ptr<CObjectLayer> pBattleFieldLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, TEXT("BattleField"));
	if (nullptr == pBattleFieldLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: BattleField");
	}
	if (FAILED(pLayer->Add(pBattleFieldGenerator)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Test Object", "Failed to Create_GameObject::Add BattleFieldGenerator");
	}
	pBattleFieldGenerator->Bind_MapTool(m_pMap_Tool);

	auto pEventColliderGenerator = CEventColliderGenerator::Create(m_pDevice, m_pContext);
	shared_ptr<CObjectLayer> pEventColliderLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, TEXT("EventCollider"));
	if (nullptr == pEventColliderLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: EventCollider");
	}
	if (FAILED(pLayer->Add(pEventColliderGenerator)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Test Object", "Failed to Create_GameObject::Add EventColliderGenerator");
	}
	pEventColliderGenerator->Bind_MapTool(m_pMap_Tool);
#endif

    return S_OK;
}

void CScene_MapTool::Tick(_float fTimeDelta)
{
	m_pMap_Tool->Tick(fTimeDelta);

#ifdef _DEBUG
	//CUI_Manager::Get_Instance()->Debug_Render();
#endif
}

void CScene_MapTool::Late_Tick(_float fTimeDelta)
{
	m_pMap_Tool->Late_Tick(fTimeDelta);
}

HRESULT CScene_MapTool::Render()
{
	m_pMap_Tool->Render();

	return S_OK;
}

#ifdef _DEBUG
void CScene_MapTool::Debug()
{
#if ACTIVATE_TOOL

	/*if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		ImGui::Begin("MATERIAL");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_EMISSIVE).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("NONBLEND");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NORMAL).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_OUTLINE).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("LIGHT");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_SHADOWDEPTH).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("MASK");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MASK).Get(), ImVec2(200, 200));
		ImGui::End();
	}*/

#endif
}
#endif

HRESULT CScene_MapTool::Ready_Components()
{
	m_pRenderer = CGameInstance::Get_Instance()->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_RENDERER_MAIN");
	}

	return S_OK;
}
HRESULT CScene_MapTool::Ready_GameObjects()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MAPTOOL, LAYER_STATIC);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}
	m_pCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_MapTool>(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_CAMERA_MAPTOOL);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAPTOOL");
	}

	m_pGizmo = CGameInstance::Get_Instance()->Clone_GameObject<CGlobalGizmo>(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_GLOBALGIZMO);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}

	pLayer->Add(m_pCamera);
	pLayer->Add(m_pGizmo);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MAPTOOL, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/ReflectionCapture.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_MapTool::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	m_pCamera->Set_DebugMode();

	return S_OK;
}

HRESULT CScene_MapTool::Ready_Lights()
{
	LIGHTDESC LightDesc;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(0.3f, 0.3f, 0.3f, 1);
	LightDesc.vDirection = (_float3(0.f, 0.f, 0.f));
	LightDesc.vAmbient = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::MAPTOOL, LightDesc, nullptr);

	return S_OK;
}

shared_ptr<CScene_MapTool> CScene_MapTool::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CScene_MapTool> pInstance = make_private_shared(CScene_MapTool, pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_MapTool::Create", "Failed To CScene_Tool::Initialize");
	}

	return pInstance;
}

#endif
