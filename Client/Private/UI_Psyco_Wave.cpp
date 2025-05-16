#include "ClientPCH.h"
#include "UI_Psyco_Wave.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"

CUI_Psyco_Wave::CUI_Psyco_Wave(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Psyco_Wave::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Psyco_Wave::Initialize(any _pPlayerEntityDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_pPlayerEntityDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }
    

    Initialize_UI();


    m_pVIBuffer_Rect    = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Psyco_Wave::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_tUIDesc_Wave.m_fTime      += fTimeDelta * 0.3f;
    //m_tUIDesc_Disonance.m_fTime += fTimeDelta * 0.3f;

}

void CUI_Psyco_Wave::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_1);
    
    Tick_UI(fTimeDelta);
}

HRESULT CUI_Psyco_Wave::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && false == CUI_Manager::Get_Instance()->Get_TutorialRender()
        && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
        && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
    {
        if (false == m_bIsBattle)
        {
            // PsycoWave_Background
            if (FAILED(m_pTexture_Wave->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pWave_Background->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bOnOff", &m_bIsBattle, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bChecker1", &m_bIsWave_Background, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fVariable", &m_fPsycoGaugeRatio_Background, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_Wave.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(6)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CGameObject::Render");
            }

            // PsycoWave
            if (FAILED(m_pTexture_Wave->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pWave->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bOnOff", &m_bIsBattle, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bChecker1", &m_bIsWave, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fVariable", &m_fPsycoGaugeRatio_Wave, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_Wave.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(6)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CGameObject::Render");
            }
        }
        else
        {
            // PsycoDisonance_Background
            if (FAILED(m_pTexture_Disonance->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pDisonance->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bOnOff", &m_bIsBattle, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bChecker2", &m_bIsDisonance_Background, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_Disonance.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc_Disonance.m_iIndex, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc_Disonance.m_iIndexMaxX, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc_Disonance.m_iIndexMaxY, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc_Disonance.m_fSizeX, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc_Disonance.m_fSizeY, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fVariable", &m_fPsycoGaugeRatio_Background, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(6)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CGameObject::Render");
            }

            // PsycoDisonance
            if (FAILED(m_pTexture_Disonance->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pDisonance->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bOnOff", &m_bIsBattle, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_bChecker2", &m_bIsDisonance, sizeof(_bool))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_tUIDesc_Disonance.m_fTime, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndex", &m_tUIDesc_Disonance.m_iIndex, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxX", &m_tUIDesc_Disonance.m_iIndexMaxX, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_iIndexMaxY", &m_tUIDesc_Disonance.m_iIndexMaxY, sizeof(_uint))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeX", &m_tUIDesc_Disonance.m_fSizeX, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fSizeY", &m_tUIDesc_Disonance.m_fSizeY, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fVariable", &m_fPsycoGaugeRatio_Wave, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CTexture::Bind_RawValue");
            }
            if (FAILED(__super::Render(6)))
            {
                MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Render", "Failed to CGameObject::Render");
            }
        }
    }

    return S_OK;
}

void CUI_Psyco_Wave::Initialize_UI()
{
//========================================================================== Transform
    m_pWave                                 = CTransform::Create(m_pDevice, m_pContext);
    m_pWave_Background                      = CTransform::Create(m_pDevice, m_pContext);
    m_pDisonance                            = CTransform::Create(m_pDevice, m_pContext);
    m_pDisonance_Background                 = CTransform::Create(m_pDevice, m_pContext);

//========================================================================== Texture
    m_pTexture_Wave                         = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoWave.png"));
    m_pTexture_Disonance                    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Psyco_Wave/PsycoDisonance.png"));

//========================================================================== Sprite
#pragma region Wave
    ZeroMemory(&m_tUIDesc_Wave, sizeof m_tUIDesc_Wave);

    m_tUIDesc_Wave.m_iIndex                 = 0;
    m_tUIDesc_Wave.m_iIndexMaxX             = 1;
    m_tUIDesc_Wave.m_iIndexMaxY             = 1;
    m_tUIDesc_Wave.m_fFramePerSec           = 1.f;
             
    m_tUIDesc_Wave.m_fTime                  = 0.f;
    m_tUIDesc_Wave.m_fSizeX                 = 339.f;
    m_tUIDesc_Wave.m_fSizeY                 = 27.f;
#pragma endregion

#pragma region Disonance
    ZeroMemory(&m_tUIDesc_Disonance, sizeof m_tUIDesc_Disonance);

    m_tUIDesc_Disonance.m_iIndex            = 0;
    m_tUIDesc_Disonance.m_iIndexMaxX        = 1;
    m_tUIDesc_Disonance.m_iIndexMaxY        = 4;
    m_tUIDesc_Disonance.m_fFramePerSec      = 1.f;

    m_tUIDesc_Disonance.m_fTime             = 0.f;
    m_tUIDesc_Disonance.m_fSizeX            = 339.f;
    m_tUIDesc_Disonance.m_fSizeY            = 108.f;
#pragma endregion

//========================================================================== _bool
    m_bIsBattle                             = false;
    m_bIsWave                               = false;
    m_bIsWave_Background                    = true;
    m_bIsDisonance                          = false;
    m_bIsDisonance_Background               = true;

//========================================================================== _int
    
//========================================================================== _float
    m_fAccTime                              = 0.f;
    
    // 실제 Gauge(상수)
    m_fPsycoGaugeOrigin                     = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fKinesisGauge;
    m_fPsycoGaugeMaxOrigin                  = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fMaxKinesisGauge;
    m_fPsycoGaugeRatioOrigin                = m_fPsycoGaugeOrigin / m_fPsycoGaugeMaxOrigin;
    m_fPsycoGaugePercentageOrigin           = m_fPsycoGaugeRatioOrigin * 280.f;


    // 계산용 변수(Wave)
    m_fPsycoGauge_Wave                      = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fKinesisGauge;
    m_fPsycoGaugeMax_Wave                   = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fMaxKinesisGauge;
    m_fPsycoGaugeRatio_Wave                 = m_fPsycoGauge_Wave / m_fPsycoGaugeMax_Wave;
    m_fPsycoGaugePercentage_Wave            = m_fPsycoGaugeRatio_Wave * 280.f;

    // 계산용 변수(Background)
    m_fPsycoGauge_Background                = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fKinesisGauge;
    m_fPsycoGaugeMax_Background             = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fMaxKinesisGauge;
    m_fPsycoGaugeRatio_Background           = m_fPsycoGauge_Background / m_fPsycoGaugeMax_Background;
    m_fPsycoGaugePercentage_Background      = m_fPsycoGaugeRatio_Background * 280.f;

//========================================================================== _float3
    m_vScale_Wave                           = _float3(m_fPsycoGaugePercentageOrigin, 32.f, 1.f);
    m_vScale_Disonance                      = _float3(m_fPsycoGaugePercentageOrigin, 32.f, 1.f);

//========================================================================== _float4
    m_vPosition_Wave                        = _float4(-63.f, -386.f, 0.f, 1.f);
    m_vPrevPosition_Wave                    = m_vPosition_Wave;

    m_vPosition_Disonance                   = _float4(-63.f, -386.f, 0.f, 1.f);

//========================================================================== Set_Scale
    m_pWave->Set_Scale(m_vScale_Wave);
    m_pWave_Background->Set_Scale(m_vScale_Wave);
    m_pDisonance->Set_Scale(m_vScale_Disonance);
    m_pDisonance_Background->Set_Scale(m_vScale_Disonance);

//========================================================================== Set_Position
    m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
    m_pWave_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
    m_pDisonance->Set_State(TRANSFORM::POSITION, m_vPosition_Disonance);
    m_pDisonance_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Disonance);

#pragma region Bounce
    m_bLeft                         = false;
    m_bRight                        = false;
    m_bTop                          = false;
    m_bBottom                       = false;
    m_bUIBounce                     = false;
    m_fOriginalPosX                 = m_vPosition_Wave.x;
    m_fOriginalPosY                 = m_vPosition_Wave.y;
    m_fMovedPositionX_Left          = m_vPosition_Wave.x + 10.f;
    m_fMovedPositionX_Right         = m_vPosition_Wave.x - 10.f;
    m_fMovedPositionY_Top           = m_vPosition_Wave.y - 5.f;
    m_fMovedPositionY_Bottom        = m_vPosition_Wave.y + 5.f;
#pragma endregion
}

void CUI_Psyco_Wave::Tick_UI(_float fTimeDelta)
{
    //Set_Psyco_Disonance();

    UIBounce_System(fTimeDelta);
    Tick_SpriteTexture(fTimeDelta);
    Consume_System(fTimeDelta);
}

void CUI_Psyco_Wave::UIBounce_System(_float fTimeDelta)
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
                m_vPosition_Wave.x += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

                if (m_vPosition_Wave.x > m_fMovedPositionX_Left)
                {
                    m_vPosition_Wave.x = m_fMovedPositionX_Left;
                    m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
                }
            }
        }
        else
        {
            m_vPosition_Wave.x -= fTimeDelta * 30.f;
            m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);


            if (m_vPosition_Wave.x < m_fOriginalPosX)
            {
                m_vPosition_Wave.x = m_fOriginalPosX;
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

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
                m_vPosition_Wave.x -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

                if (m_vPosition_Wave.x < m_fMovedPositionX_Right)
                {
                    m_vPosition_Wave.x = m_fMovedPositionX_Right;
                    m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
                }
            }
        }
        else
        {
            m_vPosition_Wave.x += fTimeDelta * 30.f;
            m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);


            if (m_vPosition_Wave.x > m_fOriginalPosX)
            {
                m_vPosition_Wave.x = m_fOriginalPosX;
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

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
                m_vPosition_Wave.y -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

                if (m_vPosition_Wave.y < m_fMovedPositionY_Top)
                {
                    m_vPosition_Wave.y = m_fMovedPositionY_Top;
                    m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
                }
            }
        }
        else
        {
            m_vPosition_Wave.y += fTimeDelta * 30.f;
            m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);


            if (m_vPosition_Wave.y > m_fOriginalPosY)
            {
                m_vPosition_Wave.y = m_fOriginalPosY;
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

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
                m_vPosition_Wave.y += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

                if (m_vPosition_Wave.y > m_fMovedPositionY_Bottom)
                {
                    m_vPosition_Wave.y = m_fMovedPositionY_Bottom;
                    m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);
                }
            }
        }
        else
        {
            m_vPosition_Wave.y -= fTimeDelta * 30.f;
            m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);


            if (m_vPosition_Wave.y < m_fOriginalPosY)
            {
                m_vPosition_Wave.y = m_fOriginalPosY;
                m_pWave->Set_State(TRANSFORM::POSITION, m_vPosition_Wave);

                m_bBottom = false;
            }
        }
    }
}

void CUI_Psyco_Wave::Tick_SpriteTexture(_float fTimeDelta)
{
    m_tUIDesc_Disonance.m_fTime += fTimeDelta * 30.f;

    if (m_tUIDesc_Disonance.m_fFramePerSec <= m_tUIDesc_Disonance.m_fTime)
    {
        m_tUIDesc_Disonance.m_iIndex += 1;
        if (m_tUIDesc_Disonance.m_iIndex >= m_tUIDesc_Disonance.m_iIndexMaxY * m_tUIDesc_Disonance.m_iIndexMaxX)
        {
            m_tUIDesc_Disonance.m_iIndex = 0;
        }
        m_tUIDesc_Disonance.m_fTime = 0.f;
    }
}

void CUI_Psyco_Wave::Consume_System(_float fTimeDelta)
{
#pragma region Set_Value
    m_fPsycoGauge_Wave      = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fKinesisGauge;
    m_fPsycoGaugeMax_Wave   = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().fMaxKinesisGauge;
    m_fPsycoGaugeRatio_Wave = m_fPsycoGauge_Wave / m_fPsycoGaugeMax_Wave;
#pragma endregion

#pragma region Set_Background_Value
    if (m_fPsycoGaugeRatio_Wave <= m_fPsycoGaugeRatio_Background)
    {        
        if (1.f <= m_fAccTime)
        {
            m_fPsycoGaugeRatio_Background -= fTimeDelta * 0.25f;
        }
        else
        {
            m_fAccTime += fTimeDelta * 0.75f;
        }
    }
    else if (m_fPsycoGaugeRatio_Wave > m_fPsycoGaugeRatio_Background)
    {
        m_fAccTime = 0.f;

        m_fPsycoGaugeRatio_Background += fTimeDelta;
    }

#pragma endregion
}

void CUI_Psyco_Wave::Set_Psyco_Wave()
{
    m_bIsBattle = false;
}

void CUI_Psyco_Wave::Set_Psyco_Disonance()
{
    m_bIsBattle = true;
}

HRESULT CUI_Psyco_Wave::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Psyco_Wave::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Psyco_Wave> CUI_Psyco_Wave::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Psyco_Wave> pInstance = make_private_shared(CUI_Psyco_Wave, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Psyco_Wave::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Psyco_Wave::Clone(any _pPlayerEntityDesc)
{
    shared_ptr<CUI_Psyco_Wave> pInstance = make_private_shared_copy(CUI_Psyco_Wave, *this);

    if (FAILED(pInstance->Initialize(_pPlayerEntityDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Psyco_Wave::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
