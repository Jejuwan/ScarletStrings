#include "ClientPCH.h"
#include "UI_Manager.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "ImGui_Manager.h"

#include "Character.h"
#include "Player.h"
#include "Light_Manager.h"

#pragma region shared_ptr For UI
#include "UI_Indicator_ObjectAttack.h"
#include "UI_Indicator_SpcObjectAttack.h"
#include "UI_Menu_Background.h"
#include "UI_Indicator_ToolTip_Tutorial_PMCC.h"
#include "UI_Indicator_ToolTip_Tutorial_LockOn.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle.h"
#include "UI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack.h"
#include "UI_Indicator_ToolTip_Tutorial_Success.h"
#include "UI_Indicator_BrainTalk_Tutorial.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "UI_Indicator_Tooltip_Tutorial_Tips.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Acceleration.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Electric.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Replication.h"
#include "UI_Indicator_ToolTip_Mizuha_SAS_Stealth.h"
#include "UI_Psyco_Wave.h"
#include "UI_Indicator_Script.h"
#include "UI_Indicator_Communication.h"
#pragma endregion

#pragma region Character
#include "Kasane.h"
#pragma endregion


CUI_Manager::CUI_Manager()
{
#pragma region UI
	m_bUI_All_RenderStop			= false;
	m_bOnOffSwitch_RenderUI			= false;
	m_bUI_Bound						= false;
	m_bCanFade						= true;
	m_bBrainTalk					= false;
	m_bMindRoom_CutScene_End		= false;
	m_bMouseClick					= false;
	m_bTutorialRender				= false;
	m_bBrainTalk_RenderNow			= false;
	m_bNoDamage						= false;
	m_bInstKillCutScene				= false;
	m_bRenderJW						= false;

	m_iMenuIndex					= 0;
	m_iMenuSwitch					= -1;
	m_iSelect_MemberIndex			= 0;
	m_iItemIndex					= 0;
	m_iItemCount					= 0;
	m_iFadeInOut					= -1;
	m_iK							= 10000;
	m_iBP							= 0;
	m_iDamageType					= 0;
	m_vIndicatorScale				= _float3{ 64.f, 64.f, 1.f };
	m_vPosition_Damage				= XMMatrixIdentity();
	m_iPossession					= 1;

	for (size_t i = 0; i < 61; i++)
	{
		m_bSkillIconBecomeActivated[i] = false;
	}
#pragma endregion



}

#ifdef _DEBUG
void CUI_Manager::Debug_Render()
{
	auto pImGuiManager = CImGui_Manager::Get_Instance();
	if (false == pImGuiManager->Is_Enable())
		return;

	_float fDefaultTimeScale = CGameInstance::Get_Instance()->Get_TimeScale(DEFAULT_FPS);

	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_4))
	{
		CGameInstance::Get_Instance()->Set_TimeScale(max(0.f, fDefaultTimeScale - 0.1f));
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(max(0.f, fDefaultTimeScale - 0.1f));
	}
	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_6))
	{
		CGameInstance::Get_Instance()->Set_TimeScale(fDefaultTimeScale + 0.1f);
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(fDefaultTimeScale + 0.1f);
	}

#pragma region Postprocess
	if (ImGui::BeginMainMenuBar())
	{
#if ACTIVATE_IMGUI
	if (ImGui::BeginMenu("Shader Effects"))
	{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (ImGui::CollapsingHeader("Postprocess"))
	{
		ImGui::Indent(16.f);
		if (ImGui::CollapsingHeader("Tonemapping"))
		{
			_float fBloomStrength = pGameInstance->Get_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING);
			if (ImGui::SliderFloat("Postprocess BloomStrength", &fBloomStrength, 0.f, 1.3f))
			{
				pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, fBloomStrength);
			}

			_float fExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING);
			if (ImGui::SliderFloat("Postprocess Exposure", &fExposure, 0.f, 3.f))
			{
				pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, fExposure);
			}

			_float fGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING);
			if (ImGui::SliderFloat("Postprocess Gamma", &fGamma, 0.f, 3.f))
			{
				pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, fGamma);
			}
		}

		if (ImGui::CollapsingHeader("GameObject"))
		{
			_float fBloomStrength = pGameInstance->Get_BloomStrength(IMAGE_PROCESS::PROCESS_BLOOM);
			if (ImGui::SliderFloat("GameObject BloomStrength", &fBloomStrength, 0.f, 1.3f))
			{
				pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_BLOOM, fBloomStrength);
			}

			_float fExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_BLOOM);
			if (ImGui::SliderFloat("GameObject Exposure", &fExposure, 0.f, 3.f))
			{
				pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_BLOOM, fExposure);
			}

			_float fGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_BLOOM);
			if (ImGui::SliderFloat("GameObject Gamma", &fGamma, 0.f, 3.f))
			{
				pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_BLOOM, fGamma);
			}
		}

		if (ImGui::CollapsingHeader("Neon"))
		{
			_float fBloomStrength = pGameInstance->Get_BloomStrength(IMAGE_PROCESS::PROCESS_NEON);
			if (ImGui::SliderFloat("Neon BloomStrength", &fBloomStrength, 0.f, 1.3f))
			{
				pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, fBloomStrength);
			}

			_float fExposure = pGameInstance->Get_Exposure(IMAGE_PROCESS::PROCESS_NEON);
			if (ImGui::SliderFloat("Neon Exposure", &fExposure, 0.f, 3.f))
			{
				pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, fExposure);
			}

			_float fGamma = pGameInstance->Get_Gamma(IMAGE_PROCESS::PROCESS_NEON);
			if (ImGui::SliderFloat("Neon Gamma", &fGamma, 0.f, 3.f))
			{
				pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, fGamma);
			}
		}

		if (ImGui::CollapsingHeader("LUT Filter"))
		{
			static _bool bLUTEnable = pGameInstance->Enable_LUTFilter();
			if (ImGui::Checkbox("LUT Filter Enable", &bLUTEnable))
			{
				pGameInstance->Enable_LUTFilter(bLUTEnable);
			}

			if (bLUTEnable)
			{
				static int iLUTIndex = pGameInstance->Get_LUTIndex();
				if (ImGui::SliderInt("LUT Filter Index", &iLUTIndex, 0, 6))
				{
					pGameInstance->Set_LUTIndex(_uint(iLUTIndex));
				}
			}
		}

		if (ImGui::CollapsingHeader("SSAO"))
		{
			_bool bEnableSSAO = pGameInstance->Enable_SSAO();
			if (ImGui::Checkbox("Enable SSAO", &bEnableSSAO))
			{
				pGameInstance->Enable_SSAO(bEnableSSAO);
			}

			if (pGameInstance->Key_Down(VK_HOME))
			{
				pGameInstance->Enable_SSAO(!bEnableSSAO);
			}
		}

		if (ImGui::CollapsingHeader("Sun & Lensflare"))
		{
			_bool bLensflareEnable = pGameInstance->Enable_Lensflare();
			if (ImGui::Checkbox("Enable Lensflare", &bLensflareEnable))
			{
				pGameInstance->Enable_Lensflare(bLensflareEnable);
			}

			_float3 vSunPosition = pGameInstance->Get_SunPosition();
			if (ImGui::DragFloat3("Sun Position", &vSunPosition.x, 5.f, -10000.f, 10000.f))
			{
				pGameInstance->Set_SunPosition(vSunPosition);
			}

			_float3 vSunColor = pGameInstance->Get_SunColor();
			if (ImGui::SliderFloat3("Sun Color", &vSunColor.x, 0.f, 5.f))
			{
				pGameInstance->Set_SunColor(vSunColor);
			}

			_float fLensflareStrength = pGameInstance->Get_LensflareStrength();
			if (ImGui::SliderFloat("Lensflare Strength", &fLensflareStrength, 0.f, 10.f))
			{
				pGameInstance->Set_LensflareStrength(fLensflareStrength);
			}
		}
		
		if (ImGui::CollapsingHeader("FXAA(Multisampling)"))
		{
			_bool bEnableFXAA = pGameInstance->Enable_FXAA();
			if (ImGui::Checkbox("Enable FXAA", &bEnableFXAA))
			{
				pGameInstance->Enable_FXAA(bEnableFXAA);
			}

			if (pGameInstance->Key_Down(VK_HOME))
			{
				pGameInstance->Enable_FXAA(!bEnableFXAA);
			}
		}
		
		if (ImGui::CollapsingHeader("DOF(Depth Of Field)"))
		{
			_bool bEnableDOF = pGameInstance->Enable_DOF();
			if (ImGui::Checkbox("Enable DOF", &bEnableDOF))
			{
				pGameInstance->Enable_DOF(bEnableDOF);
			}

			if (bEnableDOF)
			{
				_float fDepthStart = pGameInstance->Get_DOF_DepthStart();
				if (ImGui::DragFloat("DOF Start", &fDepthStart, 1.f, 0.f, 1000.f))
				{
					pGameInstance->Set_DOF_DepthStart(fDepthStart);
				}

				_float fDepthRange = pGameInstance->Get_DOF_DepthRange();
				if (ImGui::DragFloat("DOF Range", &fDepthRange, 1.f, 0.f, 1000.f))
				{
					pGameInstance->Set_DOF_DepthRange(fDepthRange);
				}
			}

			if (pGameInstance->Key_Down(VK_END))
			{
				pGameInstance->Enable_DOF(!bEnableDOF);
			}
		}
		ImGui::Unindent(16.f);
	}

	if (ImGui::CollapsingHeader("Object Effect"))
	{
		ImGui::Indent(16.f);
		if (ImGui::CollapsingHeader("Player Transperency"))
		{
			auto pObject = pGameInstance->Get_Player();
			if (pObject != nullptr)
			{
				auto pPlayer = static_cast<CPlayer*>(pObject);

				static _bool bPlayerTransperency = false;
				if (ImGui::Checkbox("Transperency Enable", &bPlayerTransperency))
				{
					pPlayer->Set_Transparency(bPlayerTransperency);
				}

				pPlayer->Debug_Transparency_Parameters();
			}
		}
		ImGui::Unindent(16.f);
	}

	ImGui::EndMenu();
	}
#endif

#pragma region Physx Render
	if (ImGui::BeginMenu("PhysxDraw"))
	{
	if (ImGui::Checkbox("Render", &m_bPhysxRender))
		CGameInstance::Get_Instance()->SetPhysxDrawStatus(m_bPhysxRender);

	if(!m_bPhysxRender)
	{
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->setVisualizationParameter(PxVisualizationParameter::eSCALE, 0.0f);
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 0.0f);
	}
	else
	{
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}
	ImGui::EndMenu();
	}
#pragma endregion

#pragma region Time Manager

	if (ImGui::BeginMenu("Time Manager"))
	{
	if (ImGui::SliderFloat("Defualt Time Scale", &fDefaultTimeScale, 0.f, 8.f))
	{
		CGameInstance::Get_Instance()->Set_TimeScale(fDefaultTimeScale);
		CGameInstance::Get_Instance()->Set_PhysXTimeScale(fDefaultTimeScale);
	}

	ImGui::EndMenu();
	}

#pragma endregion
#pragma endregion
	ImGui::EndMainMenuBar();
	}
}
#endif

HRESULT CUI_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	m_pDevice = _pDevice;
	m_pContext = _pContext;

	m_mView = XMMatrixIdentity();
	m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), g_iWinCX / g_iWinCY, 0.1f, 10000.f);

#pragma region UI
	//m_pPsycoWave = CUI_Psyco_Wave::Create(m_pDevice, m_pContext);
	//m_pPsycoWave->Initialize_Prototype();
	//m_pPsycoWave->Initialize();

	m_pIndicator_ObjectAttack = CUI_Indicator_ObjectAttack::Create(m_pDevice, m_pContext);
	m_pIndicator_ObjectAttack->Initialize_Prototype();
	m_pIndicator_ObjectAttack->Initialize();

	m_pIndicator_SpecialObjectAttack = CUI_Indicator_SpcObjectAttack::Create(m_pDevice, m_pContext);
	m_pIndicator_SpecialObjectAttack->Initialize_Prototype();
	m_pIndicator_SpecialObjectAttack->Initialize();

	m_pIndicator_ToolTip_Tutorial_PMCC = CUI_Indicator_Tooltip_Tutorial_PMCC::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_PMCC->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_PMCC->Initialize();

	m_pIndicator_ToolTip_Tutorial_LockOn = CUI_Indicator_ToolTip_Tutorial_LockOn::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_LockOn->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_LockOn->Initialize();

	m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle = CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle->Initialize();

	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack = CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack->Initialize();

	m_pIndicator_ToolTip_Tutorial_Success = CUI_Indicator_ToolTip_Tutorial_Success::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_Success->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_Success->Initialize();

	m_pIndicator_BrainTalk_Tutorial = CUI_Indicator_BrainTalk_Tutorial::Create(m_pDevice, m_pContext);
	m_pIndicator_BrainTalk_Tutorial->Initialize_Prototype();
	m_pIndicator_BrainTalk_Tutorial->Initialize();

	m_pIndicator_BrainTalk_Mizuha = CUI_Indicator_BrainTalk_Mizuha::Create(m_pDevice, m_pContext);
	m_pIndicator_BrainTalk_Mizuha->Initialize_Prototype();
	m_pIndicator_BrainTalk_Mizuha->Initialize();

	m_pIndicator_ToolTip_Tutorial_Tips = CUI_Indicator_ToolTip_Tutorial_Tips::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Tutorial_Tips->Initialize_Prototype();
	m_pIndicator_ToolTip_Tutorial_Tips->Initialize();

	m_pIndicator_ToolTip_Mizuha_SAS_Acceleration = CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Mizuha_SAS_Acceleration->Initialize_Prototype();
	m_pIndicator_ToolTip_Mizuha_SAS_Acceleration->Initialize();

	m_pIndicator_ToolTip_Mizuha_SAS_Electric = CUI_Indicator_ToolTip_Mizuha_SAS_Electric::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Mizuha_SAS_Electric->Initialize_Prototype();
	m_pIndicator_ToolTip_Mizuha_SAS_Electric->Initialize();

	m_pIndicator_ToolTip_Mizuha_SAS_Replication = CUI_Indicator_ToolTip_Mizuha_SAS_Replication::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Mizuha_SAS_Replication->Initialize_Prototype();
	m_pIndicator_ToolTip_Mizuha_SAS_Replication->Initialize();


	m_pIndicator_ToolTip_Mizuha_SAS_Stealth = CUI_Indicator_ToolTip_Mizuha_SAS_Stealth::Create(m_pDevice, m_pContext);
	m_pIndicator_ToolTip_Mizuha_SAS_Stealth->Initialize_Prototype();
	m_pIndicator_ToolTip_Mizuha_SAS_Stealth->Initialize();

	m_pScript = CUI_Indicator_Script::Create(m_pDevice, m_pContext);
	m_pScript->Initialize_Prototype();
	m_pScript->Initialize();

	m_pIndicator_Communication = CUI_Indicator_Communication::Create(m_pDevice, m_pContext);
	m_pIndicator_Communication->Initialize_Prototype();
	m_pIndicator_Communication->Initialize();

#pragma endregion
	
	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
	//m_pPsycoWave->Tick(fTimeDelta);
	m_pIndicator_ObjectAttack->Tick(fTimeDelta);
	m_pIndicator_SpecialObjectAttack->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_PMCC->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_LockOn->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Success->Tick(fTimeDelta);
	m_pIndicator_BrainTalk_Tutorial->Tick(fTimeDelta);
	m_pIndicator_BrainTalk_Mizuha->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Tips->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Acceleration->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Electric->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Replication->Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Stealth->Tick(fTimeDelta);
	m_pScript->Tick(fTimeDelta);
	m_pIndicator_Communication->Tick(fTimeDelta);


	if (CGameInstance::Get_Instance()->Key_Down(VK_ESCAPE)) // ESC::Menu
	{
		if (!m_IsShop)
		{
			CUI_Manager::Get_Instance()->Set_UI_FadeInOut();
			m_bMenu_UI = !m_bMenu_UI;

			CGameInstance::Get_Instance()->Show_Cursor(m_bMenu_UI);
		}
	}


	if (!m_bMenu_UI)
	{
		Shop_UI_Control();
	}
}

void CUI_Manager::Late_Tick(_float fTimeDelta)
{
	//m_pPsycoWave->Late_Tick(fTimeDelta);
	m_pIndicator_ObjectAttack->Late_Tick(fTimeDelta);
	m_pIndicator_SpecialObjectAttack->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_PMCC->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_LockOn->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Success->Late_Tick(fTimeDelta);
	m_pIndicator_BrainTalk_Tutorial->Late_Tick(fTimeDelta);
	m_pIndicator_BrainTalk_Mizuha->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Tutorial_Tips->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Acceleration->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Electric->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Replication->Late_Tick(fTimeDelta);
	m_pIndicator_ToolTip_Mizuha_SAS_Stealth->Late_Tick(fTimeDelta);
	m_pScript->Late_Tick(fTimeDelta);
	m_pIndicator_Communication->Late_Tick(fTimeDelta);
}

HRESULT CUI_Manager::Render()
{
	return S_OK;
}

shared_ptr<class CUI_Psyco_Wave> CUI_Manager::Get_Psyco_Wave()
{
	return m_pPsycoWave;
}

shared_ptr<CUI_Indicator_ObjectAttack> CUI_Manager::Get_Indicator()
{
	return m_pIndicator_ObjectAttack;
}

shared_ptr<class CUI_Indicator_SpcObjectAttack> CUI_Manager::Get_IndicatorSpc()
{
	return m_pIndicator_SpecialObjectAttack;
}

shared_ptr<class CUI_Indicator_Tooltip_Tutorial_PMCC> CUI_Manager::Get_Indicator_ToolTip_Tutorial_PMCC()
{
	return m_pIndicator_ToolTip_Tutorial_PMCC;
}

shared_ptr<class CUI_Indicator_ToolTip_Tutorial_LockOn> CUI_Manager::Get_Indicator_ToolTip_Tutorial_LockOn()
{
	return m_pIndicator_ToolTip_Tutorial_LockOn;
}

shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_BattleStyle> CUI_Manager::Get_Indicator_ToolTip_Tutorial_Kasane_BattleStyle()
{
	return m_pIndicator_ToolTip_Tutorial_Kasane_BattleStyle;
}

shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack> CUI_Manager::Get_Indicator_ToolTip_Tutorial_Kasane_SpecialAttack()
{
	return m_pIndicator_ToolTip_Tutorial_Kasane_SpecialAttack;
}

shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Success> CUI_Manager::Get_Indicator_ToolTip_Tutorial_Success()
{
	return m_pIndicator_ToolTip_Tutorial_Success;
}

shared_ptr<class CUI_Indicator_BrainTalk_Tutorial> CUI_Manager::Get_Indicator_BrainTalk_Tutorial()
{
	return m_pIndicator_BrainTalk_Tutorial;
}

shared_ptr<class CUI_Indicator_BrainTalk_Mizuha> CUI_Manager::Get_Indicator_BrainTalk_Mizuha()
{
	return m_pIndicator_BrainTalk_Mizuha;
}

shared_ptr<class CUI_Indicator_ToolTip_Tutorial_Tips> CUI_Manager::Get_Indicator_ToolTip_Tutorial_Tips()
{
	return m_pIndicator_ToolTip_Tutorial_Tips;
}

shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Electric> CUI_Manager::Get_Indicator_ToolTip_Mizuha_SAS_Electric()
{
	return m_pIndicator_ToolTip_Mizuha_SAS_Electric;
}

shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Acceleration> CUI_Manager::Get_Indicator_ToolTip_Mizuha_SAS_Acceleration()
{
	return m_pIndicator_ToolTip_Mizuha_SAS_Acceleration;
}

shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Replication> CUI_Manager::Get_Indicator_ToolTip_Mizuha_SAS_Replication()
{
	return m_pIndicator_ToolTip_Mizuha_SAS_Replication;
}

shared_ptr<class CUI_Indicator_ToolTip_Mizuha_SAS_Stealth> CUI_Manager::Get_Indicator_ToolTip_Mizuha_SAS_Stealth()
{
	return m_pIndicator_ToolTip_Mizuha_SAS_Stealth;
}

shared_ptr<class CUI_Indicator_Script> CUI_Manager::Get_Script()
{
	return m_pScript;
}

shared_ptr<class CUI_Indicator_Communication> CUI_Manager::Get_Indicator_Communication()
{
	return m_pIndicator_Communication;
}

HRESULT CUI_Manager::Bind_Matixies(shared_ptr<CShader> _pShader)
{
	_pShader->Bind_Matrix(SHADER_MATVIEW, m_mView);

	_pShader->Bind_Matrix(SHADER_MATPROJ, m_mProj);

	return S_OK;
}

void CUI_Manager::Bind_Character(CHARACTER _eCharacter, shared_ptr<CGameObject> _pCharacter)
{
	auto Iter = m_mapCharacter.find(_eCharacter);
	if (Iter != m_mapCharacter.end())
	{
		m_mapCharacter.erase(Iter);
	}

	m_mapCharacter.emplace(_eCharacter, dynamic_pointer_cast<CCharacter>(_pCharacter));
}

HRESULT CUI_Manager::Play_SAS_Cutscene(CHARACTER _eCharacter, const std::function<void()>& fpFinished)
{
	auto Iter = m_mapCharacter.find(_eCharacter);
	if (Iter == m_mapCharacter.end())
	{
		MSG_RETURN(E_FAIL, "CUI_Manager::Play_SAS_Cutscene", "Chracter Not Binded");
	}

	Iter->second->Execute_SAS(fpFinished);

	return S_OK;
}

HRESULT CUI_Manager::Open_Drive_Mode(CHARACTER _eCharacter, const std::function<void()>& fpFinished)
{
	auto Iter = m_mapCharacter.find(_eCharacter);
	if (Iter == m_mapCharacter.end())
	{
		MSG_RETURN(E_FAIL, "CUI_Manager::Open_Drive_Mode", "Chracter Not Binded");
	}

	auto pKasane = dynamic_pointer_cast<CKasane>(Iter->second);
	pKasane->Open_Drive_Mode(fpFinished);

	return S_OK;
}

void CUI_Manager::Shop_UI_Control()
{
	if (CGameInstance::Get_Instance()->Key_Down('U'))
	{
		m_IsShop = !m_IsShop;
	}

	if (m_bEnter_ShopUI)
	{
		if (CGameInstance::Get_Instance()->Key_Down('1'))
		{
			m_iShop_Number_index = 0;
			m_iShop_Catalog_index = 0;
			m_iShopMenu_Index--;
			if (m_iShopMenu_Index < 0)
				m_iShopMenu_Index = 1;
		}
		if (CGameInstance::Get_Instance()->Key_Down('3'))
		{
			m_iShop_Number_index = 0;
			m_iShop_Catalog_index = 0;
			m_iShopMenu_Index++;
			if (m_iShopMenu_Index > 1)
				m_iShopMenu_Index = 0;
		}
		if (CGameInstance::Get_Instance()->Key_Down('Q'))
		{
			m_iShop_Number_index = 0;
			m_iShop_Catalog_index = 0;
			m_iShopSubMenu_Index--;
			if (m_iShopSubMenu_Index < 0)
				m_iShopSubMenu_Index = 2;
		}
		if (CGameInstance::Get_Instance()->Key_Down('E'))
		{
			m_iShop_Number_index = 0;
			m_iShop_Catalog_index = 0;
			m_iShopSubMenu_Index++;
			if (m_iShopSubMenu_Index > 2)
				m_iShopSubMenu_Index = 0;
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
		{
			if (!m_bBuy_CheckBox)
			{
				m_iShop_Number_index = 0;
				m_iShop_Catalog_index++;
				if (m_iShopSubMenu_Index == 0)
				{
					if (m_iShop_Catalog_index > 8)
						m_iShop_Catalog_index = 0;
				}
				else if (m_iShopSubMenu_Index == 1)
				{
					if (m_iShop_Catalog_index > 5)
						m_iShop_Catalog_index = 0;
				}
				else if (m_iShopSubMenu_Index == 2)
				{
					if (m_iShop_Catalog_index > 2)
						m_iShop_Catalog_index = 0;
				}
			}
			else
			{
				m_iBuy_Check++;
				if (m_iBuy_Check > 1)
					m_iBuy_Check = 0;
			}
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
		{
			if (!m_bBuy_CheckBox)
			{
				m_iShop_Number_index = 0;
				m_iShop_Catalog_index--;
				if (m_iShopSubMenu_Index == 0)
				{
					if (m_iShop_Catalog_index < 0)
						m_iShop_Catalog_index = 8;
				}
				else if (m_iShopSubMenu_Index == 1)
				{
					if (m_iShop_Catalog_index < 0)
						m_iShop_Catalog_index = 5;
				}
				else if (m_iShopSubMenu_Index == 2)
				{
					if (m_iShop_Catalog_index < 0)
						m_iShop_Catalog_index = 2;
				}
			}
			else 
			{
				m_iBuy_Check--;
				if (m_iBuy_Check < 0)
					m_iBuy_Check = 1;
			}
		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
		{
			m_iShop_Number_index++;
			if (m_iShop_Number_index > 99)
				m_iShop_Number_index = 99;

		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
		{
			m_iShop_Number_index--;
			if (m_iShop_Number_index < 0)
				m_iShop_Number_index = 0;

		}

		if (CGameInstance::Get_Instance()->Key_Down(VK_RETURN))
		{
			if (!m_bBuy_Check_Done)
			{
				m_bBuy_CheckBox = !m_bBuy_CheckBox;
				m_bBuy_Check_Done = true;
			}
			else
			{
				m_bBuy_Check_Done = false;
				m_bBuy_CheckBox = !m_bBuy_CheckBox;

				if (m_iBuy_Check == 0)
					m_bBuy = true;
				else if(m_iBuy_Check == 1)
					m_bBuy = false;
			}
		}
	}
}
