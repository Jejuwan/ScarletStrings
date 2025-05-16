#pragma 
#include "Client_Define.h"
#include "FSM.h"
#include "StateTemplate.h"
#include "Monster_Snail.h"

BEGIN(Client)

class CSnail_FSM :
    public CFSM
{
private:
    CSnail_FSM() DEFAULT;
    virtual ~CSnail_FSM() DEFAULT;

public:
    HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc);

public:
    virtual void StateInitial(_float fTimeDelta) override;

private:
    virtual void ExecutePersistent(_float fTimeDelta) override;
    //idle
private:
    void BeginState_Idle();
    void ExecuteState_Idle(_float fTimeDelta);
    void EndState_Idle();
    //Walk
private:
    void BeginState_Walk();
    void ExecuteState_Walk(_float fTimeDelta);
    void EndState_Walk();
    //Walk_Left
private:
    void BeginState_Walk_Left();
    void ExecuteState_Walk_Left(_float fTimeDelta);
    void EndState_Walk_Left();
    //Walk_Right
private:
    void BeginState_Walk_Right();
    void ExecuteState_Walk_Right(_float fTimeDelta);
    void EndState_Walk_Right();
    //Walk_Back
private:
    void BeginState_Walk_Back();
    void ExecuteState_Walk_Back(_float fTimeDelta);
    void EndState_Walk_Back();

    //Run
    //private :
    //    void BeginState_Run();
    //    void ExecuteState_Run(_float fTimeDelta);
    //    void EndState_Run();
    //OilWalk
private:
    void BeginState_OilWalk();
    void ExecuteState_OilWalk(_float fTimeDelta);
    void EndState_OilWalk();
    //Shoot
private:
    void BeginState_Shoot();
    void ExecuteState_Shoot(_float fTimeDelta);
    void EndState_Shoot();
    //OnHit_Front
private:
    void BeginState_OnHit_Front();
    void ExecuteState_OnHit_Front(_float fTimeDelta);
    void EndState_OnHit_Front();
    //OnHit_Back
private:
    void BeginState_OnHit_Back();
    void ExecuteState_OnHit_Back(_float fTimeDelta);
    void EndState_OnHit_Back();
    //OnHit_Psy_Front
private:
    void BeginState_OnHit_Psy_Front();
    void ExecuteState_OnHit_Psy_Front(_float fTimeDelta);
    void EndState_OnHit_Psy_Front();
    //OnHit_Psy_Back
private:
    void BeginState_OnHit_Psy_Back();
    void ExecuteState_OnHit_Psy_Back(_float fTimeDelta);
    void EndState_OnHit_Psy_Back();
    //OnHit_Psy_Left
private:
    void BeginState_OnHit_Psy_Left();
    void ExecuteState_OnHit_Psy_Left(_float fTimeDelta);
    void EndState_OnHit_Psy_Left();
    //OnHit_Psy_Right
private:
    void BeginState_OnHit_Psy_Right();
    void ExecuteState_OnHit_Psy_Right(_float fTimeDelta);
    void EndState_OnHit_Psy_Right();
    //AirBone
private:
    void BeginState_AirBone();
    void ExecuteState_AirBone(_float fTimeDelta);
    void EndState_AirBone();
    //AirBone_Falling
private:
    void BeginState_AirBone_Falling();
    void ExecuteState_AirBone_Falling(_float fTimeDelta);
    void EndState_AirBone_Falling();
    //OnHit_Psy_Right
private:
    void BeginState_Dead();
    void ExecuteState_Dead(_float fTimeDelta);
    void EndState_Dead();
    //BCChance
private:
    void BeginState_BCChance();
    void ExecuteState_BCChance(_float fTimeDelta);
    void EndState_BCChance();
    //FinshMotion
private:
    void BeginState_FinishMotion();
    void ExecuteState_FinishMotion(_float fTimeDelta);
    void EndState_FinishMotion();

private :
    void FindNewTarget();

private :
    weak_ptr<CMonster_Snail>                        m_pOwner;

private :
    shared_ptr<CStateTemplate<CFSM>>			    m_StateInitial;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Idle;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Walk;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Walk_Left;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Walk_Right;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Walk_Back;
    //shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Run;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OilWalk;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Shoot;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Front;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Back;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Psy_Front;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Psy_Back;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Psy_Left;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_OnHit_Psy_Right;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_AirBone;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_AirBone_Falling;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_Dead;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_BCChance;
    shared_ptr<CStateTemplate<CSnail_FSM>>			m_pState_FinishMotion;
private :
    shared_ptr<CTransform>                          m_pTransform;
    shared_ptr<CTransform>                          m_pTargetTransform;
    shared_ptr<CTransform>                          m_pPlayerTransform;
    shared_ptr<CTransform>                          m_pArashTransform;
    shared_ptr<CTransform>                          m_pShidengTransform;
    shared_ptr<CAnimator>						    m_pAnimator;
    shared_ptr<CModel>  						    m_pModel;

private:
    ComPtr<ID3D11Device>							m_pDevice;
    ComPtr<ID3D11DeviceContext>						m_pContext;
    shared_ptr<CRenderer>	                        m_pRenderer;

private :
    ENTITYDESC*                                     m_pEntityDesc = nullptr;
    const ACTORDESC*                                m_pActorDesc= nullptr;
    _uint                                           m_iOilWalkStack = { 0 };
    _float                                          m_fDistance = { 0 };
    _float                                          m_fCoolTime = { 0 };
    _float                                          m_fDegree = { 0 };
    _float                                          m_fRootAmount = { 1.f };

private:
    shared_ptr<class CUI_Indicator_Monster_NoticeMark>		m_pUI_NoticeMark;
    shared_ptr<class CUI_Indicator_InstKill>                m_pUI_InstKIll;



public:
    static shared_ptr<CSnail_FSM> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc);
public:
    virtual void                                            GotoFinish() override;
};

END