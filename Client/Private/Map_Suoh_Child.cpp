#include "ClientPCH.h"
#include "Map_Suoh_Child.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include <regex>

CMap_Suoh_Child::CMap_Suoh_Child(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_Suoh_Child::CMap_Suoh_Child(const CMap_Suoh_Child& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_Suoh_Child::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SUOH_NPC_CHILD, g_aNull);

	return S_OK;
}

HRESULT CMap_Suoh_Child::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Child::Initialize", "Failed to CGameObject::Initialize");
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

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Reference"));

	m_ePointPos = POINTPOS::POS_FIRST;

	m_ChildPos.CreatePos = m_Translation;

	Input_PointPos();

	m_pModel->Set_Animation(3, 1.f, false, 0.1f, false);

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Suoh_Child::Tick(_float fTimeDelta)
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
			m_eChildState = CHILDSTATE::STATE_WALK;
		}
		if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_eChildState = CHILDSTATE::STATE_IDLE;
		}
		if (CGameInstance::Get_Instance()->Key_Down('R'))
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_Translation);
		}*/
	}

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Child Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##Child" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##Child" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##Child" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##Child" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##Child" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
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

void CMap_Suoh_Child::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Suoh_Child", &bRender);
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

}

HRESULT CMap_Suoh_Child::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Child::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Child::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Suoh_Child::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Suoh_Child::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Suoh_Child::Ready_Components()
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

void CMap_Suoh_Child::Using_Map_Tool()
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

void CMap_Suoh_Child::State_Animation(_float fTimeDelta)
{
	Go_Point();

	switch (m_eChildState)
	{
	case CHILDSTATE::STATE_IDLE:
		m_pModel->Set_Animation(1, 1.f, false, 0.1f, true);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 0.f);
		break;
	case CHILDSTATE::STATE_WALK:
		m_pModel->Set_Animation(2, 2.5f, false, 0.1f, true);
		m_pTransform->Set_State(TRANSFORM::LOOK, m_Dir);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 1.f);
		break;
	case CHILDSTATE::STATE_RUN:
		m_pModel->Set_Animation(3, 3.f, false, 0.1f, true);
		m_pTransform->Set_State(TRANSFORM::LOOK, m_Dir);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 2.f);
		break;
	default:
		break;
	}
}

void CMap_Suoh_Child::Go_Point()
{
	_float3 tmpDistance = _float3(0.f, 0.f, 0.f);
	_float3 CurrentPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	if (m_eRoundTrip == ROUNDTRIP::ROUNDTRIP_SECOND)
	{
		switch (m_ePointPos)
		{
		case POINTPOS::POS_FIRST:
			m_Dir = m_ChildPos.Pos[0] - m_ChildPos.CreatePos;
			m_Dir.y = 0.f;

			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SECOND;
			}
			break;
		case POINTPOS::POS_SECOND:
			m_Dir = m_ChildPos.CreatePos - m_ChildPos.Pos[0];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.CreatePos.x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.CreatePos.z);
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
			m_Dir = m_ChildPos.Pos[0] - m_ChildPos.CreatePos;
			m_Dir.y = 0.f;

			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SECOND;
			}
			break;
		case POINTPOS::POS_SECOND:
			m_Dir = m_ChildPos.Pos[1] - m_ChildPos.Pos[0];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[1].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[1].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_THIRD;
			}
			break;
		case POINTPOS::POS_THIRD:
			m_Dir = m_ChildPos.Pos[2] - m_ChildPos.Pos[1];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[2].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[2].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FORTH;
			}
			break;
		case POINTPOS::POS_FORTH:
			m_Dir = m_ChildPos.Pos[1] - m_ChildPos.Pos[2];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[1].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[1].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_FIFTH;
			}
			break;
		case POINTPOS::POS_FIFTH:
			m_Dir = m_ChildPos.Pos[0] - m_ChildPos.Pos[1];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.Pos[0].x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.Pos[0].z);
			if (tmpDistance.x <= 1.f && tmpDistance.z <= 1.f)
			{
				m_ePointPos = POINTPOS::POS_SIXTH;
			}
			break;
		case POINTPOS::POS_SIXTH:
			m_Dir = m_ChildPos.CreatePos - m_ChildPos.Pos[0];
			m_Dir.y = 0.f;
			tmpDistance.x = abs(CurrentPos.x - m_ChildPos.CreatePos.x);
			tmpDistance.z = abs(CurrentPos.z - m_ChildPos.CreatePos.z);
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

void CMap_Suoh_Child::Input_PointPos()
{
	if (m_strNameNum == TEXT("Suoh_NPC_Child_number0"))
	{
		m_ChildPos.Pos[0] = _float3(-13.f, 0.f, 256.f);
		m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_SECOND;
		m_eChildState = CHILDSTATE::STATE_WALK;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Child_number1"))
	{
		m_ChildPos.Pos[0] = _float3(13.f, 0.f, 301.f);
		m_ChildPos.Pos[1] = _float3(5.f, 0.f, 330.f);
		m_ChildPos.Pos[2] = _float3(-28.f, 0.f, 396.f);
		m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_SIXTH;
		m_eChildState = CHILDSTATE::STATE_WALK;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Child_number2"))
	{
		m_ChildPos.Pos[0] = _float3(-118.f, 0.f, 400.f);
		m_ChildPos.Pos[1] = _float3(-106.f, 0.f, 380.f);
		m_ChildPos.Pos[2] = _float3(-79.f, 0.f, 398.f);
		m_eRoundTrip = ROUNDTRIP::ROUNDTRIP_SIXTH;
		m_eChildState = CHILDSTATE::STATE_RUN;
	}
	if (m_strNameNum == TEXT("Suoh_NPC_Child_number3") ||
		m_strNameNum == TEXT("Suoh_NPC_Child_number4") ||
		m_strNameNum == TEXT("Suoh_NPC_Child_number5") ||
		m_strNameNum == TEXT("Suoh_NPC_Child_number6") ||
		m_strNameNum == TEXT("Suoh_NPC_Child_number7"))
	{
		m_eChildState = CHILDSTATE::STATE_IDLE;
	}
}

shared_ptr<CMap_Suoh_Child> CMap_Suoh_Child::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Suoh_Child> pInstance = make_private_shared(CMap_Suoh_Child, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Child::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Suoh_Child::Clone(any pArg)
{
	shared_ptr<CMap_Suoh_Child> pInstance = make_private_shared_copy(CMap_Suoh_Child, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Suoh_Child::Clone", "Failed to Initialize");
	}

	return pInstance;
}
