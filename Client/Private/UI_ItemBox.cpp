#include "ClientPCH.h"
#include "UI_ItemBox.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"

#define ITEMBOX_SCALE               _float3(260.f, 90.f, 0.f)

#define ITEM_POTION_SCALE           _float3(25.f, 40.f, 0.f)

#define ITEM_SHADER_SCALE           _float3(75.f, 75.f, 0.f)

CUI_ItemBox::CUI_ItemBox(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_ItemBox::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_ItemBox::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemBox::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_fRatio                        = 0.f;
    m_fGauge                        = 0.f;
    m_fGaugeMax                     = 100.f;

    _matrix PivotMatrix = XMMatrixIdentity();
    PivotMatrix = XMMatrixRotationZ(XMConvertToRadians(90.0f));

    m_vItemCoolTimeGauge_Scale      = _float3(75.f, 75.f, 0.f);
    m_vItemCoolTimeGauge_Position   = _float4(-710.f, -337.f, 0.f, 1.f);
    m_vPosition_ItemBox             = _float4(-710.f, -340.f, 0.f, 1.f);
    m_vPosition_ItemPotion          = _float4(-710.f, -337.f, 0.f, 1.f);
    m_vPosition_ItemShader          = _float4(-708.f, -337.f, 0.f, 1.f);


    m_pItemBox                      = CTransform::Create(m_pDevice, m_pContext);
    m_pItemPotion                   = CTransform::Create(m_pDevice, m_pContext);
    m_pItemShader                   = CTransform::Create(m_pDevice, m_pContext);
    m_pItemCoolTimeGauge            = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture                      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemBox2.png"));
    m_pTexture_ItemPotion           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/Item1.png"));
    m_pTexture_ItemShader           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/Item_Shader.png"));
    m_pTexture_ItemCoolTimeGauge    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Item/ItemCoolOn2.png"));

    m_pItemBox->Set_Scale(ITEMBOX_SCALE);
    m_pItemPotion->Set_Scale(ITEM_POTION_SCALE);
    m_pItemShader->Set_Scale(ITEM_SHADER_SCALE);
    m_pItemCoolTimeGauge->Set_Matrix(PivotMatrix);
    m_pItemCoolTimeGauge->Set_Scale(m_vItemCoolTimeGauge_Scale);

    m_pItemBox->Set_State(TRANSFORM::POSITION, m_vPosition_ItemBox);
    m_pItemPotion->Set_State(TRANSFORM::POSITION, m_vPosition_ItemPotion);
    m_pItemShader->Set_State(TRANSFORM::POSITION, m_vPosition_ItemShader);
    m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

    Initialize_UI();

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemBox::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }


    return S_OK;
}

void CUI_ItemBox::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_ItemBox::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    ItemSystem(fTimeDelta);
    Use_Potion();
    UIBounce_System(fTimeDelta);
}

HRESULT CUI_ItemBox::Render()
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
            /* ItemBox Slot*/
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pItemBox->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CGameObject::Render");
            }

            /* ItemPotion */
            if (FAILED(m_pTexture_ItemPotion->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pItemPotion->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1))) // 27
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CGameObject::Render");
            }

            if (true == m_bRenderSwitch)
            {
                /* ItemShader */
                if (FAILED(m_pTexture_ItemShader->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pItemShader->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(13)))
                {
                    MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CGameObject::Render");
                }
            }

            /* ItemCoolTimeGauge */
            if (FAILED(m_pTexture_ItemCoolTimeGauge->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pItemCoolTimeGauge->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fRatio, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CGameObject::Render");
            }
            if (FAILED(__super::Render(14)))
            {
                MSG_RETURN(E_FAIL, "CUI_ItemBox::Render", "Failed to CGameObject::Render");
            }
        }
    }
    
    return S_OK;
}

void CUI_ItemBox::Initialize_UI()
{
#pragma region Bounce
    m_bLeft                         = false;
    m_bRight                        = false;
    m_bTop                          = false;
    m_bBottom                       = false;
    m_bUIBounce                     = false;
    m_fOriginalPosX                 = m_vItemCoolTimeGauge_Position.x;
    m_fOriginalPosY                 = m_vItemCoolTimeGauge_Position.y;
    m_fMovedPositionX_Left          = m_vItemCoolTimeGauge_Position.x + 10.f;
    m_fMovedPositionX_Right         = m_vItemCoolTimeGauge_Position.x - 10.f;
    m_fMovedPositionY_Top           = m_vItemCoolTimeGauge_Position.y - 5.f;
    m_fMovedPositionY_Bottom        = m_vItemCoolTimeGauge_Position.y + 5.f;
#pragma endregion

    m_vPrevItemCoolTimeGauge_Position = m_vItemCoolTimeGauge_Position;

    m_iPlayerHpMax = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
}

void CUI_ItemBox::ItemSystem(_float fTimeDelta)
{
    m_fRatio = m_fGauge / m_fGaugeMax;

    m_fGauge += fTimeDelta * 50.f;

    if (1.f <= m_fRatio)
    {
        m_fRatio = 1.f;
        m_bRenderSwitch = true;
    }
}

void CUI_ItemBox::Use_Potion()
{
    if (m_iPlayerHpMax != static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp)
    {
        if (CGameInstance::Get_Instance()->Key_Down('T'))
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_06.Use_ItemSlot.wav", 1.f);
            CUI_Manager::Get_Instance()->Set_ItemCountMinus();
            CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_HEAL_FONT)->Pop();

            m_fGauge = 0.f;
            m_bRenderSwitch = false;
        }
    }
    else
    {
        if (CGameInstance::Get_Instance()->Key_Down('T'))
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_06.Use_Potion_Full.wav", 0.5f);
        }
    }

}

void CUI_ItemBox::Get_Potion()
{
    CUI_Manager::Get_Instance()->Set_ItemIndexPlus();
}

void CUI_ItemBox::UIBounce_System(_float fTimeDelta)
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
                m_vItemCoolTimeGauge_Position.x += fTimeDelta * abs(m_ptCursorMove.x * 0.25f);
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

                if (m_vItemCoolTimeGauge_Position.x > m_fMovedPositionX_Left)
                {
                    m_vItemCoolTimeGauge_Position.x = m_fMovedPositionX_Left;
                    m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);
                }
            }
        }
        else
        {
            m_vItemCoolTimeGauge_Position.x -= fTimeDelta * 30.f;
            m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);


            if (m_vItemCoolTimeGauge_Position.x < m_fOriginalPosX)
            {
                m_vItemCoolTimeGauge_Position.x = m_fOriginalPosX;
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

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
                m_vItemCoolTimeGauge_Position.x -= fTimeDelta * abs(m_ptCursorMove.x * 0.25f);
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

                if (m_vItemCoolTimeGauge_Position.x < m_fMovedPositionX_Right)
                {
                    m_vItemCoolTimeGauge_Position.x = m_fMovedPositionX_Right;
                    m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);
                }
            }
        }
        else
        {
            m_vItemCoolTimeGauge_Position.x += fTimeDelta * 30.f;
            m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);


            if (m_vItemCoolTimeGauge_Position.x > m_fOriginalPosX)
            {
                m_vItemCoolTimeGauge_Position.x = m_fOriginalPosX;
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

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
                m_vItemCoolTimeGauge_Position.y -= fTimeDelta * abs(m_ptCursorMove.y * 0.25f);
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

                if (m_vItemCoolTimeGauge_Position.y < m_fMovedPositionY_Top)
                {
                    m_vItemCoolTimeGauge_Position.y = m_fMovedPositionY_Top;
                    m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);
                }
            }
        }
        else
        {
            m_vItemCoolTimeGauge_Position.y += fTimeDelta * 30.f;
            m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);


            if (m_vItemCoolTimeGauge_Position.y > m_fOriginalPosY)
            {
                m_vItemCoolTimeGauge_Position.y = m_fOriginalPosY;
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

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
                m_vItemCoolTimeGauge_Position.y += fTimeDelta * abs(m_ptCursorMove.y * 0.25f);
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

                if (m_vItemCoolTimeGauge_Position.y > m_fMovedPositionY_Bottom)
                {
                    m_vItemCoolTimeGauge_Position.y = m_fMovedPositionY_Bottom;
                    m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);
                }
            }
        }
        else
        {
            m_vItemCoolTimeGauge_Position.y -= fTimeDelta * 30.f;
            m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);


            if (m_vItemCoolTimeGauge_Position.y < m_fOriginalPosY)
            {
                m_vItemCoolTimeGauge_Position.y = m_fOriginalPosY;
                m_pItemCoolTimeGauge->Set_State(TRANSFORM::POSITION, m_vItemCoolTimeGauge_Position);

                m_bBottom = false;
            }
        }
    }
}

HRESULT CUI_ItemBox::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_ItemBox::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_ItemBox::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_ItemBox> CUI_ItemBox::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_ItemBox> pInstance = make_private_shared(CUI_ItemBox, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_ItemBox::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_ItemBox::Clone(any _aDesc)
{
    shared_ptr<CUI_ItemBox> pInstance = make_private_shared_copy(CUI_ItemBox, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_ItemBox::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
