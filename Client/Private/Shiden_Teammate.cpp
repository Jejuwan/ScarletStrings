#include "ClientPCH.h"
#include "Shiden_Teammate.h"

#include "GameInstance.h"
#include "PhysXEngine.h"

#include "TeammateAI.h"
#include "ShidenAI.h"

#include "Shiden_Weapon.h"
#include "Bone.h"

CShiden_Teammate::CShiden_Teammate(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CTeammate(pDevice, pContext)
{
}

CShiden_Teammate::CShiden_Teammate(const CShiden_Teammate& rhs)
	: CTeammate(rhs)
{
}

HRESULT CShiden_Teammate::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SHIDEN_TEAMMATE, g_aNull);

	return S_OK;
}

HRESULT CShiden_Teammate::Initialize(any vInitPosition)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CArashi::Initialize", "Failed to CGameObject::Initialize");
	}

	// Hide Mask && Hood
	//m_pModel->Hide_Mesh(4);
	//m_pModel->Hide_Mesh(5);
	//m_pModel->Hide_Mesh(6);
	m_vecMeshPassIndexPair.push_back({ 1, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 2, 17 }); // AMB
	m_vecMeshPassIndexPair.push_back({ 3, 17 }); // AMB

	m_vecMeshPassIndexPair.push_back({ 0, 0 });
	m_vecMeshPassIndexPair.push_back({ 7, 0 });
	m_vecMeshPassIndexPair.push_back({ 8, 0 });

	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff03"));

	// Init Entity
	m_tEntityDesc.fHP = 20.f;
	m_tEntityDesc.fMaxHP = 20.f;

	auto pGameInstance = CGameInstance::Get_Instance();

	_float4 vPosition = any_cast<_float4>(vInitPosition);
	m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);

	// Init Px Settings
	{
		PxCapsuleControllerDesc cDesc;
		cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
		cDesc.position = { vPosition.x, vPosition.y, vPosition.z };
		cDesc.height = 1.f;
		cDesc.radius = 0.25f;
		cDesc.slopeLimit = 0.f;
		cDesc.contactOffset = 0.01f;
		cDesc.stepOffset = 0.0003f;
		cDesc.reportCallback = NULL;

		m_pController = pGameInstance->CreateController(&cDesc);
		m_ActorDesc.strTag = ACTORTAG::TEAMMATE;
		m_pController->getActor()->userData = &m_ActorDesc;

		//PxShape* pShape;
		//m_pController->getActor()->getShapes(&pShape, 1);
		//PxFilterData pFilterData;
		//pFilterData.word0 = (_uint)ACTORTAG::PLAYER;
		//pShape->setQueryFilterData(pFilterData);
	}

	m_pFSM = CShidenAI::Create(m_pDevice, m_pContext, shared_from_gameobject(), &m_tEntityDesc, &m_ActorDesc);

	m_pLeftWeapon = dynamic_pointer_cast<CShiden_Weapon>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_SHIDEN_WEAPON));
	if (nullptr == m_pLeftWeapon)
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Initialize", "Failed to Clone_GameObject");
	}
	m_pRightWeapon = dynamic_pointer_cast<CShiden_Weapon>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_SHIDEN_WEAPON));
	if (nullptr == m_pRightWeapon)
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Initialize", "Failed to Clone_GameObject");
	}

	m_pLeftWeaponCombineBone = m_pModel->Get_Bone("LeftWeapon");
	m_pLeftWeaponTransform = dynamic_pointer_cast<CTransform>(m_pLeftWeapon->Get_Component(COMPONENT::TRANSFORM));

	m_pRightWeaponCombineBone = m_pModel->Get_Bone("RightWeapon");
	m_pRightWeaponTransform = dynamic_pointer_cast<CTransform>(m_pRightWeapon->Get_Component(COMPONENT::TRANSFORM));

	m_matWeaponPivot = XMMatrixRotationY(XMConvertToRadians(180.f))* XMMatrixRotationX(XMConvertToRadians(90.f));

	return S_OK;
}

void CShiden_Teammate::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pFSM && m_bEnableFSM)
	{
		m_pFSM->Tick(fTimeDelta);
	}

	if (m_bEnableWeapon)
	{
		_matrix PivotMatrix = m_pModel->Get_Pivot();

		// Left
		_matrix matLeftCombineBone = m_pLeftWeaponCombineBone->Get_CombinedTransformation();
		_float4x4 matLeftWeapon = m_matWeaponPivot * matLeftCombineBone * PivotMatrix * m_pTransform->Get_Matrix();

		m_pLeftWeaponTransform->Set_Matrix(matLeftWeapon);

		m_pLeftWeapon->Tick(fTimeDelta);

		// Right
		_matrix matRightCombineBone = m_pRightWeaponCombineBone->Get_CombinedTransformation();
		_float4x4 matRightWeapon = m_matWeaponPivot * matRightCombineBone * PivotMatrix * m_pTransform->Get_Matrix();

		m_pRightWeaponTransform->Set_Matrix(matRightWeapon);

		m_pRightWeapon->Tick(fTimeDelta);
	}

	m_pModel->Tick_Animation(fTimeDelta);

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Shiden Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
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

void CShiden_Teammate::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);

	if (m_bEnableWeapon)
	{
		m_pLeftWeapon->Late_Tick(fTimeDelta);
		m_pRightWeapon->Late_Tick(fTimeDelta);
	}
}

HRESULT CShiden_Teammate::Render()
{
	/*if (FAILED(CGameObject::Render(4)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to CGameObject::Render");
	}

	return S_OK;*/
	if (m_bTransparency)
	{
		_float4 vCamPosition = *(_float4*)CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).m[3];
		if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_RawValue("g_vRimColor", &m_vRimColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimStrength", m_fRimStrength)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_Float");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimPower", m_fRimPower)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_Float");
		}

		if (FAILED(CGameObject::Render(10)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(Bind_Transform(m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CShiden_Teammate::Render", "Failed to CGameObject::Bind_Transform");
		}

		for (_uint i = 0; i < m_vecMeshPassIndexPair.size(); ++i)
		{
			_uint iMeshIndex = m_vecMeshPassIndexPair[i].first;
			_uint iPassIndex = m_vecMeshPassIndexPair[i].second;
			if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CShiden_Teammate::Render", "Failed to Bind_ShaderResourceViews");
			}
			if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CShiden_Teammate::Render", "Failed to Bind_ShaderOptions");
			}
			if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
			{
				MSG_RETURN(E_FAIL, "CShiden_Teammate::Render", "Failed to Bind_BoneMatrices");
			}

			if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
			{
				MSG_RETURN(E_FAIL, "CShiden_Teammate::Render", "Failed to Render Model");
			}
		}
	}

	return S_OK;
}

void CShiden_Teammate::Active_Weapon(_bool bActive)
{
	m_pLeftWeapon->Activate_Collider(bActive);
	m_pRightWeapon->Activate_Collider(bActive);
}

HRESULT CShiden_Teammate::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CShiden_Teammate> CShiden_Teammate::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CShiden_Teammate> pInstance = make_private_shared(CShiden_Teammate, pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CShiden_Teammate::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CShiden_Teammate::Clone(any vInitPosition)
{
	shared_ptr<CShiden_Teammate> pInstance = make_private_shared_copy(CShiden_Teammate, *this);

	if (FAILED(pInstance->Initialize(vInitPosition)))
	{
		MSG_RETURN(nullptr, "CShiden_Teammate::Clone", "Failed to Initialize");
	}

	return pInstance;
}
