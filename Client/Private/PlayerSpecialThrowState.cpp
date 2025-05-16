
#include "ClientPCH.h"
#include "PlayerSpecialThrowState.h"
#include "PlayerThrowState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerAttackState.h"
#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Model.h"
#include "Animator.h"
#include "Bone.h"
#include "Psychokinesis.h"
#include "InteractiveObject.h"
#include "Map_Special_Dynamic.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Map_Suoh_Bus.h"
#include "Map_MindRoom_Dynamic.h"
#include "Monster.h"
#include "Monster_Karen.h"
#include "Effect_Manager.h"
#include "PhysXCollider.h"
#include "Scene_MindRoom.h"
#include "Map_MindRoom_Dynamic.h"

CPlayerSpecialThrowState::CPlayerSpecialThrowState()
{
}

CPlayerSpecialThrowState::CPlayerSpecialThrowState(SPECIAL_THROW _type)
{
	m_eThrowType = _type;
}

shared_ptr<CPlayerState> CPlayerSpecialThrowState::Transition(CPlayer& _player)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(m_eEndAnim))
	{
		return make_shared<CPlayerIdleState>();
	}
    return nullptr;
}

void CPlayerSpecialThrowState::Entry(CPlayer& _player)
{
	__super::Entry(_player);

	m_eCurrentState = PLAYER_STATE::SPECIALTHROW;
	_player.Get_Camera()->Set_Fixed(true);
	_float fAnimSpeed = g_fPlayerAnimationSpeed;
	switch (m_eThrowType)
	{
	case SPECIAL_THROW::HEAVY_EQUIPMENT:
		m_eAnim = ANIMATION::PLAYER::THROW_BIG_START;
		m_eEndAnim = ANIMATION::PLAYER::THROW_BIG_END;
		break;
	case SPECIAL_THROW::BEAM:
		m_eAnim = ANIMATION::PLAYER::CAPTURE_BACKSTEP_START;
		m_eEndAnim = ANIMATION::PLAYER::CAPTURE_BACKSTEP_END;
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel
			->Get_BoneIndex("Spine"));
		m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 4.f + m_pTransform->Get_State(TRANSFORM::RIGHT)*1.5f + m_pTransform->Get_State(TRANSFORM::UP) * 1.5f);
		_player.Get_Camera()->Set_Fixed(false);
		break;
	case SPECIAL_THROW::ROCK:
		m_eAnim = ANIMATION::PLAYER::FALL_FIN_START;
		m_eEndAnim = ANIMATION::PLAYER::FALL_FIN_END;
		break;
	case SPECIAL_THROW::BULLDOZER:
		m_eAnim = ANIMATION::PLAYER::CAPTURE_BACKSTEP_START;
		m_eEndAnim = ANIMATION::PLAYER::CAPTURE_BACKSTEP_END;
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel
			->Get_BoneIndex("Spine"));
		break;
	case SPECIAL_THROW::BUS:
		m_eAnim = ANIMATION::PLAYER::JUMP_START;
		m_eEndAnim = ANIMATION::PLAYER::JUMP_LANDING;
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel
			 ->Get_BoneIndex("Spine"));
		_player.Get_Camera()->Set_Fixed(false);
		fAnimSpeed = 1.f;
		m_bGravity = false;
		static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Set_Aura(true);
		break;
	case SPECIAL_THROW::FINAL_ROCK:
		m_eAnim = ANIMATION::PLAYER::FALL_FIN_START;
		m_eEndAnim = ANIMATION::PLAYER::FALL_FIN_END;
		static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Set_Aura(true);
		break;
	case SPECIAL_THROW::LONG:
		m_eAnim = ANIMATION::PLAYER::CAPTURE_BACKSTEP_START;
		m_eEndAnim = ANIMATION::PLAYER::THROW_FORWARD;
		static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Set_Aura(true);
		break;
	case SPECIAL_THROW::MINDROOM:
		m_eAnim = ANIMATION::PLAYER::SWING_FALL_START;
		m_eEndAnim = ANIMATION::PLAYER::SWING_FALL_END;
		m_bGravity = false;
		break;
	}
	_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(m_eAnim, fAnimSpeed, false, g_fDefaultInterpolationDuration, false);
	if(m_eThrowType !=SPECIAL_THROW::MINDROOM)
	{
		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
		m_pTransform->LookAt(pTransform);
		m_pThrowObject = _player.Get_Kinesis()->Get_CapturedObject();
	}
	_player.Get_PlayerDesc().bInvincibility = true;

	if(m_eThrowType == SPECIAL_THROW::HEAVY_EQUIPMENT)
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kasane_UseGimmick00.wav"),TEXT("Voice_Kasane_UseGimmick01.wav") }), g_fPlayerSound);
	else
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Kasane_Use_SpecialAttack.wav"), g_fPlayerSound);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Special_PsycoKenisis.wav"), g_fPlayerSound);
}

void CPlayerSpecialThrowState::Update(CPlayer& _player, _float fTimeDelta)
{
	__super::Update(_player, fTimeDelta);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	switch (m_eThrowType)
	{
	case SPECIAL_THROW::HEAVY_EQUIPMENT:
		HeavyEquipment(_player,fTimeDelta);
		break;
	case SPECIAL_THROW::BEAM:
		Beam(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::ROCK:
		Rock(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::BULLDOZER:
		Bulldozer(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::BUS:
		Bus(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::FINAL_ROCK:
		Final_Rock(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::LONG:
		Long(_player, fTimeDelta);
		break;
	case SPECIAL_THROW::MINDROOM:
		Mindroom(_player, fTimeDelta);
		break;
	}

}

void CPlayerSpecialThrowState::Exit(CPlayer& _player)
{
	__super::Exit(_player);
	_player.Get_Camera()->Set_Fixed(false);
	_player.Get_Camera()->Set_Handle(true);
	_player.Get_PlayerDesc().bInvincibility = false;
	_player.Get_Kinesis()->Reset();
	//m_bGravity = false;
	m_bGravity = true;
	m_fGravityTime = 0.f;

	if(m_eThrowType == SPECIAL_THROW::MINDROOM)
		dynamic_pointer_cast<CScene_MindRoom>(CGameInstance::Get_Instance()->Get_Current_Scene_Ptr())->Clear_Dynamic();

	_player.Get_Kinesis()->Set_SpecialThrow(false);
}

void CPlayerSpecialThrowState::HeavyEquipment(CPlayer& _player, _float fTimeDelta)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::THROW_BIG_START) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_BIG_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_BIG_LOOP, g_fPlayerAnimationSpeed, false, 0.f, true);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_BIG_LOOP))
	{
		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
		/*m_pTransform->LookAt(pTransform);*/
	}
	if (m_bThrow && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_BIG_END))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_BIG_END, g_fPlayerAnimationSpeed, false, 0.f, false);
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_BIG_END))
	{
		if (m_pModel->Get_Proportion() > 0.4f && !m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			shared_ptr<CMap_Special_Dynamic> pThrowObject = _player.Get_Kinesis()->Get_SpecialThrowObject();
			if (nullptr != pThrowObject)
			{
				if (false == _player.Get_Kinesis()->Get_SpecialThrowObject()->Get_Throw())
					pThrowObject->Throw();
			}
		}
	}
	__super::Late_Update(_player, fTimeDelta);

	_matrix BoneMatrix = m_pModel->Get_Bone("CameraPos")->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	m_pCameraTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) *
		BoneMatrix
		* m_pModel->Get_Pivot()
		* m_pTransform->Get_Matrix()
	);

	//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
	if (nullptr != m_pThrowObject)
	{
		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
		m_pTransform->LookAt(pTransform);
		m_pCameraTransform->LookAtDir(XMVector3Normalize(pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)),true);
		m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
		m_pCameraTransform->Translate(-m_pCameraTransform->Get_State(TRANSFORM::LOOK)*1.f);
	}
}

void CPlayerSpecialThrowState::Beam(CPlayer& _player, _float fTimeDelta)
{

	_float3 vCamPrePos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP, 1.f, false, 0.f, true);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START))
	{
		m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 3.f);
		shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != pObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				m_pTransform->LookAt_Interpolation(pTransform);
			/*	m_pCameraTransform->LookAt(pTransform);
				_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.f + m_pTransform->Get_State(TRANSFORM::UP) * 1.5f;
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f,m_fTime*2.f)));*/
			}
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				_player.Get_Camera()->Set_Fixed(true);
				m_pTransform->LookAt_Interpolation(pTransform);
				_float3 vDir = pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
				vDir.y = 0.f;
				m_pCameraTransform->LookAtDir(XMVector3Normalize(vDir));
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pCameraTransform->Get_State(TRANSFORM::RIGHT) * 1.f + _float3(0.f, 1.5f, 0.f) );
				_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			}
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_END))
	{
		_player.Get_Camera()->Set_Fixed(false);
	}

	if (m_bThrow && _player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP_END, 1.5f, false, 0.f, false);
		m_bThrow = false;
	}

	m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vCamPrePos, m_pCameraTransform->Get_State(TRANSFORM::POSITION), 0.1f));
	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Rock(CPlayer& _player, _float fTimeDelta)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_START))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::FALL_FIN_START))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::FALL_FIN_LOOP,g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_LOOP))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::FALL_FIN_LOOP))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::FALL_FIN_END, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_END) &&
		m_pModel->Get_Proportion() > 0.5f)
	{
		m_fThrowTime += fTimeDelta;
		//m_pCameraTransform->Translate(XMVectorLerp({ fTimeDelta * 2.f * m_pCameraTransform->Get_State(TRANSFORM::LOOK) + fTimeDelta * 2.f * m_pCameraTransform->Get_State(TRANSFORM::RIGHT)},
		//	_float3(0.f,0.f,0.f),min(1.f,m_fThrowTime)));
	}
	else
	{
		_matrix BoneMatrix = m_pModel->Get_Bone("CameraPos")->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		m_pCameraTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) *
			BoneMatrix
			* m_pModel->Get_Pivot()
			* m_pTransform->Get_Matrix());

		//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				//m_pCameraTransform->Set_State(TRANSFORM::POSITION, pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK));
				m_pTransform->LookAt(pTransform);
				_float3 vDir = pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
				vDir.y = 0.f;
				m_pCameraTransform->LookAtDir(XMVector3Normalize(vDir));
			
				//m_pCameraTransform->Translate(m_pCamera3Transform->Get_State(TRANSFORM::RIGHT) * 0.2f);
				//m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(30.f));
			}
		}
	}

	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Bulldozer(CPlayer& _player, _float fTimeDelta)
{
	_float3 vCamPrePos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP, 1.5f, false, 0.f, true);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START))
	{
		m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 3.f);
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				m_pTransform->LookAt_Interpolation(pTransform);
					m_pCameraTransform->LookAt(pTransform);
					_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 3.f + m_pTransform->Get_State(TRANSFORM::RIGHT) * 1.f + m_pTransform->Get_State(TRANSFORM::UP) * 1.5f;
					m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f,m_fTime*2.f)));
			}
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				_player.Get_Camera()->Set_Fixed(true);
				m_pTransform->LookAt(pTransform);
				_float3 vDir = pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
				vDir.y = 0.f;
				m_pCameraTransform->LookAtDir(XMVector3Normalize(vDir));
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pCameraTransform->Get_State(TRANSFORM::LOOK) * 3.f + m_pCameraTransform->Get_State(TRANSFORM::RIGHT) * 1.f + _float3(0.f, 1.5f, 0.f));
			}
		}
	}

	if (m_bThrow && _player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP_END, 1.5f, false, 0.f, false);
		m_bThrow = false;
	}

	m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vCamPrePos, m_pCameraTransform->Get_State(TRANSFORM::POSITION), 0.1f));
	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Bus(CPlayer& _player, _float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	
	shared_ptr<CMap_Suoh_Bus> pBus = static_pointer_cast<CMap_Suoh_Bus>(m_pThrowObject);
	shared_ptr<CTransform> pTransform = nullptr;
	if (nullptr != pBus)
		pTransform = static_pointer_cast<CTransform>(pBus->Get_Component(COMPONENT::TRANSFORM));

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_START))
	{
		m_fRideTime += fTimeDelta;
		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
		_float3 vPos = pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::LOOK) * 3.f + _float3(0, 5, 0);
		m_pTransform->Set_State(TRANSFORM::POSITION, { XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION),_float3(vPos.x,m_pTransform->Get_State(TRANSFORM::POSITION).y,vPos.z),m_pModel->Get_Proportion() * 0.5f) });
		if (m_pModel->Get_Proportion() < 0.5f)
			m_pTransform->Translate({ 0.f,fTimeDelta * 40.f,0.f,0.f });
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::JUMP_START) /*&& m_pTransform->Get_State(TRANSFORM::POSITION).y > 8.f*/)
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BUS_RIDE_JUMP, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
			m_vStartPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			m_fRideTime = 0.f;
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BUS_RIDE_JUMP))
	{
		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
		_float3 vUpPos = pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::LOOK) * 3.f + _float3(0, 8, 0);
		_float3 vPos = pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::LOOK) * 3.f + _float3(0, 5, 0);
		m_fRideTime += fTimeDelta*1.2f;

			m_pTransform->Set_State(TRANSFORM::POSITION, { XMVectorLerp(m_vStartPos,vPos,min(1.f,m_fRideTime)) });
		
	
	m_pTransform->LookTo(pTransform->Get_State(TRANSFORM::LOOK));
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BUS_RIDE_JUMP))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BUS_RIDE_LOOP, g_fPlayerAnimationSpeed, false, 0.f, true);
		}
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BUS_RIDE_LOOP))
	{
		Drive_Bus(_player, fTimeDelta);

		_player.Get_Camera()->Set_LerpTime(0.2f);
		//_player.Get_Camera()->Set_Fixed(true);
		_float3 vCamPos = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 5.f + _float4(0, 4, 0, 1);
		//m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pCameraTransform->Get_State(TRANSFORM::POSITION),vCamPos,0.2f));
		//m_pCameraTransform->LookTo(m_pTransform->Get_State(TRANSFORM::LOOK));
		//m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(30.f));
		if (pGameInstance->Key_Down('G'))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::BUS_RIDE_END, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::BUS_RIDE_END))
	{
		_player.Get_Camera()->Set_LerpTime(0.075f);
		_player.Get_Camera()->Set_Fixed(true);
		_matrix BoneMatrix = m_pModel->Get_Bone("CameraPos")->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		m_pCameraTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) *
			BoneMatrix
			* m_pModel->Get_Pivot()
			* m_pTransform->Get_Matrix()
		);

		if (m_pModel->Get_Proportion() > 0.2f && m_pModel->Get_Proportion() < 0.5f)
		{
			m_bGravity = false;
			m_pTransform->Translate({ 0,fTimeDelta * 20.f,0 });
			pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta * 3.f);
		}

		if (m_pModel->Get_Proportion() > 0.2f &&  m_pModel->Get_Proportion() < 0.5f)
		{
			if (!m_bExecuted[0])
			{
				m_bExecuted[0] = true;
				m_vThrowDir = pTransform->Get_State(TRANSFORM::LOOK);
			}

		}
		if (m_pModel->Get_Proportion() > 0.3f && m_pModel->Get_Proportion() < 0.5f)
		{
			pTransform->Translate(fTimeDelta * 25.f * (m_vThrowDir + _float3(0, 3, 0)));

		}
		else if(m_pModel->Get_Proportion() >= 0.6f && m_pModel->Get_Proportion() < 0.78f)
		{
			pTransform->Translate((m_vThrowDir + _float3(0, -3, 0)) * fTimeDelta * 20.f);
		}
		else if (m_pModel->Get_Proportion() >= 0.85f && !m_bExecuted[10])
		{
			m_bExecuted[10] = true;
			pBus->Get_KinematicActor()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
			pBus->Get_ActorDesc().bThrow = true;

			_float3 vPos = pBus->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::BUS, initializer_list<any>{vPos,vPos,vPos});
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("bus_Explosion.mp3"), g_fPlayerSound);
		}

		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::BUS_RIDE_END))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_FALL, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
			_player.Get_Camera()->Set_Fixed(false);
			pBus->Set_Dissolve(true);
			m_bGravity = true;
			m_fGravityTime = 0.f;
			shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
			//shared_ptr<CObjectPool> pMonLayer = pGameInstance->Find_Pool(pGameInstance->Current_Scene(), LAYER_MONSTER);
			_float fMinDist = 100.f;
			pMonLayer->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->bool
				{
					shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

					if (nullptr == pMonster)
						return true;

					_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
					if (fCurDist < fMinDist)
					{
						pMonster->Get_NormalActorDesc()->bHit = true;
						pMonster->Get_NormalActorDesc()->stHitDesc.iDamage = 100.f;
					}

					return true;
				});
		}

	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::JUMP_FALL) && !m_bInAir)
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::JUMP_LANDING, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
	}
	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Final_Rock(CPlayer& _player, _float fTimeDelta)
{
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_START))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::FALL_FIN_START))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::FALL_FIN_LOOP, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_LOOP))
	{
		if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::FALL_FIN_LOOP))
		{
			_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::FALL_FIN_END, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::FALL_FIN_END) &&
		m_pModel->Get_Proportion() > 0.5f)
	{
		m_fThrowTime += fTimeDelta;
		//m_pCameraTransform->Translate(XMVectorLerp({ fTimeDelta * 2.f * m_pCameraTransform->Get_State(TRANSFORM::LOOK) + fTimeDelta * 2.f * m_pCameraTransform->Get_State(TRANSFORM::RIGHT)},
		//	_float3(0.f,0.f,0.f),min(1.f,m_fThrowTime)));
	}
	else
	{
		_matrix BoneMatrix = m_pModel->Get_Bone("CameraPos")->Get_CombinedTransformation();
		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		m_pCameraTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) *
			BoneMatrix
			* m_pModel->Get_Pivot()
			* m_pTransform->Get_Matrix());

		//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				//m_pCameraTransform->Set_State(TRANSFORM::POSITION, pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK)*7.f);
				m_pTransform->LookAt(pTransform);
				_float3 vDir = pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
				vDir.y = 0.f;
				m_pCameraTransform->LookAtDir(XMVector3Normalize(vDir));
				m_pCameraTransform->Translate(m_pCameraTransform->Get_State(TRANSFORM::LOOK) * -7.f);
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
				//m_pCameraTransform->Rotate({ 0,1,0,0 }, XMConvertToRadians(30.f));
			}
		}
	}

	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Long(CPlayer& _player, _float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	_float3 vCamPrePos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_FORWARD))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::THROW_FORWARD, 1.2f, false, 0.f, false);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_START))
	{
		m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 3.f);
		shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != pObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_TargetMonsterTransform();
			if (nullptr != pTransform)
			{
				m_pTransform->LookAt_Interpolation(pTransform);
				m_pCameraTransform->LookAt_Interpolation(pTransform);

					//m_pCameraTransform->LookAt(pTransform);
				m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pCameraTransform->Get_State(TRANSFORM::RIGHT) * 0.8f + _float3(0.f, 1.5f, 0.f));
				m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
			}
		}
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::THROW_FORWARD))
	{
		//m_pCameraTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pCameraTransform->Get_State(TRANSFORM::RIGHT) * 0.5f + _float3(0.f, 1.6f, 0.f));
		//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
		if (nullptr != m_pThrowObject)
		{
			shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
			if (nullptr != pTransform)
			{
				_player.Get_Camera()->Set_Fixed(true);
				//m_pTransform->LookAt_Interpolation(pTransform);
				_float3 vDir = pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);
				vDir.y = 0.f;

				shared_ptr<CTransform> pTargetTransform = static_pointer_cast<CMap_Dynamic>(_player.Get_Kinesis()->Get_CapturedObject())->Get_TargetMonsterTransform();
				if (nullptr != pTargetTransform)
				{
					//m_pTransform->LookAt_Interpolation(pTargetTransform);
				}
				//m_pCameraTransform->LookAtDir(XMVector3Normalize(vDir));
				if(!_player.Get_Camera()->Get_Shake())
				{
					_float3 vCurPos = m_pCameraTransform->Get_State(TRANSFORM::POSITION);
					_float3 vDestPos = m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::LOOK) * 1.f + m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.8f + _float3(0.f, 1.5f, 0.f);
					m_pCameraTransform->Set_State(TRANSFORM::POSITION, Function::Lerp(vCurPos, vDestPos, 0.05f));
				}

				/*m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
				if (m_pModel->Get_Proportion() > 0.5f)
					m_pCameraTransform->Translate(m_pCameraTransform->Get_State(TRANSFORM::UP) * fTimeDelta * 5.f);*/
			}
		}
	}
	//if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_END))
	//{
	//	_player.Get_Camera()->Set_Fixed(false);
	//}

	if (m_bThrow && _player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::CAPTURE_BACKSTEP_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::CAPTURE_BACKSTEP_END, 1.5f, false, 0.f, false);
		m_bThrow = false;
	}

	//m_pCameraTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(vCamPrePos, m_pCameraTransform->Get_State(TRANSFORM::POSITION), 0.1f));
	__super::Late_Update(_player, fTimeDelta);
}

void CPlayerSpecialThrowState::Mindroom(CPlayer& _player, _float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	_float3 vBossPos;

	if (!m_bExecuted[10])
	{
		m_bExecuted[10] = true;	
		m_pKaren = nullptr;
		shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
		_float fMinDist = 300.f;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				if (nullptr == dynamic_pointer_cast<CMonster_Karen>(_pObject))
					return true;

				m_pKaren = dynamic_pointer_cast<CMonster_Karen>(_pObject);

				vBossPos = static_pointer_cast<CTransform>(m_pKaren->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

				return true;
			});
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::SWING_FALL_START) &&
		!_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_FALL_LOOP))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::SWING_FALL_LOOP, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
	}

	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_FALL_LOOP))
	{
		//shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(_player.Get_Kinesis()->Get_CapturedObject()->Get_Component(COMPONENT::TRANSFORM));
		/*m_pTransform->LookAt(pTransform);*/

		if (!m_bExecuted[55])
		{
			m_bExecuted[55] = true;
			for(int i=0;i<30;i++)
			m_vRanMindPos[i] = Function::RandomVector(_float3(-1.f, -1.f, -1.f), _float3(1.f, 1.f, 1.f));
		}
		shared_ptr<CObjectLayer> pObjLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
		//shared_ptr<CObjectPool> pMonLayer = pGameInstance->Find_Pool(pGameInstance->Current_Scene(), LAYER_MONSTER);
		_float fMinDist = 30.f;
		_int idx = 0;
		pObjLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMap_MindRoom_Dynamic> pObj = dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_pObject);

				if (nullptr == pObj)
					return true;

				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float3 vObjPos = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vObjPos));
				if (fCurDist < fMinDist)
				{
					shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM));
					_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION) + m_pTransform->Get_State(TRANSFORM::LOOK) * 5.f
						+ m_pTransform->Get_State(TRANSFORM::UP) * 7.f;
					pObj->Set_Rot(false);

					vPos += m_vRanMindPos[idx];

					//pObj->SetPhysX(true);
					_float3 vDir = XMVector3Normalize(vPos - pTransform->Get_State(TRANSFORM::POSITION));
					pTransform->Set_State(TRANSFORM::POSITION,
						Function::Lerp(pTransform->Get_State(TRANSFORM::POSITION), vPos, 0.1f));
				}
				idx++;
				return true;
			});
		m_pTransform->LookAt_Interpolation(static_pointer_cast<CTransform>(m_pKaren->Get_Component(COMPONENT::TRANSFORM)), true, false, g_fDefaultInterpolationDuration * 20.f, 1.f);
	}
	if (/*m_bThrow && */_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Finished(ANIMATION::PLAYER::SWING_FALL_LOOP)
		 && !_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_FALL_END))
	{
		_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::PLAYER::SWING_FALL_END, g_fPlayerAnimationSpeed, false, g_fDefaultInterpolationDuration, false);
	}
	if (_player.Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Is_Playing(ANIMATION::PLAYER::SWING_FALL_END))
	{
		vBossPos = static_pointer_cast<CTransform>(m_pKaren->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
		if (m_pModel->Get_Proportion() > 0.4f && !m_bExecuted[0])
		{

			shared_ptr<CObjectLayer> pObjLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
			pObjLayer->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->bool
				{
					shared_ptr<CMap_MindRoom_Dynamic> pObj = dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_pObject);
					shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM));
					if (nullptr == pObj)
						return true;
					
					if(!m_bPileHit)
					{
						_float3 vDir = XMVector3Normalize(vBossPos - pTransform->Get_State(TRANSFORM::POSITION));
						pTransform->Translate(vDir * 1.f);
						if (XMVectorGetX(XMVector3Length(pTransform->Get_State(TRANSFORM::POSITION) - vBossPos)) < 1.5f)
						{
							m_bPileHit = true;
							m_pKaren->GetBodyCollider()->GetActorDesc().bThrowHit = true;
							_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
							pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
							pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
							pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
							pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
							pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
							pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
							m_bSlowStart = true;
							CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_DumpTruck_Smash.wav"), g_fPlayerSound);
							m_pKaren->Get_EntityDesc_Ref().fHP -= 100.f;
						}
					}

					if (m_bSlowStart)
					{
						_float3 vDir = XMVector3Normalize(vBossPos - pTransform->Get_State(TRANSFORM::POSITION));
						pTransform->Translate(vDir * -0.02f);
					}
					//else
					//{
					//	pObj->Get_Actor()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
					//	pObj->Set_Gravity(true);
					//}
				});
		}
	}
	__super::Late_Update_Root(_player, fTimeDelta,1.f);

	_matrix BoneMatrix = m_pModel->Get_Bone("CameraPos")->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	m_pCameraTransform->Set_Matrix(XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) *
		BoneMatrix
		* m_pModel->Get_Pivot()
		* m_pTransform->Get_Matrix()
	);

	//shared_ptr<CInteractiveObject> pObject = _player.Get_Kinesis()->Get_CapturedObject();
	//if (nullptr != m_pThrowObject)
	//{
	//	shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pThrowObject->Get_Component(COMPONENT::TRANSFORM));
	//	m_pTransform->LookAt(pTransform);
	//	m_pCameraTransform->LookAtDir(XMVector3Normalize(pTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)), true);
	//	m_pCameraTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(-15.f));
	//	m_pCameraTransform->Translate(-m_pCameraTransform->Get_State(TRANSFORM::LOOK) * 1.f);
	//}

	if (m_bSlowStart)
	{
		_player.Get_Camera()->Set_Shake(0.5f, 10.f);
		m_fSlowTime -= fTimeDelta;
		if (m_fSlowTime < 0.f)
		{
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);
			m_bSlowStart = false;

			shared_ptr<CObjectLayer> pObjLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
			pObjLayer->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->bool
				{
					shared_ptr<CMap_MindRoom_Dynamic> pObj = dynamic_pointer_cast<CMap_MindRoom_Dynamic>(_pObject);
					shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pObj->Get_Component(COMPONENT::TRANSFORM));
					if (nullptr == pObj)
						return true;
					pObj->Get_Actor()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
					pObj->Set_Gravity(true);
					pObj->Set_Dissolve(true);
				});
		}
	}
}

void CPlayerSpecialThrowState::Drive_Bus(CPlayer& _player, _float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CMap_Suoh_Bus> pBus = static_pointer_cast<CMap_Suoh_Bus>(_player.Get_Kinesis()->Get_RecentThrowdObject());
	shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pBus->Get_Component(COMPONENT::TRANSFORM));
	if (pGameInstance->Key_Hold('W'))
	{
		pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * m_fBusSpeed);
	}
	if (pGameInstance->Key_Hold('S'))
	{
		pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * -m_fBusSpeed);
	}
	if (pGameInstance->Key_Hold('A'))
	{
		pTransform->Rotate_Lerp({ 0,1,0,0 }, fTimeDelta * -m_fBusRotateSpeed);
	}
	if (pGameInstance->Key_Hold('D'))
	{
		pTransform->Rotate_Lerp({ 0,1,0,0 }, fTimeDelta * m_fBusRotateSpeed);
	}
	_float3 vPos = pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::LOOK) * 3.f + _float3(0, 5, 0);
	m_pTransform->Set_State(TRANSFORM::POSITION, vPos);
	m_pTransform->LookTo(pTransform->Get_State(TRANSFORM::LOOK));
}
