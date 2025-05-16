#include "ClientPCH.h"
#include "Map_Tutorial_Dynamic.h"
#include "GameInstance.h"
#include "Light.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"

#include "Monster.h"
#include "Psychokinesis.h"
#include <regex>

#include "Bone.h"
#include "Monster_DispenPerry.h"
#include "Monster.h"
#include "Map_Dynamic.h"

#include "Effect_Manager.h"

CMap_Tutorial_Dynamic::CMap_Tutorial_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Dynamic(_pDevice, _pContext)
{
}

CMap_Tutorial_Dynamic::CMap_Tutorial_Dynamic(const CMap_Tutorial_Dynamic& _rhs)
	: CMap_Dynamic(_rhs)
	, m_iDynamicIndex(_rhs.m_iDynamicIndex)
{
}

HRESULT CMap_Tutorial_Dynamic::Initialize_Prototype(_uint iIndex)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL_RENDERER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	m_iDynamicIndex = iIndex;

	Add_DissolveModel();

	return S_OK;
}

HRESULT CMap_Tutorial_Dynamic::Initialize(any pArg)
{
	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
	m_tMaterialDesc.fShininess = 0.1f;

	if (FAILED(__super::Initialize(pArg)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
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
	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)
	{
		m_vPos = m_Translation;
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

	SetPhysX(true);

	m_fMass *= 1.5f;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	m_strLayerName = LAYER_INTERACTIVE;

	// Hide_Mesh
	{
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARA ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARC)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 2);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARD)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}

		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARD)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 0);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARC)
		{
			m_pModelRenderer->Hide_Mesh(m_wstrModelTag, 1);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARA)
		{
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
	}

	// Mesh Render
	{
		m_tInstModelMeshDesc.wstrModelTag = m_wstrModelTag;
		m_tInstModelMeshDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
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

		/* 창문 */
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARA ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARC)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(2);
			m_tInstModelMeshDesc.vecPassIndex.push_back(8);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARD)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(8);
		}

		/* 뼈대 */
		if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARB ||
			m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARD)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(0);
			m_tInstModelMeshDesc.vecPassIndex.push_back(15);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARC)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(15);
		}
		else if (m_wstrModelTag == PROTOTYPE_COMPONENT_MODEL_CARA)
		{
			m_tInstModelMeshDesc.vecMeshIndex.push_back(1);
			m_tInstModelMeshDesc.vecPassIndex.push_back(15);
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

void CMap_Tutorial_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

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

	Duplicate_Model(fTimeDelta);

	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)
	{
		if (m_pTransform->Get_State(TRANSFORM::POSITION).y < 2.f)
		{
			_float3 tmp = m_pTransform->Get_State(TRANSFORM::POSITION);
			tmp.y = 2.f;
			m_pTransform->Set_State(TRANSFORM::POSITION, tmp);
		}
	}
}

void CMap_Tutorial_Dynamic::Late_Tick(_float fTimeDelta)
{
	//Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::SHADOW);
	if (m_bDissolve || m_bAura)
	{
		m_fAuraTimeFactor += fTimeDelta * 0.5f;
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
	else
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelDesc, m_pTransform);
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstModelMeshDesc, m_pTransform);
	}
	
	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::SHADOW, m_tInstModelShadowDesc, m_pTransform);

	if (m_bFocused)
	{
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::OUTLINE, m_tInstModelOutlineDesc, m_pTransform);
		m_bFocused = false;
	}
}

HRESULT CMap_Tutorial_Dynamic::Render()
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
	}

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

HRESULT CMap_Tutorial_Dynamic::Render_PBR()
{

	
	return S_OK;
}

HRESULT CMap_Tutorial_Dynamic::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	//if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	//}

	return S_OK;
}

HRESULT CMap_Tutorial_Dynamic::Render_OutLine()
{

	return S_OK;
}

HRESULT CMap_Tutorial_Dynamic::Ready_Components()
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

void CMap_Tutorial_Dynamic::Duplicate_Model(_float fTimeDelta)
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
			if (m_strName == TEXT("CarA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARA;
			else if (m_strName == TEXT("CarB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARB;
			else if (m_strName == TEXT("CarC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARC;
			else if (m_strName == TEXT("CarD"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARD;
			else if (m_strName == TEXT("CarD_Brk"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk;
			else if (m_strName == TEXT("ConcreteBarrierA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA;
			else if (m_strName == TEXT("ConcreteBarrierB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB;
			else if (m_strName == TEXT("SeiranCar"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR;
			else if (m_strName == TEXT("SeiranCar_Brk"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK;
			else if (m_strName == TEXT("Tractor"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TRACTOR;
			m_pDuplicatedObject = static_pointer_cast<CMap_Tutorial_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), strPrototype, m_copyTuplePivot));
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

void CMap_Tutorial_Dynamic::Add_DissolveModel()
{
	if (m_iDynamicIndex == 0)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CARA, g_aNull);
	}
	else if (m_iDynamicIndex == 1)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CARB, g_aNull);
	}
	else if (m_iDynamicIndex == 2)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CARC, g_aNull); // small truck
	}
	else if (m_iDynamicIndex == 3)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CARD, g_aNull); //camping car
	}
	else if (m_iDynamicIndex == 4)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CARD_Brk, g_aNull);
	}
	else if (m_iDynamicIndex == 5)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA, g_aNull);
	}
	else if (m_iDynamicIndex == 6)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB, g_aNull);
	}
	else if (m_iDynamicIndex == 7)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SEIRANCAR, g_aNull);
	}
	else if (m_iDynamicIndex == 8)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SEIRANCAR_BRK, g_aNull);
	}
	else if (m_iDynamicIndex == 9)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_TRACTOR, g_aNull);
	}
}

void CMap_Tutorial_Dynamic::Add_ModelCom()
{
	if (m_iDynamicIndex == 0)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CARA;
		m_vExtents = { 1.1f,1.f,2.7f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 1)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CARB;
		m_vExtents = { 0.8f,0.7f,2.f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 2)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CARC; // small truck
		m_vExtents = { 0.8f,0.8f,1.9f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 6.5f;
	}
	else if (m_iDynamicIndex == 3)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CARD; //camping car
		m_vExtents = { 1.2f,1.4f,2.2f };
		m_vLocalPose = { 0.f,1.5f,0.3f };
		m_fMass = 2.f;
	}
	else if (m_iDynamicIndex == 4)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CARD_Brk;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 5)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERA;
		m_vExtents = { 0.8f, 0.45f, 0.2f };
		m_vLocalPose = { 0.f,0.5f,0.f };
		m_fMass = 100.f;
	}
	else if (m_iDynamicIndex == 6)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_CONCRETEBARRIERB;
		m_vExtents = { 0.8f, 0.45f, 0.2f };
		m_vLocalPose = { 0.f,0.5f,0.f };
		m_fMass = 100.f;
	}
	else if (m_iDynamicIndex == 7)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SEIRANCAR;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 8)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_SEIRANCAR_BRK;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else if (m_iDynamicIndex == 9)
	{
		m_wstrModelTag = PROTOTYPE_COMPONENT_MODEL_TRACTOR;
		m_vExtents = { 0.8f,0.7f,2.3f };
		m_vLocalPose = { 0.f,0.8f,0.f };
		m_fMass = 7.f;
	}
	else
	{
		int a = 0;
	}

	m_pTransform = m_pModelRenderer->Add_Instance(m_wstrModelTag);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);
}

void CMap_Tutorial_Dynamic::Hide_Mesh(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	if (m_strIndexName == TEXT("CarA") || m_strIndexName == TEXT("CarC"))
	{
		m_pModel->Hide_Mesh(2);
	}
	else if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_pModel->Hide_Mesh(1);
	}

	if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_pModel->Hide_Mesh(0);
	}
	else if (m_strIndexName == TEXT("CarC"))
	{
		m_pModel->Hide_Mesh(1);
	}
	else if (m_strIndexName == TEXT("CarA"))
	{
		m_pModel->Hide_Mesh(1);
	}
}

HRESULT CMap_Tutorial_Dynamic::Mesh_Render(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	/* 창문 */
	if (m_strIndexName == TEXT("CarA") || m_strIndexName == TEXT("CarC"))
	{
		m_pModel->Render(2, m_pShader, m_iGlassPassIndex);
	}
	else if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_pModel->Render(1, m_pShader, m_iGlassPassIndex);
	}

	/* 뼈대 */
	if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_pModel->Render(0, m_pShader, m_iPassIndex);
	}
	else if (m_strIndexName == TEXT("CarC"))
	{
		m_pModel->Render(1, m_pShader, m_iPassIndex);
	}
	else if (m_strIndexName == TEXT("CarA"))
	{
		m_pModel->Render(1, m_pShader, m_iPassIndex);
	}

	return S_OK;
}

void CMap_Tutorial_Dynamic::Mesh_Index(wstring MeshName)
{
	std::wregex pattern(L"_number\\d+");
	m_strIndexName = regex_replace(MeshName, pattern, L"");

	//네온 매쉬 안그림
	if (m_strIndexName == TEXT("CarA") || m_strIndexName == TEXT("CarC"))
	{
		m_iMeshIndex = 2;
	}
	else if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_iMeshIndex = 1;
	}

	if (m_strIndexName == TEXT("CarB") || m_strIndexName == TEXT("CarD"))
	{
		m_iMeshIndex = 0;
	}
	else if (m_strIndexName == TEXT("CarC"))
	{
		m_iMeshIndex = 1;
	}
	else if (m_strIndexName == TEXT("CarA"))
	{
		m_iMeshIndex = 1;
	}
}

shared_ptr<CMap_Tutorial_Dynamic> CMap_Tutorial_Dynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Tutorial_Dynamic> pInstance = make_private_shared(CMap_Tutorial_Dynamic, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(iIndex)))
	{
		MSG_RETURN(nullptr, "CMap_Tutorial_Dynamic::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Tutorial_Dynamic::Clone(any pArg)
{
	shared_ptr<CMap_Tutorial_Dynamic> pInstance = make_private_shared_copy(CMap_Tutorial_Dynamic, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Tutorial_Dynamic::Clone", "Failed to Initialize");
	}

	return pInstance;
}
