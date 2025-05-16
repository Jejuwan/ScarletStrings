#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeLeaf_Animation.h"
#include "Monster.h"

CBehaviorTreeLeaf_Animation::CBehaviorTreeLeaf_Animation(const _uint _iAnimationIndex, _bool _bRootMotion, const string& _strReferenceBone, const string& _strRootBone,
	_float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
	: m_iAnimationLoop			(_iAnimationIndex)
	, m_fPlaySpeed				(_fPlaySpeed)
	, m_bReverse				(_bReverse)
	, m_fInterpolationDuration	(_fInterpolationDuration)
	, m_bLoop					(_bLoop)
	, m_bRootMotion				(_bRootMotion)
	, m_strReferenceBone		(_strReferenceBone)
	, m_strRootBone				(_strRootBone)
{
}

HRESULT CBehaviorTreeLeaf_Animation::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Animation";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeLeaf_Animation::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeLeaf_Animation::Initialize", "Failed to Owner");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeLeaf_Animation::Initialize", "Failed to Owner:Animator");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeLeaf_Animation::Initialize", "Failed to Owner:Model");
	}

	return S_OK;
}

void CBehaviorTreeLeaf_Animation::Activate()
{
	__super::Activate();

	if (FAILED(m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex(m_strReferenceBone.c_str()), m_pModel->Get_BoneIndex(m_strRootBone.c_str()))))
	{
		MSG_BOX("CBehaviorTreeLeaf_Animation::Activate", "Failed to SetRootBoneIndex");
	}

	m_pAnimator->Play_Animation(m_iAnimationLoop, m_fPlaySpeed, m_bReverse, m_fInterpolationDuration, m_bLoop);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CBehaviorTreeLeaf_Animation::Invoke(_float _fTimeDelta)
{
//	if (IDX(ANIMATION::DISPEN_PERRY::ATK_LEAP_JITABATA) == m_iAnimationLoop)
//	{
//		int a = 1;
//	}

	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_bRootMotion ? m_pOwner.lock()->RootMotion(_fTimeDelta) : m_pOwner.lock()->No_RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(m_iAnimationLoop))
	{
		if (m_pAnimator->Is_Finished(m_iAnimationLoop))
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

void CBehaviorTreeLeaf_Animation::Terminate()
{
	__super::Terminate();

	m_pOwner.lock()->MotionReset();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeLeaf_Animation::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeLeaf_Animation> CBehaviorTreeLeaf_Animation::Create(shared_ptr<CBlackBoard> _pBlackBoard,
	const string& _strReferenceBone, const string& _strRootBone, _bool _bRootMotion,
	_uint _iAnimationIndex, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
{
	shared_ptr<CBehaviorTreeLeaf_Animation> pInstance = make_private_shared(CBehaviorTreeLeaf_Animation,
		IDX(_iAnimationIndex), _bRootMotion, _strReferenceBone, _strRootBone, _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeLeaf_Animation::Create", "Failed to Initialize");
	}

	return pInstance;
}
