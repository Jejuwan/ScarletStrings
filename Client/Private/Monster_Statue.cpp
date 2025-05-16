#include "ClientPCH.h"
#include "Monster_Statue.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Statue_FSM.h"
#include "Monster_Karen.h"
#pragma region UI
#include "UI_Manager.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"
#pragma endregion

#include "Part.h"
#include "Bone.h"

CMonster_Statue::TYPE CMonster_Statue::m_CurruntType = CMonster_Statue::MASK_WHITE;

CMonster_Statue::CMonster_Statue(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Statue::CMonster_Statue(const CMonster_Statue& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Statue::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_STATUE, g_aNull);

	return S_OK;
}

HRESULT CMonster_Statue::Initialize(any Any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Initialize", "Failed to CMonster::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Initialize", "Failed to CMonster::Ready_Behaviors");
	}

	STATUEDESC Desc = any_cast<STATUEDESC>(Any);
	m_MaskType = Desc.MaskType;
	if (FAILED(Ready_Parts()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Initialize", "Failed to CMonster::Ready_Parts");
	}

	m_eMonsterType = STATUE;
	m_tEntityDesc.fHP = 20.f;
	m_tEntityDesc.fMaxHP = 20.f;
	m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fMaxHP;
	
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));

	m_vControllerPivot = PxExtendedVec3(0.f, 0.f, 0.f);

	m_pTransform->Set_Scale(XMVectorSet(2.f, 2.f, 2.f, 0.f));
	m_pFSM = CStatue_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	//Fetch(Desc.InitPosition);
	//m_pModel->Hide_Mesh(2);

#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion
	return S_OK;
}

void CMonster_Statue::Tick(_float fTimeDelta)
{
	// Material Settings
	/*ImGui::Begin("Material Settings");
	if (ImGui::CollapsingHeader("Statue Material"))
	{
		_uint iNumMesh = m_pModel->Get_NumMeshes();
		for (_uint i = 0; i < iNumMesh; ++i)
		{
			string MeshName = m_pModel->Get_MeshName(i);
			if (ImGui::TreeNode(MeshName.c_str()))
			{
				SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
				_bool bDirtyFlag = false;
				if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
					bDirtyFlag = true;
				if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
					bDirtyFlag = true;

				if (bDirtyFlag)
				{
					m_pModel->Set_ShaderDesc(i, tShaderDesc);
				}

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Save"))
		{
			m_pModel->Export_ShaderMaterial();
		}
	}

	ImGui::End();*/

	if (m_MaskType == 0)
	{
		auto pGameInstance = CGameInstance::Get_Instance();

		_float3 vBossPos;
		

		shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);

		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				if (nullptr == dynamic_pointer_cast<CMonster_Karen>(_pObject))
					return true;

				m_pKaren = dynamic_pointer_cast<CMonster_Karen>(_pObject);
				return true;
			});

		if(nullptr!=m_pKaren.lock())
		{
			if (m_pKaren.lock()->Get_EntityDesc().fHP / m_pKaren.lock()->Get_EntityDesc().fMaxHP < 0.9f && !m_bExecuted[1])
			{
				m_bExecuted[1] = true;
				static_pointer_cast<CStatue_FSM>(m_pFSM)->StatueFalling();
			}
			if (m_pKaren.lock()->Get_EntityDesc().fHP / m_pKaren.lock()->Get_EntityDesc().fMaxHP < 0.8f && !m_bExecuted[2])
			{
				m_bExecuted[2] = true;
				static_pointer_cast<CStatue_FSM>(m_pFSM)->StatueFalling();
			}
			if (m_pKaren.lock()->Get_EntityDesc().fHP / m_pKaren.lock()->Get_EntityDesc().fMaxHP < 0.7f && !m_bExecuted[3])
			{
				m_bExecuted[3] = true;
				static_pointer_cast<CStatue_FSM>(m_pFSM)->StatueFalling();
			}
		}
	}
	if (m_bFetched)
	{
		__super::Tick(fTimeDelta);
		m_pBigActorDesc->bActive = m_bLockOnAble;

		if (m_pNormalActorDesc->bHit)
		{
			m_bExecuteGrainEffect = true;
			m_bDecreaseGrainFactor = false;
		}

		if (CGameInstance::Get_Instance()->Key_Down('M'))
		{
			m_bExecuteGrainEffect = true;
			m_bDecreaseGrainFactor = false;
		}

#pragma region UId
		m_pUI_Hp->Tick(fTimeDelta);
#pragma endregion
	}
	else
	{
		m_pFSM->Tick(fTimeDelta);
		m_pAnimator->Tick(fTimeDelta);
	}
	
	m_bLockOnAble = m_MaskType == m_CurruntType;

	for(auto pPart : m_vecPart)
		pPart->Tick(fTimeDelta);
	
	
	m_fSeedAccTime += fTimeDelta;
	if (m_fSeedAccTime >= m_fSeedInterval)
	{
		m_fSeedAccTime = 0.f;
		m_fGlitchSeed += fTimeDelta;
	}

	if (m_bExecuteGrainEffect)
	{
		if (false == m_bDecreaseGrainFactor)
		{
			m_vGrainFactor.y += fTimeDelta * 0.5f;
			m_vGrainFactor.z += fTimeDelta;
			if (m_vGrainFactor.z >= m_fMaxGrainFactorThreshold)
				m_bDecreaseGrainFactor = true;
		}
		else
		{
			m_vGrainFactor.z -= fTimeDelta;
			if (m_vGrainFactor.z < 0.f)
			{
				m_vGrainFactor.y = 0.f;
				m_vGrainFactor.z = 0.f;

				m_bExecuteGrainEffect = false;
				m_bDecreaseGrainFactor = false;
			}
		}
	}
}

void CMonster_Statue::Late_Tick(_float fTimeDelta)
{
	if (m_bFetched)
	{
		__super::Late_Tick(fTimeDelta);

		if (m_CurruntType == m_MaskType)
			m_vecPart[1]->Late_Tick(fTimeDelta);
		else
			m_vecPart[0]->Late_Tick(fTimeDelta);
		if (m_MaskType == MASK_WHITE)
			m_vecPart[2]->Late_Tick(fTimeDelta);

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
	else
	{
		m_pFSM->Late_Tick(fTimeDelta);

		if (m_MaskType == MASK_WHITE)
		{
			m_vecPart[2]->Late_Tick(fTimeDelta);
		}
		else
		{
			m_vecPart[0]->Late_Tick(fTimeDelta);
		}

		m_vecPart[1]->Late_Tick(fTimeDelta);
	}

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SCREEN_EFFECT);
}

HRESULT CMonster_Statue::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to Bind_LightDirection");
	}

	//if (m_bExecuteGrainEffect)
	//{
	//	
	//}

	if (FAILED(m_pShader->Bind_RawValue("g_vGrainColor", &m_vGrainFactor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to Bind Grain Factor");
	}

	if (FAILED(__super::Render(20)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Statue::Render_Screen_Effect()
{
	if (FAILED(m_pNoiseTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_vLineNoiseTexture")))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render_Screen_Effect", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Float("g_fSeed", m_fGlitchSeed)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render_Screen_Effect", "Failed to GlitchSeed");
	}
	
	if (FAILED(__super::Render(19)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Statue::Render_ShadowDepth(shared_ptr<class CLight>)
{
	return S_OK;
}

void CMonster_Statue::Update_Motion(_float fTimeDelta)
{
	m_vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
}

void CMonster_Statue::BlackMaskPivotChance()
{
	m_vecPart[1]->BlackMaskPivotChance();
}

HRESULT CMonster_Statue::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_Statue::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CMonster_Statue::Fetch(any Any)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,0.f,0.f };
	cDesc.height = 1.5f;
	cDesc.radius = 1.0f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::MONSTER;
	m_pController->getActor()->userData = &m_ActorDesc;

	m_vControllerPivot;

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleRadius = 0.2f;
	desc.fCapsuleHeight = 0.5f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	m_pAttackCollider->Initialize(desc, actorDesc);

	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	if(m_MaskType == TYPE::MASK_WHITE)
		desc.fRadius = 10.f;
	else
		desc.fRadius = 10.f;

	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;

	m_pAttackActorDesc->bActive = false;
	m_strAttackBoneName = "Reference";
	m_strCenterBoneName = "Reference";
	m_strWeakBoneName = "Hood";

	m_bFetched = true;

	if (FAILED(__super::Fetch(Any)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Fetch", "Failed to CMonster::Fetch");
	}

	return S_OK;
}

shared_ptr<class CPart> CMonster_Statue::Get_Part(_uint iPartIndex)
{
	return m_vecPart[iPartIndex];
}

HRESULT CMonster_Statue::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pNoiseTexture = pGameInstance->Clone_Component<CTexture>(SCENE::MINDROOM, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_014);
	if (nullptr == m_pNoiseTexture)
	{
		MSG_RETURN(E_FAIL, "CMonster_Statue::Ready_Components", "Failed to Clone_Component: NoiseTexture");
	}

	return S_OK;
}

HRESULT CMonster_Statue::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}
	//m_pTree = CStatue_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc,&//m_tMonsterDesc);
	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, m_pTree);

	return S_OK;
}

HRESULT CMonster_Statue::Ready_Parts()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	CPart::PARTDESC Desc;
	Desc.PivotMatrix = m_pModel->Get_Pivot();
	Desc.pParentMatrix = m_pTransform->Get_WorldMatrixPtr();
	Desc.pWeakBoneMatrix = m_pModel->Get_Bone("RightWeapon")->Get_CombinedTransformationPointer();
	Desc.PlusMatrix = XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(180.f));
	Desc.bGlitch = true;

	shared_ptr<CPart> pPart = dynamic_pointer_cast<CPart>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_FLOWER, Desc));
	if (nullptr == pPart)
	{
		MSG_RETURN(E_FAIL, "Monster_Statue::Ready_Parts", "Failed to Statue::Ready_Parts");
	}
	m_vecPart.push_back(pPart);

	switch (m_MaskType)
	{
	case Client::CMonster_Statue::MASK_WHITE:
		pPart = dynamic_pointer_cast<CPart>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK1, Desc));
		if (nullptr == pPart)
		{
			MSG_RETURN(E_FAIL, "Monster_Statue::Ready_Parts", "Failed to Statue::Ready_Parts");
		}
		m_vecPart.push_back(pPart);
		
		Desc.pWeakBoneMatrix = m_pModel->Get_Bone("LeftWeapon")->Get_CombinedTransformationPointer();
		pPart = dynamic_pointer_cast<CPart>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_FAN, Desc));
		if (nullptr == pPart)
		{
			MSG_RETURN(E_FAIL, "Monster_Statue::Ready_Parts", "Failed to Statue::Ready_Parts");
		}
		m_vecPart.push_back(pPart);
		break;
	case Client::CMonster_Statue::MASK_BLACK:
		//Desc.PlusMatrix = XMMatrixRotationX(XMConvertToRadians(-90.f));
		pPart = dynamic_pointer_cast<CPart>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK2, Desc));
		if (nullptr == pPart)
		{
			MSG_RETURN(E_FAIL, "Monster_Statue::Ready_Parts", "Failed to Statue::Ready_Parts");
		}
		m_vecPart.push_back(pPart);
		break;
	case Client::CMonster_Statue::MASK_RED:
		pPart = dynamic_pointer_cast<CPart>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MONSTER_PART_STATUE_MASK3, Desc));
		if (nullptr == pPart)
		{
			MSG_RETURN(E_FAIL, "Monster_Statue::Ready_Parts", "Failed to Statue::Ready_Parts");
		}
		m_vecPart.push_back(pPart);
		break;
	default:
		break;
	}

	
	

	return S_OK;
}

shared_ptr<CMonster_Statue> CMonster_Statue::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Statue> pInstance = make_private_shared(CMonster_Statue, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Statue::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Statue::Clone(any pArg)
{
	shared_ptr<CMonster_Statue> pInstance = make_private_shared_copy(CMonster_Statue, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Statue::Clone", "Failed to Initialize");
	}

	return pInstance;
}

