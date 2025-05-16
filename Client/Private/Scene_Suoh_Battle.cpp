#include "ClientPCH.h"
#include "Scene_Suoh_battle.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"
#include "UI_Manager.h"
#include "Scene_Load.h"

#include "Camera_Main.h"
#include "Camera_Player.h"

#include "Light.h"
#include "Wave.h"

#ifdef _DEBUG
#include "TestModel.h"
#endif

#include "MenuCharacter.h"

#define TEST_CODE

CScene_Suoh_Battle::CScene_Suoh_Battle(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::SUOH_BATTLE)
{
}

HRESULT CScene_Suoh_Battle::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	//pGameInstance->Show_Cursor(true);
	//pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Apply_PostProcessing");
	}

#ifdef _DEBUG
	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	m_pModelRenderer = pGameInstance->Clone_Component<CModelRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER);
#endif

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_BackGround");
	}

	if (FAILED(Ready_Map()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Map");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Character");
	}	
	if (FAILED(Ready_Monsters()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Monsters");
	}
	/*if (FAILED(Ready_NPCs()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Monsters");
	}*/

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_Lights");
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveSpecialObject(SCENE::SUOH_BATTLE, LAYER_MAP_SPECIALDYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}

	if (FAILED(Ready_UI()))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Initialize", "Failed to Ready_UI");
	}

	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_The_Other_Appeared.mp3"), 0.5f);

	return S_OK;
}

void CScene_Suoh_Battle::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();

	if (CGameInstance::Get_Instance()->Key_Down('Y'))
	{
		auto pUIManager = CUI_Manager::Get_Instance();
		pUIManager->Play_SAS_Cutscene(CHARACTER::ARASHI);
	}
	if (CGameInstance::Get_Instance()->Key_Down('U'))
	{
		auto pUIManager = CUI_Manager::Get_Instance();
		pUIManager->Play_SAS_Cutscene(CHARACTER::SHIDEN);
	}
	if (CGameInstance::Get_Instance()->Key_Down('I'))
	{
		auto pUIManager = CUI_Manager::Get_Instance();
		pUIManager->Play_SAS_Cutscene(CHARACTER::KAGERO);
	}
	if (CGameInstance::Get_Instance()->Key_Down('O'))
	{
		auto pUIManager = CUI_Manager::Get_Instance();
		pUIManager->Play_SAS_Cutscene(CHARACTER::KYOKA);
	}
#endif
}

void CScene_Suoh_Battle::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::MINDROOM));
	}
	m_pWave->Tick();
}

HRESULT CScene_Suoh_Battle::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Suoh_Battle::Debug()
{
#if ACTIVATE_IMGUI
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		static _float	fBias = 0.f;
		static _float	fShadowShade = 1.f;
		static _float	fMinShadowAvg = 0.f;
		static _float	vTexelScale = 1.f;
		static _int2	vGridSize = _int2(4, 4);

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

		/*ImGui::Begin("Shadow");

		ImGui::Text("Settings");
		if (ImGui::InputFloat("Bias", &fBias))
		{
			m_pRenderer->Shadow_Settings(fShadowShade, fBias, vGridSize, fMinShadowAvg, vTexelScale);
		}
		if (ImGui::InputFloat("Shadow Shade", &fShadowShade))
		{
			m_pRenderer->Shadow_Settings(fShadowShade, fBias, vGridSize, fMinShadowAvg, vTexelScale);
		}
		if (ImGui::InputFloat("Min Shadow Avg", &fMinShadowAvg))
		{
			m_pRenderer->Shadow_Settings(fShadowShade, fBias, vGridSize, fMinShadowAvg, vTexelScale);
		}
		if (ImGui::InputFloat("Texel Scale", &vTexelScale))
		{
			m_pRenderer->Shadow_Settings(fShadowShade, fBias, vGridSize, fMinShadowAvg, vTexelScale);
		}
		if (ImGui::InputInt2("Grid Size", reinterpret_cast<_int*>(&vGridSize)))
		{
			m_pRenderer->Shadow_Settings(fShadowShade, fBias, vGridSize, fMinShadowAvg, vTexelScale);
		}

		static _int iIndex(0), iRes(200);
		static vector<ComPtr<ID3D11ShaderResourceView>> vecShadow;
		CGameInstance::Get_Instance()->Iterate_Shadows(
			[&](shared_ptr<CLight> pLight)
			{
				vecShadow.emplace_back(CGameInstance::Get_Instance()->Get_Shadow(pLight));
			}
		);
		if (ImGui::Button("+")) {}
		if (ImGui::IsItemActive()) ++iRes;
		ImGui::SameLine();
		if (ImGui::Button("-")) {}
		if (ImGui::IsItemActive()) --iRes; 
		ImGui::SameLine();
		if (ImGui::ArrowButton("##L", ImGuiDir_Left)) --iIndex;
		ImGui::SameLine();
		if (ImGui::ArrowButton("##R", ImGuiDir_Right)) ++iIndex;

		iIndex = std::clamp(iIndex, 0, static_cast<_int>(vecShadow.size()) - 1);
		ImGui::Image(vecShadow[iIndex].Get(), ImVec2(iRes, iRes));
		vecShadow.clear();

		ImGui::End();*/
	}
#endif
}
#endif

HRESULT CScene_Suoh_Battle::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.0f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 2.2f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.45f);

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.111f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 1.997f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 1.3f);

	// SSAO
	pGameInstance->Enable_SSAO(true);

	// LUT Filter
	pGameInstance->Enable_LUTFilter(true);
	pGameInstance->Set_LUTIndex(2);

	// Lensflare
	pGameInstance->Enable_Lensflare(true);
	pGameInstance->Set_SunPosition(_float3(1200.f, 10.f, 3000.f));
	pGameInstance->Set_SunColor(_float3(1.632f, 0.8f, 0.8f));
	pGameInstance->Set_LensflareStrength(2.038f);

	// DOF
	pGameInstance->Enable_DOF(true);
	pGameInstance->Set_DOF_DepthStart(1000.f);
	pGameInstance->Set_DOF_DepthRange(1000.f);

	pGameInstance->Enable_FXAA(true);

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Player> pPlayerCam = 
		CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	pPlayerCam->Set_DebugMode(false);
#endif

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_PLAYER);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::SUOH_BATTLE))))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Player", "Failed to CObjectLayer::Add");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Pool(SCENE::SUOH_BATTLE, POOLOBJECT_DECAL_HITCRACK, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK, 50);

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Lights()
{
	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .25;
	LightDesc.vDirection = (_float3(0.f, 0.f, 0.f) - CGameInstance::Get_Instance()->Get_SunPosition()).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .25;
	LightDesc.vSpecular = _float4(1, 1, 1, 1);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::SUOH_BATTLE, LightDesc, nullptr);

	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(500.f, 500.f);
	LightDesc.vPosition = _float3(-90.f, 90.f, -45.f);
	LightDesc.vDirection = _float3(1.f, -1.f, 0.6f);
//	LightDesc.vPosition = _float3(-0.f, 90.f, -0.f);
//	LightDesc.vDirection = _float3(0.0f, -1.f, 0.01f);
	LightDesc.fRange = 100.f;
	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::SUOH_BATTLE, LightDesc, nullptr);
	pShadow->Register_Layer(SCENE::SUOH_BATTLE, LAYER_PLAYER);
	pShadow->Register_Layer(SCENE::SUOH_BATTLE, LAYER_MONSTER);
	pShadow->Set_ShadowDesc(SHADOWDESC{ 1.f, 2.f, 0.f, 0.05f, _int2(8, 8) });

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/ReflectionCapture.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	/*if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CLOUD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_BackGround", "Failed to CObjectLayer::Add");
	}*/

#ifdef TEST_CODE
//	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, TEXT("TestModel")))))
//	{
//		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_BackGround", "Failed to CObjectLayer::Add");
//	}
//	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::SUOH_BATTLE, TEXT("TestShadow"),
//		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/TestMap/TestMap.mdl")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestShadow");
//	}
//	if (CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::SUOH_BATTLE, TEXT("TestMap"),
//		CTestModel::Create(m_pDevice, m_pContext)))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestMap");
//	}
//	pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, TEXT("TestMap")));
#endif

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_Suoh_Battle::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Monsters()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	shared_ptr<CObjectLayer> pMonster_ProjectileLayer = pGameInstance->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MONSTER_PROJECTILE);
	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MONSTER);
	if (nullptr == pMonsterLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}

	pGameInstance->Add_Pool(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, 100);
	
	m_pWave = CWave::Create();

	_float fAngle = Function::RandomFloat(-180.f, 180.f);
	for(int i=0;i<3;i++)
	{
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 130.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 150.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 170.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 190.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 210.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 230.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 250.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 270.f, Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(Function::RandomFloat(-3.f, 3.f), 5.f, 290.f, Function::RandomFloat(-180.f, 180.f)));
	
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-10.f+Function::RandomFloat(-3.f, 3.f), 5.f, 300.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-20.f+Function::RandomFloat(-3.f, 3.f), 5.f, 330.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-30.f+Function::RandomFloat(-3.f, 3.f), 5.f, 360.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-40.f+Function::RandomFloat(-3.f, 3.f), 5.f, 390.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-50.f+Function::RandomFloat(-3.f, 3.f), 5.f, 400.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-60.f+Function::RandomFloat(-3.f, 3.f), 5.f, 420.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-70.f+Function::RandomFloat(-3.f, 3.f), 5.f, 430.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-80.f+Function::RandomFloat(-3.f, 3.f), 5.f, 450.f+Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-90.f + Function::RandomFloat(-3.f, 3.f), 5.f, 460.f + Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-90.f + Function::RandomFloat(-3.f, 3.f), 5.f, 470.f + Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		m_pWave->AddWave(0, PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERHEAD, _float4(-110.f + Function::RandomFloat(-3.f, 3.f), 5.f, 500.f + Function::RandomFloat(-3.f, 3.f), Function::RandomFloat(-180.f, 180.f)));
		
	}
	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_UI()
{
	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_UI_3D);
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}
	

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Add_Layer: LAYER_UI");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_HP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, 50);
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, 5);

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_UI_Late()
{
	/*shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_INTERACTIVE_LATE);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC_LATE");
	}*/

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Ready_Map()
{
	m_pRoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_ROAD);
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_STATIC);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_DYNAMIC);
	m_pBuildingLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_BUILDING);
	m_pSpecialDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_SPECIALDYNAMIC);
	m_pETCLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::SUOH_BATTLE, LAYER_MAP_ETC);

#pragma region Road
	Load_SuohMapFile(TEXT("Suoh_Minimap"), 0);
	Load_SuohMapFile(TEXT("SG_Ground_A"), 0);
	Load_SuohMapFile(TEXT("PedestrianBridgeA"), 0);
	Load_SuohMapFile(TEXT("Suoh_Dodai"), 0);
	Load_SuohMapFile(TEXT("Suoh_Tile"), 0);
	Load_SuohMapFile(TEXT("Town_RoadB"), 0);
	Load_SuohMapFile(TEXT("Town_RoadC"), 0);

#pragma endregion

#pragma region Static
	Load_SuohMapFile(TEXT("Suoh_WallB"), 1);
	Load_SuohMapFile(TEXT("Suoh_WallC"), 1);
	Load_SuohMapFile(TEXT("Suoh_WallD"), 1);
	Load_SuohMapFile(TEXT("SG_ShutterBase"), 1);
	Load_SuohMapFile(TEXT("Town_RoadA"), 1);
	Load_SuohMapFile(TEXT("SG_ShutterA"), 1);
	Load_SuohMapFile(TEXT("SG_FlowerPot"), 1);
	Load_SuohMapFile(TEXT("Suoh_Keijiban"), 1);
	Load_SuohMapFile(TEXT("Suoh_Sekihi"), 1);
	Load_SuohMapFile(TEXT("Suoh_OmikujiB"), 1);
	Load_SuohMapFile(TEXT("SG_ArchA"), 1);
	Load_SuohMapFile(TEXT("Sumeragi_KaidanA"), 1);
	Load_SuohMapFile(TEXT("Suoh_RubbleA"), 1);
	Load_SuohMapFile(TEXT("SG_PlantsA"), 1);
	Load_SuohMapFile(TEXT("SG_TreeA"), 1);
	Load_SuohMapFile(TEXT("SG_Sumeragi_Tourou"), 1);
	Load_SuohMapFile(TEXT("MaitreyaTrii"), 1);
	Load_SuohMapFile(TEXT("Common_BaseWall"), 1);
	Load_SuohMapFile(TEXT("Suoh_CameraA"), 1);
	Load_SuohMapFile(TEXT("Suoh_CameraD"), 1);
	Load_SuohMapFile(TEXT("HydrantA"), 1);
	Load_SuohMapFile(TEXT("TelephoneBoxA"), 1);
#pragma endregion

#pragma region Building
	Load_SuohMapFile(TEXT("SG_AcademyA"), 4);
	Load_SuohMapFile(TEXT("SG_AcademyB"), 4);
	Load_SuohMapFile(TEXT("SG_AcademyC"), 4);
	Load_SuohMapFile(TEXT("Suoh_Tower"), 4);
	Load_SuohMapFile(TEXT("Suoh_Bridge"), 4);
	Load_SuohMapFile(TEXT("SG_BuildingA"), 4);
	Load_SuohMapFile(TEXT("SG_BuildingB"), 4);
	Load_SuohMapFile(TEXT("SG_BuildingC"), 4);
	Load_SuohMapFile(TEXT("Suoh_A_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_A_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_B_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_C_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_C_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_C_BuildingC"), 4);
	Load_SuohMapFile(TEXT("Suoh_C_BuildingD"), 4);
	Load_SuohMapFile(TEXT("Suoh_D_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_D_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_D_BuildingC"), 4);
	Load_SuohMapFile(TEXT("Suoh_D_BuildingD"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingC"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingD"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingE"), 4);
	Load_SuohMapFile(TEXT("Suoh_E_BuildingF"), 4);
	Load_SuohMapFile(TEXT("Suoh_F_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_F_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_G_BuildingA"), 4);
	Load_SuohMapFile(TEXT("Suoh_G_BuildingB"), 4);
	Load_SuohMapFile(TEXT("Suoh_G_BuildingC"), 4);
	Load_SuohMapFile(TEXT("Suoh_CafeBuilding"), 4);
#pragma endregion

#pragma region Signboard
	Load_SuohMapFile(TEXT("Suoh_Billboard_A0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_A1"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_A2"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_A3"), 5);

	Load_SuohMapFile(TEXT("Suoh_Billboard_C0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_D0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_G0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_I0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_I1"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_I2"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_I3"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_J0"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideA_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideA_B"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideA_C"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideB_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideC_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideC_B"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideC_C"), 5);
	Load_SuohMapFile(TEXT("Suoh_GuideC_D"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignA_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignA_B"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignA_C"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignB_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignD_D"), 5);
	Load_SuohMapFile(TEXT("Suoh_SignE"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_A"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_B"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_C"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_D"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_E"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_F"), 5);
	Load_SuohMapFile(TEXT("Suoh_BillboardX_G"), 5);
#pragma endregion

#pragma region ETC
	Load_SuohMapFile(TEXT("Suoh_BicycleA"), 5);
	Load_SuohMapFile(TEXT("Suoh_BicycleB"), 5);
	Load_SuohMapFile(TEXT("Suoh_BicycleC"), 5);
	Load_SuohMapFile(TEXT("Suoh_ScooterA"), 5);
	Load_SuohMapFile(TEXT("Suoh_ScooterB"), 5);
	Load_SuohMapFile(TEXT("Suoh_ShopBox"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_B0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_B1"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_B2"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_B3"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_F0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_G1"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_G2"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_H0"), 5);
	Load_SuohMapFile(TEXT("Suoh_Billboard_H1"), 5);
	Load_SuohMapFile(TEXT("Town_BoxA"), 5);
	Load_SuohMapFile(TEXT("Town_BoxB"), 5);
	Load_SuohMapFile(TEXT("Town_BoxC"), 5);
	Load_SuohMapFile(TEXT("Town_BoxD"), 5);
	Load_SuohMapFile(TEXT("Town_CarrierA"), 5);
	Load_SuohMapFile(TEXT("Town_CarrierB"), 5);
	Load_SuohMapFile(TEXT("Town_CarrierC"), 5);
#pragma endregion

#pragma region Dynamic
	Load_SuohMapFile(TEXT("Suoh_CarA"), 2);
	Load_SuohMapFile(TEXT("Suoh_CarB"), 2);
	Load_SuohMapFile(TEXT("Suoh_CarC"), 2);
	Load_SuohMapFile(TEXT("Suoh_CarD"), 2);
#pragma endregion

#pragma region Neon
	Load_SuohMapFile(TEXT("Suoh_Battle_WarningWallA"), 5);
	Load_SuohMapFile(TEXT("Suoh_Battle_WarningWallB"), 5);
#pragma endregion

	Load_SuohMapFile(TEXT("BusA"), 3);

	return S_OK;
}

HRESULT CScene_Suoh_Battle::Load_SuohMapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("Bin/Resources/Map/Suoh/dat/Suoh_Battle_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
			m_strPrototypeName = CMap_ToolManager::Get_Instance()->SuohMap_Name(m_strObjName);

			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObjTuple_Road0
				= { SCENE::SUOH_BATTLE, m_strObjName, Scale,Rotation,Tanslation, Mat, Name };

			if (iIndex == 0)
			{
				if (FAILED(m_pRoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 3)
			{
				if (FAILED(m_pSpecialDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}

			else if (iIndex == 4)
			{
				if (FAILED(m_pBuildingLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 5)
			{
				if (FAILED(m_pETCLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::SUOH_BATTLE,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

shared_ptr<CScene_Suoh_Battle> CScene_Suoh_Battle::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Suoh_Battle> pInstance = make_private_shared(CScene_Suoh_Battle, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Suoh_Battle", "Failed To CInitialize");
	}

	return pInstance;
}
