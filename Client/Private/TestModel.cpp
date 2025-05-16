#include "ClientPCH.h"
#include "TestModel.h"
#include "GameInstance.h"
#include "Light.h"

#ifdef _DEBUG

CTestModel::CTestModel(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTestModel::CTestModel(const CTestModel& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTestModel::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(TEXT("TestModel"), g_aNull);

	return S_OK;
}

HRESULT CTestModel::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestModel::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CTestModel::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTestModel::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);
}

HRESULT CTestModel::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTestModel::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CTestModel::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(Get_Component<CModel>(COMPONENT::MODEL)->Render_ShadowDepth(shared_from_gameobject(), Get_Component<CShader>(COMPONENT::SHADER), 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CTestModel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestModel::Ready_Components", "Failed to Ready_Components");
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, _float3(0.f, 5.f, 10.f));
//	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_Scale(_float3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

HRESULT CTestModel::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CTestModel::Ready_Behaviors", "Failed to Ready_Behaviors");
	}

	return S_OK;
}

shared_ptr<CTestModel> CTestModel::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTestModel> pInstance = make_private_shared(CTestModel, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestModel::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTestModel::Clone(any)
{
	shared_ptr<CTestModel> pInstance = make_private_shared_copy(CTestModel, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTestModel::Clone", "Failed to Initialize");
	}

	return pInstance;
}

#endif
