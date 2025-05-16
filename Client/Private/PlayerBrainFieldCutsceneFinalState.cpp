#include "ClientPCH.h"
#include "PlayerBrainFieldCutsceneFinalState.h"

#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "SkyCube.h"
#include "BrainFieldTentacle.h"
#include "Map_ToolManager.h"
#include "Animation.h"
#include "Map_MindRoom_Neon.h"
#include "RedLaserEye.h"

#include "PlayerIdleState.h"
#include "Map_MindRoom_Road.h"
#include "DecalTargetBuffer.h"

#include "Monster.h"

#include "PlayerTentacle.h"
#include "Scene_MindRoom.h"
#include "Effect_Manager.h"
#include "ScreenEffectManager.h"

shared_ptr<CPlayerState> CPlayerBrainFieldCutsceneFinalState::Transition(CPlayer& _player)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(VK_RETURN) || m_bCutsceneFinished)
	{
		auto pPlayerCamera = _player.Get_Camera();
		auto pPlayerCamTransform = pPlayerCamera->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		pPlayerCamTransform->Set_Matrix(XMMatrixIdentity());

		auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
		auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
		pSky->Reset();

		auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
		auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
		pBrainFieldSky->Reset();

		auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
		for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
		{
			_float4x4 matPivot = XMMatrixRotationX(XMConvertToRadians(45.f)) * XMMatrixRotationY(XMConvertToRadians(-90.f));
			dynamic_pointer_cast<CBrainFieldTentacle>(*pObject)->Set_Pivot(matPivot);
		}


		CMap_ToolManager::Get_Instance()->Set_Is_MindRoom(true);

		return make_shared<CPlayerIdleState>();
	}

	if (pGameInstance->Key_Down('M'))
	{
		return make_shared<CPlayerBrainFieldCutsceneFinalState>();
	}

	return nullptr;
}

void CPlayerBrainFieldCutsceneFinalState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	auto pPlayerCamera = _player.Get_Camera();
	auto pPlayerTransform = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pPlayerCamera->Play_CinematicCamera(TEXT("Cutscene_BrainField_Final"), pPlayerTransform->Get_Matrix());

	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD2, 2.f, false, 0.f, false);

	auto pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
	{
		auto pModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event1, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event2, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event3, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event4, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION5, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event5, this));
		pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION6, std::bind(&CPlayerBrainFieldCutsceneFinalState::Event6, this));
	}
	
	//auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	//auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	//pSky->Set_GradationBackGround({ 0.66f, 0.21f, 0.66f }, { 0.0f, 0.f, 0.f });

	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->Set_GradationBackGround({ 0.66f, 0.21f, 0.66f }, { 0.0f, 0.f, 0.f });

	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	RIMEFFECTDESC tRimDesc;
	ZeroMemory(&tRimDesc, sizeof(RIMEFFECTDESC));
	tRimDesc.bTransparency = false;
	tRimDesc.fDuration = 93.5f;
	tRimDesc.fRimPower = 15.f;
	tRimDesc.fRimStrength = 5.f;
	tRimDesc.vRimColor = { 1.0f, 0.f, 0.f };
	tRimDesc.vScale = { 1.f, 1.f, 1.f };
	pPlayer->Add_RimEffect(tRimDesc);

	_uint iTentacleCount = 0;
	auto pBrainFieldTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
	for (auto pObject = pBrainFieldTentacleLayer->Begin(); pObject != pBrainFieldTentacleLayer->End(); ++pObject)
	{
		auto pTentacle = dynamic_pointer_cast<CBrainFieldTentacle>(*pObject);

		_float4x4 matPivot = XMMatrixRotationX(XMConvertToRadians(90.f));
		pTentacle->Set_Pivot(matPivot);

		auto pTentacleModel = pTentacle->Get_Component<CModel>(COMPONENT::MODEL);
		pTentacleModel->Set_Animation(IDX(ANIMATION::BRAINFIELD_TENTACLE::BRAIN_FIELD_OPEN_C02_01) + iTentacleCount, 1.f, false, 0.2f, true);
		++iTentacleCount;
	}

	m_pRenderer = pGameInstance->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);

	dynamic_pointer_cast<CScene_MindRoom>(CGameInstance::Get_Instance()->Get_Current_Scene_Ptr())->Add_Dynamic();
	_player.Get_PlayerDesc().bBrainField = true;

}

void CPlayerBrainFieldCutsceneFinalState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	__super::Late_Update(_player, fTimeDelta);

#ifdef _DEBUG
	auto pGameInstance = CGameInstance::Get_Instance();

	auto pPlayerModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
	auto pAnimation = pPlayerModel->Get_Animation((_uint)ANIMATION::PLAYER::BRAINFIELD2);

	ImGui::Begin("BrainFieldAnimationSettings");

	static _float fAnimSpeed = 2.f;
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
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD2, fAnimSpeed, false, 0.f, false);
	}

	_float3 vPlayerPos = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
	ImGui::InputFloat3("Player Position", &vPlayerPos.x);

	ImGui::End();

#endif
}

void CPlayerBrainFieldCutsceneFinalState::Exit(CPlayer& _player)
{
	__super::Exit(_player);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayerEffectLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_RED_LASER_EYE);
	auto pEyeLaser = dynamic_pointer_cast<CRedLaserEye>(pPlayerEffectLayer->Get_GameObject(0));
	pEyeLaser->Set_Enable(false);

	shared_ptr<CObjectLayer> pMonsterLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
	pMonsterLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pMonster = dynamic_pointer_cast<CMonster>(pObject);
			if (pMonster->Get_Type() == CMonster::MONSTERTYPE::Karen)
				pObject->Set_Enable(true);

			return true;
		});

	auto pRoadLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ROAD);
	pRoadLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pRoad = dynamic_pointer_cast<CMap_MindRoom_Road>(pObject);
			//pRoad->Set_Enable(true);
			//pRoad->Set_Visible(false);
			//pRoad->Set_SingleColorMode(false);

			return true;
		});

	auto pDecalTarget = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_DECALTARGET);
	pDecalTarget->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			pObject->Set_Enable(true);

			return true;
		});

	auto pTeammateLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_TEAMMATE);
	pTeammateLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			pObject->Set_Enable(true);

			return true;
		});

	CScreenEffectManager::Get_Instance()->Stop_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);

	_player.Clear_RimEffect();
}

void CPlayerBrainFieldCutsceneFinalState::Event1()
{
	CMap_ToolManager::Get_Instance()->Set_Is_MindRoom(true);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pNeonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ETC);
	if (pNeonLayer)
	{
		pNeonLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
			{
				auto pNeonObject = dynamic_pointer_cast<CMap_MindRoom_Neon>(pObject);
				pNeonObject->Set_NeonColor({ 1.f, 0.f, 0.f, 1.f });

				return true;
			});
	}

	//auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	//auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	//pSky->Set_Enable(false);
	//pSky->GradientClear({ 0.87f, 0.16f, 0.5f });

	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->GradientClear({ 0.87f, 0.16f, 0.5f });

	for (shared_ptr<CPlayerTentacle> elem : static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Tentacles())
	{
		//elem->Set_Fly(true);
		elem->Set_Render(true);
		//CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_PARTICLE, make_pair(static_pointer_cast<CGameObject>(elem), "cable_02_a"));
	}
}

void CPlayerBrainFieldCutsceneFinalState::Event2()
{
	// Å« ¾È±¤
	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayerEffectLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_RED_LASER_EYE);
	auto pEyeLaser = dynamic_pointer_cast<CRedLaserEye>(pPlayerEffectLayer->Get_GameObject(0));
	pEyeLaser->Set_Enable(true);
	pEyeLaser->Set_Laser_Type(CRedLaserEye::LASER_TYPE::BIG);

	CRedLaserEye::LASERKEYFRAME tLaserKeyframe;
	tLaserKeyframe.fDuration = 0.1f;
	tLaserKeyframe.vLaserColor = _float4(0.755f, 0.475f, 0.475f, 1.f);
	tLaserKeyframe.vLaserScale = _float3(3.f, 1.f, 1.f);
	pEyeLaser->Push_Keyframe(tLaserKeyframe);

	tLaserKeyframe.fDuration = 0.05f;
	tLaserKeyframe.vLaserColor = _float4(1.5f, 1.f, 1.f, 1.f);
	tLaserKeyframe.vLaserScale = _float3(8.f, 5.f, 1.f);
	pEyeLaser->Push_Keyframe(tLaserKeyframe);

	tLaserKeyframe.fDuration = 0.05f;
	tLaserKeyframe.vLaserColor = _float4(1.5f, 0.f, 0.f, 1.f);
	tLaserKeyframe.vLaserScale = _float3(4.f, 0.05f, 1.f);
	pEyeLaser->Push_Keyframe(tLaserKeyframe);

	m_pRenderer->Enable_RadialBlur(true);
}

void CPlayerBrainFieldCutsceneFinalState::Event3()
{
	m_pRenderer->Enable_RadialBlur(false);
}

void CPlayerBrainFieldCutsceneFinalState::Event4()
{
}

void CPlayerBrainFieldCutsceneFinalState::Event5()
{
}

void CPlayerBrainFieldCutsceneFinalState::Event6()
{
	m_bCutsceneFinished = true;
	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().bInvincibility = false;
}
