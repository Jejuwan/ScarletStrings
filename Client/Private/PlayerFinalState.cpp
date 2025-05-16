#include "ClientPCH.h"
#include "PlayerFinalState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerThrowState.h"
#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"
#include "Model.h"
#include "Animator.h"
#include "Bone.h"
#include "Psychokinesis.h"
#include "Camera_Player.h"
#include "Monster_Karen.h"

CPlayerFinalState::CPlayerFinalState()
{
}

shared_ptr<CPlayerState> CPlayerFinalState::Transition(CPlayer& _player)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();


	if (m_fEndTime > 2.f)
	{
		return make_shared<CPlayerIdleState>();
	}

	return nullptr;
}

void CPlayerFinalState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	auto pGameInstance = CGameInstance::Get_Instance();

	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_LOOP, 1.5f, false, 0.2f, false);

	shared_ptr<CMonster_Karen> pKaren = nullptr;
	shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
	pMonLayer->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pObject)->bool
		{
			if (nullptr == dynamic_pointer_cast<CMonster_Karen>(_pObject))
				return true;

			m_pKaren = dynamic_pointer_cast<CMonster_Karen>(_pObject);


			return true;
		});

	shared_ptr<CTransform> pKarenTransform = static_pointer_cast<CTransform>(m_pKaren.lock()->Get_Component(COMPONENT::TRANSFORM));
	m_pTransform->LookAt(static_pointer_cast<CTransform>(m_pKaren.lock()->Get_Component(
		COMPONENT::TRANSFORM)));
	pKarenTransform->LookAt(m_pTransform);
	m_pCameraTransform->LookAt(pKarenTransform);
	m_pCameraTransform->Rotate({0,1,0,0}, XMConvertToRadians(30.f));

	_player.Get_Camera()->Set_Fixed(true);
}

void CPlayerFinalState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	
	shared_ptr<CTransform> pKarenTransform = static_pointer_cast<CTransform>(m_pKaren.lock()->Get_Component(COMPONENT::TRANSFORM));
	if (!m_bExecuted[0])
	{
		m_bExecuted[0] = true;
		shared_ptr<CTransform> pKarenTransform = static_pointer_cast<CTransform>(m_pKaren.lock()->Get_Component(COMPONENT::TRANSFORM));
		m_pTransform->Set_State(TRANSFORM::POSITION, pKarenTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 6.f);
		m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pTransform->Get_State(TRANSFORM::UP) * 1.5f + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.f);
		m_pCameraTransform->LookAt(pKarenTransform);
		m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(-25.f));
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_LOOP))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_LOOP))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP,2.5f, false, 0.2f, false);
			m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::LOOK) * 4.5f + m_pTransform->Get_State(TRANSFORM::UP) + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.f);
			m_pCameraTransform->LookAt((m_pTransform->Get_State(TRANSFORM::POSITION) + pKarenTransform->Get_State(TRANSFORM::POSITION)) / 2.f);
		}
		m_pCameraTransform->Translate(m_pCameraTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 0.2f);

		if (m_pModel->Get_Proportion() > 0.5f && !m_bExecuted[32])
		{
			m_bExecuted[32] = true;
			m_pKaren.lock()->Tentacle_Fly();
		}

	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP))
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		m_fLerpTime += fTimeDelta;
		m_pCameraTransform->Translate(m_pCameraTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * Function::Lerp(-8.f, 0.f,min(1.f, m_fLerpTime)));
		if (m_fLerpTime > 1.f)
		{
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale*0.1f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.1f);
		}
		if (m_fLerpTime > 1.2f)
		{
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);
		}
	}
	
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP))
	{
		m_fEndTime += fTimeDelta;
	}
	
	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerFinalState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_Camera()->Set_Fixed(false);
}
