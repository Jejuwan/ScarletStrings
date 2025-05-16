#include "ClientPCH.h"
#include "TitleIcon.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"

CTitleIcon::CTitleIcon(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTitleIcon::CTitleIcon(const CTitleIcon& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTitleIcon::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::TEXTURE);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE] = make_pair(PROTOTYPE_COMPONENT_TEXTURE_TITLE_ICON, g_aNull);

	return S_OK;
}

HRESULT CTitleIcon::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTitleIcon::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Scale({ 664.f, 84.f, 1.f });
	m_pTransform->Set_State(TRANSFORM::POSITION, { 0.f, -300.f, 0.9f, 1.f });

	m_vFontPosition = { 875.f, 849.f };

	m_tMaterialDesc.vDiffuse.w = 0.4f;

	return S_OK;
}

void CTitleIcon::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (m_bIncreaseAlpha)
	{
		m_tMaterialDesc.vDiffuse.w += _fTimeDelta * 0.5f;
		if (m_tMaterialDesc.vDiffuse.w >= 0.4f)
		{
			m_bIncreaseAlpha = false;
		}
	}
	else
	{
		m_tMaterialDesc.vDiffuse.w -= _fTimeDelta * 0.5f;
		if (m_tMaterialDesc.vDiffuse.w <= 0.1f)
		{
			m_bIncreaseAlpha = true;
		}
	}
}

void CTitleIcon::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CTitleIcon::Render()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Render", "Failed to CTexture::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(Bind_VIBuffer(m_pShader, 1)))
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Render", "Failed to CGameObject::Render");
	}

	_float2 vFontPosition = { m_pTransform->Get_State(TRANSFORM::POSITION).x, m_pTransform->Get_State(TRANSFORM::POSITION).y };
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), TEXT("Press Any Button"),
		m_vFontPosition, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.5f);

	return S_OK;
}

HRESULT CTitleIcon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Ready_Components", "Failed to Get_Component: CVIBuffer_Rect");
	}

	m_pTexture = Get_Component<CTexture>(COMPONENT::TEXTURE);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CTitleLogo::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

shared_ptr<CTitleIcon> CTitleIcon::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTitleIcon> pInstance = make_private_shared(CTitleIcon, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTitleIcon::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTitleIcon::Clone(any)
{
	shared_ptr<CTitleIcon> pInstance = make_private_shared_copy(CTitleIcon, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTitleIcon::Clone", "Failed to Initialize");
	}

	return pInstance;
}
