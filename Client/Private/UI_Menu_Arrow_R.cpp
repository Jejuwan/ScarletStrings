#include "ClientPCH.h"
#include "UI_Menu_Arrow_R.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define ARROW_UP_SCALE          _float3(13.f, 17.f, 1.f)
#define ARROW_UP_POSITION       _float4(880.f, 430.f, 0.f, 1.f) // 820
#define ARROW_DOWN_SCALE        _float3(13.f, 17.f, 1.f)
#define ARROW_DOWN_POSITION     _float4(880.f, 360.f, 0.f, 1.f)

CUI_Menu_Arrow_R::CUI_Menu_Arrow_R(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Arrow_R::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Arrow_R::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Initialize", "Failed to CGameObject::Initialize");
    }


    m_pArrow_Up            = CTransform::Create(m_pDevice, m_pContext);
    m_pArrow_Down          = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture_Up          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png"));
    m_pTexture_Down        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Arrow_R.png"));

    m_pArrow_Up->Set_Scale(ARROW_UP_SCALE);
    m_pArrow_Up->Set_State(TRANSFORM::POSITION, ARROW_UP_POSITION);
    m_pArrow_Down->Set_Scale(ARROW_DOWN_SCALE);
    m_pArrow_Down->Set_State(TRANSFORM::POSITION, ARROW_DOWN_POSITION);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Arrow_R::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Arrow_R::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_Menu_Arrow_R::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (FAILED(m_pTexture_Up->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pArrow_Up->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(5)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CGameObject::Render");
        }

        
        if (1 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            if (FAILED(m_pTexture_Down->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pArrow_Down->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Arrow_R::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Arrow_R::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Menu_Arrow_R> CUI_Menu_Arrow_R::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Arrow_R> pInstance = make_private_shared(CUI_Menu_Arrow_R, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Arrow_R::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Arrow_R::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Arrow_R> pInstance = make_private_shared_copy(CUI_Menu_Arrow_R, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Arrow_R::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
