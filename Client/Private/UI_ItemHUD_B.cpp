#include "ClientPCH.h"
#include "UI_ItemHUD_B.h"
#include "GameInstance.h"
#include "UI_Manager.h"  

CUI_ItemHUD_B::CUI_ItemHUD_B(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_ItemHUD_B::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_ItemHUD_B::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_pItemHUD_B = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemHUD_B.png"));

    Initialize_UI();

    m_pItemHUD_B->Set_Scale(m_vScale_ItemHUD_B);
    m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_ItemHUD_B::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_ItemHUD_B::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_2);

    UIBounce_System(fTimeDelta);
}

HRESULT CUI_ItemHUD_B::Render()
{
    if (SCENE::TEST != CGameInstance::Get_Instance()->Current_Scene())
    {
        if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
            && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
            && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
            && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
            && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
            && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
        {
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pItemHUD_B->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Render", "Failed to CTexture::Bind_OnShader");
            }

            if (FAILED(__super::Render(62)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Render", "Failed to CGameObject::Render");
            }
        }
    }
 
    return S_OK;
}

HRESULT CUI_ItemHUD_B::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemHUD_B::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_ItemHUD_B::Initialize_UI()
{
    m_vScale_ItemHUD_B          = _float3(44.f, 22.f, 0.f);
    m_vPosition_ItemHUD_B       = _float4(-710.f, -430.f, 0.f, 1.f);
    m_vPrevPosition_ItemHUD_B   = m_vPosition_ItemHUD_B;

#pragma region Bounce
    m_bLeft                         = false;
    m_bRight                        = false;
    m_bTop                          = false;
    m_bBottom                       = false;
    m_bUIBounce                     = false;
    m_fOriginalPosX                 = m_vPosition_ItemHUD_B.x;
    m_fOriginalPosY                 = m_vPosition_ItemHUD_B.y;
    m_fMovedPositionX_Left          = m_vPosition_ItemHUD_B.x + 10.f;
    m_fMovedPositionX_Right         = m_vPosition_ItemHUD_B.x - 10.f;
    m_fMovedPositionY_Top           = m_vPosition_ItemHUD_B.y - 5.f;
    m_fMovedPositionY_Bottom        = m_vPosition_ItemHUD_B.y + 5.f;
#pragma endregion

}

void CUI_ItemHUD_B::UIBounce_System(_float fTimeDelta)
{
    m_ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();

#pragma region Control
    if (0 < m_ptCursorMove.x)
    {
        m_bRight = true;
    }
    if (0 > m_ptCursorMove.x)
    {
        m_bLeft = true;
    }
    if (0 > m_ptCursorMove.y)
    {
        m_bTop = true;
    }
    if (0 < m_ptCursorMove.y)
    {
        m_bBottom = true;
    }
#pragma endregion

    // Left
    if (true == m_bLeft)
    {
        if (0 != m_ptCursorMove.x)
        {
            if (-100 > m_ptCursorMove.x)
            {
                m_vPosition_ItemHUD_B.x += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                if (m_vPosition_ItemHUD_B.x > m_fMovedPositionX_Left)
                {
                    m_vPosition_ItemHUD_B.x = m_fMovedPositionX_Left;
                    m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);
                }
            }
        }
        else
        {
            m_vPosition_ItemHUD_B.x -= fTimeDelta * 30.f;
            m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);


            if (m_vPosition_ItemHUD_B.x < m_fOriginalPosX)
            {
                m_vPosition_ItemHUD_B.x = m_fOriginalPosX;
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                m_bLeft = false;
            }
        }
    }
    // Right
    else if (true == m_bRight)
    {
        if (0 != m_ptCursorMove.x)
        {
            if (100 < m_ptCursorMove.x)
            {
                m_vPosition_ItemHUD_B.x -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                if (m_vPosition_ItemHUD_B.x < m_fMovedPositionX_Right)
                {
                    m_vPosition_ItemHUD_B.x = m_fMovedPositionX_Right;
                    m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);
                }
            }
        }
        else
        {
            m_vPosition_ItemHUD_B.x += fTimeDelta * 30.f;
            m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);


            if (m_vPosition_ItemHUD_B.x > m_fOriginalPosX)
            {
                m_vPosition_ItemHUD_B.x = m_fOriginalPosX;
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                m_bRight = false;
            }
        }
    }

    // Top
    if (true == m_bTop)
    {
        if (0 != m_ptCursorMove.y)
        {
            if (-50 > m_ptCursorMove.y)
            {
                m_vPosition_ItemHUD_B.y -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                if (m_vPosition_ItemHUD_B.y < m_fMovedPositionY_Top)
                {
                    m_vPosition_ItemHUD_B.y = m_fMovedPositionY_Top;
                    m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);
                }
            }
        }
        else
        {
            m_vPosition_ItemHUD_B.y += fTimeDelta * 30.f;
            m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);


            if (m_vPosition_ItemHUD_B.y > m_fOriginalPosY)
            {
                m_vPosition_ItemHUD_B.y = m_fOriginalPosY;
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                m_bTop = false;
            }
        }
    }
    // Bottom
    else if (true == m_bBottom)
    {
        if (0 != m_ptCursorMove.y)
        {
            if (50 < m_ptCursorMove.y)
            {
                m_vPosition_ItemHUD_B.y += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                if (m_vPosition_ItemHUD_B.y > m_fMovedPositionY_Bottom)
                {
                    m_vPosition_ItemHUD_B.y = m_fMovedPositionY_Bottom;
                    m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);
                }
            }
        }
        else
        {
            m_vPosition_ItemHUD_B.y -= fTimeDelta * 30.f;
            m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);


            if (m_vPosition_ItemHUD_B.y < m_fOriginalPosY)
            {
                m_vPosition_ItemHUD_B.y = m_fOriginalPosY;
                m_pItemHUD_B->Set_State(TRANSFORM::POSITION, m_vPosition_ItemHUD_B);

                m_bBottom = false;
            }
        }
    }
}


shared_ptr<CUI_ItemHUD_B> CUI_ItemHUD_B::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_ItemHUD_B> pInstance = make_private_shared(CUI_ItemHUD_B, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_ItemHUD_B::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_ItemHUD_B::Clone(any _aDesc)
{
    shared_ptr<CUI_ItemHUD_B> pInstance = make_private_shared_copy(CUI_ItemHUD_B, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_ItemHUD_B::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
