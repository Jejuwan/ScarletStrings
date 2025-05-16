#include "ClientPCH.h"
#include "Scene_MindRoom.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Map_ToolManager.h"
#include "UI_Manager.h"
#include "Scene_Load.h"

#include "Camera_Main.h"
#include "Camera_Player.h"
#include "Monster_Statue.h"
#include "RedLaserEye.h"

#include "MenuCharacter.h"

#include "Light.h"
#include "Effect_Manager.h"
#include "Bone.h"

#include "Map_MindRoom_Dynamic.h"
#include "InteractiveObjectFinder.h"
#include "ScreenEffectManager.h"


#ifdef _DEBUG
#include "TestModel.h"
#include "CutsceneCamera.h"
#include "CameraKeyframeGenerator.h"
#endif

#define TEST_CODE

CScene_MindRoom::CScene_MindRoom(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::MINDROOM)
{
}

HRESULT CScene_MindRoom::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	//pGameInstance->Show_Cursor(true);
	//pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

#ifdef _DEBUG
	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
	m_pModelRenderer = pGameInstance->Clone_Component<CModelRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_MODEL_RENDERER);

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_DEBUG_OBJECT);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to Add_Layer: LAYER_DEBUG_OBJECT");
	}
	shared_ptr<CCameraKeyframeGenerator> pCameraTool =
		CGameInstance::Get_Instance()->Clone_GameObject<CCameraKeyframeGenerator>(SCENE::MINDROOM,
			PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR);
	if (FAILED(pLayer->Add(pCameraTool)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_DebugObjects", "Failed to CObjectLayer::Add");
	}
#endif

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to CScene::Initialize");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_BackGround()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_BackGround");
	}

	if (FAILED(Ready_Map()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Map");
	}

	if (FAILED(Ready_Teammate()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Teammate");
	}

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Character");
	}	
	if (FAILED(Ready_Monsters()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Monsters");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_Lights");
	}

	if (FAILED(Ready_UI()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Ready_UI");
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveSpecialObject(SCENE::MINDROOM, LAYER_MAP_SPECIALDYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}

	//CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_Karen Travers.mp3"), 0.5f);

	if (FAILED(Apply_PostProcessing()))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Initialize", "Failed to Apply_PostProcessing");
	}
	   
	return S_OK;
}

void CScene_MindRoom::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down('Y'))
	{
		CGameInstance::Get_Instance()->StopAll();
		CGameInstance::Get_Instance()->PlaySoundW(L"Monster_Buffalo_Die.wav", 1.f);
		//CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RIPPLE);
	}
	if (CGameInstance::Get_Instance()->Key_Down('U'))
	{
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
	}
	if (CGameInstance::Get_Instance()->Key_Down('I'))
	{
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::COLOR_REVERSE);
	}

#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();

	if (ImGui::BeginMenu("Shadow"))
		{
		if (ImGui::BeginMenu("Shadow"))
		{
			ImGui::Text("Settings");
			static LIGHTDESC tLightDesc;
			static SHADOWDESC tShadowDesc;

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


	// Material Settings
	ImGui::Begin("Material Settings");
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

	ImGui::End();

	if (CGameInstance::Get_Instance()->Key_Down('N'))
	{
		shared_ptr<CTransform> pPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

		_float3 Look = pPlayerTramsform->Get_State(TRANSFORM::LOOK);
		Look *= 0.4f;
		_float3 Pos = pPlayerTramsform->Get_State(TRANSFORM::POSITION) + Look;
		_float3 Pos2 = Pos;
		_float3 RandomPos[7] = {};

		Pos.y += 1.75f;
		Pos2.y += 1.9f;

		const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

		_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTramsform->Get_Matrix();

		mat2._42 += 0.2f;

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::START_DRIVEMODE0,
			initializer_list<any>{mat2, mat2, mat2, mat2, mat2,
			mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2,
			mat2, mat2, mat2, mat2 });

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::START_DRIVEMODE2,
			initializer_list<any>{mat2, mat2, mat2});

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::START_DRIVEMODE_BRAINITER,
			initializer_list<any>{mat2, mat2, mat2, mat2});

		for (_int i = 0; i < 7; i++)
		{
			RandomPos[i] = pPlayerTramsform->Get_State(TRANSFORM::POSITION);
			RandomPos[i] += Look;
			RandomPos[i].y += 1.75f;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<_float> xDist(-0.5f, 0.5f);
			std::uniform_real_distribution<_float> zDist(-0.5f, 0.5f);
			RandomPos[i].x += xDist(gen);
			RandomPos[i].z += zDist(gen);
		}

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::START_DRIVEMODE1,
			initializer_list<any>{
			RandomPos[0], RandomPos[1], RandomPos[2],
				RandomPos[3], RandomPos[4], RandomPos[5]
				, RandomPos[6]});
	}
#endif // DEBUG

	
}

void CScene_MindRoom::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
		CGameInstance::Get_Instance()->Clear_InteractiveObjects();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::HIDEOUT));
		//CGameInstance::Get_Instance()->Layer
	}
}

HRESULT CScene_MindRoom::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_MindRoom::Debug()
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

HRESULT CScene_MindRoom::Apply_PostProcessing()
{
	auto pGameInstance = CGameInstance::Get_Instance();

	// POSTPROCESS
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.1f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.5f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.9f);

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

HRESULT CScene_MindRoom::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}
	shared_ptr<CCamera_Player> pPlayerCam = 
		CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to CObjectLayer::Add");
	}

	pPlayerCam->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_BrainField.cin"), TEXT("Cutscene_BrainField"));
	pPlayerCam->Append_Cinematic(TEXT("Bin/Resources/Animation/CameraKeyframe/Cutscene_BrainField_Final.cin"), TEXT("Cutscene_BrainField_Final"));

#ifdef _DEBUG
	pPlayerCam->Set_DebugMode(false);
#endif

	shared_ptr<CObjectLayer> pCutsceneCamLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_CAMERA_CUTSCENE);
	if (nullptr == pCutsceneCamLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA_CUTSCENE");
	}
	shared_ptr<CCamera_Main> pCutsceneCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE);
	if (FAILED(pCutsceneCamLayer->Add(pCutsceneCamera)))
	{
		MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	pCutsceneCamera->Set_DebugMode(true);
#endif

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_PLAYER);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::MINDROOM))))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Player", "Failed to CObjectLayer::Add");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Pool(SCENE::MINDROOM, POOLOBJECT_DECAL_HITCRACK, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK, 50);

	shared_ptr<CObjectLayer> pTentacleLayer = pGameInstance->Add_Layer(SCENE::MINDROOM, LAYER_PLAYER_CUTSCENE_TENTACLE);
	for (_uint i = 0; i < 3; ++i)
	{
		auto pTentacle = pGameInstance->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE, SCENE::MINDROOM);
		pTentacle->Set_Enable(false);
		if (FAILED(pTentacleLayer->Add(pTentacle)))
		{
			MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Player", "Failed to CObjectLayer::Add");
		}
	}

	auto pPlayerEffectLayer = pGameInstance->Add_Layer(SCENE::MINDROOM, LAYER_PLAYER_RED_LASER_EYE);
	auto pEyeLaser = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_PLAYER_RED_LASER_EYE);
	if (FAILED(pPlayerEffectLayer->Add(pEyeLaser)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Player", "Failed to CObjectLayer::Add");
	}

	pEyeLaser->Set_Enable(false);

	// TODO: Test
	//pEyeLaser->Set_Enable(true);
	//dynamic_pointer_cast<CRedLaserEye>(pEyeLaser)->Set_Laser_Type(CRedLaserEye::LASER_TYPE::BIG);

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Lights()
{
	LIGHTDESC LightDesc;
	shared_ptr<CLight> pShadow;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .25;
	LightDesc.vDirection = (_float3(0.f, -1.f, 0.f)).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .25;
	LightDesc.vSpecular = _float4(1, 1, 1, 1);
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	CGameInstance::Get_Instance()->Add_Light(SCENE::MINDROOM, LightDesc, nullptr);

	LightDesc.eLightType = LIGHTTYPE::SHADOW;
	LightDesc.eShadowType = SHADOWTYPE::DIRECTIONAL;
	LightDesc.vShadowProjection = _float2(50.f, 50.f);
	//LightDesc.vPosition = _float3(-90.f, 90.f, -45.f);
	LightDesc.vPosition = _float3(-20.f, 20.f, -20.f);
	LightDesc.vDirection = _float3(1.f, -1.f, 0.6f);
	LightDesc.fRange = 100.f;
	pShadow = CGameInstance::Get_Instance()->Add_Light(SCENE::MINDROOM, LightDesc, nullptr);
	pShadow->Register_Layer(SCENE::MINDROOM, LAYER_PLAYER);
	pShadow->Register_Layer(SCENE::MINDROOM, LAYER_MONSTER);
	pShadow->Register_Layer(SCENE::MINDROOM, LAYER_TEAMMATE);
	pShadow->Set_ShadowDesc(SHADOWDESC{ 0.6f, -0.2f, 0.f, 0.05f, _int2(8, 8) });

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_BACKGROUND);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/Final_Cube.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to CObjectLayer::Add");
	}

	shared_ptr<CObjectLayer> pBrainFieldSkyLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_BACKGROUND_BRAINFIELD);
	if (nullptr == pBrainFieldSkyLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to Add_Layer: LAYER_BACKGROUND");
	}
	auto pMindRoomCube = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/MindRoom_Cube.dds")));
	if (FAILED(pBrainFieldSkyLayer->Add(pMindRoomCube)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	pMindRoomCube->Set_Enable(false);

	shared_ptr<CObjectLayer> pDecalTargetLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_DECALTARGET);
	if (nullptr == pDecalTargetLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to Add_Layer: LAYER_DECALTARGET");
	}

	auto pDecalTargetBuffer = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_DECAL_TARGET_BUFFER);
	if (FAILED(pDecalTargetLayer->Add(pDecalTargetBuffer)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Character", "Failed to Clone_GameObject::Add");
	}
	pDecalTargetBuffer->Set_Enable(false);

#ifdef TEST_CODE
//	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, TEXT("TestModel")))))
//	{
//		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_BackGround", "Failed to CObjectLayer::Add");
//	}
//	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MINDROOM, TEXT("TestShadow"),
//		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/TestMap/TestMap.mdl")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestShadow");
//	}
//	if (CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MINDROOM, TEXT("TestMap"),
//		CTestModel::Create(m_pDevice, m_pContext)))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_Mizuha", "Failed to Add_Component_Prototype: TestMap");
//	}
//	pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, TEXT("TestMap")));
#endif

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Teammate()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_TEAMMATE);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Mizuha::Ready_BattleField", "Failed to Add_Layer: LAYER_TEAMMATE");
	}

	_float4 vInitPosition = { 1.5f, 1.f, 11.f, 1.f };
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI_TEAMMATE, vInitPosition))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	vInitPosition = { -1.5f, 1.f, 11.f, 1.f };
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN_TEAMMATE, vInitPosition))))
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_MIZUHAUI", "Failed to Clone_GameObject::Add");
	}

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Monsters()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	shared_ptr<CObjectLayer> pMonster_ProjectileLayer = pGameInstance->Add_Layer(m_eScene, LAYER_MONSTER_PROJECTILE);
	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Add_Layer(m_eScene, LAYER_MONSTER);
	if (nullptr == pMonsterLayer)
	{
		MSG_RETURN(E_FAIL, "CMindmap::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}
	CMonster_Statue::STATUEDESC StatueDesc;

	StatueDesc.InitPosition = _float4(0.f, 0.f, 0.f, 0.f);
	StatueDesc.MaskType = CMonster_Statue::MASK_WHITE;
	pMonsterLayer->Add(pGameInstance->Clone_GameObject(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_STATUE, StatueDesc));
	
	StatueDesc.MaskType = CMonster_Statue::MASK_BLACK;
	pMonsterLayer->Add(pGameInstance->Clone_GameObject(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_STATUE, StatueDesc));
	
	StatueDesc.MaskType = CMonster_Statue::MASK_RED;
	pMonsterLayer->Add(pGameInstance->Clone_GameObject(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_STATUE, StatueDesc));

	//pMonsterLayer->Add(pGameInstance->Clone_GameObject(m_eScene, PROTOTYPE_GAMEOBJECT_MONSTER_KAREN));

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_UI()
{
	shared_ptr<CObjectLayer> pLayerUI3D = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_UI_3D);
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
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
		if (FAILED(pLayerUI3D->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CHARACTER, tMenuCharacterDesc))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
		}
	}

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Add_Layer: LAYER_UI");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_HPBACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_HP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_PSYCOWAVE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMBOX))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMTAG))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGERKEY_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BLACK_BACKGROUND))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_TOOLTIP_PMCC))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_ITEMHUD_B))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_GAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_DRIVE_SIDEGAUGE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_CHANGERICON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_SKILLCHANGER_LIGHT))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_PARTY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_ITEM))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_EQUIP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_BRAINMAP))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_SAS))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_STORY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_LIBRARY))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_CATEGORY_OPTION))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_L))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_ARROW_R))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_BUTTON))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYLEADER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYMEMBER2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT1))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_PARTYSUPPORT2))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_SEPERATE_LINE))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_PROFILE_ID_CARD))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MENU_FADE_FILLTER))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	//}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_MODEL_MENU))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_NAME_TOWN))))
	{
		MSG_RETURN(E_FAIL, "CScene_SUOH::Ready_SUOHUI", "Failed to Clone_GameObject::Add");
	}
	//if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT))))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestUI", "Failed to Clone_GameObject::Add");
	//}
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, 50);
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, 5);

	return S_OK;
}

HRESULT CScene_MindRoom::Ready_UI_Late()
{
	/*shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_INTERACTIVE_LATE);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC_LATE");
	}*/

	return S_OK;
}

HRESULT CScene_MindRoom::Add_Dynamic()
{
	int dd[5] = { -2,-1,0,1,2 };
	for (_int i = 0; i < 1; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			_float3 TmpTranslation = { cosf(XMConvertToRadians(j*18.f)) * 25.f, 10.f, sinf(XMConvertToRadians(j * 18.f)) * 25.f};

			_float4x4 Mat = XMMatrixIdentity();
			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObj
				= { SCENE::MINDROOM, TEXT("Mindroom_Box"), _float3(1.f,1.f,1.f),
				Function::RandomVector(_float3(0.f,0.f,0.f),_float3(360.f,360.f,360.f)),
				TmpTranslation, Mat, TEXT("Mindroom_Box") };

			wstring protoType;
			_int ran = rand() % 3;
			if (ran == 0)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX;
			else if(ran == 1)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA;
			else if (ran == 2)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB;

			if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, protoType,
				std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObj)))))
			{
				MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
			}
		}

		/*		if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX,
					std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObj)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
				}

				if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA,
					std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObj)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
				}*/
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveObject(SCENE::MINDROOM, LAYER_MAP_DYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}
	return S_OK;
}

HRESULT CScene_MindRoom::Clear_Dynamic()
{
	shared_ptr<CObjectLayer> pObjLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC);
	pObjLayer->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pObject)->bool
		{
			shared_ptr<CMap_MindRoom_Dynamic> pObj = dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_pObject);
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr == pObj)
				return true;
			pObj->Set_Dissolve(true);
		});

	CGameInstance::Get_Instance()->Clear_InteractiveObjects();
	return S_OK;
}

HRESULT CScene_MindRoom::Add_Dynamic_Throw()
{
	int dd[5] = { -2,-1,0,1,2 };
	for (_int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			_float3 TmpTranslation = { Function::RandomFloat(-25.f,25.f) , 2.f,  Function::RandomFloat(-25.f,25.f) };

			_float4x4 Mat = XMMatrixIdentity();
			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObj
				= { SCENE::MINDROOM, TEXT("Mindroom_Box"), _float3(1.f,1.f,1.f),
				Function::RandomVector(_float3(0.f,0.f,0.f),_float3(360.f,360.f,360.f)),
				TmpTranslation, Mat, TEXT("Mindroom_Box") };

			wstring protoType;
			_int ran = rand() % 3;
			if (ran == 0)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX;
			else if (ran == 1)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEA;
			else if (ran == 2)
				protoType = PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_CRATEB;

			if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, protoType,
				std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObj)))))
			{
				MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
			}
		}

		shared_ptr<CObjectLayer> pObjLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MAP_DYNAMIC);
		pObjLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMap_MindRoom_Dynamic> pObj = dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_pObject);
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM));
				if (nullptr == pObj)
					return true;
				pObj->SetPhysX(true);
				pObj->Get_Actor()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
				pObj->Set_Gravity(true);
				pObj->Get_ActorDesc().bActive = true;
			});
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveObject(SCENE::MINDROOM, LAYER_MAP_DYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}
	return S_OK;
}

HRESULT CScene_MindRoom::Ready_Map()
{
	m_pRoadLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_ROAD);
	m_pStaticLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_STATIC);
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_DYNAMIC);
	m_pBuildingLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_BUILDING);
	m_pSpecialDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_SPECIALDYNAMIC);
	m_pETCLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MINDROOM, LAYER_MAP_ETC);

	Load_MapFile(TEXT("Common_BaseWall"), 1);

#pragma region Road
	Load_MapFile(TEXT("Sankaku_futa"), 0); // PBR
	Load_MapFile(TEXT("MindRoom_Water"), 0); // PBR

#pragma endregion

#pragma region Static
	Load_MapFile(TEXT("Sankaku_A"), 1); // PBR
	Load_MapFile(TEXT("Mindroom_StoneA"), 1); // PBR
	Load_MapFile(TEXT("Mindroom_StoneB"), 1); // PBR
	Load_MapFile(TEXT("Mindroom_StoneC"), 1); // PBR
	Load_MapFile(TEXT("Mindroom_Spaceship"), 1); // PBR

#pragma endregion

#pragma region Dynamic
	//for (size_t i = 0; i < 40; i++)
	//	if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM, PROTOTYPE_GAMEOBJECT_MAP_MINDROOM_BOX))));

	//Load_MapFile(TEXT("Mindroom_Box"), 2);
	//Load_MapFile(TEXT("Mindroom_CrateA"), 2);
	//Load_MapFile(TEXT("Mindroom_CrateB"), 2);
#pragma endregion

#pragma region Neon
	Load_MapFile(TEXT("MindRoom_Tenkyu"), 5);
	Load_MapFile(TEXT("MindRoom_Floor"), 5);
	Load_MapFile(TEXT("MindRoom_Twist"), 5);
	Load_MapFile(TEXT("MindRoom_Redline"), 5);
#pragma endregion


	return S_OK;
}

HRESULT CScene_MindRoom::Load_MapFile(wstring strObjName, _int iIndex)
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("Bin/Resources/Map/MindRoom/dat/MindRoom_Test.txt"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
			m_strPrototypeName = CMap_ToolManager::Get_Instance()->MindRoom_Name(m_strObjName);

			std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObjTuple_Road0
				= { SCENE::MINDROOM, m_strObjName, Scale,Rotation,Tanslation, Mat, Name };

			if (iIndex == 0)
			{
				if (FAILED(m_pRoadLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 1)
			{
				if (FAILED(m_pStaticLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 2)
			{
				if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 3)
			{
				if (FAILED(m_pSpecialDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}

			else if (iIndex == 4)
			{
				if (FAILED(m_pBuildingLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
					m_strPrototypeName, std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObjTuple_Road0)))))
				{
					MSG_RETURN(E_FAIL, "CScene_Test::Ready_SuohMap", "Failed to Clone_GameObject::Add");
				}
			}
			else if (iIndex == 5)
			{
				if (FAILED(m_pETCLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MINDROOM,
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

shared_ptr<CScene_MindRoom> CScene_MindRoom::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_MindRoom> pInstance = make_private_shared(CScene_MindRoom, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_MindRoom", "Failed To CInitialize");
	}

	return pInstance;
}
