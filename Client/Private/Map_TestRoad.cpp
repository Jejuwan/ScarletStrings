#include "ClientPCH.h"
#include "Map_TestRoad.h"
#include "GameInstance.h"

#include "Map_Tool.h"
#include "PhysXEngine.h"

CMap_TestRoad::CMap_TestRoad(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMap_TestRoad::CMap_TestRoad(const CMap_TestRoad& _rhs)
	:CGameObject(_rhs)
{
}

HRESULT CMap_TestRoad::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);

	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_TUTORIAL_ROAD0, g_aNull);

	return S_OK;
}

HRESULT CMap_TestRoad::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}
	m_pTransform->Translate({ -5,-2,-5,0 });
	float4x4 mat = m_pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	PxTransform pose = PxTransform(PxIdentity);
	pose.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	_float3 vScale = m_pTransform->Get_Scale();

	for (int i = 0; i < m_pModel->Get_TriangleMeshes().size(); i++)
	{
		_float3 vScale = m_pTransform->Get_Scale();
		PxRigidStatic* hfActor = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetPhysics()->createRigidStatic(pose);
		PxTriangleMeshGeometry hfGeom(m_pModel->Get_TriangleMeshes()[i], PxMeshScale({ vScale.x, vScale.y, vScale.z }));
		PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *CGameInstance::Get_Instance()->Get_PhysXEngine()->GetMaterial());

		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->addActor(*hfActor);
	}
	return S_OK;
}

void CMap_TestRoad::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CMap_TestRoad::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
//	Add_RenderObject(RENDER_GROUP::SHADOW);
}

HRESULT CMap_TestRoad::Render()
{
	/*if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CSkyCube::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}*/

	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CMap_TestRoad::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_TestRoad::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_TestRoad::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_TestRoad::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_TestRoad::Ready_Components()
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

	m_pTransform->Set_Matrix(g_mUnit);


	return S_OK;
}
shared_ptr<CMap_TestRoad> CMap_TestRoad::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CMap_TestRoad> pInstance = make_private_shared(CMap_TestRoad, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMap_TestRoad::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_TestRoad::Clone(any pArg)
{
	shared_ptr<CMap_TestRoad> pInstance = make_private_shared_copy(CMap_TestRoad, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_TestRoad::Clone", "Failed to Initialize");
	}

	return pInstance;
}
