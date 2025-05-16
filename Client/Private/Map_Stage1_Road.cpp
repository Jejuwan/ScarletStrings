#include "ClientPCH.h"
#include "Map_Stage1_Road.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Stage1_Road::CMap_Stage1_Road(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Stage1_Road::CMap_Stage1_Road(const CMap_Stage1_Road& _rhs)
	:CGameObject(_rhs)
	, m_iRoadIndex(_rhs.m_iRoadIndex)
{
}

HRESULT CMap_Stage1_Road::Initialize_Prototype(_uint iRoadIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iRoadIndex = iRoadIndex;

	return S_OK;
}

HRESULT CMap_Stage1_Road::Initialize(any pArg)
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

	Apply_Px(m_strName);

	if (m_strName == L"BossBanisterA"
		|| m_strName == L"BossBanisterB"
		|| m_strName == L"BossBanisterC"
		|| m_strName == L"BossBanisterD"
		|| m_strName == L"BossBanisterE"
		|| m_strName == L"BossBanisterF"
		|| m_strName == L"ConcretePiraE"
		|| m_strName == L"FootholdD"
		|| m_strName == L"FootholdE"
		|| m_strName == L"FootholdF"
		|| m_strName == L"FootholdG"
		|| m_strName == L"FootholdShapeA"
		|| m_strName == L"FootholdShapeB"
		|| m_strName == L"Bridge_EnkeiB"
		|| m_strName == L"Bridge_EnkeiC"
		)
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform, false);
	else
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

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
void CMap_Stage1_Road::Tick(_float fTimeDelta)
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

	auto pGameInstance = CMap_ToolManager::Get_Instance();

	if (pGameInstance->Get_Delete())
	{
		if (m_strName == pGameInstance->Get_SelectName())
		{
			pGameInstance->Set_Delete(false);

			m_bDead = true;
		}
	}

	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Stage1_Road::Late_Tick(_float fTimeDelta)
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
		ImGui::Checkbox("CMap_Stage1_Road", &bRender);
		ImGui::End();
	}
	if (!bRender) return;
#endif

	__super::Late_Tick(fTimeDelta);

	//Add_RenderObject(RENDER_GROUP::SHADOW);

	if (m_bApplyPx)
	{
		//if (!CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
		//	return;
	}

	//Add_RenderObject(RENDER_GROUP::NONBLEND);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	if (m_bEnableShadow)
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	}
}

HRESULT CMap_Stage1_Road::Render()
{
	/*if (m_bApplyPx)
	{
	}*/
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

HRESULT CMap_Stage1_Road::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Road::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Road::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Stage1_Road::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Stage1_Road::Ready_Components()
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

void CMap_Stage1_Road::Using_Map_Tool()
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

void CMap_Stage1_Road::Apply_Px(wstring Name)
{
	std::wregex pattern(L"_number\\d+");
	wstring TempStr = regex_replace(Name, pattern, L"");

	/*if (TempStr == TEXT("ConcretePiraC"))
		m_bApplyPx = false;
	else if (TempStr == TEXT("ConcretePiraA"))
		m_bApplyPx = false;
	else if (TempStr == TEXT("ConcretePiraB"))
		m_bApplyPx = false;
	else if (TempStr == TEXT("ConcretePiraJ"))
		m_bApplyPx = false;
	else if (TempStr == TEXT("ConcretePiraI"))
		m_bApplyPx = false;*/
	/*if (TempStr == TEXT("Dirt_Tile")|| )
		m_bApplyPx = true;
	else
		m_bApplyPx = false;*/

	if (TempStr == TEXT("Area_MiniMapA"))
		m_bApplyPx = false;
	else if(TempStr == TEXT("Dirt_Tile"))
		m_bApplyPx = false;
	else if (TempStr == TEXT("Dirt_Ground"))
		m_bApplyPx = false;
}

void CMap_Stage1_Road::Add_ModelCom()
{
	m_bEnableShadow = true;
	if (m_iRoadIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAP;
		//m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_Dirt_Tile;
	}
	else if (m_iRoadIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SandyOil_Tile;
	}
	else if (m_iRoadIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CIRCLE_TILE;
	}
	else if (m_iRoadIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_AREA_MINIMAPA;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAA;
	}
	else if (m_iRoadIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAB;
	}
	else if (m_iRoadIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAC;
	}
	else if (m_iRoadIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAD;
	}
	else if (m_iRoadIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAE;
	}
	else if (m_iRoadIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAF;
	}
	else if (m_iRoadIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAG;
	}
	else if (m_iRoadIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAH;
	}
	else if (m_iRoadIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAI;
	}
	else if (m_iRoadIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAJ;
	}
	else if (m_iRoadIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAK;
	}
	else if (m_iRoadIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIRAL;
	}
	else if (m_iRoadIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDD;
	}
	else if (m_iRoadIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDE;
	}
	else if (m_iRoadIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDF;
	}
	else if (m_iRoadIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDSHAPEA;
	}
	else if (m_iRoadIndex == 21)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDSHAPEB;
	}
	else if (m_iRoadIndex == 22)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_FOOTHOLDG;
	}
	else if (m_iRoadIndex == 23)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_DIRT_GROUND;
	}
	else if (m_iRoadIndex == 24)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTA;
	}
	else if (m_iRoadIndex == 25)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTB;
	}
	else if (m_iRoadIndex == 26)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTC;
	}
	else if (m_iRoadIndex == 27)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTD;
	}
	else if (m_iRoadIndex == 28)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONBUILDINGC;
		}
	else if (m_iRoadIndex == 29)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BRIDGE_ENKEIB;
		}
	else if (m_iRoadIndex == 30)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTBENDA;
		}
	else if (m_iRoadIndex == 31)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BASEMENTE;
		}
	else if (m_iRoadIndex == 32)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLA;
		}
	else if (m_iRoadIndex == 33)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLB;
		}
	else if (m_iRoadIndex == 34)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLC;
		}
	else if (m_iRoadIndex == 35)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLD;
		}
	else if (m_iRoadIndex == 36)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLE;
		}
	else if (m_iRoadIndex == 37)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLF;
		}
	else if (m_iRoadIndex == 38)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLG;
		}
	else if (m_iRoadIndex == 39)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLH;
		}
	else if (m_iRoadIndex == 40)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSWALLI;
		}
	else if (m_iRoadIndex == 41)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERA;
		}
	else if (m_iRoadIndex == 42)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERB;
		}
	else if (m_iRoadIndex == 43)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERC;
		}
	else if (m_iRoadIndex == 44)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERD;
		}
	else if (m_iRoadIndex == 45)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERE;
		}
	else if (m_iRoadIndex == 46)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOSSBANISTERF;
		}
	else if (m_iRoadIndex == 47)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONSTRUCTIONSLOPEA;
		}
	else if (m_iRoadIndex == 48)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BRIDGE_ENKEIC;
		}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Stage1_Road::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_pModel->Hide_Mesh(1);
	}

}

HRESULT CMap_Stage1_Road::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_pModel->Render(1, m_pShader, 4);
	}
	
	return S_OK;
}

void CMap_Stage1_Road::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_iMeshIndex = 1;
	}
}

shared_ptr<CMap_Stage1_Road> CMap_Stage1_Road::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Stage1_Road> pInstance = make_private_shared(CMap_Stage1_Road, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Road::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_Road::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_Road> pInstance = make_private_shared_copy(CMap_Stage1_Road, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Road::Clone", "Failed to Initialize");
	}

	return pInstance;
}
