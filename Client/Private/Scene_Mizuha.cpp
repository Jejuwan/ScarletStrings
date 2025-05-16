#include "ClientPCH.h"
#include "Scene_Mizuha.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"
#include "UI_Manager.h"

#include "Camera_Main.h"
#include "Camera_Player.h"

#include "SceneCapture.h"
#include "Wave.h"

#include "Light.h"

#include "BattleField.h"
#include "BattleField_Manager.h"
#include "EventCollider.h"
#include "Event_Manager.h"

#include "Scene_Load.h"

#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_ToolTip_Tutorial_Tips.h"

#include "UI_Indicator_ToolTip_Mizuha_SAS_Acceleration.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Electric.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Replication.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Stealth.h"

#include "MenuCharacter.h"
#include "ScreenEffectManager.h"

CScene_Mizuha::CScene_Mizuha(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::MIZUHA)
{
}

HRESULT CScene_Mizuha::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	//pGameInstance->Show_Cursor(true);
	//pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to CScene::Initialize");
	}

#ifdef _DEBUG
	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	m_pModelRenderer = pGameInstance->Clone_Component<CModelRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER);
#endif

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_BackGround");
	}

	if (FAILED(Ready_Map()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Map");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Character");
	}	

	if (FAILED(Ready_Teammate()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Teammate");
	}

	if (FAILED(Ready_Monsters()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Monsters");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_Lights");
	}
	
	if (FAILED(Ready_UI()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_UI");
	}

	if (FAILED(Ready_BattleField()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_BattleField");
	}
	if (FAILED(Ready_EventCollider()))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Initialize", "Failed to Ready_EventCollider");
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveObject(SCENE::MIZUHA, LAYER_MAP_DYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveSpecialObject(SCENE::MIZUHA, LAYER_MAP_SPECIALDYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}


	pGameInstance->PlayBGM(L"MizuhagawaDistrictOST.wav", 2.f);

	return S_OK;
}

void CScene_Mizuha::Tick(_float fTimeDelta)
{
	m_pWave->Tick();
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//if (pGameInstance->Key_Down('K'))
	//{
	//	pGameInstance->Find_Layer(SCENE::MIZUHA, LAYER_MONSTER)->Add(m_pDispenPerry);
	//	
	//	//_pDispenPerry = nullptr;
	//}

#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();

	CBattleField_Manager::Get_Instance()->Debug_Render();
#endif
}

void CScene_Mizuha::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::SUOH_BATTLE));
	}
}

HRESULT CScene_Mizuha::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Mizuha::Debug()
{
	if (CGameInstance::Get_Instance()->Key_Down('O'))
	{
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::SUNNY_FILTER);
	}

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
				if (ImGui::BeginMenu("CUTSCENE BUFFER"))
				{
					auto pSceneCapture = CSceneCapture::Get_Instance();
					ImGui::Image(pSceneCapture->Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL::CHANNEL_MAIN).Get(), ImVec2(200, 200));
					ImGui::Image(pSceneCapture->Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL::CHANNEL_SUB_0).Get(), ImVec2(200, 200));
					ImGui::Image(pSceneCapture->Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL::CHANNEL_SUB_1).Get(), ImVec2(200, 200));
					ImGui::Image(pSceneCapture->Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL::CHANNEL_SUB_2).Get(), ImVec2(200, 200));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("NEON"))
				{
					ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NEON).Get(), ImVec2(480, 270));
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Shadow"))
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
			}
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

HRESULT CScene_Mizuha::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 2.3f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.25f);

	// Neon
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.145f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 1.095f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 1.2f);

	// LUT Filter
	pGameInstance->Enable_LUTFilter(true);
	pGameInstance->Set_LUTIndex(4);

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

	// FXAA
	pGameInstance->Enable_FXAA(true);

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Player> pPlayerCam = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	pPlayerCam->Set_DebugMode(false);
#endif

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_PLAYER);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::MIZUHA))))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Player", "Failed to CObjectLayer::Add");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Pool(SCENE::MIZUHA, POOLOBJECT_DECAL_HITCRACK, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK, 50);

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Lights()
{
	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f) * 0.25;
	LightDesc.vDirection = _float3(-0.75f, -1.f, -0.75f).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * 0.25;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::MIZUHA, LightDesc, nullptr);
	
	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(500.f, 500.f);
	LightDesc.vPosition = _float3(-100.f, 100.f, 100.f);
	LightDesc.vDirection = _float3(-0.75f, -1.f, -0.75f);
	LightDesc.fRange = 200.f;
	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::MIZUHA, LightDesc, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
	pShadow->Register_Layer(SCENE::MIZUHA, LAYER_PLAYER);
	pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MONSTER);
	pShadow->Register_Layer(SCENE::MIZUHA, LAYER_TEAMMATE);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_ROAD);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_STATIC);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_DYNAMIC);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_BUILDING);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_SPECIALDYNAMIC);
	//pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MAP_ETC);
	pShadow->Set_ShadowDesc(SHADOWDESC{ 1.f, 2.f, 0.f, 0.05f, _int2(8, 8) });

//	LightDesc.eLightType = LIGHTTYPE::SHADOW;
//	LightDesc.eShadowType = SHADOWTYPE::DYNAMIC;
//	LightDesc.vShadowProjection = _float2(10.f, 10.f);
//	LightDesc.vDirection = _float3(1.f, -1.f, 1.f);
//	LightDesc.fRange = 20.f;
//	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::MIZUHA, LightDesc, Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM));
//	pShadow->Register_Layer(SCENE::MIZUHA, LAYER_PLAYER);
//	pShadow->Register_Layer(SCENE::MIZUHA, LAYER_MONSTER);
//	pShadow->Set_ShadowDesc(SHADOWDESC{ 0.8f, 0.f, 0.f, 0.05f, _int2(4, 4) });

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/LC12_Cube.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CLOUD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKasane = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE, true);
	if (FAILED(pLayer->Add(pKasane)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);
	pUIManager->Bind_Character(CHARACTER::KASANE, pKasane);

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Monsters()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	shared_ptr<CObjectLayer> pMonster_ProjectileLayer = pGameInstance->Add_Layer(m_eScene, LAYER_MONSTER_PROJECTILE);
	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Add_Layer(m_eScene, LAYER_MONSTER);
	if (nullptr == pMonsterLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}
	m_pDispenPerry = CGameInstance::Get_Instance()->Clone_GameObject(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_DISPENPERRY, _float4(-250.f, 0.f, 0.f, 1.f));
	//pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST, 10);
	//pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, 10);
	pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, 20);
	pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, 10);
	pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE, 10);
	pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO, PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO, 1);
	//pGameInstance->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_DISPENPERRY, PROTOTYPE_GAMEOBJECT_MONSTER_DISPENPERRY, 1);

	m_pWave = CWave::Create();

	m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-152.532f, -6.4f, -23.1f, 90.f));
	
	m_pWave->AddWave(1, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-142.473f, -6.674f, -40.1f, 0.f));
	m_pWave->AddWave(1, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-144.832f, -6.479f, -10.2f, 180.f));

	m_pWave->AddWave(2, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-178.345f, -6.619f, -93.7f, 0.f));
	m_pWave->Subscribe_WaveInitCallback(2, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 0, false);
		});

	m_pWave->AddWave(3, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-171.214f, -6.619f, -97.19f, -90.f));
	m_pWave->AddWave(3, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-185.752f, -6.619f, -107.7f, 0.f));
	m_pWave->AddWave(3, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-177.752f, -6.619f, -86.71f, 180.f));

	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(-342.697f, -6.02f + 2.f, -14.4f, 180.f));
	m_pWave->AddWave(4, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(-315.611f, -6.02f + 2.f, 57.43f, 180.f+45.f));
	m_pWave->Subscribe_WaveInitCallback(4, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 1, false);
		});

	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(-152.603f, -6.67f + 2.f, 32.33f, 0.f));
	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_FLY, _float4(-125.43f, -6.618f + 2.f, 47.39f, 0.f));
	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-166.964f, -6.642f, 40.92f, -90.f));
	m_pWave->AddWave(5, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-160.f, -6.63f, 60.75f, -90.f));

	m_pWave->AddWave(6, PROTOTYPE_GAMEOBJECT_MONSTER_BUFFALO, _float4(-153.439f, -6.674f, 37.87f, -90.f));
	
	m_pWave->AddWave(7, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-168.f, -6.819f, 134.6f, 0.f));
	m_pWave->AddWave(7, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-171.14f, -6.561f, 117.9f, 0.f));
	m_pWave->AddWave(7, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-66.382f, -7.262f, 125.38f, 0.f));
	m_pWave->AddWave(7, PROTOTYPE_GAMEOBJECT_MONSTER_SNAIL, _float4(-74.445f, -6.912f, 136.25f, 0.f));
	
	m_pWave->AddWave(8, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE, _float4(-72.966f, -48.478f, 52.23f, 0.f));
	m_pWave->Subscribe_WaveInitCallback(8, []()
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 2, false);
		});

	m_pWave->AddWave(9, PROTOTYPE_GAMEOBJECT_MONSTER_TWINCE, _float4(-114.261f, -21.318f, -22.23f, 0.f));
	m_pWave->Subscribe_WaveInitCallback(9, []()
	{
		auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
		pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 3, false);
	});
 
	//m_pWave->Subscribe_WaveInitCallback(11, []()
	//	{
	//		auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
	//		pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 3, false);
	//	});PERRY, _float4(-260.f, -30.f, 0.f, 1.f));

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_UI()
{
	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_UI_3D);
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Add_Layer: LAYER_UI");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_HP))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_INSTKILL))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_TIPS))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_LOCKON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_MISSPLAYER))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_NAME_BOSS))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BOSS_HP))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_BRAINTALK_MIZUHA))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, 50);
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, 5);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ELECTRIC))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_ACCELERATION))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_REPLICATION))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_SAS_STEALTH))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_UI_Late()
{
	/*shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_INTERACTIVE_LATE);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC_LATE");
	}*/


	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Teammate()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_TEAMMATE);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Add_Layer: LAYER_TEAMMATE");
	}

	_float4 vInitPosition = { 1.f, 5.f, 20.f, 1.f };
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE, vInitPosition))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	vInitPosition = { 1.f, 5.f, 15.f, 1.f };
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN_TEAMMATE, vInitPosition))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_BattleField()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_BATTLEFIELD);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Add_Layer: LAYER_BATTLEFIELD");
	}

	auto pBattleField0 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Mizuha_0.btf")));
	if (FAILED(pLayer->Add(pBattleField0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField1 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Mizuha_1.btf")));
	if (FAILED(pLayer->Add(pBattleField1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField2 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Mizuha_2.btf")));
	if (FAILED(pLayer->Add(pBattleField2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	auto pBattleField3 = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
		, wstring(TEXT("Bin/Resources/BattleField/Mizuha_3.btf")));
	if (FAILED(pLayer->Add(pBattleField3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Clone_GameObject::Add");
	}
	//auto pBattleFieldBoss = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_BATTLEFIELD
	//	, wstring(TEXT("Bin/Resources/BattleField/Mizuha_Boss.btf")));
	//if (FAILED(pLayer->Add(pBattleFieldBoss)))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Clone_GameObject::Add");
	//}

	auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
	pBattleField_Manager->Bind_BattleField(SCENE::MIZUHA, dynamic_pointer_cast<CBattleField>(pBattleField0));
	pBattleField_Manager->Bind_BattleField(SCENE::MIZUHA, dynamic_pointer_cast<CBattleField>(pBattleField1));
	pBattleField_Manager->Bind_BattleField(SCENE::MIZUHA, dynamic_pointer_cast<CBattleField>(pBattleField2));
	pBattleField_Manager->Bind_BattleField(SCENE::MIZUHA, dynamic_pointer_cast<CBattleField>(pBattleField3));
	//pBattleField_Manager->Bind_BattleField(SCENE::MIZUHA, dynamic_pointer_cast<CBattleField>(pBattleFieldBoss));

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_EventCollider()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_EVENT_COLLIDER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Add_Layer: LAYER_EVENT_COLLIDER");
	}

	// Load Battle Field
#pragma region Load Battle Field Event
	auto pBattleFieldEvent0 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BattleField_0.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent1 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BattleField_1.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent2 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BattleField_2.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBattleFieldEvent3 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BattleField_3.evt"))));
	if (FAILED(pLayer->Add(pBattleFieldEvent3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion
	// Subscribe Battle Field Event
#pragma region Subscribe Battle Field Event
	auto pEvent_Manager = CEvent_Manager::Get_Instance();
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBattleFieldEvent0);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBattleFieldEvent0->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 0, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBattleFieldEvent1);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBattleFieldEvent1->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 1, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBattleFieldEvent2);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBattleFieldEvent2->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 2, true);
		});

	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBattleFieldEvent3);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBattleFieldEvent3->Get_EventTag(), [](EVENT_TYPE eEventType, string strEventTag)
		{
			//auto pBattleField_Manager = CBattleField_Manager::Get_Instance();
			//pBattleField_Manager->Enable_BattleField(SCENE::MIZUHA, 3, true);
		});
#pragma endregion

	// Load ToolTips
#pragma region ToolTips Event
	auto pToolTipSystemLogEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_ToolTips_SystemLog.evt"))));
	if (FAILED(pLayer->Add(pToolTipSystemLogEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}

	auto pToolTipSASElectricEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_ToolTips_SAS_Electric.evt"))));
	if (FAILED(pLayer->Add(pToolTipSASElectricEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBigToolTipSASElectricEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BigToolTips_SAS_Electric.evt"))));
	if (FAILED(pLayer->Add(pBigToolTipSASElectricEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}

	auto pBigToolTipSASAccEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_ToolTips_SAS_Fast.evt"))));
	if (FAILED(pLayer->Add(pBigToolTipSASAccEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}

	auto pToolTipSASCloneEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_ToolTips_SAS_Clone.evt"))));
	if (FAILED(pLayer->Add(pToolTipSASCloneEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pToolTipSASReplEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BigToolTips_SAS_Repl.evt"))));
	if (FAILED(pLayer->Add(pToolTipSASReplEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}

	auto pToolTipSASStealthEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_ToolTips_SAS_Stealth.evt"))));
	if (FAILED(pLayer->Add(pToolTipSASStealthEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBigToolTipSASStealthEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BigToolTips_SAS_Stealth.evt"))));
	if (FAILED(pLayer->Add(pBigToolTipSASStealthEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion
	// Subscribe ToolTips Event
#pragma region Subscribe ToolTips Event
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSystemLogEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSystemLogEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_SystemLog();
		});

	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSASElectricEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSASElectricEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_SAS_Electric();

			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_SIDEN);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBigToolTipSASElectricEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBigToolTipSASElectricEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pElecToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Mizuha_SAS_Electric();
			pElecToolTip->Movie_Start();
		});

	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBigToolTipSASAccEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBigToolTipSASAccEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pElecToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Mizuha_SAS_Acceleration();
			pElecToolTip->Movie_Start();
		});
	
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSASCloneEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSASCloneEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_SAS_Clone();

			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_KYOKA);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSASReplEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSASReplEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pElecToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Mizuha_SAS_Replication();
			pElecToolTip->Movie_Start();
		});
	
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSASStealthEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSASStealthEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Tutorial_Tips();
			pToolTip->Start_Tip_SAS_Stealth();

			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_KAGERO);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBigToolTipSASStealthEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBigToolTipSASStealthEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pElecToolTip = CUI_Manager::Get_Instance()->Get_Indicator_ToolTip_Mizuha_SAS_Stealth();
			pElecToolTip->Movie_Start();
		});
	
#pragma endregion

	// Load Brain Talk Event
#pragma region BrainTalk Event
	auto pToolTipSASFastEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_SAS_Arashi.evt"))));
	if (FAILED(pLayer->Add(pToolTipSASFastEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pToolTipSASFastEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pToolTipSASFastEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_USE_MY_SAS_ARASI);
		});
#pragma endregion

	// Load Boss Cutscene Event
#pragma region Boss Cutscene Event
	auto pBossCutsceneEvent = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_Dispenperry.evt"))));
	if (FAILED(pLayer->Add(pBossCutsceneEvent)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion
	// Subscribe Boss Cutscene Event
#pragma region Subscribe Boss Cutscene Event
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBossCutsceneEvent);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBossCutsceneEvent->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			CGameInstance::Get_Instance()->Find_Layer(SCENE::MIZUHA, LAYER_MONSTER)->Add(m_pDispenPerry);
		});
#pragma endregion

	// Load BrainTalk Event
#pragma region Load BrainTalk Event
	auto pBrainTalk0 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_0.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk0)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk1 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_1.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk1)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk2 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_2.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk2)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk3 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_3.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk3)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk4 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_4.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk4)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk5 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_5.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk5)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
	auto pBrainTalk6 = dynamic_pointer_cast<CEventCollider>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA, PROTOTYPE_GAMEOBJECT_EVENT_COLLIDER
		, wstring(TEXT("Bin/Resources/EventCollider/Mizuha_BrainTalk_6.evt"))));
	if (FAILED(pLayer->Add(pBrainTalk6)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_EventCollider", "Failed to Clone_GameObject::Add");
	}
#pragma endregion

	// Subscribe BrainTalk Event
#pragma region Subscribe BrainTalk Event
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk0);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk0->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_FIRST);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk1);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk1->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_SECOND);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk2);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk2->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_THIRD);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk3);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk3->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_FOURTH);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk4);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk4->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_FIFTH);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk5);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk5->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_SIXTH);
		});
	pEvent_Manager->Bind_EventCollider(SCENE::MIZUHA, pBrainTalk6);
	pEvent_Manager->Subscribe_Event(SCENE::MIZUHA, pBrainTalk6->Get_EventTag(), [&](EVENT_TYPE eEventType, string strEventTag)
		{
			auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
			pBrainTalk->Set_BrainTalk_Conversation(CUI_Indicator_BrainTalk_Mizuha::BT_SEVENTH);
		});
#pragma endregion

	return S_OK;
}

HRESULT CScene_Mizuha::Ready_Map()
{
	m_pRoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_ROAD);
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_STATIC);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_DYNAMIC);
	m_pBuildingLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_BUILDING);
	m_pSpecialDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_SPECIALDYNAMIC);
	m_pETCLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MIZUHA, LAYER_MAP_ETC);

	Load_MizuhaMapFile(TEXT("Common_Item"), 5);
	Load_MizuhaMapFile(TEXT("Common_BaseWall"), 1);
#pragma region Road
	Load_MizuhaMapFile(TEXT("Area_MiniMapA"),0); // PBR
	Load_MizuhaMapFile(TEXT("Dirt_Tile"), 0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraA"), 0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraB"), 0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraC"), 0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraD"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraE"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraF"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraG"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraH"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraI"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraJ"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraK"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePiraL"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdD"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdE"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdF"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdShapeA"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdShapeB"),0); // PBR
	Load_MizuhaMapFile(TEXT("FootholdG"), 0); // PBR
	Load_MizuhaMapFile(TEXT("Dirt_Ground"), 0); // PBR
	Load_MizuhaMapFile(TEXT("BasementA"),0); // PBR
	Load_MizuhaMapFile(TEXT("BasementB"),0); // PBR
	Load_MizuhaMapFile(TEXT("BasementC"),0); // PBR
	Load_MizuhaMapFile(TEXT("BasementD"),0); // PBR
	Load_MizuhaMapFile(TEXT("BasementE"),0); // PBR
	Load_MizuhaMapFile(TEXT("ConstructionBuildingC"), 0); // PBR
	Load_MizuhaMapFile(TEXT("Bridge_EnkeiB"), 0); // PBR
	Load_MizuhaMapFile(TEXT("Basement_BendA"), 0); // PBR
	Load_MizuhaMapFile(TEXT("BossWallA"),0); // SteelWallA PBR
	Load_MizuhaMapFile(TEXT("BossWallB"),0); // SteelWallABendA PBR
	Load_MizuhaMapFile(TEXT("BossWallC"),0); // SteelWallABendB PBR
	Load_MizuhaMapFile(TEXT("BossWallD"),0); // SteelWallABendC PBR
	Load_MizuhaMapFile(TEXT("BossWallE"),0); // SteelWallB PBR
	Load_MizuhaMapFile(TEXT("BossWallF"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossWallG"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossWallH"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossWallI"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterA"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterB"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterC"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterD"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterE"),0); // PBR
	Load_MizuhaMapFile(TEXT("BossBanisterF"),0); // PBR
	Load_MizuhaMapFile(TEXT("Bridge_EnkeiC"), 0); // PBR

	Load_MizuhaMapFile(TEXT("ConstructionSlopeA"), 1); // PBR

	Load_MizuhaMapFile(TEXT("OutsideFloorA"), 1); // PBR
	Load_MizuhaMapFile(TEXT("OutsideFloorB"), 1); // PBR
	Load_MizuhaMapFile(TEXT("OutsideFloorC"), 1); // PBR
	Load_MizuhaMapFile(TEXT("OutsideFloorD"), 1); // PBR
	Load_MizuhaMapFile(TEXT("OutsideFloorE"), 1); // PBR
	Load_MizuhaMapFile(TEXT("OutsideFloorF"), 1); // PBR

	Load_MizuhaMapFile(TEXT("FencingD"), 1); // FootholdM PBR

#pragma endregion

#pragma region Static
	Load_MizuhaMapFile(TEXT("FenceA"),1); // PBR
	Load_MizuhaMapFile(TEXT("FenceC"),1); // PBR
	Load_MizuhaMapFile(TEXT("RealSteelWallF"),1); // REALSTEELWALL PBR
	Load_MizuhaMapFile(TEXT("RealSteelWallA"),1); // PBR
	Load_MizuhaMapFile(TEXT("RealSteelWallB"),1); // PBR
	Load_MizuhaMapFile(TEXT("RealSteelWallC"),1); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall5A"),1); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall5B"),1); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall5C"),1); // PBR
	Load_MizuhaMapFile(TEXT("HeavyEquipmentA"),1); // PBR
	Load_MizuhaMapFile(TEXT("HeavyEquipmentC"),1); // PBR
	Load_MizuhaMapFile(TEXT("ConstructionGateA"), 1); // PBR
	Load_MizuhaMapFile(TEXT("HeavyEquipmentE"), 1); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall4A"), 1); // PBR
	Load_MizuhaMapFile(TEXT("PrefabA"),1); // PBR
	Load_MizuhaMapFile(TEXT("PrefabB"),1); // PBR
	Load_MizuhaMapFile(TEXT("RockA"),1); // PBR
	Load_MizuhaMapFile(TEXT("RockB"),1); // PBR
	Load_MizuhaMapFile(TEXT("RockC"),1); // PBR
	Load_MizuhaMapFile(TEXT("RubbleA"),1); // PBR
	Load_MizuhaMapFile(TEXT("RubbleB"),1); // PBR
	Load_MizuhaMapFile(TEXT("FencingA"),1); // PBR
	Load_MizuhaMapFile(TEXT("FencingB"),1); // PBR
	Load_MizuhaMapFile(TEXT("FencingC"),1); // PBR
	Load_MizuhaMapFile(TEXT("NDF0"), 1); // PBR
	Load_MizuhaMapFile(TEXT("PlyWoodA"), 1); // PBR
	Load_MizuhaMapFile(TEXT("PlyWoodB"), 1); // PBR
	Load_MizuhaMapFile(TEXT("BioToilet"), 1); // PBR
	Load_MizuhaMapFile(TEXT("RubbleC"), 1); // PBR
	Load_MizuhaMapFile(TEXT("RubbleD"), 1); // PBR
	Load_MizuhaMapFile(TEXT("ConcreteWallA"), 1); // PBR
	Load_MizuhaMapFile(TEXT("SteelPoleA"), 1); // PBR
	Load_MizuhaMapFile(TEXT("TowerCrane_Base"), 1); // PBR
	Load_MizuhaMapFile(TEXT("FencingE"), 1); // PBR
	Load_MizuhaMapFile(TEXT("RoadBlockB"), 1); // PBR

	Load_MizuhaMapFile(TEXT("RealSteelWallD"), 4); // PBR
	Load_MizuhaMapFile(TEXT("RealSteelWallE"), 4); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall4ANon"), 4); // PBR
#pragma endregion
	
#pragma region Dynamic
	Load_MizuhaMapFile(TEXT("BarrelA"),2); // PBR
	Load_MizuhaMapFile(TEXT("BarrelB"), 2); // PBR
	Load_MizuhaMapFile(TEXT("Dumpster"), 2); // PBR
	Load_MizuhaMapFile(TEXT("BoxA"), 2); // PBR
	Load_MizuhaMapFile(TEXT("BoxB"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcreteBagA"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcreteBagB"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePipeA"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePipeB"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePipeC"), 2); // PBR
	Load_MizuhaMapFile(TEXT("ConcretePipeD"), 2); // PBR
	Load_MizuhaMapFile(TEXT("SpoolA"), 2); // PBR
	Load_MizuhaMapFile(TEXT("Brick_Palette"), 2); // PBR
	Load_MizuhaMapFile(TEXT("CableReel"), 2); // PBR
	Load_MizuhaMapFile(TEXT("IceBox"), 2); // PBR
#pragma endregion

#pragma region Special_Dynamic
	Load_MizuhaMapFile(TEXT("Special_BeamA"),3); // PBR
	Load_MizuhaMapFile(TEXT("HeavyEquipmentG"), 3); // PBR
	Load_MizuhaMapFile(TEXT("Special_Rock0"), 3); // PBR
	Load_MizuhaMapFile(TEXT("HeavyEquipmentH"), 3); // PBR
#pragma endregion

#pragma region Building
	Load_MizuhaMapFile(TEXT("SteelWall3A"), 4); // PBR
	Load_MizuhaMapFile(TEXT("SteelWall3B"), 4); // PBR
	Load_MizuhaMapFile(TEXT("CityBuilding_PartsA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("CityBuildingA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("ConstructionBuildingA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("ConstructionBuildingB"), 4); // PBR
	Load_MizuhaMapFile(TEXT("Crane_EnkeiA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("DistantBuildingA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("Ironpillar"), 4); // PBR
	Load_MizuhaMapFile(TEXT("TarpA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("TarpB"), 4); // PBR
	Load_MizuhaMapFile(TEXT("TarpC"), 4); // PBR
	Load_MizuhaMapFile(TEXT("PlyWoodC"), 4); // PBR
	Load_MizuhaMapFile(TEXT("PlyWoodD"), 4); // PBR
	Load_MizuhaMapFile(TEXT("Ground_CoalA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("CityBuilding_Tower"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingB"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingC"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingD"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingE"), 4); // PBR
	Load_MizuhaMapFile(TEXT("MizuhaBuildingF"), 4); // PBR
#pragma endregion

#pragma region ETC
	Load_MizuhaMapFile(TEXT("Ground_CarC"),5); // PBR
	Load_MizuhaMapFile(TEXT("Ground_Grass"), 5); // Non PBR ></
	Load_MizuhaMapFile(TEXT("RoadBlockA"), 5); // PBR
	Load_MizuhaMapFile(TEXT("Ground_ConeA"), 5); // PBR
	Load_MizuhaMapFile(TEXT("Ground_ConeB"), 5); // PBR
	Load_MizuhaMapFile(TEXT("TanA"), 5); // Non PBR
	Load_MizuhaMapFile(TEXT("TanB"), 5); // Non PBR
	Load_MizuhaMapFile(TEXT("TanC"), 5); // Non PBR
	Load_MizuhaMapFile(TEXT("TanD"), 5); // Non PBR
	Load_MizuhaMapFile(TEXT("TanE"), 5); // Non PBR
#pragma endregion

	Load_MizuhaMapFile(TEXT("ConstructionElevator"), 0); // PBR

#pragma region LightObject
	Load_MizuhaMapFile(TEXT("SpotlightA"), 4); // PBR
	Load_MizuhaMapFile(TEXT("FloodlightB"), 4); // PBR
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

HRESULT CScene_Mizuha::Load_MizuhaMapFile(wstring strObjName, _int iIndex)
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
				if (FAILED(m_pRoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 3)
			{
				if (FAILED(m_pSpecialDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			
			else if (iIndex == 4)
			{
				if (FAILED(m_pBuildingLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 5)
			{
				if (FAILED(m_pETCLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MIZUHA,
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

shared_ptr<CScene_Mizuha> CScene_Mizuha::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Mizuha> pInstance = make_private_shared(CScene_Mizuha, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Mizuha", "Failed To CInitialize");
	}

	return pInstance;
}
