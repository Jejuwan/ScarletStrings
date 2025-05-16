#include "ClientPCH.h"
#include "DispenPerry_BehaviorTree.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "Monster_DispenPerry.h"
#include "WaterTank.h"

HRESULT CDispenPerry_BehaviorTree::Initialize(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CBrother_BehaviorTree::Initialize", "__super::Initialize Failed");
	}

	m_pBlackBoard->Add_WeakSystem(TEXT("Owner"),			dynamic_pointer_cast<CMonster_DispenPerry>(_pOwner));
	m_pBlackBoard->Add_EntityDesc(TEXT("Owner:EntityDesc"),	_pEntityDesc);

	m_pBlackBoard->Add_System(TEXT("Owner:Transform"),		_pOwner->Get_Component(COMPONENT::TRANSFORM));
	m_pBlackBoard->Add_System(TEXT("Owner:Model"),			_pOwner->Get_Component(COMPONENT::MODEL));
	m_pBlackBoard->Add_System(TEXT("Owner:Physics"),		_pOwner->Get_Behavior(BEHAVIOR::PHYSICS));
	m_pBlackBoard->Add_System(TEXT("Owner:Animator"),		_pOwner->Get_Behavior(BEHAVIOR::ANIMATOR));

	m_pBlackBoard->Add_System(TEXT("Target:Transform"),		Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));

	Set_RootNode(CDispenPerryNode_Root::Create(m_pBlackBoard));

	return S_OK;
} 

shared_ptr<CDispenPerry_BehaviorTree> CDispenPerry_BehaviorTree::Create(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CDispenPerry_BehaviorTree> pInstance = make_private_shared(CDispenPerry_BehaviorTree);

	if (FAILED(pInstance->Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CDispenPerry_BehaviorTree::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CDispenPerry_BehaviorTree::Clone(any)
{
    return make_private_shared_copy(CDispenPerry_BehaviorTree, *this);
}
