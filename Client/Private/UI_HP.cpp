#include "ClientPCH.h"
#include "UI_HP.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"
#include "PlayerState.h"

CUI_HP::CUI_HP(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_HP::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_HP::Initialize(any _pPlayerEntityDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    
    m_pBlankHP          = CTransform::Create(m_pDevice, m_pContext);
    m_pHP1              = CTransform::Create(m_pDevice, m_pContext);

    m_pSlash            = CTransform::Create(m_pDevice, m_pContext);

    m_pOnes             = CTransform::Create(m_pDevice, m_pContext);
    m_pTens             = CTransform::Create(m_pDevice, m_pContext);
    m_pHundreds         = CTransform::Create(m_pDevice, m_pContext);
    m_pThousands        = CTransform::Create(m_pDevice, m_pContext);
    
    m_pOnesMax          = CTransform::Create(m_pDevice, m_pContext);
    m_pTensMax          = CTransform::Create(m_pDevice, m_pContext);
    m_pHundredsMax      = CTransform::Create(m_pDevice, m_pContext);
    m_pThousandsMax     = CTransform::Create(m_pDevice, m_pContext);


    m_pTexture_BlankHP  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Blank_HP.png"));
    m_pTexture_HP1      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_1.png"));
    m_pTexture_HP80     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_80.png"));
    m_pTexture_HP60     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_60.png"));
    m_pTexture_HP40     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/HP_Wave_40.png"));

    m_pTexture_Slash    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/Slash.png"));

    m_pTexture_Zero     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/0.png"));
    m_pTexture_One      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/1.png"));
    m_pTexture_Two      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/2.png"));
    m_pTexture_Three    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/3.png"));
    m_pTexture_Four     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/4.png"));
    m_pTexture_Five     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/5.png"));
    m_pTexture_Six      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/6.png"));
    m_pTexture_Seven    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/7.png"));
    m_pTexture_Eight    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/8.png"));
    m_pTexture_Nine     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/HP_Wave/Number/9.png"));


    Initialize_UI();


    /* Scale */
    m_pBlankHP->Set_Scale(m_vScale_HPBlank);
    m_pHP1->Set_Scale(m_vScale_HP);

    m_pSlash->Set_Scale(m_vScale_Slash);

    m_pOnes->Set_Scale(m_vScale_HPOnes);
    m_pTens->Set_Scale(m_vScale_HPTens);
    m_pHundreds->Set_Scale(m_vScale_HPHundreds);
    m_pThousands->Set_Scale(m_vScale_HPThousands);

    m_pOnesMax->Set_Scale(m_vScale_HPOnesMax);
    m_pTensMax->Set_Scale(m_vScale_HPTensMax);
    m_pHundredsMax->Set_Scale(m_vScale_HPHundredsMax);
    m_pThousandsMax->Set_Scale(m_vScale_HPThousandsMax);

    /* Position */
    m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank);
    m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

    m_pSlash->Set_State(TRANSFORM::POSITION, m_vPosition_Slash);

    m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_HPOnes);
    m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_HPTens);
    m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_HPHundreds);
    m_pThousands->Set_State(TRANSFORM::POSITION, m_vPosition_HPThousands);

    m_pOnesMax->Set_State(TRANSFORM::POSITION, m_vPosition_HPOnesMax);
    m_pTensMax->Set_State(TRANSFORM::POSITION, m_vPosition_HPTensMax);
    m_pHundredsMax->Set_State(TRANSFORM::POSITION, m_vPosition_HPHundredsMax);
    m_pThousandsMax->Set_State(TRANSFORM::POSITION, m_vPosition_HPThousandsMax);

    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_HP::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    Tick_SpriteTexture(fTimeDelta);
}

void CUI_HP::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_1);
    
    Tick_UI(fTimeDelta);
}

HRESULT CUI_HP::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && false == CUI_Manager::Get_Instance()->Get_TutorialRender()
        && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
        && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
    {
//=====================================================================================================================
        if (false == m_bIsFinishCalculateDamage)
        {
            if (FAILED(m_pTexture_BlankHP->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBlankHP->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bOnOff", &m_bIsFinishCalculateDamage, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(20)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }
//=====================================================================================================================
        if (0.f < m_fRatio)
        {
            if (1.f >= m_fRatio && 0.8f < m_fRatio)
            {
                if (FAILED(m_pTexture_HP1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHP1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
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
                if (FAILED(__super::Render(4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }
            }
            if (0.8f >= m_fRatio && 0.6f < m_fRatio)
            {
                if (FAILED(m_pTexture_HP80->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHP1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_80.m_fTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc_80.m_iIndex, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc_80.m_iIndexMaxX, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc_80.m_iIndexMaxY, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc_80.m_fSizeX, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc_80.m_fSizeY, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }
            }
            if (0.6f >= m_fRatio && 0.4f < m_fRatio)
            {
                if (FAILED(m_pTexture_HP60->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHP1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_60.m_fTime, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc_60.m_iIndex, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc_60.m_iIndexMaxX, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc_60.m_iIndexMaxY, sizeof(_uint))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc_60.m_fSizeX, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc_60.m_fSizeY, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(4)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }
            }
            if (0.4f >= m_fRatio)
            {
                if (FAILED(m_pTexture_HP40->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHP1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(24)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }
            }
        }
//=====================================================================================================================
#pragma region HPMAX - Thousands
        if (0 != m_iThousandsMax)
        {
            switch (m_iThousandsMax)
            {
            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
            if (FAILED(m_pThousandsMax->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }
#pragma endregion

#pragma region HPMAX - Hundreds
        switch (m_iHundredsMax)
        {
        case 0:
        {
            if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 1:
        {
            if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 2:
        {
            if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 3:
        {
            if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 4:
        {
            if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 5:
        {
            if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 6:
        {
            if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 7:
        {
            if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 8:
        {
            if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 9:
        {
            if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        default:
            break;
        }
        if (FAILED(m_pHundredsMax->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
        }
#pragma endregion

#pragma region HPMAX - Tens
        switch (m_iTensMax)
        {
        case 0:
        {
            if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 1:
        {
            if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 2:
        {
            if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 3:
        {
            if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 4:
        {
            if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 5:
        {
            if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 6:
        {
            if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 7:
        {
            if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 8:
        {
            if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 9:
        {
            if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        default:
            break;
        }
        if (FAILED(m_pTensMax->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
        }
#pragma endregion

#pragma region HPMAX - Ones
        switch (m_iOnesMax)
        {
        case 0:
        {
            if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 1:
        {
            if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 2:
        {
            if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 3:
        {
            if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 4:
        {
            if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 5:
        {
            if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 6:
        {
            if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 7:
        {
            if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 8:
        {
            if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 9:
        {
            if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        default:
            break;
        }
        if (FAILED(m_pOnesMax->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
        }
#pragma endregion

        if (FAILED(m_pTexture_Slash->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pSlash->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
        }

#pragma region HP - Thousands
        if (0 != m_iThousands)
        {
            switch (m_iThousands)
            {
            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
            if (FAILED(m_pThousands->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }
#pragma endregion

#pragma region HP - Hundreds
        if (m_fHp >= 100.f)
        {
            switch (m_iHundreds)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
            if (FAILED(m_pHundreds->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }
        else if (100.f > m_fHp)
        {

        }


#pragma endregion

#pragma region HP - Tens
        if (m_fHp >= 10.f)
        {
            switch (m_iTens)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
            if (FAILED(m_pTens->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(1)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
        }
        else if (10.f > m_fHp)
        {

        }

#pragma endregion

#pragma region HP - Ones
        switch (m_iOnes)
        {
        case 0:
        {
            if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 1:
        {
            if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 2:
        {
            if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 3:
        {
            if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 4:
        {
            if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 5:
        {
            if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 6:
        {
            if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 7:
        {
            if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 8:
        {
            if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        case 9:
        {
            if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
        }
        break;

        default:
            break;
        }
        if (FAILED(m_pOnes->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
        }
#pragma endregion


    }

    return S_OK;
}

void CUI_HP::Initialize_UI()
{
//============================================================================== // Texture_Sprite_Desc_Wave1
    ZeroMemory(&m_tUIDesc, sizeof m_tUIDesc);

    m_tUIDesc.m_iIndex              = 0;
    m_tUIDesc.m_iIndexMaxX          = 1;
    m_tUIDesc.m_iIndexMaxY          = 44;
    m_tUIDesc.m_fFramePerSec        = 0.03f;

    m_tUIDesc.m_fTime               = 0.f;
    m_tUIDesc.m_fSizeX              = 290.f;
    m_tUIDesc.m_fSizeY              = 1408.f;
//============================================================================== // Texture_Sprite_Desc_Wave2
    ZeroMemory(&m_tUIDesc_80, sizeof m_tUIDesc_80);

    m_tUIDesc_80.m_iIndex           = 0;
    m_tUIDesc_80.m_iIndexMaxX       = 1;
    m_tUIDesc_80.m_iIndexMaxY       = 30;
    m_tUIDesc_80.m_fFramePerSec     = 0.03f;
             
    m_tUIDesc_80.m_fTime            = 0.f;
    m_tUIDesc_80.m_fSizeX           = 232.f;
    m_tUIDesc_80.m_fSizeY           = 960.f;
//============================================================================== // Texture_Sprite_Desc_Wave3
    ZeroMemory(&m_tUIDesc_60, sizeof m_tUIDesc_60);

    m_tUIDesc_60.m_iIndex           = 0;
    m_tUIDesc_60.m_iIndexMaxX       = 1;
    m_tUIDesc_60.m_iIndexMaxY       = 30;
    m_tUIDesc_60.m_fFramePerSec     = 0.03f;

    m_tUIDesc_60.m_fTime            = 0.f;
    m_tUIDesc_60.m_fSizeX           = 174.f;
    m_tUIDesc_60.m_fSizeY           = 960.f;
//============================================================================== // Default Setting
//============================================================================== For. Test_Initialize
    m_fHp                           = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fHpMax                        = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fRatio                        = m_fHp / m_fHpMax;
    m_fPercentage                   = m_fRatio * 270.f;

    m_fHpBlank                      = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fHpBlankMax                   = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    m_fRatio_Blank                  = m_fHpBlank / m_fHpBlankMax;
    m_fPercentage_Blank             = m_fRatio_Blank * 540.f;
    m_fPrevHP                       = m_fHp;
    
    m_fDamage                       = 0.f;

//============================================================================== Scale
    m_vScale_HP                     = _float3(m_fPercentage, 32.f, 1.f);
    m_vScale_HPBlank                = _float3(m_fPercentage_Blank, 30.f, 1.f);
   
//============================================================================== Position
    m_vPosition_HP                  = _float4(-63.f, -436.f, 0.f, 1.f);
    m_vPosition_HPBlank             = _float4(-198.f, -422.f, 0.f, 1.f);

//============================================================================== For. HP_Bounce
    m_bDash                         = false;
    m_fAccTime                      = 0.f;
    m_fAccDashTime                  = 0.f;
    m_fBounceDistance               = 5.f;

    m_vPrevPosition_HP              = m_vPosition_HP;
    m_vPrevPosition_HPBlank         = m_vPosition_HPBlank;


#pragma region Bounce
    m_bLeft                         = false;
    m_bRight                        = false;
    m_bTop                          = false;
    m_bBottom                       = false;
    m_bUIBounce                     = false;
    m_fOriginalPosX                 = m_vPosition_HP.x;
    m_fOriginalPosY                 = m_vPosition_HP.y;
    m_fMovedPositionX_Left          = m_vPosition_HP.x + 10.f;
    m_fMovedPositionX_Right         = m_vPosition_HP.x - 10.f;
    m_fMovedPositionY_Top           = m_vPosition_HP.y - 5.f;
    m_fMovedPositionY_Bottom        = m_vPosition_HP.y + 5.f;
#pragma endregion

//============================================================================= For. HP_Shake
    m_bIsShake                      = false;
    m_iShakeCount                   = 0;
    m_fShakeTime                    = 0.f;
    m_vPosition_HP_Shake            = m_vPosition_HP;
    m_vPosition_HPBlank_Shake       = m_vPosition_HPBlank;

//============================================================================== HP Index
    m_strName                       = TEXT("Ä«»ç³× ·»µé");

    m_iOnes                         = 0;
    m_iTens                         = 0;
    m_iHundreds                     = 0;
    m_iThousands                    = 0;

    m_iOnesMax                      = 0;
    m_iTensMax                      = 0;
    m_iHundredsMax                  = 0;
    m_iThousandsMax                 = 0;

    m_vScale_Slash                  = _float3(9.f, 13.f, 1.f);
    m_vPosition_Slash               = _float4(172.f, -402.f, 0.f, 1.f);

    m_vScale_HPOnes                 = _float3(12.f, 20.f, 1.f);
    m_vScale_HPTens                 = _float3(12.f, 20.f, 1.f);
    m_vScale_HPHundreds             = _float3(12.f, 20.f, 1.f);
    m_vScale_HPThousands            = _float3(12.f, 20.f, 1.f);

    m_vScale_HPOnesMax              = _float3(9.f, 13.f, 1.f);
    m_vScale_HPTensMax              = _float3(9.f, 13.f, 1.f);
    m_vScale_HPHundredsMax          = _float3(9.f, 13.f, 1.f);
    m_vScale_HPThousandsMax         = _float3(9.f, 13.f, 1.f);

    m_vPosition_HPOnes              = _float4(138.f, -420.f, 0.f, 1.f);
    m_vPosition_HPTens              = _float4(126.f, -420.f, 0.f, 1.f);
    m_vPosition_HPHundreds          = _float4(114.f, -420.f, 0.f, 1.f);
    m_vPosition_HPThousands         = _float4(102.f, -420.f, 0.f, 1.f);

    m_vPosition_HPOnesMax           = _float4(190.f, -422.f, 0.f, 1.f);
    m_vPosition_HPTensMax           = _float4(181.f, -422.f, 0.f, 1.f);
    m_vPosition_HPHundredsMax       = _float4(172.f, -422.f, 0.f, 1.f);
    m_vPosition_HPThousandsMax      = _float4(163.f, -422.f, 0.f, 1.f);

//============================================================================== Default Setting
    m_bDamageEnd                    = true;
    m_bIsFinishCalculateDamage      = true;
    m_iMonsterPow                   = 0;
    m_fDamageDelayTime              = 0.f;

}

void CUI_HP::Tick_UI(_float fTimeDelta)
{
    //static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Get_State(TRANSFORM::POSITION);

    m_fHp                           = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;
    //m_fHpBlank                      = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().iHp;

    // HP Increase Exception Process
    if (m_fHp > m_fHpMax)
    {
        m_fHp = m_fHpMax;
    }
    if (m_fHpBlank > m_fHpBlankMax)
    {
        m_fHpBlank = m_fHpBlank;
    }

    // HP Decrease Expection Process
    if (0 >= m_fHp)
    {
        m_fHp = 0.f;
    }
    if (0 >= m_fHpBlank)
    {
        m_fHpBlank = 0.f;
    }

    m_fRatio                        = m_fHp         / m_fHpMax;
    m_fRatio_Blank                  = m_fHpBlank    / m_fHpBlankMax;

    m_fPercentage                   = m_fRatio * 270.f;
    m_fPercentage_Blank             = m_fRatio_Blank * 540.f;

    m_vScale_HP                     = _float3(m_fPercentage, 32.f, 1.f);
    m_vScale_HPBlank                = _float3(m_fPercentage_Blank, 3.f ,1.f);

    m_pHP1->Set_Scale(m_vScale_HP);
    m_pBlankHP->Set_Scale(m_vScale_HPBlank);

    Set_HpRenderFont();
    Set_DamageToPlayer();
    Shake_HPBar_System(fTimeDelta);
    Calculate_Damage_System(fTimeDelta);
    UIBounce_System(fTimeDelta);
}

void CUI_HP::Tick_SpriteTexture(_float fTimeDelta)
{
    if (1.f >= m_fRatio && 0.8f < m_fRatio)
    {
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
    if (0.8f >= m_fRatio && 0.6f < m_fRatio)
    {
        m_tUIDesc_80.m_fTime += fTimeDelta;

        if (m_tUIDesc_80.m_fFramePerSec <= m_tUIDesc_80.m_fTime)
        {
            m_tUIDesc_80.m_iIndex += 1;
            if (m_tUIDesc_80.m_iIndex >= m_tUIDesc_80.m_iIndexMaxY * m_tUIDesc_80.m_iIndexMaxX)
            {
                m_tUIDesc_80.m_iIndex = 0;
            }
            m_tUIDesc_80.m_fTime = 0.f;
        }
    }
    if (0.6f >= m_fRatio && 0.4f < m_fRatio)
    {
        m_tUIDesc_60.m_fTime += fTimeDelta;

        if (m_tUIDesc_60.m_fFramePerSec <= m_tUIDesc_60.m_fTime)
        {
            m_tUIDesc_60.m_iIndex += 1;
            if (m_tUIDesc_60.m_iIndex >= m_tUIDesc_60.m_iIndexMaxY * m_tUIDesc_60.m_iIndexMaxX)
            {
                m_tUIDesc_60.m_iIndex = 0;
            }
            m_tUIDesc_60.m_fTime = 0.f;
        }
    }
}

void CUI_HP::UIBounce_System(_float fTimeDelta)
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
                m_vPosition_HP.x += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

                if (m_vPosition_HP.x > m_fMovedPositionX_Left)
                {
                    m_vPosition_HP.x = m_fMovedPositionX_Left;
                    m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);
                }
            }
        }
        else
        {
            m_vPosition_HP.x -= fTimeDelta * 30.f;
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);


            if (m_vPosition_HP.x < m_fOriginalPosX)
            {
                m_vPosition_HP.x = m_fOriginalPosX;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

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
                m_vPosition_HP.x -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

                if (m_vPosition_HP.x < m_fMovedPositionX_Right)
                {
                    m_vPosition_HP.x = m_fMovedPositionX_Right;
                    m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);
                }
            }
        }
        else
        {
            m_vPosition_HP.x += fTimeDelta * 30.f;
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);


            if (m_vPosition_HP.x > m_fOriginalPosX)
            {
                m_vPosition_HP.x = m_fOriginalPosX;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

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
                m_vPosition_HP.y -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

                if (m_vPosition_HP.y < m_fMovedPositionY_Top)
                {
                    m_vPosition_HP.y = m_fMovedPositionY_Top;
                    m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);
                }
            }
        }
        else
        {
            m_vPosition_HP.y += fTimeDelta * 30.f;
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);


            if (m_vPosition_HP.y > m_fOriginalPosY)
            {
                m_vPosition_HP.y = m_fOriginalPosY;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

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
                m_vPosition_HP.y += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

                if (m_vPosition_HP.y > m_fMovedPositionY_Bottom)
                {
                    m_vPosition_HP.y = m_fMovedPositionY_Bottom;
                    m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);
                }
            }
        }
        else
        {
            m_vPosition_HP.y -= fTimeDelta * 30.f;
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);


            if (m_vPosition_HP.y < m_fOriginalPosY)
            {
                m_vPosition_HP.y = m_fOriginalPosY;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

                m_bBottom = false;
            }
        }
    }
}

void CUI_HP::Set_DamageToPlayer()
{
    if (0 < m_fHp)
    {
        /* Situation :: When Player Damaged */
        if (m_fPrevHP != m_fHp)
        {
            m_fRatio = m_fHp / m_fHpMax;

            m_vPosition_HP.x = -63.f - 270.f * (1.f - m_fRatio) * 0.5f; // Set CurrentHP_Position
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP);

            m_vPrevPosition_HP = m_vPosition_HP;               // Renewal Prev Variable
            m_vPrevPosition_HPBlank = m_vPosition_HPBlank;

            // For. Bounce
            m_fOriginalPosX = m_vPosition_HP.x;
            m_fMovedPositionX_Left  = m_fOriginalPosX + 10.f;
            m_fMovedPositionX_Right = m_fOriginalPosX - 10.f;

            m_bIsFinishCalculateDamage = false;
            m_bIsShake = true;
        }
        else
        {
            m_bIsShake = false;
        }
    }
}

void CUI_HP::Calculate_Damage_System(_float fTimeDelta) // Set_Delay_Damaged_HP
{
    if (false == m_bIsFinishCalculateDamage)
    {
        if (false == m_bIsShake)
        {
            m_fDamageDelayTime += fTimeDelta;

            if (1.f <= m_fDamageDelayTime)
            {
                if (m_fHp <= m_fHpBlank)
                {
                    m_fHpBlank -= fTimeDelta * 10.f;
                    m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank);
                }
                else
                {
                    m_bIsFinishCalculateDamage = true;

                }
            }
        }
    }
    else
    {
        m_iMonsterPow       = 0;
        m_fDamageDelayTime  = 0.f;
    }
}

void CUI_HP::Shake_HPBar_System(_float fTimeDetlta)
{
    m_vPosition_HP_Shake            = m_vPosition_HP;
    m_vPosition_HPBlank_Shake       = m_vPosition_HPBlank;

    if (true == m_bIsShake)
    {
        m_fShakeTime += fTimeDetlta;
        if (2 > m_iShakeCount)
        {
            if (0.f <= m_fShakeTime && 0.05f > m_fShakeTime)
            {
                m_vPosition_HP_Shake.x      += 2.f;
                m_vPosition_HP_Shake.y      += 2.f;
                m_vPosition_HPBlank_Shake.x += 2.f;
                m_vPosition_HPBlank_Shake.y += 2.f;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP_Shake);
                m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank_Shake);
            }
            if (0.05f <= m_fShakeTime && 0.1f > m_fShakeTime)
            {
                m_vPosition_HP_Shake.x      -= 3.f;
                m_vPosition_HP_Shake.y      -= 3.f;
                m_vPosition_HPBlank_Shake.x -= 3.f;
                m_vPosition_HPBlank_Shake.y -= 3.f;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP_Shake);
                m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank_Shake);
            }
            if (0.1f <= m_fShakeTime && 0.15f > m_fShakeTime)
            {
                m_vPosition_HP_Shake.x      -= 1.5f;
                m_vPosition_HP_Shake.y      += 3.f;
                m_vPosition_HPBlank_Shake.x -= 1.5f;
                m_vPosition_HPBlank_Shake.y += 3.f;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP_Shake);
                m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank_Shake);
            }
            if (0.15f <= m_fShakeTime && 0.2f > m_fShakeTime)
            {
                m_vPosition_HP_Shake.x      += 3.f;
                m_vPosition_HP_Shake.y      -= 1.5f;
                m_vPosition_HPBlank_Shake.x += 3.f;
                m_vPosition_HPBlank_Shake.y -= 1.5f;
                m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP_Shake);
                m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank_Shake);
            }
            if (0.2f <= m_fShakeTime)
            {
                m_fShakeTime    = 0.f;
                m_iShakeCount   += 1;
            }
        }
        else
        {
            m_iShakeCount = 0;
            m_vPosition_HP_Shake        = m_vPosition_HP;
            m_vPosition_HPBlank_Shake   = m_vPosition_HPBlank;
            m_pHP1->Set_State(TRANSFORM::POSITION, m_vPosition_HP_Shake);
            m_pBlankHP->Set_State(TRANSFORM::POSITION, m_vPosition_HPBlank_Shake);
            m_bIsShake = false;
            m_fPrevHP = m_fHp;
        }
    }
}

void CUI_HP::Set_HpRenderFont()
{
    /* m_fHp */
    m_iOnes                     = (_int)m_fHp % 10;
    m_iTens                     = ((_int)m_fHp / 10) % 10;
    m_iHundreds                 = ((_int)m_fHp / 100) % 10;
    m_iThousands                = ((_int)m_fHp / 1000) % 10;


    /* m_fHpMax */
    m_iOnesMax                  = (_int)m_fHpMax % 10;
    m_iTensMax                  = ((_int)m_fHpMax / 10) % 10;
    m_iHundredsMax              = ((_int)m_fHpMax / 100) % 10;
    m_iThousandsMax             = ((_int)m_fHpMax / 1000) % 10;

    /* Slash Position */
    if (0 == m_iThousandsMax)
    {
        m_vPosition_Slash = _float4(164.f, -422.f, 0.f, 1.f);
        m_pSlash->Set_State(TRANSFORM::POSITION, m_vPosition_Slash);
        
        m_vPosition_HPOnes      = _float4(152.f, -420.f, 0.f, 1.f);
        m_vPosition_HPTens      = _float4(140.f, -420.f, 0.f, 1.f);
        m_vPosition_HPHundreds  = _float4(128.f, -420.f, 0.f, 1.f);
        m_vPosition_HPThousands = _float4(116.f, -420.f, 0.f, 1.f);

        m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_HPOnes);
        m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_HPTens);
        m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_HPHundreds);
        m_pThousands->Set_State(TRANSFORM::POSITION, m_vPosition_HPThousands);
    }
    else
    {
        m_vPosition_Slash = _float4(152.f, -422.f, 0.f, 1.f);
        m_pSlash->Set_State(TRANSFORM::POSITION, m_vPosition_Slash);

        m_vPosition_HPOnes              = _float4(140.f, -420.f, 0.f, 1.f);
        m_vPosition_HPTens              = _float4(128.f, -420.f, 0.f, 1.f);
        m_vPosition_HPHundreds          = _float4(116.f, -420.f, 0.f, 1.f);
        m_vPosition_HPThousands         = _float4(104.f, -420.f, 0.f, 1.f);

        m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_HPOnes);
        m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_HPTens);
        m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_HPHundreds);
        m_pThousands->Set_State(TRANSFORM::POSITION, m_vPosition_HPThousands);
    }
}

HRESULT CUI_HP::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_HP> CUI_HP::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_HP> pInstance = make_private_shared(CUI_HP, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_HP::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_HP::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_HP> pInstance = make_private_shared_copy(CUI_HP, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_HP::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
