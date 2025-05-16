#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

CDispenPerryLeaf_Wait::CDispenPerryLeaf_Wait(DIRECTION _eDirection)
	: m_eDirection(_eDirection)
{
}

HRESULT CDispenPerryLeaf_Wait::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Wait";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Wait::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Wait::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Wait::Initialize", "Failed to Get_System: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Wait::Initialize", "Failed to Get_System: Owner:Animator");
	}

	return S_OK;
}

void CDispenPerryLeaf_Wait::Activate()
{
	__super::Activate();

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::ATK_FRONTARM_L, 3.f);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_Wait::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::DISPEN_PERRY::ATK_FRONTARM_L))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::DISPEN_PERRY::ATK_FRONTARM_L))
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

void CDispenPerryLeaf_Wait::Terminate()
{
	__super::Terminate();

	m_pOwner.lock()->MotionReset();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_Wait::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerryLeaf_Wait> CDispenPerryLeaf_Wait::Create(shared_ptr<CBlackBoard> _pBlackBoard, DIRECTION _eDirection)
{
	shared_ptr<CDispenPerryLeaf_Wait> pInstance = make_private_shared(CDispenPerryLeaf_Wait, _eDirection);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_Wait::Create", "Failed to Initialize");
	}

	return pInstance;
}
