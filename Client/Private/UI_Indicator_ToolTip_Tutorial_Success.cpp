#include "ClientPCH.h"
#include "UI_Indicator_ToolTip_Tutorial_Success.h"
#include "GameInstance.h" 
#include "UI_Manager.h"
#include "Player.h"
#include "PlayerState.h"

CUI_Indicator_ToolTip_Tutorial_Success::CUI_Indicator_ToolTip_Tutorial_Success(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Success::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Success::Initialize(any _aDesc)
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

void CUI_Indicator_ToolTip_Tutorial_Success::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Success::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Success::Render()
{
    if (true == m_bPMCC || true == m_bLockOn || true == m_bKasane_BattleStyle || true == m_bKasane_SpecialAttack)
    {
        /* Background_Frame */
        if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fBackgroundAlpha, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fBackgroundColor, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
        }
        if (FAILED(__super::Render(55)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
        }

        if (true == m_bScalingDone)
        {
            /* Background_Frame */
            if (FAILED(m_pTexture_Success->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSuccess->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fSuccessColor, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(56)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_Tips::Render", "Failed to CGameObject::Render");
            }
        }
    }
    return S_OK;
}

HRESULT CUI_Indicator_ToolTip_Tutorial_Success::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Tooltip_Tutorial_PMCC::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_ToolTip_Tutorial_Success::Initialize_UI()
{
//=========================================================================== _bool
    m_bPMCC                         = false;
    m_bLockOn                       = false;
    m_bKasane_BattleStyle           = false;
    m_bKasane_SpecialAttack         = false;

    m_bScalingX                     = false;
    m_bScalingY                     = false;
    m_bScalingDone                  = false;
    m_bRewind                       = false;
    m_bRewindScalingXY              = false;

//=========================================================================== _float
    m_fAccTime                      = 0.f;
    m_fBackgroundAlpha              = 1.f;
    m_fBackgroundColor              = 1.f; // Initialize White
    m_fSuccessColor                 = 0.f; // Initialize Black
    m_fBlinkDoneTime                = 0.f;

//=========================================================================== Transform
    m_pBackground                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSuccess                      = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== Texture
    m_pTexture_Background           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Background_Success.png"));
    m_pTexture_Success              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Information/Success.png"));

//=========================================================================== _float3
    m_vScale_Background             = _float3(600.f, 110.f, 1.f);
    m_vScale_Success                = _float3(114.f, 23.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background          = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_Success             = _float4(0.f, 200.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pSuccess->Set_Scale(m_vScale_Success);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pSuccess->Set_State(TRANSFORM::POSITION, m_vPosition_Success);
}

void CUI_Indicator_ToolTip_Tutorial_Success::Tick_UI(_float fTimeDelta)
{
    //Set_Success_PMCC();
    //Set_Success_LockOn();
    //Set_Success_Kasane_BattleStyle();
    //Set_Success_Kasane_SpecialAttack();
    Blink_System(fTimeDelta);
    Sound(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Success::Blink_System(_float fTimeDelta)
{
    Blink_Background(fTimeDelta);
}

void CUI_Indicator_ToolTip_Tutorial_Success::Blink_Background(_float fTimeDelta)
{
    if (true == m_bPMCC || true == m_bLockOn || true == m_bKasane_BattleStyle || true == m_bKasane_SpecialAttack)
    {
#pragma region Scale
        // X
        if (false == m_bScalingX && false == m_bScalingY && false == m_bRewind)
        {
            if (1.f <= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 1.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta;
            }

            if (250.f >= m_vScale_Background.x)
            {
                m_vScale_Background.x = 250.f;
                m_pBackground->Set_Scale(m_vScale_Background);

                // ScalingX Done ---> true
                m_bScalingX = true;
            }
            else
            {
                m_vScale_Background.x -= fTimeDelta * 12000.f; // Scaling Speed
                m_pBackground->Set_Scale(m_vScale_Background);
            }
        }

        // Y
        if (true == m_bScalingX && false == m_bScalingY && false == m_bRewind)
        {
            if (1.f <= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 1.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta;
            }

            if (160.f <= m_vScale_Background.y)
            {
                m_vScale_Background.y = 160.f;
                m_pBackground->Set_Scale(m_vScale_Background);

                // ScalingY Done ---> true
                m_bScalingY = true;
            }
            else
            {
                m_vScale_Background.y += fTimeDelta * 3000.f; // Scaling Speed
                m_pBackground->Set_Scale(m_vScale_Background);
            }
        }

        // X & Y
        if (true == m_bScalingX && true == m_bScalingY && false == m_bScalingDone && false == m_bRewind)
        {
            if (1.f <= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 1.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta;
            }

            if (300.f <= m_vScale_Background.x && 110.f >= m_vScale_Background.y)
            {
                m_vScale_Background.x = 300.f;
                m_vScale_Background.y = 110.f;
                m_pBackground->Set_Scale(m_vScale_Background);

                // All Scaling Done ---> Shader (false ---> true)
                m_bScalingDone = true;
            }
            else
            {
                m_vScale_Background.x += fTimeDelta * 3000.f;
                m_vScale_Background.y -= fTimeDelta * 3000.f;
                m_pBackground->Set_Scale(m_vScale_Background);
            }
        }
#pragma endregion
#pragma region Shader
        if (true == m_bScalingDone && false == m_bRewind)
        {
            if (1.f <= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 1.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta;
            }

            if (8.f >= m_fAccTime)
            {
                m_fAccTime += fTimeDelta * 20.f;

                _uint iTimeIndex = (_uint)m_fAccTime % 2;

                if (0 == iTimeIndex)
                {
                    m_fBackgroundColor = 1.f;
                    m_fSuccessColor = 0.f;
                }
                else if (1 == iTimeIndex)
                {
                    m_fBackgroundColor = 0.f;
                    m_fSuccessColor = 1.f;
                }
            }
            else
            {
                if (2.f <= m_fBlinkDoneTime)
                {
                    m_bScalingX         = false;
                    m_bScalingY         = false;
                    m_bScalingDone      = false;
                    m_bRewind           = true; // Rewind - Trigger
                    m_fAccTime          = 0.f;
                    m_fBackgroundColor  = 1.f;
                    m_fSuccessColor     = 0.f;
                    m_fBlinkDoneTime    = 0.f;
                }
                else
                {
                    m_fBlinkDoneTime += fTimeDelta;
                }
            }
        }
#pragma endregion
#pragma region Rewind
        if (true == m_bRewind && false == m_bRewindScalingXY)
        {
            if (0.f >= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 0.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta * 2.f;
            }

            if (150.f >= m_vScale_Background.x && 250.f <= m_vScale_Background.y)
            {
                m_vScale_Background.x = 150.f;
                m_vScale_Background.y = 250.f;
                m_pBackground->Set_Scale(m_vScale_Background);

                m_bRewindScalingXY = true;
            }
            else
            {
                m_vScale_Background.x -= fTimeDelta * 6000.f;
                m_vScale_Background.y += fTimeDelta * 6000.f;
                m_pBackground->Set_Scale(m_vScale_Background);
            }

        }
        if (true == m_bRewindScalingXY)
        {
            if (0.f >= m_fBackgroundAlpha)
            {
                m_fBackgroundAlpha = 0.f;
            }
            else
            {
                m_fBackgroundAlpha += fTimeDelta * 2.f;
            }

            if (0.f >= m_vScale_Background.y)
            {
                Reset_UI();
            }
            else
            {
                m_vScale_Background.x += fTimeDelta * 4500.f;
                m_vScale_Background.y -= fTimeDelta * 3000.f;
                m_pBackground->Set_Scale(m_vScale_Background);
            }
        }
#pragma endregion
    }
}

void CUI_Indicator_ToolTip_Tutorial_Success::Reset_UI()
{
//=========================================================================== _uint
    m_iSoundIndex                   = 0;

//=========================================================================== _bool
    m_bPMCC                         = false;
    m_bLockOn                       = false;
    m_bKasane_BattleStyle           = false;
    m_bKasane_SpecialAttack         = false;

    m_bScalingX                     = false;
    m_bScalingY                     = false;
    m_bScalingDone                  = false;
    m_bRewind                       = false;
    m_bRewindScalingXY              = false;

//=========================================================================== _float
    m_fAccTime                      = 0.f;
    m_fBackgroundAlpha              = 1.f;
    m_fBackgroundColor              = 1.f; // Initialize White
    m_fSuccessColor                 = 0.f; // Initialize Black
    m_fBlinkDoneTime                = 0.f;

//=========================================================================== _float3
    m_vScale_Background             = _float3(600.f, 110.f, 1.f);
    m_vScale_Success                = _float3(114.f, 23.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Background          = _float4(0.f, 200.f, 0.f, 1.f);
    m_vPosition_Success             = _float4(0.f, 200.f, 0.f, 1.f);

//=========================================================================== Set_Scale
    _vector Position = m_pBackground->Get_State(TRANSFORM::POSITION);
    _matrix Identity = XMMatrixTranslation(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position));
    m_pBackground->Set_Matrix(Identity);
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pSuccess->Set_Scale(m_vScale_Success);

//=========================================================================== Set_Position
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pSuccess->Set_State(TRANSFORM::POSITION, m_vPosition_Success);
}

void CUI_Indicator_ToolTip_Tutorial_Success::Sound(_float fTimeDelta)
{
    if (true == m_bPMCC || true == m_bLockOn || true == m_bKasane_BattleStyle || true == m_bKasane_SpecialAttack)
    {
        if (0 == m_iSoundIndex)
        {
            CGameInstance::Get_Instance()->PlaySoundW(L"UI_SFX_Success.wav", 1.f);
            ++m_iSoundIndex;
        }
    }
    else
    {
        m_iSoundIndex = 0;
    }
}

void CUI_Indicator_ToolTip_Tutorial_Success::Set_Success_PMCC()
{
    m_bPMCC = true;
}

void CUI_Indicator_ToolTip_Tutorial_Success::Set_Success_LockOn()
{
    m_bLockOn = true;
}

void CUI_Indicator_ToolTip_Tutorial_Success::Set_Success_Kasane_BattleStyle()
{
    m_bKasane_BattleStyle = true;
}

void CUI_Indicator_ToolTip_Tutorial_Success::Set_Success_Kasane_SpecialAttack()
{
    m_bKasane_SpecialAttack = true;
}

shared_ptr<CUI_Indicator_ToolTip_Tutorial_Success> CUI_Indicator_ToolTip_Tutorial_Success::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Success> pInstance = make_private_shared(CUI_Indicator_ToolTip_Tutorial_Success, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Success::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_ToolTip_Tutorial_Success::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_ToolTip_Tutorial_Success> pInstance = make_private_shared_copy(CUI_Indicator_ToolTip_Tutorial_Success, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_ToolTip_Tutorial_Success::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
