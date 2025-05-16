#pragma 
#include "Client_Define.h"
#include "FSM.h"
#include "StateTemplate.h"
#include "Monster_Karen.h"

BEGIN(Client)

class CKaren_FSM :
    public CFSM
{
private:
    CKaren_FSM() DEFAULT;
    virtual ~CKaren_FSM() DEFAULT;

public:
    HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner,  ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);

public:
    virtual void StateInitial(_float fTimeDelta) override;

private:
    virtual void ExecutePersistent(_float fTimeDelta) override;
    //idle
private:
    void BeginState_Idle();
    void ExecuteState_Idle(_float fTimeDelta);
    void EndState_Idle();
    //CutScene
private:
    void BeginState_CutScene();
    void ExecuteState_CutScene(_float fTimeDelta);
    void EndState_CutScene();
    //Walk
private:
    void BeginState_Walk();
    void ExecuteState_Walk(_float fTimeDelta);
    void EndState_Walk();
    //Run
private:
    void BeginState_Run();
    void ExecuteState_Run(_float fTimeDelta);
    void EndState_Run();
    //Kick
private:
    void BeginState_KickCombo();
    void ExecuteState_KickCombo(_float fTimeDelta);
    void EndState_KickCombo();
private:
    void BeginState_Kick1();
    void ExecuteState_Kick1(_float fTimeDelta);
    void EndState_Kick1();
private:
    void BeginState_Kick2();
    void ExecuteState_Kick2(_float fTimeDelta);
    void EndState_Kick2();
private:
    void BeginState_Kick3();
    void ExecuteState_Kick3(_float fTimeDelta);
    void EndState_Kick3();
private:
    void BeginState_Kick4();
    void ExecuteState_Kick4(_float fTimeDelta);
    void EndState_Kick4();
    //Electric
private:
    void BeginState_Electric();
    void ExecuteState_Electric(_float fTimeDelta);
    void EndState_Electric();
    //Teleport
private:
    void BeginState_Teleport();
    void ExecuteState_Teleport(_float fTimeDelta);
    void EndState_Teleport();
private:
    void BeginState_Teleport_Dodge();
    void ExecuteState_Teleport_Dodge(_float fTimeDelta);
    void EndState_Teleport_Dodge();

private:
    void Teleport(_float Distance, _vectorf Pivot = XMVectorSet(0.f, 0.f, 0.f, 0.f));
    void Teleport(_float Distance, TRANSFORM Transform);

    //IceCorn
private:
    void BeginState_IceCorn();
    void ExecuteState_IceCorn(_float fTimeDelta);
    void EndState_IceCorn();
    //CloneDash
private:
    void BeginState_CloneDash();
    void ExecuteState_CloneDash(_float fTimeDelta);
    void EndState_CloneDash();
    //DodgeCloned
private:
    void BeginState_DodgeCloned();
    void ExecuteState_DodgeCloned(_float fTimeDelta);
    void EndState_DodgeCloned();
    //Dead
private:
    void BeginState_Dead();
    void ExecuteState_Dead(_float fTimeDelta);
    void EndState_Dead();
    //Finishmotion
private:
    void BeginState_FinishMotion();
    void ExecuteState_FinishMotion(_float fTimeDelta);
    void EndState_FinishMotion();

public:
    virtual void GotoFinish() { GotoState(m_pState_FinishMotion); };

private:
    void FindNewTarget();

private :
    weak_ptr<CMonster_Karen>                                m_pOwner;

private :
    shared_ptr<CStateTemplate<CFSM>>			            m_StateInitial;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_CutScene;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Idle;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Walk;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Run;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_KickCombo;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Kick1;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Kick2;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Kick3;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Kick4;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Electric;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Teleport;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Teleport_Dodge;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_IceCorn;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_CloneDash;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_DodgeCloned;

    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_Dead;
    shared_ptr<CStateTemplate<CKaren_FSM>>			        m_pState_FinishMotion;

private :
    shared_ptr<CTransform>                                  m_pTransform;
    shared_ptr<CTransform>                                  m_pTargetTransform;
    shared_ptr<CTransform>                                  m_pPlayerTransform;
    shared_ptr<CTransform>                                  m_pArashTransform;
    shared_ptr<CTransform>                                  m_pShidengTransform;
    shared_ptr<CAnimator>						            m_pAnimator;
    shared_ptr<CModel>						                m_pModel;

private:
    ComPtr<ID3D11Device>									m_pDevice;
    ComPtr<ID3D11DeviceContext>								m_pContext;
    shared_ptr<CRenderer>	                                m_pRenderer;

private :
    ENTITYDESC*                                             m_pEntityDesc = nullptr;
    ACTORDESC*                                              m_pActorDesc = nullptr;
    _float                                                  m_fDistance = { 0 };
    _float                                                  m_fCoolTime = { 0 };
    _float                                                  m_fDegree = { 0 };
    _float                                                  m_fHeight = { 0 };
    _float                                                  m_fTime = { 0 };

private:
     shared_ptr<class CUI_Indicator_Monster_NoticeMark>		m_pUI_NoticeMark;


public:
    static shared_ptr<CKaren_FSM> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesczz, ACTORDESC* pActorDesc);
};

END