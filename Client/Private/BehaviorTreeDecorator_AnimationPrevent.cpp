#include "ClientPCH.h"
#include "GameInstance.h"
#include "BehaviorTreeDecorator_AnimationPrevent.h"

CBehaviorTreeDecorator_AnimationPrevent::CBehaviorTreeDecorator_AnimationPrevent(shared_ptr<CNode> _pNode, initializer_list<_uint> _ilPrevent)
#ifdef _DEBUG
	: CDebugDecorator(_pNode)
#else
	: CDecorator(_pNode)
#endif
	, m_lstPrevent(_ilPrevent)
{
}

HRESULT CBehaviorTreeDecorator_AnimationPrevent::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "AnimationPrevent";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationPrevent::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTreeDecorator_AnimationPrevent::Initialize", "Failed to Owner:Animator");
	}

	return S_OK;
}

void CBehaviorTreeDecorator_AnimationPrevent::Activate()
{
	__super::Activate();
}

STATUS CBehaviorTreeDecorator_AnimationPrevent::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (Is_PlayingPreventAnimation())
	{
		m_eStatus = STATUS::FAILURE;
	}
	else
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}

	return Return_Invoke();
}

void CBehaviorTreeDecorator_AnimationPrevent::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void CBehaviorTreeDecorator_AnimationPrevent::DebugLog()
{
}
#endif

const _bool CBehaviorTreeDecorator_AnimationPrevent::Is_PlayingPreventAnimation() const
{
	for (auto& iAnimIdx : m_lstPrevent)
	{
		if (m_pAnimator->Is_Playing(iAnimIdx))
		{
			return true;
		}
	}

	return false;
}

shared_ptr<CBehaviorTreeDecorator_AnimationPrevent> CBehaviorTreeDecorator_AnimationPrevent::Create(shared_ptr<CBlackBoard> _pBlackBoard, initializer_list<_uint> _ilPrevent, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBehaviorTreeDecorator_AnimationPrevent> pInstance = make_private_shared(CBehaviorTreeDecorator_AnimationPrevent, _pNode, _ilPrevent);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBehaviorTreeDecorator_AnimationPrevent::Create", "Failed to Initialize");
	}

	return pInstance;
}
