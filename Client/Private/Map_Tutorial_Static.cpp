#include "ClientPCH.h"
#include "Map_Tutorial_Static.h"
#include "GameInstance.h"
#include "Light.h"
#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Frustom.h"
#include <regex>

CMap_Tutorial_Static::CMap_Tutorial_Static(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Tutorial_Static::CMap_Tutorial_Static(const CMap_Tutorial_Static& _rhs)
	: CGameObject(_rhs)
	, m_iTutorialStaticIndex(_rhs.m_iTutorialStaticIndex)
{
}

HRESULT CMap_Tutorial_Static::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iTutorialStaticIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Tutorial_Static::Initialize(any pArg)
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

	m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

	// Hide Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCKA || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BUSA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCKA || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_GAS || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_GASBRK || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_CONTAINER)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}

		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 15;
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCKA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(3);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCKA ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_GAS ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_GASBRK
			|| m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TRUCK_CONTAINER)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
		if (m_strIndexName == PROTOTYPE_COMPONENT_MODEL_BUSA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(8);
		}
	}

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	return S_OK;
}

void CMap_Tutorial_Static::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Tutorial_Static::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;
	__super::Late_Tick(fTimeDelta);

	//if (!CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
	//	return;

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	//if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
	//{
	//	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//}
	//Add_RenderObject(RENDER_GROUP::SHADOW);
}

HRESULT CMap_Tutorial_Static::Render()
{
	return S_OK;
}

HRESULT CMap_Tutorial_Static::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Tutorial_Static::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Tutorial_Static::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Tutorial_Static::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Tutorial_Static::Ready_Components()
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

void CMap_Tutorial_Static::Using_Map_Tool()
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

void CMap_Tutorial_Static::Add_ModelCom()
{
	if (m_iTutorialStaticIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRUCKA;
	}
	else if (m_iTutorialStaticIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRUCK_CAMPINGCAR;
	}
	else if (m_iTutorialStaticIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRUCK_CONTAINER;
	}
	else if (m_iTutorialStaticIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRUCK_GAS;
	}
	else if (m_iTutorialStaticIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRUCK_GASBRK;
	}
	else if (m_iTutorialStaticIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BUSA;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Tutorial_Static::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("CarC") || m_strIndexName == TEXT("BusA"))
	{
		m_pModel->Hide_Mesh(1);
	}

	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("Truck_Gas") || m_strIndexName == TEXT("Truck_GasBrk")
		|| m_strIndexName == TEXT("Truck_Container"))
	{
		m_pModel->Hide_Mesh(0);
	}

}

HRESULT CMap_Tutorial_Static::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("CarC"))
	{
		m_pModel->Render(1, m_pShader, 5);
	}
	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("Truck_Gas") || m_strIndexName == TEXT("Truck_GasBrk")
		|| m_strIndexName == TEXT("Truck_Container"))
	{
		m_pModel->Render(0, m_pShader, 6);
	}
	if (m_strIndexName == TEXT("BusA"))
	{
		m_pModel->Render(1, m_pShader, 8);
	}

	return S_OK;
}

void CMap_Tutorial_Static::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("CarC") || m_strIndexName == TEXT("BusA"))
	{
		m_iMeshIndex = 1;
	}
	if (m_strIndexName == TEXT("TruckA") || m_strIndexName == TEXT("Truck_Gas") || m_strIndexName == TEXT("Truck_GasBrk")
		|| m_strIndexName == TEXT("Truck_Container"))
	{
		m_iMeshIndex = 0;
	}
}

shared_ptr<CMap_Tutorial_Static> CMap_Tutorial_Static::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Tutorial_Static> pInstance = make_private_shared(CMap_Tutorial_Static, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Tutorial_Static::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Tutorial_Static::Clone(any pArg)
{
	shared_ptr<CMap_Tutorial_Static> pInstance = make_private_shared_copy(CMap_Tutorial_Static, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Tutorial_Static::Clone", "Failed to Initialize");
	}

	return pInstance;
}
