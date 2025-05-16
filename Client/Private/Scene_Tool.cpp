#include "ClientPCH.h"
#include "Scene_Tool.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Tool_Model.h"
#include "Tool_Effect.h"
#include "Camera_Main.h"
#include "GlobalGizmo.h"
#include "UI_Manager.h"

#include "Scene_Test.h"
#include "Scene_Mizuha.h"
#include "Scene_Suoh.h"
#include "Scene_Suoh_Battle.h"
#include "Scene_MindRoom.h"
#include "Scene_Hideout.h"

#if ACTIVATE_TOOL

CScene_Tool::CScene_Tool(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TOOL)
{
}

HRESULT CScene_Tool::Initialize()
{
	CImGui_Manager::Get_Instance()->Enable();
	CGameInstance::Get_Instance()->Show_Cursor();
	CGameInstance::Get_Instance()->Fix_Cursor(false); 

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Ready_Components");
	}
	if (FAILED(Ready_GameObjects()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Ready_GameObjects");
	}
	if (FAILED(Ready_ToolSystems()))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Initialize", "Failed to Ready_ToolSystems");
	}

	LIGHTDESC LightDesc;
	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(.5, .5, .5, 1);
	LightDesc.vDirection = _float3(-1, -1, -1);
	LightDesc.vAmbient = _float4(.1, .1, .1, 1);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::TOOL, LightDesc, nullptr);

//	// POSTPROCESS
//	CGameInstance::Get_Instance()->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
//	CGameInstance::Get_Instance()->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.6f);
//	CGameInstance::Get_Instance()->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.3f);
//
//	// Neon
//	CGameInstance::Get_Instance()->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.145f);
//	CGameInstance::Get_Instance()->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 1.095f);
//	CGameInstance::Get_Instance()->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 1.455f);
//
//	// LUT Filter
//	CGameInstance::Get_Instance()->Enable_LUTFilter(true);
//	CGameInstance::Get_Instance()->Set_LUTIndex(4);
//
//	// SSAO
//	CGameInstance::Get_Instance()->Enable_SSAO(true);
//
//	// Lensflare
//	CGameInstance::Get_Instance()->Enable_Lensflare(true);
//	CGameInstance::Get_Instance()->Set_SunPosition(_float3(2000.f, 10.f, 3000.f));
//	CGameInstance::Get_Instance()->Set_SunColor(_float3(3.810, 0.993f, 0.786f));
//	CGameInstance::Get_Instance()->Set_LensflareStrength(1.517f);
//
//	// DOF
	CGameInstance::Get_Instance()->Enable_DOF(false);
//	CGameInstance::Get_Instance()->Set_DOF_DepthStart(800.f);
//	CGameInstance::Get_Instance()->Set_DOF_DepthRange(1000.f);
//
//	// FXAA
//	CGameInstance::Get_Instance()->Enable_FXAA(true);



	return S_OK;
}

void CScene_Tool::Tick(_float _fTimeDelta)
{
	Tool_MenuBar();

	if (TOOL::MAX != m_eTool)
	{
		Tool_System();
		Tool_Properties();
		Tool_MainControl();
		Tool_SubControl();

		m_vecTools[IDX(m_eTool)]->Tick(_fTimeDelta);
	}

//	ImPlot::ShowDemoWindow();
//	ImPlot::ShowUserGuide();
}

void CScene_Tool::Late_Tick(_float _fTimeDelta)
{
	if (TOOL::MAX != m_eTool)
	{
		m_vecTools[IDX(m_eTool)]->Late_Tick(_fTimeDelta);
	}
}

HRESULT CScene_Tool::Render()
{
	if (TOOL::MAX != m_eTool)
	{
		m_vecTools[IDX(m_eTool)]->Render();
	}

	return S_OK;
}

#ifdef _DEBUG
void CScene_Tool::Debug()
{
#if ACTIVATE_TOOL
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		CUI_Manager::Get_Instance()->Debug_Render();
	}
#endif
}
#endif

HRESULT CScene_Tool::Ready_Components()
{
	m_pRenderer = CGameInstance::Get_Instance()->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_RENDERER_MAIN");
	}

	return S_OK;
}

HRESULT CScene_Tool::Ready_GameObjects()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TOOL, LAYER_STATIC);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}

	m_pCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}
	m_pGizmo = CGameInstance::Get_Instance()->Clone_GameObject<CGlobalGizmo>(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_GLOBALGIZMO);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_GLOBALGIZMO");
	}

	pLayer->Add(m_pCamera);
	pLayer->Add(m_pGizmo);
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TOOL, PROTOTYPE_GAMEOBJECT_SKYCUBE, 
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/DebugCubemap.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Tool::Ready_GameObjects", "Failed to CObjectLayer::Add");
	}

	m_pCamera->Set_DebugMode();

	return S_OK;
}

void CScene_Tool::Tool_MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem("Hide##CScene_Tool::Tool_MenuBar"))
		{
			m_eTool = TOOL::MAX;
		}

		if (ImGui::MenuItem("Model##CScene_Tool::Tool_MenuBar"))
		{
			m_eTool = TOOL::MODEL;
		}

		if (ImGui::MenuItem("Effect##CScene_Tool::Tool_MenuBar"))
		{
			m_eTool = TOOL::EFFECT;
		}

		if (TOOL::MAX != m_eTool)
		{
			m_vecTools[IDX(m_eTool)]->Tool_MenuBar();
		}

		ImGui::EndMainMenuBar();
	}
}

HRESULT CScene_Tool::Ready_ToolSystems()
{
	m_vecTools.emplace_back(CTool_Model::Create(m_pDevice, m_pContext));
	m_vecTools.emplace_back(CTool_Effect::Create(m_pDevice, m_pContext));

	return S_OK;
}

void CScene_Tool::Tool_System()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(SYSTEM_WIDTH, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("System##CScene_Tool::Tool_System", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	m_vecTools[IDX(m_eTool)]->Tool_System();

	ImGui::Separator();

	static _bool	bGizmo(true);
	static _bool	bPicker(false);
	static _int2	vGizmoSize(_int2(10, 10));

	if (ImGui::CollapsingHeader("System##CScene_Tool::Tool_System"))
	{
		if (ImGui::TreeNode("Global Gizmo##CScene_Tool::Tool_System"))
		{
			if (ImGui::Checkbox("Show Gizmo##CScene_Tool##Tool_System", &bGizmo))
			{
				m_pGizmo->Show(bGizmo);
			}
			if (ImGui::InputInt2("Gizmo Size##CScene_Tool::Tool_System", reinterpret_cast<_int*>(&vGizmoSize)))
			{
				vGizmoSize.x = std::clamp(vGizmoSize.x, 1, 100);
				vGizmoSize.y = std::clamp(vGizmoSize.y, 1, 100);
				m_pGizmo->Initialize_Gizmo(vGizmoSize);
			}
			static _color vColor = _color(1.f, 1.f, 1.f, 0.5f);
			if (ImGui::InputFloat4("Gizmo Color##CScene_Tool::Tool_System", reinterpret_cast<_float*>(&vColor)))
			{
				m_pGizmo->Set_Color(vColor);
			}
			static const _char* szSettings = "Blend";
			if (ImGui::BeginCombo("RenderGroup##CTool_Effect::Tool_System", szSettings))
			{
				const _char* items[] = { "Blend", "NonBlend" };

				if (ImGui::Selectable(items[0], szSettings == items[0]))
				{
					szSettings = items[0];
					m_pGizmo->Set_RenderGroup(RENDER_GROUP::BLEND);
				}
				if (ImGui::Selectable(items[1], szSettings == items[1]))
				{
					szSettings = items[1];
					m_pGizmo->Set_RenderGroup(RENDER_GROUP::NONBLEND);
				}

				ImGui::EndCombo();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("PostProcessing##CScene_Tool::Tool_System"))
		{
			static const _char* szSettings = "Default";
			if (ImGui::BeginCombo("PostProcessing##CTool_Effect::Tool_System", szSettings))
			{
				const _char* items[] = { "Test", "Mizuha", "Suoh", "Suoh_Battle", "MindRoom", "Hideout" };

				if (ImGui::Selectable(items[0], szSettings == items[0]))
				{
					szSettings = items[0];
					CScene_Test::Apply_PostProcessing();
				}
				if (ImGui::Selectable(items[1], szSettings == items[1]))
				{
					szSettings = items[1];
					CScene_Mizuha::Apply_PostProcessing();
				}
				if (ImGui::Selectable(items[2], szSettings == items[2]))
				{
					szSettings = items[2];
					CScene_Suoh::Apply_PostProcessing();
				}
				if (ImGui::Selectable(items[3], szSettings == items[3]))
				{
					szSettings = items[3];
					CScene_Suoh_Battle::Apply_PostProcessing();
				}
				if (ImGui::Selectable(items[4], szSettings == items[4]))
				{
					szSettings = items[4];
					CScene_MindRoom::Apply_PostProcessing();
				}
				if (ImGui::Selectable(items[5], szSettings == items[5]))
				{
					szSettings = items[5];
					CScene_Hideout::Apply_PostProcessing();
				}

				ImGui::EndCombo();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Picker##CScene_Tool::Tool_System"))
		{
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void CScene_Tool::Tool_Properties()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - PROPERTY_WIDTH, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SetNextWindowSize(ImVec2(PROPERTY_WIDTH, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));

	ImGui::Begin("Properties##CScene_Tool::Tool_Properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	m_vecTools[IDX(m_eTool)]->Tool_Properties();

	ImGui::End();
}

void CScene_Tool::Tool_MainControl()
{
	ImGui::SetNextWindowPos(ImVec2(SYSTEM_WIDTH, ImGui::GetIO().DisplaySize.y - CONTROL_HEIGHT));
	ImGui::SetNextWindowSize(ImVec2(MAINCONTROL_WIDTH, CONTROL_HEIGHT));

	ImGui::Begin("MainControl##CScene_Tool::Tool_MainControl", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	m_vecTools[IDX(m_eTool)]->Tool_MainControl();

	ImGui::End();
}

void CScene_Tool::Tool_SubControl()
{
	ImGui::SetNextWindowPos(ImVec2(SYSTEM_WIDTH + MAINCONTROL_WIDTH, ImGui::GetIO().DisplaySize.y - CONTROL_HEIGHT));
	ImGui::SetNextWindowSize(ImVec2(SUBCONTROL_WIDTH, CONTROL_HEIGHT));

	ImGui::Begin("SubControl##CScene_Tool::Tool_SubControl", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	m_vecTools[IDX(m_eTool)]->Tool_SubControl();

	ImGui::End();
}

shared_ptr<CScene_Tool> CScene_Tool::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Tool> pInstance = make_private_shared(CScene_Tool, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Tool::Create", "Failed To CScene_Tool::Initialize");
	}

	return pInstance;
}

#endif
