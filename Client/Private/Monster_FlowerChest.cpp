#include "ClientPCH.h"
#include "Monster_FlowerChest.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "FlowerChest_FSM.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Bone.h"

CMonster_FlowerChest::CMonster_FlowerChest(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_FlowerChest::CMonster_FlowerChest(const CMonster_FlowerChest& _rhs)
	: CMonster(_rhs)
{

}

HRESULT CMonster_FlowerChest::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_FLOWERCHEST, g_aNull);

	return S_OK;
}

HRESULT CMonster_FlowerChest::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Initialize", "Failed to CMonster::Ready_Behaviors");
	}
	m_eMonsterType = FLOWERCHEST;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	
	if (pGameInstance->Current_Scene() == SCENE::TEST)
	{
		m_tEntityDesc.fHP = m_tEntityDesc.fMaxHP = 9;
		m_tEntityDesc.fKnuckBackGauge = 9999999;
	}
	else
	{
		m_tEntityDesc.fHP = 200000.f;
		m_tEntityDesc.fMaxHP = 200000.f;
		m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fMaxHP;
	}
	

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f};
	cDesc.height = 2.f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	if(pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		_float4 InitPos = any_cast<_float4>(Any);
		InitPos.y += 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
		m_pTransform->Rotate(XMVectorSet(0.f,  1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
		InitPos.w = 1.f;
		cDesc.position = { InitPos.x,InitPos.y,InitPos.z };

		m_tEntityDesc.fHP = 200000.f;
		m_tEntityDesc.fMaxHP = 20000.f; 
		m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fMaxHP;
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;
	
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	//m_pModel->SetRootBoneAddIndex(m_pModel->Get_BoneIndex("Hips"));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	m_pFSM = CFlowerChest_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 0.2f;
	desc.fCapsuleHeight = 0.5f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.iType = (_uint)CMonster::FLOWERCHEST;
	m_pAttackCollider->Initialize(desc,actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 0.7f;
	desc.fCapsuleHeight = 0.9f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);


	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleHeight = 1.f;
	desc.fCapsuleRadius = 1.2f;
	
	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc,bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "Weapon_3";
	
#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	return S_OK;
}

void CMonster_FlowerChest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// Material Settings
	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("FlowerChest Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();

#pragma region UI
	m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
}

void CMonster_FlowerChest::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);
//	Add_RenderObject(RENDER_GROUP::BLOOM);

#pragma region UI
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
	m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

	m_pUI_Hp->Set_MonsterHp(m_tEntityDesc.fHP, m_tEntityDesc.fMaxHP);
	m_pUI_Hp->Set_MonsterNameTag(Get_TypeName());
	m_pUI_Hp->Late_Tick(fTimeDelta);
	if (m_tEntityDesc.fHP < m_tEntityDesc.fMaxHP)
	{
		m_pUI_Hp->Set_IndicatorPosition(m_pCamera_Player->Get_State(TRANSFORM::POSITION), this->m_pTransform->Get_WorldMatrix());
	}
#pragma endregion
}

HRESULT CMonster_FlowerChest::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(16)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_FlowerChest::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(CGameObject::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CMonster_FlowerChest::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

HRESULT CMonster_FlowerChest::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	//m_pTree = CFlowerChest_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc,&//m_tMonsterDesc);
	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, m_pTree);

	return S_OK;
}

shared_ptr<CMonster_FlowerChest> CMonster_FlowerChest::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_FlowerChest> pInstance = make_private_shared(CMonster_FlowerChest, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_FlowerChest::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_FlowerChest::Clone(any pArg)
{
	shared_ptr<CMonster_FlowerChest> pInstance = make_private_shared_copy(CMonster_FlowerChest, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_FlowerChest::Clone", "Failed to Initialize");
	}

	return pInstance;
}
