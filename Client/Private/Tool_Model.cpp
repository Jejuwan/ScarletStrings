#include "ClientPCH.h"
#include "Tool_Model.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Bone.h"
#include "Channel.h"
#include "Mesh.h"
#include "Event.h"

#define DIALOG_LOAD					"Dialog:Model:Load"
#define DIALOG_EXPORT				"Dialog:Model:Export"
#define DIALOG_EXPORT_MESH			"Dialog:Mesh:Export"
#define DIALOG_LOAD_TEXTURE			"Dialog:Texture:Load"
#define DIALOG_LOAD_SOUNDFILE		"Dialog:SoundFile:Load"
#define DIALOG_EXPORT_KEYFRAME		"Dialog:Model:Export:Keyframe"

#if ACTIVATE_TOOL
CTool_Model::CTool_Model(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: IToolSystem(_pDevice, _pContext)
{
}

HRESULT CTool_Model::Initialize()
{
	m_pRenderer = CGameInstance::Get_Instance()->Clone_Component<CRenderer>(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN);

	m_vecShaders.emplace_back(CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTex.hlsl"), VTXMESH::tElements, VTXMESH::iNumElement));
	m_vecShaders.emplace_back(CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTexBone.hlsl"), VTXMESHANIM::tElements, VTXMESHANIM::iNumElement));

	m_pDummyTransform = CTransform::Create(m_pDevice, m_pContext);

	return S_OK;
}

void CTool_Model::Tick(_float _fTimeDelta)
{
	if (m_pModel)
	{
		if(m_bAnimationPlay)
			m_pModel->Tick_Animation(_fTimeDelta);
		if(m_iAnimationCursor != -1)
			m_iTrackPosition = (_uint)m_pModel->Get_TrackPosition();
		if (CGameInstance::Get_Instance()->Key_Down(VK_SPACE))
			m_bAnimationPlay = !m_bAnimationPlay;
	}
}

void CTool_Model::Late_Tick(_float _fTimeDelta)
{
	if (RENDER_GROUP::MAX != m_eRenderGroup && m_pShader)
	{
		m_pRenderer->Add_RenderObject(m_eRenderGroup, shared_from_this());
	}
}

HRESULT CTool_Model::Render()
{
	if (m_pModel)
	{
		if (FAILED(Bind_DummyMaterials()))
		{
			MSG_RETURN(E_FAIL, "CTool_Model::Render", "Failed to Bind_DummyMaterials");
		}
		if (FAILED(m_pDummyTransform->Bind_OnShader(m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CTool_Model::Render", "Failed to CTransform::Bind_OnShader");
		}
		if (FAILED(m_pModel->Render(m_pShader, m_iShaderPass)))
		{
			MSG_RETURN(E_FAIL, "CTool_Model::Render", "Failed to CModel::Render");
		}
	}

	return S_OK;
}

void CTool_Model::Tool_MenuBar()
{
	if (ImGui::BeginMenu("Shader##CTool_Model::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("VTXMESH##CTool_Model::Tool_MenuBar"))
		{
			m_pShader = m_vecShaders[0];
		}
		if (ImGui::MenuItem("VTXMESHANIM##CTool_Model::Tool_MenuBar"))
		{
			m_pShader = m_vecShaders[1];
		}

		ImGui::EndMenu();
	}

	if (m_pShader)
	{
		if (ImGui::BeginMenu("Shader Pass##CTool_Model::Tool_MenuBar"))
		{
			for (_uint i = 0; i < m_pShader->Get_NumPasses(); ++i)
			{
				if (ImGui::MenuItem(string("ShaderPass " + std::to_string(i) + "##CTool_Model::Tool_MenuBar").c_str()))
				{
					m_iShaderPass = i;
				}
			}

			ImGui::EndMenu();
		}
	}

	if (ImGui::BeginMenu("Render Group##CTool_Model::Tool_MenuBar"))
	{
		if (ImGui::MenuItem("Camera##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::CAMERA;
		}
		if (ImGui::MenuItem("Priority##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::PRIORITY;
		}
		if (ImGui::MenuItem("Shadow##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::SHADOW;
		}
		if (ImGui::MenuItem("NonBlend##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::NONBLEND;
		}
		if (ImGui::MenuItem("Outline##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::OUTLINE;
		}
		if (ImGui::MenuItem("Decal##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::DECAL;
		}
		if (ImGui::MenuItem("NonLight##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::NONLIGHT;
		}
		if (ImGui::MenuItem("Blend##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::BLEND;
		}
		if (ImGui::MenuItem("UI##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::UI;
		}
#ifdef _DEBUG
		if (ImGui::MenuItem("Debug##CTool_Model::Tool_MenuBar"))
		{
			m_eRenderGroup = RENDER_GROUP::DEBUG;
		}
#endif

		ImGui::EndMenu();
	}
}

void CTool_Model::Tool_System()
{
	static MODEL	eModel(MODEL::ANIM);

	if (ImGui::Button("Load##CTool_Model::Tool_System"))
	{
		if (!m_imfdEmbed_Load.IsOpened())
		{
			if (m_imfdEmbed_Export.IsOpened())
			{
				m_imfdEmbed_Export.Close();
			}

			const _char* szFilters = "Models (*.fbx, *.mdl){.fbx,.mdl},FBX (*.fbx){.fbx},Binary (*.mdl){.mdl},All files{.*}";
			m_imfdEmbed_Load.OpenDialog(DIALOG_LOAD, "Open", szFilters, "Bin/Resources/", 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType |
				ImGuiFileDialogFlags_NoDialog |
				ImGuiFileDialogFlags_DisableBookmarkMode |
				ImGuiFileDialogFlags_ReadOnlyFileNameField);
		}
		else
		{
			m_imfdEmbed_Load.Close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Export##CTool_Model::Tool_System"))
	{
		if (!m_imfdEmbed_Export.IsOpened())
		{
			if (m_imfdEmbed_Load.IsOpened())
			{
				m_imfdEmbed_Load.Close();
			}

			const _char* szFilters = "Binary (*.mdl){.mdl},Models (*.fbx, *.mdl){.fbx,.mdl},All files{.*}";
			m_imfdEmbed_Export.OpenDialog(DIALOG_EXPORT, "Export", szFilters, "../Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				0, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType |
				ImGuiFileDialogFlags_NoDialog |
				ImGuiFileDialogFlags_DisableBookmarkMode |
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imfdEmbed_Export.Close();
		}
	}

	// For Export Only Animation Keyframes
	ImGui::SameLine();
	if (ImGui::Button("Export_Keyframes##CTool_Model::Tool_System"))
	{
		if (!m_imfdEmbed_Export.IsOpened())
		{
			if (m_imfdEmbed_Load.IsOpened())
			{
				m_imfdEmbed_Load.Close();
			}

			const _char* szFilters = "Binary (*.mdl){.mdl},Models (*.fbx, *.mdl){.fbx,.mdl},All files{.*}";
			m_imfdEmbed_Export.OpenDialog(DIALOG_EXPORT_KEYFRAME, "Export", szFilters, "../Resources/", "",
				[](const char*, void*, bool*)
				{
					ImGui::Text("");
				},
				0, 1, nullptr,
				ImGuiFileDialogFlags_HideColumnType |
				ImGuiFileDialogFlags_NoDialog |
				ImGuiFileDialogFlags_DisableBookmarkMode |
				ImGuiFileDialogFlags_ConfirmOverwrite);
		}
		else
		{
			m_imfdEmbed_Export.Close();
		}
	}

	ImGui::SameLine();
	if (ImGui::RadioButton("Anim", eModel == MODEL::ANIM))
	{
		eModel = MODEL::ANIM;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("NonAnim", eModel == MODEL::NONANIM))
	{
		eModel = MODEL::NONANIM;
	}
#pragma region Pivot

	static _bool		bShowPivotSettings(false);
	static _float3		vPivotScale(0.01f, 0.01f, 0.01f);
	static _float3		vPivotRotation(0.f, 180.f, 0.f);
	static _float3		vPivotTranslation(0.f, 0.f, 0.f);
	static _float4x4	mPivot = g_mUnit;

	//static _bool		bShowPivotSettings(false);
	//static _float3	vPivotScale(1.0f, 1.0f, 1.0f);
	//static _float3	vPivotRotation(0.f, 0.f, 0.f);
	//static _float3	vPivotTranslation(0.f, 0.f, 0.f);
	//static _float4x4	mPivot = g_mUnit;

	_float fWindowWidth = ImGui::GetContentRegionAvail().x;
	_float fButtonWidth = ImGui::CalcTextSize("Pivot").x + ImGui::GetStyle().FramePadding.x * 2.f;

	ImGui::SameLine(fWindowWidth - fButtonWidth + ImGui::GetStyle().FramePadding.x);

	if (ImGui::Button("Pivot", ImVec2(fButtonWidth, 0)))
	{
		bShowPivotSettings = !bShowPivotSettings;
	}
	ImGui::Separator();

	static _uint iTab(1);
	if (bShowPivotSettings)
	{
		if (ImGui::BeginTabBar("Tab:Pivot"))
		{
			if (ImGui::BeginTabItem("Value"))
			{
				if (ImGui::Button("Reset Pivot"))
				{
					vPivotScale			= _float3(1.f, 1.f, 1.f);
					vPivotRotation		= _float3(0.f, 0.f, 0.f);
					vPivotTranslation	= _float3(0.f, 0.f, 0.f);
				}

				if (iTab == 2)
				{
					iTab = 1;
					_vector vScale, vQuaternion, vTranslation;
					if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, mPivot))
					{
						vPivotScale			= vScale;
						vPivotRotation		= Function::QuaternionToEuler(vQuaternion, false);
						vPivotTranslation	= vTranslation;
					}
					else
					{
						MSG_BOX("CTool_Model:Tool_System", "Failed to XMMatrixDecompose");
					}
				}
				ImGui::DragFloat3("Scale",				reinterpret_cast<_float*>(&vPivotScale));
				ImGui::DragFloat3("Rotation(Degree)",	reinterpret_cast<_float*>(&vPivotRotation));
				ImGui::DragFloat3("Translation",		reinterpret_cast<_float*>(&vPivotTranslation));
				ImGui::EndTabItem();

				mPivot = XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
					_float3(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);
			}

			if (ImGui::BeginTabItem("Matrix"))
			{
				if (ImGui::Button("Reset Pivot"))
				{
					vPivotScale			= _float3(1.f, 1.f, 1.f);
					vPivotRotation		= _float3(0.f, 0.f, 0.f);
					vPivotTranslation	= _float3(0.f, 0.f, 0.f);
					mPivot				= g_mUnit;
				}

				if (iTab == 1)
				{
					iTab = 2;
					mPivot = XMMatrixAffineTransformation(vPivotScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
						_float3(XMConvertToRadians(vPivotRotation.x), XMConvertToRadians(vPivotRotation.y), XMConvertToRadians(vPivotRotation.z))), vPivotTranslation);
				}
				ImGui::DragFloat4("Right",		reinterpret_cast<_float*>(&mPivot._11));
				ImGui::DragFloat4("Up",			reinterpret_cast<_float*>(&mPivot._21));
				ImGui::DragFloat4("Look",		reinterpret_cast<_float*>(&mPivot._31));
				ImGui::DragFloat4("Position",	reinterpret_cast<_float*>(&mPivot._41));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}

#pragma endregion
#pragma region File Dialog Instance

	if (m_imfdEmbed_Load.Display(DIALOG_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Load.IsOk())
		{
			if (FAILED(Push_Model(Function::ConvertString(m_imfdEmbed_Load.GetFilePathName()), eModel, mPivot)))
			{
				MSG_BOX("CTool_Model::Tool_System", "Failed to Push_Model");
			}
		}
	}

	if (m_imfdEmbed_Export.Display(DIALOG_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Export.IsOk())
		{
			if (FAILED(Export_Model(Function::ConvertString(m_imfdEmbed_Export.GetFilePathName()))))
			{
				MSG_BOX("CTool_Model::Tool_System", "Failed to Export_Model");
			}
		}
	}

	if (m_imfdEmbed_Export.Display(DIALOG_EXPORT_KEYFRAME, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Export.IsOk())
		{
			if (FAILED(Export_Animation(Function::ConvertString(m_imfdEmbed_Export.GetFilePathName()))))
			{
				MSG_BOX("CTool_Model::Tool_System", "Failed to Export_Animation");
			}
		}
	}

#pragma endregion

	if (m_imfdEmbed_Load.IsOpened() || m_imfdEmbed_Export.IsOpened())
	{
		ImGui::Separator();
	}

#pragma region Model List

	ImGui::SeparatorText("Model List");

	_float fWindowWidth0 = ImGui::GetWindowWidth();
	_float fButtonWidth0 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
	_float fButtonSpace0 = ImGui::GetStyle().ItemSpacing.x;

	ImGui::Text("Anim: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 - fButtonWidth0);
	if (ImGui::Button("-##EraseFromAnim", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		if (-1 != m_iAnimModelCursor)
		{
			m_vecAnimModels.erase(m_vecAnimModels.begin() + m_iAnimModelCursor);
			m_vecAnimModelTags.resize(m_vecAnimModels.size());

			m_iAnimModelCursor		= -1;
			m_pAnimModel			= nullptr;
			m_pModel				= nullptr;

			m_iBoneCursor			= -1;
			m_pBone					= nullptr;

			m_iAnimationCursor		= -1;
			m_pAnimation			= nullptr;
			m_iChannelCursor		= 0;
			m_pChannel				= nullptr;
			m_iKeyFrameCursor		= 0;
			m_tKeyFrame				= KEYFRAME{};

			m_iMeshCursor			= -1;
			m_pMesh					= nullptr;

			m_iMaterialCursor		= -1;
			m_tMaterial				= MATERIAL{};
			m_iTextureCursor		= -1;
			m_pTexture				= nullptr;

			m_eFocus				= MODELFOCUS::MAX;
		}
	}
	if (ImGui::BeginListBox("Anim", ImVec2(-FLT_MIN, 0.f)))
	{
		for (size_t i = 0; i < m_vecAnimModels.size(); ++i)
		{
			string strAnimModelTag;
			if (m_vecAnimModelTags[i].empty())
			{
				strAnimModelTag = string("AnimModel " + std::to_string(i));
			}
			else
			{
				strAnimModelTag = m_vecAnimModelTags[i];
			}

			if (ImGui::Selectable(strAnimModelTag.c_str(), static_cast<_int>(i) == m_iAnimModelCursor, ImGuiSelectableFlags_AllowDoubleClick))
			{
				m_iAnimModelCursor		= static_cast<_int>(i);
				m_pAnimModel			= m_vecAnimModels[i];
				m_pModel				= m_pAnimModel;

				m_iBoneCursor			= -1;
				m_pBone					= nullptr;

				m_iAnimationCursor		= -1;
				m_pAnimation			= nullptr;
				m_iChannelCursor		= 0;
				m_pChannel				= nullptr;
				m_iKeyFrameCursor		= 0;
				m_tKeyFrame				= KEYFRAME{};

				m_iMeshCursor			= -1;
				m_pMesh					= nullptr;

				m_iMaterialCursor		= -1;
				m_tMaterial				= MATERIAL{};
				m_iTextureCursor		= -1;
				m_pTexture				= nullptr;

				m_eFocus				= MODELFOCUS::MAX;
			}
		}

		ImGui::EndListBox();
	}

	ImGui::Text("NonAnim: ");
	ImGui::SameLine(fWindowWidth0 - fButtonSpace0 - fButtonWidth0);
	if (ImGui::Button("-##EraseFromNonAnim", ImVec2(fButtonWidth0, fButtonWidth0)))
	{
		if (-1 != m_iNonAnimModelCursor)
		{
			m_vecNonAnimModels.erase(m_vecNonAnimModels.begin() + m_iNonAnimModelCursor);
			m_vecNonAnimModelTags.resize(m_vecNonAnimModels.size());

			m_iNonAnimModelCursor	= -1;
			m_pNonAnimModel			= nullptr;
			m_pModel				= nullptr;

			m_iBoneCursor			= -1;
			m_pBone					= nullptr;

			m_iAnimationCursor		= -1;
			m_pAnimation			= nullptr;
			m_iChannelCursor		= 0;
			m_pChannel				= nullptr;
			m_iKeyFrameCursor		= 0;
			m_tKeyFrame				= KEYFRAME{};

			m_iMeshCursor			= -1;
			m_pMesh					= nullptr;

			m_iMaterialCursor		= -1;
			m_tMaterial				= MATERIAL{};
			m_iTextureCursor		= -1;
			m_pTexture				= nullptr;

			m_eFocus				= MODELFOCUS::MAX;
		}
	}
	if (ImGui::BeginListBox("NonAnim", ImVec2(-FLT_MIN, 0.f)))
	{
		for (size_t i = 0; i < m_vecNonAnimModels.size(); ++i)
		{
			string strNonAnimModelTag;
			if (m_vecNonAnimModelTags[i].empty())
			{
				strNonAnimModelTag = string("NonAnimModel " + std::to_string(i));
			}
			else
			{
				strNonAnimModelTag = m_vecNonAnimModelTags[i];
			}

			if (ImGui::Selectable(strNonAnimModelTag.c_str(), static_cast<_int>(i) == m_iNonAnimModelCursor, ImGuiSelectableFlags_AllowDoubleClick))
			{
				m_iNonAnimModelCursor	= static_cast<_int>(i);
				m_pNonAnimModel			= m_vecNonAnimModels[i];
				m_pModel				= m_pNonAnimModel;

				m_iBoneCursor			= -1;
				m_pBone					= nullptr;

				m_iAnimationCursor		= -1;
				m_pAnimation			= nullptr;
				m_iChannelCursor		= 0;
				m_pChannel				= nullptr;
				m_iKeyFrameCursor		= 0;
				m_tKeyFrame				= KEYFRAME{};

				m_iMeshCursor			= -1;
				m_pMesh					= nullptr;

				m_iMaterialCursor		= -1;
				m_tMaterial				= MATERIAL{};
				m_iTextureCursor		= -1;
				m_pTexture				= nullptr;

				m_eFocus				= MODELFOCUS::MAX;
			}
		}

		ImGui::EndListBox();
	}

#pragma endregion
}

void CTool_Model::Tool_Properties()
{
	static aiTextureType	eAiTexType(aiTextureType_DIFFUSE);

	if (m_pModel)
	{
		ImGui::SeparatorText("Properties");

#pragma region Bone List
		if (ImGui::CollapsingHeader("Bones"))
		{
			if (ImGui::BeginListBox("Bone: ", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pModel->Get_NumBones(); ++i)
				{
					if (ImGui::Selectable(m_pModel->Get_Bone(i)->Get_Name(), i == m_iBoneCursor))
					{
						m_eFocus		= MODELFOCUS::BONE;
						m_iBoneCursor	= i;
						m_pBone			= m_pModel->Get_Bone(i);
					}
				}
				ImGui::EndListBox();
			}
		}
#pragma endregion
		if (MODEL::ANIM == m_pModel->Get_Type())
		{
#pragma region Animation List
			if (ImGui::CollapsingHeader("Animations"))
			{
				if (ImGui::BeginListBox("Animation: ", ImVec2(-FLT_MIN, 0.f)))
				{
					for (_uint i = 0; i < m_pAnimModel->Get_NumAnimations(); ++i)
					{
						if (ImGui::Selectable(m_pAnimModel->Get_Animation(i)->Get_Name(), i == m_iAnimationCursor))
						{
							m_eFocus			= MODELFOCUS::ANIMATION;
							m_iAnimationCursor	= i;
							m_pAnimation		= m_pAnimModel->Get_Animation(i);
							m_iChannelCursor	= 0;
							m_pChannel			= nullptr;
							m_iKeyFrameCursor	= 0;
							m_tKeyFrame			= KEYFRAME{};

							m_pAnimModel->Set_DefaultAnimation(i);
							m_pAnimModel->Set_Animation(i);
							m_pAnimModel->Reset_Animation(i);
							m_pAnimModel->Reset_Animation(i);
						}
					}

					ImGui::EndListBox();
				}
			}
#pragma endregion
		}
#pragma region Mesh List
		if (ImGui::CollapsingHeader("Meshes"))
		{
			if (ImGui::BeginListBox("Mesh: ", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
				{
					if (ImGui::Selectable(string(m_pModel->Get_Mesh(i)->Get_Name() + string(" ") + std::to_string(i)).c_str(), i == m_iMeshCursor))
					{
						m_eFocus		= MODELFOCUS::MESH;
						m_iMeshCursor	= i;
						m_pMesh			= m_pModel->Get_Mesh(i);
					}
				}

				ImGui::EndListBox();
			}
		}
#pragma endregion
#pragma region Material List
		if (ImGui::CollapsingHeader("Materials"))
		{
			if (ImGui::BeginListBox("Material: ", ImVec2(-FLT_MIN, 0.f)))
			{
				for (_uint i = 0; i < m_pModel->Get_NumMaterials(); ++i)
				{
					if (ImGui::Selectable(string("Material " + std::to_string(i)).c_str(), i == m_iMaterialCursor))
					{
						m_eFocus			= MODELFOCUS::MATERIAL;
						m_iMaterialCursor	= i;
						m_tMaterial			= m_pModel->Get_Material(i);
						m_iTextureCursor	= -1;
						m_pTexture			= nullptr;
					}
				}

				ImGui::EndListBox();
			}
		}
#pragma endregion
		switch (m_eFocus)
		{
#pragma region Bones
		case MODELFOCUS::BONE:
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Bones");
		}
		break;
#pragma endregion
#pragma region Animations
		case MODELFOCUS::ANIMATION:
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Animations");

			if (m_pAnimation)
			{
				if (ImGui::TreeNodeEx(m_pAnimation->Get_Name(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::SetNextItemWidth(120.f);
					if (ImGui::BeginCombo(string("Channel(" + std::to_string(m_pAnimation->Get_NumChannels()) + ")").c_str(), string("Channel " + std::to_string(m_iChannelCursor)).c_str()))
					{
						for (_uint i = 0; i < m_pAnimation->Get_NumChannels(); ++i)
						{
							if (ImGui::Selectable((string("Channel ") + std::to_string(i)).c_str(), i == m_iChannelCursor))
							{
								m_iChannelCursor	= i;
								m_iKeyFrameCursor	= 0;
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::IsItemHovered())
					{
						if (_float fWheelDeltaV = ImGui::GetIO().MouseWheel)
						{
							if (0 > fWheelDeltaV)
							{
								if (m_iChannelCursor < static_cast<int>(m_pAnimation->Get_NumChannels() - 1))
								{
									m_iChannelCursor	+= 1;
									m_iKeyFrameCursor	= 0;
								}
							}
							else
							{
								if (m_iChannelCursor > 0)
								{
									m_iChannelCursor	-= 1;
									m_iKeyFrameCursor	= 0;
								}
							}
						}

						if (CGameInstance::Get_Instance()->Key_Down(VK_DOWN))
						{
							if (m_iKeyFrameCursor < static_cast<_int>(m_pAnimation->Get_Channel(m_iChannelCursor)->Get_NumKeyFrames() - 1))
							{
								m_iKeyFrameCursor += 1;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_UP))
						{
							if (m_iKeyFrameCursor > 0)
							{
								m_iKeyFrameCursor -= 1;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
						{
							if (m_iChannelCursor > 0)
							{
								m_iChannelCursor	-= 1;
								m_iKeyFrameCursor	= 0;
							}
						}
						if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
						{
							if (m_iChannelCursor < static_cast<_int>(m_pAnimation->Get_NumChannels() - 1))
							{
								m_iChannelCursor	+= 1;
								m_iKeyFrameCursor	= 0;
							}
						}

						if (CGameInstance::Get_Instance()->Key_Down(VK_DELETE))
						{
							if (m_iChannelCursor != -1)
							{
								if (FAILED(m_pAnimation->Remove_Channel(m_iChannelCursor)))
								{
									MSG_BOX("CTool_Model::Tool_Properties", "Failed to Remove_Channel");
								}

								if (m_iChannelCursor == m_pAnimation->Get_NumChannels())
								{
									m_iChannelCursor -= 1;
								}

								m_iKeyFrameCursor = 0;
							}
						}

						if (CGameInstance::Get_Instance()->Key_Hold(VK_LCONTROL)
							&& CGameInstance::Get_Instance()->Key_Down('X'))
						{
							for (int i = m_pAnimation->Get_NumChannels() - 1; i >= 0; --i)
							{
								auto pChannel = m_pAnimation->Get_Channel(i);
								if (pChannel->Get_NumKeyFrames() <= 2)
								{
									m_pAnimation->Remove_Channel(i);
								}
							}
						}
						/*if (CGameInstance::Get_Instance()->Key_Hold(VK_LCONTROL)
							&& CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT)
							&& CGameInstance::Get_Instance()->Key_Down('X'))
						{
							for (size_t j = 0; j < m_pAnimModel->Get_NumAnimations(); j++)
							{
								auto pAnimation = m_pAnimModel->Get_Animation(j);
								for (int i = pAnimation->Get_NumChannels() - 1; i >= 0; --i)
								{
									auto pChannel = pAnimation->Get_Channel(i);
									if (pChannel->Get_NumKeyFrames() <= 2)
									{
										pAnimation->Remove_Channel(i);
									}
								}
							}
						}*/
					}

					_float fWindowWidth1 = ImGui::GetWindowWidth();
					_float fButtonWidth1 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
					_float fButtonSpace1 = ImGui::GetStyle().ItemInnerSpacing.x;

					static _char szBoneIndex[32] = "0";
					static _uint iFindBoneIndex = 0;
					ImGui::SameLine(fWindowWidth1 - fButtonWidth1 - ImGui::GetStyle().WindowPadding.x);
					ImGui::Button("~##FindChannel", ImVec2(fButtonWidth1, fButtonWidth1));
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Find by Bone Index:");
						if (ImGui::InputText("##Input_FindChannel", szBoneIndex, IM_ARRAYSIZE(szBoneIndex), ImGuiInputTextFlags_CharsDecimal))
						{
							_uint iChannelIdx = m_pAnimation->Get_ChannelIndex(atoi(szBoneIndex));
							if (m_pAnimation->Get_NumChannels() != iChannelIdx)
							{
								m_iChannelCursor = iChannelIdx;
							}
						}
						if (ImGui::Button("Close"))
						{
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}

					if (m_iChannelCursor != -1)
					{
						ImGui::Text("Channel: %d, KeyFrame: %d, BoneIndex: %d", m_iChannelCursor, m_iKeyFrameCursor, m_pAnimation->Get_Channel(m_iChannelCursor)->Get_BoneIndex());
						ImGui::Checkbox("PlayAnimation", &m_bAnimationPlay);
						ImGui::SameLine(fWindowWidth1 - fButtonWidth1 - ImGui::GetStyle().WindowPadding.x);
						if (ImGui::Button("-##RemoveChannel", ImVec2(fButtonWidth1, fButtonWidth1)))
						{
							if (FAILED(m_pAnimation->Remove_Channel(m_iChannelCursor)))
							{
								MSG_BOX("CTool_Model::Tool_Properties", "Failed to Remove_Channel");
							}

							if (m_iChannelCursor == m_pAnimation->Get_NumChannels())
							{
								m_iChannelCursor -= 1;
							}

							m_iKeyFrameCursor = 0;
						}

						if (ImGui::BeginListBox("Keyframe Listbox", ImVec2(-FLT_MIN, 0.f)))
						{
							for (_uint i = 0; i < m_pAnimation->Get_Channel(m_iChannelCursor)->Get_NumKeyFrames(); ++i)
							{
								ImGui::PushID(i);
								if (ImGui::Selectable(string("KeyFrame " + std::to_string(i)).c_str(), i == m_iKeyFrameCursor))
								{
									m_iKeyFrameCursor = i;
									
								}
								ImGui::PopID();
							}
							ImGui::EndListBox();
						}

						if (m_iKeyFrameCursor != -1)
						{
							KEYFRAME tSelectKeyFrame{}, tCompareKeyFrame{};

							if (m_iKeyFrameCursor < static_cast<_int>(m_pAnimation->Get_Channel(m_iChannelCursor)->Get_NumKeyFrames()))
							{
								ImGui::SeparatorText(m_pAnimModel->Get_Bone(m_pAnimation->Get_Channel(m_iChannelCursor)->Get_BoneIndex())->Get_Name());
								ImGui::BeginGroup();
								{
									tSelectKeyFrame = m_pAnimation->Get_Channel(m_iChannelCursor)->Get_KeyFrame(m_iKeyFrameCursor);

									ImGui::InputFloat4	("Scale",		reinterpret_cast<_float*>(&tSelectKeyFrame.vScale),				"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat4	("Rotation",	reinterpret_cast<_float*>(&tSelectKeyFrame.vRotation),			"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat4	("Translation",	reinterpret_cast<_float*>(&tSelectKeyFrame.vTranslation),		"%.3f", ImGuiInputTextFlags_ReadOnly);
									ImGui::InputFloat	("Time",		reinterpret_cast<_float*>(&tSelectKeyFrame.fTime), 0.f, 0.f,	"%.3f", ImGuiInputTextFlags_ReadOnly);

									m_pAnimation->Get_Channel(m_iChannelCursor)->Set_KeyFrame(m_iKeyFrameCursor, tSelectKeyFrame);
								}
								ImGui::EndGroup();
								ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(110, 110, 128, 128));

								shared_ptr<CChannel> pCompareChannel = m_pAnimModel->Get_Animation(m_iCompareAnimationIdx)->Get_Channel(
									m_pAnimation->Get_Channel(m_iChannelCursor)->Get_BoneIndex(), true);
								if (nullptr != pCompareChannel
									&& m_iKeyFrameCursor < static_cast<_int>(pCompareChannel->Get_NumKeyFrames()))
								{
									ImGui::SeparatorText(string("Compare: " + string(m_pAnimModel->Get_Animation(m_iCompareAnimationIdx)->Get_Name())).c_str());
									ImGui::BeginGroup();
									{
										tCompareKeyFrame = pCompareChannel->Get_KeyFrame(m_iKeyFrameCursor);

										ImGui::InputFloat4	("Scale##Compare",			reinterpret_cast<_float*>(&tCompareKeyFrame.vScale),			"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat4	("Rotation##Compare",		reinterpret_cast<_float*>(&tCompareKeyFrame.vRotation),			"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat4	("Translation##Compare",	reinterpret_cast<_float*>(&tCompareKeyFrame.vTranslation),		"%.3f", ImGuiInputTextFlags_ReadOnly);
										ImGui::InputFloat	("Time##Compare",			reinterpret_cast<_float*>(&tCompareKeyFrame.fTime), 0.f, 0.f,	"%.3f", ImGuiInputTextFlags_ReadOnly);
									}
									ImGui::EndGroup();
									ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(110, 110, 128, 128));

									_bool bKeyFrameEqual =
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vScale),			XMLoadFloat4(&tCompareKeyFrame.vScale))) &&
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vRotation),		XMLoadFloat4(&tCompareKeyFrame.vRotation))) &&
										Function::NearZero4(XMVectorSubtract(XMLoadFloat4(&tSelectKeyFrame.vTranslation),	XMLoadFloat4(&tCompareKeyFrame.vTranslation)));

									ImGui::Text(bKeyFrameEqual ? "Equal" : "Not Equal");
								}
							}
						}
					}

					ImGui::TreePop();
				}
			}
		}
		break;
#pragma endregion
#pragma region Meshes
		case MODELFOCUS::MESH:
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Meshes");

			if (m_pMesh)
			{
				ImGui::Text("Material Index: ", m_pMesh->Get_MaterialIndex());
				ImGui::SameLine();
				if (ImGui::Button(string("Material " + std::to_string(m_pMesh->Get_MaterialIndex())).c_str()))
				{
					m_iMaterialCursor	= m_pMesh->Get_MaterialIndex();
					m_tMaterial			= m_pModel->Get_Material(m_iMaterialCursor);
					m_iTextureCursor	= -1;
				}
			}

			if (ImGui::TreeNode("Hide"))
			{
				ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 8), true);

				for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
				{
					string	strLabel = "Mesh " + std::to_string(i);
					_bool	bHidden = m_pModel->Is_MeshHidden(i);
					if (ImGui::Checkbox(strLabel.c_str(), &bHidden))
					{
						m_pModel->Hide_Mesh(i, bHidden);
					}
				}

				ImGui::EndChild();
				ImGui::TreePop();
			}

			if (m_pMesh)
			{
				if (ImGui::Button("Export"))
				{
					const _char* szFilters = "Meshes (*.msh,*.fbx){.msh},Binary (*.msh){.msh},FBX (*.fbx){.fbx},All files{.*}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_EXPORT_MESH, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
			}
		}
		break;
#pragma endregion
#pragma region Materials
		case MODELFOCUS::MATERIAL:
		{
			ImGui::NewLine();
			ImGui::SeparatorText("Materials");

			static _int		iCurrentMaterialIdx(0);
			const _char*	szTextureTypes[] = { "Diffuse", "Normal", "Ambient", "Specular", "Emissive" };
			const _char*	szPreview = szTextureTypes[iCurrentMaterialIdx];
			if (ImGui::TreeNodeEx(string("Material " + std::to_string(m_iMaterialCursor)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SetNextItemWidth(120.f);
				if (ImGui::BeginCombo("Type", szPreview))
				{
					for (int i = 0; i < IM_ARRAYSIZE(szTextureTypes); ++i)
					{
						if (ImGui::Selectable(szTextureTypes[i], i == iCurrentMaterialIdx))
						{
							szPreview				= szTextureTypes[i];
							iCurrentMaterialIdx		= i;
							m_iTextureCursor		= -1;
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::IsItemHovered())
				{
					if (_float fWheelDeltaV = ImGui::GetIO().MouseWheel)
					{
						if (0 > fWheelDeltaV && iCurrentMaterialIdx == 0)
						{
							iCurrentMaterialIdx	= 1;
							m_iTextureCursor	= -1;
						}
						if (0 < fWheelDeltaV && iCurrentMaterialIdx == 1)
						{
							iCurrentMaterialIdx	= 0;
							m_iTextureCursor	= -1;
						}
					}
				}

				switch (iCurrentMaterialIdx)
				{
				case 0:
					eAiTexType = aiTextureType_DIFFUSE;
					break;
				case 1:
					eAiTexType = aiTextureType_NORMALS;
					break;
				case 2:
					eAiTexType = aiTextureType_AMBIENT;
					break;
				case 3:
					eAiTexType = aiTextureType_SPECULAR;
					break;
				case 4:
					eAiTexType = aiTextureType_EMISSIVE;
					break;
				}

				_float fWindowWidth1 = ImGui::GetWindowWidth();
				_float fButtonWidth1 = ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemInnerSpacing.y;
				_float fButtonSpace1 = ImGui::GetStyle().ItemInnerSpacing.x;

				ImGui::SameLine(fWindowWidth1 - fButtonWidth1 * 5.f - fButtonSpace1 * 4.f - ImGui::GetStyle().WindowPadding.x);
				if (ImGui::Button("*##DuplicateFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					if (m_iTextureCursor != -1)
					{
						if (FAILED(m_tMaterial.pTexture[IDX(eAiTexType)]->Push_ShaderResourceView(m_tMaterial.pTexture[IDX(eAiTexType)]->Get_TexturePath(m_iTextureCursor), true)))
						{
							MSG_BOX("CTool_Model::Tool_Properties", "Failed to Push_ShaderResourceView");

						}
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::Button("+##AddFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					const _char* szFilters = "All files{.*},WIC files(*.png *.jpg *.jpeg){.png,.jpg,.jpeg},DDS files(*.dds){.dds}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_LOAD_TEXTURE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::Button("-##EraseFromMaterial", ImVec2(fButtonWidth1, fButtonWidth1)))
				{
					if (m_iTextureCursor != -1)
					{
						if (SUCCEEDED(m_tMaterial.pTexture[IDX(eAiTexType)]->Remove_ShaderResourceView(m_iTextureCursor)))
						{
							m_iTextureCursor = -1;
						}
						else
						{
							MSG_BOX("CTool_Model::Tool_Properties", "Failed to Remove_ShaderResourceView");
						}
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::ArrowButton("##UpFromMaterial", ImGuiDir_Up))
				{
					if (m_tMaterial.pTexture[IDX(eAiTexType)]->Swap_ShaderResourceView(m_iTextureCursor, m_iTextureCursor - 1))
					{
						--m_iTextureCursor;
					}
				}
				ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.y);
				if (ImGui::ArrowButton("##DownFromMaterial", ImGuiDir_Down))
				{
					if (m_tMaterial.pTexture[IDX(eAiTexType)]->Swap_ShaderResourceView(m_iTextureCursor, m_iTextureCursor + 1))
					{
						++m_iTextureCursor;
					}
				}

				if (ImGui::BeginListBox("Texture Listbox", ImVec2(-FLT_MIN, 0.f)))
				{
					if (m_tMaterial.pTexture[IDX(eAiTexType)])
					{
						for (_uint i = 0; i < m_tMaterial.pTexture[IDX(eAiTexType)]->Get_NumTextures(); ++i)
						{
							wstring wstrTexturePath = m_tMaterial.pTexture[IDX(eAiTexType)]->Get_TexturePath(i);
							wstring wstrFileName, wstrExtension;
							Function::SplitPath(wstrTexturePath, nullptr, nullptr, &wstrFileName, &wstrExtension);

							ImGui::PushID(i);
							if (ImGui::Selectable(Function::ConvertString(wstrFileName + wstrExtension).c_str(), i == m_iTextureCursor))
							{
								m_iTextureCursor = i;
							}
							ImGui::PopID();
						}
						ImGui::EndListBox();
					}
				}

				if (m_iTextureCursor != -1)
				{
					ImVec2 imgSize;

					D3D11_TEXTURE2D_DESC tTexture2dDesc{};
					m_tMaterial.pTexture[IDX(eAiTexType)]->Get_Texture2D(m_iTextureCursor)->GetDesc(&tTexture2dDesc);

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

					ImGui::Image(m_tMaterial.pTexture[IDX(eAiTexType)]->Get_ShaderResourceView(m_iTextureCursor).Get(), imgSize);
					if (ImGui::BeginItemTooltip())
					{
						ImGui::Text("%.0fx%.0f", fWidth, fHeight);
						ImGui::EndTooltip();
					}
				}

				ImGui::TreePop();
			}
		}
		break;
#pragma endregion
		default:
			break;
		}
	}
#pragma region File Dialog

	if (ImGuiFileDialog::Instance()->Display(DIALOG_LOAD_TEXTURE))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_tMaterial.pTexture[IDX(eAiTexType)]->Push_ShaderResourceView(Function::ConvertString(ImGuiFileDialog::Instance()->GetFilePathName()), true)))
			{
				MSG_BOX("CTool_Model::Tool_Properties", "Failed to DIALOG_LOAD_TEXTURE");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

	if (ImGuiFileDialog::Instance()->Display(DIALOG_EXPORT_MESH))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			if (FAILED(m_pMesh->Export(Function::ConvertString(ImGuiFileDialog::Instance()->GetFilePathName()))))
			{
				MSG_BOX("CTool_Model::Tool_Properties", "Failed to DIALOG_EXPORT_MESH");
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}
	
	if (ImGuiFileDialog::Instance()->Display(DIALOG_LOAD_SOUNDFILE))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_strSoundFileName = ImGuiFileDialog::Instance()->GetFilePathName();
		}
		ImGuiFileDialog::Instance()->Close();
	}

#pragma endregion
}

void CTool_Model::Tool_MainControl()
{
	if (m_pAnimModel)
	{
		if (m_iAnimationCursor != -1)
		{
			if (ImGui::SliderInt("Time", &m_iTrackPosition, 0, (_int)m_pAnimModel->Get_Duration()))
			{
				m_pAnimModel->Set_TrackPosition((_float)m_iTrackPosition);
				m_pAnimModel->Tick_Animation(0.f);
			}
			if (ImGui::BeginCombo("EventType", std::to_string(m_iEventType).c_str()))
			{

				for (_uint i = 0; i < EVENTTYPE::TYPE_END; ++i)
				{
					string Name = "";
					if (i == 0)
						Name = "Sound";
					else if (i == 1)
						Name = "AnimSpeed";
					else 
						Name = "CustomEvent" + std::to_string(i-1);
					if (ImGui::Selectable(Name.c_str(), i == m_iEventType))
					{
						m_iEventType = i;
					}
				}
				ImGui::EndCombo();
			}
			switch (m_iEventType)
			{
			case 0:
			{
				if (ImGui::Button("SoundFile", ImVec2(100.f, 20.f)))
				{
					const _char* szFilters = "All files{.*},WIC files(*.mp3 *.wav *.ogg){.mp3,.wav,.ogg},mp3 files(*.mp3){.mp3}";
					ImGuiFileDialog::Instance()->OpenDialog(DIALOG_LOAD_SOUNDFILE, "Open Texture", szFilters, "Bin/Resources/", 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
				ImGui::Text(m_strSoundFileName.c_str());
				ImGui::InputFloat("SoundVolume", &m_fSoundVolume);
			}
			break;
			case 1:
			{
				ImGui::InputFloat("AnimSpeed", &m_fAnimSpeed);
			}
			break;
			default:
				break;
			}
			if (ImGui::Button("Create", ImVec2(100.f, 20.f)))
			{
				switch (m_iEventType)
				{
				case 0:
				{
					CEvent::SOUNDDATA SoundData;
					_int Test = m_strSoundFileName.find_last_of("\\");
					m_strSoundFileName = m_strSoundFileName.erase(0, Test + 1);
					SoundData.SoundName.assign(m_strSoundFileName.begin(), m_strSoundFileName.end());
					SoundData.volume = m_fSoundVolume;
					shared_ptr<CEvent> pEvent = CEvent::Create(EVENTTYPE(m_iEventType), m_pAnimModel->Get_Animation(m_iAnimationCursor), SoundData);
					m_pAnimModel->Get_Animation(m_iAnimationCursor)->AddEvent(m_iTrackPosition, pEvent);
				}
				break;
				case 1:
				{
					shared_ptr<CEvent> pEvent = CEvent::Create(EVENTTYPE(m_iEventType), m_pAnimModel->Get_Animation(m_iAnimationCursor), m_fAnimSpeed);
					m_pAnimModel->Get_Animation(m_iAnimationCursor)->AddEvent(m_iTrackPosition, pEvent);
				}
				break;
				default:
				{
					shared_ptr<CEvent> pEvent = CEvent::Create(EVENTTYPE(m_iEventType), m_pAnimModel->Get_Animation(m_iAnimationCursor));
					m_pAnimModel->Get_Animation(m_iAnimationCursor)->AddEvent(m_iTrackPosition, pEvent);
				}
				break;
				}
				
			}
		}
	}
}

void CTool_Model::Tool_SubControl()
{
	if (m_pNonAnimModel)
	{
		vector<_float3> vec;
		vec.resize(m_pNonAnimModel->Get_Mesh(0u)->Get_NumVertices());
		m_pNonAnimModel->Get_Mesh(0u)->Copy_Vertices(vec.data());
		_float a = 0, b = 0;
		for (_uint i = 0; i < vec.size(); ++i)
		{
			if (vec[i].y > a)
				a = vec[i].y;
			if (vec[i].y < b)
				b = vec[i].y;
		}
		a = 0;
		b = 0;
	}

	if (ImGui::TreeNode("Matrix##CTool_Model::Tool_SubControl"))
	{
		static _float4x4 m = g_mUnit;
			
		if (ImGui::InputFloat4("Right##CTool_Model::Tool_SubControl", &m._11))
		{
			m_pDummyTransform->Set_Matrix(m);
		}
		if (ImGui::InputFloat4("Up##CTool_Model::Tool_SubControl", &m._21))
		{
			m_pDummyTransform->Set_Matrix(m);
		}
		if (ImGui::InputFloat4("Look##CTool_Model::Tool_SubControl", &m._31))
		{
			m_pDummyTransform->Set_Matrix(m);
		}
		if (ImGui::InputFloat4("Translation##CTool_Model::Tool_SubControl", &m._41))
		{
			m_pDummyTransform->Set_Matrix(m);
		}

		ImGui::TreePop();
	}
	if (m_iAnimationCursor != -1)
	{
		auto& MapEvents = m_pAnimation->Get_Events();
		if (MapEvents.empty())
			return;
		ImGui::SetNextItemWidth(120.f);
		if (ImGui::BeginCombo("Events", "Frames"))
		{
			for (auto iter = MapEvents.begin(); iter != MapEvents.end(); ++iter)
			{
				for (_uint i = 0; i < iter->second.size(); ++i)
				{
					m_iTrackPosition = iter->first;
					m_pAnimModel->Get_Animation(m_iAnimationCursor)->Set_TrackPosition(m_iTrackPosition);
					if (ImGui::Selectable((std::to_string(iter->first) + "Frame" + std::to_string(i)).c_str(), i == m_iEventCursor))
					{
						m_iEventCursor = i;
						m_pEvent = iter->second[i];
						
						m_iEventType = m_pEvent->Get_Type();
						switch (m_iEventType)
						{
						case 0:
						{
							CEvent::SOUNDDATA SoundData = m_pEvent->Get_SoundData();
							m_strSoundFileName.assign(SoundData.SoundName.begin(), SoundData.SoundName.end());
							m_fSoundVolume = SoundData.volume;
						}
						break;
						case 1:
						{
							m_fAnimSpeed = m_pEvent->Get_AnimSpeed();
						}
						break;
						default :
							break;
						}

					}
				}

			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			auto iter = MapEvents.find(m_iTrackPosition);
			if (iter == MapEvents.end())
				return;
			for (auto EventIter = iter->second.begin();EventIter != iter->second.end();)
			{
				if (*EventIter == m_pEvent)
				{
					EventIter = iter->second.erase(EventIter);
					m_pAnimation->MinusEventCount();
				}
				else
					++EventIter;
			}
		}
	}
}

HRESULT CTool_Model::Push_Model(const wstring& _wstrFilePath, const MODEL _eModel, _matrixf _mPivot)
{
	if (!Function::InRange(_eModel, static_cast<MODEL>(0), MODEL::MAX))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Push_Model", "Invalid MODEL");
	}

	shared_ptr<CModel> pModel = CModel::Create(m_pDevice, m_pContext, _eModel, _wstrFilePath, _mPivot);
	if (nullptr == pModel)
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Push_Model", "Failed to Create");
	}

	switch (_eModel)
	{
	case MODEL::ANIM:
		m_vecAnimModels.emplace_back(pModel);
		m_vecAnimModelTags.resize(m_vecAnimModels.size());
		break;

	case MODEL::NONANIM:
		m_vecNonAnimModels.emplace_back(pModel);
		m_vecNonAnimModelTags.resize(m_vecNonAnimModels.size());
		break;

	default:
		MSG_RETURN(E_FAIL, "CScene_Tool::Load_Model", "Invalid MODEL");
		break;
	}

	return S_OK;
}

HRESULT CTool_Model::Export_Model(const wstring& _wstrFilePath)
{
	if (nullptr == m_pModel)
	{
		return S_FALSE;
	}

	if (FAILED(m_pModel->Export(_wstrFilePath)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Export_Model", "Failed to CModel::Export");
	}

	return S_OK;
}

HRESULT CTool_Model::Export_Animation(const wstring& _wstrFilePath)
{
	if (nullptr == m_pModel)
	{
		return S_FALSE;
	}

	if (FAILED(m_pModel->Export_Animation(_wstrFilePath)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Export_Animation", "Failed to CModel::Export");
	}

	return S_OK;
}

HRESULT CTool_Model::Bind_DummyMaterials()
{
	MATERIALDESC tMtrlDesc;

	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, tMtrlDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLAMB, tMtrlDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLSPC, tMtrlDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLEMS, tMtrlDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Bind_DummyMaterials", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_MTRLSHN, tMtrlDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CTool_Model::Bind_DummyMaterials", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	return S_OK;
}

shared_ptr<CTool_Model> CTool_Model::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTool_Model> pInstance = make_private_shared(CTool_Model, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTool_Model::Create", "Failed to Initialize");
	}

	return pInstance;
}
#endif
