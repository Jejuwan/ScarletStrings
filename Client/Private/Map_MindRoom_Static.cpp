#include "ClientPCH.h"
#include "Map_MindRoom_Static.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>
#include <cmath>
#include <random>
#include <algorithm>

CMap_MindRoom_Static::CMap_MindRoom_Static(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_MindRoom_Static::CMap_MindRoom_Static(const CMap_MindRoom_Static& _rhs)
	:CGameObject(_rhs)
	, m_iIndex(_rhs.m_iIndex)
{
}

HRESULT CMap_MindRoom_Static::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iIndex = iIndex;
	return S_OK;
}

HRESULT CMap_MindRoom_Static::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Static::Initialize", "Failed to CGameObject::Initialize");
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

			if (Is_Sankaku(m_strNameNum, false))
			{
				m_Translation = Rendom_Pos();
				m_Mat._41 = Rendom_Pos().x;
				m_Mat._42 = Rendom_Pos().y;
				m_Mat._43 = Rendom_Pos().z;
			}
			
			m_pTransform->Set_Matrix(m_Mat);
		   
			CMap_ToolManager::OBJECTDESC objDesc;

			objDesc.Scale = std::get<2>(tObjTuple);
			objDesc.Rotation = std::get<3>(tObjTuple);
			objDesc.Translation = std::get<4>(tObjTuple);

			if (Is_Sankaku(m_strNameNum, false))
			{
				objDesc.Translation = Rendom_Pos();
				objDesc.matrix._41 = Rendom_Pos().x;
				objDesc.matrix._42 = Rendom_Pos().y;
				objDesc.matrix._43 = Rendom_Pos().z;
			}
			else
				objDesc.matrix = std::get<5>(tObjTuple);

			objDesc.strName = std::get<6>(tObjTuple);
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

		}
	}
	if (std::get<0>(tObjTuple) == SCENE::MINDROOM)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		m_Rotation = std::get<3>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_strNameNum = std::get<6>(tObjTuple);

		if (Is_Sankaku(m_strNameNum, false))
		{
			m_Translation = Rendom_Pos();
		}

		m_pTransform->Set_Matrix(m_Mat);
		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);
	}
	
	// Hide Mesh
	{
		//if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A)
		//{
		//	m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		//}
		//m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		//m_tInstModelDesc.iPassIndex = 0;
	}

	// Mesh Render
	{
		//m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		//if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A)
		//{
		//	m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
		//	m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		//}
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

	if (Is_Sankaku(m_strNameNum, true))
	{
		m_CreatePos = m_Translation;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<_float> RandomNumber(0.01f, 0.3f);

		m_fSpeed = RandomNumber(gen);
	}
	

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_MindRoom_Static::Tick(_float fTimeDelta)
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
	
	if (!CMap_ToolManager::Get_Instance()->Is_MindRoom())
	{
		if(Is_Sankaku(m_strNameNum, true))
			Sankaku_Rising(fTimeDelta);
	}
}

void CMap_MindRoom_Static::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_MindRoom_Static", &bRender);
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
	
	if (false == CMap_ToolManager::Get_Instance()->Is_MindRoom())
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
	}

	if (m_bRadialSpread)
	{
		m_fMaxSampleRadius += fTimeDelta * m_fSpreadSpeed;
		m_fVisualRadius = m_fMaxSampleRadius - 15.f;

		if (m_fMaxSampleRadius >= 20.f)
		{
			m_fSpreadSpeed = min(300.f, m_fSpreadSpeed + 10.f);
		}
		if (m_fMaxSampleRadius >= 800.f)
		{
			Reset_Option();
		}
	}
}

HRESULT CMap_MindRoom_Static::Render()
{

	return S_OK;
}

HRESULT CMap_MindRoom_Static::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Static::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_MindRoom_Static::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_MindRoom_Static::Render_OutLine()
{
	return S_OK;
}

void CMap_MindRoom_Static::Execute_RadialSperad()
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

void CMap_MindRoom_Static::Reset_Option()
{
}

HRESULT CMap_MindRoom_Static::Ready_Components()
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

void CMap_MindRoom_Static::Using_Map_Tool()
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

_bool CMap_MindRoom_Static::Is_Sankaku(wstring Name, _bool IsTick)
{
	_bool bCheck = { false };
	std::wregex pattern(L"_number\\d+");
	wstring TempStr = regex_replace(Name, pattern, L"");

	if (TempStr == TEXT("Sankaku_A"))
	{
		bCheck = true;
	}

	return bCheck;
}

_float3 CMap_MindRoom_Static::Rendom_Pos()
{
	_float3 Value = { 0.f, 0.f, 0.f };

	_float3 LargeSphereRandomPos = { 0.f, 0.f, 0.f };
	_float3 SmallSphereRandomPos = { 0.f, 0.f, 0.f };
	_float LargeRadius = { 1000.f };
	_float SmallRadius = { 50.f };
	_float randomAngle = {0.f};
	_float randomElevation = { 0.f };
	_float randomLargeRadius = { 0.f };
	_float randomSmallRadius = { 0.f };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<_float> angleDist(0.0f, 360.f);
	std::uniform_real_distribution<_float> largeRadiusDist(0.0f, LargeRadius);
	std::uniform_real_distribution<_float> SmallRadiusDist(0.0f, SmallRadius);
	std::uniform_real_distribution<_float> xDist(-40.f, 40.f);
	std::uniform_real_distribution<_float> zDist(-40.f, 40.f);

	while (true)
	{
		randomAngle = angleDist(gen);
		randomElevation = angleDist(gen);
		randomLargeRadius = largeRadiusDist(gen);
		randomSmallRadius = SmallRadiusDist(gen);

		LargeSphereRandomPos.x = randomLargeRadius * std::cos(randomAngle) * std::sin(randomElevation);
		LargeSphereRandomPos.y = randomLargeRadius * std::sin(randomAngle) * std::sin(randomElevation);;
		LargeSphereRandomPos.z = randomLargeRadius * std::cos(randomElevation);

		SmallSphereRandomPos.x = (SmallRadius + randomSmallRadius) * std::cos(randomAngle) * std::sin(randomElevation);
		SmallSphereRandomPos.y = randomSmallRadius * std::sin(randomAngle) * std::sin(randomElevation);;
		SmallSphereRandomPos.z = (SmallRadius + randomSmallRadius) * std::cos(randomElevation) + zDist(gen);

		if (std::abs(LargeSphereRandomPos.x) <= 40.f && std::abs(LargeSphereRandomPos.z) <= 40.f)
		{
			LargeSphereRandomPos.x = xDist(gen);
			LargeSphereRandomPos.z = zDist(gen);
			continue;
		}

		if (std::abs(SmallSphereRandomPos.x) <= 40.f && std::abs(SmallSphereRandomPos.z) <= 40.f)
		{
			SmallSphereRandomPos.x = xDist(gen);
			SmallSphereRandomPos.z = zDist(gen);
			continue;
		}

		break;
	}

	if (randomLargeRadius >= SmallRadius + randomSmallRadius)
	{
		return Value = LargeSphereRandomPos;
	}
	else
	{
		return Value = SmallSphereRandomPos;
	}
}

void CMap_MindRoom_Static::Sankaku_Rising(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	if (m_fTimer >= 0.01f)
	{
		m_Translation.y += m_fSpeed;
		m_fTimer = 0.f;
	}

	if (m_Translation.y >= 130.f)
	{
		m_Translation.y = -25.f;
	}

	m_pTransform->Set_State(TRANSFORM::POSITION, m_Translation);
}

void CMap_MindRoom_Static::Add_ModelCom()
{
	if (m_iIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_SANKAKU_A;
	}
	if (m_iIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEA;
	}
	if (m_iIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEB;
	}
	if (m_iIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_STONEC;
	}
	if (m_iIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_MINDROOM_SPACESHIP;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
}


shared_ptr<CMap_MindRoom_Static> CMap_MindRoom_Static::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_MindRoom_Static> pInstance = make_private_shared(CMap_MindRoom_Static, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_MindRoom_Static::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_MindRoom_Static::Clone(any pArg)
{
	shared_ptr<CMap_MindRoom_Static> pInstance = make_private_shared_copy(CMap_MindRoom_Static, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_MindRoom_Static::Clone", "Failed to Initialize");
	}

	return pInstance;
}
