#include "ClientPCH.h"
#include "TeammateAI.h"

#include "GameInstance.h"

#include "TeammateAI.h"
#include "Teammate.h"
#include "Player.h"

HRESULT CTeammateAI::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> pOwner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	shared_ptr<CTeammateAI> pInstance = static_pointer_cast<CTeammateAI>(shared_from_this());

	m_StateInitial = make_shared<CStateTemplate<CFSM>>();
	m_StateInitial->Set(pInstance, &CFSM::BeginStateInitial, &CFSM::StateInitial, &CFSM::EndStateInitial);
	m_pCurrentState = m_StateInitial;

	m_pState_Idle = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Idle->Set(pInstance, &CTeammateAI::BeginState_Idle, &CTeammateAI::ExecuteState_Idle, &CTeammateAI::EndState_Idle);
	m_pState_Chase = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Chase->Set(pInstance, &CTeammateAI::BeginState_Chase, &CTeammateAI::ExecuteState_Chase, &CTeammateAI::EndState_Chase);
	m_pState_Jump = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Jump->Set(pInstance, &CTeammateAI::BeginState_Jump, &CTeammateAI::ExecuteState_Jump, &CTeammateAI::EndState_Jump);
	m_pState_Hit = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Hit->Set(pInstance, &CTeammateAI::BeginState_Hit, &CTeammateAI::ExecuteState_Hit, &CTeammateAI::EndState_Hit);

	m_pState_Teleport = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Teleport->Set(pInstance, &CTeammateAI::BeginState_Teleport, &CTeammateAI::ExecuteState_Teleport, &CTeammateAI::EndState_Teleport);
	m_pState_SAS = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_SAS->Set(pInstance, &CTeammateAI::BeginState_SAS, &CTeammateAI::ExecuteState_SAS, &CTeammateAI::EndState_SAS);
	m_pState_Trans_Battle = make_shared<CStateTemplate<CTeammateAI>>();
	m_pState_Trans_Battle->Set(pInstance, &CTeammateAI::BeginState_Trans_Battle, &CTeammateAI::ExecuteState_Trans_Battle, &CTeammateAI::EndState_Trans_Battle);

	m_pOwner = dynamic_pointer_cast<CTeammate>(pOwner);
	m_pTransform = dynamic_pointer_cast<CTransform>(pOwner->Get_Component(COMPONENT::TRANSFORM));
	m_pModel = dynamic_pointer_cast<CModel>(pOwner->Get_Component(COMPONENT::MODEL));

	m_pPlayer = dynamic_pointer_cast<CPlayer>(Function::Find_Player());
	m_pPlayerTransform = dynamic_pointer_cast<CTransform>(m_pPlayer.lock()->Get_Component(COMPONENT::TRANSFORM));

	m_pActorDesc = pActorDesc;

	auto TeammatePool = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_TEAMMATE);
	if (nullptr != TeammatePool->Get_GameObject(0).get())
		m_pOtherTeammate = dynamic_pointer_cast<CTeammate>(TeammatePool->Get_GameObject(0));
	if (nullptr != m_pOtherTeammate.lock())
		m_pOtherTeammateTransform = dynamic_pointer_cast<CTransform>(m_pOtherTeammate.lock()->Get_Component(COMPONENT::TRANSFORM));
	return S_OK;
}

void CTeammateAI::StateInitial(_float fTimeDelta)
{
	m_fChaseDistanceThreshold = 3.f;

	m_fStopChaseDistanceThreshold = 2.f;
	m_fRunChaseDistanceThreshold = 6.f;

	GotoState(m_pState_Idle);
}

/*==================================== Idle ====================================*/
#pragma region Idle State
void CTeammateAI::BeginState_Idle()
{
	auto pOwner = m_pOwner.lock();
	m_pModel.lock()->Set_Animation(m_iWaitAnimIndex, 2.f, false, 0.2f, true);

	pOwner->Init_RootMotion();

	m_bOnBattle = false;
}

void CTeammateAI::ExecuteState_Idle(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);
	pOwner->RootMotion(fTimeDelta);

	_float3 vOwnerPosition = XMVectorSetY(m_pTransform.lock()->Get_State(TRANSFORM::POSITION), 0.f);
	_float3 vPlayerPosition = XMVectorSetY(m_pPlayerTransform.lock()->Get_State(TRANSFORM::POSITION), 0.f);

	if (XMVectorGetX(XMVector3Length(vPlayerPosition - vOwnerPosition)) >= m_fChaseDistanceThreshold)
	{
		GotoState(m_pState_Chase);
	}
}

void CTeammateAI::EndState_Idle()
{
	m_pOwner.lock()->Reset_Motion();
}
#pragma endregion

/*==================================== Chase ====================================*/
#pragma region Chase
void CTeammateAI::BeginState_Chase()
{
	auto pOwner = m_pOwner.lock();
	m_pModel.lock()->Set_Animation(m_iWalkAnimIndex, 2.f, false, 0.2f, true);

	pOwner->Init_RootMotion();

	m_bDash = false;
}

void CTeammateAI::ExecuteState_Chase(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);

	auto pTransform = m_pTransform.lock();
	_float3 vOwnerPosition = XMVectorSetY(pTransform->Get_State(TRANSFORM::POSITION), 0.f);
	_float3 vPlayerPosition = XMVectorSetY(m_pPlayerTransform.lock()->Get_State(TRANSFORM::POSITION), 0.f);

	pTransform->LookAt_Interpolation(vPlayerPosition, true);

	_float fSpeed = (pOwner->Get_CurrentAnimation() == m_iDashAnimIndex) ? m_fDashSpeed :
		(pOwner->Get_CurrentAnimation() == m_iRunAnimIndex) ? m_fRunSpeed : m_fWalkSpeed;
	pTransform->Translate(XMVector3Normalize(pTransform->Get_State(TRANSFORM::LOOK)) * fSpeed * fTimeDelta);

	_float3 vTranslatedPosition = pTransform->Get_State(TRANSFORM::POSITION);
	float4 vGroundPosition;
	if (pOwner->Detect_Ground(vGroundPosition))
	{
		vTranslatedPosition.y = vGroundPosition.y;
		pTransform->Set_State(TRANSFORM::POSITION, vTranslatedPosition);
	}
	//if (nullptr != m_pOtherTeammateTransform.lock())
	//{
	//	_float Dist = Function::Distance(m_pTransform.lock(), m_pOtherTeammateTransform.lock());
	//	if (Function::Distance(m_pTransform.lock(), m_pOtherTeammateTransform.lock()) <= 1.f)
	//	{
	//		m_pTransform.lock()->Translate(m_pTransform.lock()->Get_State(TRANSFORM::LOOK) * fTimeDelta);
	//		m_pOwner.lock()->RootMotion(fTimeDelta);
	//	}
	//}

	pOwner->Motion(fTimeDelta);

	vTranslatedPosition.y = 0.f;
	_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vTranslatedPosition));
	if (false == m_bDash && fDistance >= m_fRunChaseDistanceThreshold)
	{
		m_bDash = true;

		pOwner->Reset_Motion();
		m_pModel.lock()->Set_Animation(m_iDashAnimIndex, 2.f, false, 0.2f, true);
		pOwner->Init_RootMotion();

	}
	else if (m_bDash && fDistance < m_fRunChaseDistanceThreshold)
	{
		m_bDash = false;

		pOwner->Reset_Motion();
		m_pModel.lock()->Set_Animation(m_iRunAnimIndex, 2.f, false, 0.4f, true);
		pOwner->Init_RootMotion();
	}

	PxRaycastBuffer RayCallback;
	if (pOwner->Detect_Obstacle(RayCallback, 0.85f))
	{
		if (RayCallback.block.actor->userData == nullptr)
		{
			GotoState(m_pState_Jump);
		}
	}

	if (fDistance <= m_fStopChaseDistanceThreshold)
	{
		GotoState(m_pState_Idle);
	}
}

void CTeammateAI::EndState_Chase()
{
	m_pOwner.lock()->Reset_Motion();
}
#pragma endregion

/*==================================== Jump ====================================*/
#pragma region Jump
void CTeammateAI::BeginState_Jump()
{
	auto pOwner = m_pOwner.lock();
	auto pModel = m_pModel.lock();

	pModel->Reset_Animation(m_iJumpLandingAnimIndex);
	pModel->Set_Animation(m_iJumpStartAnimIndex, 2.f, false, 0.2f, false);

	pOwner->Init_RootMotion();

	m_bOnGround = true;
	m_fPrevHeight = FLT_MIN;

	m_fAccAirTime = 0.f;

	m_bJumpStartEnded = false;
	m_bFall = false;

	CGameInstance::Get_Instance()->PlaySoundW(m_wstrJumpSound, 1.f);
}

void CTeammateAI::ExecuteState_Jump(_float fTimeDelta)
{
	auto pModel = m_pModel.lock();
	auto pOwner = m_pOwner.lock();
	auto pTransform = m_pTransform.lock();
	pOwner->Update_Motion(fTimeDelta);

	if (false == m_bJumpStartEnded && pModel->Is_AnimationFinished(m_iJumpStartAnimIndex))
	{
		m_bJumpStartEnded = true;
		pOwner->Reset_Motion();
		pModel->Set_Animation(m_iJumpRiseAnimIndex, 1.f, false, 0.2f, false);
		pModel->Reset_Animation(m_iJumpRiseAnimIndex);
	}

	_float3 vOwnerPosition = pTransform->Get_State(TRANSFORM::POSITION);
	if (false == m_bFall && pModel->Is_AnimationPlaying(m_iJumpRiseAnimIndex))
	{
		if (vOwnerPosition.y < m_fPrevHeight)
		{
			m_bFall = true;
			pOwner->Reset_Motion();
			pModel->Set_Animation(m_iJumpFallAnimIndex, 1.f, false, 0.2f, false);
			pModel->Reset_Animation(m_iJumpFallAnimIndex);
		}
		else
		{
			m_fPrevHeight = vOwnerPosition.y;
		}

		m_bOnGround = false;
	}

	_bool bResetGravityVelocity = true;
	if (false == m_bOnGround)
	{
		pTransform->Translate(XMVector3Normalize(pTransform->Get_State(TRANSFORM::UP)) * m_fJumpForce * fTimeDelta);
		pTransform->Translate(XMVector3Normalize(pTransform->Get_State(TRANSFORM::LOOK)) * m_fJumpForwardForce * fTimeDelta);

		_float4 vGoundPos;
		_bool bHitGround = pOwner->Detect_Ground(vGoundPos);
		if (m_bFall)
		{
			bResetGravityVelocity = false;
			if (abs(vGoundPos.y - vOwnerPosition.y) <= 0.2f)
			{
				m_bOnGround = true;
				pOwner->Reset_Motion();

				pModel->Set_Animation(m_iJumpLandingAnimIndex, 1.f, false, 0.2f, false);
				pModel->Reset_Animation(m_iJumpLandingAnimIndex);
			}
		}

		m_fAccAirTime += fTimeDelta;
		if (m_fAccAirTime >= m_fMaxAirTime)
		{
			if (bHitGround)
			{
				pTransform->Set_State(TRANSFORM::POSITION, vGoundPos);
				m_fAccAirTime = 0.f;

				m_bOnGround = true;

				pModel->Set_Animation(m_iJumpLandingAnimIndex, 1.f, false, 0.2f, false);
				pModel->Reset_Animation(m_iJumpLandingAnimIndex);

				CGameInstance::Get_Instance()->PlaySoundW(m_wstrLandingSound, 1.f);
			}
		}
	}

	if (m_bOnGround && pModel->Is_AnimationFinished(m_iJumpLandingAnimIndex))
	{
		GotoState(m_pState_Idle);
	}

	pOwner->RootMotion(fTimeDelta, bResetGravityVelocity);
}

void CTeammateAI::EndState_Jump()
{
	auto pModel = m_pModel.lock();

	pModel->Reset_Animation(m_iJumpStartAnimIndex);
	pModel->Reset_Animation(m_iJumpRiseAnimIndex);
	pModel->Reset_Animation(m_iJumpFallAnimIndex);

	m_pOwner.lock()->Reset_Motion();
}

#pragma endregion

/*==================================== Hit ====================================*/
#pragma region Hit State
void CTeammateAI::BeginState_Hit()
{
}

void CTeammateAI::ExecuteState_Hit(_float fTimeDelta)
{
}

void CTeammateAI::EndState_Hit()
{
}
#pragma endregion

void CTeammateAI::BeginState_Teleport()
{
	auto pOwner = m_pOwner.lock();
	auto pModel = m_pModel.lock();

	pModel->Set_Animation(m_iWaitAnimIndex, 2.f, false, 0.2f, false);

	pOwner->Init_RootMotion();
	pOwner->Enable_Controller(false);
}

void CTeammateAI::ExecuteState_Teleport(_float fTimeDelta)
{
	auto pOwner = m_pOwner.lock();
	pOwner->Update_Motion(fTimeDelta);
	if (false == m_bTeleport)
	{
		m_bTeleport = true;

		auto pOwnerTransform = m_pTransform.lock();
		auto pPlayerTransform = dynamic_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM));
		_float3 vPlayerPosition = pPlayerTransform->Get_State(TRANSFORM::POSITION);

		pOwnerTransform->Set_State(TRANSFORM::POSITION, vPlayerPosition);
	}
	pOwner->Motion(fTimeDelta);

	GotoState(m_pState_Idle);
}

void CTeammateAI::EndState_Teleport()
{
	m_bTeleport = false;
	m_pOwner.lock()->Reset_Motion();
	m_pOwner.lock()->Enable_Controller(true);
}

void CTeammateAI::BeginState_SAS()
{

}

void CTeammateAI::ExecuteState_SAS(_float fTimeDelta)
{
}

void CTeammateAI::EndState_SAS()
{
}

/*==================================== Trans_Battle ====================================*/
#pragma region Trans Battle

void CTeammateAI::BeginState_Trans_Battle()
{
	m_pModel.lock()->Set_Animation(m_iUpTransBattleAnimIndex, 2.f, false, 0.2f, false);
	m_pOwner.lock()->Enable_Weapon(true);
}

void CTeammateAI::ExecuteState_Trans_Battle(_float fTimeDelta)
{
	if (m_pModel.lock()->Is_AnimationFinished(m_iUpTransBattleAnimIndex))
	{
		GotoState(m_pState_Idle);
	}
}

void CTeammateAI::EndState_Trans_Battle()
{

}

#pragma endregion

void CTeammateAI::ExecutePersistent(_float fTimeDelta)
{
	auto pOwnerTransform = m_pTransform.lock();
	if (false == m_bBattleMode)
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		auto pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);

		auto Iter = pLayer->Begin();
		while (Iter != pLayer->End())
		{
			auto pMonsterTrasnform = dynamic_pointer_cast<CTransform>((*Iter)->Get_Component(COMPONENT::TRANSFORM));

			_float3 vMonsterPosition = pMonsterTrasnform->Get_State(TRANSFORM::POSITION);
			_float3 vPosition = pOwnerTransform->Get_State(TRANSFORM::POSITION);

			if (XMVectorGetX(XMVector3Length(vMonsterPosition - vPosition)) <= m_fBattleModeDistanceThreshold)
			{
				m_bBattleMode = true;
				GotoState(m_pState_Trans_Battle);

				break;
			}

			++Iter;
		}
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pPlayerTransform = dynamic_pointer_cast<CTransform>(pGameInstance->Get_Player()->Get_Component(COMPONENT::TRANSFORM));

	_float3 vPosition = pOwnerTransform->Get_State(TRANSFORM::POSITION);
	_float3 vPlayerPosition = pPlayerTransform->Get_State(TRANSFORM::POSITION);

	_float fDistanceFromPlayer = XMVectorGetX(XMVector3Length(vPosition - vPlayerPosition));
	if (fDistanceFromPlayer >= 30.f || CGameInstance::Get_Instance()->Key_Down(VK_UP))
	{
		GotoState(m_pState_Teleport);
	}

	//if (CGameInstance::Get_Instance()->Key_Down('V'))
	//{
	//	GotoState(m_pState_SAS);
	//}
}

shared_ptr<CTeammateAI> CTeammateAI::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc)
{
	shared_ptr<CTeammateAI> pInstance = make_private_shared(CTeammateAI);

	if (FAILED(pInstance->Initialize(pDevice, pContext, Owner, pEntityDesc, pActorDesc)))
	{
		MSG_RETURN(nullptr, "CTeammateAI::Create", "Failed to Initialize");
	}

	return pInstance;
}
