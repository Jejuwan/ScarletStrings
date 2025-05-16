#pragma once

#include "Client_Define.h"
#include "TeammateAI.h"
#include "StateTemplate.h"

BEGIN(Client)

class CArashiAI final : public CTeammateAI
{
private:
	CArashiAI() DEFAULT;
	virtual ~CArashiAI() DEFAULT;

public:
	virtual HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);

#pragma region Default State
private:
	/*==================================== Idle ====================================*/
	virtual void BeginState_Idle() override;
	virtual void ExecuteState_Idle(_float fTimeDelta) override;
	virtual void EndState_Idle() override;

	/*==================================== Chase ====================================*/
	virtual void BeginState_Chase() override;
	virtual void ExecuteState_Chase(_float fTimeDelta) override;
	virtual void EndState_Chase() override;

	/*==================================== Jump ====================================*/
	virtual void BeginState_Jump() override;
	virtual void ExecuteState_Jump(_float fTimeDelta) override;
	virtual void EndState_Jump() override;

	/*==================================== Hit ====================================*/
	virtual void BeginState_Hit() override;
	virtual void ExecuteState_Hit(_float fTimeDelta) override;
	virtual void EndState_Hit() override;

	/*==================================== Trans_Battle ====================================*/
	virtual void BeginState_Trans_Battle() override;
	virtual void ExecuteState_Trans_Battle(_float fTimeDelta) override;
	virtual void EndState_Trans_Battle() override;
#pragma endregion
private:
	/*==================================== Battle Idle ====================================*/
	virtual void BeginState_Battle_Idle();
	virtual void ExecuteState_Battle_Idle(_float fTimeDelta);
	virtual void EndState_Battle_Idle();

	/*==================================== Battle Chase ====================================*/
	virtual void BeginState_Battle_Chase();
	virtual void ExecuteState_Battle_Chase(_float fTimeDelta);
	virtual void EndState_Battle_Chase();

	/*==================================== Attack Dodge ====================================*/
	virtual void BeginState_Attack_Dodge();
	virtual void ExecuteState_Attack_Dodge(_float fTimeDelta);
	virtual void EndState_Attack_Dodge();

	/*==================================== Attack ====================================*/
	virtual void BeginState_Attack();
	virtual void ExecuteState_Attack(_float fTimeDelta);
	virtual void EndState_Attack();

	/*==================================== Attack_Escape ====================================*/
	virtual void BeginState_Attack_Escape();
	virtual void ExecuteState_Attack_Escape(_float fTimeDelta);
	virtual void EndState_Attack_Escape();

	/*==================================== SAS ====================================*/
	virtual void BeginState_SAS() override;
	virtual void ExecuteState_SAS(_float fTimeDelta) override;
	virtual void EndState_SAS() override;

protected:
	virtual void ExecutePersistent(_float fTimeDelta) override;

private:
	/*==================================== Battle Idle ====================================*/


	/*==================================== Battle Chase ====================================*/
	_float											m_fAngle = { 0.f };
	_bool											m_bRightChase = { false };

	_float											m_fBattleChaseDistanceThreshold = { 10.f };
	_float											m_fChaseRadius = { 8.f };

	_float											m_fAttackDelay = { 0.f };
	_float											m_fAttackInterval = { 6.f };

	_bool											m_bAfterImageEffect = false;

	/*==================================== Attack ====================================*/
	_bool											m_bExecuteAttack = { false };
	_uint											m_iAttackAnimIndex;

	/*==================================== Attack Escape ====================================*/
	_bool											m_bExecuteEscape = { false };

	_uint											m_iEscapeStartAnimIndex;
	_uint											m_iEscapeStopAnimIndex;

	/*==================================== Persistent ====================================*/

	shared_ptr<class CMonster>						m_pTargetMonster;

private:
	shared_ptr<CStateTemplate<CArashiAI>>			m_pState_Battle_Idle;
	shared_ptr<CStateTemplate<CArashiAI>>			m_pState_Battle_Chase;
	shared_ptr<CStateTemplate<CArashiAI>>			m_pState_Attack_Dodge;
	shared_ptr<CStateTemplate<CArashiAI>>			m_pState_Attack;
	shared_ptr<CStateTemplate<CArashiAI>>			m_pState_Attack_Escape;

public:
	static shared_ptr<CTeammateAI> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);
};

END