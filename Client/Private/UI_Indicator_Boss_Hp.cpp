#include "ClientPCH.h"
#include "UI_Indicator_Boss_Hp.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Monster_DispenPerry.h"

CUI_Indicator_Boss_Hp::CUI_Indicator_Boss_Hp(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Boss_Hp::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Boss_Hp::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    

    Initialize_UI();


    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_HP::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Boss_Hp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Boss_Hp::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Boss_Hp::Render()
{
    if (true == m_bRenderStart && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene())
    {
        if (FAILED(m_pTexture_HP_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pHP_Background->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShaderTime_Background, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(49)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CGameObject::Render");
        }

        if (true == m_bRender_BackEnd)
        {
            if (FAILED(m_pTexture_HP_Back->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pHP_Back->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fRatio_Back, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(50)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CGameObject::Render");
            }
        }

        if (true == m_bRender_FrontEnd)
        {
            if (FAILED(m_pTexture_HP_Front->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pHP_Front->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(42)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Render", "Failed to CGameObject::Render");
            }
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_strBossName, _float2(g_iWinCX * 0.5f - 290.f, g_iWinCY * 0.5f - 500.f), XMVectorSet(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f), 0.4f);
    }

    return S_OK;
}

HRESULT CUI_Indicator_Boss_Hp::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Boss_Hp::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Boss_Hp::Initialize_UI()
{
//===================================================================== Texture
    m_pTexture_HP_Background        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_Hp_Background.png"));
    m_pTexture_HP_Back              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_HP_Back1.png"));
    m_pTexture_HP_Front             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Boss_HP_Front1.png"));    
//===================================================================== Transform
    m_pHP_Background                = CTransform::Create(m_pDevice, m_pContext);
    m_pHP_Back                      = CTransform::Create(m_pDevice, m_pContext);
    m_pHP_Front                     = CTransform::Create(m_pDevice, m_pContext);
//===================================================================== _bool
    m_bRenderStart                  = false;
    m_bRender_BackgroundEnd         = false;
    m_bRender_BackEnd               = false;
    m_bRender_FrontEnd              = false;
    m_bIsFinishCalculateDamage      = true;
    m_bHpBackDecreaseStart          = false;
//===================================================================== _float
    m_fAccTime_Background           = 0.f;
    m_fAccTime_Back                 = 0.f;
    m_fAccTime_Front                = 0.f;
    m_fShaderTime_Background        = 0.f;
    m_fShaderTime_Back              = 0.f;
    m_fShaderTime_Front             = 0.f;
    m_fDamageDelayTime              = 0.f;
    m_fTime_RenderFontAlpha_OutLine = 0.f;

    m_fHp                           = 50.f;
    m_fHpMax                        = 50.f;
    m_fRatio                        = m_fHp / m_fHpMax;
    m_fPercentage                   = m_fRatio * 538.f;
    
    m_fHpBack                       = 50.f;
    m_fHpBackMax                    = 50.f;
    m_fRatio_Back                   = m_fHpBack / m_fHpBackMax;
    m_fPercentage_Back              = m_fRatio_Back * 538.f;

//===================================================================== _float3
    m_vScale_Hp_Background          = _float3(584.f, 50.f, 1.f);
    m_vScale_Hp_Back                = _float3(m_fPercentage_Back, 15.f, 1.f);
    m_vScale_Hp_Front               = _float3(m_fPercentage, 15.f, 1.f);

//===================================================================== _float4
    m_vPosition_Hp_Background       = _float4(0.f, 450.f, 0.f, 1.f);
    m_vPosition_Hp_Back             = _float4(5.f, 455.f, 0.f, 1.f);
    m_vPosition_Hp_Front            = _float4(5.f, 455.f, 0.f, 1.f);

//===================================================================== Set_Scale
    m_pHP_Background->Set_Scale(m_vScale_Hp_Background);
    m_pHP_Back->Set_Scale(m_vScale_Hp_Back);
    m_pHP_Front->Set_Scale(m_vScale_Hp_Front);

//===================================================================== Set_Position
    m_pHP_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Hp_Background);
    m_pHP_Back->Set_State(TRANSFORM::POSITION, m_vPosition_Hp_Back);
    m_pHP_Front->Set_State(TRANSFORM::POSITION, m_vPosition_Hp_Front);

//===================================================================== wstring
    m_strBossName                   = L"";
}

void CUI_Indicator_Boss_Hp::Tick_UI(_float fTimeDelta)
{
    //Render_Start();
    Render_Hp(fTimeDelta);
    Set_Position_DamagedBossHP(fTimeDelta);
    Calculate_Damage_System(fTimeDelta);
}

void CUI_Indicator_Boss_Hp::Set_BossName(wstring strBossName)
{
    m_strBossName = strBossName;
}

void CUI_Indicator_Boss_Hp::Render_Start()
{
    //if (CGameInstance::Get_Instance()->Key_Down('K'/*VK_F3*/))
    //{
    //    m_bRenderStart = true;
    //}
    m_bRenderStart = true;

}

void CUI_Indicator_Boss_Hp::Render_Hp(_float fTimeDelta)
{
    Render_Shader_Background(fTimeDelta);
    Render_Shader_Back(fTimeDelta);
    Render_Shader_Front(fTimeDelta);
}

void CUI_Indicator_Boss_Hp::Render_Shader_Background(_float fTimeDelta)
{
    if (true == m_bRenderStart && false == m_bRender_BackgroundEnd)
    {
        m_fAccTime_Background += fTimeDelta;

        if (2.6f <= m_fAccTime_Background)
        {
            if (1.f <= m_fShaderTime_Background)
            {
                m_fShaderTime_Background = 1.f;
                m_fTime_RenderFontAlpha_OutLine = 1.f;
                m_bRender_BackgroundEnd = true;
            }
            else
            {
                m_fShaderTime_Background += fTimeDelta * 2.f;
                m_fTime_RenderFontAlpha_OutLine += fTimeDelta * 2.f;
            }
        }
    }
}

void CUI_Indicator_Boss_Hp::Render_Shader_Back(_float fTimeDelta)
{
    if (true == m_bRenderStart)
    {
        m_fAccTime_Back += fTimeDelta;
        
        if (2.5f <= m_fAccTime_Back && 2.55f > m_fAccTime_Back)
        {
            m_bRender_BackEnd = true;
        }
        if (2.55f <= m_fAccTime_Back && 2.6f > m_fAccTime_Back)
        {
            m_bRender_BackEnd = false;
        }
        if (2.6f <= m_fAccTime_Back && 2.65f > m_fAccTime_Back)
        {
            m_bRender_BackEnd = true;
        }
        if (2.65f <= m_fAccTime_Back && 2.7f > m_fAccTime_Back)
        {
            m_bRender_BackEnd = false;
        }
        if (2.7f <= m_fAccTime_Back)
        {
            m_bRender_BackEnd = true;
        }
    }
}

void CUI_Indicator_Boss_Hp::Render_Shader_Front(_float fTimeDelta)
{
    if (true == m_bRenderStart)
    {
        m_fAccTime_Front += fTimeDelta;

        if (2.53f <= m_fAccTime_Front && 2.58f > m_fAccTime_Front)
        {
            m_bRender_FrontEnd = true;
        }
        if (2.58f <= m_fAccTime_Front && 2.63f > m_fAccTime_Front)
        {
            m_bRender_FrontEnd = false;
        }
        if (2.63f <= m_fAccTime_Front && 2.68f > m_fAccTime_Front)
        {
            m_bRender_FrontEnd = true;
        }
        if (2.68f <= m_fAccTime_Front && 2.73f > m_fAccTime_Front)
        {
            m_bRender_FrontEnd = false;
        }
        if (2.73f <= m_fAccTime_Front)
        {
            m_bRender_FrontEnd = true;
        }
    }
}

void CUI_Indicator_Boss_Hp::Set_Position_DamagedBossHP(_float fTimeDelta)
{  
    if (0 < m_fHp) // Hp Upper 0 ---> Damaging System Operate
    {
        /* Situation :: When Player Damaged */
        if (m_fRatioBackHp > m_fRatio)
        {
            m_fRatio = m_fHp / m_fHpMax;

            m_vPosition_Hp_Front.x = 5.f - 538.f * (1.f - m_fRatio) * 0.5f; // Set CurrentHP_Position Directly
            m_pHP_Front->Set_State(TRANSFORM::POSITION, m_vPosition_Hp_Front);

            m_bIsFinishCalculateDamage = false;
        }        
    }
    else
    {
        m_bRenderStart = false; // Hp Under 0 ---> Render Off
    }
}

void CUI_Indicator_Boss_Hp::Calculate_Damage_System(_float fTimeDelta)
{
    //if (false == m_bIsFinishCalculateDamage)
    //{
    //    m_fDamageDelayTime += fTimeDelta;

    //    if (1.f <= m_fDamageDelayTime)
    //    {
    //        m_bHpBackDecreaseStart = true;
    //    }

    //    if (true == m_bHpBackDecreaseStart)
    //    {
    //        if (m_fBackHp <= m_fHp)
    //        {
    //            m_bHpBackDecreaseStart = false;
    //            m_bIsFinishCalculateDamage = true;
    //        }
    //        else
    //        {
    //            m_fBackHp -= fTimeDelta * 0.05f;

    //            m_vScale_Hp_Back.x = m_fBackHp;
    //            m_pHP_Back->Set_Scale(m_vScale_Hp_Back);
    //        }
    //    }
    //}
    //else
    //{
    //    m_fDamageDelayTime = 0.f;
    //}
}

void CUI_Indicator_Boss_Hp::Set_BossHp_Initialize(_float fHp, _float fHpMax)
{
    m_fHp                   = fHp;
    m_fHpMax                = fHpMax;
    m_fHpBack               = fHp;
    m_fHpBackMax            = fHpMax;


    m_fBackHp               = m_fHpBack;
    m_fBackHpMax            = m_fHpBackMax;
    m_fRatioBackHp          = m_fBackHp / m_fBackHpMax;
    m_fPercentage_BackHp    = m_fRatioBackHp * 538.f;


    m_fRatio                = m_fHp / m_fHpMax;
    m_fRatio_Back           = m_fHpBack / m_fHpBackMax;

    m_fPercentage           = m_fRatio * 538.f;
    m_fPercentage_Back      = m_fRatio_Back * 538.f;

    m_vScale_Hp_Front       = _float3(m_fPercentage, 15.f, 1.f);
    m_vScale_Hp_Back        = _float3(m_fPercentage_BackHp, 15.f, 1.f);

    m_pHP_Front->Set_Scale(m_vScale_Hp_Front);
    m_pHP_Back->Set_Scale(m_vScale_Hp_Back);
}

void CUI_Indicator_Boss_Hp::Set_BossHp(_float fHp, _float fHpMax)
{
    m_fHp                   = fHp;
    m_fHpMax                = fHpMax;
    m_fRatio                = m_fHp / m_fHpMax;
    m_fPercentage           = m_fRatio * 538.f;
    m_vScale_Hp_Front       = _float3(m_fPercentage, 15.f, 1.f);

    m_pHP_Front->Set_Scale(m_vScale_Hp_Front);
}

void CUI_Indicator_Boss_Hp::Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix)
{
}

shared_ptr<CUI_Indicator_Boss_Hp> CUI_Indicator_Boss_Hp::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Boss_Hp> pInstance = make_private_shared(CUI_Indicator_Boss_Hp, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Boss_Hp::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Boss_Hp::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Boss_Hp> pInstance = make_private_shared_copy(CUI_Indicator_Boss_Hp, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Boss_Hp::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
