#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerryLeaf_WeakDown.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerryLeaf_WeakDown::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Node";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WeakDown::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WeakDown::Initialize", "Failed to Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WeakDown::Initialize", "Failed to Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WeakDown::Initialize", "Failed to Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WeakDown::Initialize", "Failed to Owner:Animator");
	}

	return S_OK;
}

void CDispenPerryLeaf_WeakDown::Activate()
{
	__super::Activate();

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::DOWN_SHOCK, 3.f, false, g_fDefaultInterpolationDuration, true);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_WeakDown::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->No_RootMotion(_fTimeDelta);

	return Return_Invoke();
}

void CDispenPerryLeaf_WeakDown::Terminate()
{
	m_pOwner.lock()->MotionReset();

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_WeakDown::DebugLog()
{
}
#endif

shared_ptr<CDispenPerryLeaf_WeakDown> CDispenPerryLeaf_WeakDown::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryLeaf_WeakDown> pInstance = make_private_shared(CDispenPerryLeaf_WeakDown);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_WeakDown::Create", "Failed to Initialize");
	}

	return pInstance;
}
