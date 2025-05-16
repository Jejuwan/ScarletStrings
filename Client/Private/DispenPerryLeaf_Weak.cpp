#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerryLeaf_Weak.h"
#include "Monster_DispenPerry.h"

#define TEST_CODE

HRESULT CDispenPerryLeaf_Weak::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Weak";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Weak::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Weak::Initialize", "Failed to Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Weak::Initialize", "Failed to Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Weak::Initialize", "Failed to Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Weak::Initialize", "Failed to Owner:Animator");
	}

	return S_OK;
}

void CDispenPerryLeaf_Weak::Activate()
{
	__super::Activate();

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::DAMAGE_WEAK, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_Weak::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::DISPEN_PERRY::DAMAGE_WEAK))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::DISPEN_PERRY::DAMAGE_WEAK))
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

#ifdef TEST_CODE
	if (CGameInstance::Get_Instance()->Key_Down('B'))
	{
		m_eStatus = STATUS::FAILURE;
	}
#endif

	return Return_Invoke();
}

void CDispenPerryLeaf_Weak::Terminate()
{
	m_pOwner.lock()->MotionReset();

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_Weak::DebugLog()
{
}
#endif

shared_ptr<CDispenPerryLeaf_Weak> CDispenPerryLeaf_Weak::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryLeaf_Weak> pInstance = make_private_shared(CDispenPerryLeaf_Weak);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_Weak::Create", "Failed to Initialize");
	}

	return pInstance;
}
