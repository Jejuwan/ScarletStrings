#include "ClientPCH.h"
#include "UI_Indicator_Cursor.h"
#include "GameInstance.h" 
#include "UI_Manager.h"

CUI_Indicator_Cursor::CUI_Indicator_Cursor(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Cursor::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Cursor::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Cursor::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Cursor::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::CURSOR);

    Set_CursorPos();
}

HRESULT CUI_Indicator_Cursor::Render()
{
    if (1 == m_iShow_Cursor)
    {
        if (true == m_bIsClick)
        {
            /* Cursor_On */
            if (FAILED(m_pTexture_Cursor_On->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pCursor_On->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CGameObject::Render");
            }
        }
        else
        {
            /* Cursor_Off */
            if (FAILED(m_pTexture_Cursor_Off->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pCursor_Off->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Render", "Failed to CGameObject::Render");
            }
        }
    }

    
    return S_OK;
}

HRESULT CUI_Indicator_Cursor::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Cursor::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Cursor::Initialize_UI()
{
//=========================================================================== _bool
    m_bIsClick              = false;
    
//=========================================================================== _int
    m_iShow_Cursor          = -1;

//=========================================================================== Transform
    m_pCursor_Off           = CTransform::Create(m_pDevice, m_pContext);
    m_pCursor_On            = CTransform::Create(m_pDevice, m_pContext);
//=========================================================================== Texture
    m_pTexture_Cursor_Off   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Cursor_1.png"));
    m_pTexture_Cursor_On    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Cursor_2.png"));

//=========================================================================== Set_Scale
    m_vScale_Cursor_Off     = _float3(50.f, 50.f, 1.f);
    m_vScale_Cursor_On      = _float3(55.f, 55.f, 1.f);
    m_pCursor_Off->Set_Scale(m_vScale_Cursor_Off);
    m_pCursor_On->Set_Scale(m_vScale_Cursor_On);

//=========================================================================== Set_Position
    m_ptCursorPos           = CGameInstance::Get_Instance()->Get_CursorPos();
    m_vPosition_Cursor_Off  = _float4((_float)m_ptCursorPos.x - g_iWinCX * 0.5f, g_iWinCY - (_float)m_ptCursorPos.y  * 0.5f, 0.f, 1.f);
    m_vPosition_Cursor_On   = _float4((_float)m_ptCursorPos.x - g_iWinCX * 0.5f, g_iWinCY - (_float)m_ptCursorPos.y  * 0.5f, 0.f, 1.f);
    m_pCursor_Off->Set_State(TRANSFORM::POSITION, m_vPosition_Cursor_Off);
    m_pCursor_On->Set_State(TRANSFORM::POSITION, m_vPosition_Cursor_On);

}

void CUI_Indicator_Cursor::Set_CursorPos()
{
#pragma region Key_Control
    if (CGameInstance::Get_Instance()->Key_Down(VK_HOME) || 1 == CUI_Manager::Get_Instance()->Get_MenuSwitch())
    {
        m_iShow_Cursor = 1;
    }
    else
    {
        m_iShow_Cursor = -1;
    }

    if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON) || CGameInstance::Get_Instance()->Key_Hold(VK_LBUTTON))
    {
        m_bIsClick = true;
        CUI_Manager::Get_Instance()->Set_MouseClick(true);
    }
    else if (CGameInstance::Get_Instance()->Key_Up(VK_LBUTTON))
    {
        m_bIsClick = false;
        CUI_Manager::Get_Instance()->Set_MouseClick(false);
    }
#pragma endregion




    m_ptCursorPos           = CGameInstance::Get_Instance()->Get_CursorPos();
    m_vPosition_Cursor_Off  = _float4((_float)m_ptCursorPos.x - g_iWinCX * 0.5f, g_iWinCY * 0.5f - (_float)m_ptCursorPos.y, 0.f, 1.f);
    m_vPosition_Cursor_On   = _float4((_float)m_ptCursorPos.x - g_iWinCX * 0.5f, g_iWinCY * 0.5f - (_float)m_ptCursorPos.y, 0.f, 1.f);
    m_pCursor_Off->Set_State(TRANSFORM::POSITION, m_vPosition_Cursor_Off);
    m_pCursor_On->Set_State(TRANSFORM::POSITION, m_vPosition_Cursor_On);
}

shared_ptr<CUI_Indicator_Cursor> CUI_Indicator_Cursor::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Cursor> pInstance = make_private_shared(CUI_Indicator_Cursor, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Cursor::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Cursor::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Cursor> pInstance = make_private_shared_copy(CUI_Indicator_Cursor, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Cursor::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
