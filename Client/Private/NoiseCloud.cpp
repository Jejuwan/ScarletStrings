#include "ClientPCH.h"
#include "NoiseCloud.h"
#include "GameInstance.h"

CNoiseCloud::CNoiseCloud(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CNoiseCloud::CNoiseCloud(const CNoiseCloud& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CNoiseCloud::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXCLOUDPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	//m_umapComponentArg[COMPONENT::TEXTURE] = make_pair(PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR, g_aNull);

	return S_OK;
}

HRESULT CNoiseCloud::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Initialize", "Failed to CGameObject::Initialize");
	}
	
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 1000.f, 0.f, 1.f));
	m_pTransform->Set_Scale(XMVectorSet(10000.f, 10000.f, 1.f, 0.f));
	m_pTransform->LookAt(XMVectorSet(0.f, 0.f, 0.f, 1.f), false);

	return S_OK;
}

void CNoiseCloud::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fNoiseAccTime += _fTimeDelta * 0.15f;
}

void CNoiseCloud::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CNoiseCloud::Render()
{
	if (FAILED(m_pShader->Bind_Float("fTime", m_fNoiseAccTime)))
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Render", "Failed to Bind_ShaderResourceView");
	}
	
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CNoiseCloud::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_MASK_CIRCULAR));
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CNoiseCloud::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

shared_ptr<CNoiseCloud> CNoiseCloud::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CNoiseCloud> pInstance = make_private_shared(CNoiseCloud, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CNoiseCloud::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CNoiseCloud::Clone(any)
{
	shared_ptr<CNoiseCloud> pInstance = make_private_shared_copy(CNoiseCloud, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CNoiseCloud::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}
