#include "ClientPCH.h"
#include "HPLossEffect.h"

#include "GameInstance.h"

CHPLossEffect::CHPLossEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CHPLossEffect::CHPLossEffect(const CHPLossEffect& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CHPLossEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHPLossEffect::Initialize(any)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_CUTSCENE, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCutsceneBuffer::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Scale({ g_iWinCX, g_iWinCY, 1.f });
	m_pTransform->Set_State(TRANSFORM::POSITION, { 0.f, 0.f, 0.5f, 1.f });

	m_matProjection = XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);

	return S_OK;
}

void CHPLossEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	_float fScaledDeltaTime = _fTimeDelta * 3.f;

	m_fAccTime += fScaledDeltaTime;
	if (m_fAccTime <= 1.0f) 
	{
		m_tMaterialDesc.vDiffuse.w += fScaledDeltaTime * 2.f;
	}
	else if (m_fAccTime <= 2.0f) 
	{
		m_tMaterialDesc.vDiffuse.w -= fScaledDeltaTime * 2.f;
		if (m_tMaterialDesc.vDiffuse.w < 0.f)
			m_tMaterialDesc.vDiffuse.w = 0.f;
	}
	else 
	{
		m_fAccTime = 0.0f;
		m_iCycleCount++;

		if (m_iCycleCount >= 3)
		{
			m_iCycleCount = 0;

			m_tMaterialDesc.vDiffuse.w = 0.f;
			m_bEnable = false;
		}
	}
}

void CHPLossEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(UI_LAYER::LAYER_7);
}

HRESULT CHPLossEffect::Render()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, m_matProjection)))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CHPLossEffect::Bind_RawValue", "Failed to Bind_RawValue: SHADER_MTRLDIF");
	}

	Bind_Texture(m_pShader);

	Bind_VIBuffer(m_pShader, 9);

	return S_OK;
}

void CHPLossEffect::Set_Enable(_bool bEnable)
{
	m_bEnable = bEnable;

	m_fAccTime = 0.f;
	m_iCycleCount = 0;
	m_tMaterialDesc.vDiffuse.w = 0.f;
}

HRESULT CHPLossEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}
	m_pTransform->Set_Matrix(g_mUnit);

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: CVIBuffer_Rect");
	}

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pTexture = pGameInstance->Clone_Component<CTexture>(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_HPLOSS);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TEXTURE");
	}
	Add_Component(COMPONENT::TEXTURE, m_pTexture);

	return S_OK;
}

shared_ptr<CHPLossEffect> CHPLossEffect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CHPLossEffect> pInstance = make_private_shared(CHPLossEffect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CHPLossEffect::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CHPLossEffect::Clone(any)
{
	shared_ptr<CHPLossEffect> pInstance = make_private_shared_copy(CHPLossEffect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CHPLossEffect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
