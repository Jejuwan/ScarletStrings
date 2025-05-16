#include "ClientPCH.h"
#include "PlayerBrainFieldCutsceneState.h"

#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"

#include "PlayerIdleState.h"
#include "PlayerBrainFieldCutsceneFinalState.h"

#include "BrainFieldTentacle.h"
#include "Map_MindRoom_Road.h"
#include "SkyCube.h"
#include "Map_ToolManager.h"
#include "RedLaserEye.h"
#include "Effect_Manager.h"
#include "Bone.h"
#include "PlayerTentacle.h"
#include "ScreenEffectManager.h"
#ifdef _DEBUG
#include "Animation.h"
#endif


shared_ptr<CPlayerState> CPlayerBrainFieldCutsceneState::Transition(CPlayer& _player)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(VK_RETURN))
	{
		auto pPlayerCamera = _player.Get_Camera();
		auto pPlayerCamTransform = pPlayerCamera->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		pPlayerCamTransform->Set_Matrix(XMMatrixIdentity());

		auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
		auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
		pSky->Reset();

		auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
		for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
		{
			_float4x4 matPivot = XMMatrixRotationX(XMConvertToRadians(180.f));
			dynamic_pointer_cast<CBrainFieldTentacle>(*pObject)->Set_Pivot(matPivot);
		}

		CMap_ToolManager::Get_Instance()->Set_Is_MindRoom(true);

		for (shared_ptr<CPlayerTentacle> elem : _player.Get_Tentacles())
		{
			elem->Set_Fly(true);
			elem->Set_Render(true);
			//CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_PARTICLE, make_pair(static_pointer_cast<CGameObject>(elem), "cable_02_a"));
		}

		return make_shared<CPlayerIdleState>();
	}

	if (pGameInstance->Key_Down('N'))
	{
		return make_shared<CPlayerBrainFieldCutsceneState>();
	}

	if (m_bBrainFieldOpened)
	{
		return make_shared<CPlayerBrainFieldCutsceneFinalState>();
	}

	return nullptr;
}

void CPlayerBrainFieldCutsceneState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD1, 2.f, false, 0.f, false);

	auto pPlayerCamera = _player.Get_Camera();

	auto pPlayerTransform = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pPlayerCamera->Play_CinematicCamera(TEXT("Cutscene_BrainField"), pPlayerTransform->Get_Matrix());

	_player.Enable_BrainField(true);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
	
	_uint iTentacleIndex = 0;
	for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
	{
		auto pTentacle = (*pObject)->Get_Component<CModel>(COMPONENT::MODEL);
		pTentacle->Reset_Animation((_uint)CBrainFieldTentacle::ANIMATION::BRAIN_FIELD_OPEN_C01_CH0200_1 + iTentacleIndex);
		pTentacle->Set_Animation((_uint)CBrainFieldTentacle::ANIMATION::BRAIN_FIELD_OPEN_C01_CH0200_1 + iTentacleIndex, 2.f, false, 0.f, false);

		++iTentacleIndex;
	}

	if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
	{
		auto pModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CPlayerBrainFieldCutsceneState::Event0, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CPlayerBrainFieldCutsceneState::Event1, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CPlayerBrainFieldCutsceneState::Event2, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CPlayerBrainFieldCutsceneState::Event3, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5, std::bind(&CPlayerBrainFieldCutsceneState::Event4, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION6, std::bind(&CPlayerBrainFieldCutsceneState::Event5, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION7, std::bind(&CPlayerBrainFieldCutsceneState::Event6, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION8, std::bind(&CPlayerBrainFieldCutsceneState::Event7, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION9, std::bind(&CPlayerBrainFieldCutsceneState::Event8, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION10, std::bind(&CPlayerBrainFieldCutsceneState::Event9, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION11, std::bind(&CPlayerBrainFieldCutsceneState::Event10, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION12, std::bind(&CPlayerBrainFieldCutsceneState::Event11, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION13, std::bind(&CPlayerBrainFieldCutsceneState::Event12, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION14, std::bind(&CPlayerBrainFieldCutsceneState::Event13, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION15, std::bind(&CPlayerBrainFieldCutsceneState::Event14, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION16, std::bind(&CPlayerBrainFieldCutsceneState::Event15, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION17, std::bind(&CPlayerBrainFieldCutsceneState::Event16, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION18, std::bind(&CPlayerBrainFieldCutsceneState::Event17, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION19, std::bind(&CPlayerBrainFieldCutsceneState::Event18, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION20, std::bind(&CPlayerBrainFieldCutsceneState::Event19, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION21, std::bind(&CPlayerBrainFieldCutsceneState::Event20, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION22, std::bind(&CPlayerBrainFieldCutsceneState::Event21, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION23, std::bind(&CPlayerBrainFieldCutsceneState::Event22, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION24, std::bind(&CPlayerBrainFieldCutsceneState::Event23, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION25, std::bind(&CPlayerBrainFieldCutsceneState::Event24, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION26, std::bind(&CPlayerBrainFieldCutsceneState::Event25, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION27, std::bind(&CPlayerBrainFieldCutsceneState::Event26, this));
	}

	m_fOriginBloomStrength = pGameInstance->Get_BloomStrength(IMAGE_PROCESS::PROCESS_NEON);
	m_fOriginExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_NEON);
	m_fOriginGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_NEON);

	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);

	CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);

	//auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	//auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	//pBrainFieldSky->Reset();

	//CGameInstance::Get_Instance()->PlaySoundW(TEXT(""), g_fPlayerSound);
}

void CPlayerBrainFieldCutsceneState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	__super::Late_Update(_player, fTimeDelta);

#ifdef _DEBUG
	auto pGameInstance = CGameInstance::Get_Instance();

	auto pPlayerModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
	auto pAnimation = pPlayerModel->Get_Animation((_uint)ANIMATION::PLAYER::BRAINFIELD1);

	ImGui::Begin("BrainFieldAnimationSettings");

	static _float fAnimSpeed = 1.f;
	static _bool bPaused = true;
	if (ImGui::SliderFloat("Anim Speed", &fAnimSpeed, 0.f, 1.f))
	{
		if (false == bPaused)
			pAnimation->Set_AnimSpeed(fAnimSpeed);
	}

	_float fTrackPosition = pAnimation->Get_TrackPosition();
	_float fDuration = pAnimation->Get_Duration();
	if (ImGui::SliderFloat("TrackPosition", &fTrackPosition, 0.f, fDuration))
	{
		pAnimation->Set_TrackPosition(fTrackPosition);
	}

	if (ImGui::Checkbox("Pause", &bPaused))
	{
		if (bPaused)
		{
			pAnimation->Set_AnimSpeed(0.f);
		}
		else
		{
			pAnimation->Set_AnimSpeed(fAnimSpeed);
		}
	}

	if (pGameInstance->Key_Down(VK_SPACE))
	{
		bPaused = false;

		pAnimation->Reset();
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD1, fAnimSpeed, false, 0.f, false);
	}

	_float3 vPlayerPos = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
	ImGui::InputFloat3("Player Position", &vPlayerPos.x);

	ImGui::End();

#endif

	//auto pPlayerCamera = _player.Get_Camera();
	//auto pPlayerCamTransform = pPlayerCamera->Get_Component<CTransform>(COMPONENT::TRANSFORM);
}

void CPlayerBrainFieldCutsceneState::Exit(CPlayer& _player)
{
	__super::Exit(_player);

	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, m_fOriginBloomStrength);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, m_fOriginExposure);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, m_fOriginGamma);
}

void CPlayerBrainFieldCutsceneState::Event0()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	
	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event1()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event2()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event3()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event4()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event5()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event6()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	// ¹Ù´Ú ¿¬º¸¶ó»ö
	auto pRoadLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ROAD);
	pRoadLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pRoad = dynamic_pointer_cast<CMap_MindRoom_Road>(pObject);
			pRoad->Set_SingleColor_Lerp({ 0.72f, 0.23f, 0.72f });

			return true;
		});
	
	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
	pMonsterLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			pObject->Set_Enable(false);

			return true;
		});

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

		mat2._42 += 1.f;

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
}

void CPlayerBrainFieldCutsceneState::Event7()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	// ¹Ù´Ú Â£Àº º¸¶ó»ö
}

void CPlayerBrainFieldCutsceneState::Event8()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->Set_LerpColor({ 0.87f, 0.52f, 0.87f }, { 0.f, 0.f, 0.f }, 2.f);
	
	auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	pSky->Set_LerpColor({ 0.87f, 0.52f, 0.87f }, { 0.f, 0.f, 0.f }, 2.f);
}

void CPlayerBrainFieldCutsceneState::Event9()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	// ¹Ù´Ú °ËÁ¤
	// ÇÏ´Ã ¿¬º¸¶ó

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
	pTeammateLayer->Iterate_Objects([](shared_ptr<CGameObject> pTeammate)
		{
			pTeammate->Set_Enable(false);
			return true;
		});

	auto pStaticLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_STATIC);
	pStaticLayer->Iterate_Objects([](shared_ptr<CGameObject> pStaticObject)
		{
			pStaticObject->Set_Enable(false);
			return true;
		});


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
}

void CPlayerBrainFieldCutsceneState::Event10()
{
	// ¾È±¤
	// ¹è°æ ±×¶óµ¥ÀÌ¼Ç

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 1.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 1.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	auto pRoadLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ROAD);
	pRoadLayer->Iterate_Objects([](shared_ptr<CGameObject> pRoad)
		{
			pRoad->Set_Enable(false);
			return true;
		});

	auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	pSky->Set_Enable(false);
	pSky->Reset();
	//pSky->Set_GradationBackGround({ 0.f, 0.f, 0.f }, { 0.11f, 0.05f, 0.11f });

	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->Set_Enable(true);
	pBrainFieldSky->Set_GradationBackGround({ 0.f, 0.f, 0.f }, { 0.11f, 0.05f, 0.11f });

	auto pPlayerEffectLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_RED_LASER_EYE);
	auto pEyeLaser = pPlayerEffectLayer->Get_GameObject(0);
	pEyeLaser->Set_Enable(true);
	dynamic_pointer_cast<CRedLaserEye>(pEyeLaser)->Set_Laser_Type(CRedLaserEye::LASER_TYPE::SMALL);
}

void CPlayerBrainFieldCutsceneState::Event11()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayerEffectLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_RED_LASER_EYE);
	auto pEyeLaser = pPlayerEffectLayer->Get_GameObject(0);
	pEyeLaser->Set_Enable(false);
	dynamic_pointer_cast<CRedLaserEye>(pEyeLaser)->Set_Laser_Type(CRedLaserEye::LASER_TYPE::NONE);

	auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);

	for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
	{
		_float4x4 matPivot = XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(-180.f));
		dynamic_pointer_cast<CBrainFieldTentacle>(*pObject)->Set_Pivot(matPivot);
	}

	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.f, 0.f, 1.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.f, 1.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event12()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);

	for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
	{
		dynamic_pointer_cast<CBrainFieldTentacle>(*pObject)->Dissolve_Stripe();
	}

	{
		shared_ptr<CTransform> pPlayerTramsform =
			CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

		_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTramsform->Get_Matrix();

		mat2._42 += 0.2f;

		CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::START_DRIVEMODE_BRAINITER1,
			initializer_list<any>{mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2, mat2});
	}
}

void CPlayerBrainFieldCutsceneState::Event13()
{
	// Æø¹ß
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Enter1.wav"), 3.f);

	auto pGameInstance = CGameInstance::Get_Instance();
	//auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	//auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	//pSky->Set_GradationBackGround({ 0.f, 0.f, 0.f }, { 0.53f, 0.f, 0.1f });
	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->Set_GradationBackGround({ 0.f, 0.f, 0.f }, { 0.53f, 0.f, 0.1f });

	pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.5f);
	pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 1.8f);
	pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 0.8f);

	m_pRenderer->Enable_RadialBlur(true);

	auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
	for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
	{
		dynamic_pointer_cast<CBrainFieldTentacle>(*pObject)->EmissiveMode();
	}

	auto pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.02f, 1.02f, 1.02f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.04f, 1.04f, 1.04f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.f, 0.f };
	tRimDesc.vScale = { 1.07f, 1.07f, 1.07f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 1.f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);


}

void CPlayerBrainFieldCutsceneState::Event14()
{
	// ³¡
	m_bBrainFieldOpened = true;

	m_pRenderer->Enable_RadialBlur(false);
}

void CPlayerBrainFieldCutsceneState::Event15()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event16()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event17()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event18()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event19()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event20()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event21()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.1f, 1.1f, 1.1f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event22()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.1f, 1.1f, 1.1f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event23()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.15f, 1.15f, 1.15f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event24()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.1f, 1.1f, 1.1f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.05f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.4f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 1.f, 0.5f, 0.15f };
	tRimDesc.vScale = { 1.2f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.72f, 0.23f, 0.72f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event25()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.23f, 0.23f, 1.f };
	tRimDesc.vScale = { 1.1f, 1.1f, 1.1f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.23f, 0.23f, 1.f };
	tRimDesc.vScale = { 1.05f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.23f, 0.23f, 1.f };
	tRimDesc.vScale = { 1.2f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.23f, 0.23f, 1.f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}

void CPlayerBrainFieldCutsceneState::Event26()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());

	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.76f, 1.f, 0.05f };
	tRimDesc.vScale = { 1.12f, 1.1f, 1.1f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.76f, 1.f, 0.05f };
	tRimDesc.vScale = { 1.05f, 1.05f, 1.2f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = true;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.76f, 1.f, 0.05f };
	tRimDesc.vScale = { 1.2f, 1.05f, 1.05f };
	pPlayer->Add_RimEffect(tRimDesc);

	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 0.05f;
	tRimDesc.fRimPower = 5.f;
	tRimDesc.fRimStrength = 0.5f;
	tRimDesc.vRimColor = { 0.76f, 1.f, 0.05f };
	tRimDesc.vScale = { 11.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);
}
