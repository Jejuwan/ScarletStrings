#include "ClientPCH.h"
#include "UI_Indicator_Name_Boss.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include <bitset>

CUI_Indicator_Name_Boss::CUI_Indicator_Name_Boss(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Name_Boss::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Name_Boss::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to CGameObject::Initialize");
    }

    Initialize_UI();

    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Name_Boss::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Name_Boss::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Name_Boss::Render()
{
    if (true == m_bBossAppeared && false == m_bAnimation_End)
    {
        if (0.8f <= m_fAccNoiseTime1)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA1, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (0.8f <= m_fAccNoiseTime2)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA2, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (0.9f <= m_fAccNoiseTime3)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise3->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR3, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG3, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB3, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA3, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.1f <= m_fAccNoiseTime4)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise4->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise4->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR4, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG4, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB4, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA4, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.1f <= m_fAccNoiseTime5)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise5->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise5->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR5, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG5, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB5, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA5, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.2f <= m_fAccNoiseTime6)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise6->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise6->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR6, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG6, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB6, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA6, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.25f <= m_fAccNoiseTime7)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise7->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise7->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR7, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG7, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB7, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA7, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.3f <= m_fAccNoiseTime8)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise8->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise8->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR8, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG8, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB8, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA8, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.35f <= m_fAccNoiseTime9)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise9->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise9->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR9, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG9, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB9, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA9, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.5f <= m_fAccNoiseTime10)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise10->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise10->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR10, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG10, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB10, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA10, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.5f <= m_fAccNoiseTime11)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise11->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise11->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR11, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG11, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB11, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA11, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.5f <= m_fAccNoiseTime12)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise12->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise12->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR12, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG12, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB12, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA12, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.7f <= m_fAccNoiseTime13)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise13->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise13->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR13, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG13, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB13, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA13, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.7f <= m_fAccNoiseTime14)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise14->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise14->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR14, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG14, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB14, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA14, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.7f <= m_fAccNoiseTime15)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise15->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise15->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR15, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG15, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB15, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA15, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.75f <= m_fAccNoiseTime16)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise16->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise16->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR16, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG16, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB16, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA16, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.9f <= m_fAccNoiseTime17)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise17->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise17->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR17, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG17, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB17, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA17, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (1.9f <= m_fAccNoiseTime18)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise18->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise18->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR18, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG18, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB18, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA18, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (2.f <= m_fAccNoiseTime19)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise19->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise19->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR19, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG19, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB19, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA19, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (2.5f <= m_fAccNoiseTime20)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise20->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise20->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR20, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG20, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB20, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA20, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (4.1f <= m_fAccNoiseTime21)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise21->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise21->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR21, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG21, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB21, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA21, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (5.1f <= m_fAccNoiseTime22)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise22->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise22->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR22, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG22, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB22, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA22, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (5.15f <= m_fAccNoiseTime23)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise23->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise23->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR23, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG23, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB23, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA23, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (6.2f <= m_fAccNoiseTime24)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise24->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise24->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR24, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG24, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB24, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA24, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }
        if (6.5f <= m_fAccNoiseTime25)
        {
            /* Noise */
            if (FAILED(m_pTexture_Noise25->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pNoise25->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_R", &m_fR25, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_G", &m_fG25, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_B", &m_fB25, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fNoise_A", &m_fA25, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
            if (FAILED(__super::Render(47)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
            }
        }


        /* Background1 */
        if (FAILED(m_pTexture_Background_1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground_1->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fB1_Alpha", &m_fBackground1_Control_Alpha, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fB1_Discard", &m_fBackground1_Control_Discard, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
        }
        if (FAILED(__super::Render(44)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
        }

        if (true == m_bBackground2_Start)
        {
            if (false == m_bBackground2_Blink)
            {
                /* Background2 */
                if (FAILED(m_pTexture_Background_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBackground_2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_R", &m_fBackground2_R1, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_G", &m_fBackground2_G1, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_B", &m_fBackground2_B1, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_Alpha", &m_fBackground2_Control_Alpha1, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(__super::Render(45)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
            }
            else
            {
                /* Background2 */
                if (FAILED(m_pTexture_Background_2_2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pBackground_2_2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_R", &m_fBackground2_R2, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_G", &m_fBackground2_G2, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_B", &m_fBackground2_B2, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fB2_Alpha", &m_fBackground2_Control_Alpha2, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
                if (FAILED(__super::Render(45)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
                }
            }
        }
        /* Background3 */
        if (FAILED(m_pTexture_Background_3->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground_3->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fB3_Alpha", &m_fBackground3_Control_Alpha, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
        }
        if (FAILED(__super::Render(46)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Name_Boss::Render", "Failed to CUI_Indicator_Name_Boss::Render");
        }

        if (true == m_bRenderFont)
        {
            Render_RenderFont();
        }
    }



    return S_OK;
}

HRESULT CUI_Indicator_Name_Boss::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Menu_Background::Ready_Components", "Failed to Get_Component: SHADER");
    }
    
    return S_OK;
}

void CUI_Indicator_Name_Boss::Initialize_UI()
{    
//-------------------------------------------------------------------Transform
    m_pBackground_1                     = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_2                     = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_2_2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground_3                     = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise1                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise2                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise3                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise4                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise5                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise6                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise7                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise8                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise9                           = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise10                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise11                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise12                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise13                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise14                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise15                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise16                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise17                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise18                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise19                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise20                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise21                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise22                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise23                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise24                          = CTransform::Create(m_pDevice, m_pContext);
    m_pNoise25                          = CTransform::Create(m_pDevice, m_pContext);
    
//-------------------------------------------------------------------Texture
    m_pTexture_Background_1             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background1.png"));
    m_pTexture_Background_2             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background2.png"));
    m_pTexture_Background_2_2           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background2.png"));
    m_pTexture_Background_3             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/BossName_Background3.png"));
    m_pTexture_Noise1                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise2                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise3                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise4                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise5                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise6                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise7                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise8                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise9                   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise10                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise11                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise12                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise13                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise14                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise15                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise16                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise17                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise18                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise19                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise20                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise21                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise22                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise23                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise24                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));
    m_pTexture_Noise25                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Name/Boss/Noise.png"));

//-------------------------------------------------------------------_bool Common
    m_bBossAppeared                     = false;
    m_bAnimation_End                    = false;

    m_iAnimCount                        = 0;

//-------------------------------------------------------------------Background1
    m_bBackground1_Start                = false;
    m_bBackground1_RenderSwitch         = false;
    m_fBackground1_DelayingTime         = 0.f;
    m_fBackground1_Control_Alpha        = 0.f;
    m_fBackground1_Control_Discard      = 1.f;

//-------------------------------------------------------------------Background2
    m_bBackground2_Blink                = false;
    m_bBackground2_Start                = false;
    m_bBackground2_RenderSwitch         = false;
    m_fBackground2_DelayingTime         = 0.f;
    m_fBackground2_Control_Alpha1       = 0.f;
    m_fBackground2_Control_Alpha2       = 0.f;
    m_fBackground2_Control_Discard      = 0.f;
    m_fBackground2_Control_BlinkTime    = 0.f;
    m_fBackground2_R1                   = 1.f;
    m_fBackground2_G1                   = 1.f;
    m_fBackground2_B1                   = 1.f;
    m_fBackground2_R2                   = 0.f;
    m_fBackground2_G2                   = 0.f;
    m_fBackground2_B2                   = 0.f;

//-------------------------------------------------------------------Background3
    m_bBackground3_Start                = false;
    m_bBackground3_Switch               = false;
    m_fBackground3_DelayingTime         = 0.f;
    m_fBackground3_Control_Alpha        = 0.f;
    m_fBackground3_Control_Discard      = 0.f;

//-------------------------------------------------------------------Noise
    m_bNoise_Start1                     = false;
    m_fR1                               = 255.f;//rand() % 256;
    m_fG1                               = 255.f;//rand() % 256;
    m_fB1                               = 0.f;//rand() % 256;
    m_fA1                               = 0.f;
    m_fAccNoiseTime1                    = 0.f;

    m_bNoise_Start2                     = false;
    m_fR2                               = 127.f;//rand() % 256;
    m_fG2                               = 127.f;//rand() % 256;
    m_fB2                               = 255.f;//rand() % 256;
    m_fA2                               = 0.f;
    m_fAccNoiseTime2                    = 0.f;
    
    m_bNoise_Start3                     = false;
    m_fR3                               = 255.f;//rand() % 256;
    m_fG3                               = 127.f;//rand() % 256;
    m_fB3                               = 127.f;//rand() % 256;
    m_fA3                               = 0.f;
    m_fAccNoiseTime3                    = 0.f;

    m_bNoise_Start4                     = false;
    m_fR4                               = 127.f;//rand() % 256;
    m_fG4                               = 170.f;//rand() % 256;
    m_fB4                               = 255.f;//rand() % 256;
    m_fA4                               = 0.f;
    m_fAccNoiseTime4                    = 0.f;
    
    m_bNoise_Start5                     = false;
    m_fR5                               = 140.f;//rand() % 256;
    m_fG5                               = 187.f;//rand() % 256;
    m_fB5                               = 110.f;//rand() % 256;
    m_fA5                               = 0.f;
    m_fAccNoiseTime5                    = 0.f;
    
    m_bNoise_Start6                     = false;
    m_fR6                               = 80.f;//rand() % 256;
    m_fG6                               = 80.f;//rand() % 256;
    m_fB6                               = 80.f;//rand() % 256;
    m_fA6                               = 0.f;
    m_fAccNoiseTime6                    = 0.f;
    
    m_bNoise_Start7                     = false;
    m_fR7                               = 160.f;//rand() % 256;
    m_fG7                               = 227.f;//rand() % 256;
    m_fB7                               = 210.f;//rand() % 256;
    m_fA7                               = 0.f;
    m_fAccNoiseTime7                    = 0.f;
    
    m_bNoise_Start8                     = false;
    m_fR8                               = 157.f;//rand() % 256;
    m_fG8                               = 181.f;//rand() % 256;
    m_fB8                               = 255.f;//rand() % 256;
    m_fA8                               = 0.f;
    m_fAccNoiseTime8                    = 0.f;
    
    m_bNoise_Start9                     = false;
    m_fR9                               = 132.f;//rand() % 256;
    m_fG9                               = 127.f;//rand() % 256;
    m_fB9                               = 111.f;//rand() % 256;
    m_fA9                               = 0.f;
    m_fAccNoiseTime9                    = 0.f;
    
    m_bNoise_Start10                    = false;
    m_fR10                              = 206.f;//rand() % 256;
    m_fG10                              = 127.f;//rand() % 256;
    m_fB10                              = 176.f;//rand() % 256;
    m_fA10                              = 0.f;
    m_fAccNoiseTime10                   = 0.f;

    m_bNoise_Start11                    = false;
    m_fR11                              = 255.f;//rand() % 256;
    m_fG11                              = 20.f;//rand() % 256;
    m_fB11                              = 60.f;//rand() % 256;
    m_fA11                              = 0.f;
    m_fAccNoiseTime11                   = 0.f;

    m_bNoise_Start12                    = false;
    m_fR12                              = 10.f;//rand() % 256;
    m_fG12                              = 255.f;//rand() % 256;
    m_fB12                              = 40.f;//rand() % 256;
    m_fA12                              = 0.f;
    m_fAccNoiseTime12                   = 0.f;
    
    m_bNoise_Start13                    = false;
    m_fR13                              = 255.f;//rand() % 256;
    m_fG13                              = 255.f;//rand() % 256;
    m_fB13                              = 40.f;//rand() % 256;
    m_fA13                              = 0.f;
    m_fAccNoiseTime13                   = 0.f;

    m_bNoise_Start14                    = false;
    m_fR14                              = 0.f;//rand() % 256;
    m_fG14                              = 56.f;//rand() % 256;
    m_fB14                              = 240.f;//rand() % 256;
    m_fA14                              = 0.f;
    m_fAccNoiseTime14                   = 0.f;

    m_bNoise_Start15                    = false;
    m_fR15                              = 10.f;//rand() % 256;
    m_fG15                              = 255.f;//rand() % 256;
    m_fB15                              = 250.f;//rand() % 256;
    m_fA15                              = 0.f;
    m_fAccNoiseTime15                   = 0.f;

    m_bNoise_Start16                    = false;
    m_fR16                              = 255.f;//rand() % 256;
    m_fG16                              = 0.f;//rand() % 256;
    m_fB16                              = 40.f;//rand() % 256;
    m_fA16                              = 0.f;
    m_fAccNoiseTime16                   = 0.f;

    m_bNoise_Start17                    = false;
    m_fR17                              = 120.f;//rand() % 256;
    m_fG17                              = 65.f;//rand() % 256;
    m_fB17                              = 10.f;//rand() % 256;
    m_fA17                              = 0.f;
    m_fAccNoiseTime17                   = 0.f;

    m_bNoise_Start18                    = false;
    m_fR18                              = 120.f;//rand() % 256;
    m_fG18                              = 255.f;//rand() % 256;
    m_fB18                              = 40.f;//rand() % 256;
    m_fA18                              = 0.f;
    m_fAccNoiseTime18                   = 0.f;

    m_bNoise_Start19                    = false;
    m_fR19                              = 140.f;//rand() % 256;
    m_fG19                              = 54.f;//rand() % 256;
    m_fB19                              = 47.f;//rand() % 256;
    m_fA19                              = 0.f;
    m_fAccNoiseTime19                   = 0.f;

    m_bNoise_Start20                    = false;
    m_fR20                              = 110.f;//rand() % 256;
    m_fG20                              = 38.f;//rand() % 256;
    m_fB20                              = 254.f;//rand() % 256;
    m_fA20                              = 0.f;
    m_fAccNoiseTime20                   = 0.f;

    m_bNoise_Start21                    = false;
    m_fR21                              = 255.f;//rand() % 256;
    m_fG21                              = 255.f;//rand() % 256;
    m_fB21                              = 0.f;//rand() % 256;
    m_fA21                              = 0.f;
    m_fAccNoiseTime21                   = 0.f;

    m_bNoise_Start22                    = false;
    m_fR22                              = 255.f;//rand() % 256;
    m_fG22                              = 255.f;//rand() % 256;
    m_fB22                              = 0.f;//rand() % 256;
    m_fA22                              = 0.f;
    m_fAccNoiseTime22                   = 0.f;

    m_bNoise_Start23                    = false;
    m_fR23                              = 0.f;//rand() % 256;
    m_fG23                              = 255.f;//rand() % 256;
    m_fB23                              = 0.f;//rand() % 256;
    m_fA23                              = 0.f;
    m_fAccNoiseTime23                   = 0.f;

    m_bNoise_Start24                    = false;
    m_fR24                              = 0.f;//rand() % 256;
    m_fG24                              = 255.f;//rand() % 256;
    m_fB24                              = 255.f;//rand() % 256;
    m_fA24                              = 0.f;
    m_fAccNoiseTime24                   = 0.f;

    m_bNoise_Start25                    = false;
    m_fR25                              = 0.f;//rand() % 256;
    m_fG25                              = 0.f;//rand() % 256;
    m_fB25                              = 255.f;//rand() % 256;
    m_fA25                              = 0.f;
    m_fAccNoiseTime25                   = 0.f;
//-------------------------------------------------------------------_float3
    m_vScale_Background_1               = _float3(880.f, 256.f, 1.f);
    m_vScale_Background_2               = _float3(940.f, 256.f, 1.f);
    m_vScale_Background_3               = _float3(940.f, 256.f, 1.f);

    m_vScale_Noise1                     = _float3(100.f, 20.f, 1.f);
    m_vScale_Noise2                     = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise3                     = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise4                     = _float3(70.f, 20.f, 1.f);
    m_vScale_Noise5                     = _float3(120.f, 20.f, 1.f);
    m_vScale_Noise6                     = _float3(600.f, 120.f, 1.f);
    m_vScale_Noise7                     = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise8                     = _float3(100.f, 20.f, 1.f);
    m_vScale_Noise9                     = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise10                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise11                    = _float3(100.f, 20.f, 1.f);
    m_vScale_Noise12                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise13                    = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise14                    = _float3(70.f, 20.f, 1.f);
    m_vScale_Noise15                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise16                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise17                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise18                    = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise19                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise20                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise21                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise22                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise23                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise24                    = _float3(100.f, 20.f, 1.f);
    m_vScale_Noise25                    = _float3(60.f, 20.f, 1.f);


//-------------------------------------------------------------------_float4
    m_vPosition_Background_1            = _float4(0.f, 150.f, 0.f, 1.f);
    m_vPosition_Background_2            = _float4(0.f, 150.f, 0.f, 1.f);
    m_vPosition_Background_3            = _float4(30.f, 150.f, 0.f, 1.f);

    m_vPosition_Noise1                  = _float4(-200.f, 200.f, 0.f, 1.f);
    m_vPosition_Noise2                  = _float4(220.f, 50.f, 0.f, 1.f);
    m_vPosition_Noise3                  = _float4(100.f, 160.f, 0.f, 1.f);
    m_vPosition_Noise4                  = _float4(-70.f, 200.f, 0.f, 1.f);
    m_vPosition_Noise5                  = _float4(-70.f, 120.f, 0.f, 1.f);
    m_vPosition_Noise6                  = _float4(0.f, 160.f, 0.f, 1.f);
    m_vPosition_Noise7                  = _float4(200.f, 180.f, 0.f, 1.f);
    m_vPosition_Noise8                  = _float4(-150.f, 0.f, 0.f, 1.f);
    m_vPosition_Noise9                  = _float4(120.f, 120.f, 0.f, 1.f);
    m_vPosition_Noise10                 = _float4(-20.f, 70.f, 0.f, 1.f);
    m_vPosition_Noise11                 = _float4(-160.f, 200.f, 0.f, 1.f);
    m_vPosition_Noise12                 = _float4(160.f, 60.f, 0.f, 1.f);
    m_vPosition_Noise13                 = _float4(160.f, 120.f, 0.f, 1.f);
    m_vPosition_Noise14                 = _float4(220.f, 180.f, 0.f, 1.f);
    m_vPosition_Noise15                 = _float4(-220.f, 160.f, 0.f, 1.f);
    m_vPosition_Noise16                 = _float4(-150.f, 100.f, 0.f, 1.f);
    m_vPosition_Noise17                 = _float4(160.f, 50.f, 0.f, 1.f);
    m_vPosition_Noise18                 = _float4(60.f, 90.f, 0.f, 1.f);
    m_vPosition_Noise19                 = _float4(-20.f, 40.f, 0.f, 1.f);
    m_vPosition_Noise20                 = _float4(200.f, 200.f, 0.f, 1.f);
    m_vPosition_Noise21                 = _float4(-130.f, 180.f, 0.f, 1.f);
    m_vPosition_Noise22                 = _float4(-200.f, 60.f, 0.f, 1.f);
    m_vPosition_Noise23                 = _float4(180.f, 80.f, 0.f, 1.f);
    m_vPosition_Noise24                 = _float4(200.f, 170.f, 0.f, 1.f);
    m_vPosition_Noise25                 = _float4(-10.f, 200.f, 0.f, 1.f);


//-------------------------------------------------------------------wstring
    m_strBossName                       = L"The Other Appeared";
    m_fPosition_FontX                   = g_iWinCX * 0.5f - 250.f;
    m_fTime_RenderFont                  = 0.f;
    m_fTime_RenderFontAlpha_E_OutLine   = 0.f;
    m_bRenderFont                       = false;

//-------------------------------------------------------------------Set_Scale
    m_pBackground_1->Set_Scale(m_vScale_Background_1);
    m_pBackground_2->Set_Scale(m_vScale_Background_2);
    m_pBackground_2_2->Set_Scale(m_vScale_Background_2);
    m_pBackground_3->Set_Scale(m_vScale_Background_3);
    m_pNoise1->Set_Scale(m_vScale_Noise1);
    m_pNoise2->Set_Scale(m_vScale_Noise2);
    m_pNoise3->Set_Scale(m_vScale_Noise3);
    m_pNoise4->Set_Scale(m_vScale_Noise4);
    m_pNoise5->Set_Scale(m_vScale_Noise5);
    m_pNoise6->Set_Scale(m_vScale_Noise6);
    m_pNoise7->Set_Scale(m_vScale_Noise7);
    m_pNoise8->Set_Scale(m_vScale_Noise8);
    m_pNoise9->Set_Scale(m_vScale_Noise9);
    m_pNoise10->Set_Scale(m_vScale_Noise10);
    m_pNoise11->Set_Scale(m_vScale_Noise11);
    m_pNoise12->Set_Scale(m_vScale_Noise12);
    m_pNoise13->Set_Scale(m_vScale_Noise13);
    m_pNoise14->Set_Scale(m_vScale_Noise14);
    m_pNoise15->Set_Scale(m_vScale_Noise15);
    m_pNoise16->Set_Scale(m_vScale_Noise16);
    m_pNoise17->Set_Scale(m_vScale_Noise17);
    m_pNoise18->Set_Scale(m_vScale_Noise18);
    m_pNoise19->Set_Scale(m_vScale_Noise19);
    m_pNoise20->Set_Scale(m_vScale_Noise20);
    m_pNoise21->Set_Scale(m_vScale_Noise21);
    m_pNoise22->Set_Scale(m_vScale_Noise22);
    m_pNoise23->Set_Scale(m_vScale_Noise23);
    m_pNoise24->Set_Scale(m_vScale_Noise24);
    m_pNoise25->Set_Scale(m_vScale_Noise25);

//-------------------------------------------------------------------Set_Position
    m_pBackground_1->Set_State(TRANSFORM::POSITION, m_vPosition_Background_1);
    m_pBackground_2->Set_State(TRANSFORM::POSITION, m_vPosition_Background_2);
    m_pBackground_2_2->Set_State(TRANSFORM::POSITION, m_vPosition_Background_2);
    m_pBackground_3->Set_State(TRANSFORM::POSITION, m_vPosition_Background_3);
    m_pNoise1->Set_State(TRANSFORM::POSITION, m_vPosition_Noise1);
    m_pNoise2->Set_State(TRANSFORM::POSITION, m_vPosition_Noise2);
    m_pNoise3->Set_State(TRANSFORM::POSITION, m_vPosition_Noise3);
    m_pNoise4->Set_State(TRANSFORM::POSITION, m_vPosition_Noise4);
    m_pNoise5->Set_State(TRANSFORM::POSITION, m_vPosition_Noise5);
    m_pNoise6->Set_State(TRANSFORM::POSITION, m_vPosition_Noise6);
    m_pNoise7->Set_State(TRANSFORM::POSITION, m_vPosition_Noise7);
    m_pNoise8->Set_State(TRANSFORM::POSITION, m_vPosition_Noise8);
    m_pNoise9->Set_State(TRANSFORM::POSITION, m_vPosition_Noise9);
    m_pNoise10->Set_State(TRANSFORM::POSITION, m_vPosition_Noise10);
    m_pNoise11->Set_State(TRANSFORM::POSITION, m_vPosition_Noise11);
    m_pNoise12->Set_State(TRANSFORM::POSITION, m_vPosition_Noise12);
    m_pNoise13->Set_State(TRANSFORM::POSITION, m_vPosition_Noise13);
    m_pNoise14->Set_State(TRANSFORM::POSITION, m_vPosition_Noise14);
    m_pNoise15->Set_State(TRANSFORM::POSITION, m_vPosition_Noise15);
    m_pNoise16->Set_State(TRANSFORM::POSITION, m_vPosition_Noise16);
    m_pNoise17->Set_State(TRANSFORM::POSITION, m_vPosition_Noise17);
    m_pNoise18->Set_State(TRANSFORM::POSITION, m_vPosition_Noise18);
    m_pNoise19->Set_State(TRANSFORM::POSITION, m_vPosition_Noise19);
    m_pNoise20->Set_State(TRANSFORM::POSITION, m_vPosition_Noise20);
    m_pNoise21->Set_State(TRANSFORM::POSITION, m_vPosition_Noise21);
    m_pNoise22->Set_State(TRANSFORM::POSITION, m_vPosition_Noise22);
    m_pNoise23->Set_State(TRANSFORM::POSITION, m_vPosition_Noise23);
    m_pNoise24->Set_State(TRANSFORM::POSITION, m_vPosition_Noise24);
    m_pNoise25->Set_State(TRANSFORM::POSITION, m_vPosition_Noise25);

}

void CUI_Indicator_Name_Boss::Tick_UI(_float fTimeDelta)
{
    Start_Animation_Trigger();
    
    if (true == m_bBossAppeared)
    {
        Control_Noise(fTimeDelta);
        Control_Background1(fTimeDelta);
        Control_Background2(fTimeDelta);
        Control_Background3(fTimeDelta);
        Control_RenderFont(fTimeDelta);
    }
}

void CUI_Indicator_Name_Boss::Initialize_Variables()
{
    //-------------------------------------------------------------------_bool Common
    m_bBossAppeared                     = false;
    m_bAnimation_End                    = false;

//-------------------------------------------------------------------Background1
    m_bBackground1_Start                = false;
    m_bBackground1_RenderSwitch         = false;
    m_fBackground1_DelayingTime         = 0.f;
    m_fBackground1_Control_Alpha        = 0.f;
    m_fBackground1_Control_Discard      = 1.f;

//-------------------------------------------------------------------Background2
    m_bBackground2_Blink                = false;
    m_bBackground2_Start                = false;
    m_bBackground2_RenderSwitch         = false;
    m_fBackground2_DelayingTime         = 0.f;
    m_fBackground2_Control_Alpha1       = 0.f;
    m_fBackground2_Control_Alpha2       = 0.f;
    m_fBackground2_Control_Discard      = 0.f;
    m_fBackground2_Control_BlinkTime    = 0.f;
    m_fBackground2_R1                   = 1.f;
    m_fBackground2_G1                   = 1.f;
    m_fBackground2_B1                   = 1.f;
    m_fBackground2_R2                   = 0.f;
    m_fBackground2_G2                   = 0.f;
    m_fBackground2_B2                   = 0.f;

//-------------------------------------------------------------------Background3
    m_bBackground3_Start                = false;
    m_bBackground3_Switch               = false;
    m_fBackground3_DelayingTime         = 0.f;
    m_fBackground3_Control_Alpha        = 0.f;
    m_fBackground3_Control_Discard      = 0.f;

//-------------------------------------------------------------------Noise Color.RGBA
    m_bNoise_Start1                     = false;
    m_fR1                               = 255.f;//rand() % 256;
    m_fG1                               = 255.f;//rand() % 256;
    m_fB1                               = 0.f;//rand() % 256;
    m_fA1                               = 0.f;
    m_fAccNoiseTime1                    = 0.f;

    m_bNoise_Start2                     = false;
    m_fR2                               = 27.f;//rand() % 256;
    m_fG2                               = 27.f;//rand() % 256;
    m_fB2                               = 255.f;//rand() % 256;
    m_fA2                               = 0.f;
    m_fAccNoiseTime2                    = 0.f;
    
    m_bNoise_Start3                     = false;
    m_fR3                               = 5.f;//rand() % 256;
    m_fG3                               = 227.f;//rand() % 256;
    m_fB3                               = 27.f;//rand() % 256;
    m_fA3                               = 0.f;
    m_fAccNoiseTime3                    = 0.f;

    m_bNoise_Start4                     = false;
    m_fR4                               = 127.f;//rand() % 256;
    m_fG4                               = 70.f;//rand() % 256;
    m_fB4                               = 255.f;//rand() % 256;
    m_fA4                               = 0.f;
    m_fAccNoiseTime4                    = 0.f;
    
    m_bNoise_Start5                     = false;
    m_fR5                               = 240.f;//rand() % 256;
    m_fG5                               = 187.f;//rand() % 256;
    m_fB5                               = 110.f;//rand() % 256;
    m_fA5                               = 0.f;
    m_fAccNoiseTime5                    = 0.f;
    
    m_bNoise_Start6                     = false;
    m_fR6                               = 180.f;//rand() % 256;
    m_fG6                               = 180.f;//rand() % 256;
    m_fB6                               = 180.f;//rand() % 256;
    m_fA6                               = 0.f;
    m_fAccNoiseTime6                    = 0.f;
    
    m_bNoise_Start7                     = false;
    m_fR7                               = 160.f;//rand() % 256;
    m_fG7                               = 127.f;//rand() % 256;
    m_fB7                               = 210.f;//rand() % 256;
    m_fA7                               = 0.f;
    m_fAccNoiseTime7                    = 0.f;
    
    m_bNoise_Start8                     = false;
    m_fR8                               = 57.f;//rand() % 256;
    m_fG8                               = 81.f;//rand() % 256;
    m_fB8                               = 255.f;//rand() % 256;
    m_fA8                               = 0.f;
    m_fAccNoiseTime8                    = 0.f;
    
    m_bNoise_Start9                     = false;
    m_fR9                               = 232.f;//rand() % 256;
    m_fG9                               = 27.f;//rand() % 256;
    m_fB9                               = 11.f;//rand() % 256;
    m_fA9                               = 0.f;
    m_fAccNoiseTime9                    = 0.f;
    
    m_bNoise_Start10                    = false;
    m_fR10                              = 06.f;//rand() % 256;
    m_fG10                              = 227.f;//rand() % 256;
    m_fB10                              = 76.f;//rand() % 256;
    m_fA10                              = 0.f;
    m_fAccNoiseTime10                   = 0.f;

    m_bNoise_Start11                    = false;
    m_fR11                              = 255.f;//rand() % 256;
    m_fG11                              = 20.f;//rand() % 256;
    m_fB11                              = 60.f;//rand() % 256;
    m_fA11                              = 0.f;
    m_fAccNoiseTime11                   = 0.f;

    m_bNoise_Start12                    = false;
    m_fR12                              = 10.f;//rand() % 256;
    m_fG12                              = 255.f;//rand() % 256;
    m_fB12                              = 40.f;//rand() % 256;
    m_fA12                              = 0.f;
    m_fAccNoiseTime12                   = 0.f;

    m_bNoise_Start13                    = false;
    m_fR13                              = 255.f;//rand() % 256;
    m_fG13                              = 255.f;//rand() % 256;
    m_fB13                              = 40.f;//rand() % 256;
    m_fA13                              = 0.f;
    m_fAccNoiseTime13                   = 0.f;

    m_bNoise_Start14                    = false;
    m_fR14                              = 0.f;//rand() % 256;
    m_fG14                              = 56.f;//rand() % 256;
    m_fB14                              = 240.f;//rand() % 256;
    m_fA14                              = 0.f;
    m_fAccNoiseTime14                   = 0.f;

    m_bNoise_Start15                    = false;
    m_fR15                              = 10.f;//rand() % 256;
    m_fG15                              = 255.f;//rand() % 256;
    m_fB15                              = 250.f;//rand() % 256;
    m_fA15                              = 0.f;
    m_fAccNoiseTime15                   = 0.f;

    m_bNoise_Start16                    = false;
    m_fR16                              = 255.f;//rand() % 256;
    m_fG16                              = 0.f;//rand() % 256;
    m_fB16                              = 40.f;//rand() % 256;
    m_fA16                              = 0.f;
    m_fAccNoiseTime16                   = 0.f;

    m_bNoise_Start17                    = false;
    m_fR17                              = 120.f;//rand() % 256;
    m_fG17                              = 65.f;//rand() % 256;
    m_fB17                              = 10.f;//rand() % 256;
    m_fA17                              = 0.f;
    m_fAccNoiseTime17                   = 0.f;

    m_bNoise_Start18                    = false;
    m_fR18                              = 120.f;//rand() % 256;
    m_fG18                              = 255.f;//rand() % 256;
    m_fB18                              = 40.f;//rand() % 256;
    m_fA18                              = 0.f;
    m_fAccNoiseTime18                   = 0.f;

    m_bNoise_Start19                    = false;
    m_fR19                              = 140.f;//rand() % 256;
    m_fG19                              = 54.f;//rand() % 256;
    m_fB19                              = 47.f;//rand() % 256;
    m_fA19                              = 0.f;
    m_fAccNoiseTime19                   = 0.f;

    m_bNoise_Start20                    = false;
    m_fR20                              = 110.f;//rand() % 256;
    m_fG20                              = 38.f;//rand() % 256;
    m_fB20                              = 254.f;//rand() % 256;
    m_fA20                              = 0.f;
    m_fAccNoiseTime20                   = 0.f;

    m_bNoise_Start21                    = false;
    m_fR21                              = 255.f;//rand() % 256;
    m_fG21                              = 255.f;//rand() % 256;
    m_fB21                              = 0.f;//rand() % 256;
    m_fA21                              = 0.f;
    m_fAccNoiseTime21                   = 0.f;

    m_bNoise_Start22                    = false;
    m_fR22                              = 255.f;//rand() % 256;
    m_fG22                              = 0.f;//rand() % 256;
    m_fB22                              = 0.f;//rand() % 256;
    m_fA22                              = 0.f;
    m_fAccNoiseTime22                   = 0.f;

    m_bNoise_Start23                    = false;
    m_fR23                              = 0.f;//rand() % 256;
    m_fG23                              = 255.f;//rand() % 256;
    m_fB23                              = 0.f;//rand() % 256;
    m_fA23                              = 0.f;
    m_fAccNoiseTime23                   = 0.f;

    m_bNoise_Start24                    = false;
    m_fR24                              = 0.f;//rand() % 256;
    m_fG24                              = 0.f;//rand() % 256;
    m_fB24                              = 255.f;//rand() % 256;
    m_fA24                              = 0.f;
    m_fAccNoiseTime24                   = 0.f;

    m_bNoise_Start25                    = false;
    m_fR25                              = 255.f;//rand() % 256;
    m_fG25                              = 0.f;//rand() % 256;
    m_fB25                              = 0.f;//rand() % 256;
    m_fA25                              = 0.f;
    m_fAccNoiseTime25                   = 0.f;
//-------------------------------------------------------------------_float3
    m_vScale_Background_1               = _float3(880.f,    256.f,  1.f);
    m_vScale_Background_2               = _float3(940.f,    256.f,  1.f);
    m_vScale_Background_3               = _float3(940.f,    256.f,  1.f);

    m_vScale_Noise1                     = _float3(100.f,    20.f,   1.f);
    m_vScale_Noise2                     = _float3(80.f,     20.f,   1.f);
    m_vScale_Noise3                     = _float3(110.f,    20.f,   1.f);
    m_vScale_Noise4                     = _float3(70.f,     20.f,   1.f);
    m_vScale_Noise5                     = _float3(120.f,    20.f,   1.f);
    m_vScale_Noise6                     = _float3(600.f,    120.f,  1.f);
    m_vScale_Noise7                     = _float3(80.f,     20.f,   1.f);
    m_vScale_Noise8                     = _float3(100.f,    20.f,   1.f);
    m_vScale_Noise9                     = _float3(110.f,    20.f,   1.f);
    m_vScale_Noise10                    = _float3(80.f,     20.f,   1.f);
    m_vScale_Noise11                    = _float3(100.f,    20.f,   1.f);
    m_vScale_Noise12                    = _float3(60.f,     20.f,   1.f);
    m_vScale_Noise13                    = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise14                    = _float3(70.f, 20.f, 1.f);
    m_vScale_Noise15                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise16                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise17                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise18                    = _float3(110.f, 20.f, 1.f);
    m_vScale_Noise19                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise20                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise21                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise22                    = _float3(80.f, 20.f, 1.f);
    m_vScale_Noise23                    = _float3(60.f, 20.f, 1.f);
    m_vScale_Noise24                    = _float3(100.f, 20.f, 1.f);
    m_vScale_Noise25                    = _float3(60.f, 20.f, 1.f);
//-------------------------------------------------------------------_float4
    m_vPosition_Background_1            = _float4(0.f,  150.f,      0.f, 1.f);
    m_vPosition_Background_2            = _float4(0.f,  150.f,      0.f, 1.f);
    m_vPosition_Background_3            = _float4(30.f, 150.f,      0.f, 1.f);

    m_vPosition_Noise1                  = _float4(-200.f,   200.f,  0.f, 1.f);
    m_vPosition_Noise2                  = _float4(220.f,    50.f,   0.f, 1.f);
    m_vPosition_Noise3                  = _float4(100.f,    160.f,  0.f, 1.f);
    m_vPosition_Noise4                  = _float4(-70.f,    200.f,  0.f, 1.f);
    m_vPosition_Noise5                  = _float4(-70.f,    120.f,  0.f, 1.f);
    m_vPosition_Noise6                  = _float4(0.f,      160.f,  0.f, 1.f);
    m_vPosition_Noise7                  = _float4(200.f,    180.f,  0.f, 1.f);
    m_vPosition_Noise8                  = _float4(-150.f,   0.f,    0.f, 1.f);
    m_vPosition_Noise9                  = _float4(120.f,    120.f,  0.f, 1.f);
    m_vPosition_Noise10                 = _float4(-20.f,    70.f,   0.f, 1.f);
    m_vPosition_Noise11                 = _float4(-160.f,   200.f,  0.f, 1.f);
    m_vPosition_Noise12                 = _float4(160.f,    60.f,   0.f, 1.f);
    m_vPosition_Noise13                 = _float4(160.f, 120.f, 0.f, 1.f);
    m_vPosition_Noise14                 = _float4(220.f, 180.f, 0.f, 1.f);
    m_vPosition_Noise15                 = _float4(-220.f, 160.f, 0.f, 1.f);
    m_vPosition_Noise16                 = _float4(-150.f, 100.f, 0.f, 1.f);
    m_vPosition_Noise17                 = _float4(160.f, 50.f, 0.f, 1.f);
    m_vPosition_Noise18                 = _float4(60.f, 90.f, 0.f, 1.f);
    m_vPosition_Noise19                 = _float4(-20.f, 40.f, 0.f, 1.f);
    m_vPosition_Noise20                 = _float4(200.f, 200.f, 0.f, 1.f);
    m_vPosition_Noise21                 = _float4(-130.f, 180.f, 0.f, 1.f);
    m_vPosition_Noise22                 = _float4(-200.f, 60.f, 0.f, 1.f);
    m_vPosition_Noise23                 = _float4(180.f, 80.f, 0.f, 1.f);
    m_vPosition_Noise24                 = _float4(200.f, 170.f, 0.f, 1.f);
    m_vPosition_Noise25                 = _float4(-10.f, 200.f, 0.f, 1.f);
//-------------------------------------------------------------------wstring
    m_strBossName                       = L"The Other Appeared";
    m_fTime_RenderFontAlpha_E_OutLine   = 0.f;
    m_fTime_RenderFont                  = 0.f;
    m_fPosition_FontX                   = g_iWinCX * 0.5f - 250.f;
    m_bRenderFont                       = false; 

//-------------------------------------------------------------------Set_Scale
    m_pBackground_1->Set_Scale(m_vScale_Background_1);
    m_pBackground_2->Set_Scale(m_vScale_Background_2);
    m_pBackground_2_2->Set_Scale(m_vScale_Background_2);
    m_pBackground_3->Set_Scale(m_vScale_Background_3);
    m_pNoise1->Set_Scale(m_vScale_Noise1);
    m_pNoise2->Set_Scale(m_vScale_Noise2);
    m_pNoise3->Set_Scale(m_vScale_Noise3);
    m_pNoise4->Set_Scale(m_vScale_Noise4);
    m_pNoise5->Set_Scale(m_vScale_Noise5);
    m_pNoise6->Set_Scale(m_vScale_Noise6);
    m_pNoise7->Set_Scale(m_vScale_Noise7);
    m_pNoise8->Set_Scale(m_vScale_Noise8);
    m_pNoise9->Set_Scale(m_vScale_Noise9);
    m_pNoise10->Set_Scale(m_vScale_Noise10);
    m_pNoise11->Set_Scale(m_vScale_Noise11);
    m_pNoise12->Set_Scale(m_vScale_Noise12);
    m_pNoise13->Set_Scale(m_vScale_Noise13);
    m_pNoise14->Set_Scale(m_vScale_Noise14);
    m_pNoise15->Set_Scale(m_vScale_Noise15);
    m_pNoise16->Set_Scale(m_vScale_Noise16);
    m_pNoise17->Set_Scale(m_vScale_Noise17);
    m_pNoise18->Set_Scale(m_vScale_Noise18);
    m_pNoise19->Set_Scale(m_vScale_Noise19);
    m_pNoise20->Set_Scale(m_vScale_Noise20);
    m_pNoise21->Set_Scale(m_vScale_Noise21);
    m_pNoise22->Set_Scale(m_vScale_Noise22);
    m_pNoise23->Set_Scale(m_vScale_Noise23);
    m_pNoise24->Set_Scale(m_vScale_Noise24);
    m_pNoise25->Set_Scale(m_vScale_Noise25);

//-------------------------------------------------------------------Set_Position
    m_pBackground_1->Set_State(TRANSFORM::POSITION, m_vPosition_Background_1);
    m_pBackground_2->Set_State(TRANSFORM::POSITION, m_vPosition_Background_2);
    m_pBackground_2_2->Set_State(TRANSFORM::POSITION, m_vPosition_Background_2);
    m_pBackground_3->Set_State(TRANSFORM::POSITION, m_vPosition_Background_3);
    m_pNoise1->Set_State(TRANSFORM::POSITION, m_vPosition_Noise1);
    m_pNoise2->Set_State(TRANSFORM::POSITION, m_vPosition_Noise2);
    m_pNoise3->Set_State(TRANSFORM::POSITION, m_vPosition_Noise3);
    m_pNoise4->Set_State(TRANSFORM::POSITION, m_vPosition_Noise4);
    m_pNoise5->Set_State(TRANSFORM::POSITION, m_vPosition_Noise5);
    m_pNoise6->Set_State(TRANSFORM::POSITION, m_vPosition_Noise6);
    m_pNoise7->Set_State(TRANSFORM::POSITION, m_vPosition_Noise7);
    m_pNoise8->Set_State(TRANSFORM::POSITION, m_vPosition_Noise8);
    m_pNoise9->Set_State(TRANSFORM::POSITION, m_vPosition_Noise9);
    m_pNoise10->Set_State(TRANSFORM::POSITION, m_vPosition_Noise10);
    m_pNoise11->Set_State(TRANSFORM::POSITION, m_vPosition_Noise11);
    m_pNoise12->Set_State(TRANSFORM::POSITION, m_vPosition_Noise12);
    m_pNoise13->Set_State(TRANSFORM::POSITION, m_vPosition_Noise13);
    m_pNoise14->Set_State(TRANSFORM::POSITION, m_vPosition_Noise14);
    m_pNoise15->Set_State(TRANSFORM::POSITION, m_vPosition_Noise15);
    m_pNoise16->Set_State(TRANSFORM::POSITION, m_vPosition_Noise16);
    m_pNoise17->Set_State(TRANSFORM::POSITION, m_vPosition_Noise17);
    m_pNoise18->Set_State(TRANSFORM::POSITION, m_vPosition_Noise18);
    m_pNoise19->Set_State(TRANSFORM::POSITION, m_vPosition_Noise19);
    m_pNoise20->Set_State(TRANSFORM::POSITION, m_vPosition_Noise20);
    m_pNoise21->Set_State(TRANSFORM::POSITION, m_vPosition_Noise21);
    m_pNoise22->Set_State(TRANSFORM::POSITION, m_vPosition_Noise22);
    m_pNoise23->Set_State(TRANSFORM::POSITION, m_vPosition_Noise23);
    m_pNoise24->Set_State(TRANSFORM::POSITION, m_vPosition_Noise24);
    m_pNoise25->Set_State(TRANSFORM::POSITION, m_vPosition_Noise25);

}

void CUI_Indicator_Name_Boss::Start_Animation_Trigger()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_F3))
    //{
    //    m_bBossAppeared     = true;
    //    m_bAnimation_End    = false;
    //}

    if (0 == m_iAnimCount)
    {
        m_bBossAppeared = true;
        m_bAnimation_End = false;
    }
}

void CUI_Indicator_Name_Boss::Control_Noise(_float fTimeDelta)
{
    /* Alpha */
#pragma region Time
    m_fAccNoiseTime1 += fTimeDelta * 1.6f;
    m_fAccNoiseTime2 += fTimeDelta * 1.6f;
    m_fAccNoiseTime3 += fTimeDelta * 1.6f;
    m_fAccNoiseTime4 += fTimeDelta * 1.4f;
    m_fAccNoiseTime5 += fTimeDelta * 1.4f;
    m_fAccNoiseTime6 += fTimeDelta * 1.6f;
    m_fAccNoiseTime7 += fTimeDelta * 1.6f;
    m_fAccNoiseTime8 += fTimeDelta * 1.6f;
    m_fAccNoiseTime9 += fTimeDelta * 1.6f;
    m_fAccNoiseTime10 += fTimeDelta * 1.6f;
    m_fAccNoiseTime11 += fTimeDelta * 1.3f;
    m_fAccNoiseTime12 += fTimeDelta * 1.5f;
    m_fAccNoiseTime13 += fTimeDelta * 1.4f;
    m_fAccNoiseTime14 += fTimeDelta * 1.6f;
    m_fAccNoiseTime15 += fTimeDelta * 1.7f;
    m_fAccNoiseTime16 += fTimeDelta * 1.5f;
    m_fAccNoiseTime17 += fTimeDelta * 1.5f;
    m_fAccNoiseTime18 += fTimeDelta * 1.3f;
    m_fAccNoiseTime19 += fTimeDelta * 1.3f;
    m_fAccNoiseTime20 += fTimeDelta * 1.5f;
    m_fAccNoiseTime21 += fTimeDelta * 1.4f;
    m_fAccNoiseTime22 += fTimeDelta * 1.5f;
    m_fAccNoiseTime23 += fTimeDelta * 1.7f;
    m_fAccNoiseTime24 += fTimeDelta * 1.7f;
    m_fAccNoiseTime25 += fTimeDelta * 1.7f;

#pragma endregion

#pragma region Noise_1
    if (0.8f <= m_fAccNoiseTime1)
    {
        if (1.f <= m_fA1)
        {
            m_fA1 = 1.f;
        }
        else
        {
            m_fA1 += fTimeDelta * 4.5f;
        }
    }
#pragma endregion
#pragma region Noise_2
    if (0.8f <= m_fAccNoiseTime2)
    {
        if (1.f <= m_fA2)
        {
            m_fA2 = 1.f;
        }
        else
        {
            m_fA2 += fTimeDelta * 4.5f;
        }
    }
#pragma endregion
#pragma region Noise_3
    if (0.9f <= m_fAccNoiseTime3)
    {
        if (1.f <= m_fA3)
        {
            m_fA3 = 1.f;
        }
        else
        {
            m_fA3 += fTimeDelta * 5.f;
        }
    }
#pragma endregion
#pragma region Noise_4
    if (1.1f <= m_fAccNoiseTime4)
    {
        if (1.0f <= m_fA4)
        {
            m_fA4 = 1.f;
        }
        else
        {
            m_fA4 += fTimeDelta * 4.5f;
        }
    }
#pragma endregion
#pragma region Noise_5
    if (1.1f <= m_fAccNoiseTime5)
    {
        if (1.f <= m_fA5)
        {
            m_fA5 = 1.f;
        }
        else
        {
            m_fA5 += fTimeDelta * 5.f;
        }
    }
#pragma endregion
#pragma region Noise_6
    if (1.2f <= m_fAccNoiseTime6)
    {
        if (1.f <= m_fA2)
        {
            m_fA6 = 1.f;
        }
        else
        {
            m_fA6 += fTimeDelta * 4.f;
            m_vScale_Noise6.y -= fTimeDelta * 60.f;
            m_pNoise6->Set_Scale(m_vScale_Noise6);
        }
    }
#pragma endregion
#pragma region Noise_7
    if (1.25f <= m_fAccNoiseTime7)
    {
        if (1.f <= m_fA7)
        {
            m_fA7 = 1.f;
        }
        else
        {
            m_fA7 += fTimeDelta * 4.5f;
        }
    }
#pragma endregion
#pragma region Noise_8
    if (1.3f <= m_fAccNoiseTime8)
    {
        if (1.f <= m_fA8)
        {
            m_fA8 = 1.f;
        }
        else
        {
            m_fA8 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_9
    if (1.35f <= m_fAccNoiseTime9)
    {
        if (1.f <= m_fA9)
        {
            m_fA9 = 1.f;
        }
        else
        {
            m_fA9 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_10
    if (1.5f <= m_fAccNoiseTime10)
    {
        if (1.f <= m_fA10)
        {
            m_fA10 = 1.f;
        }
        else
        {
            m_fA10 += fTimeDelta * 5.5f;
        }
    }
    if (2.f <= m_fAccNoiseTime10)
    {
        m_bBackground2_Blink = true;
    }
#pragma endregion
#pragma region Noise_11
    if (1.5f <= m_fAccNoiseTime11)
    {
        if (1.f <= m_fA11)
        {
            m_fA11 = 1.f;
        }
        else
        {
            m_fA11 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_12
    if (1.6f <= m_fAccNoiseTime12)
    {
        if (1.f <= m_fA12)
        {
            m_fA12 = 1.f;
        }
        else
        {
            m_fA12 += fTimeDelta * 2.5f;
        }
    }
#pragma endregion
#pragma region Noise_13
    if (1.7f <= m_fAccNoiseTime13)
    {
        if (1.f <= m_fA13)
        {
            m_fA13 = 1.f;
        }
        else
        {
            m_fA13 += fTimeDelta * 6.5f;
        }
    }
#pragma endregion
#pragma region Noise_14
    if (1.7f <= m_fAccNoiseTime14)
    {
        if (1.f <= m_fA14)
        {
            m_fA14 = 1.f;
        }
        else
        {
            m_fA14 += fTimeDelta * 9.5f;
        }
    }
#pragma endregion
#pragma region Noise_15
    if (1.7f <= m_fAccNoiseTime15)
    {
        if (1.f <= m_fA15)
        {
            m_fA15 = 1.f;
        }
        else
        {
            m_fA15 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_16
    if (1.75f <= m_fAccNoiseTime16)
    {
        if (1.f <= m_fA16)
        {
            m_fA16 = 1.f;
        }
        else
        {
            m_fA16 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_17
    if (1.9f <= m_fAccNoiseTime17)
    {
        if (1.f <= m_fA17)
        {
            m_fA17 = 1.f;
        }
        else
        {
            m_fA17 += fTimeDelta * 6.5f;
        }
    }
#pragma endregion
#pragma region Noise_18
    if (1.9f <= m_fAccNoiseTime18)
    {
        if (1.f <= m_fA18)
        {
            m_fA18 = 1.f;
        }
        else
        {
            m_fA18 += fTimeDelta * 6.5f;
        }
    }
#pragma endregion
#pragma region Noise_19
    if (2.f <= m_fAccNoiseTime19)
    {
        if (1.f <= m_fA19)
        {
            m_fA19 = 1.f;
        }
        else
        {
            m_fA19 += fTimeDelta * 4.5f;
        }
    }
#pragma endregion
#pragma region Noise_20
    if (2.5f <= m_fAccNoiseTime20)
    {
        if (1.f <= m_fA20)
        {
            m_fA20 = 1.f;
        }
        else
        {
            m_fA20 += fTimeDelta * 6.5f;
        }
    }
#pragma endregion
#pragma region Noise_21
    if (4.1f <= m_fAccNoiseTime21)
    {
        if (1.f <= m_fA21)
        {
            m_fA21 = 1.f;
        }
        else
        {
            m_fA21 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_22
    if (5.1f <= m_fAccNoiseTime22)
    {
        if (1.f <= m_fA22)
        {
            m_fA22 = 1.f;
        }
        else
        {
            m_fA22 += fTimeDelta * 2.5f;
        }
    }
#pragma endregion
#pragma region Noise_23
    if (5.15f <= m_fAccNoiseTime23)
    {
        if (1.f <= m_fA23)
        {
            m_fA23 = 1.f;
        }
        else
        {
            m_fA23 += fTimeDelta * 6.5f;
        }
    }
#pragma endregion
#pragma region Noise_24
    if (6.2f <= m_fAccNoiseTime24)
    {
        if (1.f <= m_fA24)
        {
            m_fA24 = 1.f;
        }
        else
        {
            m_fA24 += fTimeDelta * 5.5f;
        }
    }
#pragma endregion
#pragma region Noise_25
    if (6.5f <= m_fAccNoiseTime25)
    {
        if (1.f <= m_fA25)
        {
            m_fA25 = 1.f;
        }
        else
        {
            m_fA25 += fTimeDelta * 2.5f;
        }
    }
#pragma endregion

}

void CUI_Indicator_Name_Boss::Control_Background1(_float fTimeDelta)
{
    /* Alpha */
    if (1.5f <= m_fBackground1_Control_Alpha)
    {
        m_fBackground1_Control_Alpha = 1.5f;
    }
    else
    {
        m_fBackground1_Control_Alpha += fTimeDelta;
    }
     
    /* Discard */
    if (0.1f >= m_fBackground1_Control_Discard)
    {
        m_fBackground1_Control_Discard = 0.1f;
    }
    else
    {
        m_fBackground1_Control_Discard -= fTimeDelta/* * 0.3f*/;
    }

    if (0.07f <= m_fBackground1_Control_Discard && 0.7f <= m_fBackground1_Control_Alpha)
    {
        m_bBackground2_Start = true;
    }
}

void CUI_Indicator_Name_Boss::Control_Background2(_float fTimeDelta)
{
    if (true == m_bBackground2_Start && false == m_bBackground2_Blink)
    {
        /* Alpha */
        if (1.5f <= m_fBackground2_Control_Alpha1)
        {
            m_fBackground2_Control_Alpha1 = 1.5f;
        }
        else
        {
            m_fBackground2_Control_Alpha1 += fTimeDelta;
        }
        /* Discard */
        if (1.f >= m_fBackground2_Control_Discard)
        {
            m_fBackground2_Control_Discard = 1.f;
        }
        else
        {
            m_fBackground2_Control_Discard -= fTimeDelta * 0.3f;
        }
    }
    if (true == m_bBackground2_Start && true == m_bBackground2_Blink)
    {
        /* Alpha */
        if (0.1f <= m_fBackground2_Control_Alpha2)
        {
            m_bBackground3_Start = true;
        }

        if (1.f <= m_fBackground2_Control_Alpha2)
        {
            m_fBackground2_Control_Alpha2 = 1.f;
        }
        else
        {
            m_fBackground2_Control_Alpha2 += fTimeDelta;
        }
    }
}

void CUI_Indicator_Name_Boss::Control_Background3(_float fTimeDelta)
{
    if (true == m_bBackground3_Start && false == m_bBackground3_Switch)
    {
        /* Alpha */
        if (1.5f <= m_fBackground3_Control_Alpha)
        {
            m_fBackground3_Control_Alpha = 1.5f;
            m_bBackground3_Switch = true;
        }
        else
        {
            m_fBackground3_Control_Alpha += fTimeDelta * 0.9f;
        }
    }
    if (true == m_bBackground3_Start && true == m_bBackground3_Switch)
    {
        /* Alpha */
        if (0.f >= m_fBackground3_Control_Alpha)
        {
            m_fBackground3_Control_Alpha = 0.f;
            m_iAnimCount = 1;
            m_bAnimation_End = true;
            Initialize_Variables();
        }
        else
        {
            m_fBackground3_Control_Alpha -= fTimeDelta * 0.25f;
        }
    }
}

void CUI_Indicator_Name_Boss::Control_RenderFont(_float fTimeDelta)
{
    m_fTime_RenderFont += fTimeDelta;

    Swap_Text();

    if (1.f <= m_fTime_RenderFont)
    {
        m_bRenderFont = true;

        if (0.75f <= m_fTime_RenderFontAlpha_E_OutLine)
        {
            m_fTime_RenderFontAlpha_E_OutLine = 0.75f;
        }
        else
        {
            m_fTime_RenderFontAlpha_E_OutLine += fTimeDelta * 0.1f;
        }
    }

}

void CUI_Indicator_Name_Boss::Render_RenderFont()
{
    CGameInstance::Get_Instance()->Render_Font(TEXT("Font_E_OutLine"), m_strBossName, _float2(m_fPosition_FontX, g_iWinCY * 0.5f - 170.f), XMVectorSet(255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f), 1.2f);
}

void CUI_Indicator_Name_Boss::Swap_Text()
{
    if (1.f <= m_fTime_RenderFont && 1.05f > m_fTime_RenderFont)
    {
        m_strBossName = L"Ya&ph+@]hh;+p-{%T";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (1.05f <= m_fTime_RenderFont && 1.1f > m_fTime_RenderFont)
    {
        m_strBossName = L"3a{Ae{|#t&p^atte&";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;
    }
    if (1.1f <= m_fTime_RenderFont && 1.15f > m_fTime_RenderFont)
    {
        m_strBossName = L"Tr A/{e+eO?p%aAr^";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (1.15f <= m_fTime_RenderFont && 1.2f > m_fTime_RenderFont)
    {
        m_strBossName = L"*htrOdedrp}}p=ae=p";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.2f <= m_fTime_RenderFont && 1.25f > m_fTime_RenderFont)
    {
        m_strBossName = L"{{4a|&h]]p]+ppa{-%";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.25f <= m_fTime_RenderFont && 1.3f > m_fTime_RenderFont)
    {
        m_strBossName = L"T4e} ehr+/^pae&";
        m_fPosition_FontX = g_iWinCX * 0.5f - 220.f;

    }
    if (1.35f <= m_fTime_RenderFont && 1.4f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th#e@-hAh p@p}aOhT";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.4f <= m_fTime_RenderFont && 1.45f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th@}[th=A Appea[{%";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.845f <= m_fTime_RenderFont && 1.5f > m_fTime_RenderFont)
    {
        m_strBossName = L"T&%{Oth|&Appea;&";
        m_fPosition_FontX = g_iWinCX * 0.5f - 230.f;

    }
    if (1.55f <= m_fTime_RenderFont && 1.6f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th@ Oth}* Appear]O";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.65f <= m_fTime_RenderFont && 1.7f > m_fTime_RenderFont)
    {
        m_strBossName = L"T d Oth@e Appear^t";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.75f <= m_fTime_RenderFont && 1.8f > m_fTime_RenderFont)
    {
        m_strBossName = L"T$h Oth[r Appear:d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (1.8f <= m_fTime_RenderFont && 1.85f > m_fTime_RenderFont)
    {
        m_strBossName = L"&he Otheehr AppeTa=-d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 280.f;

    }
    if (1.9f <= m_fTime_RenderFont && 1.95f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th%h Othe%r Ap{ear|d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (1.95f <= m_fTime_RenderFont && 2.f > m_fTime_RenderFont)
    {
        m_strBossName = L"Thae Other A[peptred";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (2.f <= m_fTime_RenderFont && 2.05f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th#{Ot{{r Ap&;a_+red";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (2.05f <= m_fTime_RenderFont && 2.1f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th* Oth:r Appear]d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.1f <= m_fTime_RenderFont && 2.15f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th[ Othe- Ap@{ered";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.15f <= m_fTime_RenderFont && 2.2f > m_fTime_RenderFont)
    {
        m_strBossName = L"%ha Ot^er Ap@p{red";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.2f <= m_fTime_RenderFont && 2.25f > m_fTime_RenderFont)
    {
        m_strBossName = L"T&e Otherr App@ar@d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (2.25f <= m_fTime_RenderFont && 2.3f > m_fTime_RenderFont)
    {
        m_strBossName = L"The @Ther A{ea=}d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 240.f;

    }
    if (2.3f <= m_fTime_RenderFont && 2.35f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Otthr|Ape|ar@d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.35f <= m_fTime_RenderFont && 2.4f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Othe%r Appe}*3aed";
        m_fPosition_FontX = g_iWinCX * 0.5f - 280.f;

    }
    if (2.4f <= m_fTime_RenderFont && 2.45f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th0 Otherr Appeare=d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (2.45f <= m_fTime_RenderFont && 2.5f > m_fTime_RenderFont)
    {
        m_strBossName = L"T4he @ther App{ared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (2.55f <= m_fTime_RenderFont && 2.6f > m_fTime_RenderFont)
    {
        m_strBossName = L"T4e @4her Appea{ed";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.6f <= m_fTime_RenderFont && 2.65f > m_fTime_RenderFont)
    {
        m_strBossName = L"The O4her App^^red";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.65f <= m_fTime_RenderFont && 2.7f > m_fTime_RenderFont)
    {
        m_strBossName = L"The|^ther_App@&red";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (2.7f <= m_fTime_RenderFont && 2.75f > m_fTime_RenderFont)
    {
        m_strBossName = L"The^Other Appe_ed";
        m_fPosition_FontX = g_iWinCX * 0.5f - 240.f;

    }
    if (2.75f <= m_fTime_RenderFont && 2.8f > m_fTime_RenderFont)
    {
        m_strBossName = L"Thae Oth(r Ap#p-ear%d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 280.f;

    }
    if (2.8f <= m_fTime_RenderFont && 2.85f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Ote&r App#eare%";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (2.85f <= m_fTime_RenderFont && 2.9f > m_fTime_RenderFont)
    {
        m_strBossName = L"Th2 Othe^r_Appe{}ed";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (2.9f <= m_fTime_RenderFont && 2.95f > m_fTime_RenderFont)
    {
        m_strBossName = L"The other #ppeare</d";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (2.95f <= m_fTime_RenderFont && 3.f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Ap@eare?*";
        m_fPosition_FontX = g_iWinCX * 0.5f - 260.f;

    }
    if (3.f <= m_fTime_RenderFont && 3.05f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other 4ppear@edT";
        m_fPosition_FontX = g_iWinCX * 0.5f - 270.f;

    }
    if (3.05f <= m_fTime_RenderFont && 3.1f > m_fTime_RenderFont)
    {
        m_strBossName = L"TThe Other Appea[[";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (3.1f <= m_fTime_RenderFont && 3.15f > m_fTime_RenderFont)
    {
        m_strBossName = L"The OTher Apeared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 240.f;

    }
    if (3.15f <= m_fTime_RenderFont && 3.2f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Appeared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (3.2f <= m_fTime_RenderFont && 3.25f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Appeared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (3.25f <= m_fTime_RenderFont && 3.3f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Appeared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }
    if (3.3f <= m_fTime_RenderFont && 3.35f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Appeare";
        m_fPosition_FontX = g_iWinCX * 0.5f - 240.f;

    }
    if (3.35f <= m_fTime_RenderFont && 3.4f > m_fTime_RenderFont)
    {
        m_strBossName = L"The Other Appeared";
        m_fPosition_FontX = g_iWinCX * 0.5f - 250.f;

    }

}

shared_ptr<CUI_Indicator_Name_Boss> CUI_Indicator_Name_Boss::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Name_Boss> pInstance = make_private_shared(CUI_Indicator_Name_Boss, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Name_Boss::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Name_Boss::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Name_Boss> pInstance = make_private_shared_copy(CUI_Indicator_Name_Boss, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Name_Boss::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
