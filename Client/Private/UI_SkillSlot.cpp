#include "ClientPCH.h"
#include "UI_SkillSlot.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player.h"

#define SKILLSLOT_SCALE        _float3(320.f, 320.f, 0.f)
#define SKILLSLOT_POSITION     _float4(720.f, -340.f, 0.f, 1.f)

#define BLINK_INTENSITY         _float(0.3f)

CUI_SkillSlot::CUI_SkillSlot(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CUI_SkillSlot::Initialize_Prototype()
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

    m_umapComponentArg[COMPONENT::RENDERER]         = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER]           = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
    m_umapComponentArg[COMPONENT::VIBUFFER_RECT]    = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

    return S_OK;
}

HRESULT CUI_SkillSlot::Initialize(any _aDesc)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillSlot::Initialize", "Failed to CGameObject::Initialize");
    }

    if (_aDesc.has_value())
    {
        //m_pEntityDesc = any_cast<PLAYERENTITYDESC*>(_pPlayerEntityDesc);
    }

    m_pSkillSlot                    = CTransform::Create(m_pDevice, m_pContext);

    m_pSkill_ElectricOff                = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_AccelerationOff            = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_CloneOff                   = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_StealthOff                 = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_ElectricOn                 = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_AccelerationOn             = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_CloneOn                    = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_StealthOn                  = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_Cool_Electric              = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_Cool_Acceleration          = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_Cool_Clone                 = CTransform::Create(m_pDevice, m_pContext);
    m_pSkill_Cool_Stealth               = CTransform::Create(m_pDevice, m_pContext);

    m_pTexture                          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillSlot.png"));
    m_pTexture_ElectricOff              = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ElectricOff.png"));
    m_pTexture_AccelerationOff          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/AccelerationOff.png"));
    m_pTexture_CloneOff                 = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/CloneOff.png"));
    m_pTexture_StealthOff               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/StealthOff.png"));
    m_pTexture_ElectricOn               = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/ElectricOn.png"));
    m_pTexture_AccelerationOn           = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/AccelerationOn.png"));
    m_pTexture_CloneOn                  = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/CloneOn.png"));
    m_pTexture_StealthOn                = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/StealthOn.png"));
    m_pTexture_SkillCool_Electric       = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillCoolOff.png"), true);
    m_pTexture_SkillCool_Acceleration   = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillCoolOff.png"), true);
    m_pTexture_SkillCool_Clone          = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillCoolOff.png"), true);
    m_pTexture_SkillCool_Stealth        = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Skill/SkillCoolOff.png"), true);
    
    Initialize_UI();
    
    _matrix pivotMatrix = XMMatrixIdentity();
    pivotMatrix = XMMatrixRotationZ(XMConvertToRadians(90.f));
    m_pSkill_Cool_Electric->Set_Matrix(pivotMatrix);
    m_pSkill_Cool_Acceleration->Set_Matrix(pivotMatrix);
    m_pSkill_Cool_Clone->Set_Matrix(pivotMatrix);
    m_pSkill_Cool_Stealth->Set_Matrix(pivotMatrix);

    m_pSkillSlot->Set_Scale(SKILLSLOT_SCALE);
    m_pSkill_ElectricOff->Set_Scale(m_vScale_Electric_Off);
    m_pSkill_AccelerationOff->Set_Scale(m_vScale_Acceleration_Off);
    m_pSkill_CloneOff->Set_Scale(m_vScale_Clone_Off);
    m_pSkill_StealthOff->Set_Scale(m_vScale_Stealth_Off);
    m_pSkill_ElectricOn->Set_Scale(m_vScale_Electric_On);
    m_pSkill_AccelerationOn->Set_Scale(m_vScale_Acceleration_On);
    m_pSkill_CloneOn->Set_Scale(m_vScale_Clone_On);
    m_pSkill_StealthOn->Set_Scale(m_vScale_Stealth_On);
    m_pSkill_Cool_Electric->Set_Scale(m_vScale_SkillCool_Electric);
    m_pSkill_Cool_Acceleration->Set_Scale(m_vScale_SkillCool_Acceleration);
    m_pSkill_Cool_Clone->Set_Scale(m_vScale_SkillCool_Clone);
    m_pSkill_Cool_Stealth->Set_Scale(m_vScale_SkillCool_Stealth);

    m_pSkillSlot->Set_State(TRANSFORM::POSITION, SKILLSLOT_POSITION);
    m_pSkill_ElectricOff->Set_State(TRANSFORM::POSITION,m_vPosition_Electric_Off);
    m_pSkill_AccelerationOff->Set_State(TRANSFORM::POSITION,m_vPosition_Acceleration_Off);
    m_pSkill_CloneOff->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_Off);
    m_pSkill_StealthOff->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_Off);
    m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
    m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
    m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
    m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
    m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
    m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
    m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
    m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);


    m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
    if (nullptr == m_pVIBuffer_Rect)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillSlot::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
    }

    return S_OK;
}

void CUI_SkillSlot::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_SkillSlot::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

    Add_RenderObject(UI_LAYER::LAYER_0);

    if (false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene() && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
    {
        Blink_Skill_Icon_Electric(fTimeDelta);
        Blink_Skill_Icon_Acceleration(fTimeDelta);
        Blink_Skill_Icon_Clone(fTimeDelta);
        Blink_Skill_Icon_Stealth(fTimeDelta);

        UIBounce_System(fTimeDelta);
    }
}

HRESULT CUI_SkillSlot::Render()
{
    if (SCENE::TEST != CGameInstance::Get_Instance()->Current_Scene())
    {
        if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI()
            && false == CUI_Manager::Get_Instance()->Get_UI_All_RenderStop()
            && false == CUI_Manager::Get_Instance()->Get_Enter_ShopUI()
            && false == CUI_Manager::Get_Instance()->Get_MindRoomCutScene()
            && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene()
            && false == CUI_Manager::Get_Instance()->Get_InstKillCutScene())
        {
#pragma region Always Rendering
            /* SkillSlot */
            if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkillSlot->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
            }

            /* Electric Off */
            if (FAILED(m_pTexture_ElectricOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkill_ElectricOff->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
            }

            /* Acceleration Off */
            if (FAILED(m_pTexture_AccelerationOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkill_AccelerationOff->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
            }

            /* Clone Off */
            if (FAILED(m_pTexture_CloneOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkill_CloneOff->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
            }

            /* Invisible Off */
            if (FAILED(m_pTexture_StealthOff->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
            }
            if (FAILED(m_pSkill_StealthOff->Bind_OnShader(m_pShader)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
            }
            if (FAILED(__super::Render(5)))
            {
                MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
            }

#pragma endregion

#pragma region Conditional Rendering
#pragma region _Rendering Electric Skill Icon
            /* 1. Skill Done */
            if (false == m_bCanActivate_Electric && true == m_bCanChargeGauge_Electric && 0 == m_fRatio_Electric)
            {
                /* No Render */
            }
            /* 2. Skill Charge */
            if (false == m_bCanActivate_Electric && true == m_bCanChargeGauge_Electric && 0 != m_fRatio_Electric)
            {
                /* Render Only Skill_Cool */
                /* Cool Gauge - Up */
                if (FAILED(m_pTexture_SkillCool_Electric->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Electric->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Electric", &m_fRatio_Electric, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(16)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 3. Full Gauge */
            if (true == m_bCanActivate_Electric && false == m_bCanChargeGauge_Electric || true == m_bRenderIcon_Electric)
            {
                /* Render Skill_Icon & Skill_Cool */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_ElectricOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_ElectricOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Electric->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Electric->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(16)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 4. Skill Activate */
            if (false == m_bCanActivate_Electric && false == m_bCanChargeGauge_Electric)
            {
                /* Render Skill Icon & Skill_Cool */
                /* Cool Gauge - Down */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_ElectricOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_ElectricOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime_Electric", &m_fBlink_Electric, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(15)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Electric->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Electric->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Electric", &m_fRatio_Electric, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(16)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region _Rendering Acceleration Skill Icon
            /* 1. Skill Done */
            if (false == m_bCanActivate_Acceleration && true == m_bCanChargeGauge_Acceleration && 0 == m_fRatio_Acceleration)
            {
                /* No Render */
            }
            /* 2. Skill Charge */
            if (false == m_bCanActivate_Acceleration && true == m_bCanChargeGauge_Acceleration && 0 != m_fRatio_Acceleration)
            {
                /* Render Only Skill_Cool */
                /* Cool Gauge - Up */
                if (FAILED(m_pTexture_SkillCool_Acceleration->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Acceleration->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Acceleration", &m_fRatio_Acceleration, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(17)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 3. Full Gauge */
            if (true == m_bCanActivate_Acceleration && false == m_bCanChargeGauge_Acceleration || true == m_bRenderIcon_Acceleration)
            {
                /* Render Skill_Icon & Skill_Cool */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_AccelerationOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_AccelerationOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Acceleration->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Acceleration->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(17)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 4. Skill Activate */
            if (false == m_bCanActivate_Acceleration && false == m_bCanChargeGauge_Acceleration)
            {
                /* Render Skill Icon & Skill_Cool */
                /* Cool Gauge - Down */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_AccelerationOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_AccelerationOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime_Acceleration", &m_fBlink_Acceleration, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(21)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Acceleration->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Acceleration->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Acceleration", &m_fRatio_Acceleration, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(17)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region _Rendering Clone Skill Icon
            /* 1. Skill Done */
            if (false == m_bCanActivate_Clone && true == m_bCanChargeGauge_Clone && 0 == m_fRatio_Clone)
            {
                /* No Render */
            }
            /* 2. Skill Charge */
            if (false == m_bCanActivate_Clone && true == m_bCanChargeGauge_Clone && 0 != m_fRatio_Clone)
            {
                /* Render Only Skill_Cool */
                /* Cool Gauge - Up */
                if (FAILED(m_pTexture_SkillCool_Clone->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Clone->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Clone", &m_fRatio_Clone, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(18)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 3. Full Gauge */
            if (true == m_bCanActivate_Clone && false == m_bCanChargeGauge_Clone || true == m_bRenderIcon_Replication)
            {
                /* Render Skill_Icon & Skill_Cool */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_CloneOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_CloneOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Clone->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Clone->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(18)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 4. Skill Activate */
            if (false == m_bCanActivate_Clone && false == m_bCanChargeGauge_Clone)
            {
                /* Render Skill Icon & Skill_Cool */
                /* Cool Gauge - Down */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_CloneOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_CloneOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime_Clone", &m_fBlink_Clone, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(22)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Clone->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Clone->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Clone", &m_fRatio_Clone, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(18)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma region _Rendering Stealth Skill Icon
            /* 1. Skill Done */
            if (false == m_bCanActivate_Stealth && true == m_bCanChargeGauge_Stealth && 0 == m_fRatio_Stealth)
            {
                /* No Render */
            }
            /* 2. Skill Charge */
            if (false == m_bCanActivate_Stealth && true == m_bCanChargeGauge_Stealth && 0 != m_fRatio_Stealth)
            {
                /* Render Only Skill_Cool */
                /* Cool Gauge - Up */
                if (FAILED(m_pTexture_SkillCool_Stealth->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Stealth->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Stealth", &m_fRatio_Stealth, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(19)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 3. Full Gauge */
            if (true == m_bCanActivate_Stealth && false == m_bCanChargeGauge_Stealth || true == m_bRenderIcon_Replication)
            {
                /* Render Skill_Icon & Skill_Cool */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_StealthOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_StealthOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(1)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Stealth->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Stealth->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(__super::Render(19)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
            /* 4. Skill Activate */
            if (false == m_bCanActivate_Stealth && false == m_bCanChargeGauge_Stealth)
            {
                /* Render Skill Icon & Skill_Cool */
                /* Cool Gauge - Down */
                /* 1. Skill_Icon */
                if (FAILED(m_pTexture_StealthOn->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_StealthOn->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fTime_Stealth", &m_fBlink_Stealth, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(23)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }

                /* 2. Skill_Cool */
                if (FAILED(m_pTexture_SkillCool_Stealth->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "g_texDiffuse")))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_ShaderResourceView");
                }
                if (FAILED(m_pSkill_Cool_Stealth->Bind_OnShader(m_pShader)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_OnShader");
                }
                if (FAILED(m_pShader->Bind_RawValue("g_fGauge_Stealth", &m_fRatio_Stealth, sizeof(_float))))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CTexture::Bind_RawValue");
                }
                if (FAILED(__super::Render(19)))
                {
                    MSG_RETURN(E_FAIL, "CUI_SkillSlot::Render", "Failed to CGameObject::Render");
                }
            }
#pragma endregion
#pragma endregion
        }
    }
    return S_OK;
}

HRESULT CUI_SkillSlot::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CUI_SkillSlot::Ready_Components", "Failed to CGameObject::Ready_Components");
    }
    m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
    if (nullptr == m_pShader)
    {
        MSG_RETURN(E_FAIL, "CUI_SkillSlot::Ready_Components", "Failed to Get_Component: SHADER");
    }

    return S_OK;
}

void CUI_SkillSlot::Initialize_UI()
{
////////////////////////////////////////////////////////////////////////////////// _bool
// For. Activate Check
    m_bCanActivate_Electric                 = false;
    m_bCanActivate_Acceleration             = false;
    m_bCanActivate_Clone                    = false;
    m_bCanActivate_Stealth                  = false;

// For. ChargeGauge Check
    m_bCanChargeGauge_Electric              = true;
    m_bCanChargeGauge_Acceleration          = true;
    m_bCanChargeGauge_Clone                 = true;
    m_bCanChargeGauge_Stealth               = true;

// For. RenderIcon Check
    m_bRenderIcon_Electric                  = false;


////////////////////////////////////////////////////////////////////////////////// _float
// For. Blink
    m_fBlink_Electric                       = 0.f;
    m_fBlink_Acceleration                   = 0.f;
    m_fBlink_Clone                          = 0.f;
    m_fBlink_Stealth                        = 0.f;
    m_fBlinkIndex                           = 0.f;

    m_fBlinkTime_Electric                   = 0.f;
    m_fBlinkTime_Acceleration               = 0.f;
    m_fBlinkTime_Clone                      = 0.f;
    m_fBlinkTime_Stealth                    = 0.f;

    m_fBlinkIndex_Electric                  = 0.f;
    m_fBlinkIndex_Acceleration              = 0.f;
    m_fBlinkIndex_Clone                     = 0.f;
    m_fBlinkIndex_Stealth                   = 0.f;
    

// Set Gauge Ratio
    m_fGauge_Electric                       = 0.f;
    m_fGauge_Acceleration                   = 0.f;
    m_fGauge_Clone                          = 0.f;
    m_fGauge_Stealth                        = 0.f;

    m_fGaugeMax_Electric                    = 100.f;
    m_fGaugeMax_Acceleration                = 100.f;
    m_fGaugeMax_Clone                       = 100.f;
    m_fGaugeMax_Stealth                     = 100.f;

    m_fRatio_Electric                       = m_fGauge_Electric     / m_fGaugeMax_Electric;
    m_fRatio_Acceleration                   = m_fGauge_Acceleration / m_fGaugeMax_Acceleration;
    m_fRatio_Clone                          = m_fGauge_Clone        / m_fGaugeMax_Clone;
    m_fRatio_Stealth                        = m_fGauge_Stealth      / m_fGaugeMax_Stealth;

////////////////////////////////////////////////////////////////////////////////// _float3
    m_vScale_Electric_Off                   = _float3(36.f, 50.f, 1.f);
    m_vScale_Acceleration_Off               = _float3(36.f, 50.f, 1.f);
    m_vScale_Clone_Off                      = _float3(44.f, 50.f, 1.f);
    m_vScale_Stealth_Off                    = _float3(44.f, 50.f, 1.f);
    
    m_vScale_Electric_On                    = _float3(72.f, 72.f, 1.f);
    m_vScale_Acceleration_On                = _float3(62.f, 72.f, 1.f);
    m_vScale_Clone_On                       = _float3(56.f, 72.f, 1.f);
    m_vScale_Stealth_On                     = _float3(64.f, 72.f, 1.f);
    
    m_vScale_SkillCool_Electric             = _float3(100.f, 100.f, 1.f);
    m_vScale_SkillCool_Acceleration         = _float3(100.f, 100.f, 1.f);
    m_vScale_SkillCool_Clone                = _float3(100.f, 100.f, 1.f);
    m_vScale_SkillCool_Stealth              = _float3(100.f, 100.f, 1.f);

////////////////////////////////////////////////////////////////////////////////// _float3
    m_vPosition_Electric_Off                = _float4(810.f, -345.f, 0.f, 1.f);
    m_vPosition_Acceleration_Off            = _float4(732.f, -425.f, 0.f, 1.f);
    m_vPosition_Clone_Off                   = _float4(634.f, -335.f, 0.f, 1.f);
    m_vPosition_Stealth_Off                 = _float4(712.f, -255.f, 0.f, 1.f);

    m_vPosition_Electric_On                 = _float4(809.f, -344.f, 0.f, 1.f);
    m_vPosition_Acceleration_On             = _float4(730.f, -425.f, 0.f, 1.f);
    m_vPosition_Clone_On                    = _float4(634.f, -334.f, 0.f, 1.f);
    m_vPosition_Stealth_On                  = _float4(712.f, -255.f, 0.f, 1.f);

    m_vPosition_SkillCool_Electric          = _float4(809.f, -344.5f, 0.f, 1.f);
    m_vPosition_SkillCool_Acceleration      = _float4(726.5f, -423.f, 0.f, 1.f);
    m_vPosition_SkillCool_Clone             = _float4(634.f, -336.f, 0.f, 1.f);
    m_vPosition_SkillCool_Stealth           = _float4(717.f, -255.f, 0.f, 1.f);

    
    /* For. MakeBound */
#pragma region Bounce_Icon
    m_bLeft                                 = false;
    m_bRight                                = false;
    m_bTop                                  = false;
    m_bBottom                               = false;
    m_bUIBounce                             = false;

    // Electric
    m_fOriginalPos_ElectricX                = m_vPosition_Electric_On.x;
    m_fOriginalPos_ElectricY                = m_vPosition_Electric_On.y;
    m_fMovedPosition_ElectricX_Left         = m_vPosition_Electric_On.x + 10.f;
    m_fMovedPosition_ElectricX_Right        = m_vPosition_Electric_On.x - 10.f;
    m_fMovedPosition_ElectricY_Top          = m_vPosition_Electric_On.y - 10.f;
    m_fMovedPosition_ElectricY_Bottom       = m_vPosition_Electric_On.y + 10.f;

    // Acceleration
    m_fOriginalPos_AccelerationX            = m_vPosition_Acceleration_On.x;
    m_fOriginalPos_AccelerationY            = m_vPosition_Acceleration_On.y;
    m_fMovedPosition_AccelerationX_Left     = m_vPosition_Acceleration_On.x + 10.f;
    m_fMovedPosition_AccelerationX_Right    = m_vPosition_Acceleration_On.x - 10.f;
    m_fMovedPosition_AccelerationY_Top      = m_vPosition_Acceleration_On.y - 10.f;
    m_fMovedPosition_AccelerationY_Bottom   = m_vPosition_Acceleration_On.y + 10.f;

    // Clone
    m_fOriginalPos_CloneX                   = m_vPosition_Clone_On.x;
    m_fOriginalPos_CloneY                   = m_vPosition_Clone_On.y;
    m_fMovedPosition_CloneX_Left            = m_vPosition_Clone_On.x + 10.f;
    m_fMovedPosition_CloneX_Right           = m_vPosition_Clone_On.x - 10.f;
    m_fMovedPosition_CloneY_Top             = m_vPosition_Clone_On.y - 10.f;
    m_fMovedPosition_CloneY_Bottom          = m_vPosition_Clone_On.y + 10.f;

    // Stealth
    m_fOriginalPos_StealthX                 = m_vPosition_Stealth_On.x;
    m_fOriginalPos_StealthY                 = m_vPosition_Stealth_On.y;
    m_fMovedPosition_StealthX_Left          = m_vPosition_Stealth_On.x + 10.f;
    m_fMovedPosition_StealthX_Right         = m_vPosition_Stealth_On.x - 10.f;
    m_fMovedPosition_StealthY_Top           = m_vPosition_Stealth_On.y - 10.f;
    m_fMovedPosition_StealthY_Bottom        = m_vPosition_Stealth_On.y + 10.f;
#pragma endregion

#pragma region Bounce_SkillCool
    // Electric
    m_fOriginalPos_ElectricCoolX                = m_vPosition_SkillCool_Electric.x;
    m_fOriginalPos_ElectricCoolY                = m_vPosition_SkillCool_Electric.y;
    m_fMovedPosition_ElectricCoolX_Left         = m_vPosition_SkillCool_Electric.x + 10.f;
    m_fMovedPosition_ElectricCoolX_Right        = m_vPosition_SkillCool_Electric.x - 10.f;
    m_fMovedPosition_ElectricCoolY_Top          = m_vPosition_SkillCool_Electric.y - 10.f;
    m_fMovedPosition_ElectricCoolY_Bottom       = m_vPosition_SkillCool_Electric.y + 10.f;

    // Acceleration
    m_fOriginalPos_AccelerationCoolX            = m_vPosition_SkillCool_Acceleration.x;
    m_fOriginalPos_AccelerationCoolY            = m_vPosition_SkillCool_Acceleration.y;
    m_fMovedPosition_AccelerationCoolX_Left     = m_vPosition_SkillCool_Acceleration.x + 10.f;
    m_fMovedPosition_AccelerationCoolX_Right    = m_vPosition_SkillCool_Acceleration.x - 10.f;
    m_fMovedPosition_AccelerationCoolY_Top      = m_vPosition_SkillCool_Acceleration.y - 10.f;
    m_fMovedPosition_AccelerationCoolY_Bottom   = m_vPosition_SkillCool_Acceleration.y + 10.f;

    // Clone
    m_fOriginalPos_CloneCoolX                   = m_vPosition_SkillCool_Clone.x;
    m_fOriginalPos_CloneCoolY                   = m_vPosition_SkillCool_Clone.y;
    m_fMovedPosition_CloneCoolX_Left            = m_vPosition_SkillCool_Clone.x + 10.f;
    m_fMovedPosition_CloneCoolX_Right           = m_vPosition_SkillCool_Clone.x - 10.f;
    m_fMovedPosition_CloneCoolY_Top             = m_vPosition_SkillCool_Clone.y - 10.f;
    m_fMovedPosition_CloneCoolY_Bottom          = m_vPosition_SkillCool_Clone.y + 10.f;

    // Stealth
    m_fOriginalPos_StealthCoolX                 = m_vPosition_SkillCool_Stealth.x;
    m_fOriginalPos_StealthCoolY                 = m_vPosition_SkillCool_Stealth.y;
    m_fMovedPosition_StealthCoolX_Left          = m_vPosition_SkillCool_Stealth.x + 10.f;
    m_fMovedPosition_StealthCoolX_Right         = m_vPosition_SkillCool_Stealth.x - 10.f;
    m_fMovedPosition_StealthCoolY_Top           = m_vPosition_SkillCool_Stealth.y - 10.f;
    m_fMovedPosition_StealthCoolY_Bottom        = m_vPosition_SkillCool_Stealth.y + 10.f;
#pragma endregion

#pragma region Bounce_Position_Reset
    m_vPrevPosition_Electric_Icon               = m_vPosition_Electric_On;
    m_vPrevPosition_Acceleration_Icon           = m_vPosition_Acceleration_On;
    m_vPrevPosition_Clone_Icon                  = m_vPosition_Clone_On;
    m_vPrevPosition_Stealth_Icon                = m_vPosition_Stealth_On;

    m_vPrevPosition_Electric_SkillCool          = m_vPosition_SkillCool_Electric;
    m_vPrevPosition_Acceleration_SkillCool      = m_vPosition_SkillCool_Acceleration;
    m_vPrevPosition_Clone_SkillCool             = m_vPosition_SkillCool_Clone;
    m_vPrevPosition_Stealth_SkillCool           = m_vPosition_SkillCool_Stealth;

#pragma endregion

}

void CUI_SkillSlot::UIBounce_System(_float fTimeDelta)
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
            if (-80 > m_ptCursorMove.x)
            {
                // Icon
                m_vPosition_Electric_On.x       += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Acceleration_On.x   += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Clone_On.x          += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Stealth_On.x        += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);

                // SkillCool
                m_vPosition_SkillCool_Electric.x       += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Acceleration.x   += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Clone.x          += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Stealth.x        += fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                
                // Icon
                if (m_vPosition_Electric_On.x > m_fMovedPosition_ElectricX_Left)
                {
                    m_vPosition_Electric_On.x = m_fMovedPosition_ElectricX_Left;
                    m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                }
                if (m_vPosition_Acceleration_On.x > m_fMovedPosition_AccelerationX_Left)
                {
                    m_vPosition_Acceleration_On.x = m_fMovedPosition_AccelerationX_Left;
                    m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                }
                if (m_vPosition_Clone_On.x > m_fMovedPosition_CloneX_Left)
                {
                    m_vPosition_Clone_On.x = m_fMovedPosition_CloneX_Left;
                    m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                }
                if (m_vPosition_Stealth_On.x > m_fMovedPosition_StealthX_Left)
                {
                    m_vPosition_Stealth_On.x = m_fMovedPosition_StealthX_Left;
                    m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
                }

                // SkillCool
                if (m_vPosition_SkillCool_Electric.x > m_fMovedPosition_ElectricCoolX_Left)
                {
                    m_vPosition_SkillCool_Electric.x = m_fMovedPosition_ElectricCoolX_Left;
                    m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                }
                if (m_vPosition_SkillCool_Acceleration.x > m_fMovedPosition_AccelerationCoolX_Left)
                {
                    m_vPosition_SkillCool_Acceleration.x = m_fMovedPosition_AccelerationCoolX_Left;
                    m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                }
                if (m_vPosition_SkillCool_Clone.x > m_fMovedPosition_CloneX_Left)
                {
                    m_vPosition_SkillCool_Clone.x = m_fMovedPosition_CloneX_Left;
                    m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                }
                if (m_vPosition_SkillCool_Stealth.x > m_fMovedPosition_StealthCoolX_Left)
                {
                    m_vPosition_SkillCool_Stealth.x = m_fMovedPosition_StealthCoolX_Left;
                    m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                }
            }
        }
        else
        {
            // Icon
            m_vPosition_Electric_On.x       -= fTimeDelta * 30.f;
            m_vPosition_Acceleration_On.x   -= fTimeDelta * 30.f;
            m_vPosition_Clone_On.x          -= fTimeDelta * 30.f;
            m_vPosition_Stealth_On.x        -= fTimeDelta * 30.f;
            m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);

            // SkillCool
            m_vPosition_SkillCool_Electric.x       -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Acceleration.x   -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Clone.x          -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Stealth.x        -= fTimeDelta * 30.f;
            m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);

            // Icon
            if (m_vPosition_Electric_On.x < m_fOriginalPos_ElectricX)
            {
                m_vPosition_Electric_On.x = m_fOriginalPos_ElectricX;
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            }
            if (m_vPosition_Acceleration_On.x < m_fOriginalPos_AccelerationX)
            {
                m_vPosition_Acceleration_On.x = m_fOriginalPos_AccelerationX;
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            }
            if (m_vPosition_Clone_On.x < m_fOriginalPos_CloneX)
            {
                m_vPosition_Clone_On.x = m_fOriginalPos_CloneX;
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            }
            if (m_vPosition_Stealth_On.x < m_fOriginalPos_StealthX)
            {
                m_vPosition_Stealth_On.x = m_fOriginalPos_StealthX;
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
            }

            // SkillCool
            if (m_vPosition_SkillCool_Electric.x < m_fOriginalPos_ElectricCoolX)
            {
                m_vPosition_SkillCool_Electric.x = m_fOriginalPos_ElectricCoolX;
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            }
            if (m_vPosition_SkillCool_Acceleration.x < m_fOriginalPos_AccelerationCoolX)
            {
                m_vPosition_SkillCool_Acceleration.x = m_fOriginalPos_AccelerationCoolX;
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            }
            if (m_vPosition_SkillCool_Clone.x < m_fOriginalPos_CloneCoolX)
            {
                m_vPosition_SkillCool_Clone.x = m_fOriginalPos_CloneCoolX;
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            }
            if (m_vPosition_SkillCool_Stealth.x < m_fOriginalPos_StealthCoolX)
            {
                m_vPosition_SkillCool_Stealth.x = m_fOriginalPos_StealthCoolX;
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);

                m_bLeft = false;
            }
        }
    }
    // Right
    else if (true == m_bRight)
    {
        if (0 != m_ptCursorMove.x)
        {
            if (80 < m_ptCursorMove.x)
            {
                // Icon
                m_vPosition_Electric_On.x       -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Acceleration_On.x   -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Clone_On.x          -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_Stealth_On.x        -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
               // SkillCool
                m_vPosition_SkillCool_Electric.x       -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Acceleration.x   -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Clone.x          -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_vPosition_SkillCool_Stealth.x        -= fTimeDelta * abs(m_ptCursorMove.x * 0.1f);
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                // Icon
                if (m_vPosition_Electric_On.x < m_fMovedPosition_ElectricX_Right)
                {
                    m_vPosition_Electric_On.x       = m_fMovedPosition_ElectricX_Right;
                    m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                }
                if (m_vPosition_Acceleration_On.x < m_fMovedPosition_AccelerationX_Right)
                {
                    m_vPosition_Acceleration_On.x = m_fMovedPosition_AccelerationX_Right;
                    m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                }
                if (m_vPosition_Clone_On.x < m_fMovedPosition_CloneX_Right)
                {
                    m_vPosition_Clone_On.x = m_fMovedPosition_CloneX_Right;
                    m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                }
                if (m_vPosition_Stealth_On.x < m_fMovedPosition_StealthX_Right)
                {
                    m_vPosition_Stealth_On.x = m_fMovedPosition_StealthX_Right;
                    m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
                }
               // SkillCool
                if (m_vPosition_SkillCool_Electric.x < m_fMovedPosition_ElectricCoolX_Right)
                {
                    m_vPosition_SkillCool_Electric.x = m_fMovedPosition_ElectricCoolX_Right;
                    m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                }
                if (m_vPosition_SkillCool_Acceleration.x < m_fMovedPosition_AccelerationCoolX_Right)
                {
                    m_vPosition_SkillCool_Acceleration.x = m_fMovedPosition_AccelerationCoolX_Right;
                    m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                }
                if (m_vPosition_SkillCool_Clone.x < m_fMovedPosition_CloneX_Right)
                {
                    m_vPosition_SkillCool_Clone.x = m_fMovedPosition_CloneX_Right;
                    m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                }
                if (m_vPosition_SkillCool_Stealth.x < m_fMovedPosition_StealthCoolX_Right)
                {
                    m_vPosition_SkillCool_Stealth.x = m_fMovedPosition_StealthCoolX_Right;
                    m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                }
            }
        }
        else
        {
            // Icon
            m_vPosition_Electric_On.x       += fTimeDelta * 30.f;
            m_vPosition_Acceleration_On.x   += fTimeDelta * 30.f;
            m_vPosition_Clone_On.x          += fTimeDelta * 30.f;
            m_vPosition_Stealth_On.x        += fTimeDelta * 30.f;
            m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
           // SkillCool
            m_vPosition_SkillCool_Electric.x       += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Acceleration.x   += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Clone.x          += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Stealth.x        += fTimeDelta * 30.f;
            m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
           // Icon
            if (m_vPosition_Electric_On.x > m_fOriginalPos_ElectricX)
            {
                m_vPosition_Electric_On.x = m_fOriginalPos_ElectricX;
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            }
            if (m_vPosition_Acceleration_On.x > m_fOriginalPos_AccelerationX)
            {
                m_vPosition_Acceleration_On.x = m_fOriginalPos_AccelerationX;
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            }
            if (m_vPosition_Clone_On.x > m_fOriginalPos_CloneX)
            {
                m_vPosition_Clone_On.x = m_fOriginalPos_CloneX;
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            }
            if (m_vPosition_Stealth_On.x > m_fOriginalPos_StealthX)
            {
                m_vPosition_Stealth_On.x = m_fOriginalPos_StealthX;
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
            }
           // SkillCool
            if (m_vPosition_SkillCool_Electric.x > m_fOriginalPos_ElectricCoolX)
            {
                m_vPosition_SkillCool_Electric.x = m_fOriginalPos_ElectricCoolX;
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            }
            if (m_vPosition_SkillCool_Acceleration.x > m_fOriginalPos_AccelerationCoolX)
            {
                m_vPosition_SkillCool_Acceleration.x = m_fOriginalPos_AccelerationCoolX;
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            }
            if (m_vPosition_SkillCool_Clone.x > m_fOriginalPos_CloneCoolX)
            {
                m_vPosition_SkillCool_Clone.x = m_fOriginalPos_CloneCoolX;
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            }
            if (m_vPosition_SkillCool_Stealth.x > m_fOriginalPos_StealthCoolX)
            {
                m_vPosition_SkillCool_Stealth.x = m_fOriginalPos_StealthCoolX;
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
               m_bRight = false;
            }
        }
    }
   // Top
    if (true == m_bTop)
    {
        if (0 != m_ptCursorMove.y)
        {
            if (-30 > m_ptCursorMove.y)
            {
                // Icon
                m_vPosition_Electric_On.y       -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Acceleration_On.y   -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Clone_On.y          -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Stealth_On.y        -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
               // SkillCool
                m_vPosition_SkillCool_Electric.y       -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Acceleration.y   -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Clone.y          -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Stealth.y        -= fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
               // Icon
                if (m_vPosition_Electric_On.y < m_fMovedPosition_ElectricY_Top)
                {
                    m_vPosition_Electric_On.y       = m_fMovedPosition_ElectricY_Top;
                    m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                }
                if (m_vPosition_Acceleration_On.y < m_fMovedPosition_AccelerationY_Top)
                {
                    m_vPosition_Acceleration_On.y = m_fMovedPosition_AccelerationY_Top;
                    m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                }
                if (m_vPosition_Clone_On.y < m_fMovedPosition_CloneY_Top)
                {
                    m_vPosition_Clone_On.y = m_fMovedPosition_CloneY_Top;
                    m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                }
                if (m_vPosition_Stealth_On.y < m_fMovedPosition_StealthY_Top)
                {
                    m_vPosition_Stealth_On.y = m_fMovedPosition_StealthY_Top;
                    m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
                }
               // SkillCool
                if (m_vPosition_SkillCool_Electric.y < m_fMovedPosition_ElectricCoolY_Top)
                {
                    m_vPosition_SkillCool_Electric.y = m_fMovedPosition_ElectricCoolY_Top;
                    m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                }
                if (m_vPosition_SkillCool_Acceleration.y < m_fMovedPosition_AccelerationCoolY_Top)
                {
                    m_vPosition_SkillCool_Acceleration.y = m_fMovedPosition_AccelerationCoolY_Top;
                    m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                }
                if (m_vPosition_SkillCool_Clone.y < m_fMovedPosition_CloneY_Top)
                {
                    m_vPosition_SkillCool_Clone.y = m_fMovedPosition_CloneY_Top;
                    m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                }
                if (m_vPosition_SkillCool_Stealth.y < m_fMovedPosition_StealthCoolY_Top)
                {
                    m_vPosition_SkillCool_Stealth.y = m_fMovedPosition_StealthCoolY_Top;
                    m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                }
            }
        }
        else
        {
            // Icon
            m_vPosition_Electric_On.y       += fTimeDelta * 30.f;
            m_vPosition_Acceleration_On.y   += fTimeDelta * 30.f;
            m_vPosition_Clone_On.y          += fTimeDelta * 30.f;
            m_vPosition_Stealth_On.y        += fTimeDelta * 30.f;
            m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
           // SkillCool
            m_vPosition_SkillCool_Electric.y       += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Acceleration.y   += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Clone.y          += fTimeDelta * 30.f;
            m_vPosition_SkillCool_Stealth.y        += fTimeDelta * 30.f;
            m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
           // Icon
            if (m_vPosition_Electric_On.y > m_fOriginalPos_ElectricY)
            {
                m_vPosition_Electric_On.y       = m_fOriginalPos_ElectricY;
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            }
            if (m_vPosition_Acceleration_On.y > m_fOriginalPos_AccelerationY)
            {
                m_vPosition_Acceleration_On.y = m_fOriginalPos_AccelerationY;
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            }
            if (m_vPosition_Clone_On.y > m_fOriginalPos_CloneY)
            {
                m_vPosition_Clone_On.y = m_fOriginalPos_CloneY;
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            }
            if (m_vPosition_Stealth_On.y > m_fOriginalPos_StealthY)
            {
                m_vPosition_Stealth_On.y = m_fOriginalPos_StealthY;
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
            }
           // SkillCool
            if (m_vPosition_SkillCool_Electric.y > m_fOriginalPos_ElectricCoolY)
            {
                m_vPosition_SkillCool_Electric.y = m_fOriginalPos_ElectricCoolY;
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            }
            if (m_vPosition_SkillCool_Acceleration.y > m_fOriginalPos_AccelerationCoolY)
            {
                m_vPosition_SkillCool_Acceleration.y = m_fOriginalPos_AccelerationCoolY;
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            }
            if (m_vPosition_SkillCool_Clone.y > m_fOriginalPos_CloneCoolY)
            {
                m_vPosition_SkillCool_Clone.y = m_fOriginalPos_CloneCoolY;
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            }
            if (m_vPosition_SkillCool_Stealth.y > m_fOriginalPos_StealthCoolY)
            {
                m_vPosition_SkillCool_Stealth.y = m_fOriginalPos_StealthCoolY;
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
               m_bTop = false;
            }
       }
    }
    // Bottom
    else if (true == m_bBottom)
    {
        if (0 != m_ptCursorMove.y)
        {
            if (30 < m_ptCursorMove.y)
            {
                // Icon
                m_vPosition_Electric_On.y       += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Acceleration_On.y   += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Clone_On.y          += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_Stealth_On.y        += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
                // SkillCool
                m_vPosition_SkillCool_Electric.y       += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Acceleration.y   += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Clone.y          += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_vPosition_SkillCool_Stealth.y        += fTimeDelta * abs(m_ptCursorMove.y * 0.1f);
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);

                // Icon
                if (m_vPosition_Electric_On.y > m_fMovedPosition_ElectricY_Bottom)
                {
                    m_vPosition_Electric_On.y = m_fMovedPosition_ElectricY_Bottom;
                    m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
                }
                if (m_vPosition_Acceleration_On.y > m_fMovedPosition_AccelerationY_Bottom)
                {
                    m_vPosition_Acceleration_On.y = m_fMovedPosition_AccelerationY_Bottom;
                    m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
                }
                if (m_vPosition_Clone_On.y > m_fMovedPosition_CloneY_Bottom)
                {
                    m_vPosition_Clone_On.y = m_fMovedPosition_CloneY_Bottom;
                    m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
                }
                if (m_vPosition_Stealth_On.y > m_fMovedPosition_StealthY_Bottom)
                {
                    m_vPosition_Stealth_On.y = m_fMovedPosition_StealthY_Bottom;
                    m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
                }

               // SkillCool
               if (m_vPosition_SkillCool_Electric.y > m_fMovedPosition_ElectricCoolY_Bottom)
               {
                   m_vPosition_SkillCool_Electric.y = m_fMovedPosition_ElectricCoolY_Bottom;
                   m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
               }
               if (m_vPosition_SkillCool_Acceleration.y > m_fMovedPosition_AccelerationCoolY_Bottom)
               {
                   m_vPosition_SkillCool_Acceleration.y = m_fMovedPosition_AccelerationCoolY_Bottom;
                   m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
               }
               if (m_vPosition_SkillCool_Clone.y > m_fMovedPosition_CloneY_Bottom)
               {
                   m_vPosition_SkillCool_Clone.y = m_fMovedPosition_CloneY_Bottom;
                   m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
               }
               if (m_vPosition_SkillCool_Stealth.y > m_fMovedPosition_StealthCoolY_Bottom)
               {
                   m_vPosition_SkillCool_Stealth.y = m_fMovedPosition_StealthCoolY_Bottom;
                   m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
               }
            }
        }
        else
        {
            // Icon
            m_vPosition_Electric_On.y       -= fTimeDelta * 30.f;
            m_vPosition_Acceleration_On.y   -= fTimeDelta * 30.f;
            m_vPosition_Clone_On.y          -= fTimeDelta * 30.f;
            m_vPosition_Stealth_On.y        -= fTimeDelta * 30.f;
            m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
            
            // SkillCool
            m_vPosition_SkillCool_Electric.y       -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Acceleration.y   -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Clone.y          -= fTimeDelta * 30.f;
            m_vPosition_SkillCool_Stealth.y        -= fTimeDelta * 30.f;
            m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);

            // Icon
            if (m_vPosition_Electric_On.y < m_fOriginalPos_ElectricY)
            {
                m_vPosition_Electric_On.y       = m_fOriginalPos_ElectricY;
                m_pSkill_ElectricOn->Set_State(TRANSFORM::POSITION, m_vPosition_Electric_On);
            }
            if (m_vPosition_Acceleration_On.y < m_fOriginalPos_AccelerationY)
            {
                m_vPosition_Acceleration_On.y = m_fOriginalPos_AccelerationY;
                m_pSkill_AccelerationOn->Set_State(TRANSFORM::POSITION, m_vPosition_Acceleration_On);
            }
            if (m_vPosition_Clone_On.y < m_fOriginalPos_CloneY)
            {
                m_vPosition_Clone_On.y = m_fOriginalPos_CloneY;
                m_pSkill_CloneOn->Set_State(TRANSFORM::POSITION, m_vPosition_Clone_On);
            }
            if (m_vPosition_Stealth_On.y < m_fOriginalPos_StealthY)
            {
                m_vPosition_Stealth_On.y = m_fOriginalPos_StealthY;
                m_pSkill_StealthOn->Set_State(TRANSFORM::POSITION, m_vPosition_Stealth_On);
            }

            // SkillCool
            if (m_vPosition_SkillCool_Electric.y < m_fOriginalPos_ElectricCoolY)
            {
                m_vPosition_SkillCool_Electric.y = m_fOriginalPos_ElectricCoolY;
                m_pSkill_Cool_Electric->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Electric);
            }
            if (m_vPosition_SkillCool_Acceleration.y  < m_fOriginalPos_AccelerationCoolY)
            {
                m_vPosition_SkillCool_Acceleration.y = m_fOriginalPos_AccelerationCoolY;
                m_pSkill_Cool_Acceleration->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Acceleration);
            }
            if (m_vPosition_SkillCool_Clone.y < m_fOriginalPos_CloneCoolY)
            {
                m_vPosition_SkillCool_Clone.y = m_fOriginalPos_CloneCoolY;
                m_pSkill_Cool_Clone->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Clone);
            }
            if (m_vPosition_SkillCool_Stealth.y < m_fOriginalPos_StealthCoolY)
            {
                m_vPosition_SkillCool_Stealth.y = m_fOriginalPos_StealthCoolY;
                m_pSkill_Cool_Stealth->Set_State(TRANSFORM::POSITION, m_vPosition_SkillCool_Stealth);
                m_bBottom = false;
            }
        }
    }
}

void CUI_SkillSlot::Activate_Skill_Electric()
{
    if (false == CUI_Manager:: Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_bCanActivate_Electric = false;
        m_bCanChargeGauge_Electric = false;
    }
}

void CUI_SkillSlot::Activate_Skill_Acceleration()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_bCanActivate_Acceleration = false;
        m_bCanChargeGauge_Acceleration = false;

    }
}

void CUI_SkillSlot::Activate_Skill_Clone()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_bCanActivate_Clone = false;
        m_bCanChargeGauge_Clone = false;

    }
}

void CUI_SkillSlot::Activate_Skill_Stealth()
{
    if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
    {
        m_bCanActivate_Stealth = false;
        m_bCanChargeGauge_Stealth = false;

    }
}

void CUI_SkillSlot::SkillStop_Electic()
{
    m_bCanChargeGauge_Electric = true;
    m_fBlink_Electric = 0.f;
}

void CUI_SkillSlot::SkillStop_Acceleration()
{
    m_bCanChargeGauge_Acceleration = true;
    m_fBlink_Acceleration = 0.f;
}

void CUI_SkillSlot::SkillStop_Replication()
{
    m_bCanChargeGauge_Clone = true;
    m_fBlink_Clone = 0.f;

}

void CUI_SkillSlot::SkillStop_Stealth()
{
    m_bCanChargeGauge_Stealth = true;
    m_fBlink_Stealth = 0.f;

}

void CUI_SkillSlot::Blink_Skill_Icon_Electric(_float fTimeDelta)
{
    //m_fBlinkIndex += fTimeDelta; 

    //_uint iBlinkIndex = (_uint)m_fBlinkIndex % 2;

#pragma region Blink Electric Icon
    /* Skill Icon Blink */
    if (false == m_bCanActivate_Electric)
    {
        if (false == m_bCanChargeGauge_Electric)
        {
            m_fBlinkTime_Electric += fTimeDelta;

            m_fBlinkIndex_Electric = (_uint)m_fBlinkTime_Electric % 2;

            m_fRatio_Electric = m_fGauge_Electric / m_fGaugeMax_Electric;

            /* Gauge_Dw */
            m_fGauge_Electric -= fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeDecreaseTime;

            /* Blink Code */
            if (0 == m_fBlinkIndex_Electric)
            {
                m_fBlink_Electric += fTimeDelta * 3.f;
            }
            if (1 == m_fBlinkIndex_Electric)
            {
                m_fBlink_Electric -= fTimeDelta * 3.f;
            }

            /* Exception */
            if (0.f >= m_fRatio_Electric)
            {
                m_fRatio_Electric = 0.f;
                m_fGauge_Electric = 0.f;
                m_fBlinkTime_Electric = 0.f;
                m_fBlinkIndex_Electric = 0.f;
                m_bCanChargeGauge_Electric = true;
                m_bRenderIcon_Electric = false;
            }
        }
        else
        {
            m_fRatio_Electric = m_fGauge_Electric / m_fGaugeMax_Electric;

            m_fGauge_Electric += fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeIncreaseTime; // Skill Gauge Charging

            m_fBlinkTime_Electric = 0.f;

            if (0.2f <= m_fRatio_Electric)
            {
                m_bRenderIcon_Electric = true; // Skill Charging End
            }
            else
            {
                m_bRenderIcon_Electric = false;
            }

            if (1.f <= m_fRatio_Electric)
            {
                m_fRatio_Electric = 1.f; // Exception
                m_fGauge_Electric = m_fGaugeMax_Electric;
                m_bCanActivate_Electric = true; // Skill Charging End
            }
        }
    }
    else
    {
        m_bCanChargeGauge_Electric = false;
    }

#pragma endregion
}

void CUI_SkillSlot::Blink_Skill_Icon_Acceleration(_float fTimeDelta)
{
#pragma region Blink Icon Acceleration
    /* Skill Icon Blink */
    if (false == m_bCanActivate_Acceleration)
    {
        if (false == m_bCanChargeGauge_Acceleration)
        {
            m_fBlinkTime_Acceleration += fTimeDelta;

            m_fBlinkIndex_Acceleration = (_uint)m_fBlinkTime_Acceleration % 2;

            m_fRatio_Acceleration = m_fGauge_Acceleration / m_fGaugeMax_Acceleration;

            /* Gauge_Dw */
            m_fGauge_Acceleration -= fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeDecreaseTime;

            /* Blink Code */
            if (0 == m_fBlinkIndex_Acceleration)
            {
                m_fBlink_Acceleration += fTimeDelta * 3.f;
            }
            if (1 == m_fBlinkIndex_Acceleration)
            {
                m_fBlink_Acceleration -= fTimeDelta * 3.f;
            }

            /* Exception */
            if (0.f >= m_fRatio_Acceleration)
            {
                m_fRatio_Acceleration = 0.f;
                m_fGauge_Acceleration = 0.f;
                m_fBlinkTime_Acceleration = 0.f;
                m_fBlinkIndex_Acceleration = 0.f;
                m_bCanChargeGauge_Acceleration = true;
                m_bRenderIcon_Acceleration = false;
            }
        }
        else
        {
            m_fRatio_Acceleration = m_fGauge_Acceleration / m_fGaugeMax_Acceleration;

            m_fGauge_Acceleration += fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeIncreaseTime; // Skill Gauge Charging

            m_fBlinkTime_Acceleration = 0.f;

            if (0.2f <= m_fRatio_Acceleration)
            {
                m_bRenderIcon_Acceleration = true; // Skill Charging End
            }
            else
            {
                m_bRenderIcon_Acceleration = false;
            }

            if (1.f <= m_fRatio_Acceleration)
            {
                m_fRatio_Acceleration = 1.f; // Exception
                m_fGauge_Acceleration = m_fGaugeMax_Acceleration;
                m_bCanActivate_Acceleration = true; // Skill Charging End
            }
        }
    }
    else
    {
        m_bCanChargeGauge_Acceleration = false;
    }

}

void CUI_SkillSlot::Blink_Skill_Icon_Clone(_float fTimeDelta)
{
#pragma region Blink Icon Clone
    /* Skill Icon Blink */
    if (false == m_bCanActivate_Clone)
    {
        if (false == m_bCanChargeGauge_Clone)
        {
            m_fBlinkTime_Clone += fTimeDelta;

            m_fBlinkIndex_Clone = (_uint)m_fBlinkTime_Clone % 2;

            m_fRatio_Clone = m_fGauge_Clone / m_fGaugeMax_Clone;

            /* Gauge_Dw */
            m_fGauge_Clone -= fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeDecreaseTime;

            /* Blink Code */
            if (0 == m_fBlinkIndex_Clone)
            {
                m_fBlink_Clone += fTimeDelta * 3.f;
            }
            if (1 == m_fBlinkIndex_Clone)
            {
                m_fBlink_Clone -= fTimeDelta * 3.f;
            }

            /* Exception */
            if (0.f >= m_fRatio_Clone)
            {
                m_fRatio_Clone = 0.f;
                m_fGauge_Clone = 0.f;
                m_fBlinkTime_Clone = 0.f;
                m_fBlinkIndex_Clone = 0.f;
                m_bCanChargeGauge_Clone = true;
                m_bRenderIcon_Replication = false;
            }
        }
        else
        {
            m_fRatio_Clone = m_fGauge_Clone / m_fGaugeMax_Clone;

            m_fGauge_Clone += fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeIncreaseTime; // Skill Gauge Charging

            m_fBlinkTime_Clone = 0.f;

            if (0.2f <= m_fRatio_Clone)
            {
                m_bRenderIcon_Replication = true; // Skill Charging End
            }
            else
            {
                m_bRenderIcon_Replication = false;
            }

            if (1.f <= m_fRatio_Clone)
            {
                m_fRatio_Clone = 1.f; // Exception
                m_fGauge_Clone = m_fGaugeMax_Clone;
                m_bCanActivate_Clone = true; // Skill Charging End
            }
        }
    }
    else
    {
        m_bCanChargeGauge_Clone = false;
    }

#pragma endregion
}

void CUI_SkillSlot::Blink_Skill_Icon_Stealth(_float fTimeDelta)
{
#pragma region Blink Icon Stealth
    /* Skill Icon Blink */
    if (false == m_bCanActivate_Stealth)
    {
        if (false == m_bCanChargeGauge_Stealth)
        {
            m_fBlinkTime_Stealth += fTimeDelta;

            m_fBlinkIndex_Stealth = (_uint)m_fBlinkTime_Stealth % 2;

            m_fRatio_Stealth = m_fGauge_Stealth / m_fGaugeMax_Stealth;

            /* Gauge_Dw */
            m_fGauge_Stealth -= fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeDecreaseTime;

            /* Blink Code */
            if (0 == m_fBlinkIndex_Stealth)
            {
                m_fBlink_Stealth += fTimeDelta * 3.f;
            }
            if (1 == m_fBlinkIndex_Stealth)
            {
                m_fBlink_Stealth -= fTimeDelta * 3.f;
            }

            /* Exception */
            if (0.f >= m_fRatio_Stealth)
            {
                m_fRatio_Stealth = 0.f;
                m_fGauge_Stealth = 0.f;
                m_fBlinkTime_Stealth = 0.f;
                m_fBlinkIndex_Stealth = 0.f;
                m_bCanChargeGauge_Stealth = true;
                m_bRenderIcon_Replication = false;
            }
        }
        else
        {
            m_fRatio_Stealth = m_fGauge_Stealth / m_fGaugeMax_Stealth;

            m_fGauge_Stealth += fTimeDelta * static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerDesc().SAS_GaugeIncreaseTime; // Skill Gauge Charging

            m_fBlinkTime_Stealth = 0.f;

            if (0.2f <= m_fRatio_Stealth)
            {
                m_bRenderIcon_Replication = true; // Skill Charging End
            }
            else
            {
                m_bRenderIcon_Replication = false;
            }

            if (1.f <= m_fRatio_Stealth)
            {
                m_fRatio_Stealth = 1.f; // Exception
                m_fGauge_Stealth = m_fGaugeMax_Stealth;
                m_bCanActivate_Stealth = true; // Skill Charging End
            }
        }
    }
    else
    {
        m_bCanChargeGauge_Stealth = false;
    }

#pragma endregion
}

shared_ptr<CUI_SkillSlot> CUI_SkillSlot::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CUI_SkillSlot> pInstance = make_private_shared(CUI_SkillSlot, _pDevice, _pContext);
    
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CUI_SkillSlot::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CUI_SkillSlot::Clone(any _aDesc)
{
    shared_ptr<CUI_SkillSlot> pInstance = make_private_shared_copy(CUI_SkillSlot, *this);

    if (FAILED(pInstance->Initialize(_aDesc)))
    {
        MSG_RETURN(nullptr, "CUI_SkillSlot::Create", "Failed to Initialize_Prototype");
    }

    return pInstance;
}
