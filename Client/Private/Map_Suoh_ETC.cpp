#include "ClientPCH.h"
#include "Map_Suoh_ETC.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Psychokinesis.h"

#include <regex>

CMap_Suoh_ETC::CMap_Suoh_ETC(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_Suoh_ETC::CMap_Suoh_ETC(const CMap_Suoh_ETC& _rhs)
	: CMap_Dynamic(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_Suoh_ETC::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Suoh_ETC::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
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
	if (std::get<0>(tObjTuple) == SCENE::SUOH ||
		std::get<0>(tObjTuple) == SCENE::SUOH_BATTLE)
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


	//_float4x4 mat = m_pTransform->Get_Matrix();
	//_vector Rotation, Scale, Pos;
	//XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);
	//
	//PxTransform pose = PxTransform(PxIdentity);
	//pose.p = PxVec3(mat._41, mat._42, mat._43);
	//_float4 vQuat;
	//XMStoreFloat4(&vQuat, Rotation);
	//pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	//
	//shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	//_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
	//m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(m_vExtents.x, m_vExtents.y, m_vExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	//m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));
	//
	//m_pActor->attachShape(*m_pShape);
	//if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::CUTSCENE_MIZUHA)
	//	SetPhysX(false);
	//else
	//	SetPhysX(true);
	////m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	//
	//ACTORDESC actorDesc;
	//actorDesc.bThrow = false;
	//m_pActor->userData = &actorDesc;
	//
	//m_fMass *= 1.5f;
	//m_fThrowForce = 150000.f;
	//PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
	//pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	/*if (!m_bSceneMapTool)
	{*/
		// Hide Mesh
		{
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1)
			{
				m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 2);
				m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 3);
			}
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEA ||
				m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEB || 
				m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEC)
			{
				m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
			}
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERA)
			{
				m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
			}
			m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
			m_tInstModelDesc.iPassIndex = 15;
		}

		// Mesh Render
		{
			m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1)
			{
				m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
				m_tInstModelMeshDesc.vecPassIndex.push_back(10);
			}
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1)
			{
				m_tInstModelMeshDesc.vecMeshIndex.push_back(3);
				m_tInstModelMeshDesc.vecPassIndex.push_back(10);
			}
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEA ||
				m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEB ||
				m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEC)
			{
				m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
				m_tInstModelMeshDesc.vecPassIndex.push_back(10);
			}
			if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERA)
			{
				m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
				m_tInstModelMeshDesc.vecPassIndex.push_back(6);
			}
		}
	//}

	if (!Maptool_No_Render(m_strName))
		//m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

	m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelDesc.iPassIndex = 15;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}
		};

	m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelShadowDesc.iPassIndex = 2;

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_ETC::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif
	if (m_bDead)
		return;

	Duplicate_Model(fTimeDelta);
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Suoh_ETC::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Suoh_ETC", &bRender);
			ImGui::End();

			ImGui::Begin("test normal shadow");
			static float fNormalDepthOffsetScale = 0;
			if (ImGui::InputFloat("fNormalDepthOffsetScale", &fNormalDepthOffsetScale))
			{
				m_pShader->Bind_Float("fNormalDepthOffsetScale", fNormalDepthOffsetScale);
			}
			ImGui::End();
		}
	if (!bRender) return;
#endif

	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_bSceneMapTool)
	{
		if (!Maptool_No_Render(m_strName))
		{
			m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
			m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);
		}
	}
	else
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

	}

}

HRESULT CMap_Suoh_ETC::Render()
{
	/*if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}*/

	if (FAILED(CInteractiveObject::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}
	
	return S_OK;
}

HRESULT CMap_Suoh_ETC::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_ETC::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_ETC::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Suoh_ETC::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_ETC::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
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

	return S_OK;
}

void CMap_Suoh_ETC::Using_Map_Tool()
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

_bool CMap_Suoh_ETC::Maptool_No_Render(wstring Name)
{
	_bool bCheck = { false };

	std::wregex pattern(L"_number\\d+");
	wstring TempStr = regex_replace(Name, pattern, L"");

	if (TempStr == TEXT("SG_Ground_A"))
		bCheck = true;

	/*if (TempStr == TEXT("PedestrianBridgeA"))
		bCheck = true;*/

	return bCheck;
}

void CMap_Suoh_ETC::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEA;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEB;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BICYCLEC;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERA;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_SCOOTERB;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_SHOPBOX;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B0;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B1;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B2;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_B3;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_F0;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G1;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_G2;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H0;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BILLBOARD_H1;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_BOXA;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_BOXB;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_BOXC;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_BOXD;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERA;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERB;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	if (m_iIndex == 21)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_CARRIERC;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Suoh_ETC::Duplicate_Model(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_bReadyDuplicate)
	{
		m_fDuplicateTime -= fTimeDelta;
		if (m_fDuplicateTime < 0.f)
		{
			m_bReadyDuplicate = false;
			shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
			wstring strPrototype;

			if (m_strName == TEXT("Suoh_BicycleA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEA;
			else if (m_strName == TEXT("Suoh_BicycleB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEB;
			else if (m_strName == TEXT("Suoh_BicycleC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BICYCLEC;
			else if (m_strName == TEXT("Suoh_ScooterA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERA;
			else if (m_strName == TEXT("Suoh_ScooterB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SCOOTERB;
			else if (m_strName == TEXT("Suoh_ShopBox"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_SHOPBOX;
			else if (m_strName == TEXT("Suoh_Billboard_B0"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B0;
			else if (m_strName == TEXT("Suoh_Billboard_B1"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B1;
			else if (m_strName == TEXT("Suoh_Billboard_B2"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B2;
			else if (m_strName == TEXT("Suoh_Billboard_B3"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_B3;
			else if (m_strName == TEXT("Suoh_Billboard_F0"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_F0;
			else if (m_strName == TEXT("Suoh_Billboard_G1"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G1;
			else if (m_strName == TEXT("Suoh_Billboard_G2"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_G2;
			else if (m_strName == TEXT("Suoh_Billboard_H0"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H0;
			else if (m_strName == TEXT("Suoh_Billboard_H1"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SUOH_BILLBOARD_H1;
			else if (m_strName == TEXT("Town_BoxA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXA;
			else if (m_strName == TEXT("Town_BoxB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXB;
			else if (m_strName == TEXT("Town_BoxC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXC;
			else if (m_strName == TEXT("Town_BoxD"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_BOXD;
			else if (m_strName == TEXT("Town_CarrierA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERA;
			else if (m_strName == TEXT("Town_CarrierB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERB;
			else if (m_strName == TEXT("Town_CarrierC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TOWN_CARRIERC;

			m_pDuplicatedObject = static_pointer_cast<CMap_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), strPrototype, m_copyTuplePivot));
			pLayer->Add(m_pDuplicatedObject);
			if (!m_bStrong)
				m_pDuplicatedObject->Throw();
			else
			{
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_DuplicatedObject(m_pDuplicatedObject);
				m_pDuplicatedObject->SetPhysX(false);
			}
			Interactive_PhychoKinesis(true);
			m_pDuplicatedObject->Interactive_PhychoKinesis(true);
		}
	}
}


shared_ptr<CMap_Suoh_ETC> CMap_Suoh_ETC::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_ETC> pInstance = make_private_shared(CMap_Suoh_ETC, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_ETC::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_ETC::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_ETC> pInstance = make_private_shared_copy(CMap_Suoh_ETC, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_ETC::Clone", "Failed to Initialize");
	}

	return pInstance;
}
