#pragma 
#include "Client_Define.h"
#include "FSM.h"
#include "StateTemplate.h"
#include "Monster_Statue.h"

BEGIN(Engine)
class CObjectPool;
END

BEGIN(Client)

class CStatue_FSM :
    public CFSM
{
private:
    CStatue_FSM() DEFAULT;
    virtual ~CStatue_FSM() DEFAULT;

public:
    HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner,  ENTITYDESC* pEntityDesc, ACTORDESC* pActorDesc);

public:
    virtual void StateInitial(_float fTimeDelta) override;

private:
    virtual void ExecutePersistent(_float fTimeDelta) override;

#pragma region Cutscene
private:
    // Cutscene Intialize
    void BeginState_Cutscene_Initialize();
    void ExecuteState_Cutscene_Initialize(_float fTimeDelta);
    void EndState_Cutscene_Initialize();

    void BeginState_Cutscene_Positioning();
    void ExecuteState_Cutscene_Positioning(_float fTimeDelta);
    void EndState_Cutscene_Positioning();

    void BeginState_Cutscene_KarenTravers();
    void ExecuteState_Cutscene_KarenTravers(_float fTimeDelta);
    void EndState_Cutscene_KarenTravers();
#pragma endregion

    //idle
private:
    void BeginState_Idle();
    void ExecuteState_Idle(_float fTimeDelta);
    void EndState_Idle();
    //attack
private:
    void BeginState_Attack();
    void ExecuteState_Attack(_float fTimeDelta);
    void EndState_Attack();
    //OnHit_Front
private:
    void BeginState_OnHit_Front();
    void ExecuteState_OnHit_Front(_float fTimeDelta);
    void EndState_OnHit_Front();
    //Dead
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

public :
    void StatueFalling();
    void Stop_Cutscene();

private :
    weak_ptr<CMonster_Statue>                               m_pOwner;

private :
    shared_ptr<CStateTemplate<CFSM>>			            m_StateInitial;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Idle;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Attack;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_OnHit;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Dead;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_BCChance;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_FinishMotion;

    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Cutscene_Init;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Cutscene_Positioning;
    shared_ptr<CStateTemplate<CStatue_FSM>>			        m_pState_Cutscene_KarenTravers;

private :
    shared_ptr<CTransform>                                  m_pTransform;
    shared_ptr<CTransform>                                  m_pTargetTransform;
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
    _uint                                                   m_MaskType = { 3 };
    weak_ptr<CObjectPool>                                 m_FallenPool[3];
    weak_ptr<CObjectPool>                                 m_BFObjectPool[3];
    _uint                                                 m_iPoolIdx = 0;

    // Cutscene
private:
    _uint                                                   m_iCutsceneIndex;

private:
    _bool                                                   m_bCutscenePlaying;

private:
     shared_ptr<class CUI_Indicator_Monster_NoticeMark>		m_pUI_NoticeMark;

private:
    _bool m_bExecuted[10];

public:
    static shared_ptr<CStatue_FSM> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CGameObject> Owner, ENTITYDESC* pEntityDesczz, ACTORDESC* pActorDesc);
};

END