#include "ClientPCH.h"
#include "PlayerWeapon.h"
#include "Player.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "PlayerIdleState.h"
#include "Model.h"
#include "Bone.h"
#include "Effect_Manager.h"

CPlayerWeapon::CPlayerWeapon(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon& _rhs)
	:CGameObject(_rhs)
{

}

CPlayerWeapon::~CPlayerWeapon()
{
}

HRESULT CPlayerWeapon::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON, g_aNull);

	return S_OK;
}

HRESULT CPlayerWeapon::Initialize(any pArg)
{
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER_WEAPON, &m_tMaterialDesc);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pArg.has_value())
		m_WeaponDesc = any_cast<WEAPONDESC>(pArg);

	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());

	m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 0.5f;

	ACTORDESC attackDesc;
	attackDesc.strTag = ACTORTAG::PLAYER_ATTACK;
	attackDesc.stAttackDesc.iDamage = 1.f;
	m_pCollider->Initialize(desc, attackDesc);

	m_pBigCollider = make_shared<CPhysXCollider>();
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 1.f;

	attackDesc.strTag = ACTORTAG::PLAYER_BIGATTACK;
	attackDesc.stAttackDesc.iDamage = 1.f;
	attackDesc.bActive = false;
	m_pBigCollider->Initialize(desc, attackDesc);

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,5.f,10.f };
	cDesc.height = 0.00001;
	cDesc.radius = 0.5f;
	cDesc.slopeLimit = XMConvertToRadians(10.f);
	cDesc.contactOffset = 0.001f;
	cDesc.stepOffset = 0.0002f;
	//cDesc.reportCallback = m_pPlayerControllerHitReport.get();

	if(pGameInstance->Current_Scene() != SCENE::MINDROOM)
	{
		m_pController = pGameInstance->CreateController(&cDesc);
		m_ActorDesc.strTag = ACTORTAG::PLAYER_ATTACK;
		m_pController->getActor()->userData = &m_ActorDesc;
		m_pController->getActor()->setMass(0.f);
	}

	_uint iNumMeshes = m_pModel->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
		tShaderDesc.tMaterialDesc.vEmissive = _float4(1.f, 1.f, 1.f, 1.f);

		m_pModel->Set_ShaderDesc(i, tShaderDesc);
	}

	return S_OK;
}

void CPlayerWeapon::Tick(_float fTimeDelta)
{
	//ImGui::Begin("Weapon Material##Weapon");
	//ImGui::DragFloat4("Diffuse##Weapon", reinterpret_cast<_float*>(&m_tMaterialDesc.vDiffuse));
	//ImGui::DragFloat4("Ambient##Weapon", reinterpret_cast<_float*>(&m_tMaterialDesc.vAmbient));
	//ImGui::DragFloat4("Specular##Weapon", reinterpret_cast<_float*>(&m_tMaterialDesc.vSpecular));
	//ImGui::DragFloat4("Emissive##Weapon", reinterpret_cast<_float*>(&m_tMaterialDesc.vEmissive));
	//ImGui::End();

	__super::Tick(fTimeDelta);

#ifdef _DEBUG
#ifdef ACTIVATE_IMGUI
	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_2))
	{
		auto& tActor = m_pCollider->GetActorDesc();
		tActor.stAttackDesc.iDamage = 9999.f;
	}
#endif
#endif

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	_float3 vPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);

	if(pGameInstance->Key_Down('L')) Apply_Effect(false);

	_matrix BoneMatrix = pGameInstance->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(m_WeaponDesc.szBoneName.c_str())->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = pGameInstance->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

	m_pTransform->Set_Matrix(XMMatrixScaling(100.f,100.f,100.f)*
		BoneMatrix 
		*PivotMatrix
		*pGameInstance->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_Matrix()
	);

	PxTransform p;
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	p.p = PxVec3(vPos.x, vPos.y, vPos.z);
	_float4 vQuat = XMQuaternionRotationMatrix(m_pTransform->Get_Matrix());
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	if (pGameInstance->Current_Scene() != SCENE::MINDROOM)
		m_pController->getActor()->setGlobalPose(p);
	//_float3 vDisp;
	//XMStoreFloat3(&vDisp, vPos - vPrePos);
	//m_pController->move(PxVec3(vDisp.x, vDisp.y, vDisp.z), 0.0f, fTimeDelta, m_pPlayer->Get_ControllerFilters());
	m_pCollider->GetActor()->setGlobalPose(p);
	m_pCollider->GetActorDesc().mOwnerMat = m_pTransform->Get_Matrix();
	m_pCollider->GetActorDesc().vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pBigCollider->GetActor()->setGlobalPose(p);
	m_pBigCollider->GetActorDesc().vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);

	_matrix originMat = m_pTransform->Get_Matrix();
	shared_ptr<CTransform> pCamTransform = static_pointer_cast<CTransform>(m_pPlayer->Get_Camera()->Get_Component(COMPONENT::TRANSFORM));
	m_pTransform->LookAtDir(pCamTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION));
	m_pTransform->Translate(pCamTransform->Get_State(TRANSFORM::LOOK) * -1.f);
	_float fAngle = Function::RandomFloat(0, 3.14f);
	m_pTransform->Rotate(TRANSFORM::LOOK, fAngle);
	m_pCollider->GetActorDesc().mEffectMat = m_pTransform->Get_Matrix();
	m_pBigCollider->GetActorDesc().mEffectMat = m_pTransform->Get_Matrix();
	m_pTransform->Set_Matrix(originMat);

	if (m_pCollider && ((ACTORDESC*)(m_pCollider->GetActor()->userData))->bHit)
	{
		/*_float4x4 m = m_pTransform->Get_Matrix();
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1, initializer_list<any>{m,m,m,m,m});*/
		((ACTORDESC*)(m_pCollider->GetActor()->userData))->bHit = false;
	}

#pragma region EFFECT
	switch (m_eVFXState)
	{
	case VFX_STATE::FADEIN:
	{
		m_fVFXRatio = std::clamp(m_fVFXRatio + fTimeDelta / m_fVFXDurationFade, 0.f, 1.f);
		if (m_fVFXRatio >= 1.f)
		{
			m_eVFXState = VFX_STATE::TRAIL;
		}
	}
	break;
	case VFX_STATE::TRAIL:
	{
	}
	break;
	case VFX_STATE::FADEOUT:
	{
		m_fVFXRatio = std::clamp(m_fVFXRatio - fTimeDelta / m_fVFXDurationFade, 0.f, 1.f);
	}
	break;
	case VFX_STATE::STATIC:
	case VFX_STATE::MAX:
	default:
		break;
	}
#pragma endregion
}

void CPlayerWeapon::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	_matrix BoneMatrix = pGameInstance->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(m_WeaponDesc.szBoneName.c_str())->Get_CombinedTransformation();
	//if (XMVector3Equal(BoneMatrix.r[3], XMVectorSet(0.f, 0.f, 0.f, 1.f)))
	//	return;
	
	Add_RenderObject(RENDER_GROUP::NONBLEND);
//	Add_RenderObject(RENDER_GROUP::OUTLINE);
	Add_RenderObject(RENDER_GROUP::BLOOM);
}

HRESULT CPlayerWeapon::Render()
{
	//	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK1 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK2 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK3 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_BACKSTEP ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_DASH ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_UPPER ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK1 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK2 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::JUMP_ATTACK3 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK4 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_AIRDOWN ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_CHARGE ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_FOLLOW1 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_FOLLOW2 ||
		m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::ATTACK_JUSTDODGE)
	{
		if (VFX_STATE::STATIC == m_eVFXState)
		{
			if (FAILED(__super::Render(0)))
			{
				MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
			}
		}
		else
		{
			if (FAILED(m_pShader->Bind_Float("g_fMaterialFadeRatio", m_fVFXRatio)))
			{
				MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
			}
			if (FAILED(__super::Render(5)))
			{
				MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
			}
		}
	}
	
	return S_OK;
}

HRESULT CPlayerWeapon::Render_OutLine()
{
	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to CGameObject::Render_OutLine");
	}

	return S_OK;
}

HRESULT CPlayerWeapon::Render_Bloom()
{
	return S_OK;
}

HRESULT CPlayerWeapon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CPlayerWeapon::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

void CPlayerWeapon::Apply_Effect(_bool _bElec)
{
	m_eVFXState = VFX_STATE::FADEIN;

//	auto vfxmgr = CEffect_Manager::Get_Instance();
//	_float4x4 mOriginal = m_pTransform->Get_Matrix();
//	m_pTransform->Set_Scale(_float3(1.f, 0.005f, 1.f));
//	_float4x4 mFetch = m_pTransform->Get_Matrix();
//	m_pTransform->Set_Matrix(mOriginal);
//	vfxmgr->Fetch(!_bElec ? VFX::PLAYER_WEAPON_INITIALIZE : VFX::PLAYER_WEAPON_INITIALIZE_ELEC, mFetch);
//	_matrix m = XMMatrixScaling(2.f, 0.2f, 1.f);
//	m.r[3] = _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_INITIALIZE : VFX::PLAYER_WEAPON_INITIALIZE_ELEC, m_pTransform);
}

void CPlayerWeapon::Set_Emissive(_float4 vColor)
{
	m_tMaterialDesc.vEmissive = vColor;
}

shared_ptr<CPlayerWeapon> CPlayerWeapon::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CPlayerWeapon> pInstance = make_private_shared(CPlayerWeapon, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CPlayerWeapon::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPlayerWeapon::Clone(any pArg)
{
	shared_ptr<CPlayerWeapon> pInstance = make_private_shared_copy(CPlayerWeapon, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CPlayerWeapon::Clone", "Failed to Initialize");
	}

	return pInstance;
}
