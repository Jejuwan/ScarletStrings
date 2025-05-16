#include "ClientPCH.h"
#include "KeyframeGenerator.h"

#include "GameInstance.h"

#define DIALOG_LOAD			"Dialog::Load"
#define DIALOG_EXPORT		"Dialog::Export"

CKeyframeGenerator::CKeyframeGenerator(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CKeyframeGenerator::CKeyframeGenerator(const CKeyframeGenerator& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CKeyframeGenerator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyframeGenerator::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CKeyframeGenerator::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CKeyframeGenerator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CKeyframeGenerator::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CKeyframeGenerator::Render()
{
	return S_OK;
}

HRESULT CKeyframeGenerator::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

_bool CKeyframeGenerator::Interpolate_Keyframe(_uint iKeyframeIndex, _float fKeyframeAccTime, _float4x4& mInterpolatedMatrix)
{
	if (m_vecKeyframes.size() <= iKeyframeIndex + 1)
		return false;

	const std::pair<_float4x4, _float>& tCurrentKeyframe = m_vecKeyframes[iKeyframeIndex];
	const std::pair<_float4x4, _float>& tNextKeyframe = m_vecKeyframes[iKeyframeIndex + 1];

	_vector vCurrentTranslation, vCurrentScale, vCurrentRotationQuat;
	XMMatrixDecompose(&vCurrentScale, &vCurrentRotationQuat, &vCurrentTranslation, XMLoadFloat4x4(&tCurrentKeyframe.first));

	_vector vNextTranslation, vNextScale, vNextRotationQuat;
	XMMatrixDecompose(&vNextScale, &vNextRotationQuat, &vNextTranslation, XMLoadFloat4x4(&tNextKeyframe.first));

	_float fInterpolatedTime = fKeyframeAccTime / tCurrentKeyframe.second;

	_vector vInterpolatedRotationQuat = XMQuaternionSlerp(vCurrentRotationQuat, vNextRotationQuat, fInterpolatedTime);
	_vector vInterpolatedTranslation = XMVectorLerp(vCurrentTranslation, vNextTranslation, fInterpolatedTime);
	_vector vInterpolatedScale = XMVectorLerp(vCurrentScale, vNextScale, fInterpolatedTime);

	_matrix matInterpolatedMatrix = XMMatrixScalingFromVector(vInterpolatedScale);
	matInterpolatedMatrix *= XMMatrixRotationQuaternion(vInterpolatedRotationQuat);
	matInterpolatedMatrix *= XMMatrixTranslationFromVector(vInterpolatedTranslation);

	/*
	_matrix mNextMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float fValue0 = tCurrentKeyframe.first.m[i][j];
			float fValue1 = tNextKeyframe.first.m[i][j];
			mNextMatrix.r[i].m128_f32[j] = fValue0 + (fValue1 - fValue0) * fInterpolatedTime;
		}
	}*/

	mInterpolatedMatrix = matInterpolatedMatrix;

	return true;
}

HRESULT CKeyframeGenerator::Import(const wstring& _wstrImportPath)
{
	std::ifstream inFile(_wstrImportPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CKeyframeGenerator::Import", "Failed to Open File");
	}

	_uint iNumKeyframe = 0;
	inFile.read(reinterpret_cast<char*>(&iNumKeyframe), sizeof(_uint));

	m_vecKeyframes.clear();
	m_vecKeyframes.reserve(iNumKeyframe);

	for (_uint i = 0; i < iNumKeyframe; ++i)
	{
		std::pair<_float4x4, _float> tKeyframe;
		inFile.read(reinterpret_cast<char*>(&tKeyframe), sizeof(std::pair<_float4x4, _float>));

		m_vecKeyframes.push_back(tKeyframe);
	}

	inFile.close();

	return S_OK;
}

HRESULT CKeyframeGenerator::Export(const wstring& _wstrExportPath)
{
	std::ofstream outFile(_wstrExportPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CCameraKeyframeGenerator::Export", "Failed to Open File");
	}

	_uint iNumKeyframe = (_uint)m_vecKeyframes.size();
	outFile.write(reinterpret_cast<const char*>(&iNumKeyframe), sizeof(_uint));

	for (_uint i = 0; i < m_vecKeyframes.size(); ++i)
	{
		const std::pair<_float4x4, _float> tKeyframe = m_vecKeyframes[i];

		outFile.write(reinterpret_cast<const char*>(&tKeyframe), sizeof(std::pair<_float4x4, _float>));
	}

	outFile.close();

	return S_OK;
}

#if ACTIVATE_IMGUI
void CKeyframeGenerator::Render_File_Dialog()
{
	ImGui::BeginGroup();
	ImVec2 vSize = ImGui::GetItemRectSize();

	if (ImGui::Button("Import", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, vSize.y)))
	{
		if (!m_imfdEmbed_Load.IsOpened())
		{
			if (m_imfdEmbed_Export.IsOpened())
			{
				m_imfdEmbed_Export.Close();
			}

			const _char* szFilters = "All files{.*}";
			m_imfdEmbed_Load.OpenDialog(DIALOG_LOAD, "Open", szFilters, "../Resources/", 1, nullptr,
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
	if (ImGui::Button("Export", ImVec2((vSize.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, vSize.y)))
	{
		if (!m_imfdEmbed_Export.IsOpened())
		{
			if (m_imfdEmbed_Load.IsOpened())
			{
				m_imfdEmbed_Load.Close();
			}

			const _char* szFilters = "All files{.*}";
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
	ImGui::EndGroup();

	if (m_imfdEmbed_Load.Display(DIALOG_LOAD, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Load.IsOk())
		{
			wstring wstrImportPath = Function::ConvertString(m_imfdEmbed_Load.GetFilePathName());
			Import(wstrImportPath);
		}
		m_imfdEmbed_Load.Close();
	}

	if (m_imfdEmbed_Export.Display(DIALOG_EXPORT, ImGuiWindowFlags_NoCollapse, ImVec2(0.f, 0.f), ImVec2(0.f, 240.f)))
	{
		if (m_imfdEmbed_Export.IsOk())
		{
			wstring wstrExportPath = Function::ConvertString(m_imfdEmbed_Export.GetFilePathName());
			Export(wstrExportPath);
		}
		m_imfdEmbed_Export.Close();
	}
}
#endif
