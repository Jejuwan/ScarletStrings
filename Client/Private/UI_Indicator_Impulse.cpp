#include "ClientPCH.h"
#include "UI_Indicator_Impulse.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_Impulse::CUI_Indicator_Impulse(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Impulse::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Impulse::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    else
    {
        m_fGauge    = 0.f;
        m_vScale    = _float3(110.f, 110.f, 1.f);
        m_vPosition = _float4(-800.f, 0.f, 0.f, 1.f);
    }
    
    Initialize_UI();

    m_pItemBox = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/PsycoImpulse.png"));
    
    m_pItemBox->Set_Scale(m_vScale);
    m_pItemBox->Set_State(TRANSFORM::POSITION, m_vPosition);
    
    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Impulse::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_tUIDesc.m_fTime += fTimeDelta;

    if (m_tUIDesc.m_fFramePerSec <= m_tUIDesc.m_fTime)
    {
        m_tUIDesc.m_iIndex += 1;
        if (m_tUIDesc.m_iIndex >= m_tUIDesc.m_iIndexMaxY * m_tUIDesc.m_iIndexMaxX)
        {
            m_tUIDesc.m_iIndex = 0;
        }
        m_tUIDesc.m_fTime = 0.f;
    }
}

void CUI_Indicator_Impulse::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    //Set_IndicatorPosition();
}

HRESULT CUI_Indicator_Impulse::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pItemBox->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc.m_fTime, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc.m_iIndex, sizeof(_uint))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc.m_iIndexMaxX, sizeof(_uint))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc.m_iIndexMaxY, sizeof(_uint))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc.m_fSizeX, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc.m_fSizeY, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(8)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Render", "Failed to CGameObject::Render");
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_Impulse::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Impulse::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Impulse::Initialize_UI()
{
    ZeroMemory(&m_tUIDesc, sizeof m_tUIDesc);

    m_tUIDesc.m_iIndex          = 0;
    m_tUIDesc.m_iIndexMaxX      = 1;
    m_tUIDesc.m_iIndexMaxY      = 6;
    m_tUIDesc.m_fFramePerSec    = 0.03f;

    m_tUIDesc.m_fTime           = 0.f;
    m_tUIDesc.m_fSizeX          = 82.f;
    m_tUIDesc.m_fSizeY          = 492.f;
}

void CUI_Indicator_Impulse::Set_IndicatorPosition(_float4 _Position)
{
    m_vPosition = _Position;

    m_pItemBox->Set_State(TRANSFORM::POSITION, m_vPosition);
}

shared_ptr<CUI_Indicator_Impulse> CUI_Indicator_Impulse::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Impulse> pInstance = make_private_shared(CUI_Indicator_Impulse, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Impulse::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Impulse::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Impulse> pInstance = make_private_shared_copy(CUI_Indicator_Impulse, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Impulse::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
