#include "ClientPCH.h"
#include "Scene_Hideout.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"
#include "Camera_Main.h"
#include "Camera_Player.h"
#include "UI_Manager.h"

#include "Light.h"
#include "Scene_Load.h"
#include "MenuCharacter.h"


#ifdef _DEBUG
#include "TestModel.h"
#endif

#define TEST_CODE

CScene_Hideout::CScene_Hideout(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::HIDEOUT)
{
}

HRESULT CScene_Hideout::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	//pGameInstance->Show_Cursor(true);
	//pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Apply_PostProcessing");
	}

#ifdef _DEBUG
	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	m_pModelRenderer = pGameInstance->Clone_Component<CModelRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER);
#endif

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_BackGround");
	}

	if (FAILED(Ready_Map()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Map");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Character");
	}	
	if (FAILED(Ready_Monsters()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Monsters");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_Lights");
	}
	
	if (FAILED(Ready_UI()))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Initialize", "Failed to Ready_UI");
	}

	m_fSoundVolume = 1.f;
	pGameInstance->PlayBGM(L"BGM_My_Sweet_Hideout.mp3", m_fSoundVolume);

	return S_OK;
}

void CScene_Hideout::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();
//
//	if (CGameInstance::Get_Instance()->Key_Down('Y'))
//	{
//		auto pUIManager = CUI_Manager::Get_Instance();
//		pUIManager->Play_SAS_Cutscene(CHARACTER::ARASHI);
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('U'))
//	{
//		auto pUIManager = CUI_Manager::Get_Instance();
//		pUIManager->Play_SAS_Cutscene(CHARACTER::SHIDEN);
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('I'))
//	{
//		auto pUIManager = CUI_Manager::Get_Instance();
//		pUIManager->Play_SAS_Cutscene(CHARACTER::KAGERO);
//	}
//	if (CGameInstance::Get_Instance()->Key_Down('O'))
//	{
//		auto pUIManager = CUI_Manager::Get_Instance();
//		pUIManager->Play_SAS_Cutscene(CHARACTER::KYOKA);
//	}
#endif
//	/* Effect_Test*/
//	shared_ptr<CTransform> TempPlayerTramsform =
//		CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
//
//	_float4x4 mEffect = TempPlayerTramsform->Get_Matrix();
//	_float3 PlayerLook = TempPlayerTramsform->Get_State(TRANSFORM::LOOK);
//	_float3 PlayerPosition = TempPlayerTramsform->Get_State(TRANSFORM::POSITION);
//	PlayerPosition.y += 2.f;
//
//	/* Elec_Attack */
//	if (CGameInstance::Get_Instance()->Key_Down('K'))
//	{
//		CEffect_Manager::Get_Instance()->Fetch(VFX::ELEC_ATTACK_CIRCLE, PlayerLook + PlayerPosition);
//		CEffect_Manager::Get_Instance()->Fetch(VFX::ELEC_ATTACK_MIDDLE, PlayerLook + PlayerPosition);
//		CEffect_Manager::Get_Instance()->Fetch(VFX::ELEC_ATTACK_EXPLOSION, PlayerLook + PlayerPosition);
//	}
}

void CScene_Hideout::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::CUTSCENE_MIZUHA));
	}

	//// BGM Volume Control
	//if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
	//{
	//	if (1.f <= m_fSoundVolume)
	//	{
	//		m_fSoundVolume = 1.f;
	//	}
	//	else
	//	{
	//		m_fSoundVolume += fTimeDelta;
	//	}
	//}
	//else
	//{
	//	if (0.f >= m_fSoundVolume)
	//	{
	//		m_fSoundVolume = 0.f;
	//	}
	//	else
	//	{
	//		m_fSoundVolume -= fTimeDelta;
	//	}
	//}
}

HRESULT CScene_Hideout::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Hideout::Debug()
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

			if (ImGui::BeginMenu("Material Settings"))
			{
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

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

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

HRESULT CScene_Hideout::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.5f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.4f);

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
	pGameInstance->Enable_DOF(true);
	pGameInstance->Set_DOF_DepthStart(160.f);
	pGameInstance->Set_DOF_DepthRange(200.f);

	pGameInstance->Enable_FXAA(true);

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Player> pPlayerCam = 
		CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	pPlayerCam->Set_DebugMode(false);
#endif

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_PLAYER);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::HIDEOUT))))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Player", "Failed to CObjectLayer::Add");
	}

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Lights()
{
	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .25;
	LightDesc.vDirection = (_float3(0.f, -1.f, 0.f)).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .25;
	LightDesc.vSpecular = _float4(1, 1, 1, 1);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::HIDEOUT, LightDesc, nullptr);

	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(500.f, 500.f);
	LightDesc.vPosition = _float3(-90.f, 90.f, -45.f);
	LightDesc.vDirection = _float3(1.f, -1.f, 0.6f);
//	LightDesc.vPosition = _float3(-0.f, 90.f, -0.f);
//	LightDesc.vDirection = _float3(0.0f, -1.f, 0.01f);
	LightDesc.fRange = 100.f;
	/*pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::HIDEOUT, LightDesc, nullptr);
	pShadow->Register_Layer(SCENE::HIDEOUT, LAYER_MAP_ROAD);
	pShadow->Register_Layer(SCENE::HIDEOUT, LAYER_MAP_BUILDING);
	pShadow->Register_Layer(SCENE::HIDEOUT, LAYER_MAP_STATIC);
	pShadow->Register_Layer(SCENE::HIDEOUT, LAYER_PLAYER);
	pShadow->Set_ShadowDesc(SHADOWDESC{ 1.f, -0.5f, 0.f, 0.05f, _int2(8, 8) });*/

	return S_OK;
}

HRESULT CScene_Hideout::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/ReflectionCapture.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	/*if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CLOUD))))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_BackGround", "Failed to CObjectLayer::Add");
	}*/

#ifdef TEST_CODE
//	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, TEXT("TestModel")))))
//	{
//		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_BackGround", "Failed to CObjectLayer::Add");
//	}
//	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::HIDEOUT, TEXT("TestShadow"),
//		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/TestMap/TestMap.mdl")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestShadow");
//	}
//	if (CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::HIDEOUT, TEXT("TestMap"),
//		CTestModel::Create(m_pDevice, m_pContext)))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestMap");
//	}
//	pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, TEXT("TestMap")));
#endif

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Monsters()
{
	return S_OK;
}

HRESULT CScene_Hideout::Ready_UI()
{
	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_UI_3D);
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}


	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Add_Layer: LAYER_UI");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_HP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_COMMUNICATION))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_FADE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_SHOP_INFO))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_CURSOR))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	}
	return S_OK;
}

HRESULT CScene_Hideout::Ready_UI_Late()
{
	/*shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_INTERACTIVE_LATE);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC_LATE");
	}*/

	return S_OK;
}

HRESULT CScene_Hideout::Ready_Map()
{
	m_pRoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_ROAD);
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_STATIC);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_DYNAMIC);
	m_pBuildingLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_BUILDING);
	m_pSpecialDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_SPECIALDYNAMIC);
	m_pETCLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::HIDEOUT, LAYER_MAP_ETC);

	Load_MapFile(TEXT("Common_BaseWall"), 1);
	Load_MapFile(TEXT("Common_Item"), 5);
	Load_MapFile(TEXT("Arashi_Teammate"), 5);

#pragma region Static
	Load_MapFile(TEXT("Hideout_Floor"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Carpet"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Cole"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Fireplace"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Kitchen"), 1); // PBR
	Load_MapFile(TEXT("Hideout_LowTable"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Pouf"), 1); // PBR
	Load_MapFile(TEXT("Hideout_SofaA"), 1); // PBR
	Load_MapFile(TEXT("Hideout_SofaB"), 1); // PBR
	Load_MapFile(TEXT("Hideout_SofaC"), 1); // PBR
	Load_MapFile(TEXT("Hideout_FabricChair"), 1); // PBR
	Load_MapFile(TEXT("Hideout_MounatinScreen"), 1); // PBR
	Load_MapFile(TEXT("Hideout_PerfectBodyPhoto"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Table"), 1); // PBR
	Load_MapFile(TEXT("Hideout_TeamPhoto"), 1); // PBR
	Load_MapFile(TEXT("Hideout_BenchA"), 1); // PBR
	Load_MapFile(TEXT("Hideout_BenchPress"), 1); // PBR
	Load_MapFile(TEXT("Hideout_DumbbellA"), 1); // PBR
	Load_MapFile(TEXT("Hideout_DumbbellB"), 1); // PBR
	Load_MapFile(TEXT("Hideout_DumbbellC"), 1); // PBR
	Load_MapFile(TEXT("Hideout_Kettlebell"), 1); // PBR
	Load_MapFile(TEXT("Hideout_DumbbellRack"), 1); // PBR

#pragma endregion

	return S_OK;
}

HRESULT CScene_Hideout::Load_MapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("Bin/Resources/Map/Hideout/dat/Hideout_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
			m_strPrototypeName = CMap_ToolManager::Get_Instance()->Hideout_Name(m_strObjName);

			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObjTuple_Road0
				= { SCENE::HIDEOUT, m_strObjName, Scale,Rotation,Tanslation, Mat, Name };

			if (iIndex == 0)
			{
				if (FAILED(m_pRoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 3)
			{
				if (FAILED(m_pSpecialDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}

			else if (iIndex == 4)
			{
				if (FAILED(m_pBuildingLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 5)
			{
				if (FAILED(m_pETCLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::HIDEOUT,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Hideout::Ready_Hideout", "Failed to Clone_GameObject::Add");
				}
			}
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

shared_ptr<CScene_Hideout> CScene_Hideout::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Hideout> pInstance = make_private_shared(CScene_Hideout, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Hideout", "Failed To CInitialize");
	}

	return pInstance;
}
