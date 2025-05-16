#include "ClientPCH.h"
#include "Map_Special_Dynamic.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"
#include "PlayerState.h"
#include "PlayerSpecialThrowState.h"
#include "Psychokinesis.h"

#include "Monster.h"
#include <regex>

//int g_iStage1SpecialDynamicIndex = {0};

CMap_Special_Dynamic::CMap_Special_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_Special_Dynamic::CMap_Special_Dynamic(const CMap_Special_Dynamic& _rhs)
	:CMap_Dynamic(_rhs),
	m_eType(_rhs.m_eType),
	m_fWorldThreshHold(_rhs.m_fWorldThreshHold),
	m_fScreenThreshHold(_rhs.m_fScreenThreshHold)
{

}

HRESULT CMap_Special_Dynamic::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);

	Add_ModelCom();
	Add_SpecialType();
	return S_OK;
}

HRESULT CMap_Special_Dynamic::Initialize(any pArg)
{
	if (FAILED(__super::Initialize(pArg)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	Create_Light(m_pTransform);

	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
		any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

	m_copyTuplePivot = tObjTuple;

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
	if (std::get<0>(tObjTuple) == SCENE::TEST)
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
	m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
	m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(m_vExtents.x, m_vExtents.y, m_vExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));

	m_pActor->attachShape(*m_pShape);
	m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//PxShape* pShape;
	//m_pActor->getShapes(&pShape, 1);
	//pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);

	ACTORDESC actorDesc;
	actorDesc.bThrow = false;
	m_pActor->userData = &actorDesc;

	m_fMass *= 1.5f;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Special_Dynamic::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.1f;

	if (m_bActivePhysX)
	{
		if (nullptr != m_pActor && !(m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC))
		{
			PxMat44 worldMat(m_pActor->getGlobalPose());
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
			_float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
			_float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
			_float4x4 rotmat = XMMatrixRotationQuaternion(q);

			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 OriginScale = m_pTransform->Get_Scale();
			m_pTransform->Set_Matrix(rotmat);
			m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
			m_pTransform->Set_Scale(OriginScale);
		}
	}
	else
	{
		if (nullptr != m_pActor)
		{
			PxMat44 worldMat(m_pActor->getGlobalPose());


			_float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

			PxTransform px;
			px.p = PxVec3(vPos.x, vPos.y, vPos.z);
			px.q = PxQuat(q.x, q.y, q.z, q.w);

			m_pActor->setGlobalPose(px);
		}
	}

	auto pGameInstance = CGameInstance::Get_Instance();

	//if (nullptr != m_pActor)
	//{
	//	ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
	//	if (pActorDesc != nullptr && pActorDesc->bThrowHit && pActorDesc->bThrow)
	//	{
	//		if (!m_bExecuted[52])
	//		{
	//			m_bExecuted[52] = true;
	//			ThrowHit();
	//		}
	//	}
	//}
	if (m_bThrow)
	{
		m_fThrowTime -= fTimeDelta;
		if (m_fThrowTime < 1.5f)
		{
			SetPhysX(true);
			Set_Gravity(true);
		}
		if (m_fThrowTime < 0.f)
		{
			m_bDissolve = true;
			m_bInteractivable = false;

			m_iPassIndex = 16;
			m_iGlassPassIndex = 16;

			if (nullptr != m_pActor)
			{
				pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
				m_pActor->release();
				m_pActor = nullptr;
			}
		}
	}

	if (m_bThrowHit)
	{
			if (!m_bExecuted[56])
		{
			m_bExecuted[56] = true;
			m_vThrowLook = _float3(Function::RandomFloat(-1.f, 1.f), Function::RandomFloat(-1.f, 0.f), Function::RandomFloat(-1.f, 1.f));
		}

		m_ThrowHitTime -= fTimeDelta;
		if (m_ThrowHitTime > 0.f)
		{
			m_pTransform->Translate(_float3(-m_vThrowLook.x * fTimeDelta * 2.f, -m_vThrowLook.y * fTimeDelta * 2.f, -m_vThrowLook.z * fTimeDelta * 2.f));
			m_pTransform->Rotate(m_vThrowLook, fTimeDelta * 0.5f);
		}
		if (m_ThrowHitTime < 0.f)
		{
			m_pTransform->Translate(_float3(-m_vThrowLook.x * fTimeDelta * 2.f, -m_vThrowLook.y * fTimeDelta * 2.f - m_ThrowHitTime * m_ThrowHitTime * 1.f, -m_vThrowLook.z * fTimeDelta * 2.f));
			m_pTransform->Rotate(m_vThrowLook, fTimeDelta * 0.5f);
		}

		if (m_ThrowHitTime < 0.f && !m_bExecuted[51])
		{
			m_bExecuted[51] = true;
			//SetPhysX(true);
			//Set_Gravity(true);
			if (nullptr != m_pActor)
			{
			}
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);
		}
		if (m_ThrowHitTime < -0.3f)
		{
			m_bDissolve = true;
			m_bInteractivable = false;
			Interactive_PhychoKinesis(false);
			m_iPassIndex = 16;
			m_iGlassPassIndex = 16;
		}
	}


	if (m_bDissolve)
	{
		if (m_eType != SPECIAL_THROW::BEAM && m_eType != SPECIAL_THROW::BUS)
			m_fDissolveTime += fTimeDelta * 1.f;
		else
			m_fDissolveTime += fTimeDelta * 0.1f;

		if (m_fDissolveTime >= 1.f)
		{
			m_bAlive = false;
		}
	}

}

void CMap_Special_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CMap_Special_Dynamic::Render()
{
	if (m_bDissolve)
	{
		if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
		{
			return E_FAIL;
		}

		if (FAILED(m_pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
		{
			return E_FAIL;
		}

		if (FAILED(CInteractiveObject::Render(16)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
		}

		if (FAILED(CInteractiveObject::Render(15)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
		}
	}


	return S_OK;
}

HRESULT CMap_Special_Dynamic::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Special_Dynamic::Ready_Components()
{
	if (FAILED(CInteractiveObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
	}
	//m_pModel->Initialize_MaterialDesc(&m_tMaterialDesc);

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

void CMap_Special_Dynamic::Add_ModelCom()
{

}

void CMap_Special_Dynamic::Add_SpecialType()
{

}

shared_ptr<CMap_Special_Dynamic> CMap_Special_Dynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Special_Dynamic> pInstance = make_private_shared(CMap_Special_Dynamic, _pDevice, _pContext);

	//g_iStage1SpecialDynamicIndex = iIndex;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_SpecialDynamic::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Special_Dynamic::Clone(any pArg)
{
	shared_ptr<CMap_Special_Dynamic> pInstance = make_private_shared_copy(CMap_Special_Dynamic, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_SpecialDynamic::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CMap_Special_Dynamic::Throw()
{
	
}

void CMap_Special_Dynamic::ThrowHit()
{

}