#include "ClientPCH.h"
#include "UI_Indicator_Damage_Font.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_Damage_Font::CUI_Indicator_Damage_Font(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Damage_Font::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Damage_Font::Initialize(any _aDesc)
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

void CUI_Indicator_Damage_Font::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Damage_Font::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Damage_Font::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI() && true == m_bRenderDegree
        && false == CUI_Manager::Get_Instance()->Get_NoDamage())
    {
#pragma region Damage_Type::Normal_1
        if (1 == m_iDamage_Type)
        {
#pragma region Damage - Tens
            switch (m_iDamage_Tens)
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

#pragma region Damage - Ones
            switch (m_iDamage_Ones)
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
#pragma endregion

#pragma region Damage_Type::Critical_2
        // Critical
        if (2 == m_iDamage_Type)
        {
            if (0 != m_iDamage_Hundreds)
            {
#pragma region Damage - Hundreds
                switch (m_iDamage_Hundreds)
                {
                case 0:
                {
                    if (FAILED(m_pTexture_C_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 1:
                {
                    if (FAILED(m_pTexture_C_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 2:
                {
                    if (FAILED(m_pTexture_C_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 3:
                {
                    if (FAILED(m_pTexture_C_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 4:
                {
                    if (FAILED(m_pTexture_C_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 5:
                {
                    if (FAILED(m_pTexture_C_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 6:
                {
                    if (FAILED(m_pTexture_C_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 7:
                {
                    if (FAILED(m_pTexture_C_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 8:
                {
                    if (FAILED(m_pTexture_C_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                    {
                        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                    }
                }
                break;

                case 9:
                {
                    if (FAILED(m_pTexture_C_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
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
            }

#pragma region Damage - Tens
            switch (m_iDamage_Tens)
            {
            case 0:
            {
                if (FAILED(m_pTexture_C_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_C_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_C_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_C_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_C_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_C_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_C_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_C_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_C_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_C_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
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

#pragma region Damage - Ones
            switch (m_iDamage_Ones)
            {
            case 0:
            {
                if (FAILED(m_pTexture_C_Zero->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 1:
            {
                if (FAILED(m_pTexture_C_One->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 2:
            {
                if (FAILED(m_pTexture_C_Two->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 3:
            {
                if (FAILED(m_pTexture_C_Three->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 4:
            {
                if (FAILED(m_pTexture_C_Four->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 5:
            {
                if (FAILED(m_pTexture_C_Five->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 6:
            {
                if (FAILED(m_pTexture_C_Six->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 7:
            {
                if (FAILED(m_pTexture_C_Seven->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 8:
            {
                if (FAILED(m_pTexture_C_Eight->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
            }
            break;

            case 9:
            {
                if (FAILED(m_pTexture_C_Nine->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
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
#pragma endregion

#pragma region Damage_Type::SomerSaultKick_5
        // SomerSaultKick
        if (5 == m_iDamage_Type)
        {
            if (0 != m_iDamage_Hundreds)
            {
#pragma region Damage - Hundreds
                switch (m_iDamage_Hundreds)
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
            }

#pragma region Damage - Tens
            switch (m_iDamage_Tens)
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

#pragma region Damage - Ones
            switch (m_iDamage_Ones)
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
#pragma endregion

#pragma region Damage_Type::Throw Object_10
        // Throw Object
        if (10 == m_iDamage_Type)
        {
            if (0 != m_iDamage_Hundreds)
            {
#pragma region Damage - Hundreds
                switch (m_iDamage_Hundreds)
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
            }

#pragma region Damage - Tens
            switch (m_iDamage_Tens)
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

#pragma region Damage - Ones
            switch (m_iDamage_Ones)
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
#pragma endregion
    }
    else if (true == CUI_Manager::Get_Instance()->Get_NoDamage())
    {
        if (FAILED(m_pTexture_NoDamage->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pNoDamage->Bind_OnShader(m_pShader)))
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

    return S_OK;
}

HRESULT CUI_Indicator_Damage_Font::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Damage_Font::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

_bool CUI_Indicator_Damage_Font::Return()
{
    return m_bDamage_RenderEnd;
}

HRESULT CUI_Indicator_Damage_Font::Fetch(any)
{
//=========================================================================== _bool
    m_bDamage_RenderEnd     = false; // 다 쓰면 true로 바꾸기
    m_bCanRendom            = true;
    m_bRenderDegree         = true;

//=========================================================================== _int
    m_iDamage_Type          = 0;
    m_iDamage_RendomNumber  = 0;

    m_iDamage_Ones          = 0;
    m_iDamage_Tens          = 0;
    m_iDamage_Hundreds      = 0;

    m_iRenderFontPhase      = 0;

//=========================================================================== _float
    m_fAccTime              = 0.f;
    m_fFontAlpha            = 0.f;
    m_fDamageFont_Rise      = 0.f;

//=========================================================================== _float3
    m_vScale_Font           = _float3(44.f, 60.f, 1.f);
    m_vScale_C_Font         = _float3(64.f, 92.f, 1.f);
    m_vScale_NoDamage       = _float3(113.f, 16.f, 1.f);
//=========================================================================== _float4
    //m_vPosition_Font_H      = _float4(-48.f, rand() % 300, 0.f, 1.f);
    //m_vPosition_Font_T      = _float4(-24.f, m_vPosition_Font_H.y + 0.f, 0.f, 1.f);
    //m_vPosition_Font_O      = _float4(0.f, m_vPosition_Font_H.y + 0.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    if (1 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        m_pOnes->Set_Scale(m_vScale_Font);
        m_pTens->Set_Scale(m_vScale_Font);
        m_pHundreds->Set_Scale(m_vScale_Font);
    }
    else if (2 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        m_pOnes->Set_Scale(m_vScale_C_Font);
        m_pTens->Set_Scale(m_vScale_C_Font);
        m_pHundreds->Set_Scale(m_vScale_C_Font);
    }
    else if (5 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        m_pOnes->Set_Scale(m_vScale_Font);
        m_pTens->Set_Scale(m_vScale_Font);
        m_pHundreds->Set_Scale(m_vScale_Font);
    }
    else if (10 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        m_pOnes->Set_Scale(m_vScale_Font);
        m_pTens->Set_Scale(m_vScale_Font);
        m_pHundreds->Set_Scale(m_vScale_Font);
    }

    m_HitPosition = CUI_Manager::Get_Instance()->Get_DamagePosition();

//=========================================================================== Set_Position
    Setting_DamagePosition(m_HitPosition);

//=========================================================================== Set_Position
    //m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_Font_O);
    //m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_Font_T);
    //m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_Font_H);

    return S_OK;
}

void CUI_Indicator_Damage_Font::Initialize_UI()
{
//=========================================================================== Transform
    m_pOnes                 = CTransform::Create(m_pDevice, m_pContext);
    m_pTens                 = CTransform::Create(m_pDevice, m_pContext);
    m_pHundreds             = CTransform::Create(m_pDevice, m_pContext);
    m_pNoDamage             = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    // Normal
    m_pTexture_Zero         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/0.png"));
    m_pTexture_One          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/1.png"));
    m_pTexture_Two          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/2.png"));
    m_pTexture_Three        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/3.png"));
    m_pTexture_Four         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/4.png"));
    m_pTexture_Five         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/5.png"));
    m_pTexture_Six          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/6.png"));
    m_pTexture_Seven        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/7.png"));
    m_pTexture_Eight        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/8.png"));
    m_pTexture_Nine         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Normal/9.png"));

    // Critical
    m_pTexture_C_Zero       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/0.png"));
    m_pTexture_C_One        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/1.png"));
    m_pTexture_C_Two        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/2.png"));
    m_pTexture_C_Three      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/3.png"));
    m_pTexture_C_Four       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/4.png"));
    m_pTexture_C_Five       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/5.png"));
    m_pTexture_C_Six        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/6.png"));
    m_pTexture_C_Seven      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/7.png"));
    m_pTexture_C_Eight      = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/8.png"));
    m_pTexture_C_Nine       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/Critical/9.png"));

    // No Damage
    m_pTexture_NoDamage     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Damage_Font/NoDamage.png"));

//=========================================================================== _bool
    m_bDamage_RenderEnd     = false; // 다 쓰면 true로 바꾸기
    m_bCanRendom            = true;
    m_bRenderDegree         = true;

//=========================================================================== _int
    m_iDamage_Type          = 0;
    m_iDamage_RendomNumber  = 0;

    m_iDamage_Ones          = 0;
    m_iDamage_Tens          = 0;
    m_iDamage_Hundreds      = 0;

    m_iRenderFontPhase      = 0;

//=========================================================================== _float
    m_fAccTime              = 0.f;
    m_fFontAlpha            = 0.f;
    m_fDamageFont_Rise      = 0.f;

//=========================================================================== _float3
    m_vScale_Font           = _float3(44.f, 60.f, 1.f);
    m_vScale_C_Font         = _float3(64.f, 92.f, 1.f);
    m_vScale_NoDamage       = _float3(113.f, 16.f, 1.f);
//=========================================================================== _float4
    //m_vPosition_Font_H      = _float4(-48.f, rand() % 600 +0.f, 0.f, 1.f);
    //m_vPosition_Font_T      = _float4(-24.f, rand() % 600 + 0.f, 0.f, 1.f);
    //m_vPosition_Font_O      = _float4(0.f, rand() % 600 + 0.f, 0.f, 1.f);

//=========================================================================== Set_Position
    Setting_DamagePosition(CUI_Manager::Get_Instance()->Get_DamagePosition());

//=========================================================================== Set_Scale
    m_pOnes->Set_Scale(m_vScale_Font);
    m_pTens->Set_Scale(m_vScale_Font);
    m_pHundreds->Set_Scale(m_vScale_Font);
    m_pNoDamage->Set_Scale(m_vScale_NoDamage);
//=========================================================================== Set_Position
    //m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_Font_O);
    //m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_Font_T);
    //m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_Font_H);
}

void CUI_Indicator_Damage_Font::Tick_UI(_float fTimeDelta)
{
    Setting_RandomDamage();

    DamageFont_Rendering(fTimeDelta);

    Setting_DamagePosition(m_HitPosition);
}

void CUI_Indicator_Damage_Font::Setting_RandomDamage()
{
    if (true == m_bCanRendom)
    {
        m_iDamage_Type = CUI_Manager::Get_Instance()->Get_DamageType();

        //if (1 > m_iDamage_Type || 2 < m_iDamage_Type)
        //    return;

#pragma region SCENE::TEST
        if (SCENE::TEST == CGameInstance::Get_Instance()->Current_Scene()) // 3
        {
            switch (m_iDamage_Type)
            {
            case 1: // Normal
            {
                m_iDamage_RendomNumber  = rand() % 40 + 30;

                m_iDamage_Tens          = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones          = m_iDamage_RendomNumber & 10;
            }
            break;

            case 2: // Critical
            {
                m_iDamage_RendomNumber  = (rand() % 40 + 30) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds      = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens          = (m_iDamage_RendomNumber / 10) & 10;
                m_iDamage_Ones          = m_iDamage_RendomNumber & 10;
            }
            break;

            case 5: // SomerSaultKick
            {
                m_iDamage_RendomNumber = (rand() % 40 + 70) * (rand() % 2 + 1); // Critical 데미지 (1배 ~ 2배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) & 10;
                m_iDamage_Ones = m_iDamage_RendomNumber & 10;
            }
            break;

            case 10: // Throw Object
            {
                m_iDamage_RendomNumber  = (rand() % 40 + 100) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds      = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens          = (m_iDamage_RendomNumber / 10) & 10;
                m_iDamage_Ones          = m_iDamage_RendomNumber & 10;
            }
            break;

            default:
                break;
            }
        }
#pragma endregion

#pragma region SCENE::MIZUHA
        if (SCENE::MIZUHA == CGameInstance::Get_Instance()->Current_Scene())
        {
            switch (m_iDamage_Type)
            {
            case 1: // Normal
            {
                m_iDamage_RendomNumber  = rand() % 40 + 60;

                m_iDamage_Tens          = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones          = m_iDamage_RendomNumber % 10;
            }
            break;

            case 2: // Critical
            {
                m_iDamage_RendomNumber  = (rand() % 40 + 60) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds      = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens          = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones          = m_iDamage_RendomNumber % 10;
            }
            break;

            case 5: // SomerSaultKick
            {
                m_iDamage_RendomNumber = (rand() % 40 + 70) * (rand() % 1 + 2); // Critical 데미지 (1배 ~ 2배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 10: // Throw Object
            {
                m_iDamage_RendomNumber = (rand() % 40 + 100) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            default:
                break;
            }
        }
#pragma endregion

#pragma region SCENE::SUOH_BATTLE
        if (SCENE::SUOH_BATTLE == CGameInstance::Get_Instance()->Current_Scene())
        {
            switch (m_iDamage_Type)
            {
            case 1: // Normal
            {
                m_iDamage_RendomNumber = rand() % 40 + 60;

                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 2: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 60) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 5: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 70) * (rand() % 2 + 1); // Critical 데미지 (1배 ~ 2배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 10: // Throw Object
            {
                m_iDamage_RendomNumber = (rand() % 40 + 100) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            default:
                break;
            }
        }
#pragma endregion

#pragma region SCENE::MINDROOM
        if (SCENE::MINDROOM == CGameInstance::Get_Instance()->Current_Scene())
        {
            switch (m_iDamage_Type)
            {
            case 1: // Normal
            {
                m_iDamage_RendomNumber = rand() % 40 + 60;

                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 2: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 60) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 5: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 70) * (rand() % 2 + 1); // Critical 데미지 (1배 ~ 2배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 10: // Throw Object
            {
                m_iDamage_RendomNumber = (rand() % 40 + 100) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            default:
                break;
            }
        }
#pragma endregion

#pragma region SCENE::REALTEST
        if (SCENE::REALTEST == CGameInstance::Get_Instance()->Current_Scene())
        {
            switch (m_iDamage_Type)
            {
            case 1: // Normal
            {
                m_iDamage_RendomNumber = rand() % 40 + 60;

                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 2: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 60) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;
            
            case 5: // Critical
            {
                m_iDamage_RendomNumber = (rand() % 40 + 70) * (rand() % 2 + 1); // Critical 데미지 (1배 ~ 2배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            case 10: // Throw Object
            {
                m_iDamage_RendomNumber = (rand() % 40 + 100) * (rand() % 2 + 2); // Critical 데미지 (2배 ~ 3배)

                m_iDamage_Hundreds = (m_iDamage_RendomNumber / 100) % 10;
                m_iDamage_Tens = (m_iDamage_RendomNumber / 10) % 10;
                m_iDamage_Ones = m_iDamage_RendomNumber % 10;
            }
            break;

            default:
                break;
            }
        }
#pragma endregion

        m_bCanRendom = false;
    }
}

void CUI_Indicator_Damage_Font::Setting_DamagePosition(_matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);


    _vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition ;

    _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
    _vector vPosition_L = vPosition + vCrossLookCam * -0.25f;
    _vector vPosition_R = vPosition + vCrossLookCam * 0.25f;

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);
    vPosition_L = vPosition;
    vPosition_R = vPosition;

    //vPosition_L = XMVectorSetX(vPosition_L, XMVectorGetX(vPosition_L) - 22.f);
    //vPosition_R = XMVectorSetX(vPosition_R, XMVectorGetX(vPosition_R) + 22.f);

    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    _float fScreenX_L = XMVectorGetX(vPosition_L);
    _float fScreenY_L = XMVectorGetY(vPosition_L);

    _float fScreenX_R = XMVectorGetX(vPosition_R);
    _float fScreenY_R = XMVectorGetY(vPosition_R);

    m_vPosition_Font_T.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Font_T.y = (fScreenY) * 0.5f * g_iWinCY + m_fDamageFont_Rise;

    m_vPosition_Font_O.x = (fScreenX_L) * 0.5f * g_iWinCX + 18.f;
    m_vPosition_Font_O.y = (fScreenY) * 0.5f * g_iWinCY + m_fDamageFont_Rise;

    m_vPosition_Font_H.x = (fScreenX_R) * 0.5f * g_iWinCX - 18.f;
    m_vPosition_Font_H.y = (fScreenY) * 0.5f * g_iWinCY + m_fDamageFont_Rise;

    m_vPosition_Font_NoDamage.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Font_NoDamage.y = (fScreenY) * 0.5f * g_iWinCY + m_fDamageFont_Rise;

    m_pTens->Set_State(TRANSFORM::POSITION, m_vPosition_Font_T);
    m_pOnes->Set_State(TRANSFORM::POSITION, m_vPosition_Font_O);
    m_pHundreds->Set_State(TRANSFORM::POSITION, m_vPosition_Font_H);
    m_pNoDamage->Set_State(TRANSFORM::POSITION, m_vPosition_Font_NoDamage);
//=========================================================================================================================== Culling
    // Look_Vector (Monster ---> Camera)
    _vector vLookCamera_FromMonster = MatCam.r[3] - _WorldMatrix.r[3];
    _vector vLookMonster_FromCamera = _WorldMatrix.r[3] - MatCam.r[3];

    //// Calculate the angle between vLookMonster and MatCam.r[2]
    //_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookCamera_FromMonster, _WorldMatrix.r[2]));
    //_float fDeterminant = XMVectorGetX(XMVector3Length(vLookCamera_FromMonster)) * XMVectorGetX(XMVector3Length(MatCam.r[2]));

    //// Calculate the angle in degrees
    //_float fDegree = XMConvertToDegrees(acosf(fDotProduct / fDeterminant));

    //_float fDegree = XMConvertToDegrees(acosf(fDotProduct));

    _float fDotProduct = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLookCamera_FromMonster), XMVector3Normalize(MatCam.r[2])))));

    if (90.f <= fDotProduct && 270.f >= fDotProduct)
    {
        m_bRenderDegree = true;
    }
    else
    {
        m_bRenderDegree = false;
    }
}

void CUI_Indicator_Damage_Font::DamageFont_Rendering(_float fTimeDelta)
{
    if (0 == m_iRenderFontPhase)
    {
        if (1.f <= m_fFontAlpha)
        {
            m_fFontAlpha = 1.f;
            m_iRenderFontPhase += 1;
        }
        else
        {
            m_fFontAlpha += fTimeDelta * 10.f;
            m_fDamageFont_Rise += fTimeDelta * 500.f;
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
            m_bDamage_RenderEnd = true; // Master - Key
        }
        else
        {
            m_fFontAlpha -= fTimeDelta * 20.f;
            m_fDamageFont_Rise += fTimeDelta * 200.f;
        }
    }

#pragma region Damage - Normal
    if (1 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        // Set Scalling - X
        if (22.f >= m_vScale_Font.x)
        {
            m_vScale_Font.x = 22.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.x -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        // Set Scailing - Y
        if (30.f >= m_vScale_Font.y)
        {
            m_vScale_Font.y = 30.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.y -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
    }
#pragma endregion
#pragma region Damage - Critical
    if (2 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        // Set Scalling - X
        if (30.f >= m_vScale_C_Font.x)
        {
            m_vScale_C_Font.x = 30.f;

            m_pHundreds->Set_Scale(m_vScale_C_Font);
            m_pTens->Set_Scale(m_vScale_C_Font);
            m_pOnes->Set_Scale(m_vScale_C_Font);
        }
        else
        {
            m_vScale_C_Font.x -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_C_Font);
            m_pTens->Set_Scale(m_vScale_C_Font);
            m_pOnes->Set_Scale(m_vScale_C_Font);
        }
        // Set Scailing - Y
        if (44.f >= m_vScale_C_Font.y)
        {
            m_vScale_C_Font.y = 44.f;

            m_pHundreds->Set_Scale(m_vScale_C_Font);
            m_pTens->Set_Scale(m_vScale_C_Font);
            m_pOnes->Set_Scale(m_vScale_C_Font);
        }
        else
        {
            m_vScale_C_Font.y -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_C_Font);
            m_pTens->Set_Scale(m_vScale_C_Font);
            m_pOnes->Set_Scale(m_vScale_C_Font);
        }
    }
#pragma endregion
#pragma region Damage - SomerSaultKick
    if (5 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        // Set Scalling - X
        if (30.f >= m_vScale_Font.x)
        {
            m_vScale_Font.x = 30.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.x -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        // Set Scailing - Y
        if (44.f >= m_vScale_Font.y)
        {
            m_vScale_Font.y = 44.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.y -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
    }
#pragma endregion
#pragma region Damage - Throw Object
    if (10 == CUI_Manager::Get_Instance()->Get_DamageType())
    {
        // Set Scalling - X
        if (30.f >= m_vScale_Font.x)
        {
            m_vScale_Font.x = 30.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.x -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        // Set Scailing - Y
        if (44.f >= m_vScale_Font.y)
        {
            m_vScale_Font.y = 44.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
        else
        {
            m_vScale_Font.y -= fTimeDelta * 150.f;

            m_pHundreds->Set_Scale(m_vScale_Font);
            m_pTens->Set_Scale(m_vScale_Font);
            m_pOnes->Set_Scale(m_vScale_Font);
        }
    }
#pragma endregion
}

shared_ptr<CUI_Indicator_Damage_Font> CUI_Indicator_Damage_Font::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Damage_Font> pInstance = make_private_shared(CUI_Indicator_Damage_Font, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Damage_Font::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Damage_Font::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_Damage_Font> pInstance = make_private_shared_copy(CUI_Indicator_Damage_Font, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Damage_Font::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
// PLAYER_STATE Player_State = pPlayer->Get_PlayerState()->Get_CurrentState();
