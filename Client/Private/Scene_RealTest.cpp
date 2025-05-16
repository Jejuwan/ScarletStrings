#include "ClientPCH.h"
#include "Scene_RealTest.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Player.h"
#include "Light.h"
#include "PhysXEngine.h"

#include "UI_Manager.h"

#include "Scene_Load.h"
#include "Effect_Manager.h"

#define TEST_CODE
//#define TEST_SHADOW

#include "CutsceneTentacle.h"

#ifdef _DEBUG
#include "TestModel.h"
#include "Monster_FallenObject.h"
#include "CameraKeyframeGenerator.h"
#endif
#include "Bone.h"

#include "Scene_Mizuha.h"

CScene_RealTest::CScene_RealTest(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::REALTEST)
{
}

HRESULT CScene_RealTest::Initialize()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Show_Cursor(true);
	pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.8f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.975f);

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

	if (FAILED(Ready_Character()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_TestObject");
	}

	if (FAILED(Ready_TestObject(PROTOTYPE_GAMEOBJECT_MONSTER_FLOWERCHEST)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_TestObject");
	}

	if (FAILED(Ready_TestUI()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestUI");
	}

	if (FAILED(Ready_Lights()))
	{
		MSG_RETURN(E_FAIL, "CScene_Text::Initialize", "Failed to Ready_TestUI_Late");
	}

	if (FAILED(Ready_TestDynamic()))
	{
		MSG_RETURN(E_FAIL, "CScene_RealTest::Initialize", "Failed to Ready_TestDynamic");
	}

	if (FAILED(CGameInstance::Get_Instance()->Append_InteractiveObject(SCENE::REALTEST, LAYER_MAP_DYNAMIC)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to Append_InteractiveObject");
	}
	CScene_Mizuha::Apply_PostProcessing();
	CGameInstance::Get_Instance()->Fix_Cursor(false);
	return S_OK;
}

void CScene_RealTest::Tick(_float fTimeDelta)
{
#ifdef _DEBUG
	auto pDataManager = CUI_Manager::Get_Instance();
	pDataManager->Debug_Render();
#endif

	if (CGameInstance::Get_Instance()->Key_Down('U'))
	{
		shared_ptr<CTransform> pPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

		_float3 Look = pPlayerTramsform->Get_State(TRANSFORM::LOOK);
		  
		Look *= 20.f;

		_float3 Pos = pPlayerTramsform->Get_State(TRANSFORM::POSITION) + Look;
		Pos.y += 5.f;

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK0,
			initializer_list<any>{Pos, Pos});

		_float3 RandomPos[6] = {};


		for (_int i = 0; i < 6; i++)
		{
			RandomPos[i] = pPlayerTramsform->Get_State(TRANSFORM::POSITION);
			RandomPos[i] += Look;
			RandomPos[i].y += 1.75f;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<_float> xDist(-1.5f, 1.5f);
			std::uniform_real_distribution<_float> yDist(-1.f, 1.f);
			std::uniform_real_distribution<_float> zDist(-1.5f, 1.5f);
			RandomPos[i].x += xDist(gen);
			RandomPos[i].y += yDist(gen);
			RandomPos[i].z += zDist(gen);
		}

		/*CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK1,
			initializer_list<any>{RandomPos[0], RandomPos[1], RandomPos[2], RandomPos[3], RandomPos[4], RandomPos[5]});*/
	}
	/*m_fEffect_Timer += fTimeDelta;

	if (m_fEffect_Timer >= 0.5f)
	{

		shared_ptr pPlayerTransform = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM));

		_float3 Pos = pPlayerTransform->Get_State(TRANSFORM::POSITION);
		Pos.y += 2.f;

		const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

		_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTransform->Get_Matrix();

		mat2._42 += 0.25f;

		CEffect_Manager::Get_Instance()->Fetch(VFX::BRAINFIELD_BRAINITER, mat2);

		m_fEffect_Timer = 0.f;
	}*/
}

void CScene_RealTest::Late_Tick(_float fTimeDelta)
{
	// TODO: TestCode
	if (CGameInstance::Get_Instance()->Key_Down(VK_NEXT))
	{
		CGameInstance::Get_Instance()->ClearPhysX();
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::TEST));
	}
}

HRESULT CScene_RealTest::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_RealTest::Debug()
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
		if (ImGui::BeginMenu("UI_3D"))
		{
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_UI_3D).Get(), ImVec2(480, 270));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Neon"))
		{
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NEON).Get(), ImVec2(480, 270));
			ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NEON_COMBINE).Get(), ImVec2(480, 270));
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Shadow"))
		{
		if (ImGui::BeginMenu("Shadow"))
		{
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

		CGameInstance::Get_Instance()->Iterate_Shadows(
			[&](shared_ptr<CLight> pLight)
			{
				ImGui::Image(CGameInstance::Get_Instance()->Get_Shadow(pLight).Get(), ImVec2(200, 200));
				ImGui::SameLine();
			}
		);

		ImGui::EndMenu();
		}
		ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		}
	}
#endif
}
#endif

HRESULT CScene_RealTest::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_CAMERA);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	shared_ptr<CCamera_Player> pPlayerCam = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Player>(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_PLAYER);
	if (FAILED(pLayer->Add(pPlayerCam)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to CObjectLayer::Add");
	}

#ifdef _DEBUG
	//pPlayerCam->Set_DebugMode(false);
#endif

	return S_OK;
}

HRESULT CScene_RealTest::Ready_Lights()
{
	LIGHTDESC LightDesc;

	LightDesc.eLightType = LIGHTTYPE::DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1, 1, 1, 1) * .25;
	LightDesc.vDirection = (_float3(-1, -1, -1)).normalize();
	LightDesc.vAmbient = _float4(1, 1, 1, 1) * .25;
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	CGameInstance::Get_Instance()->Add_Light(SCENE::REALTEST, LightDesc, nullptr);

	return S_OK;
}

HRESULT CScene_RealTest::Ready_BackGround()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_BACKGROUND);

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_SKYCUBE,
		std::make_any<wstring>(TEXT("Bin/Resources/Cubemap/Final_Cube.dds"))))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
	return S_OK;
}

HRESULT CScene_RealTest::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_PLAYER);

	auto pPlayer = CGameInstance::Get_Instance()->Clone_GameObject<CPlayer>(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER, SCENE::REALTEST);
	if (FAILED(pLayer->Add(pPlayer)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Pool(SCENE::REALTEST, POOLOBJECT_DECAL_HITCRACK, PROTOTYPE_GAMEOBJECT_DECAL_HITCRACK, 50);

	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeDecreaseTime = 0.f;

	// TODO: �ƾ� ���߿� �׽�Ʈ �ڵ���.
//#ifdef _DEBUG
//	{
//		pPlayer->Get_PlayerDesc().fGravity = 0.f;
//		auto pPlayerTransform = pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM);
//		pPlayerTransform->Set_Matrix(XMMatrixIdentity());
//		pPlayer->Get_Controller()->setFootPosition(PxExtendedVec3(0.f, 0.f, 0.f));
//
//		pPlayer->Get_Camera()->Lock(true);
//
//		shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_DEBUG_OBJECT);
//		if (nullptr == pLayer)
//		{
//			MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to Add_Layer: LAYER_DEBUG_OBJECT");
//		}
//		shared_ptr<CCameraKeyframeGenerator> pCameraTool =
//			CGameInstance::Get_Instance()->Clone_GameObject<CCameraKeyframeGenerator>(SCENE::REALTEST,
//				PROTOTYPE_GAMEOBJECT_CAMERA_KEYFRAME_GENERATOR);
//		if (FAILED(pLayer->Add(pCameraTool)))
//		{
//			MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_DebugObjects", "Failed to CObjectLayer::Add");
//		}
//
//		shared_ptr<CObjectLayer> pCutsceneCamLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_CAMERA_CUTSCENE);
//		if (nullptr == pCutsceneCamLayer)
//		{
//			MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA_CUTSCENE");
//		}
//		shared_ptr<CCamera_Main> pCutsceneCamera = CGameInstance::Get_Instance()->Clone_GameObject<CCamera_Main>(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CAMERA_CUTSCENE);
//		if (FAILED(pCutsceneCamLayer->Add(pCutsceneCamera)))
//		{
//			MSG_RETURN(E_FAIL, "CCutscene_Mizuha::Ready_Camera", "Failed to CObjectLayer::Add");
//		}
//
//		pCutsceneCamera->Set_DebugMode(true);
//		CPipeLine::Get_Instance()->Set_Camera(pCutsceneCamera);
//
//		shared_ptr<CObjectLayer> pTentacleLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_PLAYER_CUTSCENE_TENTACLE);
//		for (_uint i = 0; i < 3; ++i)
//		{
//			auto pTentacle = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_PLAYER_CUTSCENE_TENTACLE, SCENE::MINDROOM);
//			pTentacle->Set_Enable(false);
//			if (FAILED(pTentacleLayer->Add(pTentacle)))
//			{
//				MSG_RETURN(E_FAIL, "CScene_MindRoom::Ready_Player", "Failed to CObjectLayer::Add");
//			}
//		}
//	}
//#endif

	return S_OK;
}

HRESULT CScene_RealTest::Ready_Character()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_CHARACTER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_TestObject", "Failed to Add_Layer: LAYER_CHARACTER");
	}

	auto pArashi = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_ARASHI);
	if (FAILED(pLayer->Add(pArashi)))
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pShiden = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_SHIDEN, true);
	if (FAILED(pLayer->Add(pShiden)))
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKyoka = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KYOKA, true);
	if (FAILED(pLayer->Add(pKyoka)))
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKagero = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KAGERO);
	if (FAILED(pLayer->Add(pKagero)))
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pKasane = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_CHARACTER_KASANE, true);
	if (FAILED(pLayer->Add(pKasane)))
	{
		MSG_RETURN(E_FAIL, "CScene_REALTEST::Ready_Character", "Failed to Clone_GameObject::Add");
	}

	auto pUIManager = CUI_Manager::Get_Instance();
	pUIManager->Bind_Character(CHARACTER::ARASHI, pArashi);
	pUIManager->Bind_Character(CHARACTER::SHIDEN, pShiden);
	pUIManager->Bind_Character(CHARACTER::KYOKA, pKyoka);
	pUIManager->Bind_Character(CHARACTER::KAGERO, pKagero);
	pUIManager->Bind_Character(CHARACTER::KASANE, pKasane);

	return S_OK;
}

HRESULT CScene_RealTest::Ready_TestObject(const wstring& _wstrProto)
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_STATIC);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}
	TESTOBJDESC desc;
	desc.vSpawnPos = { 0,0,0 };
	desc.eType = TESTOBJDESC::STATIC;
	shared_ptr<CGameObject> pTestCube;
	if (FAILED(pLayer->Add(pTestCube = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_TESTCUBE, &desc))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
#ifdef CUBE_SCALE
	pTestCube->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(CUBE_SCALE);
#endif
	shared_ptr<CObjectLayer> pMonster_ProjectileLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_MONSTER_PROJECTILE);
	shared_ptr<CObjectLayer> pMonsterLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_MONSTER);
	if (nullptr == pMonsterLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_TestObject", "Failed to Add_Layer: LAYER_STATIC");
	}

	for(int i=0;i<1;i++)
	{
		_float4 MonsterInitializePos = _float4(-50 + 10 * i, 5, i, 1.f);
		if (FAILED(pMonsterLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, _wstrProto, MonsterInitializePos))))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to CObjectLayer::Add");
		}
	}
#ifdef TEST_CODE
	shared_ptr<CGameObject> pObject = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST,
		PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP, _float4(0.f, 10.f, 0.f, 1.f));
	pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, _float3(0.f, 5.f, 0.f));
	//pObject->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Set_Gravity(false);
	
	if (FAILED(pMonsterLayer->Add(pObject)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_BackGround", "Failed to CObjectLayer::Add");
	}
#endif

#ifdef TEST_SHADOW
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::REALTEST, TEXT("TestShadow"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/TestMap/TestMap.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_REALTEST", "Failed to Add_Component_Prototype: TestShadow");
	}
	//if (CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::REALTEST, TEXT("TestMap"),
	//	CTestModel::Create(m_pDevice, m_pContext)))
	//{
	//	MSG_RETURN(E_FAIL, "CLoader::Load_Cutscene_REALTEST", "Failed to Add_Component_Prototype: TestMap");
	//}
	//pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, TEXT("TestMap")));
#endif

	return S_OK;
}

HRESULT CScene_RealTest::Ready_TestUI()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_UI);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_MIZUHA::Ready_REALTESTUI", "Failed to Add_Layer: LAYER_UI");
	}

	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_DAMAGE_FONT, 50);
	CGameInstance::Get_Instance()->Add_Pool(m_eScene, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT, 5);

	return S_OK;
}

HRESULT CScene_RealTest::Ready_TestDynamic()
{
	m_pDynamicLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::REALTEST, LAYER_MAP_DYNAMIC);

	for (_int i = 0; i < 100; i++)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<_float> RandomNumberX(-30.f, 30.f);
		std::uniform_real_distribution<_float> RandomNumberZ(-30.f, 30.f);

		_float3 TmpTranslation = { RandomNumberX(gen), 5.f, RandomNumberZ(gen) };

		_float4x4 Mat = XMMatrixIdentity();
		std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring > tObj
			= { SCENE::REALTEST, TEXT("ConcreteBarrierA"), _float3(1.f,1.f,1.f),
			_float3(0.f,0.f,0.f),
			TmpTranslation, Mat, TEXT("ConcreteBarrierA") };

		if (FAILED(m_pDynamicLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA,
			std::make_any<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(tObj)))))
		{
			MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
		}
	}
#ifdef _DEBUG
	//CMonster_FallenObject::FALLENDESC FallenDesc;
	//FallenDesc.InitPos = { Function::RandomFloat(-100.f, 100.f), Function::RandomFloat(30.f, 100.f), Function::RandomFloat(-100.f, 100.f), 0.f };
	//FallenDesc.Randomize = true;
	//auto Object = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::REALTEST, L"Test_FallingObject");
	//Object->Fetch(FallenDesc);
	//if (FAILED(m_pDynamicLayer->Add(Object)))
	//{
	//	MSG_RETURN(E_FAIL, "CScene_Realtest::Initialize", "Failed to Clone_DynamicObject");
	//}
#endif
	return S_OK;
}

shared_ptr<CScene_RealTest> CScene_RealTest::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_RealTest> pInstance = make_private_shared(CScene_RealTest, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Load", "Failed To CInitialize");
	}


	return pInstance;
}
