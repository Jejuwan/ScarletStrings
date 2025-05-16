#include "ClientPCH.h"
#include "Map_Stage1_Building.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Stage1_Building::CMap_Stage1_Building(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Stage1_Building::CMap_Stage1_Building(const CMap_Stage1_Building& _rhs)
	:CGameObject(_rhs)
	, m_iBuldingIndex(_rhs.m_iBuldingIndex)
{
}

HRESULT CMap_Stage1_Building::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iBuldingIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Stage1_Building::Initialize(any pArg)
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

	m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelDesc.iPassIndex = 15;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}
		};

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_Building::Tick(_float fTimeDelta)
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

	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Stage1_Building::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		bCheckOnce = false;
		ImGui::Begin("Please");
		ImGui::Checkbox("CMap_Stage1_Building", &bRender);
		ImGui::End();
	}
	if (!bRender) return;
#endif

	__super::Late_Tick(fTimeDelta);

	//if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
	//{
	//	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//}
	//Add_RenderObject(RENDER_GROUP::SHADOW);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
}

HRESULT CMap_Stage1_Building::Render()
{
	if (FAILED(__super::Render(0))) 
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render"); 
	}

	/*Mesh_Index(m_strIndexName);

	if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	Mesh_Render(m_strIndexName);*/
	
	return S_OK;
}

HRESULT CMap_Stage1_Building::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Building::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Building::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Building::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Stage1_Building::Ready_Components()
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

void CMap_Stage1_Building::Using_Map_Tool()
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

void CMap_Stage1_Building::Add_ModelCom()
{
	if (m_iBuldingIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLD;
	}
	else if (m_iBuldingIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_REALSTEELWALLE;
	}
	else if (m_iBuldingIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL4ANON;
	}
	else if (m_iBuldingIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL3A;
	}
	else if (m_iBuldingIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STEELWALL3B;
	}
	else if (m_iBuldingIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CITYBUILDING_PARTSA;
	}
	else if (m_iBuldingIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CITYBUILDINGA;
	}
	else if (m_iBuldingIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGA;
	}
	else if (m_iBuldingIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGB;
	}
	else if (m_iBuldingIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CRANE_ENKEIA;
	}
	else if (m_iBuldingIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_DISTANTBUILDINGA;
	}
	else if (m_iBuldingIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_IRONPILLAR;
	}
	else if (m_iBuldingIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TARPA;
	}
	else if (m_iBuldingIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TARPB;
	}
	else if (m_iBuldingIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TARPC;
	}
	else if (m_iBuldingIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PLYWOODC;
	}
	else if (m_iBuldingIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PLYWOODD;
	}
	else if (m_iBuldingIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_GROUND_COALA;
	}
	else if (m_iBuldingIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CITYBUILDING_TOWER;
	}
	else if (m_iBuldingIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGA;
	}
	else if (m_iBuldingIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGB;
	}
	else if (m_iBuldingIndex == 21)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGC;
	}
	else if (m_iBuldingIndex == 22)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGD;
	}
	else if (m_iBuldingIndex == 23)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGE;
	}
	else if (m_iBuldingIndex == 24)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MIZUHABUILDINGF;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Stage1_Building::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Ground_Grass"))
	{
		m_pModel->Hide_Mesh(0);
	}

}

HRESULT CMap_Stage1_Building::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Ground_Grass"))
	{
		m_pModel->Render(0, m_pShader, 10);
	}
	
	return S_OK;
}

void CMap_Stage1_Building::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("Ground_Grass"))
	{
		m_iMeshIndex = 0;
	}
}

shared_ptr<CMap_Stage1_Building> CMap_Stage1_Building::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Stage1_Building> pInstance = make_private_shared(CMap_Stage1_Building, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Building::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_Building::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_Building> pInstance = make_private_shared_copy(CMap_Stage1_Building, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Building::Clone", "Failed to Initialize");
	}

	return pInstance;
}
