#include "ClientPCH.h"
#include "PlayerBrainFieldExitState.h"
#include "Animator.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "Map_ToolManager.h"
#include "Scene_MindRoom.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Map_MindRoom_Road.h"
#include "Map_MindRoom_Static.h"
#include "PlayerTentacle.h"
#include "ScreenEffectManager.h"
#include "SkyCube.h"

shared_ptr<CPlayerState> CPlayerBrainFieldExitState::Transition(CPlayer& _player)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BRAINFIELD_CLOS_NF))
	{
		CMap_ToolManager::Get_Instance()->Set_Is_MindRoom(false);
		dynamic_pointer_cast<CScene_MindRoom>(CGameInstance::Get_Instance()->Get_Current_Scene_Ptr())->Clear_Dynamic();
		dynamic_pointer_cast<CScene_MindRoom>(CGameInstance::Get_Instance()->Get_Current_Scene_Ptr())->Add_Dynamic_Throw();
		_player.Get_PlayerDesc().bBrainField = false;
		_player.Get_PlayerDesc().bInvincibility = false;
		
		return make_shared<CPlayerIdleState>();
	}
}

void CPlayerBrainFieldExitState::Entry(CPlayer& _player)
{
	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_BrainField_Exit.wav"), g_fPlayerSound);

	auto pScreenEffectManager = CScreenEffectManager::Get_Instance();
	pScreenEffectManager->Stop_ScreenEffect(SCREEN_EFFECT::COLOR_REVERSE);
	pScreenEffectManager->Stop_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
	pScreenEffectManager->Stop_ScreenEffect(SCREEN_EFFECT::RIPPLE);
	pScreenEffectManager->Stop_ScreenEffect(SCREEN_EFFECT::RIPPLE_TINY);
	
	_player.Enable_BrainField(false);

	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD_CLOSE_BF, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);

	auto pPlayerPosiiton = _player.Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
	pPlayerPosiiton.y += 0.5f;
	_player.Get_Camera()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, pPlayerPosiiton);

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pRoadLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ROAD);
	pRoadLayer->Iterate_Objects([](shared_ptr<CGameObject> pRoad)
		{
			auto pRoadObject = dynamic_pointer_cast<CMap_MindRoom_Road>(pRoad);
			pRoadObject->Set_Enable(true);
			pRoadObject->Set_SingleColorMode(false);

			pRoadObject->Execute_RadialSperad();

			return true;
		});

	auto pStaticLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_STATIC);
	pStaticLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			auto pStaticObject = dynamic_pointer_cast<CMap_MindRoom_Static>(pObject);
			if (pStaticObject)
			{
				pStaticObject->Set_Enable(true);

				pStaticObject->Execute_RadialSperad();
			}

			return true;
		});

	auto pDecalTarget = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_DECALTARGET);
	pDecalTarget->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			pObject->Set_Enable(false);

			return true;
		});
	
	auto pNeonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_ETC);
	pNeonLayer->Iterate_Objects([](shared_ptr<CGameObject> pObject)
		{
			pObject->Set_Enable(false);

			return true;
		});

	for (shared_ptr<CPlayerTentacle> elem : _player.Get_Tentacles())
	{
		elem->Reset();
	}

	auto pBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND);
	auto pSky = dynamic_pointer_cast<CSkyCube>(pBackGroundLayer->Get_GameObject(0));
	pSky->Set_Enable(true);
	pSky->Reset();

	auto pBrainFieldBackGroundLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_BACKGROUND_BRAINFIELD);
	auto pBrainFieldSky = dynamic_pointer_cast<CSkyCube>(pBrainFieldBackGroundLayer->Get_GameObject(0));
	pBrainFieldSky->Set_Enable(false);


	_player.Get_PlayerDesc().bInvincibility = true;
}

void CPlayerBrainFieldExitState::Update(CPlayer& _player, _float fTimeDelta)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BRAINFIELD_CLOSE_BF))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BRAINFIELD_CLOSE_BF))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BRAINFIELD_CLOS_NF, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
}

void CPlayerBrainFieldExitState::Exit(CPlayer& _player)
{
	_player.Clear_RimEffect();
}
