#include "ClientPCH.h"
#include "UI_Indicator_InstKill.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Indicator_InstKill::CUI_Indicator_InstKill(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_InstKill::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_InstKill::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Initialize", "Failed to CGameObject::Initialize");
    }


    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_BrainTalk_Tutorial::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_InstKill::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_InstKill::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(RENDER_GROUP::BLOOM);
    Add_RenderObject(UI_LAYER::LAYER_5);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_InstKill::Render_Bloom()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && SCENE::TEST != CGameInstance::Get_Instance()->Current_Scene()
        && true == m_bRenderSwitch
        && true == m_bRenderDegree)
    {
        // Indicator_Bloom
        if (FAILED(m_pTexture_Indicator->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_Bloom->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_Float("g_fBloomStrength", 1.5f)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShader_Indicator_Bloom, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(__super::Render(81)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }

        // Fillter_Square_Bloom
        if (FAILED(m_pTexture_Fillter_Square->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pFillter_Square->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_Float("g_fBloomStrength", 1.5f)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(__super::Render(80)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
    }
 
    return S_OK;
}

HRESULT CUI_Indicator_InstKill::Render()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
        && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
        && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
        && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
        && SCENE::TEST != CGameInstance::Get_Instance()->Current_Scene()
        && true == m_bRenderSwitch
        && true == m_bRenderDegree)
    {
        if (true == m_bRenderSwitch_ScalingIndicator)
        {
            // Indicator_Scaling
            if (FAILED(m_pTexture_Indicator->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pIndicator_Scaling->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShader_Indicator_Scaling, sizeof(_float))))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
            if (FAILED(__super::Render(82)))
            {
                MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
            }
        }

        // Indicator
        if (FAILED(m_pTexture_Indicator->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShader_Indicator, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(__super::Render(82)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }

        // Line
        if (FAILED(m_pTexture_Line->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pLine->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShader_Line, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(__super::Render(79)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        
        // Tag
        if (FAILED(m_pTexture_Tag->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pTag->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fTime", &m_fShader_Tag, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
        if (FAILED(__super::Render(82)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Render", "Failed to CUI_Indicator_InstKill::Render");
        }
    }
    
    return S_OK;
}

void CUI_Indicator_InstKill::Initialize_UI()
{
//============================================================== CTransform
    m_pLine                     = CTransform::Create(m_pDevice, m_pContext);
    m_pFillter_Square           = CTransform::Create(m_pDevice, m_pContext);
    m_pFillter_Rhombus          = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator                = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_Bloom          = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_Scaling        = CTransform::Create(m_pDevice, m_pContext);
    m_pTag                      = CTransform::Create(m_pDevice, m_pContext);
//============================================================== CTexture
    m_pTexture_Line             = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Line.png"));
    m_pTexture_Fillter_Square   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_BackFillter.png"));
    m_pTexture_Fillter_Rhombus  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Fillter.png"));
    m_pTexture_Indicator        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Indicator.png"));
    m_pTexture_Tag              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/InstK/Monster_InstK_Tag.png"));
//============================================================== _bool
    m_bRenderSwitch             = false;
    m_bTagScaling               = false;
    m_bLineScaling              = false;
    m_bRenderSwitch_ScalingIndicator    = false;
    m_bIndicator_Scaling        = false;
    m_bRenderDegree             = false;
//============================================================== _int
    m_iTagIndex                 = 0;
    m_iTagNumber                = 0;
    m_iLinePhase                = 0;
//============================================================== _float
    m_fShader_Line              = 1.f;
    m_fShader_Indicator         = 0.f;
    m_fShader_Indicator_Bloom   = 1.f;
    m_fShader_Indicator_Scaling = 1.f;
    m_fShader_Tag               = 0.f;
    m_fAccTagSetScaleTime       = 0.f;
    m_fLineScalingTime          = 0.f;
//============================================================== _float3
    m_vScale_Line               = _float3(900.f, 7.f, 1.f);
    m_vScale_Fillter_Square     = _float3(82.f, 78.f, 1.f);
    m_vScale_Fillter_Rhombus    = _float3(100.f, 100.f, 1.f);
    m_vScale_Indicator          = _float3(464.f, 464.f, 1.f);
    m_vScale_Indicator_Scaling  = _float3(174.f, 174.f, 1.f);
    m_vScale_Tag                = _float3(1.f, 1.f, 1.f);


//============================================================== _float4
    m_vPosition_Line            = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Fillter_Square  = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Fillter_Rhombus = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator       = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Tag             = _float4(0.f, 0.f, 0.f, 1.f);

//============================================================== Set_Scale
    m_pLine->Set_Scale(m_vScale_Line);
    m_pFillter_Square->Set_Scale(m_vScale_Fillter_Square);
    m_pFillter_Rhombus->Set_Scale(m_vScale_Fillter_Rhombus);
    m_pIndicator->Set_Scale(m_vScale_Indicator);
    m_pIndicator_Bloom->Set_Scale(m_vScale_Indicator);
    m_pIndicator_Scaling->Set_Scale(m_vScale_Indicator_Scaling);
    m_pTag->Set_Scale(m_vScale_Tag);

//============================================================== Set_Position
    m_pLine->Set_State(TRANSFORM::POSITION, m_vPosition_Line);
    m_pFillter_Square->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter_Square);
    m_pFillter_Rhombus->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter_Rhombus);
    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_Bloom->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_Scaling->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pTag->Set_State(TRANSFORM::POSITION, m_vPosition_Tag);

}

void CUI_Indicator_InstKill::Tick_UI(_float fTimeDelta)
{
    //TestKey();

    System(fTimeDelta);
}

void CUI_Indicator_InstKill::TestKey()
{
    if (false == m_bRenderSwitch)
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_bRenderSwitch = true;
        }
    }
    else
    {
        if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
        {
            m_bRenderSwitch             = false;
            m_bRenderSwitch_ScalingIndicator    = false;
            m_bLineScaling              = false;
            m_fShader_Line              = 1.f;
            m_vScale_Line.x             = 900.f;
            m_pLine->Set_Scale(m_vScale_Line);

            m_fShader_Indicator         = 0.f;
            m_vScale_Indicator          = _float3(464.f, 464.f, 1.f);
            m_fShader_Indicator_Bloom   = 1.f;
            m_fShader_Indicator_Scaling = 1.f;
            m_pIndicator->Set_Scale(m_vScale_Indicator);

            m_iLinePhase                = 0;
            m_iTagIndex                 = 0;
            m_iTagNumber                = 0;
            m_fShader_Tag               = 0.f;
            m_fAccTagSetScaleTime       = 0.f;
            m_vScale_Tag                = _float3(0.f, 0.f, 1.f);
            m_pTag->Set_Scale(m_vScale_Tag);

        }
    }

}

void CUI_Indicator_InstKill::System(_float fTimeDelta)
{
    if (true == m_bRenderSwitch)
    {
#pragma region Line
        if (false == m_bLineScaling)
        {
            if (10.f >= m_vScale_Line.x)
            {
                m_vScale_Line.x = 10.f;
                m_pLine->Set_Scale(m_vScale_Line);
            }
            else
            {
                m_vScale_Line.x -= fTimeDelta * 5000.f;

                if (0 >= m_vScale_Line.x)
                {
                    m_vScale_Line.x = 0.001f;
                }

                m_pLine->Set_Scale(m_vScale_Line);
            }

            if (0.f >= m_fShader_Line)
            {
                m_fShader_Line = 0.f;
            }
            else
            {
                m_fShader_Line -= fTimeDelta * 3.f;
            }
        }
        else
        {
            LineScaling(fTimeDelta);
        }
#pragma endregion

#pragma region Indicator
        if (1.f <= m_fShader_Indicator)
        {
            m_fShader_Indicator = 1.f;
        }
        else
        {
            m_fShader_Indicator += fTimeDelta * 2.f;
        }

        if (0.f >= m_fShader_Indicator_Bloom)
        {
            m_fShader_Indicator_Bloom = 0.f;
        }
        else
        {
            m_fShader_Indicator_Bloom -= fTimeDelta * 4.f;
        }

        if (174.f >= m_vScale_Indicator.x && 174.f >= m_vScale_Indicator.y)
        {
            m_vScale_Indicator.x = 174.f;
            m_vScale_Indicator.y = 174.f;
            m_pIndicator->Set_Scale(m_vScale_Indicator);
            m_bRenderSwitch_ScalingIndicator = true;
        }
        else
        {
            m_vScale_Indicator.x -= fTimeDelta * 3000.f;
            m_vScale_Indicator.y -= fTimeDelta * 3000.f;
            m_pIndicator->Set_Scale(m_vScale_Indicator);
        }
#pragma endregion

#pragma region Tag
        if (1.f <= m_fShader_Tag)
        {
            m_fShader_Tag = 1.f;
        }
        else
        {
            m_fShader_Tag += fTimeDelta * 2.f;
        }

        if (0 == m_iTagIndex)
        {
            if (60.f <= m_vScale_Tag.x && 60.f <= m_vScale_Tag.y)
            {
                m_vScale_Tag.x = 60.f;
                m_vScale_Tag.y = 60.f;
                m_pTag->Set_Scale(m_vScale_Tag);
                m_iTagIndex = 1;
            }
            else
            {
                m_vScale_Tag.x += fTimeDelta * 300.f;
                m_vScale_Tag.y += fTimeDelta * 300.f;
                m_pTag->Set_Scale(m_vScale_Tag);
            }
        }
        else if (1 == m_iTagIndex)
        {
            if (52.f >= m_vScale_Tag.x && 52.f >= m_vScale_Tag.y)
            {
                m_vScale_Tag.x = 52.f;
                m_vScale_Tag.y = 52.f;
                m_pTag->Set_Scale(m_vScale_Tag);
                m_iTagIndex = 2;
            }
            else
            {
                m_vScale_Tag.x -= fTimeDelta * 150.f;
                m_vScale_Tag.y -= fTimeDelta * 150.f;
                m_pTag->Set_Scale(m_vScale_Tag);
            }
        }
        else if (2 == m_iTagIndex)
        {
            TagBounce(fTimeDelta);
        }
#pragma endregion
    }
}

void CUI_Indicator_InstKill::TagBounce(_float fTimeDelta)
{
    if (0 == m_iTagNumber)
    {
        if (false == m_bTagScaling)
        {
            if (60.f <= m_vScale_Tag.x && 60.f <= m_vScale_Tag.y)
            {
                m_vScale_Tag.x = 60.f;
                m_vScale_Tag.y = 60.f;
                m_pTag->Set_Scale(m_vScale_Tag);
                m_bTagScaling = true;
                m_bIndicator_Scaling = true;
                m_bLineScaling = true;
            }
            else
            {
                m_vScale_Tag.x += fTimeDelta * 200.f;
                m_vScale_Tag.y += fTimeDelta * 200.f;
                m_pTag->Set_Scale(m_vScale_Tag);
            }
        }
        else
        {
            if (52.f >= m_vScale_Tag.x && 52.f >= m_vScale_Tag.y)
            {
                m_vScale_Tag.x = 52.f;
                m_vScale_Tag.y = 52.f;
                m_pTag->Set_Scale(m_vScale_Tag);
                m_bIndicator_Scaling = false;

                if (1.f <= m_fAccTagSetScaleTime)
                {
                    m_fAccTagSetScaleTime = 0.f;
                    m_iTagNumber = 1;
                }
                else
                {
                    m_fAccTagSetScaleTime += fTimeDelta * 2.f;
                }
            }
            else
            {
                m_vScale_Tag.x -= fTimeDelta * 150.f;
                m_vScale_Tag.y -= fTimeDelta * 150.f;
                m_pTag->Set_Scale(m_vScale_Tag);
            }
        }
    }
    else if (1 == m_iTagNumber)
    {
        if (1.f <= m_fAccTagSetScaleTime)
        {
            m_fAccTagSetScaleTime = 0.f;
            m_iTagNumber = 0;
            m_bTagScaling = false;
        }
        else
        {
            m_fAccTagSetScaleTime += fTimeDelta * 2.f;
        }
    }

    if (true == m_bIndicator_Scaling)
    {
        if (0.f >= m_vScale_Indicator_Scaling.x && 0.f >= m_vScale_Indicator_Scaling.y)
        {
            m_vScale_Indicator_Scaling.x = 0.f;
            m_vScale_Indicator_Scaling.y = 0.f;
            m_pIndicator_Scaling->Set_Scale(m_vScale_Indicator_Scaling);
        }
        else
        {
            m_vScale_Indicator_Scaling.x -= fTimeDelta * 500.f;
            m_vScale_Indicator_Scaling.y -= fTimeDelta * 500.f;
            m_pIndicator_Scaling->Set_Scale(m_vScale_Indicator_Scaling);
        }

        if (0.f >= m_fShader_Indicator_Scaling)
        {
            m_fShader_Indicator_Scaling = 0.f;
        }
        else
        {
            m_fShader_Indicator_Scaling -= fTimeDelta * 1.5f;
        }
    }
    else
    {
        m_vScale_Indicator_Scaling.x = 174.f;
        m_vScale_Indicator_Scaling.y = 174.f;
        m_pIndicator_Scaling->Set_Scale(m_vScale_Indicator_Scaling);

        m_fShader_Indicator_Scaling = 1.f;
    }
}

void CUI_Indicator_InstKill::LineScaling(_float fTimeDelta)
{
    if (0 == m_iLinePhase)
    {
        if (1.f <= m_fLineScalingTime)
        {
            m_fLineScalingTime = 1.f;
            m_fShader_Line = 1.f;
            m_vScale_Line.x = 1.f;
            m_pLine->Set_Scale(m_vScale_Line);
            m_iLinePhase = 1;
        }
        else
        {
            m_fLineScalingTime += fTimeDelta * 2.f;
        }
    }
    else if (1 == m_iLinePhase)
    {
        if (400.f <= m_vScale_Line.x)
        {
            m_vScale_Line.x = 400.f;
            m_pLine->Set_Scale(m_vScale_Line);
            m_fLineScalingTime = 0.f;
            m_iLinePhase = 0;
        }
        else
        {
            m_vScale_Line.x += fTimeDelta * 1500.f;
            m_pLine->Set_Scale(m_vScale_Line);
        }

        if (0.f >= m_fShader_Line)
        {
            m_fShader_Line = 0.f;
            
        }
        else
        {
            m_fShader_Line -= fTimeDelta * 2.f;
        }
    }
}

void CUI_Indicator_InstKill::Set_InstKillRenderStart()
{
    m_bRenderSwitch = true;
}

void CUI_Indicator_InstKill::Set_InstKillRenderEnd()
{
    m_bRenderSwitch                     = false;
    m_bRenderDegree                     = false;

    m_bRenderSwitch                     = false;
    m_bRenderSwitch_ScalingIndicator    = false;
    m_fShader_Line                      = 1.f;
    m_vScale_Line.x                     = 900.f;
    m_pLine->Set_Scale(m_vScale_Line);
    
    m_fShader_Indicator                 = 0.f;
    m_vScale_Indicator                  = _float3(464.f, 464.f, 1.f);
    m_fShader_Indicator_Bloom           = 1.f;
    m_fShader_Indicator_Scaling         = 1.f;
    m_pIndicator->Set_Scale(m_vScale_Indicator);
    
    m_iTagIndex                         = 0;
    m_iTagNumber                        = 0;
    m_fShader_Tag                       = 0.f;
    m_fAccTagSetScaleTime               = 0.f;
    m_vScale_Tag                        = _float3(0.f, 0.f, 1.f);
    m_pTag->Set_Scale(m_vScale_Tag);
}

void CUI_Indicator_InstKill::Set_IndicatorPosition(_matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);


    _vector vPosition = XMVectorSet(0.f, 1.5f, 0.f, 1.f);

    /* World */
    vPosition = XMVector3TransformCoord(vPosition, _WorldMatrix);

    _vector vLookCam = MatCam.r[3] - vPosition ;

    _vector vCrossLookCam = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));

    vPosition = XMVector3TransformCoord(vPosition, ProjectedMatrix);


    //vPosition_L = XMVectorSetX(vPosition_L, XMVectorGetX(vPosition_L) - 22.f);
    //vPosition_R = XMVectorSetX(vPosition_R, XMVectorGetX(vPosition_R) + 22.f);

    _float fScreenX = XMVectorGetX(vPosition);
    _float fScreenY = XMVectorGetY(vPosition);

    m_vPosition_Line.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Line.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pLine->Set_State(TRANSFORM::POSITION, m_vPosition_Line);

    m_vPosition_Fillter_Square.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Fillter_Square.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pFillter_Square->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter_Square);

    m_vPosition_Fillter_Rhombus.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Fillter_Rhombus.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pFillter_Rhombus->Set_State(TRANSFORM::POSITION, m_vPosition_Fillter_Rhombus);

    m_vPosition_Indicator.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pIndicator->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_Bloom->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);
    m_pIndicator_Scaling->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator);

    m_vPosition_Tag.x = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Tag.y = (fScreenY) * 0.5f * g_iWinCY;
    m_pTag->Set_State(TRANSFORM::POSITION, m_vPosition_Tag);


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

HRESULT CUI_Indicator_InstKill::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Ready_Components", "Failed to CGameObject::Ready_Components");
    }

    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);

    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_InstKill::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

shared_ptr<CUI_Indicator_InstKill> CUI_Indicator_InstKill::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_InstKill> pInstance = make_private_shared(CUI_Indicator_InstKill, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_InstKill::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_InstKill::Clone(any aDesc)
{
    shared_ptr<CUI_Indicator_InstKill> pInstance = make_private_shared_copy(CUI_Indicator_InstKill, *this);

    if (FAILED(pInstance->Initialize(aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_InstKill::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
