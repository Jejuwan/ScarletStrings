#include "ClientPCH.h"
#include "Map_Stage1_Dynamic.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"

#include "Monster.h"
#include "Psychokinesis.h"
#include <regex>

CMap_Stage1_Dynamic::CMap_Stage1_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_Stage1_Dynamic::CMap_Stage1_Dynamic(const CMap_Stage1_Dynamic& _rhs)
	: CMap_Dynamic(_rhs)
	, m_iDynamicIndex(_rhs.m_iDynamicIndex)
{
}

HRESULT CMap_Stage1_Dynamic::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iDynamicIndex = iIndex;
	Add_DissolveModel();

	return S_OK;
}

HRESULT CMap_Stage1_Dynamic::Initialize(any pArg)
{
	m_vAuraColor = float3(0.3f, 0.1f, 0.3f);
	m_fAuraBlendFactor = 0.3f;

	if (FAILED(__super::Initialize(pArg)))
	{
		MSG_RETURN(E_FAIL, "CMap_Stage1_Dynamic::Initialize", "Failed to CGameObject::Initialize");
	}

	Add_ModelCom();
	Create_Light(m_pTransform);

	std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
		any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

	m_copyTuplePivot = tObjTuple;

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
	if (std::get<0>(tObjTuple) == SCENE::TEST ||
		std::get<0>(tObjTuple) == SCENE::REALTEST)
	{
		m_strName = std::get<1>(tObjTuple);
		m_Scale = std::get<2>(tObjTuple);
		if (m_strName == TEXT("BoxA") || m_strName == TEXT("BoxB"))
		{
			m_Scale = _float3(1.8f,1.8f,1.8f);
		}
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

	_float4x4 mat = m_pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	PxTransform pose = PxTransform(PxIdentity);
	pose.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
	m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(m_vExtents.x, m_vExtents.y, m_vExtents.z), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));

	m_pActor->attachShape(*m_pShape);
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::CUTSCENE_MIZUHA)
		SetPhysX(false);
	else
		SetPhysX(true);

	PxSetGroup(*m_pActor, (_uint)ACTORTAG::INTERACTIVE);
	m_ActorDesc.strTag = ACTORTAG::INTERACTIVE;
	m_ActorDesc.bThrow = false;
	m_ActorDesc.bThrowHit = false;
	m_ActorDesc.stAttackDesc.iDamage = 10;
	m_pActor->userData = &m_ActorDesc;

	m_fMass *= 1.5f;
	m_fThrowForce = 150000.f;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	// Hide_Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_GROUND_CARC)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 2);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_GROUND_CARC)
		{
			m_pModel->Hide_Mesh(1);
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}

		m_tInstModelDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelDesc.iPassIndex = 15;
		m_tInstModelDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
			{
				if (m_bDissolve)
				{
					if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
					{
						return E_FAIL;
					}

					if (FAILED(pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
					{
						return E_FAIL;
					}
				}

				if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
				{
					MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
				}

				return S_OK;
			};
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_GROUND_CARC)
		{
			m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (m_bDissolve)
					{
						if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
						{
							return E_FAIL;
						}

						if (FAILED(pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
						{
							return E_FAIL;
						}
					}

					if (FAILED(pModel->Bind_ShaderResourceViews(1, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};

			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BOXB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_BOXA)
		{
			m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel> pModel, shared_ptr<CShader> pShader)
				{
					if (m_bDissolve)
					{
						if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
						{
							return E_FAIL;
						}

						if (FAILED(pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
						{
							return E_FAIL;
						}

					}
					if (FAILED(pModel->Bind_ShaderResourceViews(0, pShader)))
					{
						MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_Reflection(pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_Reflection");
					}
					_float4 vCamPosition = *(_float4*)CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).m[3];
					vCamPosition.w = 1.f;
					if (FAILED(pShader->Bind_RawValue(SHADER_CAMPOS, &vCamPosition, sizeof(_float4))))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_CamPosition");
					}

					if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), pShader)))
					{
						MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
					}

					return S_OK;
				};

			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(6);
		}
	}

	// Shadow
	{
		m_tInstModelShadowDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelShadowDesc.iPassIndex = 2;
	}

	// Outline
	{
		m_tInstModelOutlineDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelOutlineDesc.iPassIndex = 3;
		m_tInstModelOutlineDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
			{
				if (FAILED(pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
				{
					MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
				}

				if (FAILED(pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutlineColor, sizeof(_float3))))
				{
					MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
				}

				return S_OK;
			};
	}

	PxSetGroup(*m_pActor, (_uint)ACTORTAG::INTERACTIVE);
	m_ActorDesc.strTag = ACTORTAG::INTERACTIVE;
	m_ActorDesc.bThrow = false;
	m_ActorDesc.bThrowHit = false;
	m_ActorDesc.stAttackDesc.iDamage = 10;
	m_pActor->userData = &m_ActorDesc;

	SetPhysX(false);

	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if(CGameInstance::Get_Instance()->Current_Scene() != SCENE::CUTSCENE_MIZUHA)
	{
		if (m_bInitPhysX)
		{
			m_bInitPhysX = false;
			SetPhysX(true);
			Set_Gravity(true);
		}
		else if (m_bInitPhysXDelay)
		{
			m_fInitPhysXAccTime += fTimeDelta;
			if (m_fInitPhysXAccTime >= m_fInitPhysXDelay)
			{
				m_bInitPhysX = true;
				m_bInitPhysXDelay = false;
			}
		}
	}

	Duplicate_Model(fTimeDelta);

	// TODO: Must Delete
	// TODO: 이거 제발 지워 ㅜㅜ
//#ifdef _DEBUG
//#ifdef ACTIVATE_IMGUI
//	ImGui::Begin("Aura");
//
//	ImGui::SliderFloat3("Aura Color", &m_vAuraColor.x, 0.f, 1.f);
//	ImGui::SliderFloat("Aura Factor", &m_fAuraBlendFactor, 0.f, 1.f);
//
//	ImGui::End();
//#endif
//#endif
}

void CMap_Stage1_Dynamic::Late_Tick(_float fTimeDelta)
{
#if ACTIVATE_IMGUI
	static _bool bRender(true);
	if (bCheckOnce)
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			bCheckOnce = false;
			ImGui::Begin("Please");
			ImGui::Checkbox("CMap_Stage1_Dynamic", &bRender);
			ImGui::End();
		}
	if (!bRender) return;
#endif

	if (m_bDead)
		return;

	if (m_bDissolve || m_bAura)
	{
		m_fAuraTimeFactor += fTimeDelta * 0.5f;
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
	else
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);

		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);
	}

	//m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

	if (m_bFocused)
	{
		//Add_RenderObject(RENDER_GROUP::OUTLINE);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::OUTLINE, m_tInstModelOutlineDesc, m_pTransform);
		m_bFocused = false;
	}

	//if (CFrustom::Get_Instance()->Culling_Bounding_Sphere(m_pTransform->Get_State(TRANSFORM::POSITION), m_pModel->Get_FarestPoint()))
	//{
	//    Add_RenderObject(RENDER_GROUP::NONBLEND);
	//}
	//Add_RenderObject(RENDER_GROUP::SHADOW);

}

HRESULT CMap_Stage1_Dynamic::Render()
{
	if (m_bDissolve)
	{
		if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
		{
			return E_FAIL;
		}

		if (FAILED(m_pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
		{
			return E_FAIL;
		}

		//if (FAILED(CInteractiveObject::Render(16)))
		//{
		//    MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
		//}
	}
	/*else
	{
		if (FAILED(CInteractiveObject::Render(0)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
		}

		if (Car_Mesh(m_strIndexName))
		{
			Mesh_Index(m_strIndexName);

			if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
			}

			Mesh_Render(m_strIndexName);
		}
	}*/

	if (m_bAura)
	{
		if (FAILED(m_pAuraTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "texAura")))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_ShaderResourceView:Aura");
		}

		if (FAILED(m_pShader->Bind_RawValue("vAuraColor", &m_vAuraColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Color");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraBlendFactor", m_fAuraBlendFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Blend Factor");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraTimeFactor", m_fAuraTimeFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Time Factor");
		}
	}

	if (FAILED(CGameObject::Render(m_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	Mesh_Index(m_strIndexName);
	if (FAILED(m_pModel->Bind_ShaderResourceViews(m_iMeshIndex, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	Mesh_Render(m_strIndexName);

	return S_OK;
}

HRESULT CMap_Stage1_Dynamic::Render_OutLine()
{
	return __super::Render_OutLine();
}

HRESULT CMap_Stage1_Dynamic::Ready_Components()
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

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pDissolveTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002));
	if (nullptr == m_pDissolveTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

void CMap_Stage1_Dynamic::Using_Map_Tool()
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

void CMap_Stage1_Dynamic::Add_ModelCom()
{
	if (m_iDynamicIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BARRELA;
		m_vExtents = { 1.3f,1.f,1.3f };
		m_vLocalPose = { 0,0.9f,0 };
		m_fMass = 3.f;
	}
	else if (m_iDynamicIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BARRELB;
		m_vExtents = { 1.3f,1.f,1.3f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 3.f;
	}
	else if (m_iDynamicIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_DUMPSTER;
		m_vExtents = { 1.5f,1.5f,2.6f };
		m_vLocalPose = { -0.2f,1.2f,0 };
		m_fMass = 1.f;
	}
	else if (m_iDynamicIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOXA;
		m_vExtents = { 1.f,1.f,1.f };
		m_vLocalPose = { 0,0.9f,0 };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BOXB;
		m_vExtents = { 1.1f,1.9f,1.1f };
		m_vLocalPose = { 0,1.9f,0 };
		m_fMass = 5.f;
	}
	else if (m_iDynamicIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA;
		m_vExtents = { 0.7f,1.f,0.8f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB;
		m_vExtents = { 1.f,0.5f,1.2f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA;
		m_vExtents = { 1.,1.1,1.f };
		m_vLocalPose = { -0.7f,1.4f,0.2f };
		m_fMass = 5.f;
	}
	else if (m_iDynamicIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB;
		m_vExtents = { 2.f,2.f,1.4f };
		m_vLocalPose = { 0,1.f,0.f };
		m_fMass = 4.f;
	}
	else if (m_iDynamicIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC;
		m_vExtents = { 1.6,1.8f,2.7f };
		m_vLocalPose = { 0,0.f,-2.5f };
		m_fMass = 1.f;
	}
	else if (m_iDynamicIndex == 10)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED;
		m_vExtents = { 0.8,0.7,2.3 };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 1.f;
	}
	else if (m_iDynamicIndex == 11)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SPOOLA;
		m_vExtents = { 0.8,0.7,2.3 };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 1.f;
	}
	else if (m_iDynamicIndex == 13)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE;
		m_vExtents = { 1.2f,0.7,1.2f };
		m_vLocalPose = { -0.6f,0.8,-0.6f };
		m_fMass = 3.f;
	}
	else if (m_iDynamicIndex == 14)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CABLEREEL;
		m_vExtents = { 1.3f,1.6f,1.3f };
		m_vLocalPose = { -0.8f,1.6f,-1.8f };
		m_fMass = 1.5f;
	}
	else if (m_iDynamicIndex == 15)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_ICEBOX;
		m_vExtents = { 0.8,0.4f,0.5f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 16)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_GROUND_CARC;
		m_vExtents = { 0.8,0.7,2.3 };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 1.f;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);
}

void CMap_Stage1_Dynamic::Add_DissolveModel()
{
	if (m_iDynamicIndex == 0)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BARRELA, g_aNull);
	}
	else if (m_iDynamicIndex == 1)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BARRELB, g_aNull);
	}
	else if (m_iDynamicIndex == 2)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_DUMPSTER, g_aNull);
	}
	else if (m_iDynamicIndex == 3)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BOXA, g_aNull);
	}
	else if (m_iDynamicIndex == 4)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BOXB, g_aNull);
	}
	else if (m_iDynamicIndex == 5)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGA, g_aNull);
	}
	else if (m_iDynamicIndex == 6)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEBAGB, g_aNull);
	}
	else if (m_iDynamicIndex == 7)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEA, g_aNull);
	}
	else if (m_iDynamicIndex == 8)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEB, g_aNull);
	}
	else if (m_iDynamicIndex == 9)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPEC, g_aNull);
	}
	else if (m_iDynamicIndex == 10)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEPIPED, g_aNull);
	}
	else if (m_iDynamicIndex == 11)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPOOLA, g_aNull);
	}
	else if (m_iDynamicIndex == 13)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_BRICK_PALETTE, g_aNull);
	}
	else if (m_iDynamicIndex == 14)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CABLEREEL, g_aNull);
	}
	else if (m_iDynamicIndex == 15)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_ICEBOX, g_aNull);
	}
	else if (m_iDynamicIndex == 16)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_GROUND_CARC, g_aNull);
	}
}

void CMap_Stage1_Dynamic::Duplicate_Model(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_bReadyDuplicate)
	{
		m_fDuplicateTime -= fTimeDelta;
		if (m_fDuplicateTime < 0.f)
		{
			m_bReadyDuplicate = false;
			shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
			wstring strPrototype;
			if (m_strName == TEXT("BarrelA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_BARRELA;
			else if (m_strName == TEXT("BarrelB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_BARRELB;
			else if (m_strName == TEXT("Dumpster"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_DUMPSTER;
			else if (m_strName == TEXT("BoxA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_BOXA;
			else if (m_strName == TEXT("BoxB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_BOXB;
			else if (m_strName == TEXT("ConcreteBagA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGA;
			else if (m_strName == TEXT("ConcreteBagB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBAGB;
			else if (m_strName == TEXT("ConcretePipeA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEA;
			else if (m_strName == TEXT("ConcretePipeB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEB;
			else if (m_strName == TEXT("ConcretePipeC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPEC;
			else if (m_strName == TEXT("ConcretePipeD"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEPIPED;
			else if (m_strName == TEXT("SpoolA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SPOOLA;
			else if (m_strName == TEXT("Brick_Palette"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_BRICK_PALETTE;
			else if (m_strName == TEXT("CableReel"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CABLEREEL;
			else if (m_strName == TEXT("IceBox"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_ICEBOX;
			m_pDuplicatedObject = static_pointer_cast<CMap_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), strPrototype, m_copyTuplePivot));
			pLayer->Add(m_pDuplicatedObject);
			if (!m_bStrong)
				m_pDuplicatedObject->Throw();
			else
			{
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_DuplicatedObject(m_pDuplicatedObject);
				m_pDuplicatedObject->SetPhysX(false);
			}
			Interactive_PhychoKinesis(true);
			m_pDuplicatedObject->Interactive_PhychoKinesis(true);
		}
	}
}

void CMap_Stage1_Dynamic::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		m_pModel->Hide_Mesh(2);
	}

	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		m_pModel->Hide_Mesh(1);
	}
	/*if (m_strIndexName == TEXT("BoxB"))
	{
		m_pModel->Hide_Mesh(0);
	}*/
}

HRESULT CMap_Stage1_Dynamic::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	/* 창문 */
	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(2, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		//m_pModel->Render(2, m_pShader, 5);
	}

	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		if (FAILED(m_pModel->Bind_ShaderResourceViews(1, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(1, m_pShader, 6);
	}

	if (m_strIndexName == TEXT("BoxB") || m_strIndexName == TEXT("BoxA"))
	{

		if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
		}
		m_pModel->Render(0, m_pShader, 6);
	}

	return S_OK;
}

void CMap_Stage1_Dynamic::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		m_iMeshIndex = 2;
	}
	if (m_strIndexName == TEXT("Ground_CarC"))
	{
		m_iMeshIndex = 1;
	}
	/*if (m_strIndexName == TEXT("BoxB"))
	{
		m_iMeshIndex = 0;
	}*/
}

_bool CMap_Stage1_Dynamic::Car_Mesh(wstring MeshName)
{
	_bool bCheck = { false };

	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (MeshName == TEXT("Ground_CarC"))
		bCheck = true;
	else if (MeshName == TEXT("BoxA"))
		bCheck = true;
	else if (MeshName == TEXT("BoxB"))
		bCheck = true;

	return bCheck;
}


shared_ptr<CMap_Stage1_Dynamic> CMap_Stage1_Dynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Stage1_Dynamic> pInstance = make_private_shared(CMap_Stage1_Dynamic, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Dynamic::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_Dynamic::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_Dynamic> pInstance = make_private_shared_copy(CMap_Stage1_Dynamic, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_Dynamic::Clone", "Failed to Initialize");
	}

	return pInstance;
}
