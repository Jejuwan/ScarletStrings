#include "ClientPCH.h"
#include "UI_Shop.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "RenderTarget_Manager.h"

#define SCALE_BACK                  _float3(g_iWinCX, g_iWinCY, 1.f)
#define POSITION_BACK               _float4(0.f, 0.f, 0.9f, 1.f)

#define BACKGROUND_FRAME_SCALE      _float3(g_iWinCX, g_iWinCY, 1.f)
#define BACKGROUND_FRAME_POSITION   _float4(0.f, 0.f, 0.8f, 1.f)

#define SEPERATELINE_SCALE          _float3(g_iWinCX, 2.f, 1.f)
#define SEPERATELINE_POSITION       _float4(0.f, 400.f, 0.0f, 1.f)

#define SCALE_INFO_L                _float3(1800.f, 60.f, 1.f)
#define POSITION_INFO_L             _float4(0.f, -430.f, 0.6f, 1.f)

CUI_Shop::CUI_Shop(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Shop::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_2, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Shop::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop::Initialize", "Failed to CGameObject::Initialize");
    }

    m_pBackground           = CTransform::Create(m_pDevice, m_pContext);
    m_pInfo_L               = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture_Background   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Background.png"));
    m_pTexture_Info_L       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Info_L.dds"));

    m_pBackground->Set_Scale(SCALE_BACK);
    m_pInfo_L->Set_Scale(SCALE_INFO_L);

    m_pBackground->Set_State(TRANSFORM::POSITION, POSITION_BACK);
    m_pInfo_L->Set_State(TRANSFORM::POSITION, POSITION_INFO_L);

    return S_OK;
}

void CUI_Shop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Shop::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_Shop::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        /* Background */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }

        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(6)))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CGameObject::Render");
        }

        /* Info_L */
        if (FAILED(m_pTexture_Info_L->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pInfo_L->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(4)))
        {
            MSG_RETURN(E_FAIL, "CUI_Shop::Render", "Failed to CGameObject::Render");
        }
    }

    return S_OK;
}

HRESULT CUI_Shop::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Shop::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop::Ready_Components", "Failed to Get_Component: SHADER");
    }

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Shop::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}
shared_ptr<CUI_Shop> CUI_Shop::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Shop> pInstance = make_private_shared(CUI_Shop, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Shop::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Shop::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Shop> pInstance = make_private_shared_copy(CUI_Shop, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Shop::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
