#include "ClientPCH.h"
#include "Scene_Test.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Player.h"
#include "Light.h"
#include "PhysXEngine.h"
#include "Map_ToolManager.h"
#include "Wave.h"
#include "UI_Manager.h"
#include "Scene_Load.h"
#include "BattleField.h"
#include "BattleField_Manager.h"
#include "Event_Manager.h"
#include "EventCollider.h"
#include "ScreenEffectManager.h"

#include "UI_Psyco_Wave.h"
#include "UI_Indicator_ToolTip_Tutorial_PMCC.h"
#include "UI_Indicator_ToolTip_Tutorial_LockOn.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack.h"
#include "UI_Indicator_ToolTip_Tutorial_Success.h"
#include "UI_Indicator_BrainTalk_Tutorial.h"
#include "UI_Indicator_ToolTip_Tutorial_Tips.h"
#include "MenuCharacter.h"

#define ACTIVATE_SHADOW TRUE

CScene_Test::CScene_Test(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TEST)
{
}

HRESULT CScene_Test::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Show_Cursor(true);
	pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif
	
	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to CScene::Initialize");
	}

#ifdef _DEBUG
	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	m_pModelRenderer = pGameInstance->Clone_Component<CModelRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER);
#endif

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_BackGround");
	}
	if (FAILED(Ready_Monster()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_TestObject");
	}
	if (FAILED(Ready_TestUI()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestUI");
	}
	
	if (FAILED(Ready_TestMap()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestMap");
	}

	if (FAILED(Ready_TestUI_Late()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestUI_Late");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestUI_Late");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_Character");
	}

	if (FAILED(Ready_BattleField()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_BattleField");
	}

	if (FAILED(Ready_EventCollider()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_EventCollider");
	}

	// TODO: tEsT cOdE: rEmOvE!@#!#!@#!@
	{
		//shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, TEXT("Layer_TestDecal"));
		//if (nullptr == pLayer)
		//{
		//	MSG_RETURN(E_FAIL, "CScene_Test::tEsTcOdE", "Failed to Add_Layer: Layer_TestDecal");
		//}

		//auto pTestDecal = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK);
		//if (FAILED(pLayer->Add(pTestDecal)))
		//{
		//	MSG_RETURN(E_FAIL, "CScene_Test::tEsTcOdE", "Failed to CObjectLayer::Add");
		//}
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveObject(SCENE::TEST, LAYER_MAP_DYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}

	CGameInstance::Get_Instance()->Fix_Cursor(false);

	pGameInstance->PlayBGM(L"TutorialMapBGM.wav", 0.5f);
	pGameInstance->PlaySoundW(L"Tutorial_Start.wav", 0.5f);

	return S_OK;
}

void CScene_Test::Tick(_float _fTimeDelta)
{
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();

	CBattleField_Manager::Get_Instance()->Debug_Render();

#endif
	auto pGameInstance = CGameInstance::Get_Instance();
	if (false == m_bPMCCSuccess)
	{
		if (pGameInstance->Key_Down('W') || pGameInstance->Key_Down('A') || pGameInstance->Key_Down('S') || pGameInstance->Key_Down('D'))
		{
			m_bPMCCSuccess = true;

			auto pSuccessIndicator = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Success();
			pSuccessIndicator->Set_Success_PMCC();
		}
	}
	if (false == m_bLockOnSuccess)
	{
		if (pGameInstance->Key_Down('Q'))
		{
			m_bLockOnSuccess = true;

			auto pSuccessIndicator = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Success();
			pSuccessIndicator->Set_Success_LockOn();
		}
	}
	if (false == m_bBattleStyleSuccess)
	{
		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			m_bBattleStyleSuccess = true;

			auto pSuccessIndicator = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Success();
			pSuccessIndicator->Set_Success_Kasane_BattleStyle();
		}
	}
	if (false == m_bSpecialAttackSuccess)
	{
		if (pGameInstance->Key_Down('C'))
		{
			m_bSpecialAttackSuccess = true;

			auto pSuccessIndicator = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Success();
			pSuccessIndicator->Set_Success_Kasane_SpecialAttack();
		}
	}

	m_pWave->Tick();
#ifdef TEST_CODE
	//if (CGameInstance::Get_Instance()->Key_Down('1'))
	//{
	//	shared_ptr<CTransform> pTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	//	_float4x4 mOriginal = pTransform->Get_Matrix();
	//	pTransform->Rotate(TRANSFORM::RIGHT, XM_PIDIV2);
	//	pTransform->Set_Scale(_float3(10.f, 10.f, 10.f));
	//	pTransform->Translate(_float3(0.f, 0.1f, 0.f));
	//	_float4x4 mEffect = pTransform->Get_Matrix();
	//	pTransform->Set_Matrix(mOriginal);
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::KINESIS_DIFFUSE, mEffect);
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::KINESIS_DISTORTION, mEffect);
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::KINESIS_PARTICLE, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION));
	//}
//	if (CGameInstance::Get_Instance()->Key_Down('2'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::POPCORN,			Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION) + _float3(0.f, 2.f, 0.f));
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('3'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::SPLASH_BILLBOARD,	Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION) + _float3(0.f, 2.f, 0.f));
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('4'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::TESTTRAILTLD,		make_pair(Function::Find_Player(), "RightForeArm"));
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('5'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::TESTTRAILTQD,		make_tuple(Function::Find_Player(), "RightForeArm", "RightHandHelp_end"));
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('6'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::TESTMESHAFTERIMAGE,	Function::Find_Player());
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('7'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::TESTPARTICLETARGET,	Function::Find_Player());
//	}
#endif
}

void CScene_Test::Late_Tick(_float _fTimeDelta)
{
	// TODO: TestCode
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::SUOH));
		CUI_Manager::Get_Instance()->Set_TutorialRender(false);
		//CGameInstance::Get_Instance()->Layer
	}
}

HRESULT CScene_Test::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Test::Debug()
{
#if ACTIVATE_IMGUI
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		static LIGHTDESC tLightDesc;
		static SHADOWDESC tShadowDesc;
		
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
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_SSAO).Get(), ImVec2(480, 270));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("MASK"))
		{
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS).Get(), ImVec2(200, 200));
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MASK).Get(), ImVec2(200, 200));
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_BLOOM).Get(), ImVec2(200, 200));
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS_COMBINE).Get(), ImVec2(200, 200));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("UI_3D"))
		{
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_UI_3D).Get(), ImVec2(480, 270));
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_UI_3D_CHARACTER).Get(), ImVec2(480, 270));
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
		}

		/*if (ImGui::BeginMenu("Shadow"))
		{
		if (ImGui::BeginMenu("Shadow"))
		{
			ImGui::Text("Settings");

			CGameInstance::Get_Instance()->Iterate_Shadows(
				[&](shared_ptr<CLight> pLight)
				{
					tLightDesc = pLight->Get_LightDesc();
					tShadowDesc.fBias = pLight->Get_ShadowDesc().fBias;
					tShadowDesc.fShadowShade = pLight->Get_ShadowDesc().fShadowShade;
					tShadowDesc.vGridSize = pLight->Get_ShadowDesc().vGridSize;
					tShadowDesc.fMinShadowAvg = pLight->Get_ShadowDesc().fMinShadowAvg;
					tShadowDesc.fTexelScale = pLight->Get_ShadowDesc().fTexelScale;

					ImGui::Image(CGameInstance::Get_Instance()->Get_Shadow(pLight).Get(), ImVec2(200, 200));
					if (ImGui::InputFloat3("Direction", reinterpret_cast<_float*>(&tLightDesc.vDirection)))
					{
						pLight->Set_LightDesc(tLightDesc);
					}
					if (ImGui::InputFloat3("Position", reinterpret_cast<_float*>(&tLightDesc.vPosition)))
					{
						pLight->Set_LightDesc(tLightDesc);
					}
					if (ImGui::InputFloat2("Projection", reinterpret_cast<_float*>(&tLightDesc.vShadowProjection)))
					{
						pLight->Set_LightDesc(tLightDesc);
					}
					if (ImGui::InputFloat("Range", reinterpret_cast<_float*>(&tLightDesc.fRange)))
					{
						pLight->Set_LightDesc(tLightDesc);
					}
					ImGui::Separator();
					if (ImGui::InputFloat("Bias", &tShadowDesc.fBias))
					{
						pLight->Set_ShadowDesc(tShadowDesc);
					}
					if (ImGui::InputFloat("Shadow Shade", &tShadowDesc.fShadowShade))
					{
						pLight->Set_ShadowDesc(tShadowDesc);
					}
					if (ImGui::InputFloat("Min Shadow Avg", &tShadowDesc.fMinShadowAvg))
					{
						pLight->Set_ShadowDesc(tShadowDesc);
					}
					if (ImGui::InputFloat("Texel Scale", &tShadowDesc.fTexelScale))
					{
						pLight->Set_ShadowDesc(tShadowDesc);
					}
					if (ImGui::InputInt2("Grid Size", reinterpret_cast<_int*>(&tShadowDesc.vGridSize)))
					{
						pLight->Set_ShadowDesc(tShadowDesc);
					}
				}
			);

			ImGui::EndMenu();
		}
		ImGui::EndMenu();
		}*/
		ImGui::EndMainMenuBar();
		}

		// Material Settings
		/*ImGui::Begin("Material Settings");
		m_pModelRenderer->Iterate_Models([](wstring wstrModelTag, shared_ptr<CModel> pModel) -> _bool
			{
				int size = WideCharToMultiByte(CP_UTF8, 0, wstrModelTag.c_str(), -1, nullptr, 0, nullptr, nullptr);
				std::string strModelTag(size, 0);
				WideCharToMultiByte(CP_UTF8, 0, wstrModelTag.c_str(), -1, &strModelTag[0], size, nullptr, nullptr);

				if (ImGui::CollapsingHeader(strModelTag.c_str()))
				{
					_uint iNumMesh = pModel->Get_NumMeshes();
					for (_uint i = 0; i < iNumMesh; ++i)
					{
						string MeshName = pModel->Get_MeshName(i);
						if (ImGui::TreeNode(MeshName.c_str()))
						{
							SHADERDESC tShaderDesc = pModel->Get_ShaderDesc(i);
							_bool bDirtyFlag = false;
							if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
								bDirtyFlag = true;
							if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
								bDirtyFlag = true;
							if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
								bDirtyFlag = true;
							if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
								bDirtyFlag = true;
							if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
								bDirtyFlag = true;

							if (bDirtyFlag)
							{
								pModel->Set_ShaderDesc(i, tShaderDesc);
							}

							ImGui::TreePop();
						}
					}

					if (ImGui::Button("Save"))
					{
						pModel->Export_ShaderMaterial();
					}
					if (ImGui::Button("Load"))
					{
						
					}
				}

				return true;
			});

		ImGui::End();*/
	}
#endif
}
#endif

HRESULT CScene_Test::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.0f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.7f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.3f);

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.111f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 0.5f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 1.5f);

	// SSAO
	pGameInstance->Enable_SSAO(true);

	// LUT Filter
	pGameInstance->Enable_LUTFilter(true);
	pGameInstance->Set_LUTIndex(4);

	// Lensflare
	pGameInstance->Enable_Lensflare(true);
	pGameInstance->Set_SunPosition(_float3(2000.f, 10.f, 3000.f));
	pGameInstance->Set_SunColor(_float3(1.4f, 1.2f, 1.0f));
	pGameInstance->Set_LensflareStrength(2.0f);

	// DOF
	pGameInstance->Enable_DOF(false);
	pGameInstance->Set_DOF_DepthStart(30.f);
	pGameInstance->Set_DOF_DepthRange(160.f);

	pGameInstance->Enable_FXAA(true);

	return S_OK;
}

HRESULT CScene_Test::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Player> pPlayerCam = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	pPlayerCam->Set_DebugMode(false);
#endif
	/*
	shared_ptr<CCamera_Main> pMainCam = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN);
	if (FAILED(pLayer->Add(pMainCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to CObjectLayer::Add");
	}

	pMainCam->Set_DebugMode();
	*/
	return S_OK;
}

HRESULT CScene_Test::Ready_Lights()
{
//	CGameInstance::Get_Instance()->Enable_SSAO(false);

	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .5;
	LightDesc.vDirection = (_float3(1.f, -1.f, -0.6f)).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .5;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, LightDesc, nullptr);
 
#if ACTIVATE_SHADOW
	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(700.f, 700.f);
	LightDesc.vPosition = _float3(-65.f, 65.f, 300.f);
	LightDesc.vDirection = _float3(1.f, -1.f, -0.6f);
	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, LightDesc, nullptr);
	pShadow->Register_Layer(SCENE::TEST, LAYER_PLAYER);
	pShadow->Register_Layer(SCENE::TEST, LAYER_MONSTER);
	pShadow->Set_ShadowDesc(SHADOWDESC{ 1.f, 0.2f, 0.f, 0.05f, _int2(8, 8) });
#endif

	//LightDesc.eLightType = LIGHTTYPE::SHADOW;
	//LightDesc.eShadowType = SHADOWTYPE::DYNAMIC;
	//LightDesc.vShadowProjection = _float2(10.f, 10.f);
	//LightDesc.vDirection = _float3(1.f, -1.f, 0.6f);
	//LightDesc.fRange = 10.f;
	//pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, LightDesc, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
	//pShadow->Register_Layer(SCENE::TEST, LAYER_PLAYER);
  
	//LightDesc.eLightType = LIGHTTYPE::SHADOW;
	//LightDesc.eShadowType = SHADOWTYPE::DYNAMIC;
	//LightDesc.vShadowProjection = _float2(10.f, 10.f);
	//LightDesc.vDirection = _float3(1.f, -1.f, 0.6f);
	//LightDesc.fRange = 10.f;
	//pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, LightDesc, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
	//pShadow->Register_Layer(SCENE::TEST, LAYER_PLAYER);

	return S_OK;
}

HRESULT CScene_Test::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_SKYCUBE, 
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/ReflectionCapture.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_PLAYER);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::TEST))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Pool(SCENE::TEST, POOLOBJECT_DECAL_HITCRACK, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK, 50);

	/*if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TESTROAD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
	}*/

	return S_OK;
}

HRESULT CScene_Test::Ready_Monster()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	
	shared_ptr<CObjectLayer> pMonster_ProjectileLayer = pGameInstance->Add_Layer(SCENE::TEST, LAYER_MONSTER_PROJECTILE);
	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Add_Layer(SCENE::TEST, LAYER_MONSTER);
	if (nullptr == pMonsterLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}

	pGameInstance->Add_Pool(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, 10);
	pGameInstance->Add_Pool(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, 10);
	pGameInstance->Add_Pool(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, 10);
	pGameInstance->Add_Pool(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, 10);
	pGameInstance->Add_Pool(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS, PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS, 1);
	
	m_pWave = CWave::Create();


	m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(44.690f, 12.84f, 117.f, -90.f));

	m_pWave->AddWave(1, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(42.690f, 12.84f, 112.f, 0.f));
	m_pWave->AddWave(1, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(45.260f, 12.84f, 123.427f, 180.f));

	m_pWave->AddWave(2, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(117.215f, 0.02f, 129.390f, -90.f));
	m_pWave->AddWave(2, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(118.018f, 0.02f, 122.132f, -90.f));
	m_pWave->Subscribe_WaveInitCallback(2, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 0, false);
		});


	m_pWave->AddWave(3, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(107.067f, 0.02f + 1.5f, 158.274f, 180.f));
	m_pWave->AddWave(3, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(117.589f, 0.02f + 1.5f, 158.275f, 180.f));
	m_pWave->Subscribe_WaveInitCallback(3, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 1, false);
		});

	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(115.f, -1.84f, 186.46f, 180.f));
	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(120.883f, -2.f, 187.46f, 180.f));
	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(115.f, -4.f + 1.5f, 203.38f, 180.f));
	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(119.f, -4.1f + 1.5f, 204.38f, 180.f));
	m_pWave->Subscribe_WaveInitCallback(4, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 2, false);
		});

	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(106.5f, -5.61f, 227.82f, 90.f));
	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(115.680f, -5.135f, 214.34f, 0.f));
	m_pWave->Subscribe_WaveInitCallback(5, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 3, false);
		});

	m_pWave->AddWave(6, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(105.624f, -5.836f, 234.0f, 180.f));
	m_pWave->AddWave(6, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(115.680f, -5.135f, 214.34f, 0.f));
	m_pWave->AddWave(6, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, _float4(121.934f, -5.745f, 231.34f, -90.f));

	m_pWave->AddWave(7, PROTOTYPE_GAMEOBJECT_MONSTER_TUTOBOSS, _float4(121.934f, -5.745f, 231.34f, 180.f));
	m_pWave->Subscribe_WaveInitCallback(7, []()
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Tutorial();
			//auto pPsycoWave = CUI_Manager::Get_Instance()->Get_Psyco_Wave();

			pBrainTalk->Set_BrainTalk(4);
			//pPsycoWave->Set_Psyco_Disonance();
		});
	
	return S_OK;
}

HRESULT CScene_Test::Ready_TestUI()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Add_Layer: LAYER_UI");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_HP))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_LOCKON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_BATTLESTYLE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_KASANE_SPECIALATTACK))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SUCCESS))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}


	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_UI_3D);
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_TestUI_Late()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_INTERACTIVE_LATE);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC_LATE");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON))))
	{ 
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_NOTICEMARK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}

	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_TUTORIAL))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	////if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT))))
	////{
	////	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	////}
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, 50);
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, 5);

	return S_OK;
}

HRESULT CScene_Test::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKasane = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE, true);
	if (FAILED(pLayer->Add(pKasane)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);
	pUIManager->Bind_Character(CHARACTER::KASANE, pKasane);

	return S_OK;
}

HRESULT CScene_Test::Ready_BattleField()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_BATTLEFIELD);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Add_Layer: LAYER_BATTLEFIELD");
	}

	auto pBattleField0 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Tutorial_0.btf")));
	if (FAILED(pLayer->Add(pBattleField0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField1 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Tutorial_1.btf")));
	if (FAILED(pLayer->Add(pBattleField1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField2 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Tutorial_2.btf")));
	if (FAILED(pLayer->Add(pBattleField2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField3 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Tutorial_3.btf")));
	if (FAILED(pLayer->Add(pBattleField3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField4 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Tutorial_4.btf")));
	if (FAILED(pLayer->Add(pBattleField4)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}

	auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
	pBattleField_Manager->Bind_BattleField(SCENE::TEST, dynamic_pointer_cast<CBattleField>(pBattleField0));
	pBattleField_Manager->Bind_BattleField(SCENE::TEST, dynamic_pointer_cast<CBattleField>(pBattleField1));
	pBattleField_Manager->Bind_BattleField(SCENE::TEST, dynamic_pointer_cast<CBattleField>(pBattleField2));
	pBattleField_Manager->Bind_BattleField(SCENE::TEST, dynamic_pointer_cast<CBattleField>(pBattleField3));
	pBattleField_Manager->Bind_BattleField(SCENE::TEST, dynamic_pointer_cast<CBattleField>(pBattleField4));

	return S_OK;
}

HRESULT CScene_Test::Ready_EventCollider()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_EVENT_COLLIDER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BattleField", "Failed to Add_Layer: LAYER_EVENT_COLLIDER");
	}

	// Load Battle Field
#pragma region LoadBattleField 
	auto pBattleFieldEvent0 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BattleField_0.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent1 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BattleField_1.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent2 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BattleField_2.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent3 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BattleField_3.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent4 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BattleField_4.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent4)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion

	// Subscribe Battle Field Event
#pragma region Subscribe Battle Field Event
	auto pEvent_Manager = CEvent_Manager::Get_Instance();
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBattleFieldEvent0);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBattleFieldEvent0->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 0, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBattleFieldEvent1);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBattleFieldEvent1->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 1, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBattleFieldEvent2);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBattleFieldEvent2->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 2, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBattleFieldEvent3);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBattleFieldEvent3->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 3, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBattleFieldEvent4);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBattleFieldEvent4->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::TEST, 4, true);
		});

#pragma endregion

	// Tutorial Event
#pragma region Load Tips Event
	auto pTipsPMCCEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_Tips_PMCC.evt"))));
	if (FAILED(pLayer->Add(pTipsPMCCEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pTipsLockOnEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_Tips_LockOn.evt"))));
	if (FAILED(pLayer->Add(pTipsLockOnEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pTipsBattleStyleEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_Tips_BattleStyle.evt"))));
	if (FAILED(pLayer->Add(pTipsBattleStyleEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pTipsSpecialAttackEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_Tips_SpecialAttack.evt"))));
	if (FAILED(pLayer->Add(pTipsSpecialAttackEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}

	// Tool Tips
	auto pToolTipJumpEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_ToolTips_Jump.evt"))));
	if (FAILED(pLayer->Add(pToolTipJumpEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pToolTipPsychokinesisEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_ToolTips_Psychokinesis.evt"))));
	if (FAILED(pLayer->Add(pToolTipPsychokinesisEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pToolTipBasicAttackEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_ToolTips_BasicAttack.evt"))));
	if (FAILED(pLayer->Add(pToolTipBasicAttackEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pToolTipSomerSaultKickEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_ToolTips_SomerSaultKick.evt"))));
	if (FAILED(pLayer->Add(pToolTipSomerSaultKickEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pToolTipSomerSpecialAttackEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_ToolTips_SpecialAttack.evt"))));
	if (FAILED(pLayer->Add(pToolTipSomerSpecialAttackEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion

	// Subscribe Tips Event
#pragma region Subscribe Tips Event
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pTipsPMCCEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pTipsPMCCEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_PMCC();
			pToolTip->Movie_Start();

			m_bPMCCSuccess = false;
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pTipsLockOnEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pTipsLockOnEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_LockOn();
			pToolTip->Movie_Start();

			m_bLockOnSuccess = false;
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pTipsBattleStyleEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pTipsBattleStyleEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Kasane_BattleStyle();
			pToolTip->Movie_Start();

			m_bBattleStyleSuccess = false;
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pTipsSpecialAttackEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pTipsSpecialAttackEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack();
			pToolTip->Movie_Start();

			m_bSpecialAttackSuccess = false;
		});

	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pToolTipJumpEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pToolTipJumpEvent->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_Jump();
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pToolTipPsychokinesisEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pToolTipPsychokinesisEvent->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_Psycokinesis();
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pToolTipBasicAttackEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pToolTipBasicAttackEvent->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_BasicAttack();
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pToolTipSomerSaultKickEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pToolTipSomerSaultKickEvent->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_SomerSaultKick();
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pToolTipSomerSpecialAttackEvent);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pToolTipSomerSpecialAttackEvent->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_Tip_SpecialAttack();
		});
#pragma endregion

	// BrainTalk Event
#pragma region BrainTalk Event
	auto pBrainTalk0 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BrainTalk_0.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk1 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BrainTalk_1.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk2 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BrainTalk_2.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk3 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Tutorial_BrainTalk_3.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion

	// Subscribe BrainTalk Event
#pragma region Subscribe BrainTalk Event
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBrainTalk0);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBrainTalk0->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Tutorial();
			pBrainTalk->Set_BrainTalk(0);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBrainTalk1);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBrainTalk1->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Tutorial();
			pBrainTalk->Set_BrainTalk(1);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBrainTalk2);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBrainTalk2->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Tutorial();
			pBrainTalk->Set_BrainTalk(2);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::TEST, pBrainTalk3);
	pEvent_Manager->Subscribe_Event(SCENE::TEST, pBrainTalk3->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Tutorial();
			pBrainTalk->Set_BrainTalk(3);
		});
#pragma endregion

	return S_OK;
}

HRESULT CScene_Test::Ready_TestMap()
{
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_MAP_ROAD);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_MAP_DYNAMIC);
	m_pPlantLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_MAP_ETC);

	/*if (nullptr == m_pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Add_Layer: LAYER_MAP_ROAD");
	}*/

	Load_TutorialMapFile(TEXT("Tutorial_Road4"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_Road5"), 0);
	//Load_TutorialMapFile(TEXT("Tutorial_Sakura"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad0"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad1"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad2"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad3"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad4"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad5"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad6"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad7"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad8"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoad9"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoadA"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoadB"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_EnterRoadC"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RoadWall"), 0);

	Load_TutorialMapFile(TEXT("StoneA"), 0);
	Load_TutorialMapFile(TEXT("HugeTree"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootA"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootB"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootC"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootD"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootE"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootF"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_RootG"), 0);
	Load_TutorialMapFile(TEXT("HugeDarkTree"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootA"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootB"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootC"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootD"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootE"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootF"), 0);
	Load_TutorialMapFile(TEXT("Tutorial_DarkRootG"), 0);

#pragma region Dynamic
	Load_TutorialMapFile(TEXT("CarA"), 1);
	Load_TutorialMapFile(TEXT("CarB"), 1);
	Load_TutorialMapFile(TEXT("CarC"), 1);
	Load_TutorialMapFile(TEXT("CarD"), 1);
	Load_TutorialMapFile(TEXT("ConcreteBarrierA"), 1);
	Load_TutorialMapFile(TEXT("ConcreteBarrierB"), 1);
#pragma endregion

#pragma region Static
	Load_TutorialMapFile(TEXT("TruckA"), 0);
	Load_TutorialMapFile(TEXT("Truck_CampingCar"), 0);
	Load_TutorialMapFile(TEXT("Truck_Container"), 0);
	Load_TutorialMapFile(TEXT("Truck_Gas"), 0);
	Load_TutorialMapFile(TEXT("Truck_GasBrk"), 0);
	Load_TutorialMapFile(TEXT("BusA"), 0);
#pragma endregion

#pragma region Plant
	Load_TutorialMapFile(TEXT("Grass"), 2);
	Load_TutorialMapFile(TEXT("BeechA"), 2);
	Load_TutorialMapFile(TEXT("BeechB"), 2);
	Load_TutorialMapFile(TEXT("BeechC"), 2);
	Load_TutorialMapFile(TEXT("BeechD"), 2);
	Load_TutorialMapFile(TEXT("BeechE"), 2);
#pragma endregion

	/*if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_MAP_TUTIRAL_ROAD0))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
	}*/
	return S_OK;
}

HRESULT CScene_Test::Load_TutorialMapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("Bin/Resources/Map/Tutorial/dat/Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
			m_strPrototypeName = CMap_ToolManager::Get_Instance()->TutorialMap_Name(m_strObjName);

			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObjTuple_Road0
				= { SCENE::TEST, m_strObjName, Scale,Rotation,Tanslation, Mat, m_strObjName };

			if (iIndex == 0)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pPlantLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST,
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

shared_ptr<CScene_Test> CScene_Test::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Test> pInstance = make_private_shared(CScene_Test, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Load", "Failed To CInitialize");
	}

	return pInstance;
}
