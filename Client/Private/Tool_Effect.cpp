#include "ClientPCH.h"
#include "Tool_Effect.h"
#include "Scene_Tool.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Bone.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "EffectInstance.h"
#include "EffectRegular_Visual.h"
#include "EffectRegular_VisualCurve.h"
#include "EffectRegular_VisualDetailCurve.h"
#include "EffectRegular_VisualMesh.h"
#include "EffectRegular_MeshAnim.h"
#include "EffectRegular_Distortion.h"
#include "EffectInstance_ParticlePoint.h"
#include "EffectInstance_ParticlePointCurve.h"
#include "EffectInstance_ParticlePointDynamicCurve.h"
#include "EffectInstance_ParticlePointVectorFieldCurve.h"
#include "EffectInstance_ParticleRect.h"
#include "EffectInstance_ParticleMesh.h"
#include "EffectInstance_ParticlePointTarget.h"
#include "EffectInstance_TrailLineDiffuse.h"
#include "EffectInstance_TrailQuadDiffuse.h"
#include "EffectInstance_MeshAfterImage.h"
#include "Curves.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"
#include "CurvePlot.h"
#include "CurveContainer.h"

#define DIALOG_LOAD				"Dialog:Effect:Load"
#define DIALOG_EXPORT			"Dialog:Effect:Export"

#define DIALOG_LOADTEXTURE		"Dialog:Instance:LoadTexture"
#define DIALOG_LOADEFFECT		"Dialog:Instance:LoadEffect"
#define DIALOG_EXPORTEFFECT		"Dialog:Instance:ExportEffect"
#define DIALOG_EXPORTMSHX		"Dialog:Instance:MeshInstance"

#define DIALOG_TRAILTARGET		"Dialog:TrailTarget"

#if ACTIVATE_TOOL

CTool_Effect::CTool_Effect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: IToolSystem(_pDevice, _pContext)
{
}

HRESULT CTool_Effect::Initialize()
{
	m_pRenderer = CGameInstance::Get_Instance()->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);

	m_pDummyTransform = CTransform::Create(m_pDevice, m_pContext);
	m_pFetchTransform = CTransform::Create(m_pDevice, m_pContext);

	m_vecShaders.emplace_back(CGameInstance::Get_Instance()->Clone_Component<CShader>(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX));
	m_vecShaders.emplace_back(CGameInstance::Get_Instance()->Clone_Component<CShader>(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX));
	m_vecShaders.emplace_back(CGameInstance::Get_Instance()->Clone_Component<CShader>(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE));

	m_pNewCurvePlot = CCurvePlot::Create("New CurvePlot");

	return S_OK;
}

void CTool_Effect::Tick(_float _fTimeDelta)
{
	static _float fSpd = 5.f;

	if (CGameInstance::Get_Instance()->Key_Hold('W'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(1.f, 0.f, 0.f), _fTimeDelta * fSpd);
		}
		else
		{
			m_pDummyTransform->Translate(_float3(0.f, 0.f, _fTimeDelta) * fSpd);
			if (m_pEffect)
			{
				m_pEffect->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Translate(_float3(0.f, 0.f, _fTimeDelta) * fSpd);
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold('A'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(0.f, 1.f, 0.f), -_fTimeDelta * fSpd);
		}
		else
		{
			m_pDummyTransform->Translate(_float3(-_fTimeDelta, 0.f, 0.f) * fSpd);
			if (m_pEffect)
			{
				m_pEffect->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Translate(_float3(-_fTimeDelta, 0.f, 0.f) * fSpd);
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold('S'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(1.f, 0.f, 0.f), -_fTimeDelta * fSpd);
		}
		else
		{
			m_pDummyTransform->Translate(_float3(0.f, 0.f, -_fTimeDelta) * fSpd);
			if (m_pEffect)
			{
				m_pEffect->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Translate(_float3(0.f, 0.f, -_fTimeDelta) * fSpd);
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold('D'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(0.f, 1.f, 0.f), -_fTimeDelta * fSpd);
		}
		else
		{
			m_pDummyTransform->Translate(_float3(_fTimeDelta, 0.f, 0.f) * fSpd);
			if (m_pEffect)
			{
				m_pEffect->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Translate(_float3(_fTimeDelta, 0.f, 0.f) * fSpd);
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold('Q'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(0.f, 0.f, 1.f), _fTimeDelta * fSpd);
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold('E'))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
		{
			m_pFetchTransform->Rotate(_float3(0.f, 0.f, 1.f), -_fTimeDelta * fSpd);
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(VK_OEM_MINUS))
	{
		fSpd /= (1.f + _fTimeDelta);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(VK_OEM_PLUS))
	{
		fSpd *= (1.f + _fTimeDelta);
	}
	if (m_bEffectTick)
	{
		if (m_pEffect)
		{
			if (!m_pEffect->Is_Release())
			{
				m_pEffect->Tick(_fTimeDelta);
			}
		}
	}

	if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (m_pEffect)
		{
			if (!CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT))
			{
			//	m_pEffect->Fetch(_float3(0.f, 0.f, 0.f));
			}
			else
			{
			//	m_pEffect->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, _float3(0.f, 0.f, 0.f));
			}
		}
	}
	
	if (m_pTargetModel)
	{
		m_pTargetModel->Tick_Animation(_fTimeDelta);
	}

	if (CGameInstance::Get_Instance()->Key_Down('U'))
	{
		auto pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Show_Cursor(true);
		pGameInstance->Fix_Cursor(false);
#if ACTIVATE_IMGUI
#endif

		// POSTPROCESS
		pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_TONEMAPPING, 0.01f);
		pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.2f);
		pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_TONEMAPPING, 1.f);

		pGameInstance->Set_BloomStrength(IMAGE_PROCESS::PROCESS_NEON, 0.111f);
		pGameInstance->Set_Exposure(IMAGE_PROCESS::PROCESS_NEON, 2.315f);
		pGameInstance->Set_Gamma(IMAGE_PROCESS::PROCESS_NEON, 0.202f);

		// SSAO
		pGameInstance->Enable_SSAO(true);

		// LUT Filter
		pGameInstance->Enable_LUTFilter(true);
		pGameInstance->Set_LUTIndex(1);

		// Lensflare
		pGameInstance->Enable_Lensflare(false);
		pGameInstance->Set_SunPosition(_float3(2000.f, 10.f, 3000.f));
		pGameInstance->Set_SunColor(_float3(1.4f, 1.2f, 1.0f));
		pGameInstance->Set_LensflareStrength(2.0f);

		// DOF
		pGameInstance->Enable_DOF(true);
		pGameInstance->Set_DOF_DepthStart(160.f);
		pGameInstance->Set_DOF_DepthRange(200.f);

		pGameInstance->Enable_FXAA(true);
	}

#ifdef TOOL_DEMO
	if (m_bDemoPlot)
	{
		Demo_Plot();
	}
	if (m_bDemoCurve)
	{
		Demo_Curve();
	}
	if (m_bDemoEase)
	{
		Demo_Ease();
	}
#endif
}

void CTool_Effect::Late_Tick(_float _fTimeDelta)
{
//	if (m_bEffectTick)
	{
		if (m_pEffect)
		{
			m_pEffect->Late_Tick(_fTimeDelta);
		}
	}

	if (RENDER_GROUP::MAX != m_eModelRenderGroup && m_pModelShader)
	{
		m_pRenderer->Add_RenderObject(m_eModelRenderGroup, shared_from_this());
	}
}

HRESULT CTool_Effect::Render()
{
	if (m_pTargetModel)
	{
		if (FAILED(Bind_DummyMaterials()))
		{
			MSG_RETURN(E_FAIL, "CTool_Effect::Render", "Failed to Bind_DummyMaterials");
		}
		if (FAILED(m_pDummyTransform->Bind_OnShader(m_pModelShader)))
		{
			MSG_RETURN(E_FAIL, "CTool_Effect::Render", "Failed to CTransform::Bind_OnShader");
		}
		if (FAILED(m_pTargetModel->Render(m_pModelShader, m_iModelShaderPass)))
		{
			MSG_RETURN(E_FAIL, "CTool_Effect::Render", "Failed to CModel::Render");
		}
	}

	return S_OK;
}

void CTool_Effect::Tool_MenuBar()
{
	if (ImGui::BeginMenu("Manager##CTool_Effect::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("Save##CTool_Effect::Tool_MenuBar"))
		{
			Save_Effects("..");
		}

		if (ImGui::BeginMenu("Load##CTool_Effect::Tool_MenuBar"))
		{
			if (ImGui::MenuItem("Visual"))
			{
				Load_Effects(EFFECT::VFX_Visual);
			}
			if (ImGui::MenuItem("Distortion"))
			{
				Load_Effects(EFFECT::VFX_Distortion);
			}
			if (ImGui::MenuItem("ParticlePoint"))
			{
				Load_Effects(EFFECT::VFX_ParticlePoint);
			}
			if (ImGui::MenuItem("TrailLineDiffuse"))
			{
				Load_Effects(EFFECT::VFX_TrailLineDiffuse);
			}
			if (ImGui::MenuItem("TrailQuad"))
			{
				Load_Effects(EFFECT::VFX_TrailQuad);
			}
			if (ImGui::MenuItem("MeshAfterImage"))
			{
				Load_Effects(EFFECT::VFX_MeshAfterImage);
			}
			if (ImGui::MenuItem("ParticlePointTarget"))
			{
				Load_Effects(EFFECT::VFX_ParticlePointTarget);
			}
			if (ImGui::MenuItem("VisualMesh"))
			{
				Load_Effects(EFFECT::VFX_VisualMesh);
			}
			if (ImGui::MenuItem("ParticleMesh"))
			{
				Load_Effects(EFFECT::VFX_ParticleMesh);
			}
			if (ImGui::MenuItem("ParticleRect"))
			{
				Load_Effects(EFFECT::VFX_ParticleRect);
			}
			if (ImGui::MenuItem("ParticlePointCurve"))
			{
				Load_Effects(EFFECT::VFX_ParticlePointCurve);
			}
			if (ImGui::MenuItem("VisualCurve"))
			{
				Load_Effects(EFFECT::VFX_VisualCurve);
			}
			if (ImGui::MenuItem("ParticlePointDynamicCurve"))
			{
				Load_Effects(EFFECT::VFX_ParticlePointDynamicCurve);
			}
			if (ImGui::MenuItem("MeshAnim"))
			{
				Load_Effects(EFFECT::VFX_MeshAnim);
			}
			if (ImGui::MenuItem("ParticlePointVectorFieldCurve"))
			{
				Load_Effects(EFFECT::VFX_ParticlePointVectorFieldCurve);
			}
			if (ImGui::MenuItem("VisualDetailCurve"))
			{
				Load_Effects(EFFECT::VFX_VisualDetailCurve);
			}
			if (ImGui::MenuItem("ParticleLine"))
			{
				Load_Effects(EFFECT::VFX_ParticleLine);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Effect Shader##CTool_Effect::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("PosTexVfx##CTool_Effect::Tool_MenuBar"))
		{
			m_pEffectShader = m_vecShaders[0];
		}

		ImGui::Separator();

		if (ImGui::MenuItem("InstPointVfx##CTool_Effect::Tool_MenuBar"))
		{
			m_pEffectShader = m_vecShaders[1];
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Model Shader##CTool_Effect::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("VTXMESHANIM##CTool_Effect::Tool_MenuBar"))
		{
			m_pModelShader = m_vecShaders[2];
		}

		ImGui::EndMenu();
	}

	if (m_pModelShader)
	{
		if (ImGui::BeginMenu("Model Shader Pass##CTool_Effect::Tool_MenuBar"))
		{
			for (_uint i = 0; i < m_pModelShader->Get_NumPasses(); ++i)
			{
				if (ImGui::MenuItem(string("ShaderPass " + std::to_string(i) + "##CTool_Effect::Tool_MenuBar").c_str()))
				{
					m_iModelShaderPass = i;
				}
			}

			ImGui::EndMenu();
		}
	}

	if (ImGui::BeginMenu("Model Render Group##CTool_Effect::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("Camera##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::CAMERA;
		}
		if (ImGui::MenuItem("Priority##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::PRIORITY;
		}
		if (ImGui::MenuItem("Shadow##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::SHADOW;
		}
		if (ImGui::MenuItem("NonBlend##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::NONBLEND;
		}
		if (ImGui::MenuItem("Outline##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::OUTLINE;
		}
		if (ImGui::MenuItem("Decal##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::DECAL;
		}
		if (ImGui::MenuItem("NonLight##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::NONLIGHT;
		}
		if (ImGui::MenuItem("Blend##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::BLEND;
		}
		if (ImGui::MenuItem("UI##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::UI;
		}
#ifdef _DEBUG	// ?
		if (ImGui::MenuItem("Debug##CTool_Effect::Tool_MenuBar"))
		{
			m_eModelRenderGroup = RENDER_GROUP::DEBUG;
		}
#endif
		ImGui::EndMenu();
	}

	if (m_pTargetModel && m_pModelShader)
	{
		if (ImGui::BeginMenu("Animation##CTool_Effect::Tool_MenuBar"))
		{
			for (_uint i = 0; i < m_pTargetModel->Get_NumAnimations(); ++i)
			{
				if (ImGui::MenuItem(m_pTargetModel->Get_Animation(i)->Get_Name()))
				{
					m_pTargetModel->Set_Animation(i);
					m_iAnimationIndex = i;
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Bone0##CTool_Effect::Tool_MenuBar"))
		{
			for (_uint i = 0; i < m_pTargetModel->Get_NumBones(); ++i)
			{
				if (ImGui::MenuItem(m_pTargetModel->Get_Bone(i)->Get_Name()))
				{
					strcpy_s(m_szBone0, m_pTargetModel->Get_Bone(i)->Get_Name());
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Bone1##CTool_Effect::Tool_MenuBar"))
		{
			for (_uint i = 0; i < m_pTargetModel->Get_NumBones(); ++i)
			{
				if (ImGui::MenuItem(m_pTargetModel->Get_Bone(i)->Get_Name()))
				{
					strcpy_s(m_szBone1, m_pTargetModel->Get_Bone(i)->Get_Name());
				}
			}

			ImGui::EndMenu();
		}
	}

	if (ImGui::BeginMenu("Macro##CTool_Effect::Tool_MenuBar"))
	{
		if (ImGui::BeginMenu("Texture##CTool_Effect::Tool_MenuBar"))
		{
			if (ImGui::BeginMenu("Noise##CTool_Effect::Tool_MenuBar"))
			{
				if (ImGui::MenuItem("Macro1##CTool_Effect::Tool_MenuBar"))
				{
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_mip_noi_001.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_003.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_009.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_016.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_022.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_036.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_040.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_042.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_047.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_053.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_054.dds");
					Load_LoadTexture("Bin/Resources/Texture/Effect/Noise/T_ef_scl_noi_056.dds");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

#ifdef TOOL_DEMO
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Plot"))
		{
			m_bDemoPlot = !m_bDemoPlot;
		}
		if (ImGui::MenuItem("Curves"))
		{
			m_bDemoCurve = !m_bDemoCurve;
		}
		if (ImGui::MenuItem("Ease"))
		{
			m_bDemoEase = !m_bDemoEase;
		}

		ImGui::EndMenu();
	}
#endif
}

void CTool_Effect::Tool_System()
{
#pragma region File Dialog

	if (ImGui::Button("Load##CTool_Effect::Tool_System"))
	{
		if (!m_imfdEmbed_Load.IsOpened())
		{
			if (m_imfdEmbed_Export.IsOpened())
			{
				m_imfdEmbed_Export.Close();
			}

			const _char* szFilters = "DDS files(*.dds){.dds},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},Mesh files(*.msh *mshx){.msh,.mshx},All files{.*}";
			m_imfdEmbed_Load.OpenDialog(DIALOG_LOAD, "Open", szFilters, "Bin/Resources/", 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ReadOnlyFileNameField);
		}
		else
		{
			m_imfdEmbed_Load.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Export##CTool_Effect::Tool_System"))
	{
		if (!m_imfdEmbed_Export.IsOpened())
		{
			if (m_imfdEmbed_Load.IsOpened())
			{
				m_imfdEmbed_Load.Close();
			}

			const _char* szFilters = "DDS files(*.dds){.dds},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},All files{.*}";
			m_imfdEmbed_Export.OpenDialog(DIALOG_EXPORT, "Export", szFilters, "../Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				0, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType			|
				ImGuiFileDialogFlags_NoDialog				|
				ImGuiFileDialogFlags_DisableBookmarkMode	|
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imfdEmbed_Export.Close();
		}
	}

	ImGui::Separator();
#pragma region File Dialog Instance

	if (m_imfdEmbed_Load.Display(DIALOG_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, DIALOG_HEIGHT)))
	{
		if (m_imfdEmbed_Load.IsOk())
		{
			string strExt;
			Function::SplitPath(m_imfdEmbed_Load.GetFilePathName(), nullptr, nullptr, nullptr, &strExt);

			if (string(".dds")	== strExt
			||	string(".png")	== strExt
			||	string(".jpg")	== strExt
			||	string(".jpeg")	== strExt)
			{
				if (FAILED(Load_LoadTexture(m_imfdEmbed_Load.GetFilePathName())))
				{
					MSG_BOX("CTool_Effect::Tool_System", "Failed to Load_LoadTexture");
				}
			}
			else if (string(".msh") == strExt)
			{
				if (FAILED(Load_LoadMesh(m_imfdEmbed_Load.GetFilePathName())))
				{
					MSG_BOX("CTool_Effect::Tool_System", "Failed to Load_LoadMesh");
				}
			}
			else if (string(".mshx") == strExt)
			{
				if (FAILED(Load_LoadMeshInstance(m_imfdEmbed_Load.GetFilePathName())))
				{
					MSG_BOX("CTool_Effect::Tool_System", "Failed to Load_LoadMeshInstance");
				}
			}
		}
	}

	if (m_imfdEmbed_Export.Display(DIALOG_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, DIALOG_HEIGHT)))
	{
		if (m_imfdEmbed_Export.IsOk())
		{

		}
	}

#pragma endregion
	if (m_imfdEmbed_Load.IsOpened() || m_imfdEmbed_Export.IsOpened())
	{
		ImGui::Separator();
	}

#pragma endregion
#pragma region List

	if (ImGui::CollapsingHeader("Texture##CTool_Effect::Tool_System"))
	{
		static _bool bOpenLoadTextureModal(false);

		if (ImGui::TreeNode("LoadTexture##CTool_Effect::Tool_System"))
		{
			if (-1 != m_iLoadTextureCursor)
			{
				ImVec2 imgSize;
				
				D3D11_TEXTURE2D_DESC tTexture2dDesc{};
				m_vecLoadTextures[m_iLoadTextureCursor]->Get_Texture2D()->GetDesc(&tTexture2dDesc);
				
				_float fWidth	= static_cast<_float>(tTexture2dDesc.Width);
				_float fHeight	= static_cast<_float>(tTexture2dDesc.Height);
				_float fAspect	= fWidth / fHeight;
				
				if (fAspect > 1.f)
				{
					imgSize.x = 300.f;
					imgSize.y = 300.f / fAspect;
				}
				else
				{
					imgSize.x = 300.f * fAspect;
					imgSize.y = 300.f;
				}
				
				ImGui::Image(m_vecLoadTextures[m_iLoadTextureCursor]->Get_ShaderResourceView().Get(), imgSize);
			}

			ImGui::TreePop();
		}

		if (ImGui::BeginListBox("LoadTexture##CTool_Effect::Tool_System", ImVec2(-FLT_MIN, 0.f)))
		{
			for (size_t i = 0; i < m_vecLoadTextures.size(); ++i)
			{
				string strLoadTextureTag;
				if (m_vecLoadTextureTags[i].empty())
				{
					strLoadTextureTag = string("LoadTexture " + std::to_string(i));
				}
				else
				{
					strLoadTextureTag = m_vecLoadTextureTags[i];
				}

				if (ImGui::Selectable(strLoadTextureTag.c_str(), static_cast<_int>(i) == m_iLoadTextureCursor, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_iLoadTextureCursor = static_cast<_int>(i);
					m_pLoadTexture = m_vecLoadTextures[i];

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						bOpenLoadTextureModal = true;
					}
				}
			}

			ImGui::EndListBox();
		}

		if (ImGui::Button("Push SpriteSheet##CTool_Effect::Tool_System"))
		{
			Push_SpriteSheet();
		}
#pragma region Modal

		if (bOpenLoadTextureModal)
		{
			ImGui::OpenPopup("Mod LoadTexture##CTool_Effect::Tool_System");
		}

		if (ImGui::BeginPopupModal("Mod LoadTexture##CTool_Effect::Tool_System", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static _char szModLoadTextureTag[MAX_PATH] = "Tag";

			ImGui::Text("Change Tag:");
			ImGui::InputText("Tag", szModLoadTextureTag, IM_ARRAYSIZE(szModLoadTextureTag));
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0)))
			{
				m_vecLoadTextureTags[m_iLoadTextureCursor] = szModLoadTextureTag;

				bOpenLoadTextureModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
			{
				bOpenLoadTextureModal = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

#pragma endregion
#pragma endregion
#pragma region Mesh

	static _bool bOpenLoadMeshModal(false);

	if (ImGui::CollapsingHeader("Mesh##CTool_Effect::Tool_System"))
	{
		if (ImGui::BeginListBox("LoadMesh##CTool_Effect::Tool_System", ImVec2(-FLT_MIN, 0.f)))
		{
			for (size_t i = 0; i < m_vecLoadMeshes.size(); ++i)
			{
				string strLoadMeshTag;
				if (m_vecLoadMeshTags[i].empty())
				{
					strLoadMeshTag = string("Mesh " + std::to_string(i));
				}
				else
				{
					strLoadMeshTag = m_vecLoadMeshTags[i];
				}

				if (ImGui::Selectable(strLoadMeshTag.c_str(), static_cast<_int>(i) == m_iLoadMeshCursor, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_iLoadMeshCursor = static_cast<_int>(i);
					m_pLoadMesh = m_vecLoadMeshes[i];

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						bOpenLoadMeshModal = true;
					}
				}

				if (ImGui::BeginPopupContextItem())
				{
					static _int iNumInstance(100);
					if (ImGui::Button("Create Instance##CTool_Effect::Tool_System"))
					{
						m_vecLoadMeshInstances.emplace_back(CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, MODEL::NONANIM, m_pLoadMesh, iNumInstance));
						m_vecLoadMeshInstanceTags.emplace_back(string("MeshInstance " + std::to_string(m_vecLoadMeshInstances.size() - 1)));
					}
					ImGui::SameLine();
					ImGui::InputInt("Num Instance##CTool_Effect::Tool_System", &iNumInstance);
					ImGui::EndPopup();
				}
			}

			ImGui::EndListBox();
		}

#pragma region Modal

		if (bOpenLoadMeshModal)
		{
			ImGui::OpenPopup("Mod LoadMesh##CTool_Effect::Tool_System");
		}

		if (ImGui::BeginPopupModal("Mod LoadMesh##CTool_Effect::Tool_System", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static _char szModLoadMeshTag[MAX_PATH] = "Tag";

			ImGui::Text("Change Tag:");
			ImGui::InputText("Tag", szModLoadMeshTag, IM_ARRAYSIZE(szModLoadMeshTag));
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0)))
			{
				m_vecLoadMeshTags[m_iLoadMeshCursor] = szModLoadMeshTag;

				bOpenLoadMeshModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
			{
				bOpenLoadMeshModal = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
#pragma endregion
#pragma endregion
#pragma region MeshInstance

	static _bool bOpenLoadMeshInstanceModal(false);

	if (ImGui::CollapsingHeader("MeshInstance##CTool_Effect::Tool_System"))
	{
		if (ImGui::BeginListBox("LoadMeshInstance##CTool_Effect::Tool_System", ImVec2(-FLT_MIN, 0.f)))
		{
			for (size_t i = 0; i < m_vecLoadMeshInstances.size(); ++i)
			{
				string strLoadMeshInstanceTag;
				if (m_vecLoadMeshInstanceTags[i].empty())
				{
					strLoadMeshInstanceTag = string("MeshInstance " + std::to_string(i));
				}
				else
				{
					strLoadMeshInstanceTag = m_vecLoadMeshInstanceTags[i];
				}

				if (ImGui::Selectable(strLoadMeshInstanceTag.c_str(), static_cast<_int>(i) == m_iLoadMeshInstanceCursor, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_iLoadMeshInstanceCursor = static_cast<_int>(i);
					m_pLoadMeshInstance = m_vecLoadMeshInstances[i];

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						bOpenLoadMeshInstanceModal = true;
					}
				}

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Export##CTool_Effect::Tool_System"))
					{
						const _char* szFilters = "MeshInstance (*.mshx){.mshx},All files{.*}";
						ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORTMSHX, "Export MeshInstance", szFilters, "../Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
					}
					ImGui::EndPopup();
				}
			}

			ImGui::EndListBox();
		}

#pragma region Modal

		if (bOpenLoadMeshInstanceModal)
		{
			ImGui::OpenPopup("Mod LoadMeshInstance##CTool_Effect::Tool_System");
		}

		if (ImGui::BeginPopupModal("Mod LoadMeshInstance##CTool_Effect::Tool_System", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static _char szModLoadMeshInstanceTag[MAX_PATH] = "Tag";

			ImGui::Text("Change Tag:");
			ImGui::InputText("Tag", szModLoadMeshInstanceTag, IM_ARRAYSIZE(szModLoadMeshInstanceTag));
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0)))
			{
				m_vecLoadMeshInstanceTags[m_iLoadMeshInstanceCursor] = szModLoadMeshInstanceTag;

				bOpenLoadMeshInstanceModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
			{
				bOpenLoadMeshInstanceModal = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
#pragma endregion
#pragma region Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_EXPORTMSHX))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(Export_MeshInstance(ImGuiFileDialog::Instance()->GetFilePathName())))
			{
				MSG_BOX("CTool_Effect::Tool_System", "Failed to Export_MeshInstance");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
#pragma endregion
#pragma region Plot

	if (ImGui::CollapsingHeader("Curves##CTool_Effect::Tool_System:Header"))
	{
		if (ImGui::BeginTabBar("Curves##CTool_Effect::Tool_System:Tabs"))
		{
			if (ImGui::BeginTabItem("New##CTool_Effect::Tool_System:Curves"))
			{
				m_pNewCurvePlot->Tick();

				if (ImGui::Button("Push##CTool_Effect::Tool_System:Curves:New"))
				{
					m_vecCurvePlots.emplace_back(m_pNewCurvePlot->Clone());
					if (m_vecCurvePlotTags.end() != std::find(m_vecCurvePlotTags.begin(), m_vecCurvePlotTags.end(), m_vecCurvePlots.back()->Get_Name()))
					{
						m_vecCurvePlots.back()->Set_Name(string(m_vecCurvePlots.back()->Get_Name() + string(".")));
					}
					m_vecCurvePlotTags.emplace_back(m_vecCurvePlots.back()->Get_Name());
				}

				ImGui::EndTabItem();
			}
			if (m_pCurrentCurvePlot)
			{
				if (ImGui::BeginTabItem("Edit##CTool_Effect::Tool_System:Curves"))
				{
					m_pCurrentCurvePlot->Tick();

					if (ImGui::Button("Push##CTool_Effect::Tool_System:Curves:Edit"))
					{
						m_vecCurvePlots.emplace_back(m_pCurrentCurvePlot->Clone());
						if (m_vecCurvePlotTags.end() != std::find(m_vecCurvePlotTags.begin(), m_vecCurvePlotTags.end(), m_vecCurvePlots.back()->Get_Name()))
						{
							m_vecCurvePlots.back()->Set_Name(string(m_vecCurvePlots.back()->Get_Name() + string(".")));
						}
						m_vecCurvePlotTags.emplace_back(m_vecCurvePlots.back()->Get_Name());
					}

					ImGui::EndTabItem();
				}
			}
			if (ImGui::BeginTabItem("List##CTool_Effect::Tool_System::Curves"))
			{
				if (ImGui::BeginListBox("CurvePlot List##CTool_Effect::Tool_System::Curves", ImVec2(-FLT_MIN, 0.f)))
				{
					for (size_t i = 0; i < m_vecCurvePlots.size(); ++i)
					{
						string strCurvePlotTag;
						if (m_vecCurvePlotTags[i].empty())
						{
							strCurvePlotTag = string("Curve " + std::to_string(i));
						}
						else
						{
							strCurvePlotTag = m_vecCurvePlotTags[i];
						}

						if (ImGui::Selectable(strCurvePlotTag.c_str(), static_cast<_int>(i) == m_iCurvePlotCursor, ImGuiSelectableFlags_AllowDoubleClick))
						{
							m_iCurvePlotCursor = static_cast<_int>(i);

							m_pCurrentCurvePlot = m_vecCurvePlots[i];
						}
					}

					ImGui::EndListBox();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

#pragma endregion
}

void CTool_Effect::Tool_Properties()
{
#pragma region Sprite Sheet

	static _int		iSpriteSheetIndex(0);
	static _int2	vSpriteSheetSize(_int2(1, 1));

	if (ImGui::CollapsingHeader("Sprite Sheet##CTool_Effect::Tool_Properties"))
	{
		static _bool	bOpenSpriteSheetModal(false);

		if (ImGui::TreeNode("Sprite Sheet##CTool_Effect::Tool_Properties"))
		{
			if (-1 != m_iSpriteSheetCursor)
			{
				ImVec2 imgSize;

				D3D11_TEXTURE2D_DESC tTexture2dDesc{};
				m_vecSpriteSheets[m_iSpriteSheetCursor]->Get_Texture2D()->GetDesc(&tTexture2dDesc);

				_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
				_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
				_float fAspect = fWidth / fHeight;

				if (fAspect > 1.f)
				{
					imgSize.x = 300.f;
					imgSize.y = 300.f / fAspect;
				}
				else
				{
					imgSize.x = 300.f * fAspect;
					imgSize.y = 300.f;
				}

				ImGui::Image(m_vecSpriteSheets[m_iSpriteSheetCursor]->Get_ShaderResourceView().Get(), imgSize);
			}

			ImGui::TreePop();
		}

		if (ImGui::BeginListBox("SpriteSheet##CTool_Effect::Tool_Properties", ImVec2(-FLT_MIN, 0.f)))
		{
			for (size_t i = 0; i < m_vecSpriteSheets.size(); ++i)
			{
				string strSpriteSheetTag;
				if (m_vecSpriteSheetTags[i].empty())
				{
					strSpriteSheetTag = string("SpriteSheet " + std::to_string(i));
				}
				else
				{
					strSpriteSheetTag = m_vecSpriteSheetTags[i];
				}

				if (ImGui::Selectable(strSpriteSheetTag.c_str(), static_cast<_int>(i) == m_iSpriteSheetCursor, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_iSpriteSheetCursor = static_cast<_int>(i);
					m_pSpriteSheet = m_vecSpriteSheets[i];

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						bOpenSpriteSheetModal = true;
					}
				}
			}

			ImGui::EndListBox();
		}

		if (ImGui::InputInt2("SpriteSheet Size##CTool_Effect::Tool_Properties", reinterpret_cast<_int*>(&vSpriteSheetSize)))
		{
			vSpriteSheetSize.x = max(0, vSpriteSheetSize.x);
			vSpriteSheetSize.y = max(0, vSpriteSheetSize.y);

			if (m_pEffectShader)
			{
				if (FAILED(m_pEffectShader->Bind_IntArray(SHADER_SPRITESHEET_SIZE, reinterpret_cast<_int*>(&vSpriteSheetSize), 2)))
				{
					MSG_BOX("CTool_Effect::Tool_Properties", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
				}
			}
		}
#pragma region Modal

		if (bOpenSpriteSheetModal)
		{
			ImGui::OpenPopup("Mod SpriteSheet##CTool_Effect::Tool_Properties");
		}

		if (ImGui::BeginPopupModal("Mod SpriteSheet##CTool_Effect::Tool_Properties"))
		{
			static _char szModSpriteSheetTag[MAX_PATH] = "Tag";

			ImGui::Text("Change Tag:");
			ImGui::InputText("Tag", szModSpriteSheetTag, IM_ARRAYSIZE(szModSpriteSheetTag));
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0)))
			{
				m_vecSpriteSheetTags[m_iSpriteSheetCursor] = szModSpriteSheetTag;

				bOpenSpriteSheetModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
			{
				bOpenSpriteSheetModal = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

#pragma endregion
	}

#pragma endregion
#pragma region Effects

	if (ImGui::CollapsingHeader("Effects##CToolEffect::Tool_Properties"))
	{
#pragma region Effect Regular
#pragma region List

		if (ImGui::TreeNode("Effect Regular##CTool_Effect::Tool_Properties"))
		{
			static _bool	bOpenEffectRegularModal(false);

			if (ImGui::TreeNode("List##EffectRegular##CTool_Effect::Tool_Properties"))
			{
				if (ImGui::BeginListBox("Effect Regular List##CTool_Effect::Tool_Properties", ImVec2(-FLT_MIN, 0.f)))
				{
					for (size_t i = 0; i < m_vecEffectRegulars.size(); ++i)
					{
						string strEffectRegularTag;
						if (m_vecEffectRegularTags[i].empty())
						{
							strEffectRegularTag = string("EffectRegular" + std::to_string(i));
						}
						else
						{
							strEffectRegularTag = m_vecEffectRegularTags[i];
						}

						if (ImGui::Selectable(strEffectRegularTag.c_str(), static_cast<_int>(i) == m_iEffectRegularCursor, ImGuiSelectableFlags_AllowDoubleClick))
						{
							m_iEffectRegularCursor	= static_cast<_int>(i);
							m_pEffectRegular		= m_vecEffectRegulars[i];
							m_pEffect				= m_pEffectRegular;
							m_bEffectTick			= false;

							if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
							{
								bOpenEffectRegularModal = true;
							}
						}

						if (ImGui::BeginPopupContextItem())
						{
							m_iEffectRegularCursor	= static_cast<_int>(i);
							m_pEffectRegular		= m_vecEffectRegulars[i];
							m_pEffect				= m_pEffectRegular;
						//	m_bEffectTick			= false;

							static MATERIALDESC	tMtrlDesc{};

							tMtrlDesc = m_vecEffectRegulars[i]->Get_Material();

							ImGui::Text(strEffectRegularTag.c_str());
							switch (m_vecEffectRegulars[i]->Get_Type())
							{
							case EFFECT::VFX_Visual:
							{
								shared_ptr<CEffectRegular_Visual> pInstance = dynamic_pointer_cast<CEffectRegular_Visual>(m_vecEffectRegulars[i]);
								VFX_Visual tDesc = pInstance->Get_Data();

							//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_Visual");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_Visual"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}
								ImGui::Separator();
								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_Visual",&tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Fade##VFX_Visual", &tDesc.bFade))
								{
									pInstance->Set_Data(tDesc);
								}
								if (tDesc.bFade)
								{
									if (ImGui::InputFloat2("Fade Scope##VFX_Visual", reinterpret_cast<_float*>(&tDesc.vFadeScope)))
									{
										pInstance->Set_Data(tDesc);
									}
								}
								if (ImGui::InputFloat("Bloom##VFX_Visual", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("BloomPass##VFX_Visual", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_Visual", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_Visual", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_Distortion:
							{
								shared_ptr<CEffectRegular_Distortion> pInstance = dynamic_pointer_cast<CEffectRegular_Distortion>(m_vecEffectRegulars[i]);
								VFX_Distortion tDesc = pInstance->Get_Data();

							//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_Distortion");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									ImVec2 imgSize;

									D3D11_TEXTURE2D_DESC tTexture2dDesc{};
									tDesc.pTexture->Get_Texture2D()->GetDesc(&tTexture2dDesc);

									_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
									_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
									_float fAspect = fWidth / fHeight;

									if (fAspect > 1.f)
									{
										imgSize.x = 100.f;
										imgSize.y = 100.f / fAspect;
									}
									else
									{
										imgSize.x = 100.f * fAspect;
										imgSize.y = 100.f;
									}

									ImGui::Image(tDesc.pTexture->Get_ShaderResourceView().Get(), imgSize);

									if (ImGui::Button("Change##VFX_Distortion"))
									{
										if (m_pSpriteSheet)
										{
											tDesc.pTexture = m_pSpriteSheet;
											pInstance->Set_Data(tDesc);
										}
									}

									ImGui::TreePop();
								}
								ImGui::Separator();
								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_Distortion", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Distortion Power##VFX_Distortion", &tDesc.fDistortionPower))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Bloom##VFX_Distortion", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("BloomPass##VFX_Distortion", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_Distortion", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_Distortion", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_VisualMesh:
							{
								shared_ptr<CEffectRegular_VisualMesh> pInstance = dynamic_pointer_cast<CEffectRegular_VisualMesh>(m_vecEffectRegulars[i]);
								VFX_VisualMesh tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_VisualMesh");

								ImGui::Separator();

								if (ImGui::TreeNode("Mesh##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::Button("Change##VFX_VisualMesh"))
									{
										tDesc.pMesh = m_pLoadMesh;
										pInstance->Set_Data(tDesc);
									}
									ImGui::SameLine();
									if (tDesc.pMesh)
									{
										ImGui::Text(tDesc.pMesh->Get_Name());
									}
									else
									{
										ImGui::Text("Mesh Unloaded");
									}

									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_Visual"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("ExtraMask"))
									{
										if (tDesc.pExtraMask)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pExtraMask->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pExtraMask->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pExtraMask = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_Visual"))
										{
											tDesc.pExtraMask = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Mask"))
									{
										if (tDesc.pMask)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pMask->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pMask->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_Visual"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pMask = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_Visual"))
										{
											tDesc.pMask = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_VisualMesh", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("RollPitchYaw##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vRollPitchYaw)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("UV Speed##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffsetSpeed)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Speed1##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffsetSpeed1)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Speed2##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffsetSpeed2)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Speed3##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffsetSpeed3)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("UV Offset##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffset)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Offset1##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffset1)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Offset2##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffset2)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Offset3##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vUVOffset3)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Diffuse Tiling##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vDiffuseTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Emissive Tiling##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vEmissiveTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Specular Tiling##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vSpecularTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Mask Tiling##VFX_VisualMesh", reinterpret_cast<_float*>(&tDesc.vMaskTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Mask Alpha##VFX_VisualMesh", &tDesc.bMaskAlpha))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("ExtraMask Alpha##VFX_VisualMesh", &tDesc.bExtraMaskAlpha))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Reverse GrayScale0##VFX_VisualMesh", &tDesc.bReverseGrayScale0))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Reverse GrayScale1##VFX_VisualMesh", &tDesc.bReverseGrayScale1))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("SRT##VFX_VisualMesh", &tDesc.bSRT))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("RotateWorldAxis##VFX_VisualMesh", &tDesc.bRotateWorldAxis))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Billboard##VFX_VisualMesh", &tDesc.bBillboard))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("BloomPass##VFX_VisualMesh", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_VisualMesh", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_VisualCurve:
							{
								shared_ptr<CEffectRegular_VisualCurve> pInstance = dynamic_pointer_cast<CEffectRegular_VisualCurve>(m_vecEffectRegulars[i]);
								VFX_VisualCurve tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_VisualCurve");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_VisualCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_VisualCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_VisualCurve"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_VisualCurve", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Distortion Power##VFX_VisualCurve", &tDesc.fDistortionPower))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Diffuse Tiling##VFX_VisualCurve", reinterpret_cast<_float*>(&tDesc.fDiffuseTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Emissive Tiling##VFX_VisualCurve", reinterpret_cast<_float*>(&tDesc.fEmissiveTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_VisualCurve", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("BloomPass##VFX_VisualCurve", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_VisualCurve", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_VisualDetailCurve:
							{
								shared_ptr<CEffectRegular_VisualDetailCurve> pInstance = dynamic_pointer_cast<CEffectRegular_VisualDetailCurve>(m_vecEffectRegulars[i]);
								VFX_VisualDetailCurve tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_VisualDetailCurve");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_VisualCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_VisualCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_VisualCurve"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Mask"))
									{
										if (tDesc.pMask)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pMask->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pMask->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_VisualCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pMask = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_VisualCurve"))
										{
											tDesc.pMask = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_VisualCurve", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Distortion Power##VFX_VisualCurve", &tDesc.fDistortionPower))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Diffuse Tiling##VFX_VisualCurve", reinterpret_cast<_float*>(&tDesc.fDiffuseTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Emissive Tiling##VFX_VisualCurve", reinterpret_cast<_float*>(&tDesc.fEmissiveTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Local##VFX_VisualCurve", &tDesc.bLocal))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_VisualCurve", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("BloomPass##VFX_VisualCurve", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_VisualCurve", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_MeshAnim:
							{
								shared_ptr<CEffectRegular_MeshAnim> pInstance = dynamic_pointer_cast<CEffectRegular_MeshAnim>(m_vecEffectRegulars[i]);
								VFX_MeshAnim tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_MeshAnim");

								ImGui::Separator();

								if (ImGui::TreeNode("Mesh##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									for (size_t i = 0; i < tDesc.vecMesh.size(); ++i)
									{
										if (ImGui::Button(string("Change##VFX_MeshAnim" + std::to_string(i)).c_str()))
										{
											tDesc.vecMesh[i] = m_pLoadMesh;
											pInstance->Set_Data(tDesc);
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_MeshAnim"))
										{
											tDesc.vecMesh[i] = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::SameLine();
										if (ImGui::Button("Remove##VFX_MeshAnim"))
										{
											tDesc.vecMesh.erase(std::next(tDesc.vecMesh.begin(), i));
											pInstance->Set_Data(tDesc);
											continue;
										}
										ImGui::SameLine();
										if (tDesc.vecMesh[i])
										{
											ImGui::Text(tDesc.vecMesh[i]->Get_Name());
										}
										else
										{
											ImGui::Text("Mesh Unloaded");
										}
									}

									if (ImGui::Button("Add Slot##VFX_MeshAnim"))
									{
										tDesc.vecMesh.emplace_back(m_pLoadMesh);
										pInstance->Set_Data(tDesc);
									}

									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Image##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_MeshAnim"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_MeshAnim"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_MeshAnim"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_MeshAnim", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("UV Speed##VFX_MeshAnim", reinterpret_cast<_float*>(&tDesc.vUVOffsetSpeed)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Diffuse Tiling##VFX_MeshAnim", reinterpret_cast<_float*>(&tDesc.fDiffuseTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Emissive Tiling##VFX_MeshAnim", reinterpret_cast<_float*>(&tDesc.fEmissiveTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("BloomPass##VFX_MeshAnim", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("ShaderPass##VFX_MeshAnim", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							}
							ImGui::Separator();

							ImGui::Text("Material");
							if (ImGui::InputFloat4("Diffuse##EffectRegular##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vDiffuse)))
							{
								m_vecEffectRegulars[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Ambient##EffectRegular##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vAmbient)))
							{
								m_vecEffectRegulars[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Specular##EffectRegular##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vSpecular)))
							{
								m_vecEffectRegulars[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Emissive##EffectRegular##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vEmissive)))
							{
								m_vecEffectRegulars[i]->Set_Material(tMtrlDesc);
							}

							ImGui::Separator();

							if (ImGui::Button("Export##VFX_Visual##CTool_Effect::Tool_Properties"))
							{
								const _char* szFilters = "VFX (*.vfx){.vfx},All files{.*}";
								ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORTEFFECT, "Export Effect", szFilters, "../Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
							}
							ImGui::SameLine();
							if (ImGui::Button("Close"))
							{
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}
					}

					ImGui::EndListBox();
				}

				ImGui::TreePop();
			}

#pragma endregion
#pragma region Modal

			if (bOpenEffectRegularModal)
			{
				ImGui::OpenPopup("Mod EffectRegular##CTool_Effect::Tool_Properties");
			}

			if (ImGui::BeginPopupModal("Mod EffectRegular##CTool_Effect::Tool_Properties"))
			{
				static _char szModEffectRegularTag[MAX_PATH] = "Tag";

				ImGui::Text("Change Tag:");
				ImGui::InputText("Tag", szModEffectRegularTag, IM_ARRAYSIZE(szModEffectRegularTag));
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120.f, 0)))
				{
					m_vecEffectRegularTags[m_iEffectRegularCursor] = szModEffectRegularTag;

					bOpenEffectRegularModal = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
				{
					bOpenEffectRegularModal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

#pragma endregion
#pragma region Create

			if (ImGui::TreeNode("Create##EffectRegular##CTool_Effect::Tool_Properties"))
			{
				static EFFECT					eEffect(EFFECT::VFX_Visual);

				static VFX_Visual				tVFXVisualDesc{};
				static VFX_Distortion			tVFXDistortionDesc{};
				static VFX_VisualMesh			tVFXVisualMeshDesc{};
				static VFX_VisualCurve			tVFXVisualCurveDesc{};
				static VFX_MeshAnim				tVFXMeshAnimDesc{};
				static VFX_VisualDetailCurve	tVFXDetailCurveDesc{};

				static const _char* szEffectType = "Visual";
				if (ImGui::BeginCombo("Type##EffectRegular##CTool_Effect::Tool_Properties", szEffectType))
				{
					const _char* items[] = { "Visual", "Distortion", "VisualMesh", "VisualCurve", "MeshAnim", "DetailCurve" };

					if (ImGui::Selectable(items[0], szEffectType == items[0]))
					{
						szEffectType = items[0];
						eEffect = EFFECT::VFX_Visual;
					}
					if (ImGui::Selectable(items[1], szEffectType == items[1]))
					{
						szEffectType = items[1];
						eEffect = EFFECT::VFX_Distortion;
					}
					if (ImGui::Selectable(items[2], szEffectType == items[2]))
					{
						szEffectType = items[2];
						eEffect = EFFECT::VFX_VisualMesh;
					}
					if (ImGui::Selectable(items[3], szEffectType == items[3]))
					{
						szEffectType = items[3];
						eEffect = EFFECT::VFX_VisualCurve;
					}
					if (ImGui::Selectable(items[4], szEffectType == items[4]))
					{
						szEffectType = items[4];
						eEffect = EFFECT::VFX_MeshAnim;
					}
					if (ImGui::Selectable(items[5], szEffectType == items[5]))
					{
						szEffectType = items[5];
						eEffect = EFFECT::VFX_VisualDetailCurve;
					}

					ImGui::EndCombo();
				}
				if (ImGui::Button("Create##EffectRegular##CTool_Effect::Tool_Properties"))
				{
					switch (eEffect)
					{
					case EFFECT::VFX_Visual:
					{
						VFX_Visual tDesc = tVFXVisualDesc;
						tDesc.pDiffuse = m_pSpriteSheet;
						tDesc.vSpriteSheetSize = vSpriteSheetSize;
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_Distortion:
					{
						VFX_Distortion tDesc = tVFXDistortionDesc;
						tDesc.pTexture = m_pSpriteSheet;
						tDesc.vSpriteSheetSize = vSpriteSheetSize;
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_VisualMesh:
					{
						VFX_VisualMesh tDesc = tVFXVisualMeshDesc;
						tDesc.pMesh = m_pLoadMesh;
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_VisualCurve:
					{
						VFX_VisualCurve tDesc = tVFXVisualCurveDesc;
						tDesc.pDiffuse = m_pSpriteSheet;
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_MeshAnim:
					{
						VFX_MeshAnim tDesc = tVFXMeshAnimDesc;
						tDesc.vecMesh.emplace_back(m_pLoadMesh);
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_VisualDetailCurve:
					{
						VFX_VisualDetailCurve tDesc = tVFXDetailCurveDesc;
						tDesc.pDiffuse = m_pSpriteSheet;
						Push_EffectRegular(eEffect, tDesc);
					}
					break;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Import##EffectRegular##CTool_Effect::Tool_Properties"))
				{
					const _char* szFilters = "VFX (*.vfx){.vfx},All files{.*}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_LOADEFFECT, "Load Effect", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}

				switch (eEffect)
				{
				case EFFECT::VFX_Visual:
				{
					if (ImGui::InputFloat("LifeTime##VFX:Visual##CTool_Effect::Tool_Properties", &tVFXVisualDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:Visual##CTool_Effect::Tool_Properties", &tVFXVisualDesc.iShaderPass))
					{
						tVFXVisualDesc.iShaderPass = max(0, tVFXVisualDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_Distortion:
				{
					if (ImGui::InputFloat("LifeTime##VFX:Distortion##CTool_Effect::Tool_Properties", &tVFXDistortionDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:Distortion##CTool_Effect::Tool_Properties", &tVFXDistortionDesc.iShaderPass))
					{
						tVFXDistortionDesc.iShaderPass = max(0, tVFXDistortionDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_VisualMesh:
				{
					if (ImGui::InputFloat("LifeTime##VFX:Distortion##CTool_Effect::Tool_Properties", &tVFXVisualMeshDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:Distortion##CTool_Effect::Tool_Properties", &tVFXVisualMeshDesc.iShaderPass))
					{
						tVFXVisualMeshDesc.iShaderPass = max(0, tVFXVisualMeshDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_VisualCurve:
				{
					if (ImGui::InputFloat("LifeTime##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXVisualCurveDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXVisualCurveDesc.iShaderPass))
					{
						tVFXVisualCurveDesc.iShaderPass = max(0, tVFXVisualCurveDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_MeshAnim:
				{
					if (ImGui::InputFloat("LifeTime##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXMeshAnimDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXMeshAnimDesc.iShaderPass))
					{
						tVFXMeshAnimDesc.iShaderPass = max(0, tVFXMeshAnimDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_VisualDetailCurve:
				{
					if (ImGui::InputFloat("LifeTime##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXDetailCurveDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:VisualCurve##CTool_Effect::Tool_Properties", &tVFXDetailCurveDesc.iShaderPass))
					{
						tVFXDetailCurveDesc.iShaderPass = max(0, tVFXDetailCurveDesc.iShaderPass);
					}
				}
				break;
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

#pragma endregion
#pragma endregion
#pragma region Effect Instance

		if (ImGui::TreeNode("Effect Instance##CTool_Effect::Tool_Properties"))
		{
#pragma region List
			
			static _bool	bOpenEffectInstanceModal(false);

			if (ImGui::TreeNode("List##EffectInstance##CTool_Effect::Tool_Properties"))
			{
				if (ImGui::BeginListBox("Effect Instance List##CTool_Effect::Tool_Properties", ImVec2(-FLT_MIN, 0.f)))
				{
					for (size_t i = 0; i < m_vecEffectInstances.size(); ++i)
					{
						string strEffectInstanceTag;
						if (m_vecEffectInstanceTags[i].empty())
						{
							strEffectInstanceTag = string("EffectInstance" + std::to_string(i));
						}
						else
						{
							strEffectInstanceTag = m_vecEffectInstanceTags[i];
						}

						if (ImGui::Selectable(strEffectInstanceTag.c_str(), static_cast<_int>(i) == m_iEffectInstanceCursor, ImGuiSelectableFlags_AllowDoubleClick))
						{
							if (auto pVFX = dynamic_pointer_cast<CEffectInstance_MeshAfterImage>(m_vecEffectInstances[i]))
							{
								if (m_pTargetModel)
								{
									if (SUCCEEDED(pVFX->Fetch(make_pair(m_pDummyTransform, m_pTargetModel))))
									{
										m_iEffectInstanceCursor	= static_cast<_int>(i);
										m_pEffectInstance		= m_vecEffectInstances[i];
										m_pEffect				= m_pEffectInstance;
										m_bEffectTick			= false;
									}
								}
							}
							else
							{
								m_iEffectInstanceCursor	= static_cast<_int>(i);
								m_pEffectInstance		= m_vecEffectInstances[i];
								m_pEffect				= m_pEffectInstance;
								m_bEffectTick			= false;
							}

							if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
							{
								bOpenEffectInstanceModal = true;
							}
						}

						if (ImGui::BeginPopupContextItem())
						{
							m_iEffectInstanceCursor	= static_cast<_int>(i);
							m_pEffectInstance		= m_vecEffectInstances[i];
							m_pEffect				= m_pEffectInstance;
							m_bEffectTick			= false;

							static MATERIALDESC	tMtrlDesc{};

							tMtrlDesc = m_vecEffectInstances[i]->Get_Material();

							ImGui::Text(strEffectInstanceTag.c_str());
							switch (m_vecEffectInstances[i]->Get_Type())
							{
							case EFFECT::VFX_ParticlePoint:
							{
								shared_ptr<CEffectInstance_ParticlePoint> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePoint>(m_vecEffectInstances[i]);
								VFX_ParticlePoint tDesc = pInstance->Get_Data();

								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticlePoint");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;
	
											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);
	
											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;
	
											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}
	
											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticlePoint"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;
	
											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);
	
											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;
	
											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}
	
											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
	
										if (ImGui::Button("Change##Emission##VFX_ParticlePoint"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticlePoint"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}
								ImGui::Separator();
								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_ParticlePoint", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Instance LifeTime##VFX_ParticlePoint", &tDesc.fInstanceLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Start Gap##VFX_ParticlePoint", &tDesc.fStartGap))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Speed##VFX_ParticlePoint", &tDesc.fSpeed))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Velocity Scale##VFX_ParticlePoint", &tDesc.fVelocityScale))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Theta##VFX_ParticlePoint", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePoint", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Horizontal Scope##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vHorizontalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Vertical Scope##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vVerticalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Rotation Scope##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vRotationScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Gravity##VFX_ParticlePoint", &tDesc.bGravity))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Material Fade##VFX_ParticlePoint", &tDesc.bMaterialFade))
								{
									pInstance->Set_Data(tDesc);
								}
								if (tDesc.bMaterialFade)
								{
									ImGui::SameLine();
									if (ImGui::Checkbox("Apply Aplha##VFX_ParticlePoint", &tDesc.bApplyAlpha))
									{
										pInstance->Set_Data(tDesc);
									}
									if (ImGui::InputFloat2("Scope##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vMaterialFadeScope)))
									{
										pInstance->Set_Data(tDesc);
									}
									if (ImGui::InputFloat("Weight##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.fMaterialFadeWeight)))
									{
										pInstance->Set_Data(tDesc);
									}
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Size##VFX_ParticlePoint", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Max Instance##VFX_ParticlePoint", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Bloom##VFX_ParticlePoint", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePoint", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePoint", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_Visual", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_TrailLineDiffuse:
							{
								shared_ptr<CEffectInstance_TrailLineDiffuse> pInstance = dynamic_pointer_cast<CEffectInstance_TrailLineDiffuse>(m_vecEffectInstances[i]);
								VFX_TrailLineDiffuse tDesc = pInstance->Get_Data();

								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_TrailLineDiffuse");

								ImGui::Separator();

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_TrailLineDiffuse", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Trail Interval##VFX_TrailLineDiffuse", &tDesc.fInterval))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Thickness##VFX_TrailLineDiffuse", &tDesc.fThickness))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_TrailLineDiffuse", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Bloom##VFX_TrailLineDiffuse", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_TrailLineDiffuse", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_TrailLineDiffuse", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_TrailQuad:
							{
								shared_ptr<CEffectInstance_TrailQuadDiffuse> pInstance = dynamic_pointer_cast<CEffectInstance_TrailQuadDiffuse>(m_vecEffectInstances[i]);
								VFX_TrailQuad tDesc = pInstance->Get_Data();

								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_TrailQuad");

								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_TrailQuad"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emissive"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##VFX_TrailQuad"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##VFX_TrailQuad"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}
								ImGui::Separator();
								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_TrailQuad", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Trail Interval##VFX_TrailQuad", &tDesc.fInterval))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Distortion Power", &tDesc.fDistortionPower))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Instance Texture##VFX_TrailQuad", &tDesc.bInstanceTexture))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Bloom##VFX_TrailQuad", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_TrailQuad", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Max Instance##VFX_TrailQuad", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_TrailQuad", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_MeshAfterImage:
							{
								shared_ptr<CEffectInstance_MeshAfterImage> pInstance = dynamic_pointer_cast<CEffectInstance_MeshAfterImage>(m_vecEffectInstances[i]);
								VFX_MeshAfterImage tDesc = pInstance->Get_Data();

								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);
								ImGui::SameLine();
								ImGui::TextDisabled("VFX_MeshAfterImage");
								ImGui::Separator();

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_MeshAfterImage", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Interval##VFX_MeshAfterImage", &tDesc.fInterval))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("After Image Interval##VFX_MeshAfterImage", &tDesc.fAfterImageInterval))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Fade Out##VFX_MeshAfterImage", &tDesc.fFadeOut))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_MeshAfterImage", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Bloom##VFX_MeshAfterImage", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_MeshAfterImage", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_MeshAfterImage", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticlePointTarget:
							{
								shared_ptr<CEffectInstance_ParticlePointTarget> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointTarget>(m_vecEffectInstances[i]);
								VFX_ParticlePointTarget tDesc = pInstance->Get_Data();
								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticlePointTarget");
								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticlePointTarget"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticlePointTarget"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticlePointTarget"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("Instance LifeTime##VFX_ParticlePointTarget", &tDesc.fInstanceLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Start Gap##VFX_ParticlePointTarget", &tDesc.fStartGap))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Speed##VFX_ParticlePointTarget", &tDesc.fSpeed))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Velocity Scale##VFX_ParticlePointTarget", &tDesc.fVelocityScale))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Theta##VFX_ParticlePointTarget", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePointTarget", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Horizontal Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vHorizontalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Vertical Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vVerticalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Rotation Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vRotationScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Gravity##VFX_ParticlePointTarget", &tDesc.bGravity))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Material Fade##VFX_ParticlePointTarget", &tDesc.bMaterialFade))
								{
									pInstance->Set_Data(tDesc);
								}
								if (tDesc.bMaterialFade)
								{
									ImGui::SameLine();
									if (ImGui::Checkbox("Apply Alpha##VFX_ParticlePointTarget", &tDesc.bApplyAlpha))
									{
										pInstance->Set_Data(tDesc);
									}
									if (ImGui::InputFloat2("Material Fade Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vMaterialFadeScope)))
									{
										pInstance->Set_Data(tDesc);
									}
									if (ImGui::InputFloat("Material Fade Weight##VFX_ParticlePointTarget", &tDesc.fMaterialFadeWeight))
									{
										pInstance->Set_Data(tDesc);
									}
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticlePointTarget", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Size##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_Visual", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Bloom##VFX_ParticlePointTarget", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointTarget", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointTarget", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticleMesh:
							{
								shared_ptr<CEffectInstance_ParticleMesh> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleMesh>(m_vecEffectInstances[i]);
								VFX_ParticleMesh tDesc = pInstance->Get_Data();
								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticleMesh");
								ImGui::Separator();

								if (ImGui::TreeNode("Mesh##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::Button("Change##VFX_ParticleMesh"))
									{
										tDesc.pMesh = m_pLoadMesh;
										pInstance->Set_Data(tDesc);
									}
									ImGui::SameLine();
									if (tDesc.pMesh)
									{
										ImGui::Text(tDesc.pMesh->Get_Name());
									}
									else
									{
										ImGui::Text("Mesh Instance Unloaded");
									}

									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticleMesh"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticleMesh"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticleMesh"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_ParticlePointTarget", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Theta##VFX_ParticlePointTarget", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePointTarget", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Blend##VFX_ParticlePointTarget", &tDesc.bBlend))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticlePointTarget", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Bloom##VFX_ParticlePointTarget", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointTarget", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointTarget", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticleRect:
							{
								shared_ptr<CEffectInstance_ParticleRect> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleRect>(m_vecEffectInstances[i]);
								VFX_ParticleRect tDesc = pInstance->Get_Data();
								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticleRect");
								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticleRect"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticleRect"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticleRect"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::Checkbox("Loop##VFX_ParticleRect", &tDesc.bLoop))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Fade##VFX_ParticleRect", &tDesc.bFade))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("PhysX##VFX_ParticleRect", &tDesc.bPhysX))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Gravity##VFX_ParticleRect", &tDesc.bGravity))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticleRect", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("LifeTime##VFX_ParticlePointTarget", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Instance LifeTime##VFX_ParticlePointTarget", &tDesc.fInstanceLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Speed##VFX_ParticlePointTarget", &tDesc.fSpeed))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Speed Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vSpeedScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Scale##VFX_ParticlePointTarget", &tDesc.fScale))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Scale Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vScaleScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Horizontal Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vScaleScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Vertical Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vHorizontalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Rotation Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vRotationScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Theta##VFX_ParticlePointTarget", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePointTarget", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat2("Horizontal Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vHorizontalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Vertical Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vVerticalScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Rotation Scope##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vRotationScope)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								
								if (ImGui::InputFloat("Bloom##VFX_ParticlePointTarget", &tDesc.fBloom))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointTarget", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointTarget", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticlePointCurve:
							{
								shared_ptr<CEffectInstance_ParticlePointCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointCurve>(m_vecEffectInstances[i]);
								VFX_ParticlePointCurve tDesc = pInstance->Get_Data();
								//	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("VFX_Visual").x);

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticlePointCurve");
								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticlePointCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticlePointCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticlePointCurve"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("Theta##VFX_ParticlePointTarget", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePointTarget", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePointTarget", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Instance Life Time Scale##VFX_ParticlePointTarget", &tDesc.fInstLifeTimeScale))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Instance Start Time Scale##VFX_ParticlePointTarget", &tDesc.fInstStartTimeScale))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Velocity Tail##VFX_ParticlePointTarget", &tDesc.fVelocityTail))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticleRect", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_ParticleRect", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Size##VFX_ParticleRect", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointTarget", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointTarget", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticlePointDynamicCurve:
							{
								shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointDynamicCurve>(m_vecEffectInstances[i]);
								VFX_ParticlePointDynamicCurve tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticlePointDynamicCurve");
								ImGui::Separator();

								if (ImGui::TreeNode("Mesh##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::Button("Change##VFX_ParticlePointDynamicCurve"))
									{
										tDesc.pMesh = m_pLoadMesh;
										pInstance->Set_Data(tDesc);
									}
									ImGui::SameLine();
									if (tDesc.pMesh)
									{
										ImGui::Text(tDesc.pMesh->Get_Name());
									}
									else
									{
										ImGui::Text("Mesh Unloaded");
									}

									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticlePointDynamicCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticlePointDynamicCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticlePointDynamicCurve"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("Theta##VFX_ParticlePointDynamicCurve", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticlePointDynamicCurve", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Instance Life Time Scale##VFX_ParticlePointDynamicCurve", &tDesc.fInstLifeTimeScale))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Instance Start Time Scale##VFX_ParticlePointDynamicCurve", &tDesc.fInstStartTimeScale))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Velocity Tail##VFX_ParticlePointDynamicCurve", &tDesc.fVelocityTail))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Local##VFX_ParticlePointDynamicCurve", &tDesc.bLocal))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::SameLine();
								if (ImGui::Checkbox("Reverse##VFX_ParticlePointDynamicCurve", &tDesc.bReverse))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Local Space Max##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vLocalSpaceMax)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Local Space Min##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vLocalSpaceMin)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Spawn Range##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vSpawnRange)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticlePointDynamicCurve", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_ParticlePointDynamicCurve", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Size##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointDynamicCurve", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointDynamicCurve", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticlePointVectorFieldCurve:
							{
								shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointVectorFieldCurve>(m_vecEffectInstances[i]);
								VFX_ParticlePointVectorFieldCurve tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticlePointVectorFieldCurve");
								ImGui::Separator();

								if (ImGui::TreeNode("Mesh##EffectRegular##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::Button("Change##VFX_VisualMesh"))
									{
										tDesc.pMesh = m_pLoadMesh;
										pInstance->Set_Data(tDesc);
									}
									ImGui::SameLine();
									if (tDesc.pMesh)
									{
										ImGui::Text(tDesc.pMesh->Get_Name());
									}
									else
									{
										ImGui::Text("Mesh Unloaded");
									}

									ImGui::TreePop();
								}
								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticlePointDynamicCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticlePointDynamicCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticlePointDynamicCurve"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Noise##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pNoise)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pNoise->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pNoise->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Noise##VFX_ParticlePointDynamicCurve"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pNoise = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Noise##VFX_ParticlePointDynamicCurve"))
										{
											tDesc.pNoise = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								if (ImGui::Button("Reset##VFX_ParticlePointDynamicCurve"))
								{
									pInstance->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::LOOK, _float3(0.f, 0.f, 1.f));
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_ParticlePointDynamicCurve", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Velocity Scale##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vVelocityScale)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Spawn Scale##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vSpawnScale)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Rotation Scale##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vRotationScale)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Noise Scale##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vNoiseScale)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Noise Offset##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vNoiseOffset)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Noise Tiling##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vNoiseTiling)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Local Space##VFX_ParticlePointVectorFieldCurve", &tDesc.bLocal))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Spawn Method##VFX_ParticlePointDynamicCurve", &tDesc.iSpawnMethod))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Spawn Term##VFX_ParticlePointDynamicCurve", &tDesc.fSpawnTerm))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Spawn Offset##VFX_ParticlePointDynamicCurve", &tDesc.fSpawnOffset))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Terminate Method##VFX_ParticlePointDynamicCurve", &tDesc.iTerminateMethod))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Terminate Term##VFX_ParticlePointDynamicCurve", &tDesc.fTerminateTerm))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticlePointDynamicCurve", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Size##VFX_ParticlePointDynamicCurve", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Noise Pass##VFX_ParticlePointDynamicCurve", &tDesc.iNoisePass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticlePointDynamicCurve", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticlePointDynamicCurve", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							case EFFECT::VFX_ParticleLine:
							{
								shared_ptr<CEffectInstance_ParticleLine> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleLine>(m_vecEffectInstances[i]);
								VFX_ParticleLine tDesc = pInstance->Get_Data();

								ImGui::SameLine();
								ImGui::TextDisabled("VFX_ParticleLine");
								ImGui::Separator();

								if (ImGui::TreeNode("Image##EffectInstance##CTool_Effect::Tool_Properties"))
								{
									if (ImGui::TreeNode("Diffuse##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pDiffuse)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pDiffuse->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pDiffuse->Get_ShaderResourceView().Get(), imgSize);
										}
										if (ImGui::Button("Change##Diffuse##VFX_ParticleLine"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pDiffuse = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::TreePop();
									}
									if (ImGui::TreeNode("Emission##EffectInstance##CTool_Effect::Tool_Properties"))
									{
										if (tDesc.pEmission)
										{
											ImVec2 imgSize;

											D3D11_TEXTURE2D_DESC tTexture2dDesc{};
											tDesc.pEmission->Get_Texture2D()->GetDesc(&tTexture2dDesc);

											_float fWidth = static_cast<_float>(tTexture2dDesc.Width);
											_float fHeight = static_cast<_float>(tTexture2dDesc.Height);
											_float fAspect = fWidth / fHeight;

											if (fAspect > 1.f)
											{
												imgSize.x = 100.f;
												imgSize.y = 100.f / fAspect;
											}
											else
											{
												imgSize.x = 100.f * fAspect;
												imgSize.y = 100.f;
											}

											ImGui::Image(tDesc.pEmission->Get_ShaderResourceView().Get(), imgSize);
										}

										if (ImGui::Button("Change##Emission##VFX_ParticleLine"))
										{
											if (m_pSpriteSheet)
											{
												tDesc.pEmission = m_pSpriteSheet;
												pInstance->Set_Data(tDesc);
											}
										}
										ImGui::SameLine();
										if (ImGui::Button("Erase##Emission##VFX_ParticleLine"))
										{
											tDesc.pEmission = nullptr;
											pInstance->Set_Data(tDesc);
										}
										ImGui::TreePop();
									}
									ImGui::TreePop();
								}

								ImGui::Text("Data");
								if (ImGui::InputFloat("LifeTime##VFX_ParticleLine", &tDesc.fLifeTime))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Thickness##VFX_ParticleLine", &tDesc.fThickness))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Spawn Range##VFX_ParticleLine", reinterpret_cast<_float*>(&tDesc.vSpawnRange)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("ReversePower##VFX_ParticleLine", &tDesc.fReversePower))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Reverse##VFX_ParticleLine", &tDesc.bReverse))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::Checkbox("Preserve Direction##VFX_ParticleLine", &tDesc.bPreserveDirection))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputFloat("Theta##VFX_ParticleLine", &tDesc.fTheta))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat("Phi##VFX_ParticleLine", &tDesc.fPhi))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Force Reference##VFX_ParticleLine", reinterpret_cast<_float*>(&tDesc.vForceReference)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat3("Normal##VFX_ParticleLine", reinterpret_cast<_float*>(&tDesc.vNormal)))
								{
									pInstance->Set_Data(tDesc);
								}
								ImGui::Separator();
								if (ImGui::InputInt("Max Instance##VFX_ParticleLine", &tDesc.iMaxInstance))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputFloat2("Size##VFX_ParticleLine", reinterpret_cast<_float*>(&tDesc.vSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt2("SpriteSheet Size##VFX_ParticleLine", reinterpret_cast<_int*>(&tDesc.vSpriteSheetSize)))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Bloom Pass##VFX_ParticleLine", &tDesc.iBloomPass))
								{
									pInstance->Set_Data(tDesc);
								}
								if (ImGui::InputInt("Shader Pass##VFX_ParticleLine", &tDesc.iShaderPass))
								{
									pInstance->Set_Data(tDesc);
								}
							}
							break;
							}
							ImGui::Separator();

							ImGui::Text("Material");
							if (ImGui::InputFloat4("Diffuse##EffectInstance##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vDiffuse)))
							{
								m_vecEffectInstances[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Ambient##EffectInstance##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vAmbient)))
							{
								m_vecEffectInstances[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Specular##EffectInstance##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vSpecular)))
							{
								m_vecEffectInstances[i]->Set_Material(tMtrlDesc);
							}
							if (ImGui::InputFloat4("Emissive##EffectInstance##CTool_Effect::Tool_Properties", reinterpret_cast<_float*>(&tMtrlDesc.vEmissive)))
							{
								m_vecEffectInstances[i]->Set_Material(tMtrlDesc);
							}
							
							ImGui::Separator();

							if (ImGui::Button("Export##VFX_ParticlePoint##CTool_Effect::Tool_Properties"))
							{
								const _char* szFilters = "VFX (*.vfx){.vfx},All files{.*}";
								ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORTEFFECT, "Export Effect", szFilters, "../Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
							}
							ImGui::SameLine();
							if (ImGui::Button("Close"))
							{
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}
					}

					ImGui::EndListBox();
				}

				ImGui::TreePop();
			}

#pragma endregion
#pragma region Modal

			if (bOpenEffectInstanceModal)
			{
				ImGui::OpenPopup("Mod EffectInstance##CTool_Effect::Tool_Properties");
			}

			if (ImGui::BeginPopupModal("Mod EffectInstance##CTool_Effect::Tool_Properties"))
			{
				static _char szModEffectInstanceTag[MAX_PATH] = "Tag";

				ImGui::Text("Change Tag:");
				ImGui::InputText("Tag", szModEffectInstanceTag, IM_ARRAYSIZE(szModEffectInstanceTag));
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120.f, 0)))
				{
					m_vecEffectInstanceTags[m_iEffectInstanceCursor] = szModEffectInstanceTag;

					bOpenEffectInstanceModal = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120.f, 0)))
				{
					bOpenEffectInstanceModal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

#pragma endregion
#pragma region Create

			if (ImGui::TreeNode("Create##EffectInstance##CTool_Effect::Tool_Properties"))
			{
				static EFFECT		eEffect(EFFECT::VFX_ParticlePoint);

				static VFX_ParticlePoint					tVFXParticleDesc{};
				static VFX_ParticlePointTarget				tVFXParticleTargetDesc{};
				static VFX_TrailLineDiffuse					tVFXTrailLineDiffuse{};
				static VFX_TrailQuad						tVFXTrailQuad{};
				static VFX_MeshAfterImage					tVFXMeshAfterImage{};
				static VFX_ParticleMesh						tVFXParticleMesh{};
				static VFX_ParticleRect						tVFXParticleRect{};
				static VFX_ParticlePointCurve				tVFXParticlePointCurve{};
				static VFX_ParticlePointDynamicCurve		tVFXParticlePointDynamicCurve{};
				static VFX_ParticlePointVectorFieldCurve	tVFXParticlePointVectorFieldCurve{};
				static VFX_ParticleLine						tVFXParticleLine{};

				static const _char* szEffectType = "Particle";
				if (ImGui::BeginCombo("Type##EffectInstance##CTool_Effect::Tool_Properties", szEffectType))
				{
					const _char* items[] = { "Particle", "ParticleTarget", "LineTrailDiffuse", "QuadTrail", "MeshAfterImage", "ParticleMesh", "ParticleRect", "ParticlePointCurve", "ParticlePointDynamicCurve", "ParticlePointVectorFieldCurve", "ParticleLine"};

					if (ImGui::Selectable(items[0], szEffectType == items[0]))
					{
						szEffectType = items[0];
						eEffect = EFFECT::VFX_ParticlePoint;
					}
					if (ImGui::Selectable(items[1], szEffectType == items[1]))
					{
						szEffectType = items[1];
						eEffect = EFFECT::VFX_ParticlePointTarget;
					}
					if (ImGui::Selectable(items[2], szEffectType == items[2]))
					{
						szEffectType = items[2];
						eEffect = EFFECT::VFX_TrailLineDiffuse;
					}
					if (ImGui::Selectable(items[3], szEffectType == items[3]))
					{
						szEffectType = items[3];
						eEffect = EFFECT::VFX_TrailQuad;
					}
					if (ImGui::Selectable(items[4], szEffectType == items[4]))
					{
						szEffectType = items[4];
						eEffect = EFFECT::VFX_MeshAfterImage;
					}
					if (ImGui::Selectable(items[5], szEffectType == items[5]))
					{
						szEffectType = items[5];
						eEffect = EFFECT::VFX_ParticleMesh;
					}
					if (ImGui::Selectable(items[6], szEffectType == items[6]))
					{
						szEffectType = items[6];
						eEffect = EFFECT::VFX_ParticleRect;
					}
					if (ImGui::Selectable(items[7], szEffectType == items[7]))
					{
						szEffectType = items[7];
						eEffect = EFFECT::VFX_ParticlePointCurve;
					}
					if (ImGui::Selectable(items[8], szEffectType == items[8]))
					{
						szEffectType = items[8];
						eEffect = EFFECT::VFX_ParticlePointDynamicCurve;
					}
					if (ImGui::Selectable(items[9], szEffectType == items[9]))
					{
						szEffectType = items[9];
						eEffect = EFFECT::VFX_ParticlePointVectorFieldCurve;
					}
					if (ImGui::Selectable(items[10], szEffectType == items[10]))
					{
						szEffectType = items[10];
						eEffect = EFFECT::VFX_ParticleLine;
					}

					ImGui::EndCombo();
				}
				if (ImGui::Button("Create##EffectInstance##CTool_Effect::Tool_Properties"))
				{
					switch (eEffect)
					{
					case EFFECT::VFX_ParticlePoint:
					{
						VFX_ParticlePoint tDesc			= tVFXParticleDesc;
						tDesc.pDiffuse					= m_pSpriteSheet;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticlePointTarget:
					{
						VFX_ParticlePointTarget tDesc	= tVFXParticleTargetDesc;
						tDesc.pDiffuse					= m_pSpriteSheet;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_TrailLineDiffuse:
					{
						VFX_TrailLineDiffuse tDesc		= tVFXTrailLineDiffuse;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_TrailQuad:
					{
						VFX_TrailQuad tDesc				= tVFXTrailQuad;
						tDesc.pDiffuse					= m_pSpriteSheet;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_MeshAfterImage:
					{
						VFX_MeshAfterImage tDesc		= tVFXMeshAfterImage;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticleMesh:
					{
						VFX_ParticleMesh tDesc			= tVFXParticleMesh;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticleRect:
					{
						VFX_ParticleRect tDesc			= tVFXParticleRect;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticlePointCurve:
					{
						VFX_ParticlePointCurve tDesc	= tVFXParticlePointCurve;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticlePointDynamicCurve:
					{
						VFX_ParticlePointDynamicCurve tDesc	= tVFXParticlePointDynamicCurve;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticlePointVectorFieldCurve:
					{
						VFX_ParticlePointVectorFieldCurve tDesc	= tVFXParticlePointVectorFieldCurve;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					case EFFECT::VFX_ParticleLine:
					{
						VFX_ParticleLine tDesc	= tVFXParticleLine;
						Push_EffectInstance(eEffect, tDesc);
					}
					break;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Import##EffectInstance##CTool_Effect::Tool_Properties"))
				{
					const _char* szFilters = "VFX (*.vfx){.vfx},All files{.*}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_LOADEFFECT, "Load Effect", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}

				switch (eEffect)
				{
				case EFFECT::VFX_ParticlePoint:
				{
					if (ImGui::InputFloat("LifeTime##VFX:Particle##CTool_Effect::Tool_Properties", &tVFXParticleDesc.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:Particle##CTool_Effect::Tool_Properties", &tVFXParticleDesc.iShaderPass))
					{
						tVFXParticleDesc.iShaderPass = max(0, tVFXParticleDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_TrailLineDiffuse:
				{
					if (ImGui::InputFloat("LifeTime##VFX:TrailLineDiffuse##CTool_Effect::Tool_Properties", &tVFXTrailLineDiffuse.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:TrailLineDiffuse##CTool_Effect::Tool_Properties", &tVFXTrailLineDiffuse.iShaderPass))
					{
						tVFXTrailLineDiffuse.iShaderPass = max(0, tVFXTrailLineDiffuse.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_TrailQuad:
				{
					if (ImGui::InputFloat("LifeTime##VFX:TrailQuadDiffuse##CTool_Effect::Tool_Properties", &tVFXTrailQuad.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:TrailQuadDiffuse##CTool_Effect::Tool_Properties", &tVFXTrailQuad.iShaderPass))
					{
						tVFXTrailQuad.iShaderPass = max(0, tVFXTrailQuad.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_MeshAfterImage:
				{
					if (ImGui::InputFloat("LifeTime##VFX:MeshAfterImage##CTool_Effect::Tool_Properties", &tVFXMeshAfterImage.fLifeTime))
					{
					}
					if (ImGui::InputInt("ShaderPass##VFX:MeshAfterImage##CTool_Effect::Tool_Properties", &tVFXMeshAfterImage.iShaderPass))
					{
						tVFXMeshAfterImage.iShaderPass = max(0, tVFXMeshAfterImage.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticlePointTarget:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticlePointTarget##CTool_Effect::Tool_Properties", &tVFXParticleTargetDesc.iShaderPass))
					{
						tVFXParticleTargetDesc.iShaderPass = max(0, tVFXParticleTargetDesc.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticleMesh:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticleMesh##CTool_Effect::Tool_Properties", &tVFXParticleMesh.iShaderPass))
					{
						tVFXParticleMesh.iShaderPass = max(0, tVFXParticleMesh.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticleRect:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticleMesh##CTool_Effect::Tool_Properties", &tVFXParticleRect.iShaderPass))
					{
						tVFXParticleRect.iShaderPass = max(0, tVFXParticleRect.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticlePointCurve:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticlePointCurve##CTool_Effect::Tool_Properties", &tVFXParticlePointCurve.iShaderPass))
					{
						tVFXParticlePointCurve.iShaderPass = max(0, tVFXParticlePointCurve.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticlePointDynamicCurve:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticlePointDynamicCurve##CTool_Effect::Tool_Properties", &tVFXParticlePointDynamicCurve.iShaderPass))
					{
						tVFXParticlePointDynamicCurve.iShaderPass = max(0, tVFXParticlePointDynamicCurve.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticlePointVectorFieldCurve:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticlePointDynamicCurve##CTool_Effect::Tool_Properties", &tVFXParticlePointVectorFieldCurve.iShaderPass))
					{
						tVFXParticlePointVectorFieldCurve.iShaderPass = max(0, tVFXParticlePointVectorFieldCurve.iShaderPass);
					}
				}
				break;
				case EFFECT::VFX_ParticleLine:
				{
					if (ImGui::InputInt("ShaderPass##VFX:VFX_ParticleLine##CTool_Effect::Tool_Properties", &tVFXParticleLine.iShaderPass))
					{
						tVFXParticleLine.iShaderPass = max(0, tVFXParticleLine.iShaderPass);
					}
				}
				break;
				}

				ImGui::TreePop();
			}

#pragma endregion
			ImGui::TreePop();
		}
#pragma endregion
#pragma endregion
	}
#pragma region File Dialog Instance

	if (ImGuiFileDialog::Instance()->Display(DIALOG_LOADEFFECT))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string strFile = ImGuiFileDialog::Instance()->GetFilePathName();
			size_t nPos = strFile.find("Bin");
			if (nPos != string::npos)
			{
				strFile = strFile.substr(nPos);
			}
			shared_ptr<CEffect> pEffect = Load_Effect(strFile);
			if (pEffect)
			{
				if (shared_ptr<CEffectRegular> pInstance = dynamic_pointer_cast<CEffectRegular>(pEffect))
				{
					Function::SplitPath(ImGuiFileDialog::Instance()->GetFilePathName(), nullptr, nullptr, &strFile, nullptr);
					Push_EffectRegular(pInstance, strFile);
				}
				else if (shared_ptr<CEffectInstance> pInstance = dynamic_pointer_cast<CEffectInstance>(pEffect))
				{
					Function::SplitPath(ImGuiFileDialog::Instance()->GetFilePathName(), nullptr, nullptr, &strFile, nullptr);
					Push_EffectInstance(pInstance, strFile);
				}
				else
				{
					MSG_BOX("CTool_Effect::Tool_Properties", "Failed to cast");
				}
			}
			else
			{
				MSG_BOX("CTool_Effect::Tool_Properties", "Failed to Load_Effect");
			}
		}
		else if (ImGuiFileDialog::Instance()->IsOpened(DIALOG_LOADEFFECT))
		{
			ImGuiFileDialog::Instance()->Close();
		}
	}

	if (ImGuiFileDialog::Instance()->Display(DIALOG_EXPORTEFFECT))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(Export_Effect(m_pEffect, ImGuiFileDialog::Instance()->GetFilePathName())))
			{
				MSG_BOX("CTool_Effect::Tool_Properties", "Failed to Load_Effect");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
}

void CTool_Effect::Tool_MainControl()
{
	if (ImGui::TreeNode("Effect Control##CTool_Effect::Tool_MainControl"))
	{
		if (ImGui::Button("Fetch##CTool_Effect::Tool_MainControl"))
		{
			if (m_pEffect)
			{
				m_pEffect->Fetch(m_mEffectRegularFetch);
			}
			else
			{
			//	MSG_BOX("CTool_Effect::Tool_MainControl", "Invalid Effect");
			}
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Repeat##CTool_Effect::Tool_MainControl", &m_bEffectRepeat))
		{
			if (m_pEffect)
			{
				m_pEffect->Set_ToolRepeat(m_bEffectRepeat);
			}
		}
		ImGui::SameLine(); 
		if (ImGui::Checkbox("Tick##CTool_Effect::Tool_MainControl", &m_bEffectTick))
		{
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Model##CTool_Effect::Tool_MainControl"))
	{
		if (!m_pTargetModel)
		{
			if (ImGui::Button("Load Model##CTool_Effect::Tool_MainControl"))
			{
				const _char* szFilters = "Model (*.mdl){.mdl},All files{.*}";
				ImGuiFileDialog::Instance()->OpenDialog(DIALOG_TRAILTARGET, "Trail Target Model", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
			}
		}
		else
		{
			if (ImGui::Button("Release Model##CTool_Effect::Tool_MainControl"))
			{
				m_pTargetModel = nullptr;
			}
		}

		if (m_pTargetModel)
		{
			ImGui::SameLine();
			ImGui::Text("Model Loaded!");
		}
		else
		{
			ImGui::SameLine();
			ImGui::Text("Load Model...");
		}

		static _bool		bReset			= false;
		static _bool		bMulti			= false;
		static _float4x4	mLocalPivot0	= g_mUnit;
		static _float4x4	mLocalPivot1	= g_mUnit;

		if (ImGui::Button("Fetch##CTool_Effect::Tool_MainControl"))
		{
			if (m_pEffectInstance)
			{
				switch (m_pEffectInstance->Get_Type())
				{
				case EFFECT::VFX_TrailLineDiffuse:
				{
				//	bMulti = false;
					if (m_pTargetModel->Get_BoneIndex(m_szBone0) != m_pTargetModel->Get_NumBones())
					{
						CEffect_Manager::Get_Instance()->Fetch(m_pEffectInstance, make_tuple(m_pDummyTransform, m_pTargetModel, m_szBone0), bReset);
					}
					else
					{
						MSG_BOX("CTool_Effect::Tool_MainControl", "Invalid Bone");
					}
				}
				break;
				case EFFECT::VFX_TrailQuad:
				{
				//	bMulti = true;
					if (bMulti)
					{
						if (m_pTargetModel->Get_BoneIndex(m_szBone0) != m_pTargetModel->Get_NumBones()
						||	m_pTargetModel->Get_BoneIndex(m_szBone1) != m_pTargetModel->Get_NumBones())
						{
							CEffect_Manager::Get_Instance()->Fetch(m_pEffectInstance, make_tuple(m_pDummyTransform, m_pTargetModel, m_szBone0, m_szBone1), bReset);
						}
						else
						{
							MSG_BOX("CTool_Effect::Tool_MainControl", "Invalid Bone");
						}
					}
					else
					{
						if (m_pTargetModel->Get_BoneIndex(m_szBone0) != m_pTargetModel->Get_NumBones())
						{
							CEffect_Manager::Get_Instance()->Fetch(m_pEffectInstance, make_tuple(m_pDummyTransform, m_pTargetModel, m_szBone0, mLocalPivot0, mLocalPivot1), bReset);
						}
						else
						{
							MSG_BOX("CTool_Effect::Tool_MainControl", "Invalid Bone");
						}
					}
				}
				break;
				case EFFECT::VFX_ParticlePointTarget:
				{
					CEffect_Manager::Get_Instance()->Fetch(m_pEffectInstance, make_pair(m_pDummyTransform, m_pTargetModel), bReset);
				}
				break;
				case EFFECT::VFX_MeshAfterImage:
				{
					 CEffect_Manager::Get_Instance()->Fetch(m_pEffectInstance, make_pair(m_pDummyTransform, m_pTargetModel), bReset);
				}
				break;
				}
			}
		}
		ImGui::SameLine();
		ImGui::Checkbox("Reset Pool##CTool_Effect::Tool_MainControl", &bReset);
		ImGui::SameLine();
		ImGui::Checkbox("Multi Bone##CTool_Effect::Tool_MainControl", &bMulti);

		if (!bMulti)
		{
			if (ImGui::TreeNode("LocalPivot0"))
			{
				ImGui::InputFloat4("Right##LocalPivot0", reinterpret_cast<float*>(&mLocalPivot0.m[0][0]));
				ImGui::InputFloat4("Up##LocalPivot0", reinterpret_cast<float*>(&mLocalPivot0.m[1][0]));
				ImGui::InputFloat4("Look##LocalPivot0", reinterpret_cast<float*>(&mLocalPivot0.m[2][0]));
				ImGui::InputFloat4("Translation##LocalPivot0", reinterpret_cast<float*>(&mLocalPivot0.m[3][0]));
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("LocalPivot1"))
			{
				ImGui::InputFloat4("Right##LocalPivot1", reinterpret_cast<float*>(&mLocalPivot1.m[0][0]));
				ImGui::InputFloat4("Up##LocalPivot1", reinterpret_cast<float*>(&mLocalPivot1.m[1][0]));
				ImGui::InputFloat4("Look##LocalPivot1", reinterpret_cast<float*>(&mLocalPivot1.m[2][0]));
				ImGui::InputFloat4("Translation##LocalPivot1", reinterpret_cast<float*>(&mLocalPivot1.m[3][0]));
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	if (m_pCurveContainer)
	{
		if (ImGui::TreeNode("CurveContainer##CTool_Effect::Tool_MainControl"))
		{
			static _float fTime(0.f);

			_uint	iIndex	= m_pCurveContainer->Find_Index(fTime);
			_float4	vInit	= m_pCurveContainer->Get_InitialValue();

		//	m_pCurrentCurvePlot = m_pCurveContainer->Get_CurvePlot(fTime);

			if (ImGui::InputFloat4("Initial Value", reinterpret_cast<_float*>(&vInit)))
			{
				m_pCurveContainer->Set_InitialValue(vInit);
			}
			if (ImGui::SliderFloat("Time##CTool_Effect::Tool_MainControl", &fTime, 0.f, m_pCurveContainer->Get_Total()))
			{
				fTime = std::clamp(fTime, 0.f, m_pCurveContainer->Get_Total());
				m_pCurvePlot = m_pCurveContainer->Get_CurvePlot(fTime);
			}
			ImGui::Text("Value: %f", m_pCurveContainer->Get_Value(fTime).x);

			ImGui::Separator();

			for (_uint i = 0; i < m_pCurveContainer->Get_CurveCount(); ++i)
			{
				_float	fDuration	= m_pCurveContainer->Get_Duration(i + 1);
				_float4	vValue		= m_pCurveContainer->Get_ContainerValue(i + 1);

				if (ImGui::InputFloat((string("Duration##") + std::to_string(i + 1)).c_str(), &fDuration))
				{
					m_pCurveContainer->Set_Duration(i + 1, fDuration);
				}
				if (ImGui::InputFloat4((string("Value##") + std::to_string(i + 1)).c_str(), reinterpret_cast<_float*>(&vValue)))
				{
					m_pCurveContainer->Set_Value(i + 1, vValue);
				}
				if (ImGui::Button(string("Open CurvePlot##" + std::to_string(i + 1)).c_str()))
				{
					m_pCurrentCurvePlot = m_pCurveContainer->Get_CurvePlot(i);
				}
				ImGui::SameLine();
				if (ImGui::Button(string("Load CurvePlot##" + std::to_string(i + 1)).c_str()))
				{
					if (m_pCurrentCurvePlot)
					{
						m_pCurveContainer->Set_CurvePlot(i, m_pCurrentCurvePlot->Clone());
					}
				}
				ImGui::Separator();
			}

			if (ImGui::TreeNode("Add_CurvePlot##CTool_Effect::Tool_MainControl"))
			{
				static _float	fDuration(1.f);
				static _float4	vValue(XMVectorReplicate(1.f));

				ImGui::InputFloat("Duration##CTool_Effect::Tool_MainControl##Add_CurvePlot", &fDuration);

				if (ImGui::Button("Add CurvePlot"))
				{
					m_pCurveContainer->Add_Curve(CCurvePlot::Create("New CurvePlot"), iIndex, fDuration, vValue);
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Delete_CurvePlot##CTool_Effect::Tool_MainControl"))
			{
				static _float	fDuration(1.f);
				static _float4	vValue(XMVectorReplicate(1.f));

				ImGui::InputFloat("Duration##CTool_Effect::Tool_MainControl##Delete_CurvePlot", &fDuration);

				if (ImGui::Button("Delete CurvePlot"))
				{
					m_pCurveContainer->Remove_Curve(iIndex);
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

#pragma region File Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_TRAILTARGET))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(Push_TrailTarget(ImGuiFileDialog::Instance()->GetFilePathName())))
			{
				MSG_BOX("CTool_Effect::Tool_MainControl", "Failed to Push_TrailTarget");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
}

void CTool_Effect::Tool_SubControl()
{
	if (ImGui::TreeNode("ClipBoard##CTool_Effect::Tool_SubControl"))
	{
		if (ImGui::Button("ResetModelTransform"))
		{
			m_pDummyTransform->Set_Matrix(g_mUnit);
		}
		static _float fSpd = 1.f;
		if (ImGui::DragFloat("AnimSpd", &fSpd, 0.1f, 0.f, 5.f))
		{
			m_pTargetModel->Set_Animation(m_iAnimationIndex, fSpd, false, g_fDefaultInterpolationDuration, true);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Regular Fetch##CTool_Effect::Tool_SubControl"))
	{
		static _bool bFetchTransform(false);
		if (ImGui::Checkbox("Fetch Transform##CTool_Effect::Tool_SubControl", &bFetchTransform))
		{
			m_pFetchTransform->Set_Matrix(g_mUnit);
		}
		if (bFetchTransform)
		{
			m_mEffectRegularFetch = m_pFetchTransform->Get_Matrix();
			if (CGameInstance::Get_Instance()->Key_Down('F'))
			{
				if (m_pEffect)
				{
					m_pEffect->Fetch(m_mEffectRegularFetch);
				}
			}
		}
		if (ImGui::InputFloat4("Right##CTool_Effect::Tool_SubControl", reinterpret_cast<_float*>(&m_mEffectRegularFetch.m[0][0])))
		{
		}
		if (ImGui::InputFloat4("Up##CTool_Effect::Tool_SubControl", reinterpret_cast<_float*>(&m_mEffectRegularFetch.m[1][0])))
		{
		}
		if (ImGui::InputFloat4("Look##CTool_Effect::Tool_SubControl", reinterpret_cast<_float*>(&m_mEffectRegularFetch.m[2][0])))
		{
		}
		if (ImGui::InputFloat4("Translation##CTool_Effect::Tool_SubControl", reinterpret_cast<_float*>(&m_mEffectRegularFetch.m[3][0])))
		{
		}
		ImGui::Separator();
		static _float4 vScale		= _float4(1.f, 1.f, 1.f, 0.f);
		static _float4 vQuaternion	= XMQuaternionIdentity();
		static _float4 vTranslation	= _float4();
		if (ImGui::InputFloat4("Scale", reinterpret_cast<_float*>(&vScale)))
		{
		}
		if (ImGui::InputFloat4("Quaternion", reinterpret_cast<_float*>(&vQuaternion)))
		{
		}
		if (ImGui::InputFloat4("Translation", reinterpret_cast<_float*>(&vTranslation)))
		{
		}
		if (ImGui::Button("Apply"))
		{
			shared_ptr<CTransform> pDummy = CTransform::Create();
			pDummy->Set_Matrix(g_mUnit);
			pDummy->Set_Scale(vScale);
			pDummy->Rotate(vQuaternion);
			pDummy->Translate(vTranslation);
			m_mEffectRegularFetch = pDummy->Get_Matrix();
		}
		ImGui::TreePop();
	}

	if (m_pEffect)
	{
		if (ImGui::TreeNode("CurveContainer##CTool_Effect::Tool_SubControl"))
		{
#pragma region ParticlePoint
			if (shared_ptr<CEffectInstance_ParticlePoint> pEffect = dynamic_pointer_cast<CEffectInstance_ParticlePoint>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXRegular:VisualMesh"))
				{
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Diffuse"))
					{
						m_pCurveContainer = pEffect->Get_DiffuseCurve();
					}
					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region VisualMesh
			else if (shared_ptr<CEffectRegular_VisualMesh> pEffect = dynamic_pointer_cast<CEffectRegular_VisualMesh>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXRegular:VisualMesh"))
				{
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Translation"))
					{
						m_pCurveContainer = pEffect->Get_TranslationCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Diffuse"))
					{
						m_pCurveContainer = pEffect->Get_DiffuseCurve();
					}
					if (ImGui::Selectable("Emissive"))
					{
						m_pCurveContainer = pEffect->Get_EmissiveCurve();
					}
					if (ImGui::Selectable("Distortion"))
					{
						m_pCurveContainer = pEffect->Get_DistortionCurve();
					}
					if (ImGui::Selectable("Rotation"))
					{
						m_pCurveContainer = pEffect->Get_RotationCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}
					ImGui::EndListBox();
				}
				if (ImGui::Button("Set_Bone"))
				{
					pEffect->Set_Bone(m_pDummyTransform, m_pTargetModel, m_pTargetModel->Get_Bone(m_szBone0));
				}
			}
#pragma endregion
#pragma region ParticlePointCurve
			else if (shared_ptr<CEffectInstance_ParticlePointCurve> pEffect = dynamic_pointer_cast<CEffectInstance_ParticlePointCurve>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXInstance:ParticlePointCurve"))
				{
					if (ImGui::Selectable("Instance LifeTime"))
					{
						m_pCurveContainer = pEffect->Get_InstLifeTimeCurve();
					}
					if (ImGui::Selectable("Start"))
					{
						m_pCurveContainer = pEffect->Get_StartCurve();
					}
					if (ImGui::Selectable("Speed"))
					{
						m_pCurveContainer = pEffect->Get_Speed();
					}
					if (ImGui::Selectable("Gravity Scale"))
					{
						m_pCurveContainer = pEffect->Get_GravityScaleCurve();
					}
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Diffuse"))
					{
						m_pCurveContainer = pEffect->Get_DiffuseCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region ParticlePointDynamicCurve
			else if (shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pEffect = dynamic_pointer_cast<CEffectInstance_ParticlePointDynamicCurve>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXInstance:ParticlePointDynamicCurve"))
				{
					if (ImGui::Selectable("Instance LifeTime"))
					{
						m_pCurveContainer = pEffect->Get_InstLifeTimeCurve();
					}
					if (ImGui::Selectable("Start"))
					{
						m_pCurveContainer = pEffect->Get_StartCurve();
					}
					if (ImGui::Selectable("Speed"))
					{
						m_pCurveContainer = pEffect->Get_Speed();
					}
					if (ImGui::Selectable("Gravity Scale"))
					{
						m_pCurveContainer = pEffect->Get_GravityScaleCurve();
					}
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}
					if (ImGui::Selectable("Rotation"))
					{
						m_pCurveContainer = pEffect->Get_RotationCurve();
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region VisualCurve
			else if (shared_ptr<CEffectRegular_VisualCurve> pEffect = dynamic_pointer_cast<CEffectRegular_VisualCurve>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXRegular:VisualCurve"))
				{
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}
					if (ImGui::Selectable("Distortion"))
					{
						m_pCurveContainer = pEffect->Get_DistortionCurve();
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region VisualDetailCurve
			else if (shared_ptr<CEffectRegular_VisualDetailCurve> pEffect = dynamic_pointer_cast<CEffectRegular_VisualDetailCurve>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXRegular:VisualDetailCurve"))
				{
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}
					if (ImGui::Selectable("Distortion"))
					{
						m_pCurveContainer = pEffect->Get_DistortionCurve();
					}
					if (ImGui::Selectable("Diffuse"))
					{
						m_pCurveContainer = pEffect->Get_DiffuseCurve();
					}
					if (ImGui::Selectable("Emissive"))
					{
						m_pCurveContainer = pEffect->Get_EmissiveCurve();
					}
					if (ImGui::Selectable("Translation"))
					{
						m_pCurveContainer = pEffect->Get_TranslationCurve();
					}
					if (ImGui::Selectable("Rotation"))
					{
						m_pCurveContainer = pEffect->Get_RotationCurve();
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region MeshAnim
			else if (shared_ptr<CEffectRegular_MeshAnim> pEffect = dynamic_pointer_cast<CEffectRegular_MeshAnim>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXRegular:MeshAnim"))
				{
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Translation"))
					{
						m_pCurveContainer = pEffect->Get_TranslationCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Diffuse"))
					{
						m_pCurveContainer = pEffect->Get_DiffuseCurve();
					}
					if (ImGui::Selectable("Emissive"))
					{
						m_pCurveContainer = pEffect->Get_EmissiveCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region ParticlePointVectorFieldCurve
			else if (shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pEffect = dynamic_pointer_cast<CEffectInstance_ParticlePointVectorFieldCurve>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXInstance:VectorField"))
				{
					if (ImGui::Selectable("LifeTime"))
					{
						m_pCurveContainer = pEffect->Get_InstLifeTimeCurve();
					}
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("VelocityX"))
					{
						m_pCurveContainer = pEffect->Get_VelocityXCurve();
					}
					if (ImGui::Selectable("VelocityY"))
					{
						m_pCurveContainer = pEffect->Get_VelocityYCurve();
					}
					if (ImGui::Selectable("VelocityZ"))
					{
						m_pCurveContainer = pEffect->Get_VelocityZCurve();
					}
					if (ImGui::Selectable("Rotation"))
					{
						m_pCurveContainer = pEffect->Get_RotationCurve();
					}
					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region ParticleMesh
			else if (shared_ptr<CEffectInstance_ParticleMesh> pEffect = dynamic_pointer_cast<CEffectInstance_ParticleMesh>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXInstance:ParticleMesh"))
				{
					if (ImGui::Selectable("LifeTime"))
					{
						m_pCurveContainer = pEffect->Get_InstLifeTimeCurve();
					}
					if (ImGui::Selectable("Start"))
					{
						m_pCurveContainer = pEffect->Get_StartCurve();
					}
					if (ImGui::Selectable("Speed"))
					{
						m_pCurveContainer = pEffect->Get_Speed();
					}
					if (ImGui::Selectable("Gravity"))
					{
						m_pCurveContainer = pEffect->Get_GravityScaleCurve();
					}
					if (ImGui::Selectable("Scale"))
					{
						m_pCurveContainer = pEffect->Get_ScaleCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Rotation"))
					{
						m_pCurveContainer = pEffect->Get_RotationCurve();
					}
					ImGui::EndListBox();
				}
			}
#pragma endregion
#pragma region ParticleLine
			else if (shared_ptr<CEffectInstance_ParticleLine> pEffect = dynamic_pointer_cast<CEffectInstance_ParticleLine>(m_pEffect))
			{
				if (ImGui::BeginListBox("VFXInstance:ParticleLine"))
				{
					if (ImGui::Selectable("LifeTime"))
					{
						m_pCurveContainer = pEffect->Get_InstLifeTimeCurve();
					}
					if (ImGui::Selectable("Start"))
					{
						m_pCurveContainer = pEffect->Get_StartCurve();
					}
					if (ImGui::Selectable("Length"))
					{
						m_pCurveContainer = pEffect->Get_LengthCurve();
					}
					if (ImGui::Selectable("Delay"))
					{
						m_pCurveContainer = pEffect->Get_DelayCurve();
					}
					if (ImGui::Selectable("Bloom"))
					{
						m_pCurveContainer = pEffect->Get_BloomCurve();
					}
					if (ImGui::Selectable("Fade"))
					{
						m_pCurveContainer = pEffect->Get_FadeCurve();
					}
					if (ImGui::Selectable("Sprite"))
					{
						m_pCurveContainer = pEffect->Get_SpriteCurve();
					}
					ImGui::EndListBox();
				}
			}
#pragma endregion
			ImGui::TreePop();
		}
	}
}

void CTool_Effect::Push_SpriteSheet()
{
	if (m_iLoadTextureCursor != -1)
	{
		m_vecSpriteSheets.emplace_back(m_pLoadTexture);
		m_vecSpriteSheetTags.resize(m_vecSpriteSheets.size());
	}
}

void CTool_Effect::Push_EffectRegular(EFFECT _eEffect, any _tDesc)
{
	if (!Function::InRange(_eEffect, static_cast<EFFECT>(0), EFFECT::MAX))
	{
		MSG_RETURN(, "CTool_Effect::Push_EffectRegular", "Invalid EFFECT");
	}

	switch (_eEffect)
	{
	case EFFECT::VFX_Visual:
	{
		shared_ptr<CEffectRegular_Visual> pInstance = dynamic_pointer_cast<CEffectRegular_Visual>(CEffectRegular_Visual::Create(m_pDevice, m_pContext)->Clone());
		VFX_Visual tDesc = any_cast<VFX_Visual>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid SpriteSheet");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_Distortion:
	{
		shared_ptr<CEffectRegular_Distortion> pInstance = dynamic_pointer_cast<CEffectRegular_Distortion>(CEffectRegular_Distortion::Create(m_pDevice, m_pContext)->Clone());
		VFX_Distortion tDesc = any_cast<VFX_Distortion>(_tDesc);
		if (nullptr == tDesc.pTexture)
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid SpriteSheet");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_VisualMesh:
	{
		shared_ptr<CEffectRegular_VisualMesh> pInstance = dynamic_pointer_cast<CEffectRegular_VisualMesh>(CEffectRegular_VisualMesh::Create(m_pDevice, m_pContext)->Clone());
		VFX_VisualMesh tDesc = any_cast<VFX_VisualMesh>(_tDesc);
		if (nullptr == tDesc.pMesh)
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid");
		}
		pInstance->Set_Data(tDesc);
		pInstance->Add_Component(COMPONENT::MESH, tDesc.pMesh);
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_VisualCurve:
	{
		shared_ptr<CEffectRegular_VisualCurve> pInstance = dynamic_pointer_cast<CEffectRegular_VisualCurve>(CEffectRegular_VisualCurve::Create(m_pDevice, m_pContext)->Clone());
		VFX_VisualCurve tDesc = any_cast<VFX_VisualCurve>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_MeshAnim:
	{
		shared_ptr<CEffectRegular_MeshAnim> pInstance = dynamic_pointer_cast<CEffectRegular_MeshAnim>(CEffectRegular_MeshAnim::Create(m_pDevice, m_pContext)->Clone());
		VFX_MeshAnim tDesc = any_cast<VFX_MeshAnim>(_tDesc);
		if (tDesc.vecMesh.empty())
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid");
		}
		pInstance->Set_Data(tDesc);
		pInstance->Add_Component(COMPONENT::MESH, tDesc.vecMesh.front());
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_VisualDetailCurve:
	{
		shared_ptr<CEffectRegular_VisualDetailCurve> pInstance = dynamic_pointer_cast<CEffectRegular_VisualDetailCurve>(CEffectRegular_VisualDetailCurve::Create(m_pDevice, m_pContext)->Clone());
		VFX_VisualDetailCurve tDesc = any_cast<VFX_VisualDetailCurve>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectRegular", "Invalid");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectRegulars.emplace_back(pInstance);
	}
	break;
	}

	m_vecEffectRegularTags.resize(m_vecEffectRegulars.size());
}

void CTool_Effect::Push_EffectRegular(shared_ptr<CEffectRegular> _pEffect, const string& _strTag)
{
	m_vecEffectRegulars.emplace_back(_pEffect);
	m_vecEffectRegularTags.resize(m_vecEffectRegulars.size());
	m_vecEffectRegularTags.back() = _strTag;
}

void CTool_Effect::Push_EffectInstance(EFFECT _eEffect, any _tDesc)
{
	if (!Function::InRange(_eEffect, static_cast<EFFECT>(0), EFFECT::MAX))
	{
		MSG_RETURN(, "CTool_Effect::Push_EffectInstance", "Invalid EFFECT");
	}

	switch (_eEffect)
	{
	case EFFECT::VFX_ParticlePoint:
	{
		shared_ptr<CEffectInstance_ParticlePoint> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePoint>(CEffectInstance_ParticlePoint::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticlePoint tDesc = any_cast<VFX_ParticlePoint>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectInstance", "Invalid Texture");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticlePointTarget:
	{
		shared_ptr<CEffectInstance_ParticlePointTarget> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointTarget>(CEffectInstance_ParticlePointTarget::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticlePointTarget tDesc = any_cast<VFX_ParticlePointTarget>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectInstance", "Invalid Texture");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_TrailLineDiffuse:
	{
		shared_ptr<CEffectInstance_TrailLineDiffuse> pInstance = dynamic_pointer_cast<CEffectInstance_TrailLineDiffuse>(CEffectInstance_TrailLineDiffuse::Create(m_pDevice, m_pContext)->Clone());
		VFX_TrailLineDiffuse tDesc = any_cast<VFX_TrailLineDiffuse>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_TrailQuad:
	{
		shared_ptr<CEffectInstance_TrailQuadDiffuse> pInstance = dynamic_pointer_cast<CEffectInstance_TrailQuadDiffuse>(CEffectInstance_TrailQuadDiffuse::Create(m_pDevice, m_pContext)->Clone());
		VFX_TrailQuad tDesc = any_cast<VFX_TrailQuad>(_tDesc);
		if (nullptr == tDesc.pDiffuse)
		{
			MSG_BREAK("CTool_Effect::Push_EffectInstance", "Invalid Texture");
		}
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_MeshAfterImage:
	{
		shared_ptr<CEffectInstance_MeshAfterImage> pInstance = dynamic_pointer_cast<CEffectInstance_MeshAfterImage>(CEffectInstance_MeshAfterImage::Create(m_pDevice, m_pContext)->Clone());
		VFX_MeshAfterImage tDesc = any_cast<VFX_MeshAfterImage>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticleMesh:
	{
		shared_ptr<CEffectInstance_ParticleMesh> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleMesh>(CEffectInstance_ParticleMesh::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticleMesh tDesc = any_cast<VFX_ParticleMesh>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticleRect:
	{
		shared_ptr<CEffectInstance_ParticleRect> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleRect>(CEffectInstance_ParticleRect::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticleRect tDesc = any_cast<VFX_ParticleRect>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticlePointCurve:
	{
		shared_ptr<CEffectInstance_ParticlePointCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointCurve>(CEffectInstance_ParticlePointCurve::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticlePointCurve tDesc = any_cast<VFX_ParticlePointCurve>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticlePointDynamicCurve:
	{
		shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointDynamicCurve>(CEffectInstance_ParticlePointDynamicCurve::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticlePointDynamicCurve tDesc = any_cast<VFX_ParticlePointDynamicCurve>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	case EFFECT::VFX_ParticlePointVectorFieldCurve:
	{
		shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pInstance = dynamic_pointer_cast<CEffectInstance_ParticlePointVectorFieldCurve>(CEffectInstance_ParticlePointVectorFieldCurve::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticlePointVectorFieldCurve tDesc = any_cast<VFX_ParticlePointVectorFieldCurve>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	case EFFECT::VFX_ParticleLine:
	{
		shared_ptr<CEffectInstance_ParticleLine> pInstance = dynamic_pointer_cast<CEffectInstance_ParticleLine>(CEffectInstance_ParticleLine::Create(m_pDevice, m_pContext)->Clone());
		VFX_ParticleLine tDesc = any_cast<VFX_ParticleLine>(_tDesc);
		pInstance->Set_Data(tDesc);
		m_vecEffectInstances.emplace_back(pInstance);
	}
	break;
	}

	m_vecEffectInstanceTags.resize(m_vecEffectInstances.size());
}

void CTool_Effect::Push_EffectInstance(shared_ptr<CEffectInstance> _pEffect, const string& _strTag)
{
	m_vecEffectInstances.emplace_back(_pEffect);
	m_vecEffectInstanceTags.resize(m_vecEffectInstances.size());
	m_vecEffectInstanceTags.back() = _strTag;
}

HRESULT CTool_Effect::Save_Effects(string _strResourcePath)
{
	for (auto& pEffect : m_vecEffectRegulars)
	{
		if (m_mapEffectPath.end() != m_mapEffectPath.find(pEffect))
		{
			if (FAILED(Export_Effect(pEffect,
				Function::ModString(CGameInstance::Get_Instance()->AbsolutePath(_strResourcePath), "\\", "/") +
				Function::ModString(m_mapEffectPath[pEffect], "Bin"))))
			{
				return E_FAIL;
			}
		}
	}
	for (auto& pEffect : m_vecEffectInstances)
	{
		if (m_mapEffectPath.end() != m_mapEffectPath.find(pEffect))
		{
			if (FAILED(Export_Effect(pEffect,
				Function::ModString(CGameInstance::Get_Instance()->AbsolutePath(_strResourcePath), "\\", "/") +
				Function::ModString(m_mapEffectPath[pEffect], "Bin"))))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CTool_Effect::Load_LoadTexture(const string& _strLoadTextureFilePath)
{
	shared_ptr<CTexture> pTexture = CTexture::Create(m_pDevice, m_pContext, Function::ConvertString(_strLoadTextureFilePath), 1, true);
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Load_LoadTexture", "Failed to CTexture::Create");
	}

	m_vecLoadTextures.emplace_back(pTexture);
	m_vecLoadTextureTags.resize(m_vecLoadTextures.size());

	return S_OK;
}

HRESULT CTool_Effect::Load_LoadMesh(const string& _strLoadMeshFilePath)
{
	shared_ptr<CMesh> pMesh = CMesh::Read(m_pDevice, m_pContext, Function::ConvertString(_strLoadMeshFilePath));
	if (nullptr == pMesh)
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Load_LoadMesh", "Failed to CMesh::Read");
	}

	m_vecLoadMeshes.emplace_back(pMesh);
	m_vecLoadMeshTags.resize(m_vecLoadMeshes.size());

	return S_OK;
}

HRESULT CTool_Effect::Load_LoadMeshInstance(const string& _strLoadMeshFilePath)
{
	shared_ptr<CVIBufferInstance_Mesh> pMesh = CVIBufferInstance_Mesh::Read(m_pDevice, m_pContext, Function::ConvertString(_strLoadMeshFilePath));
	if (nullptr == pMesh)
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Load_LoadMeshInstance", "Failed to CVIBufferInstance_Mesh::Read");
	}

	m_vecLoadMeshInstances.emplace_back(pMesh);
	m_vecLoadMeshInstanceTags.resize(m_vecLoadMeshInstances.size());

	return S_OK;
}

HRESULT CTool_Effect::Export_MeshInstance(const string& _strFilePath)
{
	if (FAILED(m_pLoadMeshInstance->Export(Function::ConvertString(_strFilePath))))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Export_MeshInstance", "Failed to CVIBufferInstance_Mesh::Export");
	}

	return S_OK;
}

HRESULT CTool_Effect::Export_LoadTexture(const string& _strLoadTextureFilePath)
{
	return S_FALSE;
}

shared_ptr<CEffect> CTool_Effect::Load_Effect(const string& _strFilePath)
{
	std::ifstream inFile(_strFilePath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(nullptr, "CTool_Effect::Load_Effect", "Failed to Open File");
	}

	shared_ptr<CEffect> pInstance;
	EFFECT eEffect(EFFECT::MAX);
	inFile.read(reinterpret_cast<_byte*>(&eEffect),	sizeof(EFFECT));

	switch (eEffect)
	{
	case EFFECT::VFX_Visual:
		pInstance = CEffectRegular_Visual::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_Distortion:
		pInstance = CEffectRegular_Distortion::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_VisualMesh:
		pInstance = CEffectRegular_VisualMesh::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePoint:
		pInstance = CEffectInstance_ParticlePoint::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointTarget:
		pInstance = CEffectInstance_ParticlePointTarget::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_TrailLineDiffuse:
		pInstance = CEffectInstance_TrailLineDiffuse::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_TrailQuad:
		pInstance = CEffectInstance_TrailQuadDiffuse::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_MeshAfterImage:
		pInstance = CEffectInstance_MeshAfterImage::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_ParticleMesh:
		pInstance = CEffectInstance_ParticleMesh::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_ParticleRect:
		pInstance = CEffectInstance_ParticleRect::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_ParticlePointCurve:
		pInstance = CEffectInstance_ParticlePointCurve::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_ParticlePointDynamicCurve:
		pInstance = CEffectInstance_ParticlePointDynamicCurve::Read(m_pDevice, m_pContext, inFile);
		break;
		
	case EFFECT::VFX_VisualCurve:
		pInstance = CEffectRegular_VisualCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_MeshAnim:
		pInstance = CEffectRegular_MeshAnim::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticlePointVectorFieldCurve:
		pInstance = CEffectInstance_ParticlePointVectorFieldCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_VisualDetailCurve:
		pInstance = CEffectRegular_VisualDetailCurve::Read(m_pDevice, m_pContext, inFile);
		break;

	case EFFECT::VFX_ParticleLine:
		pInstance = CEffectInstance_ParticleLine::Read(m_pDevice, m_pContext, inFile);
		break;

	default:
		MSG_RETURN(nullptr, "CTool_Effect::Load_Effect", "Invalid EFFECT");
	}

	inFile.close();

	m_mapEffectPath.emplace(pInstance, _strFilePath);

	return pInstance;
}

void CTool_Effect::Load_Effects(EFFECT _eEffect)
{
	auto itKey = CEffect_Manager::Get_Instance()->Load_Effects(_eEffect);
	for (auto iter = itKey.first; iter != itKey.second; ++iter)
	{
		auto strPath = Function::ConvertString(iter->second);
		auto pEffect = Load_Effect(strPath);
		string strFile;
		Function::SplitPath(strPath, nullptr, nullptr, &strFile, nullptr);
		if (pEffect)
		{
			if (shared_ptr<CEffectRegular> pInstance = dynamic_pointer_cast<CEffectRegular>(pEffect))
			{
				Push_EffectRegular(pInstance, strFile);
			}
			else if (shared_ptr<CEffectInstance> pInstance = dynamic_pointer_cast<CEffectInstance>(pEffect))
			{
				Push_EffectInstance(pInstance, strFile);
			}
			else
			{
				MSG_BOX("CTool_Effect::Load_Effects", "Failed to cast");
			}
		}
		else
		{
			MSG_BOX("CTool_Effect::Load_Effects", "Failed to Load_Effect");
		}
	}
}

HRESULT CTool_Effect::Export_Effect(shared_ptr<CEffect> _pEffect, const string& _strFilePath)
{
	if (FAILED(_pEffect->Export(Function::ConvertString(_strFilePath))))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Export_Effect", "Failed to CEffect::Export");
	}

	return S_OK;
}

HRESULT CTool_Effect::Push_TrailTarget(const string& _strFilePath)
{
	m_pTargetModel = CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, Function::ConvertString(_strFilePath));
	
	if(nullptr == m_pTargetModel)
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Push_TrailTarget", "Failed to CModel::Create");
	}

	return S_OK;
}

HRESULT CTool_Effect::Bind_DummyMaterials()
{
	MATERIALDESC tMtrlDesc;

	if (FAILED(m_pModelShader->Bind_Vector(SHADER_MTRLDIF, tMtrlDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pModelShader->Bind_Vector(SHADER_MTRLAMB, tMtrlDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pModelShader->Bind_Vector(SHADER_MTRLSPC, tMtrlDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pModelShader->Bind_Vector(SHADER_MTRLEMS, tMtrlDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pModelShader->Bind_Float(SHADER_MTRLSHN, tMtrlDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CTool_Effect::Bind_DummyMaterials", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	return S_OK;
}

#ifdef TOOL_DEMO
HRESULT CTool_Effect::Demo_Plot()
{
	ImPlot::ShowDemoWindow();

	return S_OK;
}

HRESULT CTool_Effect::Demo_Ease()
{
	ImGui::Begin("Ease##CTool_Effect::Demo_Ease");
	
	static Curves::EASE_TYPE eEase = Curves::EASE_TYPE::TYPE_IN;
	static Curves::EASE eFunction = Curves::EASE::LINEAR;
	static const _char* szEase = "In";
	static const _char* szFunction = "Linear";

	if (ImGui::BeginCombo("Type##CTool_Effect::Demo_Ease::EaseType", szEase))
	{
		const _char* items[] = { "In", "Out", "InOut" };
		if (ImGui::Selectable(items[0], szEase == items[0]))
		{
			szEase = items[0];
			eEase = Curves::EASE_TYPE::TYPE_IN;
		}
		if (ImGui::Selectable(items[1], szEase == items[1]))
		{
			szEase = items[1];
			eEase = Curves::EASE_TYPE::TYPE_OUT;
		}
		if (ImGui::Selectable(items[2], szEase == items[2]))
		{
			szEase = items[2];
			eEase = Curves::EASE_TYPE::TYPE_INOUT;
		}
		ImGui::EndCombo();
	}
	if (ImGui::BeginCombo("Function##CTool_Effect::Demo_Ease", szFunction))
	{
		const _char* items[] = { "Linear", "Sine", "Polynominal", "Sinusoidal", "Exponential", "Circular", "Elastic", "Back", "Bounce", "Overshoot" };
		if (ImGui::Selectable(items[0], szFunction == items[0]))
		{
			szFunction = items[0];
			eFunction = Curves::EASE::LINEAR;
		}
		if (ImGui::Selectable(items[1], szFunction == items[1]))
		{
			szFunction = items[1];
			eFunction = Curves::EASE::SINE;
		}
		if (ImGui::Selectable(items[2], szFunction == items[2]))
		{
			szFunction = items[2];
			eFunction = Curves::EASE::POLYNOMIAL;
		}
		if (ImGui::Selectable(items[3], szFunction == items[3]))
		{
			szFunction = items[3];
			eFunction = Curves::EASE::SINUSOIDAL;
		}
		if (ImGui::Selectable(items[4], szFunction == items[4]))
		{
			szFunction = items[4];
			eFunction = Curves::EASE::EXPONENTIAL;
		}
		if (ImGui::Selectable(items[5], szFunction == items[5]))
		{
			szFunction = items[5];
			eFunction = Curves::EASE::CIRCULAR;
		}
		if (ImGui::Selectable(items[6], szFunction == items[6]))
		{
			szFunction = items[6];
			eFunction = Curves::EASE::ELASTIC;
		}
		if (ImGui::Selectable(items[7], szFunction == items[7]))
		{
			szFunction = items[7];
			eFunction = Curves::EASE::BACK;
		}
		if (ImGui::Selectable(items[8], szFunction == items[8]))
		{
			szFunction = items[8];
			eFunction = Curves::EASE::BOUNCE;
		}
		if (ImGui::Selectable(items[9], szFunction == items[9]))
		{
			szFunction = items[9];
			eFunction = Curves::EASE::OVERSHOOT;
		}
		ImGui::EndCombo();
	}

	static _bool bPower(false);
	static _float fPower(2.f);
	if (ImGui::Checkbox("Power##CTool_Effect::Demo_Ease::Checkbox", &bPower))
	{
	}
	if (bPower)
	{
		ImGui::SameLine();
		ImGui::InputFloat("Power##CTool_Effect::Demo_Ease::InputFloat", &fPower);
	}

	static vector<_float> x(static_cast<size_t>(PLOT_PRECISION + 1)), y(static_cast<size_t>(PLOT_PRECISION + 1));
	
	switch (eEase)
	{
	case Curves::EASE_TYPE::TYPE_IN:
		for (_uint i = 0; i <= PLOT_PRECISION; i++)
		{
			x[i] = i / PLOT_PRECISION;
			y[i] = Curves::Ease_In(eFunction, x[i], bPower ? fPower : 0.f);
		}
		break;
	case Curves::EASE_TYPE::TYPE_OUT:
		for (_uint i = 0; i <= 100; i++)
		{
			x[i] = i / PLOT_PRECISION;
			y[i] = Curves::Ease_Out(eFunction, x[i], bPower ? fPower : 0.f);
		}
		break;
	case Curves::EASE_TYPE::TYPE_INOUT:
		for (_uint i = 0; i <= PLOT_PRECISION; i++)
		{
			x[i] = i / PLOT_PRECISION;
			y[i] = Curves::Ease_InOut(eFunction, x[i], bPower ? fPower : 0.f);
		}
		break;
	}

	if (ImPlot::BeginPlot("Plot"))
	{
		ImPlot::PlotLine("Line", x.data(), y.data(), static_cast<_int>(PLOT_PRECISION + 1.f));
		ImPlot::EndPlot();
	}

	ImGui::End();

	return S_OK;
}

HRESULT CTool_Effect::Demo_Curve()
{
	ImGui::Begin("Curves##CTool_Effect::Demo_Ease");

	static vector<_float> x(101), y(101);

	for (_uint i = 0; i < 101; i++)
	{
		x[i] = i / 100.f;
		y[i] = i / 100.f;

		_float2 b = Curves::Bezier_Curve(initializer_list{ _float2(0.f, 0.f), _float2(0.f, 1.f) ,_float2(1.f, 1.f) }, x[i]);
		y[i] = b.y;
	}

	if (ImPlot::BeginPlot("Plot"))
	{
		ImPlot::PlotLine("Line", x.data(), y.data(), 101);
		ImPlot::EndPlot();
	}

	for (_uint i = 0; i < 101; i++)
	{
		x[i] = i / 100.f;
		y[i] = i / 100.f;

		_float2 b = Curves::Bezier_Curve(initializer_list{ _float2(0.f, 0.f), _float2(0.f, 1.f) ,_float2(1.f, 1.f) }, x[i]);
		x[i] = b.x;
		y[i] = b.y;
	}

	if (ImPlot::BeginPlot("Plot"))
	{
		ImPlot::PlotLine("Line", x.data(), y.data(), 101);
		ImPlot::EndPlot();
	}

	ImGui::End();

	return S_OK;
}
#endif

shared_ptr<CTool_Effect> CTool_Effect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTool_Effect> pInstance =  make_private_shared(CTool_Effect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTool_Effect::Create", "Failed to Initialize");
	}

	return pInstance;
}

#endif
