#include "ClientPCH.h"
#include "Map_Road.h"
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

CMap_Road::CMap_Road(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Road::CMap_Road(const CMap_Road& _rhs)
	:CGameObject(_rhs)
	, m_iRoadIndex(_rhs.m_iRoadIndex)
{
}

HRESULT CMap_Road::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iRoadIndex = iIndex;

	return S_OK;
}

HRESULT CMap_Road::Initialize(any pArg)
{
	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
	m_tMaterialDesc.fShininess = 0.1f;
	
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

	if (m_bApplyPx)
	{
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);
	}
	
	// Hide Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD0 ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD2)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD5)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD4 ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROADWALL)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 5);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD6)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 3);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD8)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 9);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 8);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADA)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 7);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGETREE || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 2);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGETREE ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		/* 도로 */
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5 || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD4)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 3);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5 ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD4)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD7)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 7);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9)
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

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD7)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(7);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGETREE || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGETREE ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}

		/* 도로*/
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5 || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD4)
		{
			m_tInstModelMeshDesc.fpBindResource = [](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (FAILED(pModel->Bind_ShaderResourceViews(3, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};
			m_tInstModelMeshDesc.vecMeshIndex.push_back(3);
			m_tInstModelMeshDesc.vecPassIndex.push_back(9);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5 ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD4)
		{
			m_tInstModelMeshDesc.fpBindResource = [](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (FAILED(pModel->Bind_ShaderResourceViews(1, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(9);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5)
		{
			m_tInstModelMeshDesc.fpBindResource = [](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (FAILED(pModel->Bind_ShaderResourceViews(2, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};
			m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
			m_tInstModelMeshDesc.vecPassIndex.push_back(9);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5)
		{
			m_tInstModelMeshDesc.fpBindResource = [](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (FAILED(pModel->Bind_ShaderResourceViews(3, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};
			m_tInstModelMeshDesc.vecMeshIndex.push_back(3);
			m_tInstModelMeshDesc.vecPassIndex.push_back(9);
		}
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9)
		{
			m_tInstModelMeshDesc.fpBindResource = [](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (FAILED(pModel->Bind_ShaderResourceViews(0, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(5);
		}
	}

	// Neon Render
	{
		m_tInstNeonMeshDesc.wstrModelTag = m_wstrModelTag;

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD0 || 
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD2)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(1);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD5)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(0);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD4 ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROADWALL)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(5);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD6)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(3);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(8);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADA)
		{
			m_tInstNeonMeshDesc.vecMeshIndex.push_back(7);
			m_tInstNeonMeshDesc.vecPassIndex.push_back(14);
		}

		m_tInstNeonMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
			{
				if (pShader->Bind_Float("g_fAccTime", m_fTimer))
				{
					MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
				}
			};
	}

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	return S_OK;
}

void CMap_Road::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

//#ifdef _DEBUG
//#ifdef ACTIVATE_IMGUI
//	ImGui::Begin("Mtrl Settings");
//
//	ImGui::SliderFloat4("Specular Settings", &m_tMaterialDesc.vSpecular.x, 0.f, 1.f);
//	ImGui::SliderFloat("Specular Energy", &m_tMaterialDesc.fShininess, 0.f, 1.f);
//	ImGui::End();
//#endif
//#endif
	
	m_fTimer += fTimeDelta * 0.1f;
}

void CMap_Road::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta);

	//Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::NEON);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

	if (m_bEnableShadow)
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	}

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NEON, m_tInstNeonMeshDesc, m_pTransform);
	//Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::SHADOW);
	//Add_RenderObject(RENDER_GROUP::NEON);
}

HRESULT CMap_Road::Render()
{
	//if (FAILED(__super::Render(0)))
	//{
	//	MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	//}
	//
	//Mesh_Index(m_strIndexName);

	////네온 매쉬 따로 그림
	//if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
	//{
	//	MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	//}

	//Mesh_Render(m_strIndexName);

	return S_OK;
}

HRESULT CMap_Road::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Road::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Road::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Road::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Road::Render_Neon()
{
	if (m_pTransform->Bind_OnShader(m_pShader))
	{
		MSG_RETURN(E_FAIL, "CModel::Render_Neon", "Failed to Bind_OnShader");
	}

	if (m_pShader->Bind_Float("g_fAccTime", m_fTimer))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
	}
	
	Mesh_Index(m_strIndexName);

	//네온 매쉬 따로 그림
	if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	Neon_Render(m_strIndexName);

	return S_OK;
}

HRESULT CMap_Road::Ready_Components()
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

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pNeonCrashTexture = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_NEON_CRASH));

	return S_OK;
}

void CMap_Road::Using_Map_Tool()
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

void CMap_Road::Apply_Px(wstring Name)
{
    std::wregex pattern(L"_number\\d+");
    wstring TempStr = regex_replace(Name, pattern, L"");

    if (TempStr == TEXT("Tutorial_Road5"))
        m_bApplyPx = false;
    else if (TempStr == TEXT("Tutorial_Road4"))
        m_bApplyPx = false;
    else if (TempStr == TEXT("Tutorial_EnterRoad6"))
        m_bApplyPx = false;
    else if (TempStr == TEXT("HugeTree"))
        m_bApplyPx = false;
    else if (TempStr == TEXT("HugeDarkTree"))
        m_bApplyPx = false;
}

void CMap_Road::Add_ModelCom()
{
	if (m_iRoadIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD0;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD1;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD2;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD3;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD5;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_SAKURA;
	}
	else if (m_iRoadIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD0;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD1;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD2;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD3;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD4;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 12)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD5;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD6;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD7;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD8;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROAD9;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 17)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADA;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 18)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADB;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 19)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ENTERROADC;
		m_bEnableShadow = false;
	}
	else if (m_iRoadIndex == 20)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROADWALL;
	}
	else if (m_iRoadIndex == 25)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_LOGSA;
	}
	else if (m_iRoadIndex == 26)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_LOGSB;
	}
	else if (m_iRoadIndex == 27)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_LOGSC;
		}
	else if (m_iRoadIndex == 28)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SAKURA_BRAKA;
		}
	else if (m_iRoadIndex == 29)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SAKURA_BRAKB;
		}
	else if (m_iRoadIndex == 30)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHA;
		}
	else if (m_iRoadIndex == 31)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHB;
		}
	else if (m_iRoadIndex == 32)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SAKURA_BRANCHC;
		}
	else if (m_iRoadIndex == 33)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_STONEA;
		}
	else if (m_iRoadIndex == 35)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_PILLAR;
		}
	else if (m_iRoadIndex == 36)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HUGETREE;
		}
	else if (m_iRoadIndex == 37)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTA;
		}
	else if (m_iRoadIndex == 38)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTB;
		}
	else if (m_iRoadIndex == 39)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTC;
		}
	else if (m_iRoadIndex == 40)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTD;
		}
	else if (m_iRoadIndex == 41)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTE;
		}
	else if (m_iRoadIndex == 42)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTF;
		}
	else if (m_iRoadIndex == 43)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROOTG;
		}

	else if (m_iRoadIndex == 44)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_HUGEDARKTREE;
		}
	else if (m_iRoadIndex == 45)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTA;
		}
	else if (m_iRoadIndex == 46)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTB;
		}
	else if (m_iRoadIndex == 47)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTC;
		}
	else if (m_iRoadIndex == 48)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTD;
		}
	else if (m_iRoadIndex == 49)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TUTORIAL_DARKROOTE;
	}
	else
	{
		int a = 0;
	}
	
	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);
}

void CMap_Road::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_pModel->Hide_Mesh(1);
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad1") || */m_strIndexName == TEXT("Tutorial_EnterRoad5"))
	{
		m_pModel->Hide_Mesh(0);
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad3") || */m_strIndexName == TEXT("Tutorial_EnterRoad4")
		/*|| m_strIndexName == TEXT("Tutorial_EnterRoaC")*/ || m_strIndexName == TEXT("Tutorial_RoadWall"))
	{
		m_pModel->Hide_Mesh(5);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad6"))
	{
		m_pModel->Hide_Mesh(3);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad8"))
	{
		m_pModel->Hide_Mesh(9);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad9"))
	{
		m_pModel->Hide_Mesh(8);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoadA"))
	{
		m_pModel->Hide_Mesh(7);
	}
	/*else if (m_strIndexName == TEXT("Tutorial_EnterRoadB"))
	{
		m_pModel->Hide_Mesh(4);
	}*/
	
	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_pModel->Hide_Mesh(2);

	}
	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_pModel->Hide_Mesh(1);

	}
	/* 도로 */
	if (m_strIndexName == TEXT("Tutorial_Road5") || m_strIndexName == TEXT("Tutorial_Road4"))
	{
		m_pModel->Hide_Mesh(3);
	}

	if (m_strIndexName == TEXT("Tutorial_Road5") || m_strIndexName == TEXT("Tutorial_Road4"))
	{
		m_pModel->Hide_Mesh(1);
	}

	if (m_strIndexName == TEXT("Tutorial_EnterRoad7"))
	{
		m_pModel->Hide_Mesh(7);
	}
	if (m_strIndexName == TEXT("Tutorial_EnterRoad9"))
	{
		m_pModel->Hide_Mesh(0);
	}
}

HRESULT CMap_Road::Neon_Render(wstring MeshName)
{
	if (FAILED(m_pNeonCrashTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
	{
		MSG_RETURN(E_FAIL, "CMap_Road::Neon_Render", "Fail to Bind_ShaderResourceView");
	}

	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");
	
	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_pModel->Render(1, m_pShader, 14);
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad1") || */m_strIndexName == TEXT("Tutorial_EnterRoad5"))
	{
		m_pModel->Render(0, m_pShader, 14);
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad3") || */m_strIndexName == TEXT("Tutorial_EnterRoad4")
		/*|| m_strIndexName == TEXT("Tutorial_EnterRoadC")*/ || m_strIndexName == TEXT("Tutorial_RoadWall"))
	{
		m_pModel->Render(5, m_pShader, 14);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad6"))
	{
		m_pModel->Render(3, m_pShader, 14);
	}
	/*else if (m_strIndexName == TEXT("Tutorial_EnterRoad8"))
	{
		m_pModel->Render(9, m_pShader, 4);
	}*/
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad9"))
	{
		m_pModel->Render(8, m_pShader, 14);
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoadA"))
	{
		m_pModel->Render(7, m_pShader, 14);
	}
	/*else if (m_strIndexName == TEXT("Tutorial_EnterRoadB"))
	{
		m_pModel->Render(4, m_pShader, 4);
	}*/
	
	return S_OK;
}

HRESULT CMap_Road::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Tutorial_EnterRoad7"))
	{
		m_pModel->Render(7, m_pShader, 5);
	}

	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_pModel->Render(2, m_pShader, 5);

	}
	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_pModel->Render(1, m_pShader, 5);

	}

	/* 도로*/
	if (m_strIndexName == TEXT("Tutorial_Road5") || m_strIndexName == TEXT("Tutorial_Road4"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(3, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(3, m_pShader, 9);
	}

	if (m_strIndexName == TEXT("Tutorial_Road5") || m_strIndexName == TEXT("Tutorial_Road4"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(1, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(1, m_pShader, 9);
	}

	if (m_strIndexName == TEXT("Tutorial_Road5"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(2, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(2, m_pShader, 9);
	}

	if (m_strIndexName == TEXT("Tutorial_Road5"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(3, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(3, m_pShader, 9);
	}
	if (m_strIndexName == TEXT("Tutorial_EnterRoad9"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(0, m_pShader, 5);
	}

	return S_OK;
}

void CMap_Road::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("Tutorial_EnterRoad0") || m_strIndexName == TEXT("Tutorial_EnterRoad2"))
	{
		m_iMeshIndex = 1;
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad1") || */m_strIndexName == TEXT("Tutorial_EnterRoad5"))
	{
		m_iMeshIndex = 0;
	}
	else if (/*m_strIndexName == TEXT("Tutorial_EnterRoad3") || */m_strIndexName == TEXT("Tutorial_EnterRoad4")
		|| m_strIndexName == TEXT("Tutorial_EnterRoadC") || m_strIndexName == TEXT("Tutorial_RoadWall"))
	{
		m_iMeshIndex = 5;
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad6"))
	{
		m_iMeshIndex = 3;
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad8"))
	{
		m_iMeshIndex = 9;
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoad9"))
	{
		m_iMeshIndex = 8;
	}
	else if (m_strIndexName == TEXT("Tutorial_EnterRoadA"))
	{
		m_iMeshIndex = 7;
	}
	/*else if (m_strIndexName == TEXT("Tutorial_EnterRoadB"))
	{
		m_iMeshIndex = 4;
	}*/
	
	if (m_strIndexName == TEXT("Tutorial_EnterRoad7"))
	{
		m_iMeshIndex = 7;
	}

	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_iMeshIndex = 2;
	}
	if (m_strIndexName == TEXT("HugeTree") || m_strIndexName == TEXT("HugeDarkTree"))
	{
		m_iMeshIndex = 1;
	}
	/* 도로 */
	if (/*m_strIndexName == TEXT("Tutorial_Road5") || */m_strIndexName == TEXT("Tutorial_Road4"))
	{
		m_iMeshIndex = 3;
	}
	if (/*m_strIndexName == TEXT("Tutorial_Road5") || */m_strIndexName == TEXT("Tutorial_Road4"))
	{
		m_iMeshIndex = 1;
	}
}

shared_ptr<CMap_Road> CMap_Road::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Road> pInstance = make_private_shared(CMap_Road, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Road::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Road::Clone(any pArg)
{
	shared_ptr<CMap_Road> pInstance = make_private_shared_copy(CMap_Road, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Road::Clone", "Failed to Initialize");
	}

	return pInstance;
}
