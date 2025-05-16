#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerryLeaf_SpinningArmSwipe::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "SpinningArmSwipe";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_SpinningArmSwipe::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_SpinningArmSwipe::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_SpinningArmSwipe::Initialize", "Failed to Get_System: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_SpinningArmSwipe::Initialize", "Failed to Get_System: Owner:Animator");
	}

	return S_OK;
}

void CDispenPerryLeaf_SpinningArmSwipe::Activate()
{
	__super::Activate();

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::ATK_SPIN, 3.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_SpinningArmSwipe::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::DISPEN_PERRY::ATK_SPIN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::DISPEN_PERRY::ATK_SPIN))
		{
			m_eStatus = STATUS::SUCCESS;
		}
		else
		{
			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CDispenPerryLeaf_SpinningArmSwipe::Terminate()
{
	__super::Terminate();

	m_pOwner.lock()->SetWeaponeAllActiveOff();
	m_pOwner.lock()->MotionReset();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_SpinningArmSwipe::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerryLeaf_SpinningArmSwipe> CDispenPerryLeaf_SpinningArmSwipe::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryLeaf_SpinningArmSwipe> pInstance = make_private_shared(CDispenPerryLeaf_SpinningArmSwipe);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_SpinningArmSwipe::Create", "Failed to Initialize");
	}

	return pInstance;
}
