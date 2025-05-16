#pragma once
#include "Client_Define.h"
#include "BehaviorTree.h"
#pragma region Node
#include "DispenPerryNode_Root.h"
#pragma endregion
#pragma region Leaf
#include "BehaviorTreeLeaf_Animation.h"
#include "DispenPerryLeaf_Damage.h"
#include "DispenPerryLeaf_Weak.h"
#include "DispenPerryLeaf_WeakDown.h"
#include "DispenPerryLeaf_Chance.h"
#include "DispenPerryLeaf_Wait.h"
#include "DispenPerryLeaf_Turn.h"
#include "DispenPerryLeaf_Walk.h"
#include "DispenPerryLeaf_FrontArmSwipe.h"
#include "DispenPerryLeaf_SpinningArmSwipe.h"
#include "DispenPerryLeaf_WaterShot.h"
#include "DispenPerryLeaf_MultiWaterShot.h"
#pragma endregion
#pragma region Decorator
#include "BehaviorTreeDecorator_AnimationPrevent.h"
#include "BehaviorTreeDecorator_AnimationEnter.h"
#include "BehaviorTreeDecorator_AnimationExit.h"
#include "BehaviorTreeDecorator_Range.h"
#include "BehaviorTreeDecorator_Repeat.h"
#include "BehaviorTreeDecorator_Skill.h"
#include "BehaviorTreeDecorator_Health.h"
#include "BehaviorTreeDecorator_Random.h"
#include "BehaviorTreeDecorator_RunOnce.h"
#include "BehaviorTreeDecorator_RelativeAngle.h"
#pragma endregion
#pragma region Composite
#pragma region Parallel
#pragma endregion
#pragma region Sequence
#include "DispenPerrySequence_LeapingBodySlam.h"
#pragma endregion
#pragma region Selector
#include "DispenPerrySelector_Phase.h"
#include "DispenPerrySelector_Phase01.h"
#include "DispenPerrySelector_Phase02.h"
#include "DispenPerrySelector_LeapingBodySlam.h"
#include "DispenPerrySelector_Hit.h"
#include "DispenPerrySelector_Weak.h"
#include "DispenPerrySelector_SetField.h"
#pragma endregion
#pragma endregion

BEGIN(Client)

class CDispenPerry_BehaviorTree final : public CBehaviorTree
{
private:
	explicit CDispenPerry_BehaviorTree() DEFAULT;
	virtual ~CDispenPerry_BehaviorTree() DEFAULT;

private:
	virtual HRESULT									Initialize(shared_ptr<class CGameObject>, const ENTITYDESC*) override;

public:
	static shared_ptr<CDispenPerry_BehaviorTree>	Create(shared_ptr<class CGameObject>, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>					Clone(any = g_aNull) override;
};

END
