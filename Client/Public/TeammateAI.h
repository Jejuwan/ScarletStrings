#pragma once

#include "Client_Define.h"
#include "FSM.h"
#include "StateTemplate.h"

BEGIN(Engine)
class CTransform;
class CModel;
END

BEGIN(Client)

class CTeammateAI : public CFSM
{
protected:
	CTeammateAI() DEFAULT;
	virtual ~CTeammateAI() DEFAULT;

public:
	virtual HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);

public:
	virtual void StateInitial(_float fTimeDelta) override;

protected:
	/*==================================== Idle ====================================*/
	virtual void BeginState_Idle();
	virtual void ExecuteState_Idle(_float fTimeDelta);
	virtual void EndState_Idle();

	/*==================================== Chase ====================================*/
	virtual void BeginState_Chase();
	virtual void ExecuteState_Chase(_float fTimeDelta);
	virtual void EndState_Chase();

	/*==================================== Jump ====================================*/
	virtual void BeginState_Jump();
	virtual void ExecuteState_Jump(_float fTimeDelta);
	virtual void EndState_Jump();

	/*==================================== Hit ====================================*/
	virtual void BeginState_Hit();
	virtual void ExecuteState_Hit(_float fTimeDelta);
	virtual void EndState_Hit();

	/*==================================== Teleport ====================================*/
	virtual void BeginState_Teleport();
	virtual void ExecuteState_Teleport(_float fTimeDelta);
	virtual void EndState_Teleport();

	/*==================================== SAS ====================================*/
	virtual void BeginState_SAS();
	virtual void ExecuteState_SAS(_float fTimeDelta);
	virtual void EndState_SAS();

	/*==================================== Trans_Battle ====================================*/
	virtual void BeginState_Trans_Battle();
	virtual void ExecuteState_Trans_Battle(_float fTimeDelta);
	virtual void EndState_Trans_Battle();

protected:
	virtual void ExecutePersistent(_float fTimeDelta) override;

protected:
	/*==================================== Idle ====================================*/
	_float											m_fChaseDistanceThreshold;
	_uint											m_iWaitAnimIndex;

	/*==================================== Chase ====================================*/
	_float											m_fStopChaseDistanceThreshold;
	_float											m_fRunChaseDistanceThreshold;

	_float											m_fWalkSpeed = { 2.f };
	_float											m_fRunSpeed = { 7.5f };
	_float											m_fDashSpeed = { 11.5f };

	_bool											m_bDash = { false };

	_uint											m_iWalkAnimIndex;
	_uint											m_iRunAnimIndex;
	_uint											m_iDashAnimIndex;

	/*==================================== Jump ====================================*/
	_float											m_fJumpForce = { 6.f };
	_float											m_fJumpForwardForce = { 5.f };

	_bool											m_bJumpStartEnded = { false };
	_bool											m_bFall = { false };

	_float											m_fPrevHeight = { FLT_MIN };

	_bool											m_bOnGround = { true };

	_float											m_fAccAirTime = { 0.f };
	_float											m_fMaxAirTime = { 2.f };

	_uint											m_iJumpStartAnimIndex;
	_uint											m_iJumpRiseAnimIndex;
	_uint											m_iJumpFallAnimIndex;
	_uint											m_iJumpLandingAnimIndex;

	wstring											m_wstrJumpSound;
	wstring											m_wstrLandingSound;

	/*==================================== Teleport ====================================*/
	_bool											m_bTeleport = { false };

	/*==================================== Persistent ====================================*/
	_bool											m_bBattleMode = { false };
	_bool											m_bOnBattle = { false };
	_float											m_fBattleModeDistanceThreshold = { 20.f };
	_float											m_fExecuteBattleDistanceThreshold = { 13.f };

	_uint											m_iUpTransBattleAnimIndex;

protected:
	weak_ptr<class CTeammate>						m_pOwner;
	weak_ptr<class CTeammate>						m_pOtherTeammate;
	weak_ptr<CTransform>							m_pTransform;
	weak_ptr<CTransform>							m_pOtherTeammateTransform;

	weak_ptr<CModel>								m_pModel;

	weak_ptr<class CPlayer>							m_pPlayer;
	weak_ptr<CTransform>							m_pPlayerTransform;

protected:
	ACTORDESC*										m_pActorDesc;

protected:
	shared_ptr<CStateTemplate<CFSM>>			    m_StateInitial;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Idle;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Chase;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Jump;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Hit;

	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Teleport;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_SAS;
	shared_ptr<CStateTemplate<CTeammateAI>>			m_pState_Trans_Battle;

protected:
	ComPtr<ID3D11Device>							m_pDevice;
	ComPtr<ID3D11DeviceContext>						m_pContext;

public:
	static shared_ptr<CTeammateAI> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);
};

END