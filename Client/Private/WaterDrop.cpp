#include "ClientPCH.h"
#include "WaterDrop.h"
#include "GameInstance.h"
#include "PhysXCollider.h"
#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
#endif
#define VIBUFFER

const _float CWaterDrop::s_fForce	= 100.f;

CWaterDrop::CWaterDrop(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CWaterDrop::CWaterDrop(const CWaterDrop& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CWaterDrop::Initialize_Prototype()
{
#ifdef VIBUFFER
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_SPHERE);
	//m_bitBehavior |= BIT(BEHAVIOR::PHYSICS);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_SPHERE] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_SPHERE_HIGHRES, g_aNull);
#else
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MESH);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MESH] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_MESH_SPHERE, g_aNull);
#endif

	m_tMaterialDesc.vDiffuse = _float4(0.2f, 0.6f, 0.8f, 0.5f);

	m_tEntityDesc.vMaxSpeed	= _float3(100.f, 100.f, 100.f);
	m_tEntityDesc.vResist	= _float3(0.4f, 0.2f, 0.4f);
	
	return S_OK;
}

HRESULT CWaterDrop::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Initialize", "Failed to CMonster::Initialize");
	}
	m_pAttackCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::SPHERE;
	desc.fRadius = 2.f;

	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::MONSTER_ATTACK;
	actorDesc.stAttackDesc.iDamage = 10;
	m_pAttackCollider->Initialize(desc, actorDesc);//atk
	m_pAttackActorDesc = (ACTORDESC*)m_pAttackCollider->GetActor()->userData;
	m_pAttackActorDesc->bActive = true;
	

	m_pTexDiffuse	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Unused/T_ef_scl_wat_008.dds"));
//	m_pTexNormal	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Unused/T_ef_one_wat_018_NML.dds"));
	m_pTexNoise		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Effect/Unused/T_ef_scl_water_Noise01.dds"));

#ifndef VIBUFFER
	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
#endif

	return S_OK;
}

void CWaterDrop::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pTransform->Translate(m_vDir * _fTimeDelta * 2.f);
	m_fTimeAcc += _fTimeDelta;
	PxTransform Transform;
	_matrix Matrix = m_pTransform->Get_Matrix();
	_vector Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_vector Quat = XMQuaternionRotationMatrix(Matrix);
	Transform.q = PxQuat(XMVectorGetX(Quat), XMVectorGetY(Quat), XMVectorGetZ(Quat), XMVectorGetW(Quat));
	Transform.p = PxVec3(XMVectorGetX(Pos), XMVectorGetY(Pos), XMVectorGetZ(Pos));

	//if (m_pAttackActorDesc->bHit)
	//	int Test = 0;

	m_pAttackCollider->GetActor()->setGlobalPose(Transform);
//#ifdef _DEBUG
//	if(nullptr == m_pTargetTransform)
//		m_pTargetTransform = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
//#endif


	if (m_pAttackActorDesc->bHit)
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, -1000.f, 0.f, 0.f));
		Matrix = m_pTransform->Get_Matrix();
		Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
		Quat = XMQuaternionRotationMatrix(Matrix);
		Transform.q = PxQuat(XMVectorGetX(Quat), XMVectorGetY(Quat), XMVectorGetZ(Quat), XMVectorGetW(Quat));
		Transform.p = PxVec3(XMVectorGetX(Pos), XMVectorGetY(Pos), XMVectorGetZ(Pos));
		m_pAttackCollider->GetActor()->setGlobalPose(Transform);

		m_bRelease = true;
	}
	if (!m_bRelease)
	{
		m_bRelease = m_fTimeAcc > 3.f;
	}
}

void CWaterDrop::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_fBloomStrength)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CWaterDrop::Render()
{
#if ACTIVATE_IMGUI
	static float	fWorldScale		= 4.f;
	static float	fTimeScale		= 24.f;
	static float	fWaveScale		= 0.1f;
	static float	fOffsetScale	= 1.f;
	static float	fFlowTimeScale	= 1.f;
	static float	fFlowScale		= 1.2f;
	static float	fMinAlpha		= 0.6f;
	static float	fMaxAlpha		= 2.f;
	static float4	vOffset			= _float4(0.f, 1.f, 2.f, 3.f);

	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		if (ImGui::BeginMainMenuBar())
		{
		if (ImGui::BeginMenu("WaterDrop"))
		{
		ImGui::InputFloat("BloomStrength", &m_fBloomStrength);
		ImGui::InputFloat("WorldScale", &fWorldScale);
		ImGui::InputFloat("TimeScale", &fTimeScale);
		ImGui::InputFloat("WaveScale", &fWaveScale);
		ImGui::InputFloat("OffsetScale", &fOffsetScale);
		ImGui::InputFloat("FlowTimeScale", &fFlowTimeScale);
		ImGui::InputFloat("FlowScale", &fFlowScale);
		ImGui::InputFloat("fMinAlpha", &fMinAlpha);
		ImGui::InputFloat("fMaxAlpha", &fMaxAlpha);
		ImGui::InputFloat4("Offset", &vOffset.x);
		ImGui::InputFloat4("Diffuse", &m_tMaterialDesc.vDiffuse.x);
		ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		}
	}

	if (FAILED(m_pShader->Bind_Float("fWorldScale", fWorldScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fWorldScale");
	}
	if (FAILED(m_pShader->Bind_Float("fTimeScale", fTimeScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fTimeScale");
	}
	if (FAILED(m_pShader->Bind_Float("fWaveScale", fWaveScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fWaveScale");
	}
	if (FAILED(m_pShader->Bind_Float("fOffsetScale", fOffsetScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fOffsetScale");
	}
	if (FAILED(m_pShader->Bind_Float("fFlowTimeScale", fFlowTimeScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fFlowTimeScale");
	}
	if (FAILED(m_pShader->Bind_Float("fFlowScale", fFlowScale)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fFlowScale");
	}
	if (FAILED(m_pShader->Bind_Float("fMinAlpha", fMinAlpha)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fMinAlpha");
	}
	if (FAILED(m_pShader->Bind_Float("fMaxAlpha", fMaxAlpha)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fMaxAlpha");
	}
	if (FAILED(m_pShader->Bind_Vector("vOffset", vOffset)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Vector: vOffset");
	}
#else
	if (FAILED(m_pShader->Bind_Float("fWorldScale", 4.f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fWorldScale");
	}
	if (FAILED(m_pShader->Bind_Float("fTimeScale", 24.f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fTimeScale");
	}
	if (FAILED(m_pShader->Bind_Float("fWaveScale", 0.1f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fWaveScale");
	}
	if (FAILED(m_pShader->Bind_Float("fOffsetScale", 1.f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fOffsetScale");
	}
	if (FAILED(m_pShader->Bind_Float("fFlowTimeScale", 1.f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fFlowTimeScale");
	}
	if (FAILED(m_pShader->Bind_Float("fFlowScale", 1.2f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fFlowScale");
	}
	if (FAILED(m_pShader->Bind_Float("fMinAlpha", 0.6f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fMinAlpha");
	}
	if (FAILED(m_pShader->Bind_Float("fMaxAlpha", 2.f)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: fMaxAlpha");
	}
	if (FAILED(m_pShader->Bind_Vector("vOffset", _float4(0.f, 1.f, 2.f, 3.f))))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Vector: vOffset");
	}
#endif

	if (FAILED(m_pTexNoise->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXMASK)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
	}
	if (FAILED(m_pTexDiffuse->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
	}
//	if (FAILED(m_pTexNormal->Bind_ShaderResourceView(m_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
//	{
//		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
//	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_fBloomStrength)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_TIME, CGameInstance::Get_Instance()->Get_ActivatedTime())))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: SHADER_TIME");
	}

#ifdef VIBUFFER
	if (FAILED(__super::Render(3)))
#else
	if (FAILED(__super::Render(15)))
#endif
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CWaterDrop::Render_Bloom()
{
	if (FAILED(m_pTexNoise->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXMASK)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
	}
	if (FAILED(m_pTexDiffuse->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
	}
//	if (FAILED(m_pTexNormal->Bind_ShaderResourceView(m_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
//	{
//		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_ShaderResourceView");
//	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_fBloomStrength)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_TIME, CGameInstance::Get_Instance()->Get_ActivatedTime())))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render", "Failed to Bind_Float: SHADER_TIME");
	}

	if (FAILED(CGameObject::Render(4)))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Render_Bloom", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CWaterDrop::Fetch(any _mWorld)
{
	m_pTransform->Set_Matrix(any_cast<_float4x4>(_mWorld));
	//m_pPhysics->Flattern(true, true, true);
	//m_pPhysics->Force(TRANSFORM::LOOK, s_fForce);
	m_fTimeAcc = 0.f;
	m_bRelease = false;
	m_pAttackActorDesc->bHit = false;
	m_pTargetTransform = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_vDir = m_pTargetTransform->Get_State(TRANSFORM::POSITION) + XMVectorSet(0.f, 1.f, 0.f, 0.f) - m_pTransform->Get_State(TRANSFORM::POSITION);
	return S_OK;
}

_bool CWaterDrop::Return()
{
	return m_bRelease;
}

HRESULT CWaterDrop::Release()
{
	//m_pTargetTransform = nullptr; 
	return S_OK;
}

HRESULT CWaterDrop::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);

	return S_OK;
}

HRESULT CWaterDrop::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CWaterDrop::Ready_Behaviors", "Failed to CGameObject::Ready_Behaviors");
	}

	//m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);

	return S_OK;
}

shared_ptr<CWaterDrop> CWaterDrop::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CWaterDrop> pInstance = make_private_shared(CWaterDrop, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CWaterDrop::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CWaterDrop::Clone(any)
{
	shared_ptr<CWaterDrop> pInstance = make_private_shared_copy(CWaterDrop, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CWaterDrop::Clone", "Failed to Initialize");
	}

	return pInstance;
}
