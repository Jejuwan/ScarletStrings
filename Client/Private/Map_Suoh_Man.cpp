#include "ClientPCH.h"
#include "Map_Suoh_Man.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"
#include "NPC_Part.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Suoh_Man::CMap_Suoh_Man(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Suoh_Man::CMap_Suoh_Man(const CMap_Suoh_Man& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_Suoh_Man::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_MAN, g_aNull);

	return S_OK;
}

HRESULT CMap_Suoh_Man::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Man::Initialize", "Failed to CGameObject::Initialize");
	}

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
	if (std::get<0>(tObjTuple) == SCENE::SUOH)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		m_Rotation = std::get<3>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_strNameNum = std::get<6>(tObjTuple);
		m_pTransform->Set_Matrix(m_Mat);
		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);
	}
	
	Ready_Parts();
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));

	m_ePointPos = POINTPOS::POS_FIRST;

	m_ManPos.CreatePos = m_Translation;

	Input_Pos();

	m_pModel->Set_Animation(3, 1.f, false, 0.1f, false);

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_Man::Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	bCheckOnce = true;
#endif
	if (m_bDead)
		return;

	State_Animation(fTimeDelta);

	m_pModel->Tick_Animation(fTimeDelta);
	
	if (m_bSceneMapTool)
	{
		Using_Map_Tool();

		/*if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
		{
			m_eMANSTATE = MANSTATE::STATE_WALK;
		}
		if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_eMANSTATE = MANSTATE::STATE_IDLE;
		}
		if (CGameInstance::Get_Instance()->Key_Down('R'))
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_Translation);
		}*/
	}

	for (auto& pPart : m_pParts)
		pPart->Tick(fTimeDelta);
	

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Man Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##Man" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##Man" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##Man" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##Man" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##Man" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();
}

void CMap_Suoh_Man::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Suoh_Man", &bRender);
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

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);

	__super::Late_Tick(fTimeDelta);

	for (auto& pPart : m_pParts)
		pPart->Late_Tick(fTimeDelta);
}

HRESULT CMap_Suoh_Man::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}
	
	return S_OK;
}

HRESULT CMap_Suoh_Man::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Man::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Man::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Man::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_Man::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
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

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: RENDERER");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

HRESULT CMap_Suoh_Man::Ready_Parts()
{
	return S_OK;
}

void CMap_Suoh_Man::Using_Map_Tool()
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

void CMap_Suoh_Man::State_Animation(_float fTimeDelta)
{
	Go_Point();

	switch (m_eManState)
	{
	case MANSTATE::STATE_IDLE :
		m_pModel->Set_Animation(3, 1.f, false, 0.1f, true);
		break;
	case MANSTATE::STATE_WALK:
		m_pModel->Set_Animation(4, 2.5f, false, 0.1f, true);
		m_pTransform->Set_State(TRANSFORM::LOOK, m_Dir);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * m_fSpeed);
		break;
	case MANSTATE::STATE_RUN:
		m_pModel->Set_Animation(5, 3.f, false, 0.1f, true);
		m_pTransform->Set_State(TRANSFORM::LOOK, m_Dir);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * m_fSpeed);
		break;
	case MANSTATE::STATE_TALK:
		m_pModel->Set_Animation(26, 3.f, false, 0.1f, true);
		break;
	default:
		break;
	}
}

void CMap_Suoh_Man::Go_Point()
{
	_float3 tmpDistance = _float3(0.f, 0.f, 0.f);
	_float3 CurrentPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	if (m_eRoundTrip == ROUNDTRIP::ROUNDTRIP_SECOND)
	{
		switch (m_ePointPos)
		{
		case POINTPOS::POS_FIRST:
			m_Dir = m_ManPos.Pos[0] - m_ManPos.CreatePos;
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SECOND;
			}
			break;
		case POINTPOS::POS_SECOND:
			m_Dir = m_ManPos.CreatePos - m_ManPos.Pos[0];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.CreatePos.x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.CreatePos.z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FIRST;
			}
			break;
		default:
			break;
		}
	}
	else if (m_eRoundTrip == ROUNDTRIP::ROUNDTRIP_SIXTH)
	{
		switch (m_ePointPos)
		{
		case POINTPOS::POS_FIRST:
			m_Dir = m_ManPos.Pos[0] - m_ManPos.CreatePos;
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SECOND;
			}
			break;
		case POINTPOS::POS_SECOND:
			m_Dir = m_ManPos.Pos[1] - m_ManPos.Pos[0];
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[1].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[1].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_THIRD;
			}
			break;
		case POINTPOS::POS_THIRD:
			m_Dir = m_ManPos.Pos[2] - m_ManPos.Pos[1];
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[2].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[2].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FORTH;
			}
			break;
		case POINTPOS::POS_FORTH:
			m_Dir = m_ManPos.Pos[1] - m_ManPos.Pos[2];
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[1].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[1].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FIFTH;
			}
			break;
		case POINTPOS::POS_FIFTH:
			m_Dir = m_ManPos.Pos[0] - m_ManPos.Pos[1];
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SIXTH;
			}
			break;
		case POINTPOS::POS_SIXTH :
			m_Dir = m_ManPos.CreatePos - m_ManPos.Pos[0];
			tmpDistance.x = abs(CurrentPos.x - m_ManPos.CreatePos.x);
			tmpDistance.z = abs(CurrentPos.z - m_ManPos.CreatePos.z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FIRST;
			}
			break;
		default:
			break;
		}
	}
}

void CMap_Suoh_Man::Input_Pos()
{
	if (m_strNameNum == TEXT("Suoh_NPC_Man_number0") ||
		m_strNameNum == TEXT("Suoh_NPC_Man_number1") ||
		m_strNameNum == TEXT("Suoh_NPC_Man_number4") ||
		m_strNameNum == TEXT("Suoh_NPC_Man_number5") ||
		m_strNameNum == TEXT("Suoh_NPC_Man_number6"))
	{
		m_eManState = MANSTATE::STATE_IDLE;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Man_number3"))
	{
		m_eManState = MANSTATE::STATE_TALK;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Man_number2"))
	{
		m_ManPos.Pos[0] = _float3(-12.35f, 0.f, 135.f);
		m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_SECOND;
		m_eManState = MANSTATE::STATE_WALK;
		m_fSpeed = 1.f;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Man_number7"))
	{
		m_ManPos.Pos[0] = _float3(18.3f, 0.f, 175.f);
		m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_SECOND;
		m_eManState = MANSTATE::STATE_WALK;
		m_fSpeed = 1.f;
	}
}

shared_ptr<CMap_Suoh_Man> CMap_Suoh_Man::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_Man> pInstance = make_private_shared(CMap_Suoh_Man, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Man::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Man::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_Man> pInstance = make_private_shared_copy(CMap_Suoh_Man, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Man::Clone", "Failed to Initialize");
	}

	return pInstance;
}
