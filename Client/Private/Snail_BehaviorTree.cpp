#include "ClientPCH.h"
#include "Snail_BehaviorTree.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "Monster_Snail.h"

HRESULT CSnail_BehaviorTree::Initialize(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc, CMonster::MonsterDesc* MonsterDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CSnail_BehaviorTree::Initialize", "Failed to CBehavior::Initialize");
	}

	m_pBlackBoard->Add_System(TEXT("Owner:Transform"),		_pOwner->Get_Component(COMPONENT::TRANSFORM));
	m_pBlackBoard->Add_System(TEXT("Owner:Model"),			_pOwner->Get_Component(COMPONENT::MODEL));
	//m_pBlackBoard->Add_System(TEXT("Owner:Physics"),		_pOwner->Get_Behavior(BEHAVIOR::PHYSICS));
	m_pBlackBoard->Add_System(TEXT("Owner:Animator"),		_pOwner->Get_Behavior(BEHAVIOR::ANIMATOR));

	m_pBlackBoard->Add_System(TEXT("Target:Transform"),		Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));

	m_pBlackBoard->Add_EntityDesc(TEXT("Owner:EntityDesc"),	_pEntityDesc);

	//shared_ptr<CSnail_RootNode> pRoot = CSnail_RootNode::Create(m_pBlackBoard);

	m_pBlackBoard->Add_Anything(TEXT("Owner:MonsterDesc"), MonsterDesc);

	m_pBlackBoard->Add_System(TEXT("Owner"),				dynamic_pointer_cast<CMonster_Snail>(_pOwner));
	//m_pBlackBoard->Add_Anything(TEXT("Owner:Root"),			dynamic_pointer_cast<CSnail_RootNode>(pRoot));

	Set_RootNode(CSnail_RootNode::Create(m_pBlackBoard));

	return S_OK;
}

shared_ptr<CSnail_BehaviorTree> CSnail_BehaviorTree::Create(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc, CMonster::MonsterDesc* MonsterDesc)
{
	shared_ptr<CSnail_BehaviorTree> pInstance = make_private_shared(CSnail_BehaviorTree);

	if (FAILED(pInstance->Initialize(_pOwner, _pEntityDesc, MonsterDesc)))
	{
		MSG_RETURN(nullptr, "CSnail_BehaviorTree::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CSnail_BehaviorTree::Clone(any)
{
	return make_private_shared_copy(CSnail_BehaviorTree, *this);
}
