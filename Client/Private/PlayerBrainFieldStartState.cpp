#include "ClientPCH.h"
#include "PlayerBrainFieldStartState.h"

#include "GameInstance.h"
#include "Player.h"
#include "CutsceneTentacle.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"

#include "PlayerIdleState.h"
#include "PlayerBrainFieldCutsceneState.h"

#include "Scene_MindRoom.h"
#include "PlayerTentacle.h"
#include "Bone.h"

shared_ptr<CPlayerState> CPlayerBrainFieldStartState::Transition(CPlayer& _player)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(VK_RETURN))
	{
		for (shared_ptr<CPlayerTentacle> elem : _player.Get_Tentacles())
		{
			elem->Set_Fly(true);
			elem->Set_Render(true);
			//CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_PARTICLE, make_pair(static_pointer_cast<CGameObject>(elem), "cable_02_a"));
		}
		return make_shared<CPlayerIdleState>();
	}

	if (pGameInstance->Key_Down('B'))
	{
		return make_shared<CPlayerBrainFieldStartState>();
	}

	auto pPlayerTransform = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM);
	_float3 vPlayerPos = pPlayerTransform->Get_State(TRANSFORM::POSITION);
	vPlayerPos.y = 0.f;

	auto pPlayerCamera = _player.Get_Camera();
	auto pPlayerCamTransform = pPlayerCamera->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	_float3 vCamPos = pPlayerCamTransform->Get_State(TRANSFORM::POSITION);
	vCamPos.y = 0.f;

	if (XMVectorGetX(XMVector3Length(vPlayerPos - vCamPos)) <= 0.3f)
	{
		return make_shared<CPlayerBrainFieldCutsceneState>();
	}

	return nullptr;
}

void CPlayerBrainFieldStartState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	//  <---> FINAL_STAT::EXIT()

	auto pModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
	pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CPlayerBrainFieldStartState::Execute_TentacleEffect, this));
	pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CPlayerBrainFieldStartState::Create_Tentacle, this));
	pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CPlayerBrainFieldStartState::Tentacle_Anim, this));
	pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION4, std::bind(&CPlayerBrainFieldStartState::ZoomIn, this));

	auto pPlayerTransform = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM);
	_float3 vPlayerPosition = pPlayerTransform->Get_State(TRANSFORM::POSITION);
	_float3 vLookDir = pPlayerTransform->Get_State(TRANSFORM::LOOK);

	m_vecCutsceneTentacle.clear();

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pTentacleLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_CUTSCENE_TENTACLE);
	pTentacleLayer->Iterate_Objects([&](shared_ptr<CGameObject> pObject)
	{
		auto pCutsceneTentacle = dynamic_pointer_cast<CCutsceneTentacle>(pObject);
		pCutsceneTentacle->Set_Enable(false);

		auto pTransform = pCutsceneTentacle->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		pTransform->Set_State(TRANSFORM::POSITION, vPlayerPosition);
		pTransform->LookTo(vLookDir);

		m_vecCutsceneTentacle.push_back(pCutsceneTentacle);

		return true;
	});

	m_eCurrentState = PLAYER_STATE::BRAIN_FIELD;
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD_START, 2.f, false, 0.f, false);

	m_bCamZoomIn = false;

	auto pPlayerCamera = _player.Get_Camera();
	pPlayerCamera->Lock(true);

	_player.Get_PlayerDesc().bInvincibility = true;

	//CGameInstance::Get_Instance()->SetChannelVolume(CSound_Manager::SOUND_BGM, 0.f);
	CGameInstance::Get_Instance()->StopAll();
}

void CPlayerBrainFieldStartState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);
	__super::Late_Update(_player, fTimeDelta);

	auto pPlayerCamera = _player.Get_Camera()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	auto pPlayerModel = _player.Get_Component<CModel>(COMPONENT::MODEL);
	auto matPlayerHead = pPlayerModel->Get_Bone("Head")->Get_CombinedTransformation()
		* pPlayerModel->Get_Pivot() * m_pTransform->Get_Matrix();

	_vector vHeadPosition = XMLoadFloat4((_float4*)matPlayerHead.m[3]);
	pPlayerCamera->LookAt_Interpolation(vHeadPosition, false, true);

	_float3 vCamLook = pPlayerCamera->Get_State(TRANSFORM::LOOK);
	if (m_bCamZoomIn)
	{
		pPlayerCamera->Translate(vCamLook * fTimeDelta * 4.f);
	}
	else
	{
		pPlayerCamera->Translate(vCamLook * fTimeDelta * 2.f);
	}

	for (shared_ptr<CPlayerTentacle> elem : _player.Get_Tentacles())
	{
		elem->Set_Fly(true);
		//elem->Set_Render(true);
		//CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_PARTICLE, make_pair(static_pointer_cast<CGameObject>(elem), "cable_02_a"));
	}
	_player.Get_PlayerDesc().bInvincibility = true;
}

void CPlayerBrainFieldStartState::Exit(CPlayer& _player)
{
	__super::Exit(_player);

	for (auto pCutsceneTentacle : m_vecCutsceneTentacle)
	{
		pCutsceneTentacle->Set_Enable(false);
	}
	m_vecCutsceneTentacle.clear();

	auto pPlayerCamera = _player.Get_Camera();
	pPlayerCamera->Lock(false);
}

#pragma region Event Callback
void CPlayerBrainFieldStartState::Execute_TentacleEffect()
{
	auto pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	pPlayer->SAS_Overdrive();

	
	//CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM);
	//CGameInstance::Get_Instance()->StopSound(CSound_Manager::SOUND_BGM2);
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Enter0.wav"), 3.f);
}

void CPlayerBrainFieldStartState::Create_Tentacle()
{
	for (auto pCutsceneTentacle : m_vecCutsceneTentacle)
	{
		pCutsceneTentacle->Set_Enable(true);
	}

	auto pTentableA = m_vecCutsceneTentacle[0]->Get_Component<CModel>(COMPONENT::MODEL);
	pTentableA->Reset_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START01));
	pTentableA->Set_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START01), 2.f, false, 0.2f, false);

	auto pTentableB = m_vecCutsceneTentacle[1]->Get_Component<CModel>(COMPONENT::MODEL);
	pTentableB->Reset_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START02));
	pTentableB->Set_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START02), 2.f, false, 0.2f, false);

	auto pTentacleTransform = m_vecCutsceneTentacle[1]->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	pTentacleTransform->Translate(XMVectorSet(0.f, 1.5f, 0.f, 0.f));

	auto pTentableC = m_vecCutsceneTentacle[2]->Get_Component<CModel>(COMPONENT::MODEL);
	pTentableC->Reset_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START03));
	pTentableC->Set_Animation(_uint(CCutsceneTentacle::ANIMATION::BRAIN_START03), 2.f, false, 0.2f, false);
}

void CPlayerBrainFieldStartState::Tentacle_Anim()
{
}

void CPlayerBrainFieldStartState::ZoomIn()
{
	m_bCamZoomIn = true;
}

#pragma endregion