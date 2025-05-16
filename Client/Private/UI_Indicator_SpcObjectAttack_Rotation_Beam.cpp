#include "ClientPCH.h"
#include "UI_Indicator_SpcObjectAttack_Rotation_Beam.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_SpcObjectAttack_Rotation_Beam::CUI_Indicator_SpcObjectAttack_Rotation_Beam(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_SpcObjectAttack_Rotation_Beam::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack_Rotation_Beam::Initialize(any _aDesc)
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

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_7);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        if (true == m_bRenderSwitch)
        {
            // Fillter
            if (FAILED(m_pTexture_Fillter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pFillter->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(68)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
            }
            
            // RotationArrow
            if (FAILED(m_pTexture_RotationArrow->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pRotationArrow->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(69)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
            }

            // Background
            if (FAILED(m_pTexture_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pBackground->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(70)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
            }

            // Mouse
            if (FAILED(m_pTexture_Mouse->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pMouse->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(70)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
            }

            if (true == m_bRotateMouse && false == m_bFinish)
            {
                // Impulse
                if (FAILED(m_pTexture_Impulse->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pImpulse->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fImpulseAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(71)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
                }
            }

            if (true == m_bFinish)
            {
                // HighLightFillter
                if (FAILED(m_pTexture_HighLightFillter->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHighLightFillter->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fHighLightFillterAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(72)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
                }

                // HighLightImpulse
                if (FAILED(m_pTexture_HighLightImpulse->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pHighLightImpulse->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTimeAlpha", &m_fHighLightImpulseAlpha, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(73)))
                {
                    MSG_RETURN(E_FAIL, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Render", "Failed to CGameObject::Render");
                }
            }
        }
    }
    
    return S_OK;
}

HRESULT CUI_Indicator_SpcObjectAttack_Rotation_Beam::Ready_Components()
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

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Initialize_UI()
{
//=========================================================================== CTransform
    m_pFillter                      = CTransform::Create(m_pDevice, m_pContext);
    m_pRotationArrow                = CTransform::Create(m_pDevice, m_pContext);
    m_pBackground                   = CTransform::Create(m_pDevice, m_pContext);
    m_pMouse                        = CTransform::Create(m_pDevice, m_pContext);
    m_pImpulse                      = CTransform::Create(m_pDevice, m_pContext);
    m_pHighLightFillter             = CTransform::Create(m_pDevice, m_pContext);
    m_pHighLightImpulse             = CTransform::Create(m_pDevice, m_pContext);

//=========================================================================== CTexture
    m_pTexture_Fillter              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png"));
    m_pTexture_RotationArrow        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Rotation_Arrow.png"));
    m_pTexture_Background           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Background.png"));
    m_pTexture_Mouse                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Mouse.png"));
    m_pTexture_Impulse              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Impulse.png"));
    m_pTexture_HighLightFillter     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/Fillter.png"));
    m_pTexture_HighLightImpulse     = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/SpecialObject/SpcImpulse.png"));

//=========================================================================== _bool
    m_bRenderSwitch                 = false;
    m_bRotateMouse                  = false;
    m_bFinish                       = false;
    m_bFinish_HighLightFillter      = false;
    m_bFinish_HighLightImpulse      = false;
//=========================================================================== _int

//=========================================================================== _float
    m_fTime                         = 0.f;
    m_fImpulseAlpha                 = 1.f;
    m_fHighLightFillterAlpha        = 1.f;
    m_fHighLightImpulseAlpha        = 0.f;

//=========================================================================== _float3
    m_vScale_Fillter                = _float3(720.f, 720.f, 1.f);
    m_vScale_RotationArrow          = _float3(208.f, 160.f, 1.f);//_float3(82.f, 60.f, 1.f);
    m_vScale_Background             = _float3(134.f, 134.f, 1.f);
    m_vScale_Mouse                  = _float3(160.f, 160.f, 1.f);
    m_vScale_Impulse                = _float3(140.f, 140.f, 1.f);//_float3(67.f, 67.f, 67.f);//
    m_vScale_HighLightFillter       = _float3(540.f, 540.f, 1.f);
    m_vScale_HighLightImpulse       = _float3(100.f, 100.f, 1.f);

//=========================================================================== _float4
    m_vPosition_Fillter             = _float4(0.f, 150.f, 0.f, 1.f);
    m_vPosition_RotationArrow       = m_vPosition_Fillter;
    m_vPosition_Background          = _float4(0.f, -150.f, 0.f, 1.f);
    m_vPosition_Mouse               = m_vPosition_Background + m_Distance;
    m_vPosition_Impulse             = m_vPosition_Background;
    m_vPosition_HighLightFillter    = m_vPosition_Background;
    m_vPosition_HighLightImpulse    = m_vPosition_Background;
    
//=========================================================================== Set_Scale
    m_pFillter->Set_Scale(m_vScale_Fillter);
    m_pRotationArrow->Set_Scale(m_vScale_RotationArrow);
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMouse->Set_Scale(m_vScale_Mouse);
    m_pImpulse->Set_Scale(m_vScale_Impulse);
    m_pHighLightFillter->Set_Scale(m_vScale_HighLightFillter);
    m_pHighLightImpulse->Set_Scale(m_vScale_HighLightImpulse);
    
//=========================================================================== Set_Position
    m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter);
    m_pRotationArrow->Set_State(TRANSFORM::POSITION, m_vPosition_RotationArrow);
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMouse->Set_State(TRANSFORM::POSITION, m_vPosition_Mouse);
    m_pImpulse->Set_State(TRANSFORM::POSITION, m_vPosition_Impulse);
    m_pHighLightFillter->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightFillter);
    m_pHighLightImpulse->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse);

}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Tick_UI(_float fTimeDelta)
{
    //Set_RenderStart();
    System(fTimeDelta);
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Set_RenderStart()
{
    //// Test
    //if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
    //{
    //    m_bRenderSwitch = true;
    //}

    m_bRenderSwitch = true;
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Set_RotateMouse()
{
    m_bRotateMouse = true;
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Set_Finish()
{
    m_bFinish = true;
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::System(_float fTimeDelta)
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI() 
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI())
    {
        if (true == m_bRenderSwitch)
        {
#pragma region 1. Alpha
            if (true == m_bRotateMouse)
            {
                if (0.f >= m_fImpulseAlpha)
                {
                    m_fImpulseAlpha = 1.f;
                }
                else
                {
                    m_fImpulseAlpha -= fTimeDelta * 10.f;
                }
            }

#pragma endregion
#pragma region 2. Rotation
            // RotationArrow
            m_pRotationArrow->Rotate(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(10.f));

            // Mouse
            if (true == m_bRotateMouse)
            {
                m_Distance = XMVector3TransformCoord(m_Distance, XMMatrixRotationZ(XMConvertToRadians(13.f)));
                m_vPosition_Mouse = m_vPosition_Background + m_Distance;
                m_pMouse->Set_State(TRANSFORM::POSITION, m_vPosition_Mouse);
            }
#pragma endregion
#pragma region 3. Scale
            if (false == m_bFinish)
            {
                // Fillter
                if (360.f >= m_vScale_Fillter.x && 360.f >= m_vScale_Fillter.y)
                {
                    m_vScale_Fillter.x = 360.f;
                    m_vScale_Fillter.y = 360.f;
                    m_pFillter->Set_Scale(m_vScale_Fillter);
                }
                else
                {
                    m_vScale_Fillter.x -= fTimeDelta * 300.f;
                    m_vScale_Fillter.y -= fTimeDelta * 300.f;
                    m_pFillter->Set_Scale(m_vScale_Fillter);
                }

                // RotationArrow
                if (82.f >= m_vScale_RotationArrow.x && 60.f >= m_vScale_RotationArrow.y)
                {
                    m_vScale_RotationArrow.x = 82.f;
                    m_vScale_RotationArrow.y = 60.f;
                    m_pRotationArrow->Set_Scale(m_vScale_RotationArrow);
                }
                else
                {
                    m_vScale_RotationArrow.x -= fTimeDelta * 600.f;
                    m_vScale_RotationArrow.y -= fTimeDelta * 600.f;
                    m_pRotationArrow->Set_Scale(m_vScale_RotationArrow);
                }

                // Background
                if (67.f >= m_vScale_Background.x && 67.f >= m_vScale_Background.y)
                {
                    m_vScale_Background.x = 67.f;
                    m_vScale_Background.y = 67.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                }
                else
                {
                    m_vScale_Background.x -= fTimeDelta * 300.f;
                    m_vScale_Background.y -= fTimeDelta * 300.f;
                    m_pBackground->Set_Scale(m_vScale_Background);
                }

                // Mouse
                if (70.f >= m_vScale_Mouse.x && 70.f >= m_vScale_Mouse.y)
                {
                    m_vScale_Mouse.x = 70.f;
                    m_vScale_Mouse.y = 70.f;
                    m_pMouse->Set_Scale(m_vScale_Mouse);
                }
                else
                {
                    m_vScale_Mouse.y -= fTimeDelta * 300.f;
                    m_vScale_Mouse.x -= fTimeDelta * 300.f;
                    m_pMouse->Set_Scale(m_vScale_Mouse);
                }

                // Impulse
                if (true == m_bRotateMouse)
                {
                    if (220.f <= m_vScale_Impulse.x && 220.f <= m_vScale_Impulse.y)
                    {
                        m_vScale_Impulse.x = 100.f;
                        m_vScale_Impulse.y = 100.f;
                        m_pImpulse->Set_Scale(m_vScale_Impulse);
                    }
                    else
                    {
                        m_vScale_Impulse.x += fTimeDelta * 600.f;
                        m_vScale_Impulse.y += fTimeDelta * 600.f;
                        m_pImpulse->Set_Scale(m_vScale_Impulse);
                    }
                }
            }
            else
            {
                m_vPosition_HighLightFillter = m_vPosition_Background;
                m_vPosition_HighLightImpulse = m_vPosition_Background;
                m_pHighLightFillter->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightFillter);
                m_pHighLightImpulse->Set_State(TRANSFORM::POSITION, m_vPosition_HighLightImpulse);

                // HighLightFillter
                if (100.f >= m_vScale_HighLightFillter.x && 100.f >= m_vScale_HighLightFillter.y)
                {
                    m_vScale_HighLightFillter.x = 100.f;
                    m_vScale_HighLightFillter.y = 100.f;
                    m_pHighLightFillter->Set_Scale(m_vScale_HighLightFillter);

                    if (0.f >= m_fHighLightFillterAlpha)
                    {
                        m_fHighLightFillterAlpha = 0.f;
                        m_bFinish_HighLightFillter = true;
                    }
                    else
                    {
                        m_fImpulseAlpha -= fTimeDelta * 50.f;
                    }

                }
                else
                {
                    m_vScale_HighLightFillter.x -= fTimeDelta * 600.f;
                    m_vScale_HighLightFillter.y -= fTimeDelta * 600.f;
                    m_pHighLightFillter->Set_Scale(m_vScale_HighLightFillter);

                    if (0.f >= m_fHighLightFillterAlpha)
                    {
                        m_fHighLightFillterAlpha = 0.f;
                    }
                    else
                    {
                        m_fHighLightFillterAlpha -= fTimeDelta * 50.f;
                    }
                }

                // HighLigntImpulse
                if (350.f <= m_vScale_HighLightImpulse.x && 350.f <= m_vScale_HighLightImpulse.y)
                {
                    m_vScale_HighLightImpulse.x = 350.f;
                    m_vScale_HighLightImpulse.y = 350.f;
                    m_pHighLightImpulse->Set_Scale(m_vScale_HighLightImpulse);

                    if (0.f >= m_fHighLightImpulseAlpha)
                    {
                        m_fHighLightImpulseAlpha = 0.f;
                        m_bFinish_HighLightImpulse = true;
                    }
                    else
                    {
                        m_fHighLightImpulseAlpha -= fTimeDelta * 50.f;
                    }
                }
                else
                {
                    m_vScale_HighLightImpulse.x += fTimeDelta * 600.f;
                    m_vScale_HighLightImpulse.y += fTimeDelta * 600.f;
                    m_pHighLightImpulse->Set_Scale(m_vScale_HighLightImpulse);

                    if (1.f <= m_fHighLightImpulseAlpha)
                    {
                        m_fHighLightImpulseAlpha = 1.f;
                    }
                    else
                    {
                        m_fHighLightImpulseAlpha += fTimeDelta * 50.f;
                    }
                }

                if (true == m_bFinish_HighLightFillter && true == m_bFinish_HighLightImpulse)
                {
                    Reset();
                }
            }
#pragma endregion
#pragma region 4. Position
            // Fillter
            if (70.f >= m_vPosition_Fillter.y)
            {
                m_vPosition_Fillter.y = 70.f;
                m_vPosition_RotationArrow.y = m_vPosition_Fillter.y;

                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter);
                m_pRotationArrow->Set_State(TRANSFORM::POSITION, m_vPosition_RotationArrow);
            }
            else
            {
                m_vPosition_Fillter.y -= fTimeDelta * 1000.f;
                m_vPosition_RotationArrow.y = m_vPosition_Fillter.y;

                m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter);
                m_pRotationArrow->Set_State(TRANSFORM::POSITION, m_vPosition_RotationArrow);
            }

            // Background
            if (-70.f >= m_vPosition_Background.y)
            {
                m_vPosition_Background.y = -70.f;
                m_vPosition_Mouse.y = m_vPosition_Mouse.y;

                m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
                m_pMouse->Set_State(TRANSFORM::POSITION, m_vPosition_Mouse);
                m_pImpulse->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
            }
            else
            {
                m_vPosition_Background.y += fTimeDelta * 1000.f;
                m_vPosition_Mouse.y = m_vPosition_Mouse.y;

                m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
                m_pMouse->Set_State(TRANSFORM::POSITION, m_vPosition_Mouse);
            }
#pragma endregion
        }
    }
}

void CUI_Indicator_SpcObjectAttack_Rotation_Beam::Reset()
{
//=========================================================================== _bool
    m_bRenderSwitch             = false;
    m_bRotateMouse              = false;
    m_bFinish                   = false;

//=========================================================================== _int

//=========================================================================== _float
    m_fTime                     = 0.f;
    m_fImpulseAlpha             = 1.f;

//=========================================================================== _float3
    m_vScale_Fillter            = _float3(720.f, 720.f, 1.f);
    m_vScale_RotationArrow      = _float3(208.f, 160.f, 1.f);//_float3(82.f, 60.f, 1.f);
    m_vScale_Background         = _float3(134.f, 134.f, 1.f);
    m_vScale_Mouse              = _float3(160.f, 160.f, 1.f);
    m_vScale_Impulse            = _float3(140.f, 140.f, 1.f);//_float3(67.f, 67.f, 67.f);//
    
//=========================================================================== _float4
    m_vPosition_Fillter         = _float4(0.f, 150.f, 0.f, 1.f);
    m_vPosition_RotationArrow   = m_vPosition_Fillter;
    m_vPosition_Background      = _float4(0.f, -150.f, 0.f, 1.f);
    m_vPosition_Mouse           = m_vPosition_Background + m_Distance;
    m_vPosition_Impulse         = m_vPosition_Background;
    
//=========================================================================== Set_Scale
    m_pFillter->Set_Scale(m_vScale_Fillter);
    m_pRotationArrow->Set_Scale(m_vScale_RotationArrow);
    m_pBackground->Set_Scale(m_vScale_Background);
    m_pMouse->Set_Scale(m_vScale_Mouse);
    m_pImpulse->Set_Scale(m_vScale_Impulse);
    
//=========================================================================== Set_Position
    m_pFillter->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter);
    m_pRotationArrow->Set_State(TRANSFORM::POSITION, m_vPosition_RotationArrow);
    m_pBackground->Set_State(TRANSFORM::POSITION, m_vPosition_Background);
    m_pMouse->Set_State(TRANSFORM::POSITION, m_vPosition_Mouse);
    m_pImpulse->Set_State(TRANSFORM::POSITION, m_vPosition_Impulse);
}

shared_ptr<CUI_Indicator_SpcObjectAttack_Rotation_Beam> CUI_Indicator_SpcObjectAttack_Rotation_Beam::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack_Rotation_Beam> pInstance = make_private_shared(CUI_Indicator_SpcObjectAttack_Rotation_Beam, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_SpcObjectAttack_Rotation_Beam::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_SpcObjectAttack_Rotation_Beam> pInstance = make_private_shared_copy(CUI_Indicator_SpcObjectAttack_Rotation_Beam, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_SpcObjectAttack_Rotation_Beam::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
