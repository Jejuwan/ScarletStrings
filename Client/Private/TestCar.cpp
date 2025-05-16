#include "ClientPCH.h"
#include "TestCar.h"

#include "GameInstance.h"
#include "PhysXEngine.h"

CTestCar::CTestCar(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CInteractiveObject(_pDevice, _pContext)
{
}

CTestCar::CTestCar(const CTestCar& _rhs)
	: CInteractiveObject(_rhs)
{
}

HRESULT CTestCar::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_TEST_CAR_A, g_aNull);

	return S_OK;
}

HRESULT CTestCar::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKasane::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	m_pTransform->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(-10.f, 0.f, -35.f, 1.f));
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	PxShape* shape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(0.8f, 0.7f, 2.3f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
	shape->setLocalPose(PxTransform(PxVec3(0, 0.8, 0)));
	m_pActor->attachShape(*shape);



	PxTransform pLocal;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, 0.1f);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);

	return S_OK;
}

void CTestCar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _DEBUG
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Test Car Transform"))
	{
	_float4x4 mCarMatrix = m_pTransform->Get_WorldMatrix();

	_bool bTransformDirtyFlag = false;
	_vector vScale, vQuaternion, vTranslation;

	_float3 vNewPosition;
	_float3 vNewRotation;
	if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mCarMatrix))
	{
		ImGui::SeparatorText("Test Car Transform");
		vNewPosition = vTranslation;
		if (ImGui::DragFloat3("Test Car Position", &vNewPosition.x, 0.005f, -FLT_MAX, +FLT_MAX))
			bTransformDirtyFlag = true;
		vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
		if (ImGui::DragFloat3("Test Car Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
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

	if (CGameInstance::Get_Instance()->Key_Down('E'))
	{
		m_pActor->addForce(PxVec3(2555, 555, 555));
		m_pActor->addTorque(PxVec3(555, 555, 555));
	}

	PxMat44 worldMat(m_pActor->getGlobalPose());

	_float4 c1 = { worldMat.column0.x, worldMat.column0.y, worldMat.column0.z, worldMat.column0.w };
	_float4 c2 = { worldMat.column1.x, worldMat.column1.y, worldMat.column1.z, worldMat.column1.w };
	_float4 c3 = { worldMat.column2.x,worldMat.column2.y,worldMat.column2.z,worldMat.column2.w };
	_float4 c4 = { worldMat.column3.x,worldMat.column3.y,worldMat.column3.z,worldMat.column3.w };

	_float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
	_float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
	_float4x4 rotmat = XMMatrixRotationQuaternion(q);

	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float4x4 mat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	mat *= rotmat;
	m_pTransform->Set_Matrix(mat);
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
#endif
}

void CTestCar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);

	if (m_bFocused)
	{
		Add_RenderObject(RENDER_GROUP::OUTLINE);
		m_bFocused = false;
	}
}

HRESULT CTestCar::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestCar::Render_OutLine()
{
	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestCar::Ready_Components()
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

shared_ptr<CTestCar> CTestCar::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTestCar> pInstance = make_private_shared(CTestCar, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestCar::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestCar::Clone(any)
{
	shared_ptr<CTestCar> pInstance = make_private_shared_copy(CTestCar, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTestCar::Clone", "Failed to Initialize");
	}

	return pInstance;
}
