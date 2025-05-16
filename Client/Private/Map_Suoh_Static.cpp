#include "ClientPCH.h"
#include "Map_Suoh_Static.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Suoh_Static::CMap_Suoh_Static(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Suoh_Static::CMap_Suoh_Static(const CMap_Suoh_Static& _rhs)
	:CGameObject(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_Suoh_Static::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Suoh_Static::Initialize(any pArg)
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

	// Hide Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_PLANTSA ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_TREEA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 3);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 15;
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_PLANTSA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SG_TREEA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(3);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
	}
	
	if (!m_bSceneMapTool)
	{
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);
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

	m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
	m_tInstModelShadowDesc.iPassIndex = 2;

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_Static::Tick(_float fTimeDelta)
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
}

void CMap_Suoh_Static::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Suoh_Static", &bRender);
			ImGui::End();
		}
	if (!bRender) return;
#endif

	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);
	
	/*if (m_bApplyPx)
	{
		if (!CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
			return;
	}*/

	//Add_RenderObject(RENDER_GROUP::SHADOW);
	//Add_RenderObject(RENDER_GROUP::NONBLEND);


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

HRESULT CMap_Suoh_Static::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}
	
	if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Static::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Static::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Static::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Static::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_Static::Ready_Components()
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

void CMap_Suoh_Static::Using_Map_Tool()
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

_bool CMap_Suoh_Static::Maptool_No_Render(wstring Name)
{
	_bool bCheck = { false };

	std::wregex pattern(L"_number\\d+");
	wstring TempStr = regex_replace(Name, pattern, L"");

	if (TempStr == TEXT("Suoh_RubbleA"))
		bCheck = true;

	if (TempStr == TEXT("SG_Sumeragi_Tourou"))
		bCheck = true;

	if (TempStr == TEXT("SG_PlantsA"))
		bCheck = true;

	if (TempStr == TEXT("SG_TreeA"))
		bCheck = true;

	return bCheck;
}

void CMap_Suoh_Static::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_WALLB;
	}
	else if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_WALLC;
	}
	else if (m_iIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_WALLD;
	}
	else if (m_iIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERBASE;
	}
	else if (m_iIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TOWN_ROADA;
	}
	else if (m_iIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_SHUTTERA;
	}
	else if (m_iIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_FLOWERPOT;
	}
	else if (m_iIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_KEIJIBAN;
	}
	else if (m_iIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_SEKIHI;
	}
	else if (m_iIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_OMIKUJIB;
	}
	else if (m_iIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_ARCHA;
	}
	else if (m_iIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUMERAGI_KAIDANA;
	}
	else if (m_iIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_RUBBLEA;
	}
	else if (m_iIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_PLANTSA;
	}
	else if (m_iIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_TREEA;
	}
	else if (m_iIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SG_SUMERAGI_TOUROU;
	}
	else if (m_iIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MAITREYATRII;
	}
	else if (m_iIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAA;
	}
	else if (m_iIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_CAMERAD;
	}
	else if (m_iIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HYDRANTA;
	}
	else if (m_iIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TELEPHONEBOXA;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

shared_ptr<CMap_Suoh_Static> CMap_Suoh_Static::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_Static> pInstance = make_private_shared(CMap_Suoh_Static, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Static::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Static::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_Static> pInstance = make_private_shared_copy(CMap_Suoh_Static, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Static::Clone", "Failed to Initialize");
	}

	return pInstance;
}
