#include "ClientPCH.h"
#include "TestCube.h"
#include "GameInstance.h"
#include "PhysXEngine.h"

CTestCube::CTestCube(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CInteractiveObject(_pDevice, _pContext)
{
}

CTestCube::CTestCube(const CTestCube& _rhs)
	: CInteractiveObject(_rhs)
{
}

HRESULT CTestCube::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_CUBE);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_CUBE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_CUBE, g_aNull);

	return S_OK;
}

HRESULT CTestCube::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	TESTOBJDESC* desc = any_cast<TESTOBJDESC*>(pArg);

	m_pTransform->Set_State(TRANSFORM::POSITION, {desc->vSpawnPos.x,desc->vSpawnPos.y,desc->vSpawnPos.z,1});
	
	_float4x4 mat = m_pTransform->Get_Matrix();
	PxTransform pose = PxTransform(PxIdentity);
	pose.p = { mat._41,mat._42,mat._43 };
	_float4 vQuat;
	XMStoreFloat4(&vQuat, XMQuaternionRotationMatrix(XMLoadFloat4x4(&mat)));
	pose.q = { vQuat.x,vQuat.y,vQuat.z,vQuat.w };
	_float3 vScale = m_pTransform->Get_Scale();

	if(desc->eType == TESTOBJDESC::STATIC)
	{
		m_pTransform->Set_Scale(XMVectorSet(500.f, 0.1f, 500.f,0.f));
		_float3 vScale = m_pTransform->Get_Scale();
		PxRigidStatic* hfActor = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetPhysics()->createRigidStatic(pose);
		PxTriangleMeshGeometry hfGeom(m_pVIBuffer_Cube->Get_TriangleMesh(), PxMeshScale({ vScale.x, vScale.y, vScale.z }));
		PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *CGameInstance::Get_Instance()->Get_PhysXEngine()->GetMaterial());
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->addActor(*hfActor);
	}
	else if (desc->eType == TESTOBJDESC::DYNAMIC)
	{
		/*PxRigidDynamic* dyn = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
		dyn->setLinearDamping(0.2f);
		dyn->setAngularDamping(0.1f);
		PxTriangleMeshGeometry geom;
		geom.triangleMesh = m_pVIBuffer_Cube->Get_TriangleMesh();
		geom.scale = PxVec3(1.f, 1.f, 1.f);

		dyn->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_GYROSCOPIC_FORCES, true);
		dyn->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);

		PxMaterial* pMaterial =  pGameInstance->Get_PhysXEngine()->GetMaterial();
		PxShape* shape = PxRigidActorExt::createExclusiveShape(*dyn, geom, *pMaterial);
		shape->setContactOffset(0.05f);
		shape->setRestOffset(0.0f);

		PxRigidBodyExt::updateMassAndInertia(*dyn, 100.f);

		pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*dyn);

		dyn->setWakeCounter(100000000.f);
		dyn->setSolverIterationCounts(50, 1);
		dyn->setMaxDepenetrationVelocity(5.f);*/
	}

	return S_OK;
}

void CTestCube::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTestCube::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	//Add_RenderObject(RENDER_GROUP::OUTLINE);
}

// Super Class Uses 2D Texture
// It Forces Users to Bind Texture Manually
// Usually must be set from prototype
HRESULT CTestCube::Render()
{
	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestCube::Render_OutLine()
{
	if (m_bFocused)
	{
		if (FAILED(__super::Render(2)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to CGameObject::Render_OutLine");
		}

		m_bFocused = false;
	}

	return S_OK;
}

HRESULT CTestCube::Ready_Components()
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

	m_pTexture = CGameInstance::Get_Instance()->Clone_Component<CTexture>(SCENE::REALTEST, PROTOTYPE_COMPONENT_TEXTURE_PURPLE);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Clone_Component: PROTOTYPE_COMPONENT_TEXTURE_BLACK");
	}

	m_pVIBuffer_Cube = Get_Component<CVIBuffer_Cube>(COMPONENT::VIBUFFER_CUBE);
	if (nullptr == m_pVIBuffer_Cube)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: VIBUFFER_CUBE");
	}

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}

shared_ptr<CTestCube> CTestCube::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTestCube> pInstance = make_private_shared(CTestCube, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestCube::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestCube::Clone(any pArg)
{
	shared_ptr<CTestCube> pInstance = make_private_shared_copy(CTestCube, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CTestCube::Clone", "Failed to Initialize");
	}

	return pInstance;
}
