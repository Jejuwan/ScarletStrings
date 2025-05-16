#include "ClientPCH.h"
#include "TestBarrigate.h"

#include "GameInstance.h"
#include "PhysXEngine.h"

CTestBarrigate::CTestBarrigate(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CInteractiveObject(_pDevice, _pContext)
{
}

CTestBarrigate::CTestBarrigate(const CTestBarrigate& _rhs)
	: CInteractiveObject(_rhs)
{
}

HRESULT CTestBarrigate::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_TEST_BARRIGATE, g_aNull);

	return S_OK;
}

HRESULT CTestBarrigate::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKasane::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	m_pTransform->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(-18.f, 0.f, -35.f, 1.f));
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	PxShape* shape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(0.8, 0.45, 0.2), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	shape->setLocalPose(PxTransform(PxVec3(0, 0.5, 0)));
	m_pActor->attachShape(*shape);

	PxTransform pLocal;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, 10.0f);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);
	return S_OK;
}

void CTestBarrigate::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Test Barrigate Transform"))
	{
	_float4x4 mCarMatrix = m_pTransform->Get_WorldMatrix();

	_bool bTransformDirtyFlag = false;
	_vector vScale, vQuaternion, vTranslation;

	_float3 vNewPosition;
	_float3 vNewRotation;
	if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mCarMatrix))
	{
		ImGui::SeparatorText("Test Barrigate Transform");
		vNewPosition = vTranslation;
		if (ImGui::DragFloat3("Test Barrigate Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
			bTransformDirtyFlag = true;
		vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
		if (ImGui::DragFloat3("Test Barrigate Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
			bTransformDirtyFlag = true;
	}

	if (bTransformDirtyFlag)
	{
		_float4x4 mNewWorld = XMMatrixAffineTransformation(vScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
			_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);

		m_pTransform->Set_Matrix(mNewWorld);
	}

	ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}
#endif

	if (CGameInstance::Get_Instance()->Key_Down('R'))
	{
		m_pActor->addForce(PxVec3(-2555, 555, 555));
		m_pActor->addTorque(PxVec3(555, 555, 555));
	}
	PxMat44 worldMat(m_pActor->getGlobalPose());

	_float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
	_float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
	_float4x4 rotmat = XMMatrixRotationQuaternion(q);
	
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float4x4 mat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	mat *= rotmat;
	m_pTransform->Set_Matrix(mat);
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4( p.x,p.y,p.z,1.f ));
	
	//_matrix rotMat = XMMatrixRotationQuaternion({ m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w });
	//m_pTransform->Set_Matrix(rotMat);	
	//m_pTransform->Set_State(TRANSFORM::POSITION, _float4(m_pActor->getGlobalPose().p.x, m_pActor->getGlobalPose().p.y,
	//	m_pActor->getGlobalPose().p.z, 1.f));
}

void CTestBarrigate::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);

	if (m_bFocused) 
	{
		Add_RenderObject(RENDER_GROUP::OUTLINE);
		m_bFocused = false;
	}
}

HRESULT CTestBarrigate::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestBarrigate::Render_OutLine()
{
	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestBarrigate::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CTestBarrigate> CTestBarrigate::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTestBarrigate> pInstance = make_private_shared(CTestBarrigate, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestBarrigate::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestBarrigate::Clone(any)
{
	shared_ptr<CTestBarrigate> pInstance = make_private_shared_copy(CTestBarrigate, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTestBarrigate::Clone", "Failed to Initialize");
	}

	return pInstance;
}
