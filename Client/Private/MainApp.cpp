#include "ClientPCH.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Effect_Manager.h"                                                                                                                    
#include "Scene_Load.h"
#include "SimulationEventCallBack.h"
#include "PhysXEngine.h"
#include "Map_ToolManager.h"
#include "UI_Manager.h"
#include "ModelRenderer.h"

#include "CameraKeyframeGenerator.h"
#include "ComputeAccelerator.h" 
#include "ScreenEffectManager.h"

#define START_SCENE		SCENE::TITLE

#ifdef _DEBUG 
#define MUTE_SFX		FALSE
#define GAME_STATUS		TRUE
#endif

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())	
#if ACTIVATE_IMGUI
	, m_pImGui_Manager(CImGui_Manager::Get_Instance())
#endif
{
}

CMainApp::~CMainApp()
{ 
	CEffect_Manager::Destroy_Instance();
	CScreenEffectManager::Destroy_Instance();
	CGameInstance::Release_Engine(); 
	CUI_Manager::Destroy_Instance();
	CMainApp::Destroy_Instance();
#if ACTIVATE_IMGUI
	CImGui_Manager::Destroy_Instance();
	CMap_ToolManager::Destroy_Instance();
#endif
#ifdef D3D11_LIVE_OBJECT_REF_COUNTER_CHECKER
#if defined(DEBUG) || defined(_DEBUG)
	if (nullptr != m_pDevice)
	{
		ID3D11Debug* d3dDebug;
		HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		}
		if (d3dDebug != nullptr)
		{
			d3dDebug->Release();
		}
	}
#endif
#endif
	delete pSimulationEventCallback;
	pSimulationEventCallback = nullptr;
}

HRESULT CMainApp::Initialize()
{
	if (!m_pGameInstance)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Null Exception: m_pGameInstance");
	}

	GRAPHICDESC tGraphicDesc{};

	_matrix Matrix = XMMatrixRotationY(XMConvertToRadians(90.f));

	tGraphicDesc.eWinMode = GRAPHICDESC::WINDOW;
	tGraphicDesc.hWnd = g_hWnd;
	tGraphicDesc.iWinCX = g_iWinCX;
	tGraphicDesc.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(m_pDevice, m_pContext, tGraphicDesc, SCENE::STATIC, SCENE::MAX, COLLISION_GROUP::MAX, SOUND_CHANNEL::MAX)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: m_pGameInstance->Initialize_Engine");
	}

	if (FAILED(CComputeAccelerator::Get_Instance()->Initialize(m_pDevice, m_pContext,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/ComputeShader.hlsl"), nullptr, 0))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: CComputeAccelerator::Initialize");
	}

	if (FAILED(Default_Settings()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Default_Settings");
	}

	if (FAILED(Ready_Component_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Ready_Component_Prototype");
	}

#if	ACTIVATE_IMGUI
	if (FAILED(m_pImGui_Manager->Initialize(g_hWnd, m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: CImGui_Manager::Initialize");
	}
#endif
	if (FAILED(CEffect_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to CEffect_Manager::Initialize");
	}

	if (FAILED(CScreenEffectManager::Get_Instance()->Initialize(m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to CScreenEffectManager::Initialize");
	}

	if (FAILED(m_pGameInstance->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, START_SCENE), false)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Open_Scene");
	}

	pSimulationEventCallback = new CSimulationEventCallBack;

	m_pGameInstance->Get_PhysXEngine()->GetScene()->setSimulationEventCallback(pSimulationEventCallback);


	CUI_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext);

	m_strFPS.resize(MAX_PATH);
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_ScarletStrings"), TEXT("Bin/Resources/Texture/ScarletStrings.spritefont"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Add_Font");
	}
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_OutLine"), TEXT("Bin/Resources/Texture/OutLine.spritefont"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp:Initialize", "Failed to Add_Font");
	}
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_E_OutLine"), TEXT("Bin/Resources/Texture/Eng_OutLine.spritefont"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp:Initialize", "Failed to Add_Font");
	}
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Nexon"), TEXT("Bin/Resources/Texture/136ex.spritefont"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp:Initialize", "Failed to Add_Font");
	}
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Godo"), TEXT("Bin/Resources/Texture/Godo.spritefont"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp:Initialize", "Failed to Add_Font");
	}

	CMap_ToolManager::Get_Instance()->Set_Hwnd(g_hWnd);

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->SetRenderer()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Physx RendererSetting");
	}
#if MUTE_SFX
	m_pGameInstance->Mute();
#endif
#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(_float _fTimeDelta)
{
#if ACTIVATE_IMGUI
	if (m_pImGui_Manager)
	{
		m_pImGui_Manager->Tick();
	}
	if (m_pImGui_Manager->Is_Enable())
	{
#if GAME_STATUS
		static _bool bGameStatus = true;
		ImGui_GameStatus(_fTimeDelta, &bGameStatus);
#endif
	}
#endif
	CEffect_Manager::Get_Instance()->Tick(_fTimeDelta);
	CScreenEffectManager::Get_Instance()->Tick(_fTimeDelta);
	CUI_Manager::Get_Instance()->Tick(_fTimeDelta);
	CMap_ToolManager::Get_Instance()->Tick(_fTimeDelta);
	CEffect_Manager::Get_Instance()->Late_Tick(_fTimeDelta);
	CScreenEffectManager::Get_Instance()->Late_Tick(_fTimeDelta);
	CUI_Manager::Get_Instance()->Late_Tick(_fTimeDelta);
	CMap_ToolManager::Get_Instance()->Late_Tick(_fTimeDelta);
	if (m_pGameInstance)
	{
		m_pGameInstance->Tick_Engine(_fTimeDelta);
	}
	m_fTimeAcc += _fTimeDelta;

	//	static shared_ptr<ISystem> sys;
	//	vector<int> a, b;
	//	for (int i = 0; i < 100; i++) a.emplace_back(i);
	//	b.resize(100);
	////CComputeAccelerator::Get_Instance()->Register_ShaderResourceView(sys, TEXT("a"), sizeof(int), 100);
	//	CComputeAccelerator::Get_Instance()->Register_UnorderedAccessView(sys, TEXT("a"), sizeof(int), 100);
	//	CComputeAccelerator::Get_Instance()->Update_Data(sys, TEXT("a"), "g_intInput", "g_intOutput", a.data(), sizeof(int) * 100);
	//	CComputeAccelerator::Get_Instance()->Compute(0, 10, 1, 1);
	//	CComputeAccelerator::Get_Instance()->Receive_Data(sys, TEXT("a"), b.data(), sizeof(int) * 100);
	//	int c = 1;

	//	vector<int> a(2000);
	//	a.resize(2000);
	//	vector<int> b(2000);
	//	b.resize(2000);
	//	CComputeAccelerator::Get_Instance()->Register_ShaderResourceView(nullptr, TEXT("i"), sizeof(int), 2000);
	//	CComputeAccelerator::Get_Instance()->Register_UnorderedAccessView(nullptr, TEXT("o"), sizeof(int), 2000);
	//
	//	CComputeAccelerator::Get_Instance()->Update_Data(nullptr, TEXT("i"), TEXT("o"), "g_inputInt", "g_outputInt", a.data());
	//	CComputeAccelerator::Get_Instance()->Compute(1, 1, 1, 1);
	//	CComputeAccelerator::Get_Instance()->Receive_Data(nullptr, TEXT("o"), b.data());
	//
	//	int c = 1;
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pMainRenderer)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Render", "Null Exception: m_pMainRenderer");
	}

	m_pGameInstance->Clear_BackBuffer_View(BACK_BUFFER_COLOR);
	m_pGameInstance->Clear_DepthStencil_View();

	//m_pMainModelRenderer->Update_Instance();
	m_pMainRenderer->Draw_RenderGroup();

#ifdef _DEBUG
	m_pGameInstance->Debug_Engine();

	//++m_iRenderCnt;

	//if (m_fTimeAcc >= 1.f)
	//{
	//	wsprintf(&m_strFPS.front(), TEXT("FPS : %d"), m_iRenderCnt);

	//	m_iRenderCnt = 0;

	//	m_fTimeAcc = 0.f;
	//}

	//m_pGameInstance->Render_Font(TEXT("Font_ScarletStrings"), m_strFPS, _float2(1150.f, 0.f), XMVectorReplicate(1.f), 0.6f);

#if ACTIVATE_IMGUI
	m_pImGui_Manager->Render();
#endif
#endif
	m_pGameInstance->Present();

	return S_OK;
}

#pragma region Windows Management

LRESULT CMainApp::WndProcHandler(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->WndProcHandler(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}

#pragma endregion
#pragma region Timer Management

void CMainApp::Tick_Timer()
{
	if (m_pGameInstance)
	{
		m_pGameInstance->Tick_Timer();
	}
}

_bool CMainApp::Check_Timer(const _float _fFPS)
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->Check_Timer(_fFPS);
	}

	//m_pGameInstance->Add_Timer(1 / 3.f);

	//if (m_pGameInstance->Check_Timer(1 / 3.f))

	return false;
}

_float CMainApp::Get_TimeDelta(const _float _fFPS) const
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->Get_TimeDelta(_fFPS);
	}

	return 0.f;
}

#pragma endregion

HRESULT CMainApp::Default_Settings()
{
	if (FAILED(m_pGameInstance->Add_Timer(DEFAULT_FPS)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Default_Settings", "Failed To Add Timer");
	}

	if (FAILED(m_pGameInstance->Set_Default_Timer(DEFAULT_FPS)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Default_Settings", "Failed To Set_Default_Timer");
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
#pragma region Static Components
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER, m_pMainModelRenderer = CModelRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN, m_pMainRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_pMainRenderer->Bind_ModelRenderer(m_pMainModelRenderer);

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_KEYFRAME,
		CKeyframe::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_03,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_03");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_022,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_one_lin_022.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_022");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_012,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_scl_noi_012.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_012");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_010,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_scl_noi_010.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_010");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_050,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_scl_noi_050.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_050");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_UI_1,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ui_noise_1.png"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_UI_1");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_002,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_ev_scl_noi_002.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_002");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_NEON_CRASH,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_pr_neon_crash1_3.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_NEON_CRASH");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ef_one_noi_002.dds"), 1, true))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_MASK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Drive/T_ef_scl_noi_013_02.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_NOISE_MASK");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_GLITCH_PATTERN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_glitch_pattern.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_GLITCH_PATTERN");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_AURA_005,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Aura/T_ef_scl_aur_005.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_AURA_005");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_DECAL_EM_APP_START,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Decal/T_ef_unq_em_app_start.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_DECAL_EM_APP_START");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_DECAL_CLS_LIN_006,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Decal/T_ef_scl_lin_006.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_DECAL_CLS_LIN_006");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_DECAL_EV_ONE_CIR_001,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Decal/T_ef_ev_one_cir_001.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_DECAL_EV_ONE_CIR_001");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_DECAL_CLS_NOI_048,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Decal/T_ef_scl_noi_048.dds"), 1))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_DECAL_CLS_NOI_048");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/CircularMask.dds")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_MASK_LIN_010,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/T_ef_one_lin_010.dds")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_MASK_LIN_010");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_HPLOSS,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Screen/T_HPloss_eff.dds")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_HPLOSS");
	}

#pragma endregion
#pragma region Static VIBuffers

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_RECT,
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_RECT");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_CUBE,
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_CUBE");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT,
		CVIBufferInstance_Point::Create(m_pDevice, m_pContext, 10000))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT");
	}
	//if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE,
	//	CVIBufferInstance_Line::Create(m_pDevice, m_pContext, 100))))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE");
	//}
#if ACTIVATE_TOOL
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT,
		CVIBufferInstance_Rect::Create(m_pDevice, m_pContext, 10000))))
#else
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TOOL, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT,
		CVIBufferInstance_Rect::Create(m_pDevice, m_pContext, 100))))
#endif
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_ToolScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

#pragma endregion
#pragma region Static Shaders
	shared_ptr<CShader> pShader;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOS,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/Pos.hlsl"), VTXPOS::tElements, VTXPOS::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOS");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXCUBEPOSTEX");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Bind_Matrix: SHADER_MATORTHOGRAPHIC");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_2,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_2.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_2");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Bind_Matrix: SHADER_MATORTHOGRAPHIC");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Bind_Matrix: SHADER_MATORTHOGRAPHIC");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTexVfx.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Bind_Matrix: SHADER_MATORTHOGRAPHIC");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Loading.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING");
	}
	if (FAILED(pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Bind_Matrix: SHADER_MATORTHOGRAPHIC");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Cutscene.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXCLOUDPOSTEX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Cloud.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MizuhaScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXCLOUDPOSTEX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX_DECAL,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTex_Decal.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_MizuhaScene", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX_DECAL");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXSKYBOXPOSTEX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/SkyboxPosTex.hlsl"), VTXCUBEPOSTEX::tElements, VTXCUBEPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXSKYBOXPOSTEX");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstMesh.hlsl"), VTXPOSNORTANTEXINSTTRANSCOLORARG::tElements, VTXPOSNORTANTEXINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_SPHERE,
		CVIBuffer_Sphere::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_CUBE");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_SPHERE_HIGHRES,
		CVIBuffer_Sphere::Create(m_pDevice, m_pContext, 1024))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_CUBE");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTex.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTex.hlsl"), VTXMESH::tElements, VTXMESH::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESHVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTexVfx.hlsl"), VTXMESH::tElements, VTXMESH::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEXVFX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTexBone.hlsl"), VTXMESHANIM::tElements, VTXMESHANIM::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstPointVfx.hlsl"), VTXPOSSIZEINSTTRANSCOLORARG::tElements, VTXPOSSIZEINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINEVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstLineVfx.hlsl"), VTXPOSSIZEINSTTRANSCOLORARG::tElements, VTXPOSSIZEINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINEVFX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstRect.hlsl"), VTXPOSTEXINSTTRANSARG2::tElements, VTXPOSTEXINSTTRANSARG2::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECTVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstRectVfx.hlsl"), VTXPOSTEXINSTTRANSARG2::tElements, VTXPOSTEXINSTTRANSARG2::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECTVFX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstMeshVfx.hlsl"), VTXMESHINSTTRANSCOLORARG::tElements, VTXMESHINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHANIMVFX,
		CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstMeshAnimVfx.hlsl"), VTXMESHANIMINSTTRANSCOLORARG::tElements, VTXMESHANIMINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX");
	}
#pragma endregion

	return S_OK;
}

#if ACTIVATE_IMGUI
struct PlotBuffer
{
	void AddPoint(_float x, _float y)
	{
		if (m_vecData.size() < m_iSize)
		{
			m_vecData.push_back(ImVec2(x, y));
		}
		else
		{
			m_vecData[m_iIndex] = ImVec2(x, y);
			m_iIndex = (m_iIndex + 1) % m_iSize;
		}
	}
	_int				m_iSize = 2000;
	_int				m_iIndex = 0;
	ImVector<ImVec2>	m_vecData;
};
void CMainApp::ImGui_GameStatus(_float _fTimeDelta, _bool* _pOpen)
{
	static _float		fFPS(DEFAULT_FPS);
	static _float		fPlotFPS(DEFAULT_FPS);
	static ImVec2		vFPSScope(30.f, DEFAULT_FPS);
	static _float		fTimer(0.f);
	static _float		fHistory(5.f);
	static PlotBuffer	tBuffer;
	static _bool		bGraph(true);

	fTimer += _fTimeDelta;
	fPlotFPS = 1.f / _fTimeDelta;
	if (fTimer > 0.05f)
	{
		fFPS = fPlotFPS;
		fTimer -= 0.05f;
	}
	tBuffer.AddPoint(m_fTimeAcc, fPlotFPS);

	static int location = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (location >= 0)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	else if (location == -2)
	{
		// Center window
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	if (ImGui::Begin("Game Status", _pOpen, window_flags))
	{
		ImGui::Text("Game Status");
		ImGui::Separator();

		ImGui::Text("FPS: %.0f", fFPS);

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, location == -1))	location = -1;
			if (ImGui::MenuItem("Center", NULL, location == -2))	location = -2;
			if (ImGui::MenuItem("Top-left", NULL, location == 0))	location = 0;
			if (ImGui::MenuItem("Top-right", NULL, location == 1))	location = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, location == 2))	location = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, location == 3))	location = 3;
			ImGui::Separator();
			if (ImGui::BeginMenu("Graph"))
			{
				if (ImGui::MenuItem("Graph##FPS_Graph", NULL, bGraph))	bGraph = !bGraph;
				ImGui::SliderFloat("History", &fHistory, 1, 5, "%.1f s");
				ImGui::SliderFloat2("Scope", &vFPSScope.x, 0, DEFAULT_FPS);
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (_pOpen && ImGui::MenuItem("Close")) *_pOpen = false;
			ImGui::EndPopup();
		}

		if (bGraph && ImPlot::BeginPlot("##FPS_Plot", ImVec2(-1.f, 100.f), ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoFrame))
		{
			ImPlot::SetupAxes(nullptr, nullptr,
				ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickLabels,
				ImPlotAxisFlags_NoInitialFit | ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_Opposite);
			ImPlot::SetupAxisLimits(ImAxis_X1, m_fTimeAcc - fHistory, m_fTimeAcc, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, vFPSScope[0], vFPSScope[1]);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotLine("FPS", &tBuffer.m_vecData[0].x, &tBuffer.m_vecData[0].y, tBuffer.m_vecData.size(), 0, tBuffer.m_iIndex, 2 * sizeof(float));
			ImPlot::EndPlot();
		}
	}
	ImGui::End();
}
#endif