#include "ClientPCH.h"
#include "Character.h"
#include "GameInstance.h"

CCharacter::CCharacter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCharacter::CCharacter(const CCharacter& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pModel->Set_Animation(0);

	m_mView = XMMatrixInverse(nullptr, XMMatrixIdentity());

	return S_OK;
}

void CCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCharacter::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CCharacter::Render()
{
	return S_OK;
}

HRESULT CCharacter::Render_OutLine()
{
	return S_OK;
}

HRESULT CCharacter::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CCharacter::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	auto pGameInstance = CGameInstance::Get_Instance();
	m_pNoiseLine = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_LINE_03));
	m_pTrinagleNoise = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_012));
	m_pDissolveNoise = dynamic_pointer_cast<CTexture>(pGameInstance->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_010));

	return S_OK;
}

void CCharacter::Execute_SAS(const std::function<void()>& fpFinished)
{
	m_bExecuteSAS = true;
	m_pModel->Set_Animation(0, 2.f, false, 0.f, false);

	m_vOutLineColor = _float3(1.0f, 0.86f, 0.7f);
	m_vAdditiveColor = _float3(1.0f, 0.f, 0.f);

	m_iDissolve = 0;
	m_bDissolve = false;

	m_fpFinished = fpFinished;
}

HRESULT CCharacter::Render(_uint iPassIndex)
{
	if (FAILED(__super::Render(iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CCharacter::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CCharacter::Render_SAS_Cutscene()
{
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_MTRLSHN, m_tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, m_mView)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_RawValue("g_vAdditiveColor", &m_vAdditiveColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pShader->Bind_ShaderResourceView("g_vLineNoiseTexture", m_pNoiseLine->Get_ShaderResourceView())))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Int("g_iDissolve", m_iDissolve)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Int");
	}

	if (m_bDissolve)
	{
		if (FAILED(m_pShader->Bind_ShaderResourceView("g_vTriangleNoiseTexture", m_pTrinagleNoise->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_ShaderResourceView");
		}
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXDISSOLVE, m_pDissolveNoise->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_ShaderResourceView");
		}
		
		if (FAILED(m_pShader->Bind_Float("g_fThreshold", (m_fDissolveAccTime / m_fDissolveTime))))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
		}
	}
	
	if (FAILED(m_pModel->Render(m_pShader, 7)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to CModel::Render");
	}

	return S_OK;
}

HRESULT CCharacter::Render_SAS_Cutscene_Outline()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTransform->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, m_mView)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutLineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(m_pShader->Bind_Int("g_iDissolve", m_iDissolve)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Int");
	}

	if (m_bDissolve)
	{
		if (FAILED(m_pShader->Bind_ShaderResourceView("g_vTriangleNoiseTexture", m_pTrinagleNoise->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_ShaderResourceView");
		}
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXDISSOLVE, m_pDissolveNoise->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_ShaderResourceView");
		}

		if (FAILED(m_pShader->Bind_Float("g_fThreshold", (m_fDissolveAccTime / m_fDissolveTime))))
		{
			MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
		}
	}

	if (FAILED(m_pModel->Render(m_pShader, 8)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to CModel::Render");
	}

	return S_OK;
}

void CCharacter::Tick_SAS(_uint iAnimIndex, _float fTimeDelta)
{
	if (m_bExecuteSAS)
	{
		m_pModel->Tick_Animation(fTimeDelta);

		if (m_pModel->Is_AnimationFinished(iAnimIndex))
		{
			m_bExecuteSAS = false;
			m_bDissolve = true;

			m_iDissolve = 1;
		}

		_float fScaledTime = fTimeDelta * 2.f;
		m_vOutLineColor -= _float3(fScaledTime, fScaledTime, fScaledTime);

		m_vAdditiveColor.x -= fScaledTime;
		if (m_vAdditiveColor.x < 0.f)
		{
			m_vAdditiveColor.x = 0.f;
		}
	}

	if (m_bDissolve)
	{
		m_fDissolveAccTime += fTimeDelta;
		if (m_fDissolveAccTime >= m_fDissolveTime)
		{
			m_fDissolveAccTime = 0.f;
			m_bDissolve = false;

			m_pModel->Reset_Animation(iAnimIndex);
			m_iDissolve = 0;

			if (m_fpFinished)
			{
				m_fpFinished();
				m_fpFinished = nullptr;
			}
		}
	}
}
