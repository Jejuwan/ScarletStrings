#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_AnimationExit.h"
#include "Monster.h"

CBehaviorTreeDecorator_AnimationExit::CBehaviorTreeDecorator_AnimationExit(shared_ptr<CNode> _pNode,
	const _uint _iAnimationIndex, _bool _bRootMotion, const string& _strReferenceBone, const string& _strRootBone,
	_float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
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
	, m_bRootMotion				(_bRootMotion)
	, m_strReferenceBone		(_strReferenceBone)
	, m_strRootBone				(_strRootBone)
{
}

HRESULT CBehaviorTreeDecorator_AnimationExit::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "AnimationExit";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationExit::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationExit::Initialize", "Failed to Owner");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationExit::Initialize", "Failed to Owner:Animator");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationExit::Initialize", "Failed to Owner:Model");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_AnimationExit::Activate()
{
	__super::Activate();

	m_bComplete = false;
}

STATUS CBehaviorTreeDecorator_AnimationExit::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (!m_bComplete)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}

	if (m_pNode->Is_Terminated())
	{
		if (!m_bComplete)
		{
			if (FAILED(m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex(m_strReferenceBone.c_str()), m_pModel->Get_BoneIndex(m_strRootBone.c_str()))))
			{
				MSG_BOX("CBehaviorTreeLeaf_Animation::Activate", "Failed to SetRootBoneIndex");
			}

			m_pAnimator->Play_Animation(m_iAnimationIndex, m_fPlaySpeed, m_bReverse, m_fInterpolationDuration, m_bLoop);

			m_pOwner.lock()->InitRootMotion();

			m_bComplete = true;
		}

		m_pOwner.lock()->Update_Motion(_fTimeDelta);
		m_bRootMotion ? m_pOwner.lock()->RootMotion(_fTimeDelta) : m_pOwner.lock()->No_RootMotion(_fTimeDelta);

		if (m_pAnimator->Is_Playing(m_iAnimationIndex))
		{
			if (m_pAnimator->Is_Finished(m_iAnimationIndex))
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

	return Return_Invoke();
}

void CBehaviorTreeDecorator_AnimationExit::Terminate()
{
	m_pOwner.lock()->MotionReset();

	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_AnimationExit::DebugLog()
{
}
#endif

shared_ptr<CBehaviorTreeDecorator_AnimationExit> CBehaviorTreeDecorator_AnimationExit::Create(shared_ptr<CBlackBoard> _pBlackBoard,
	const string& _strReferenceBone, const string& _strRootBone, _bool _bRootMotion,
	_uint _iAnimationIndex, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_AnimationExit> pInstance = make_private_shared(CBehaviorTreeDecorator_AnimationExit, _pNode,
		IDX(_iAnimationIndex), _bRootMotion, _strReferenceBone, _strRootBone, _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_AnimationExit::Create", "Failed to Initialize");
	}

	return pInstance;
}
