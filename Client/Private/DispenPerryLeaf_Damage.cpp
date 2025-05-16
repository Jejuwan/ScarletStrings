#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"

HRESULT CDispenPerryLeaf_Damage::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Damage";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Damage::Initialize", "Failed to Target:Transform");
	}

	return S_OK;
}

void CDispenPerryLeaf_Damage::Activate()
{
	__super::Activate();

	// 투사체 피격 위치 계산 후 DIRECTION
	switch (m_pOwner.lock()->Get_RecentEvent().second)
	{
	case DP_STATE::DAMAGE_I:
		m_bSuccess = true;
		m_eAnimation = ANIMATION::DISPEN_PERRY::DAMAGE_B;
		break;
	case DP_STATE::DAMAGE_M:
		m_bSuccess = true;
		m_eAnimation = ANIMATION::DISPEN_PERRY::DAMAGE_KNOCKBACK_B;
		break;
	default:
		m_bSuccess = false;
		break;
	}

	if (m_bSuccess)
	{
		m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
		m_pAnimator->Play_Animation(m_eAnimation, 3.f, false, g_fDefaultInterpolationDuration, false);
		m_pOwner.lock()->InitRootMotion();
	}
}

STATUS CDispenPerryLeaf_Damage::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_bSuccess)
	{
		m_pOwner.lock()->Update_Motion(_fTimeDelta);
		m_pOwner.lock()->RootMotion(_fTimeDelta);

		if (m_pAnimator->Is_Playing(m_eAnimation))
		{
			if (m_pAnimator->Is_Finished(m_eAnimation))
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
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CDispenPerryLeaf_Damage::Terminate()
{
	__super::Terminate();

	if (m_bSuccess)
	{
		m_pOwner.lock()->MotionReset();
	}
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_Damage::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerryLeaf_Damage> CDispenPerryLeaf_Damage::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryLeaf_Damage> pInstance = make_private_shared(CDispenPerryLeaf_Damage);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_Damage::Create", "Failed to Initialize");
	}

	return pInstance;
}
