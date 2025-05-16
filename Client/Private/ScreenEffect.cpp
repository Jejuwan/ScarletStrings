#include "ClientPCH.h"
#include "ScreenEffect.h"

#include "GameInstance.h"

CScreenEffect::CScreenEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CScreenEffect::CScreenEffect(const CScreenEffect& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CScreenEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenEffect::Initialize(any)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScreenRippleEffect::Initialize", "Failed to CGameObject::Initialize");
	}

	m_matProjection = XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransform->Set_Scale(XMVectorSet(g_iWinCX, g_iWinCY, 1.f, 0.f));
	m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorSet(0.f, 0.f, 0.5f, 1.f));

	m_fShaderTime = 0.f;

	return S_OK;
}

void CScreenEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CScreenEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::POSTPROCESS);
	m_fShaderTime += _fTimeDelta;
}

HRESULT CScreenEffect::Render()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Copy_From_BackBuffer(RENDERTARGET_CUTSCENE_MAIN);

	if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXDIFFUSE, pGameInstance->Get_RenderTarget_ShaderResourceView(RENDERTARGET_CUTSCENE_MAIN))))
	{
		MSG_RETURN(E_FAIL, "CScreenEffect::Render", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CScreenEffect::Render", "Failed to CTransform::Bind_OnShader");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, m_matProjection)))
	{
		MSG_RETURN(E_FAIL, "CScreenEffect::Render", "Failed to CTransform::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_Float("g_fTime", m_fShaderTime)))
	{
		MSG_RETURN(E_FAIL, "CScreenEffect::Render", "Failed to Bind_Float: ShaderTime");
	}

	return S_OK;
}

void CScreenEffect::Set_Enable(_bool bEnable)
{
	m_bEnable = bEnable;

	if (!m_bEnable)
	{
		m_fShaderTime = 0.f;
	}
}

HRESULT CScreenEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Ready_Components", "Failed to Get_Component: VIBUFFER_RECT");
	}

	return S_OK;
}