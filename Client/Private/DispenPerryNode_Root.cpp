#include "ClientPCH.h"
#include "GameInstance.h"
#include "DispenPerry_BehaviorTree.h"
#include "Monster_DispenPerry.h"
#include "SkillContainer.h"
#include "Skill.h"
#ifdef _DEBUG
#include "WaterTank.h"
#include "TestAlphaBuffer.h"
#endif

#define TREE_DEBUG_WINDOW_WIDTH	500.f

HRESULT CDispenPerryNode_Root::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Root";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryNode_Root::Initialize", "Failed to __super::Initialize");
	}

	m_pBlackBoard->Add_Anything(TEXT("Reset"), &m_bResetTree);

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CDispenPerryNode_Root::Initialize", "Failed to Ready_Skills");
	}

	m_pOwner = m_pBlackBoard->Get_WeakSystem<CMonster_DispenPerry>(TEXT("Owner"));
	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CDispenPerrySelector_Main::Initialize", "Failed to Get_Anything: Owner");
	}

	m_pState = m_pOwner.lock()->Get_StatePtr();
	m_pBlackBoard->Add_Anything(TEXT("Owner:State"), m_pState);

#ifdef _DEBUG
	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
//	m_pWaterTank = m_pOwner.lock()->Get_WaterTank();
	m_pRenderer = CGameInstance::Get_Instance()->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);
#endif

	Add_Child(CDispenPerrySelector_Hit::Create(m_pBlackBoard));
	Add_Child(CDispenPerrySelector_Phase::Create(m_pBlackBoard));

	m_pDPPhase = m_pBlackBoard->Get_Anything<DP_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pDPPhase)
	{
		MSG_RETURN(E_FAIL, "CDispenPerryNode_Root::Initialize", "Failed to Get_Anything: Owner:Phase");
	}

	return S_OK;
}

#ifdef _DEBUG
void CDispenPerryNode_Root::Late_Tick(_float _fTimeDelta)
{
	_uint idx(0);
	_float3 vMonPos = m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
	_float3 vPlayer = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
	for (auto& pDebugRange : m_mapAlphaBuffer)
	{
		++idx;
		_float3 vPos = _float3(vMonPos.x, (vMonPos.y + vPlayer.y) * 0.5f + 0.01f * _float(idx), vMonPos.z);
		pDebugRange.second->Get_Component<CTransform>(COMPONENT::TRANSFORM)
			->Set_State(TRANSFORM::POSITION, vPos);
		pDebugRange.second->Late_Tick(_fTimeDelta);
	}
}

HRESULT CDispenPerryNode_Root::Render()
{
	return S_OK;
}
#endif

void CDispenPerryNode_Root::Activate()
{
	__super::Activate();
}

STATUS CDispenPerryNode_Root::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);
	
	if (DP_PHASE::PHASE0 != *m_pDPPhase)
	{
		m_pSkillContainer->Tick(_fTimeDelta);
	}

	m_eStatus = __super::Invoke(_fTimeDelta);

#ifndef _DEBUG
	_float fDefaultTimeScale = CGameInstance::Get_Instance()->Get_TimeScale(DEFAULT_FPS);

	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_4))
	{
		CGameInstance::Get_Instance()->Set_TimeScale(max(0.f, fDefaultTimeScale - 0.1f));
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(max(0.f, fDefaultTimeScale - 0.1f));
	}
	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_6))
	{
		CGameInstance::Get_Instance()->Set_TimeScale(fDefaultTimeScale + 0.1f);
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(fDefaultTimeScale + 0.1f);
	}
#endif

	return Return_Invoke();
}

void CDispenPerryNode_Root::Terminate()
{
	__super::Terminate();
}

#ifdef _DEBUG
void CDispenPerryNode_Root::Debug(any)
{
	DebugLog();
}
#if ACTIVATE_IMGUI
void CDispenPerryNode_Root::DebugLog()
{
	static _bool bTreeDebugFix(true);
	static _bool bTreeOpenNode(true);
	static _bool bTreeSkillInfo(false);

	if (CGameInstance::Get_Instance()->Key_Down(VK_RSHIFT)) bTreeDebugFix = !bTreeDebugFix;
	if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) && CGameInstance::Get_Instance()->Key_Down(VK_RETURN)) bTreeOpenNode = !bTreeOpenNode;
	if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) && CGameInstance::Get_Instance()->Key_Down('S')) bTreeSkillInfo = !bTreeSkillInfo;

	if (bTreeDebugFix)
	{
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - TREE_DEBUG_WINDOW_WIDTH, ImGui::GetTextLineHeightWithSpacing()));
		ImGui::SetNextWindowSize(ImVec2(TREE_DEBUG_WINDOW_WIDTH, ImGui::GetIO().DisplaySize.y - ImGui::GetTextLineHeightWithSpacing()));
		ImGui::Begin("DispenPerry BehaviorTree##Fix", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	}
	else
	{
		ImGui::Begin("DispenPerry BehaviorTree##Free");
	}

	ImGui::SeparatorText("Settings");

	ImGui::Checkbox("Fix Debug Window (RSHIFT)", &bTreeDebugFix);
	ImGui::Checkbox("Fix Tree Open (LSHIFT + RETURN)", &bTreeOpenNode);
	ImGui::Checkbox("Skill Information (LSHIFT + S)", &bTreeSkillInfo);

	ImGui::SeparatorText("Information");

	ImGui::Text("HP: %f/%f", m_pEntityDesc->fHP, m_pEntityDesc->fMaxHP);
	ImGui::Text("WaterTank HP: %f/%f", m_pOwner.lock()->Get_WaterTank()->Get_HP(), m_pOwner.lock()->Get_WaterTank()->Get_HP(true));
	ImGui::Text("Distance: %f", Function::Distance(Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM),
		m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"))));

	ImGui::SeparatorText("Skill");

	if (bTreeSkillInfo)
	{
		m_pSkillContainer->DebugLog();
	}
	else
	{
		ImGui::TextDisabled("Skill Information");
	}

	ImGui::SeparatorText("Behavior Tree Node Flow");

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

	CDebugSelector::Debug(bTreeOpenNode);
	
	ImGui::PopStyleColor();

	ImGui::End();
}
#endif
#endif

HRESULT CDispenPerryNode_Root::Ready_Skills()
{
	m_pSkillContainer = CSkillContainer::Create();

	m_pSkillContainer->Register_Skill(TEXT("Atk: FrontArmSwipe"),		1u, ATK_ARMSWIPE_COOLTIME);
	m_pSkillContainer->Register_Skill(TEXT("Atk: SpinningArmSwipe"),	1u, ATK_ARMSWIPE_COOLTIME);
	m_pSkillContainer->Register_Skill(TEXT("Atk: WaterShot"),			1u, ATK_WATERSHOT_COOLTIME);
	m_pSkillContainer->Register_Skill(TEXT("Atk: MultiWaterShot"),		1u, ATK_WATERSHOT_COOLTIME);
	m_pSkillContainer->Register_Skill(TEXT("Atk: LeapingBodySlam"),		1u, ATK_LEAP_COOLTIME);

#ifdef _DEBUG
	shared_ptr<CTestAlphaBuffer> pTAB;
	shared_ptr<CTexture> pTex = CTexture::Create(CGameInstance::Get_Instance()->Get_Device(), CGameInstance::Get_Instance()->Get_Context(), TEXT("Bin/Resources/Texture/Circle.dds"));
	MATERIALDESC tMtrlDesc;
	_float4x4 mUp = _float4x4(_float4(1.f, 0.f, 0.f, 0.f), _float4(0.f, 0.f, 1.f, 0.f), _float4(0.f, -1.f, 0.f, 0.f), _float4(0.f, 1.f, 0.f, 1.f));

	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_ARMSWIPE_RANGE_MIN, ATK_ARMSWIPE_RANGE_MIN, ATK_ARMSWIPE_RANGE_MIN) * 2.f);
	tMtrlDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Min: ArmSwipe"), pTAB);
	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_ARMSWIPE_RANGE_MAX, ATK_ARMSWIPE_RANGE_MAX, ATK_ARMSWIPE_RANGE_MAX) * 2.f);
	tMtrlDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Max: ArmSwipe"), pTAB);
	
	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_WATERSHOT_RANGE_MIN, ATK_WATERSHOT_RANGE_MIN, ATK_WATERSHOT_RANGE_MIN) * 2.f);
	tMtrlDesc.vDiffuse = _float4(0.f, 0.f, 1.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Min: WaterShot"), pTAB);
	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_WATERSHOT_RANGE_MAX, ATK_WATERSHOT_RANGE_MAX, ATK_WATERSHOT_RANGE_MAX) * 2.f);
	tMtrlDesc.vDiffuse = _float4(0.f, 0.f, 1.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Max: WaterShot"), pTAB);
	
	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_LEAP_RANGE_MIN, ATK_LEAP_RANGE_MIN, ATK_LEAP_RANGE_MIN) * 2.f);
	tMtrlDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Min: Leap"), pTAB);
	pTAB = CGameInstance::Get_Instance()->Clone_GameObject<CTestAlphaBuffer>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_TEST_ALPHA);
	pTAB->Set_Texture(pTex);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Matrix(mUp);
	pTAB->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(ATK_LEAP_RANGE_MAX, ATK_LEAP_RANGE_MAX, ATK_LEAP_RANGE_MAX) * 2.f);
	tMtrlDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	pTAB->Set_Material(tMtrlDesc);
	m_mapAlphaBuffer.emplace(TEXT("Max: Leap"), pTAB);
#endif

	m_pBlackBoard->Add_System(TEXT("Owner:SkillContainer"), m_pSkillContainer);
	
	return S_OK;
}

shared_ptr<CDispenPerryNode_Root> CDispenPerryNode_Root::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CDispenPerryNode_Root> pInstance = make_private_shared(CDispenPerryNode_Root);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDispenPerryNode_Root::Create", "Failed to Initialize");
	}

	return pInstance;
}
