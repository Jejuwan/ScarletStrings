#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerryLeaf_Turn.h"
#include "Monster_DispenPerry.h"
#include "Bone.h"

CDispenPerryLeaf_Turn::CDispenPerryLeaf_Turn(DIRECTION _eDirection)
	: m_eDirection(_eDirection)
{
}

HRESULT CDispenPerryLeaf_Turn::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Turn";
#endif

	switch (m_eDirection)
	{
	case DIRECTION::LEFT:
		m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_L_START;
		m_eAnimationRoop	= ANIMATION::DISPEN_PERRY::TURN_L_ROOP;
		break;
	case DIRECTION::RIGHT:
		m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_R_START;
		m_eAnimationRoop	= ANIMATION::DISPEN_PERRY::TURN_R_ROOP;
		break;

	case DIRECTION::DIR_END:
		break;

	default:
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Invalid Direction");
	}

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_EntityDesc: Owner:EntityDesc");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Walk::Initialize", "Failed to Get_System: Target:Transform");
	}

	m_mPivot = m_pModel->Get_Pivot();
	m_pBone = m_pModel->Get_Bone("Reference")->Get_CombinedTransformationPointer();
	if (nullptr == m_pBone)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_Turn::Initialize", "Failed to Get_Bone");
	}

	return S_OK;
}

void CDispenPerryLeaf_Turn::Activate()
{
	__super::Activate();

	m_bComplete				= false;
	m_bInitialAnimation		= true;
	m_fInitialRelative		= Function::RelativeAngle(m_pTransform, m_pTargetTransform);

	if (DIRECTION::DIR_END == m_eDirection)
	{
		if (m_fRelativeHorizontal < 0.f)
		{
		//	m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_L_START;
			m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_L_ROOP;
			m_eAnimationRoop	= ANIMATION::DISPEN_PERRY::TURN_L_ROOP;
		}
		else
		{
		//	m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_R_START;
			m_eAnimation		= ANIMATION::DISPEN_PERRY::TURN_R_ROOP;
			m_eAnimationRoop	= ANIMATION::DISPEN_PERRY::TURN_R_ROOP;
		}
	}

	m_pAnimator->Reset_Animation(ANIMATION::DISPEN_PERRY::MAX);

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(m_eAnimation, 3.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_Turn::Invoke(_float _fTimeDelta)
{
	_matrix m = *m_pBone * m_pTransform->Get_Matrix();

	Begin_Invoke(_fTimeDelta);

	m_fRelativeHorizontal	= Function::RelativeAngle(m.r[2], m.r[3], m_pTargetTransform->Get_State(TRANSFORM::POSITION));
	m_bComplete				= m_fInitialRelative * m_fRelativeHorizontal <= 0;

	if (CGameInstance::Get_Instance()->Exist_Event(m_pTransform))
	{
		m_eStatus = STATUS::FAILURE;
	}
	else if (!m_bInitialAnimation && m_bComplete)
	{
		m_pAnimator->Reset_Animation(m_bInitialAnimation ? m_eAnimation : m_eAnimationRoop);
		m_pTransform->LookTo(m.r[2]);
		m_eStatus = STATUS::SUCCESS;
	}
	else
	{
		m_pOwner.lock()->Update_Motion(_fTimeDelta);
		m_pOwner.lock()->RootMotion(_fTimeDelta);

		if (m_pAnimator->Is_Playing(m_eAnimation))
		{
			if (m_pAnimator->Is_Finished(m_eAnimation))
			{
				m_bInitialAnimation = false;
				m_pAnimator->Reset_Animation(m_eAnimation);
				m_pTransform->LookTo(m.r[2]);
				m_pOwner.lock()->MotionReset();
				m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
				m_pAnimator->Play_Animation(m_eAnimationRoop, 3.f, false, g_fDefaultInterpolationDuration, false);
				m_pOwner.lock()->InitRootMotion();
				m_eStatus = STATUS::RUNNING;
			}
			else
			{
				m_eStatus = STATUS::RUNNING;
			}
		}
		else if (m_pAnimator->Is_Playing(m_eAnimationRoop))
		{
			if (m_pAnimator->Is_Finished(m_eAnimationRoop))
			{
				m_pAnimator->Reset_Animation(m_eAnimationRoop);
				m_pTransform->LookTo(m.r[2]);
				m_pOwner.lock()->MotionReset();
				m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
				m_pAnimator->Play_Animation(m_eAnimationRoop, 3.f, false, g_fDefaultInterpolationDuration, false);
				m_pOwner.lock()->InitRootMotion();
				m_eStatus = STATUS::RUNNING;
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

	return Return_Invoke();
}

void CDispenPerryLeaf_Turn::Terminate()
{
	m_pOwner.lock()->MotionReset();

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_Turn::DebugLog()
{
}
#endif 

shared_ptr<CDispenPerryLeaf_Turn> CDispenPerryLeaf_Turn::Create(shared_ptr<CBlackBoard> _pBlackBoard , DIRECTION _eDirection)
{
	shared_ptr<CDispenPerryLeaf_Turn> pInstance = make_private_shared(CDispenPerryLeaf_Turn, _eDirection);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_Turn::Create", "Failed to Initialize");
	}

	return pInstance;
}
