#include "ClientPCH.h"
#include "UI_Menu_Button.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define BUTTON_1_SCALE          _float3(28.f, 28.f, 1.f)
#define BUTTON_1_POSITION       _float4(-870.f, 435.f, 0.f, 1.f)
#define BUTTON_3_SCALE          _float3(28.f, 28.f, 1.f)
#define BUTTON_3_POSITION       _float4(848.f, 435.f, 0.f, 1.f)
//#define BUTTON_Q_SCALE          _float3(13.f, 17.f, 1.f)
//#define BUTTON_Q_POSITION       _float4(888.f, 360.f, 0.f, 1.f)
//#define BUTTON_E_SCALE          _float3(13.f, 17.f, 1.f)
//#define BUTTON_E_POSITION       _float4(888.f, 360.f, 0.f, 1.f)


CUI_Menu_Button::CUI_Menu_Button(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Button::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Button::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Button::Initialize", "Failed to CGameObject::Initialize");
    }


    m_pButton_1           = CTransform::Create(m_pDevice, m_pContext);
    m_pButton_3           = CTransform::Create(m_pDevice, m_pContext);
    //m_pButton_Q           = CTransform::Create(m_pDevice, m_pContext);
    //m_pButton_E           = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture_1          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/1.png"));
    m_pTexture_3          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/3.png"));
    //m_pTexture_Q          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Q.png"));
    //m_pTexture_E          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/E.png"));


    m_pButton_1->Set_Scale(BUTTON_1_SCALE);
    m_pButton_1->Set_State(TRANSFORM::POSITION, BUTTON_1_POSITION);
    m_pButton_3->Set_Scale(BUTTON_3_SCALE);
    m_pButton_3->Set_State(TRANSFORM::POSITION, BUTTON_3_POSITION);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Button::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Button::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Button::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_Menu_Button::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (FAILED(m_pTexture_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pButton_1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CGameObject::Render");
        }

        if (FAILED(m_pTexture_3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pButton_3->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Button::Render", "Failed to CGameObject::Render");
        }
        

    }

    return S_OK;
}

HRESULT CUI_Menu_Button::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Button::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Button::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Menu_Button> CUI_Menu_Button::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Button> pInstance = make_private_shared(CUI_Menu_Button, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Button::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Button::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Button> pInstance = make_private_shared_copy(CUI_Menu_Button, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Button::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
