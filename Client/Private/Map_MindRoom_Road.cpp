#include "ClientPCH.h"
#include "Map_MindRoom_Road.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

#include "ImGui_Manager.h"

CMap_MindRoom_Road::CMap_MindRoom_Road(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_MindRoom_Road::CMap_MindRoom_Road(const CMap_MindRoom_Road& _rhs)
	:CGameObject(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_MindRoom_Road::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;

	return S_OK;
}

HRESULT CMap_MindRoom_Road::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Initialize", "Failed to CGameObject::Initialize");
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
	if (std::get<0>(tObjTuple) == SCENE::MINDROOM||
		std::get<0>(tObjTuple) == SCENE::REALTEST)
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
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 26; // 23
		// 데칼 타겟이며 그림자도 나와야 함.
		m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
			{
				if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
				}
				else
					return S_OK;
			};
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(20);
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(20);
		}
		m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
		{
			if (pShader->Bind_Float("g_fAccTime", m_fTimer))
			{
				MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
			}
			else
				return S_OK;
		};
	}
	
	if(m_iIndex ==0)
		m_pModelRenderer->Bake_Physics(m_wstrModelTag, m_pTransform);

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_MindRoom_Road::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif

	if (m_bDead)
		return;
	
	__super::Tick(fTimeDelta);

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.05f;

	if (m_bSingleColorMode)
	{
		m_fColorFactor += fTimeDelta * 0.3f;
		if (m_fColorFactor >= 1.f)
		{
			m_fColorFactor = 1.f;
			m_vSingleColor = { 0.35f, 0.07f, 0.35f };
		}
	}

	if (CGameInstance::Get_Instance()->Key_Down('O'))
		Set_Visible(true);
	if (CGameInstance::Get_Instance()->Key_Down('P'))
		Set_Visible(false);

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//		ImGui::Begin("Radial Spread");
//
//		ImGui::SliderFloat("MaxSampleRadius", &m_fMaxSampleRadius, 0.f, 500.f);
//		ImGui::SliderFloat("VisualRadius", &m_fVisualRadius, 0.f, 500.f);
//
//		ImGui::End();
//	}
//#endif
}

void CMap_MindRoom_Road::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_MindRoom_Road", &bRender);
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

	if (m_bSingleColorMode)
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NEON, m_tInstModelDesc, m_pTransform);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NEON, m_tInstModelMeshDesc, m_pTransform);
	}
	else
	{
		//if (false == CMap_ToolManager::Get_Instance()->Is_MindRoom())
		{
			m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
			if (m_bVisible)
			{
				m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);
			}
		}
	}

	if (m_bRadialSpread)
	{
		m_fMaxSampleRadius += fTimeDelta * m_fSpreadSpeed;
		m_fVisualRadius = m_fMaxSampleRadius - 15.f;

		if (m_fMaxSampleRadius >= 20.f)
		{
			m_fSpreadSpeed = min(300.f, m_fSpreadSpeed + 10.f);
			//m_fVisualRadius = m_fMaxSampleRadius - 25.f;
		}
		if (m_fMaxSampleRadius >= 800.f)
		{
			Reset_Option();
		}
	}
}

HRESULT CMap_MindRoom_Road::Render()
{
	return S_OK;
}

HRESULT CMap_MindRoom_Road::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_MindRoom_Road::Render_OutLine()
{
	return S_OK;
}

void CMap_MindRoom_Road::Set_SingleColor_Lerp(_float3 vColor)
{
	m_bSingleColorMode = true;
	m_vSingleColor = vColor;
	m_fColorFactor = 0.f;

	m_tInstModelDesc.iPassIndex = 22;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(pShader->Bind_RawValue("g_vSingleColor", &m_vSingleColor, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render", "Failed to CModelRenderer::Render");
			}
			if (FAILED(pShader->Bind_Float("g_fColorFactor", m_fColorFactor)))
			{
				MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render", "Failed to CModelRenderer::Render");
			}

			return S_OK;
		};

	for (_uint i = 0; i < m_tInstModelMeshDesc.vecPassIndex.size(); ++i)
	{
		m_tInstModelMeshDesc.vecPassIndex[i] = 22;
	}
	m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(pShader->Bind_RawValue("g_vSingleColor", &m_vSingleColor, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render", "Failed to CModelRenderer::Render");
			}
			if (FAILED(pShader->Bind_Float("g_fColorFactor", m_fColorFactor)))
			{
				MSG_RETURN(E_FAIL, "CMap_MindRoom_Road::Render", "Failed to CModelRenderer::Render");
			}

			return S_OK;
		};
}

void CMap_MindRoom_Road::Set_SingleColor(_float3 vColor)
{
	m_bSingleColorMode = true;
	m_vSingleColor = vColor;
}

void CMap_MindRoom_Road::Set_SingleColorMode(_bool bSingleColorMode)
{
	m_bSingleColorMode = bSingleColorMode;

	if (false == m_bSingleColorMode)
	{
		m_tInstModelDesc.iPassIndex = 23;
		m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
			{
				if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
				}

				return S_OK;
			};

		for (_uint i = 0; i < m_tInstModelMeshDesc.vecPassIndex.size(); ++i)
		{
			m_tInstModelMeshDesc.vecPassIndex[i] = 20;
		}
		m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
			{
				if (pShader->Bind_Float("g_fAccTime", m_fTimer))
				{
					MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
				}

				return S_OK;
			};
	}
}

void CMap_MindRoom_Road::Effect_SwipeClear()
{
}

void CMap_MindRoom_Road::Set_Visible(_bool bVisible)
{
	m_bVisible = bVisible;

	if (bVisible)
	{
		m_tInstModelDesc.iPassIndex = 23;
	}
	else
	{
		// Decal Target Depth Render
		m_tInstModelDesc.iPassIndex = 24;
	}
}

void CMap_MindRoom_Road::Execute_RadialSperad()
{
	m_bRadialSpread = true;

	m_fMaxSampleRadius = 2.f;
	m_fVisualRadius = 0.f;

	m_fSpreadSpeed = 10.f;

	m_tInstModelDesc.iPassIndex = 25;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			auto pGameInstance = CGameInstance::Get_Instance();
			if (FAILED(pGameInstance->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}

			_float3 vPlayerPosition = pGameInstance->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
			if (FAILED(pShader->Bind_RawValue("g_vTargetPosition", &vPlayerPosition, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Target Position");
			}

			if (FAILED(pShader->Bind_Float("g_fSampleRadius", m_fMaxSampleRadius)))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Radius");
			}
			if (FAILED(pShader->Bind_Float("g_fSampleMinRadius", m_fVisualRadius)))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Radius");
			}

			return S_OK;
		};

	for (_uint i = 0; i < m_tInstModelMeshDesc.vecPassIndex.size(); ++i)
	{
		m_tInstModelMeshDesc.vecPassIndex[i] = 25;
	}
	m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
		{
			auto pGameInstance = CGameInstance::Get_Instance();
			_float3 vPlayerPosition = pGameInstance->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);
			if (FAILED(pShader->Bind_RawValue("g_vTargetPosition", &vPlayerPosition, sizeof(_float3))))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Target Position");
			}

			if (FAILED(pShader->Bind_Float("g_fSampleRadius", m_fMaxSampleRadius)))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Radius");
			}
			if (FAILED(pShader->Bind_Float("g_fSampleMinRadius", m_fVisualRadius)))
			{
				MSG_RETURN(E_FAIL, "Map_MindRoom_Neon", "InstModel Render: Bind Radius");
			}

			return S_OK;
		};
}

void CMap_MindRoom_Road::Reset_Option()
{
	m_bRadialSpread = false;

	m_tInstModelDesc.iPassIndex = 23;
	m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
			}

			return S_OK;
		};

	for (_uint i = 0; i < m_tInstModelMeshDesc.vecPassIndex.size(); ++i)
	{
		m_tInstModelMeshDesc.vecPassIndex[i] = 20;
	}
	m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
		{
			if (FAILED(pShader->Bind_Float("g_fAccTime", m_fTimer)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render", "Failed to m_fTimer");
			}

			return S_OK;
		};
}

HRESULT CMap_MindRoom_Road::Ready_Components()
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

void CMap_MindRoom_Road::Using_Map_Tool()
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

void CMap_MindRoom_Road::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_FUTA;
	}
	if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_WATER;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}


shared_ptr<CMap_MindRoom_Road> CMap_MindRoom_Road::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_MindRoom_Road> pInstance = make_private_shared(CMap_MindRoom_Road, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_MindRoom_Road::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_MindRoom_Road::Clone(any pArg)
{
	shared_ptr<CMap_MindRoom_Road> pInstance = make_private_shared_copy(CMap_MindRoom_Road, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_MindRoom_Road::Clone", "Failed to Initialize");
	}

	return pInstance;
}
