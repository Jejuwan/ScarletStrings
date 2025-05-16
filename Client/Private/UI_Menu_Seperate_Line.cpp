#include "ClientPCH.h"
#include "UI_Menu_Seperate_Line.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Menu_Seperate_Line::CUI_Menu_Seperate_Line(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Menu_Seperate_Line::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Menu_Seperate_Line::Initialize(any _pPlayerEntityDesc)
{    
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_vScale                = _float3(513.f, 60.f, 1.f);
    m_vPosition             = _float4(-670.f, -78.f, 0.9f, 1.f);

    m_pSeperate_Line        = CTransform::Create(m_pDevice, m_pContext);
    m_pTexture              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Menu/Seperate_Line.png"));

    m_pSeperate_Line->Set_Scale(m_vScale);
    m_pSeperate_Line->Set_State(TRANSFORM::POSITION, m_vPosition);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Menu_Seperate_Line::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Menu_Seperate_Line::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);
}

HRESULT CUI_Menu_Seperate_Line::Render()
{
    if (true == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        if (0 == CUI_Manager::Get_Instance()->Get_MenuIndex())
        {
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSeperate_Line->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(10)))
            {
                MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Menu_Seperate_Line::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Seperate_Line::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Menu_Seperate_Line> CUI_Menu_Seperate_Line::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Menu_Seperate_Line> pInstance = make_private_shared(CUI_Menu_Seperate_Line, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Seperate_Line::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Menu_Seperate_Line::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Menu_Seperate_Line> pInstance = make_private_shared_copy(CUI_Menu_Seperate_Line, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Menu_Seperate_Line::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
