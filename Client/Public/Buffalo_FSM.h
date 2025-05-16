#pragma 
#include "Client_Define.h"
#include "FSM.h"
#include "StateTemplate.h"
#include "Monster_Buffalo.h"

BEGIN(Client)

class CBuffalo_FSM :
    public CFSM
{
private :
    CBuffalo_FSM() DEFAULT;
    virtual ~CBuffalo_FSM() DEFAULT;

public:
    HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc);

public:
    virtual void StateInitial(_float fTimeDelta) override;

private :
    virtual void ExecutePersistent(_float fTimeDelta) override;
//idle
private :
    void BeginState_Idle();
    void ExecuteState_Idle(_float fTimeDelta);
    void EndState_Idle();
//Walk
private :
    void BeginState_Walk();
    void ExecuteState_Walk(_float fTimeDelta);
    void EndState_Walk();
//Run
private :
    void BeginState_Run();
    void ExecuteState_Run(_float fTimeDelta);
    void EndState_Run();
//Swing
private :
    void BeginState_Swing();
    void ExecuteState_Swing(_float fTimeDelta);
    void EndState_Swing();
//Rush
private :
    void BeginState_Rush();
    void ExecuteState_Rush(_float fTimeDelta);
    void EndState_Rush();
//BackKick
private :
    void BeginState_BackKick();
    void ExecuteState_BackKick(_float fTimeDelta);
    void EndState_BackKick();
//OnHit_Front
private :
    void BeginState_OnHit_Front();
    void ExecuteState_OnHit_Front(_float fTimeDelta);
    void EndState_OnHit_Front();
//OnHit_Back
private :
    void BeginState_OnHit_Back();
    void ExecuteState_OnHit_Back(_float fTimeDelta);
    void EndState_OnHit_Back();
//OnHit_Psy_Front
private :
    void BeginState_OnHit_Psy_Front();
    void ExecuteState_OnHit_Psy_Front(_float fTimeDelta);
    void EndState_OnHit_Psy_Front();
//OnHit_Psy_Back
private :
    void BeginState_OnHit_Psy_Back();
    void ExecuteState_OnHit_Psy_Back(_float fTimeDelta);
    void EndState_OnHit_Psy_Back();
//OnHit_Psy_Left
private :
    void BeginState_OnHit_Psy_Left();
    void ExecuteState_OnHit_Psy_Left(_float fTimeDelta);
    void EndState_OnHit_Psy_Left();
//OnHit_Psy_Right
private :
    void BeginState_OnHit_Psy_Right();
    void ExecuteState_OnHit_Psy_Right(_float fTimeDelta);
    void EndState_OnHit_Psy_Right();
//OnHit_Psy_Right
private :
    void BeginState_Dead();
    void ExecuteState_Dead(_float fTimeDelta);
    void EndState_Dead();

private :
    weak_ptr<CMonster_Buffalo>                              m_pOwner;

private :
    shared_ptr<CStateTemplate<CFSM>>			            m_StateInitial;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Idle;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Walk;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Run;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Swing;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Rush;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_BackKick;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Front;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Back;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Psy_Front;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Psy_Back;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Psy_Left;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_OnHit_Psy_Right;
    shared_ptr<CStateTemplate<CBuffalo_FSM>>			    m_pState_Dead;
    
private :
    shared_ptr<CTransform>                                  m_pTransform;
    shared_ptr<CTransform>                                  m_pTargetTransform;
    shared_ptr<CAnimator>						            m_pAnimator;
    shared_ptr<CModel>  						            m_pModel;

private:
    ComPtr<ID3D11Device>									m_pDevice;
    ComPtr<ID3D11DeviceContext>								m_pContext;
    shared_ptr<CRenderer>	                                m_pRenderer;

private :
    ENTITYDESC*                                             m_pEntityDesc = nullptr;
    const ACTORDESC*                                        m_pActorDesc = nullptr;
    _float                                                  m_fDistance = { 0 };
    _float                                                  m_fCoolTime = { 0 };
    _float                                                  m_fDegree = { 0 };

private:
    shared_ptr<class CUI_Indicator_Monster_NoticeMark>		m_pUI_NoticeMark;

public:
    static shared_ptr<CBuffalo_FSM> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesc, const ACTORDESC* pActorDesc);
};

END