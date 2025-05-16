#include "ClientPCH.h"
#include "UI_Indicator_Monster_Hp.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Camera_Player.h"

CUI_Indicator_Monster_Hp::CUI_Indicator_Monster_Hp(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_Indicator_Monster_Hp::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_Indicator_Monster_Hp::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Initialize", "Failed to CGameObject::Initialize");
    }


    Initialize_UI();


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_NoticeMark::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_Indicator_Monster_Hp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Indicator_Monster_Hp::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_4);

    Tick_UI(fTimeDelta);
}

HRESULT CUI_Indicator_Monster_Hp::Render()
{
    if (/*nullptr != m_pCamera_Player->Get_LockOnTransform() && true == m_pCamera_Player->Get_LockOn() || */0.f < m_fHp && m_fHp < m_fHpMax && true == m_bRenderDegree && false == m_bExecution)
    {
        /* Hp_Background */
        if (FAILED(m_pTexture_Indicator_Hp_Background->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_Hp_Background->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(__super::Render(1)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CUI_Indicator_Monster_Hp::Render");
        }

        ///* Hp_Back */
        //if (FAILED(m_pTexture_Indicator_Hp_Back->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        //{
        //    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
        //}
        //if (FAILED(m_pIndicator_Hp_Back->Bind_OnShader(m_pShader)))
        //{
        //    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_OnShader");
        //}
        //if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fRatio_Blank, sizeof(_float))))
        //{
        //    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Bind_RawValue", "Failed to CTexture::Bind_Raw_Value");
        //}
        //if (FAILED(__super::Render(43)))
        //{
        //    MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CUI_Indicator_Monster_Hp::Render");
        //}

        /* Hp_Front */
        if (FAILED(m_pTexture_Indicator_Hp_Front->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_ShaderResourceView");
        }
        if (FAILED(m_pIndicator_Hp_Front->Bind_OnShader(m_pShader)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CTexture::Bind_OnShader");
        }
        if (FAILED(m_pShader->Bind_RawValue("g_fRatio", &m_fRatio, sizeof(_float))))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Bind_RawValue", "Failed to CTexture::Bind_Raw_Value");
        }
        if (FAILED(__super::Render(42)))
        {
            MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Render", "Failed to CUI_Indicator_Monster_Hp::Render");
        }

        CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_strMonsterName, _float2(g_iWinCX * 0.5f + m_vPosition_RenderMonster_Name.x - 160.f * (3.f / m_fDistanceMonsterToCamera), g_iWinCY * 0.5f - m_vPosition_RenderMonster_Name.y - 55.f * (3.f / m_fDistanceMonsterToCamera)), XMVectorSet(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f), 3.f / m_fDistanceMonsterToCamera);
    }



    return S_OK;
}

HRESULT CUI_Indicator_Monster_Hp::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_Indicator_Monster_Hp::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_Indicator_Monster_Hp::Initialize_UI()
{
//================================================================================================ Transform
    m_pIndicator_Hp_Background          = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_Hp_Back                = CTransform::Create(m_pDevice, m_pContext);
    m_pIndicator_Hp_Front               = CTransform::Create(m_pDevice, m_pContext);
    
//================================================================================================ Texture
    m_pTexture_Indicator_Hp_Background  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpBackground.png"));
    m_pTexture_Indicator_Hp_Back        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpBack.png"));
    m_pTexture_Indicator_Hp_Front       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Indicator/Monster_HpFront.png"));

//================================================================================================ _bool
    m_bDamageEnd                        = false;
    m_bIsFinishCalculateDamage          = false;
    m_bRenderDegree                     = false;
    m_bExecution                        = false;
//================================================================================================ _float(Time)
    m_fTime                             = 0.f;
    m_fDamageDelayTime                  = 0.f;

//================================================================================================ _float(HP)
    m_fHp                               = 10.f;
    m_fHpMax                            = 10.f;
    m_fRatio                            = m_fHp / m_fHpMax;
    m_fPercentage                       = m_fRatio * 160.f;
    m_fPrevHP                           = m_fHp;

    m_fHpBlank                          = 10.f;
    m_fHpBlankMax                       = 10.f;
    m_fRatio_Blank                      = m_fHpBlank / m_fHpBlankMax;
    m_fPercentage_Blank                 = m_fRatio_Blank * 160.f;

//================================================================================================ _float3
    m_vScale_Indicator_Hp_Background    = _float3(m_fPercentage, 8.f, 1.f);
    m_vScale_Indicator_Hp_Back          = _float3(m_fPercentage_Blank, 10.f, 1.f);
    m_vScale_Indicator_Hp_Front         = _float3(m_fPercentage, 10.f, 1.f);

//================================================================================================ _float4
    m_vPosition_Indicator_Hp_Background = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_Hp_Back       = _float4(0.f, 0.f, 0.f, 1.f);
    m_vPosition_Indicator_Hp_Front      = _float4(0.f, 0.f, 0.f, 1.f);

//================================================================================================ Position
    m_pIndicator_Hp_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Background);
    m_pIndicator_Hp_Back->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Back);
    m_pIndicator_Hp_Front->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Front);

//================================================================================================ Scale
    m_pIndicator_Hp_Background->Set_Scale(m_vScale_Indicator_Hp_Background);
    m_pIndicator_Hp_Back->Set_Scale(m_vScale_Indicator_Hp_Back);
    m_pIndicator_Hp_Front->Set_Scale(m_vScale_Indicator_Hp_Front);

//================================================================================================ Scale
    m_strMonsterName                    = TEXT("Å×½ºÆ®");
    m_fDistanceMonsterToCamera          = 0.f;
}

void CUI_Indicator_Monster_Hp::Tick_UI(_float fTimeDelta)
{
    LockOn(fTimeDelta);
    //Calculate_Damage_System(fTimeDelta);
    //Set_DamageToMonster();
}

void CUI_Indicator_Monster_Hp::LockOn(_float fTimeDelta)
{
    shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_CAMERA);
    m_pCamera_Player = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

    if (nullptr == m_pCamera_Player)
    {
        return;
    }
    else
    {
        if (nullptr != m_pCamera_Player->Get_LockOnTransform() && true == m_pCamera_Player->Get_LockOn())
        {
            auto pTransform = m_pCamera_Player->Get_LockOnTransform();
            Set_IndicatorPosition(m_pCamera_Player->Get_State(TRANSFORM::POSITION), pTransform->Get_WorldMatrix());
        }
    }
}

void CUI_Indicator_Monster_Hp::Set_MonsterHp(_float fHp, _float fHpMax)
{
    m_fHp                           = fHp;
    m_fHpMax                        = fHpMax;
    m_fHpBlank                      = fHp;
    m_fHpBlankMax                   = fHpMax;

    m_fRatio                        = m_fHp         / m_fHpMax;
    m_fRatio_Blank                  = m_fHpBlank    / m_fHpBlankMax;

    m_fPercentage                   = m_fRatio * 160.f;
    m_fPercentage_Blank             = m_fRatio_Blank * 160.f;

    //m_vScale_Indicator_Hp_Front.x   = m_fPercentage;
    //m_vScale_Indicator_Hp_Back.x    = m_fPercentage_Blank;

    //m_pIndicator_Hp_Front->Set_Scale(m_vScale_Indicator_Hp_Front);
    //m_pIndicator_Hp_Back->Set_Scale(m_vScale_Indicator_Hp_Back);
}                                                         

void CUI_Indicator_Monster_Hp::Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix)
{
    _matrix ProjectedMatrix = _WorldMatrix * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW)
        * CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);

    _matrix MatCam = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::WORLD);

    _vector vPosition = XMVectorSet(0.f, 2.5f, 0.f, 1.f);

    _vector vPositionCoord = XMVector3TransformCoord(vPosition, ProjectedMatrix);

//===========================================================================================================================
    _float fAspectRatio                     = (_float)g_iWinCX / (_float)g_iWinCY;
    
    // Look Vector Camera ---> Monster
    _vector vLookCam                        = MatCam.r[3] - vPositionCoord;

    _vector vCrossLookCamY                  = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookCam));
    _vector vCrossLookCamX                  = XMVector3Normalize(XMVector3Cross(vCrossLookCamY, vLookCam));

//===========================================================================================================================
    _float fScreenX                         = XMVectorGetX(vPositionCoord);
    _float fScreenY                         = XMVectorGetY(vPositionCoord);

//===========================================================================================================================
    m_vPosition_Indicator_Hp_Background.x   = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_Hp_Background.y   = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_Hp_Back.x         = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_Hp_Back.y         = (fScreenY) * 0.5f * g_iWinCY;

    m_vPosition_Indicator_Hp_Front.x        = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_Indicator_Hp_Front.y        = (fScreenY) * 0.5f * g_iWinCY;
    
    //m_vPosition_RenderMonster_Name.x = (fScreenX) * 0.5f * g_iWinCX;
    //m_vPosition_RenderMonster_Name.y = (fScreenY) * 0.5f * g_iWinCY-1.f;
    /* Position - RenderFont Monster Name */
    m_vPosition_RenderMonster_Name.x        = (fScreenX) * 0.5f * g_iWinCX;
    m_vPosition_RenderMonster_Name.y        = (fScreenY) * 0.5f * g_iWinCY;

//===========================================================================================================================
    m_pIndicator_Hp_Background->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Background);
    m_pIndicator_Hp_Back->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Back);
    m_pIndicator_Hp_Front->Set_State(TRANSFORM::POSITION, m_vPosition_Indicator_Hp_Front);

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

    m_fDistanceMonsterToCamera = XMVectorGetX(XMVector3Length(_WorldMatrix.r[3] - MatCam.r[3]));
}

void CUI_Indicator_Monster_Hp::Set_MonsterNameTag(wstring _strMonsterNameTag)
{
    m_strMonsterName = _strMonsterNameTag;
}

void CUI_Indicator_Monster_Hp::Set_MonsterNameTagRemove_Execution()
{
    m_bExecution = true;
}

void CUI_Indicator_Monster_Hp::Set_DamageToMonster()
{
    if (0 < m_fHp)
    {
        /* Situation :: When Monster Damaged */
        if (m_fPrevHP != m_fHp)
        {
            m_bIsFinishCalculateDamage = false;
        }
    }
}

void CUI_Indicator_Monster_Hp::Calculate_Damage_System(_float fTimeDelta)
{
    if (false == m_bIsFinishCalculateDamage)
    {
        m_fDamageDelayTime += fTimeDelta;

        if (1.f <= m_fDamageDelayTime)
        {
            if (m_fHp < m_fHpBlank)
            {
                m_fTime += fTimeDelta;
            }
            else
            {
                m_bIsFinishCalculateDamage = true;
                m_fTime = 0.f;
            }
        }

    }
    else
    {
        m_fDamageDelayTime = 0.f;
    }
}

shared_ptr<CUI_Indicator_Monster_Hp> CUI_Indicator_Monster_Hp::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_Indicator_Monster_Hp> pInstance = make_private_shared(CUI_Indicator_Monster_Hp, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_Hp::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_Indicator_Monster_Hp::Clone(any _aDesc)
{
    shared_ptr<CUI_Indicator_Monster_Hp> pInstance = make_private_shared_copy(CUI_Indicator_Monster_Hp, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_Indicator_Monster_Hp::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
