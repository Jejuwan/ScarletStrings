#include "ClientPCH.h"
#include "UI_Menu_Fillter.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Menu_Fillter::CUI_Menu_Fillter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Fillter::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Fillter::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Initialize", "Failed to CGameObject::Initialize");
    }
    
    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_vScale                = _float3(182.f, 50.f, 1.f);
    m_vPosition             = _float4(-749.f, 438.f, 0.7f, 1.f);
    m_vHighLightScale       = _float3(152.f, 10.f, 1.f);
    m_vHighLightPosition    = _float4(-749.f, 425.f, 0.6f, 1.f); 


    m_pFillter              = CTransform::Create(m_pDevice, m_pContext);
    m_pFillter_HighLight    = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/MenuFillter.png"));
    m_pTexture_HighLight    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/HighLight.png"));

    m_pFillter->Set_Scale(m_vScale);
    m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
    m_pFillter_HighLight->Set_Scale(m_vHighLightScale);
    m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Fillter::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Fillter::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_1);

    KeyWork();

    if (CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        Shop_KeyWork();
    }
}

HRESULT CUI_Menu_Fillter::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pFillter->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CGameObject::Render");
        }


        if (FAILED(m_pTexture_HighLight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pFillter_HighLight->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Render", "Failed to CGameObject::Render");
        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Fillter::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Fillter::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Menu_Fillter::KeyWork()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        /*m_vPosition = _float4(-749.f, 438.f, 0.9f, 1.f);
        m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);

        m_vHighLightPosition = _float4(-749.f, 425.f, 0.6f, 1.f);
        m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);*/

        if (CGameInstance::Get_Instance()->Key_Down(0x31))
        {
            CUI_Manager::Get_Instance()->Set_MenuIndexMlnus();
            _int iIndex = CUI_Manager::Get_Instance()->Get_MenuIndex();

            switch (iIndex)
            {
            case 0:
            {
                m_vPosition = _float4(-749.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-749.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 1:
            {
                m_vPosition = _float4(-539.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-539.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 2:
            {
                m_vPosition = _float4(-329.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-329.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 3:
            {
                m_vPosition = _float4(-119.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-119.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 4:
            {
                m_vPosition = _float4(91.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(91.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 5:
            {
                m_vPosition = _float4(301.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(301.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 6:
            {
                m_vPosition = _float4(511.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(511.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 7:
            {
                m_vPosition = _float4(721.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(721.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;

            default:
                break;
            }

        }
        if (CGameInstance::Get_Instance()->Key_Down(0x33))
        {
            CUI_Manager::Get_Instance()->Set_MenuIndexPlus();
            _int iIndex = CUI_Manager::Get_Instance()->Get_MenuIndex();

            switch (iIndex)
            {
            case 0:
            {
                m_vPosition = _float4(-749.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-749.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 1:
            {
                m_vPosition = _float4(-539.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-539.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 2:
            {
                m_vPosition = _float4(-329.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-329.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 3:
            {
                m_vPosition = _float4(-119.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(-119.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 4:
            {
                m_vPosition = _float4(91.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(91.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 5:
            {
                m_vPosition = _float4(301.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(301.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 6:
            {
                m_vPosition = _float4(511.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(511.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;
            case 7:
            {
                m_vPosition = _float4(721.f, 438.f, 0.9f, 1.f);
                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
                m_vHighLightPosition = _float4(721.f, 425.f, 0.8f, 1.f);
                m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
            }
            break;

            default:
                break;
            }
        }
    }
}

void CUI_Menu_Fillter::Shop_KeyWork()
{
    _int iIndex = CUI_Manager::Get_Instance()->Get_ShopMenu_Index();

    switch (iIndex)
    {
    case 0:
    {
        m_vPosition = _float4(-749.f, 438.f, 0.9f, 1.f);
        m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
        m_vHighLightPosition = _float4(-749.f, 425.f, 0.8f, 1.f);
        m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
    }
    break;
    case 1:
    {
        m_vPosition = _float4(-539.f, 438.f, 0.9f, 1.f);
        m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition);
        m_vHighLightPosition = _float4(-539.f, 425.f, 0.8f, 1.f);
        m_pFillter_HighLight->Set_State(TRANSFORM::POSITION, m_vHighLightPosition);
    }
    break;
    default:
        break;
    }
}

shared_ptr<CUI_Menu_Fillter> CUI_Menu_Fillter::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Fillter> pInstance = make_private_shared(CUI_Menu_Fillter, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Fillter::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Fillter::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Fillter> pInstance = make_private_shared_copy(CUI_Menu_Fillter, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Fillter::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
