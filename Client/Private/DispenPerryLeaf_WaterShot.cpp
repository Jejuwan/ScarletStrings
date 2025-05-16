#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"
#include "Effect_Manager.h"

HRESULT CDispenPerryLeaf_WaterShot::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "WaterShot";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to __super::Initialize");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to Get_System: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to Get_System: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryLeaf_WaterShot::Initialize", "Failed to Target:Transform");
	}

	return S_OK;
}

void CDispenPerryLeaf_WaterShot::Activate()
{
	__super::Activate();

	m_bShoot = false;
	m_pTransform->LookAt_Interpolation(m_pTargetTransform, true, false, 2.f, 1.f);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pAnimator->Play_Animation(ANIMATION::DISPEN_PERRY::ATK_WATERBALL, 3.f, false, g_fDefaultInterpolationDuration, false);
	m_pOwner.lock()->InitRootMotion();
}

STATUS CDispenPerryLeaf_WaterShot::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pOwner.lock()->Update_Motion(_fTimeDelta);
	m_pOwner.lock()->RootMotion(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::DISPEN_PERRY::ATK_WATERBALL))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::DISPEN_PERRY::ATK_WATERBALL))
		{
			m_eStatus = STATUS::SUCCESS;
		}
		else
		{
			if (!m_bShoot)
			{
				if (m_pAnimator->Get_TrackPosition(ANIMATION::DISPEN_PERRY::ATK_WATERBALL) >= 80.f)
				{
					_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
					_float3 TargetPos = m_pTargetTransform->Get_State(TRANSFORM::POSITION);
					_float3 Look = m_pTransform->Get_State(TRANSFORM::LOOK);
					_float3 Distance = TargetPos - Pos;

					Distance *= 0.1f;
					Distance.y += 8.f;

					_float3 Amount = Pos + Look + Distance;
					CEffect_Manager::Get_Instance()->Fetch(VFX::DISPENPERRY_SHOOTING0, Amount);
					CEffect_Manager::Get_Instance()->Fetch(VFX::DISPENPERRY_SHOOTING1, Amount);

					m_bShoot = true;
					m_pOwner.lock()->WaterShot();
				}
			}

			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CDispenPerryLeaf_WaterShot::Terminate()
{
	__super::Terminate();

	m_pOwner.lock()->SetWeaponeAllActiveOff();
	m_pOwner.lock()->MotionReset();
}

#if ACTIVATE_IMGUI
void CDispenPerryLeaf_WaterShot::DebugLog()
{
	ImGui::Text("Time Acc: %f", m_fTimeAcc);
}
#endif

shared_ptr<CDispenPerryLeaf_WaterShot> CDispenPerryLeaf_WaterShot::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryLeaf_WaterShot> pInstance = make_private_shared(CDispenPerryLeaf_WaterShot);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryLeaf_WaterShot::Create", "Failed to Initialize");
	}

	return pInstance;
}
