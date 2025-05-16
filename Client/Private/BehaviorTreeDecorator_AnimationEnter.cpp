#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_AnimationEnter.h"
#include "Monster.h"

CBehaviorTreeDecorator_AnimationEnter::CBehaviorTreeDecorator_AnimationEnter(shared_ptr<CNode> _pNode,
	const _uint _iAnimationIndex, _bool _bRootMotion, const string& _strReferenceBone, const string& _strRootBone,
	_float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop, initializer_list<_uint> _ilPreventAnimOverlap)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_iAnimationIndex			(_iAnimationIndex)
	, m_fPlaySpeed				(_fPlaySpeed)
	, m_bReverse				(_bReverse)
	, m_fInterpolationDuration	(_fInterpolationDuration)
	, m_bLoop					(_bLoop)
	, m_lstPreventOverlapAnimIdx(_ilPreventAnimOverlap)
	, m_bRootMotion				(_bRootMotion)
	, m_strReferenceBone		(_strReferenceBone)
	, m_strRootBone				(_strRootBone)
{
}

HRESULT CBehaviorTreeDecorator_AnimationEnter::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "AnimationEnter";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationEnter::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationEnter::Initialize", "Failed to Owner");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationEnter::Initialize", "Failed to Owner:Animator");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationEnter::Initialize", "Failed to Owner:Model");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_AnimationEnter::Activate()
{
	__super::Activate();

	m_bComplete = false;

	if (FAILED(m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex(m_strReferenceBone.c_str()), m_pModel->Get_BoneIndex(m_strRootBone.c_str()))))
	{
		MSG_BOX("CBehaviorTreeLeaf_Animation::Activate", "Failed to SetRootBoneIndex");
	}

	if (UINT_MAX == Is_PlayingPreventOverlapAnimation()
	&&	!m_pAnimator->Is_Playing(m_iAnimationIndex))
	{
		m_pAnimator->Play_Animation(m_iAnimationIndex, m_fPlaySpeed, m_bReverse, m_fInterpolationDuration, m_bLoop);
		m_pOwner.lock()->InitRootMotion();
	}
}

STATUS CBehaviorTreeDecorator_AnimationEnter::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

//	_uint iPreventAnimIdx = Is_PlayingPreventOverlapAnimation();
//	if (UINT_MAX != iPreventAnimIdx)
//	{
//		if (m_pAnimator->Is_Finished(iPreventAnimIdx))
//		{
//			m_pOwner.lock()->MotionReset();
//			m_eStatus = m_pNode->Invoke(_fTimeDelta);
//		}
//		else
//		{
//			m_eStatus = STATUS::RUNNING;
//		}
//	}
//	else 
	if (m_pAnimator->Is_Playing(m_iAnimationIndex))
	{
		if (m_pAnimator->Is_Finished(m_iAnimationIndex))
		{
			if (!m_bComplete)
			{
				m_pOwner.lock()->MotionReset();
				m_bComplete = true;
			}

			m_eStatus = m_pNode->Invoke(_fTimeDelta);
		}
		else
		{
			m_pOwner.lock()->Update_Motion(_fTimeDelta);
			m_bRootMotion ? m_pOwner.lock()->RootMotion(_fTimeDelta) : m_pOwner.lock()->No_RootMotion(_fTimeDelta);
			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_AnimationEnter::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_AnimationEnter::DebugLog()
{
}
#endif

const _uint CBehaviorTreeDecorator_AnimationEnter::Is_PlayingPreventOverlapAnimation() const
{
	for (auto& iAnimIdx : m_lstPreventOverlapAnimIdx)
	{
		if (m_pAnimator->Is_Playing(iAnimIdx))
		{
			return iAnimIdx;
		}
	}

	return UINT_MAX;
}

shared_ptr<CBehaviorTreeDecorator_AnimationEnter> CBehaviorTreeDecorator_AnimationEnter::Create(shared_ptr<CBlackBoard> _pBlackBoard,
	const string& _strReferenceBone, const string& _strRootBone, _bool _bRootMotion,
	_uint _iAnimationIndex, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop, initializer_list<_uint> _ilPreventAnimOverlap, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_AnimationEnter> pInstance = make_private_shared(CBehaviorTreeDecorator_AnimationEnter, _pNode,
		IDX(_iAnimationIndex), _bRootMotion, _strReferenceBone, _strRootBone, _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop, _ilPreventAnimOverlap);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_AnimationEnter::Create", "Failed to Initialize");
	}

	return pInstance;
}
