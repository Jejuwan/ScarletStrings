#include "ClientPCH.h"
#include "Map_Suoh_Bus.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"

#include "Player.h"
#include "Psychokinesis.h"
#include <regex>

CMap_Suoh_Bus::CMap_Suoh_Bus(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Special_Dynamic(_pDevice, _pContext)
{
}

CMap_Suoh_Bus::CMap_Suoh_Bus(const CMap_Suoh_Bus& _rhs)
	:CMap_Special_Dynamic(_rhs)
{
}

HRESULT CMap_Suoh_Bus::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER) | BIT(COMPONENT::MODEL);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);
	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BUSA, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Suoh_Bus::Initialize(any pArg)
{
	if (FAILED(CMap_Dynamic::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	Add_ModelCom();

	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple = 
		any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

	if (std::get<0>(tObjTuple) == SCENE::MAPTOOL)
	{
		m_strName = std::get<1>(tObjTuple);
		m_bSceneMapTool = true;

		if (CMap_ToolManager::Get_Instance()->Get_Load())
		{
			m_strNameNum = std::get<6>(tObjTuple);

			CMap_ToolManager::Get_Instance()->NameNum_List(m_strNameNum);

			m_Scale = std::get<2>(tObjTuple);
			m_Rotation = std::get<3>(tObjTuple);
			m_Translation = std::get<4>(tObjTuple);
			m_Mat = std::get<5>(tObjTuple);
			m_pTransform->Set_Matrix(m_Mat);
		   
			CMap_ToolManager::OBJECTDESC objDesc;

			objDesc.Scale = std::get<2>(tObjTuple);
			objDesc.Rotation = std::get<3>(tObjTuple);
			objDesc.Translation = std::get<4>(tObjTuple);
			objDesc.matrix = std::get<5>(tObjTuple);
			objDesc.strName = std::get<6>(tObjTuple);
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

		}
	}
	if (std::get<0>(tObjTuple) == SCENE::SUOH_BATTLE)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		m_Rotation = std::get<3>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_pTransform->Set_Matrix(m_Mat);
		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);
	}

	// Hide Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 15;
		m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
			{
				if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
				}
			};
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
	}
;
	
	_float4x4 mat = m_pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	PxTransform pose = PxTransform(PxIdentity);
	pose.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pKinematicActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
	m_pKinematicShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(m_vExtents.x, m_vExtents.y, m_vExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	m_pKinematicShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));

	m_pKinematicActor->attachShape(*m_pKinematicShape);
	//SetPhysX(false);
	//m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	m_pKinematicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	m_pKinematicActor->userData = &m_ActorDesc;

	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pKinematicActor);
	
	m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelDesc.iPassIndex = 15;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}
		};
	m_pCollider = make_shared<CPhysXCollider>();
	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::INTERACTIVE_BUS;
	actorDesc.bActive = true;
	actorDesc.stAttackDesc.iDamage = 100.f;

	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::BOX;
	desc.vHalfExtents = m_vExtents * 1.1f;
	desc.bSetLocalPose = true;
	desc.vLocalPose = m_vLocalPose;
	m_pCollider->Initialize(desc, actorDesc);
	

	SetPhysX(true);

	m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelShadowDesc.iPassIndex = 2;

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_Bus::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif

	if (m_bDead)
		return;
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	__super::Tick(fTimeDelta);

	auto pGameInstance = CGameInstance::Get_Instance();

	if (m_bThrowStart)
	{
		m_fThrowReadyTime += fTimeDelta;
		//m_bDissolve = true;
	}


	//_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//cout << vPos.x << '\t' << vPos.y << '\t' << vPos.z << endl;

	if (m_bActivePhysX)
	{
		if (nullptr != m_pKinematicActor)
		{
			PxMat44 worldMat(m_pKinematicActor->getGlobalPose());
			m_pKinematicActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());


			PxTransform p;
			p.p = PxVec3(vPos.x, vPos.y, vPos.z);
			p.q = PxQuat(q.x, q.y, q.z, q.w);
			if (m_pKinematicActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
				m_pKinematicActor->setKinematicTarget(p);
			else
			{
				Set_Gravity(true);
				_float4 q = { m_pKinematicActor->getGlobalPose().q.x,m_pKinematicActor->getGlobalPose().q.y,m_pKinematicActor->getGlobalPose().q.z,m_pKinematicActor->getGlobalPose().q.w };
				_float4 p = { m_pKinematicActor->getGlobalPose().p.x,m_pKinematicActor->getGlobalPose().p.y,m_pKinematicActor->getGlobalPose().p.z,1.f };
				_float4x4 rotmat = XMMatrixRotationQuaternion(q);

				_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
				_float3 OriginScale = m_pTransform->Get_Scale();
				m_pTransform->Set_Matrix(rotmat);
				m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
				m_pTransform->Set_Scale(OriginScale);
			}
			m_pCollider->GetActor()->setGlobalPose(p);
		}
	}
	else
	{
		if (nullptr != m_pKinematicActor)
		{
			PxMat44 worldMat(m_pKinematicActor->getGlobalPose());

			_float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

			PxTransform px;
			px.p = PxVec3(vPos.x, vPos.y, vPos.z);
			px.q = PxQuat(q.x, q.y, q.z, q.w);

			m_pKinematicActor->setGlobalPose(px);
			m_pCollider->GetActor()->setGlobalPose(px);
		}
	}

	m_pCollider->GetActorDesc().bThrow = true;
	CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	pPlayer->Get_Kinesis()->Set_RecentThrowObject(static_pointer_cast<CInteractiveObject>(shared_from_this()));
}

void CMap_Suoh_Bus::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Suoh_Bus", &bRender);
			ImGui::End();
		}
	if (!bRender) return;
#endif

	if (m_bDead)
		return;

	if (m_bDissolve || m_bAura)
	{
		m_fAuraTimeFactor += fTimeDelta * 0.5f;
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
	else
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	}
}

HRESULT CMap_Suoh_Bus::Render()
{

	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
	}

	if (m_bAura)
	{
		m_iPassIndex = 18;

		if (FAILED(m_pAuraTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "texAura")))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_ShaderResourceView:Aura");
		}

		if (FAILED(m_pShader->Bind_RawValue("vAuraColor", &m_vAuraColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Color");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraBlendFactor", m_fAuraBlendFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Blend Factor");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraTimeFactor", m_fAuraTimeFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Time Factor");
		}
	}

	if (m_bDissolve)
	{
		m_iPassIndex = 16;

		if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
		{
			return E_FAIL;
		}

		if (FAILED(m_pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
		{
			return E_FAIL;
		}
	}

	if (FAILED(CGameObject::Render(m_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}
	
	if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Bus::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(CGameObject::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Suoh_Bus::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Suoh_Bus::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Suoh_Bus::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_Bus::Ready_Components()
{
	if (FAILED(CMap_Dynamic::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pDissolveTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002));
	if (nullptr == m_pDissolveTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

void CMap_Suoh_Bus::Using_Map_Tool()
{
	m_strSelectedName = CMap_ToolManager::Get_Instance()->Get_SelectName();
	if (m_strName == m_strSelectedName)
	{
		/* Scale, Radian, Translation, matrix */
		m_tuplePivot = CMap_ToolManager::Get_Instance()->Get_TuplePivot(); 

		m_strObjName = std::get<0>(m_tuplePivot);
		m_Scale = std::get<1>(m_tuplePivot);
		m_Rotation = std::get<2>(m_tuplePivot);
		m_Translation = std::get<3>(m_tuplePivot);
		m_Mat = std::get<4>(m_tuplePivot);
		m_pTransform->Set_Matrix(m_Mat);

		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);

		if (CMap_ToolManager::Get_Instance()->Get_Emplace())
		{
			CMap_ToolManager::OBJECTDESC objDesc;
			ZeroMemory(&objDesc, sizeof(CMap_ToolManager::OBJECTDESC));

			objDesc.strName = m_strName;
			objDesc.Scale = m_Scale;
			objDesc.Rotation = m_Rotation;
			objDesc.Translation = m_Translation;
			objDesc.matrix = m_pTransform->Get_Matrix();
			objDesc.bSave = false;
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

			CMap_ToolManager::Get_Instance()->Set_Emplace(false);
		}
	}
	auto pGameInstance = CMap_ToolManager::Get_Instance();

	if (pGameInstance->Get_AllClear())
	{
		m_bDead = true;
		pGameInstance->Set_AllClear(false);
	}

	if (pGameInstance->Get_Delete())
	{
		if (m_strName == pGameInstance->Get_SelectName())
		{
			pGameInstance->Set_Delete(false);

			m_bDead = true;
		}
	}
}

void CMap_Suoh_Bus::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BUSA;
		m_vExtents = { 2.5f,2.7f,7.f };
		m_vLocalPose = { 0.f,2.f,0.f };
		m_fMass = 7000.f;
		m_fPickWorldDistance = 15.f;
		m_eType = SPECIAL_THROW::BUS;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);
}

shared_ptr<CMap_Suoh_Bus> CMap_Suoh_Bus::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_Bus> pInstance = make_private_shared(CMap_Suoh_Bus, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Bus::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Bus::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_Bus> pInstance = make_private_shared_copy(CMap_Suoh_Bus, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Bus::Clone", "Failed to Initialize");
	}

	return pInstance;
}
