#include "ClientPCH.h"
#include "Map_Suoh_Neon.h"
#include "GameInstance.h"
#include "Light_Manager.h"
#include "Light.h"
#include "Pipeline.h"
#include "Camera.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Suoh_Neon::CMap_Suoh_Neon(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Suoh_Neon::CMap_Suoh_Neon(const CMap_Suoh_Neon& _rhs)
	:CGameObject(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_Suoh_Neon::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER)| BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Suoh_Neon::Initialize(any pArg)
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
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 0;
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
	}

	// Neon Render
	{
		m_tInstNeonMeshDesc.wstrModelTag = m_wstrModelTag;

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(0);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(18);
		}

		m_tInstNeonMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
		{
			if (pShader->Bind_Float("g_fAccTime", m_fTimer))
			{
				MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
			}
		};

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(0);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(19);
		}
	}

	return S_OK;
}

void CMap_Suoh_Neon::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}
	
	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Suoh_Neon::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);

	//m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	//m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NEON, m_tInstNeonMeshDesc, m_pTransform);
}

HRESULT CMap_Suoh_Neon::Render()
{


	return S_OK;
}

HRESULT CMap_Suoh_Neon::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	return S_OK;
}

HRESULT CMap_Suoh_Neon::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_Neon::Render_Neon()
{
	//if (m_pTransform->Bind_OnShader(m_pShader))
	//{
	//	MSG_RETURN(E_FAIL, "CModel::Render_Neon", "Failed to Bind_OnShader");
	//}

	//if (m_pShader->Bind_Float("g_fAccTime", m_fTimer))
	//{
	//	MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
	//}
	//
	//Mesh_Index(m_strIndexName);

	////네온 매쉬 따로 그림
	//if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
	//{
	//	MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	//}

	//Neon_Render(m_strIndexName);

	return S_OK;
}

HRESULT CMap_Suoh_Neon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	//m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	//if (nullptr == m_pShader)
	//{
	//	MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	//}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}

	return S_OK;
}

void CMap_Suoh_Neon::Using_Map_Tool()
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
		auto pGameInstance = CMap_ToolManager::Get_Instance();

		if (pGameInstance->Get_AllClear())
		{
			pGameInstance->Set_AllClear(false);
			m_bDead = true;
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
}

void CMap_Suoh_Neon::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLA;
	}
	else if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SUOH_BATTLE_WARNINGWALLB;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}

void CMap_Suoh_Neon::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallA"))
	{
		m_pModel->Hide_Mesh(1);
	}
	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallB"))
	{
		m_pModel->Hide_Mesh(0);
	}
}

HRESULT CMap_Suoh_Neon::Neon_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");
	
	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallA"))
	{
		m_pModel->Render(0, m_pShader, 18);
	}
	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallB"))
	{
		m_pModel->Render(0, m_pShader, 14);
	}
	return S_OK;
}

void CMap_Suoh_Neon::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallA"))
	{
		m_iMeshIndex = 1;
	}
	if (m_strIndexName == TEXT("Suoh_Battle_WarningWallB"))
	{
		m_iMeshIndex = 0;
	}
}

shared_ptr<CMap_Suoh_Neon> CMap_Suoh_Neon::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_Neon> pInstance = make_private_shared(CMap_Suoh_Neon, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Neon::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Neon::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_Neon> pInstance = make_private_shared_copy(CMap_Suoh_Neon, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Neon::Clone", "Failed to Initialize");
	}

	return pInstance;
}
