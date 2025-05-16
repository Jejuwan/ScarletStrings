#include "ClientPCH.h"
#include "UI_Menu_Category_Library.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#define BACKGROUND_SCALE_ON                 _float3(198.f, 67.f, 1.f)
#define BACKGROUND_POSITION_ON              _float4(510.f, 440.f, 0.5f, 1.f)

#define BACKGROUND_SCALE_OFF                _float3(186.f, 56.f, 1.f)
#define BACKGROUND_POSITION_OFF             _float4(512.f, 437.f, 0.5f, 1.f)

CUI_Menu_Category_Library::CUI_Menu_Category_Library(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Category_Library::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Category_Library::Initialize(any _pPlayerEntityDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_pBackground = CTransform::Create(m_pDevice, m_pContext);
    //m_pHP               = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture_On = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOn.png"));
    m_pTexture_Off = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/CategoryOff.dds"));

    m_pBackground->Set_Scale(BACKGROUND_SCALE_OFF);
    m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION_OFF);

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Category_Library::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


}

void CUI_Menu_Category_Library::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_2);

    if (CGameInstance::Get_Instance()->Key_Down(0x31)) // 1
    {
        if (6 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(BACKGROUND_SCALE_ON);
            m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION_ON);
        }
        else
        {
            m_pBackground->Set_Scale(BACKGROUND_SCALE_OFF);
            m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION_OFF);
        }
    }
    if (CGameInstance::Get_Instance()->Key_Down(0x33)) // 3
    {
        if (6 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            m_pBackground->Set_Scale(BACKGROUND_SCALE_ON);
            m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION_ON);
        }
        else
        {
            m_pBackground->Set_Scale(BACKGROUND_SCALE_OFF);
            m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION_OFF);
        }
    }

}

HRESULT CUI_Menu_Category_Library::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_strFont = TEXT("???"); //TEXT("라이브러리");

        if (6 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            if (FAILED(m_pTexture_On->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFont, _float2(1451.f, 89.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
        }
        else
        {
            if (FAILED(m_pTexture_Off->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(8)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Render", "Failed to CGameObject::Render");
            }

            CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strFont, _float2(1452.f, 94.f), XMVectorSet(0.96f, 0.81f, 0.74f, 1.0f), 0.5f);
        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Category_Library::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Category_Library::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Menu_Category_Library> CUI_Menu_Category_Library::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Category_Library> pInstance = make_private_shared(CUI_Menu_Category_Library, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Category_Library::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Category_Library::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Category_Library> pInstance = make_private_shared_copy(CUI_Menu_Category_Library, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Category_Library::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
