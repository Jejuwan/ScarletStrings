#include "ClientPCH.h"
#include "Monster_Fly.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
//#include "Fly_BehaviorTree.h"
#include "Fly_FSM.h"
#include "Bone.h"
#include "UI_Indicator_Monster_NoticeMark.h"
#include "UI_Indicator_Monster_Hp.h"
#include "Camera_Player.h"


CMonster_Fly::CMonster_Fly(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CMonster_Fly::CMonster_Fly(const CMonster_Fly& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CMonster_Fly::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MONSTER_FLY, g_aNull);

	return S_OK;
}

HRESULT CMonster_Fly::Initialize(any Any)
{
	m_vControllerPivot = PxExtendedVec3(0.f, 1.5f, 0.f);
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Initialize", "Failed to CGameObject::Initialize");
	}
	if (FAILED(Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Initialize", "Failed to ReadyBehaviors");
	}
	m_eMonsterType = FLY;

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Current_Scene() == SCENE::TEST)
	{
		m_tEntityDesc.fMaxHP = m_tEntityDesc.fHP = 9;
		m_tEntityDesc.fKnuckBackGauge = 999999;
	}
	else
		m_tEntityDesc.fKnuckBackGauge = m_tEntityDesc.fHP = m_tEntityDesc.fMaxHP = 20;



	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,1000.f,0.f };
	cDesc.height = 0.0001f;
	cDesc.radius = 1.f;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.0002f;
	cDesc.reportCallback = NULL;

	if (pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		_float4 InitPos = any_cast<_float4>(Any);
		InitPos.y += 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, InitPos);
		m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(InitPos.w));
		InitPos.w = 1.f;
		cDesc.position = { InitPos.x,InitPos.y,InitPos.z };
	}

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::FLY;
	m_pController->getActor()->userData = &m_ActorDesc;
	
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));
	m_pModel->SetRootBoneAddIndex(m_pModel->Get_BoneIndex("Hips"));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION1, std::bind(&CMonster::SetWeaponeActiveOn, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION2, std::bind(&CMonster::SetWeaponeActiveOff, this));
	m_pModel->Bind_Event(EVENTTYPE::CUSTOMFUNCTION3, std::bind(&CMonster::ShootBulletToPlayer, this));

	m_pFSM = CFly_FSM::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 1.3f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10.f;
	actorDesc.stAttackDesc.iType = (_uint)CMonster::FLY;
	m_pAttackCollider->Initialize(desc, actorDesc);


	m_pBigAttackCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;

	actorDesc.strTag = ACTORTAG::MONSTER_BIGATTACK;
	actorDesc.stAttackDesc.iDamage = 0.f;
	m_pBigAttackCollider->Initialize(desc, actorDesc);


	m_pBodyCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 1.3f;
	
	ACTORDESC bodyActorDesc;
	bodyActorDesc.strTag = ACTORTAG::MONSTER_BIGBODY;
	m_pBodyCollider->Initialize(desc, bodyActorDesc);

	m_pDodgeCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 3.f;
	
	bodyActorDesc.strTag = ACTORTAG::MONSTER_DODGESENCE;
	m_pDodgeCollider->Initialize(desc, bodyActorDesc);

	m_pNormalActorDesc = (ACTORDESC*)m_pController->getActor()->userData;
	m_pBigActorDesc = (ACTORDESC*)m_pBodyCollider->GetActor()->userData;
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;
	m_pDodgeActorDesc = (ACTORDESC*)m_pDodgeCollider->GetActor()->userData;

	m_pBodyCollider->SetActive(true);
	m_pAttackActorDesc->bActive = false;

	m_strAttackBoneName = "Reference";
	m_strWeakBoneName = "Reference";
	m_pProjectile = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PROJECTILE_WATERDROP);
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER_PROJECTILE);
	pLayer->Add(m_pProjectile);
#pragma region UI
	m_pUI_Hp = dynamic_pointer_cast<CUI_Indicator_Monster_Hp>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_MONSTER_HP));
#pragma endregion

	return S_OK;
}

void CMonster_Fly::Tick(_float fTimeDelta)
{
	_matrix BoneMatrix = m_pModel->Get_Bone(m_strCenterBoneName.c_str())->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();
	if (m_pNormalActorDesc->bCapture = m_pDodgeActorDesc->bCapture)
		int test = 0;
	if (m_pDodgeCollider)
	{
		PxTransform p;
		p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
		_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
		p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
		m_pDodgeCollider->GetActor()->setGlobalPose(p);
	}

	__super::Tick(fTimeDelta);

	if (m_pNormalActorDesc->bCapture)
	{
		m_pNormalActorDesc->bCapture = false;
		m_pDodgeActorDesc->bCapture = false;
	}

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Fly Material"))
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

void CMonster_Fly::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::BLOOM);
	//Add_RenderObject(RENDER_GROUP::OUTLINE);

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

HRESULT CMonster_Fly::Render()
{
	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CMonster_FlowerChest::Render", "Failed to Bind_LightDirection");
	}

	if (FAILED(__super::Render(16)))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMonster_Fly::Render_OutLine()
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
		MSG_RETURN(E_FAIL, "CMonster_Fly::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

void CMonster_Fly::Update_Motion(_float fTimeDelta)
{
	m_vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
}

void CMonster_Fly::RootMotion(_float fTimeDelta, _bool HeightFix)
{
	__super::RootMotion(fTimeDelta); 
	if (HeightFix) {
		const PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
		PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
		PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here


		PxExtendedVec3 vPos = m_pController->getPosition();
		
		bool hasHit = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->raycast(
			PxVec3(vPos.x, vPos.y, vPos.z),
			PxVec3(0, -1, 0),
			300.f,
			buf,
			PxHitFlag::eMESH_ANY,
			PxQueryFilterData(PxQueryFlag::Enum::eSTATIC));

		_float fMinDist = 10.f;

		if (hasHit) {
			PxU32 nbHits = buf.nbTouches;
			for (PxU32 i = 0; i < nbHits; ++i) {
				const PxRaycastHit& touch = buf.touches[i];
				ACTORDESC* pActorDesc = (ACTORDESC*)touch.actor->userData;
				if (nullptr == pActorDesc)
				{
					PxF32 fDist = touch.distance;
					if (fMinDist > fDist)
						fMinDist = fDist;
				}
			}
		}
		if(fMinDist <= 4.f)
			m_pController->move(PxVec3(0.f, 1.f, 0.f) * fTimeDelta, 0.0f, fTimeDelta, m_ControllerFilters);
		else 
			m_pController->move(PxVec3(0.f, -1.f, 0.f) * fTimeDelta, 0.0f, fTimeDelta, m_ControllerFilters);
	}
}

void CMonster_Fly::RootMotionRotate(_float fTimeDelta, _bool HeightFix)
{
	_vector vRot = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector v = XMQuaternionRotationMatrix(XMMatrixRotationX(XMConvertToRadians(90.f)) * m_pTransform->Get_WorldMatrix());
	_matrix m = XMMatrixRotationQuaternion(v);
	_float4 vUpdatePos;
	vUpdatePos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//cout << vUpdatePos.x<< '\t' << vUpdatePos.y << '\t' << vUpdatePos.z << endl;
	_vector MoveDir = XMVector3TransformNormal((m_pModel->Get_RootPos() - m_vPreRootPos), m);
	//cout << XMVectorGetX(m_pModel->Get_RootPos()) << '\t' << XMVectorGetY(m_pModel->Get_RootPos()) << '\t' << XMVectorGetZ(m_pModel->Get_RootPos()) << endl;
	//cout << XMVectorGetX(MoveDir) << '\t' << XMVectorGetY(MoveDir) << '\t' << XMVectorGetZ(MoveDir) << endl;
	

	//CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, shared_from_gameobject());
	vUpdatePos += MoveDir;

	vUpdatePos.w = 1.f;

	m_pTransform->Set_State(TRANSFORM::POSITION, vUpdatePos);

	m_vPreRootPos = m_pModel->Get_RootPos();
}

void CMonster_Fly::InitMovePos(_float fTimeDelta)
{
	_float3 vDisp;
	XMStoreFloat3(&vDisp, m_pTransform->Get_State(TRANSFORM::POSITION) - m_vPrePos);
	m_CollisionFlag = m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, PxControllerFilters());0.f;

	PxExtendedVec3 vControllerPos = m_pController->getFootPosition() + m_vControllerPivot;

	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet((float)vControllerPos.x, (float)vControllerPos.y, (float)vControllerPos.z, 1.f));
}

void CMonster_Fly::MoveRootPos()
{
	m_vPreRootPos = _float4(0, 0, 0, 1);
}

HRESULT CMonster_Fly::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}


HRESULT CMonster_Fly::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster_Fly::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	//m_umapBehavior.emplace(BEHAVIOR::CUSTOM, CFly_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc, &//m_tMonsterDesc));

	return S_OK;
}

shared_ptr<CMonster_Fly> CMonster_Fly::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMonster_Fly> pInstance = make_private_shared(CMonster_Fly, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMonster_Fly::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_Fly::Clone(any pArg)
{
	shared_ptr<CMonster_Fly> pInstance = make_private_shared_copy(CMonster_Fly, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_Fly::Clone", "Failed to Initialize");
	}

	return pInstance;
}
