#include "ClientPCH.h"
#include "UI_Indicator_SpcObjectAttack_Rock.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_SpcObjectAttack_Rock::CUI_Indicator_SpcObjectAttack_Rock(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_SpcObjectAttack_Rock::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack_Rock::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Initialize", "Failed to CGameObject::Initialize");
    }
    
    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_SpcObjectAttack_Rock::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_SpcObjectAttack_Rock::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_SpcObjectAttack_Rock::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        if (true == m_bRenderSwitch)
        {
            // Fillter
            if (FAILED(m_pTexture_Fillter1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFillter1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(74)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
            if (FAILED(m_pTexture_Fillter2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFillter2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(74)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }

            // Key
            if (FAILED(m_pTexture_Key_A->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_A->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(70)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
            if (FAILED(m_pTexture_Key_D->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pKey_D->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(70)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }

            // DownArrow
            if (FAILED(m_pTexture_DownArrow1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pDownArrow1->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(69)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }
            if (FAILED(m_pTexture_DownArrow2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pDownArrow2->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(69)))
            {
                MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
            }

            if (true == m_bInputKey)
            {
                // HighLightImpulse
                if (FAILED(m_pTexture_HighLightImpulse1->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHighLightImpulse1->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fHighLightImpulseAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(73)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }

                if (FAILED(m_pTexture_HighLightImpulse2->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHighLightImpulse2->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rock::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fHighLightImpulseAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(73)))
                {
                    MSG_RETURN(E_FAIL, "CUI_HP::Render", "Failed to CGameObject::Render");
                }
            }
        }
    }

    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack_Rock::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_SpcObjectAttack_Rock::Initialize_UI()
{
//=========================================================================== CTransform
    m_pFillter1                     = CTransform::Create(m_pDevice, m_pContext);
    m_pFillter2                     = CTransform::Create(m_pDevice, m_pContext);
    m_pDownArrow1                   = CTransform::Create(m_pDevice, m_pContext);
    m_pDownArrow2                   = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_A                        = CTransform::Create(m_pDevice, m_pContext);
    m_pKey_D                        = CTransform::Create(m_pDevice, m_pContext);
    m_pHighLightImpulse1            = CTransform::Create(m_pDevice, m_pContext);
    m_pHighLightImpulse2            = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== CTexture
    m_pTexture_Fillter1             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png"));
    m_pTexture_Fillter2             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png"));
    m_pTexture_DownArrow1           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/SpcDown.png"));
    m_pTexture_DownArrow2           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/SpcDown.png"));
    m_pTexture_Key_A                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Mouse_L.png"));
    m_pTexture_Key_D                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Mouse_R.png"));
    m_pTexture_HighLightImpulse1    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/SpcImpulse.png"));
    m_pTexture_HighLightImpulse2    = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/SpcImpulse.png"));

//=========================================================================== _bool
    m_bRenderSwitch                 = false;
    m_bFinish                       = false;
    m_bScaleFinish_Fillter          = false;
    m_bScaleFinish_DownArrow        = false;
    m_bScaleFinish_Key              = false;
    m_bScaleFinish_HighLightImpulse = false;
    m_bScaleFinish                  = false;
    m_bStart_WaitingKeyInput        = false;
    m_bInputKey                     = false;
//=========================================================================== _int
    m_iPositionChanger              = 1;

//=========================================================================== _float
    m_fTime                         = 0.f;
    m_fHighLightImpulseAlpha        = 0.f;

//=========================================================================== _float3
    m_vScale_Fillter                = _float3(900.f, 900.f, 1.f );
    m_vScale_DownArrow              = _float3(350.f, 350.f, 1.f);
    m_vScale_Key                    = _float3(350.f, 350.f, 1.f);
    m_vScale_HighLightImpulse       = _float3(60.f, 60.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Fillter1            = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_Fillter2            = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_DownArrow1          = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_DownArrow2          = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_Key_A               = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_Key_D               = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_HighLightImpulse1   = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_HighLightImpulse2   = _float4( 150.f, -100.f, 0.f, 1.f);
    
//=========================================================================== Set_Scale
    m_pFillter1->Set_Scale(m_vScale_Fillter);
    m_pFillter2->Set_Scale(m_vScale_Fillter);
    m_pDownArrow1->Set_Scale(m_vScale_DownArrow);
    m_pDownArrow2->Set_Scale(m_vScale_DownArrow);
    m_pKey_A->Set_Scale(m_vScale_Key);
    m_pKey_D->Set_Scale(m_vScale_Key);
    m_pHighLightImpulse1->Set_Scale(m_vScale_HighLightImpulse);
    m_pHighLightImpulse2->Set_Scale(m_vScale_HighLightImpulse);

//=========================================================================== Set_Position
    m_pFillter1->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter1);
    m_pFillter2->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter2);
    m_pDownArrow1->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow1);
    m_pDownArrow2->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow2);
    m_pKey_A->Set_State(TRANSFORM::POSITION, m_vPosition_Key_A);
    m_pKey_D->Set_State(TRANSFORM::POSITION, m_vPosition_Key_D);
    m_pHighLightImpulse1->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse1);
    m_pHighLightImpulse2->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse2);

}

void CUI_Indicator_SpcObjectAttack_Rock::Tick_UI(_float fTimeDelta)
{
    //Set_RenderStart();

    System(fTimeDelta);
}

void CUI_Indicator_SpcObjectAttack_Rock::Set_RenderStart()
{
    //if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    //{
    //    m_bRenderSwitch = true;
    //} 
    //if (CGameInstance::Get_Instance()->Key_Down(VK_F8))
    //{
    //    m_bInputKey = true;
    //}

    m_bRenderSwitch = true;
}

void CUI_Indicator_SpcObjectAttack_Rock::Set_KeyInput()
{
    m_bInputKey = true;
}

void CUI_Indicator_SpcObjectAttack_Rock::System(_float fTimeDelta)
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        if (true == m_bRenderSwitch)
        {
#pragma region 1. Alpha

#pragma endregion
#pragma region 2. DownArrow
            if (true == m_bScaleFinish_Fillter && true == m_bScaleFinish_DownArrow && true == m_bScaleFinish_Key)
            {
                m_bStart_WaitingKeyInput = true;
            }
            else
            {
                m_vPosition_DownArrow1.y = m_vPosition_Key_A.y + 50.f;
                m_vPosition_DownArrow2.y = m_vPosition_Key_D.y + 50.f;
                m_pDownArrow1->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow1);
                m_pDownArrow2->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow2);
            }
#pragma endregion
#pragma region 3. Scale
            if (false == m_bScaleFinish_Fillter)
            {
                // Fillter
                if (360.f >= m_vScale_Fillter.x && 360.f >= m_vScale_Fillter.y)
                {
                    m_vScale_Fillter.x = 360.f;
                    m_vScale_Fillter.y = 360.f;
                    m_pFillter1->Set_Scale(m_vScale_Fillter);
                    m_pFillter2->Set_Scale(m_vScale_Fillter);
                    m_bScaleFinish_Fillter = true;
                }
                else
                {
                    m_vScale_Fillter.x -= fTimeDelta * 1000.f;
                    m_vScale_Fillter.y -= fTimeDelta * 1000.f;
                    m_pFillter1->Set_Scale(m_vScale_Fillter);
                    m_pFillter2->Set_Scale(m_vScale_Fillter);
                }
            }

            if (false == m_bScaleFinish_DownArrow)
            {
                // DownArrow
                if (70.f >= m_vScale_DownArrow.x && 70.f >= m_vScale_DownArrow.y)
                {
                    m_vScale_DownArrow.x = 70.f;
                    m_vScale_DownArrow.y = 70.f;
                    m_pDownArrow1->Set_Scale(m_vScale_DownArrow);
                    m_pDownArrow2->Set_Scale(m_vScale_DownArrow);
                    m_bScaleFinish_DownArrow = true;
                }
                else
                {
                    m_vScale_DownArrow.x -= fTimeDelta * 1000.f;
                    m_vScale_DownArrow.y -= fTimeDelta * 1000.f;
                    m_pDownArrow1->Set_Scale(m_vScale_DownArrow);
                    m_pDownArrow2->Set_Scale(m_vScale_DownArrow);
                }
            }

            if (false == m_bScaleFinish_Key)
            {
                // Key
                if (48.f >= m_vScale_Key.x && 62.f >= m_vScale_Key.y)
                {
                    m_vScale_Key.x = 48.f;
                    m_vScale_Key.y = 62.f;
                    m_pKey_A->Set_Scale(m_vScale_Key);
                    m_pKey_D->Set_Scale(m_vScale_Key);
                    m_bScaleFinish_Key = true;
                }
                else
                {
                    m_vScale_Key.x -= fTimeDelta * 1000.f;
                    m_vScale_Key.y -= fTimeDelta * 1000.f;
                    m_pKey_A->Set_Scale(m_vScale_Key);
                    m_pKey_D->Set_Scale(m_vScale_Key);
                }
            }
#pragma endregion
#pragma region 4. Position
            if (true == m_bStart_WaitingKeyInput && false == m_bInputKey)
            {
                if (m_vPosition_Key_A.y + 50.f <= m_vPosition_DownArrow1.y)
                {
                    m_iPositionChanger = -1;
                }
                else if (m_vPosition_Key_A.y + 43.f >= m_vPosition_DownArrow1.y)
                {
                    m_iPositionChanger = 1;
                }
                m_vPosition_DownArrow1.y += fTimeDelta * 100.f * m_iPositionChanger;
                m_vPosition_DownArrow2.y += fTimeDelta * 100.f * m_iPositionChanger;
                m_pDownArrow1->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow1);
                m_pDownArrow2->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow2);
            }       
            if (true == m_bInputKey)
            {
                if (100.f >= m_vScale_Key.x && 100.f >= m_vScale_Key.y)
                {
                    m_vScale_Key.x = 100.f;
                    m_vScale_Key.y = 100.f;
                    m_pKey_A->Set_Scale(m_vScale_Key);
                    m_pKey_D->Set_Scale(m_vScale_Key);
                }
                else
                {
                    m_vScale_Key.x += fTimeDelta * 100.f;
                    m_vScale_Key.y += fTimeDelta * 100.f;
                    m_pKey_A->Set_Scale(m_vScale_Key);
                    m_pKey_D->Set_Scale(m_vScale_Key);
                }

                // HighLightImpulse
                if (350.f <= m_vScale_HighLightImpulse.x && 350.f <= m_vScale_HighLightImpulse.y)
                {
                    m_vScale_HighLightImpulse.x = 350.f;
                    m_vScale_HighLightImpulse.y = 350.f;
                    m_pHighLightImpulse1->Set_Scale(m_vScale_HighLightImpulse);
                    m_pHighLightImpulse2->Set_Scale(m_vScale_HighLightImpulse);
                    Reset();
                }
                else
                {
                    m_vScale_HighLightImpulse.x += fTimeDelta * 800.f;
                    m_vScale_HighLightImpulse.y += fTimeDelta * 800.f;
                    m_pHighLightImpulse1->Set_Scale(m_vScale_HighLightImpulse);
                    m_pHighLightImpulse2->Set_Scale(m_vScale_HighLightImpulse);

                    if (1.f <= m_fHighLightImpulseAlpha)
                    {
                        m_fHighLightImpulseAlpha = 1.f;
                    }
                    else
                    {
                        m_fHighLightImpulseAlpha += fTimeDelta;
                    }
                }
            }
#pragma endregion
        }
    }
}

void CUI_Indicator_SpcObjectAttack_Rock::Reset()
{
//=========================================================================== _bool
    m_bRenderSwitch                 = false;
    m_bFinish                       = false;
    m_bScaleFinish_Fillter          = false;
    m_bScaleFinish_DownArrow        = false;
    m_bScaleFinish_Key              = false;
    m_bScaleFinish_HighLightImpulse = false;
    m_bScaleFinish                  = false;
    m_bStart_WaitingKeyInput        = false;
    m_bInputKey                     = false;
//=========================================================================== _int
    m_iPositionChanger              = 1;

//=========================================================================== _float
    m_fTime                         = 0.f;
    m_fHighLightImpulseAlpha        = 0.f;

//=========================================================================== _float3
    m_vScale_Fillter                = _float3(900.f, 900.f, 1.f );
    m_vScale_DownArrow              = _float3(350.f, 350.f, 1.f);
    m_vScale_Key                    = _float3(266.f, 350.f, 1.f);
    m_vScale_HighLightImpulse       = _float3(60.f, 60.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Fillter1            = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_Fillter2            = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_DownArrow1          = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_DownArrow2          = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_Key_A               = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_Key_D               = _float4( 150.f, -100.f, 0.f, 1.f);
    m_vPosition_HighLightImpulse1   = _float4(-150.f, -100.f, 0.f, 1.f);
    m_vPosition_HighLightImpulse2   = _float4( 150.f, -100.f, 0.f, 1.f);
    
//=========================================================================== Set_Scale
    m_pFillter1->Set_Scale(m_vScale_Fillter);
    m_pFillter2->Set_Scale(m_vScale_Fillter);
    m_pDownArrow1->Set_Scale(m_vScale_DownArrow);
    m_pDownArrow2->Set_Scale(m_vScale_DownArrow);
    m_pKey_A->Set_Scale(m_vScale_Key);
    m_pKey_D->Set_Scale(m_vScale_Key);
    m_pHighLightImpulse1->Set_Scale(m_vScale_HighLightImpulse);
    m_pHighLightImpulse2->Set_Scale(m_vScale_HighLightImpulse);

//=========================================================================== Set_Position
    m_pFillter1->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter1);
    m_pFillter2->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter2);
    m_pDownArrow1->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow1);
    m_pDownArrow2->Set_State(TRANSFORM::POSITION, m_vPosition_DownArrow2);
    m_pKey_A->Set_State(TRANSFORM::POSITION, m_vPosition_Key_A);
    m_pKey_D->Set_State(TRANSFORM::POSITION, m_vPosition_Key_D);
    m_pHighLightImpulse1->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse1);
    m_pHighLightImpulse2->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse2);
}

shared_ptr<CUI_Indicator_SpcObjectAttack_Rock> CUI_Indicator_SpcObjectAttack_Rock::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack_Rock> pInstance = make_private_shared(CUI_Indicator_SpcObjectAttack_Rock, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack_Rock::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_SpcObjectAttack_Rock::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack_Rock> pInstance = make_private_shared_copy(CUI_Indicator_SpcObjectAttack_Rock, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack_Rock::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

