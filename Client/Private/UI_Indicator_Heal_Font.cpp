#include "ClientPCH.h"
#include "UI_Indicator_Heal_Font.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"

CUI_Indicator_Heal_Font::CUI_Indicator_Heal_Font(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Heal_Font::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Heal_Font::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Heal_Font::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Heal_Font::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Heal_Font::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI() && true == m_bRenderDegree)
    {
        if (100 <= m_iHeal_Split)
        {
#pragma region Heal - Hundred
            switch (m_iHeal_Hundreds)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
#pragma endregion
            if (FAILED(m_pHundreds->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFontAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(63)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CGameObject::Render");
            }

#pragma region Heal - Tens
            switch (m_iHeal_Tens)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }

#pragma endregion
            if (FAILED(m_pTens->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFontAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(63)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CGameObject::Render");
            }

#pragma region Heal - Ones
            switch (m_iHeal_Ones)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
#pragma endregion
            if (FAILED(m_pOnes->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFontAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(63)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CGameObject::Render");
            }
        }
        else
        {
#pragma region Heal - Tens
            switch (m_iHeal_Tens)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }

#pragma endregion
            if (FAILED(m_pTens->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFontAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(63)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CGameObject::Render");
            }

#pragma region Heal - Ones
            switch (m_iHeal_Ones)
            {
            case 0:
            {
                if (FAILED(m_pTexture_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            default:
                break;
            }
#pragma endregion
            if (FAILED(m_pOnes->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fFontAlpha, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to m_pShader::Bind_RawValue");
            }
            if (FAILED(__super::Render(63)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CGameObject::Render");
            }
        }


    }

    return S_OK;
}

HRESULT CUI_Indicator_Heal_Font::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Heal_Font::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Heal_Font::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

_bool CUI_Indicator_Heal_Font::Return()
{
    return m_bHeal_RenderEnd;
}

HRESULT CUI_Indicator_Heal_Font::Fetch(any)
{
//=========================================================================== _bool
    m_bHeal_RenderEnd       = false; // 다 쓰면 true로 바꾸기
    m_bRenderDegree         = true;

//=========================================================================== _int
    m_iHeal_Ones            = 0;
    m_iHeal_Tens            = 0;
    m_iHeal_Hundreds        = 0;

    m_iHeal_Split           = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fHealValue;
    m_iRenderFontPhase      = 0;

//=========================================================================== _float
    m_fAccTime              = 0.f;
    m_fFontAlpha            = 0.f;
    m_fHealFont_Rise        = 0.f;

//=========================================================================== _float3
    m_vScale_Font           = _float3(28.f, 40.f, 1.f);

//=========================================================================== Set_Scale
    m_pOnes->Set_Scale(m_vScale_Font);
    m_pTens->Set_Scale(m_vScale_Font);
    m_pHundreds->Set_Scale(m_vScale_Font);

    shared_ptr<CTransform> pPlayerTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Component(COMPONENT::TRANSFORM));
    m_matPlayerPosition = pPlayerTransform->Get_WorldMatrix();

//=========================================================================== Set_Position
    Setting_HealPosition(m_matPlayerPosition);

    return S_OK;
}

void CUI_Indicator_Heal_Font::Initialize_UI()
{
//=========================================================================== Transform
    m_pOnes                 = CTransform::Create(m_pDevice, m_pContext);
    m_pTens                 = CTransform::Create(m_pDevice, m_pContext);
    m_pHundreds             = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Zero         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/0.png"));
    m_pTexture_One          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/1.png"));
    m_pTexture_Two          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/2.png"));
    m_pTexture_Three        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/3.png"));
    m_pTexture_Four         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/4.png"));
    m_pTexture_Five         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/5.png"));
    m_pTexture_Six          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/6.png"));
    m_pTexture_Seven        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/7.png"));
    m_pTexture_Eight        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/8.png"));
    m_pTexture_Nine         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Heal/9.png"));

//=========================================================================== _bool
    m_bHeal_RenderEnd       = false; // 다 쓰면 true로 바꾸기
    m_bRenderDegree         = true;

//=========================================================================== _int
    m_iHeal_Ones            = 0;
    m_iHeal_Tens            = 0;
    m_iHeal_Hundreds        = 0;

    m_iHeal_Split           = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fHealValue;
    m_iRenderFontPhase      = 0;

//=========================================================================== _float
    m_fRenderFontDelayTime  = 0.f;
    m_fAccTime              = 0.f;
    m_fFontAlpha            = 0.f;
    m_fHealFont_Rise        = 0.f;

//=========================================================================== _float3
    m_vScale_Font           = _float3(31.f, 44.f, 1.f);

//=========================================================================== Set_Scale
    m_pOnes->Set_Scale(m_vScale_Font);
    m_pTens->Set_Scale(m_vScale_Font);
    m_pHundreds->Set_Scale(m_vScale_Font);

    shared_ptr<CTransform> pPlayerTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Component(COMPONENT::TRANSFORM));
    m_matPlayerPosition = pPlayerTransform->Get_WorldMatrix();

//=========================================================================== Set_Position
    Setting_HealPosition(m_matPlayerPosition);
}

void CUI_Indicator_Heal_Font::Tick_UI(_float fTimeDelta)
{
    Setting_SplitHeal();
    HealFont_Rendering(fTimeDelta);
    Setting_HealPosition(m_matPlayerPosition);
}

void CUI_Indicator_Heal_Font::Setting_SplitHeal()
{
    m_iHeal_Split = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fHealValue;

    if (100 <= m_iHeal_Split)
    {
        m_iHeal_Hundreds    = (m_iHeal_Split / 100) % 10;
        m_iHeal_Tens        = (m_iHeal_Split / 10) % 10;
        m_iHeal_Ones        = m_iHeal_Split % 10;
    }
    else
    {
        m_iHeal_Tens = (m_iHeal_Split / 10) % 10;
        m_iHeal_Ones = m_iHeal_Split % 10;
    }
}

void CUI_Indicator_Heal_Font::Setting_HealPosition(_matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition;

    _vector vCrossLookCam   = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
    _vector vPosition_L     = vPosition + vCrossLookCam * -0.25f;
    _vector vPosition_R     = vPosition + vCrossLookCam * 0.25f;

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);
    vPosition_L = vPosition;
    vPosition_R = vPosition;

    
    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    _float fScreenX_L = XMVectorGetX(vPosition_L);
    _float fScreenY_L = XMVectorGetY(vPosition_L);

    _float fScreenX_R = XMVectorGetX(vPosition_R);
    _float fScreenY_R = XMVectorGetY(vPosition_R);

    m_vPosition_Font_T.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Font_T.y = (fScreenY) * 0.5f * g_iWinCY + m_fHealFont_Rise;

    m_vPosition_Font_O.x = (fScreenX_L) * 0.5f * g_iWinCX + 20.f;
    m_vPosition_Font_O.y = (fScreenY) * 0.5f * g_iWinCY + m_fHealFont_Rise;

    m_vPosition_Font_H.x = (fScreenX_R) * 0.5f * g_iWinCX - 20.f;
    m_vPosition_Font_H.y = (fScreenY) * 0.5f * g_iWinCY + m_fHealFont_Rise;

    m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_Font_T);
    m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_Font_O);
    m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_Font_H);

//=========================================================================================================================== Culling
    // Look Vector Monster ---> Camera
    _vector vLookMonster = MatCam.r[3] - _WorldMatrix.r[3];

    // Calculate the angle between vLookMonster and MatCam.r[2]
    _float fDotProduct = XMVectorGetX(XMVector3Dot(vLookMonster, MatCam.r[2]));
    _float fDeterminant = XMVectorGetX(XMVector3Length(vLookMonster)) * XMVectorGetX(XMVector3Length(MatCam.r[2]));

    // Calculate the angle in degrees
    _float fDegree = XMConvertToDegrees(acosf(fDotProduct / fDeterminant));

    if (90.f <= fDegree && 270.f >= fDegree)
    {
        m_bRenderDegree = true;
    }
    else
    {
        m_bRenderDegree = false;
    }
}

void CUI_Indicator_Heal_Font::HealFont_Rendering(_float fTimeDelta)
{
    if (0.75f <= m_fRenderFontDelayTime)
    {
        if (0 == m_iRenderFontPhase)
        {
            if (1.f <= m_fFontAlpha)
            {
                m_fFontAlpha = 0.8f;
                m_iRenderFontPhase += 1;
            }
            else
            {
                m_fFontAlpha += fTimeDelta * 10.f;
                m_fHealFont_Rise += fTimeDelta * 500.f;
            }
        }
        if (1 == m_iRenderFontPhase)
        {
            if (1.f <= m_fAccTime)
            {
                m_fAccTime = 1.f;
                m_iRenderFontPhase += 1;
            }
            else
            {
                m_fAccTime += fTimeDelta * 0.75f;
            }
        }
        if (2 == m_iRenderFontPhase)
        {
            if (0.f >= m_fFontAlpha)
            {
                m_fFontAlpha = 0.f;
                m_bHeal_RenderEnd = true; // Master - Key
            }
            else
            {
                m_fFontAlpha -= fTimeDelta * 20.f;
                m_fHealFont_Rise += fTimeDelta * 200.f;
            }
        }
    }
    else
    {
        m_fRenderFontDelayTime += fTimeDelta;
    }

}

shared_ptr<CUI_Indicator_Heal_Font> CUI_Indicator_Heal_Font::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Heal_Font> pInstance = make_private_shared(CUI_Indicator_Heal_Font, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Heal_Font::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Heal_Font::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Heal_Font> pInstance = make_private_shared_copy(CUI_Indicator_Heal_Font, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Heal_Font::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
