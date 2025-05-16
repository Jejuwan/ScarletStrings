#include "ClientPCH.h"
#include "EventCollider.h"

#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"

CEventCollider::CEventCollider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CEventCollider::CEventCollider(const CEventCollider& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEventCollider::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	return S_OK;
}

HRESULT CEventCollider::Initialize(any wstrFilePath)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	m_tInstDebugCubeDesc.wstrModelTag = PROTOTYPE_COMPONENT_MODEL_DEBUG_CUBE;
	m_tInstDebugCubeDesc.iPassIndex = 0;

	if (CGameInstance::Get_Instance()->Current_Scene() != SCENE::MAPTOOL)
	{
		Import(any_cast<wstring>(wstrFilePath));
	}

	return S_OK;
}

void CEventCollider::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pActorDesc && m_pActorDesc->bHit)
	{
		m_pActorDesc->bHit = false;

		if (m_fpOnEnter)
		{
			m_fpOnEnter(m_eEventType, m_strEventTag);
			m_fpOnEnter = nullptr;

			m_bEnable = false;
		}
	}
}

void CEventCollider::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::MAPTOOL)
		m_pModelRenderer->Add_RenderObject(RENDER_GROUP::NONBLEND, m_tInstDebugCubeDesc, m_pTransform);
}

HRESULT CEventCollider::Render()
{
	return S_OK;
}

HRESULT CEventCollider::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBattleField::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CBattleField::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}

	m_pTransform = m_pModelRenderer->Add_Instance(PROTOTYPE_COMPONENT_MODEL_DEBUG_CUBE);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);
	return S_OK;
}

void CEventCollider::Import(const wstring& wstrFilePath)
{
	std::ifstream inFile(wstrFilePath, std::ios::binary);
	if (!inFile.is_open())
	{
		MSG_RETURN(, "CEventCollider::Import", "Failed to Open File");
	}

	EVENT_TYPE eEventType;
	inFile.read(reinterpret_cast<char*>(&eEventType), sizeof(EVENT_TYPE));

	size_t iTagSize;
	inFile.read(reinterpret_cast<char*>(&iTagSize), sizeof(size_t));
	std::string strEventTag(iTagSize, '\0');
	inFile.read(&strEventTag[0], iTagSize);

	_float4x4 matWorld;
	inFile.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

	Set_EventType(eEventType);
	Set_EventTag(strEventTag);

	m_pTransform->Set_Matrix(matWorld);

	// Bake Physics Collider
	{
		_float4x4 mat = m_pTransform->Get_Matrix();
		_vector Rotation, Scale, Pos;
		XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

		*(_float4*)mat.m[0] = XMVector3Normalize(*(_float4*)mat.m[0]);
		*(_float4*)mat.m[1] = XMVector3Normalize(*(_float4*)mat.m[1]);
		*(_float4*)mat.m[2] = XMVector3Normalize(*(_float4*)mat.m[2]);
		Rotation = XMQuaternionRotationMatrix(mat);

		PxTransform vTransform = PxTransform(PxIdentity);
		vTransform.p = PxVec3(mat._41, mat._42, mat._43);
		_float4 vQuat;
		XMStoreFloat4(&vQuat, Rotation);
		vTransform.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);

		m_pEventCollider = make_shared<CPhysXCollider>();
		CPhysXCollider::PHYSXCOLLIDERDESC desc;
		desc.eType = CPhysXCollider::TYPE::BOX;

		_float4 vExtents = Scale;
		desc.vHalfExtents = _float3(vExtents.x, vExtents.y, vExtents.z);

		ACTORDESC actorDesc;
		actorDesc.strTag = ACTORTAG::EVENT;
		m_pEventCollider->Initialize(desc, actorDesc);

		auto pEventActor = m_pEventCollider->GetActor();
		pEventActor->setGlobalPose(vTransform);

		m_pActorDesc = (ACTORDESC*)m_pEventCollider->GetActor()->userData;
	}

	inFile.close();
}

shared_ptr<CEventCollider> CEventCollider::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CEventCollider> pInstance = make_private_shared(CEventCollider, pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEventCollider::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEventCollider::Clone(any wstrFilePath)
{
	shared_ptr<CEventCollider> pInstance = make_private_shared_copy(CEventCollider, *this);

	if (FAILED(pInstance->Initialize(wstrFilePath)))
	{
		MSG_RETURN(nullptr, "CEventCollider::Clone", "Failed to Initialize");
	}

	return pInstance;
}
