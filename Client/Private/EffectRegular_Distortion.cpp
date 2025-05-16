#include "ClientPCH.h"
#include "EffectRegular_Distortion.h"
#include "GameInstance.h"

CEffectRegular_Distortion::CEffectRegular_Distortion(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CEffectRegular(_pDevice, _pContext, EFFECT::VFX_Distortion)
{
}

CEffectRegular_Distortion::CEffectRegular_Distortion(const CEffectRegular_Distortion& _rhs)
    : CEffectRegular(_rhs)
	, m_tData		(_rhs.m_tData)
{
}

HRESULT CEffectRegular_Distortion::Initialize_Prototype()
{
   	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CEffectRegular_Distortion::Initialize(any)
{
    if (FAILED(__super::Initialize()))
    {
        MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Initialize", "Failed to CEffectRegular::Initialize");
    }

    return S_OK;
}

void CEffectRegular_Distortion::Tick(_float _fTimeDelta)
{
    __super::Tick(_fTimeDelta);

	_float fInterval = m_tData.fLifeTime / (m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y);
	m_iSpriteIndex = static_cast<_int>(m_fTimeAcc / fInterval);

	m_bRelease = m_fTimeAcc > m_tData.fLifeTime;

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (m_bToolRepeat && m_bRelease)
		{
			Fetch(m_mFetch);
		}
	}
#endif
}

void CEffectRegular_Distortion::Late_Tick(_float _fTimeDelta)
{
    __super::Late_Tick(_fTimeDelta);

    Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectRegular_Distortion::Render()
{
	if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pTexture->Get_ShaderResourceView().GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, m_iSpriteIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_tData.fDistortionPower)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to Bind_Int: SHADER_DISTORTION_POWER");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

    if (FAILED(__super::Render(m_tData.iShaderPass)))
    {
        MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to CEffectRegular::Render");
    }

    return S_OK;
}

HRESULT CEffectRegular_Distortion::Render_Bloom()
{
//	MATERIALDESC	tOriginalMateral	= m_tMaterialDesc;
//	_float			fOriginalBloom		= m_tData.fBloom;
//	std::swap(m_tMaterialDesc.vDiffuse.w, m_tData.fBloom);

	if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pTexture->Get_ShaderResourceView().GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, m_iSpriteIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render_Bloom", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_tData.fDistortionPower)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render_Bloom", "Failed to Bind_Int: SHADER_DISTORTION_POWER");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render_Bloom", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Render", "Failed to CEffectRegular::Render");
	}

//	m_tMaterialDesc	= tOriginalMateral;
//	m_tData.fBloom	= fOriginalBloom;

	return S_OK;
}

HRESULT CEffectRegular_Distortion::Fetch(any _vPosition3)
{
    if (FAILED(__super::Fetch(_vPosition3)))
    {
        MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Fetch", "Failed to CEffectRegular::Fetch");
    }

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_mFetch = any_cast<_float4x4>(_vPosition3);
	}
#endif

    return S_OK;
}

_bool CEffectRegular_Distortion::Return()
{
    return m_bRelease;
}

HRESULT CEffectRegular_Distortion::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Release", "Failed to CGameObject::Release");
	}

	return S_OK;
}

HRESULT CEffectRegular_Distortion::Initialize(std::ifstream& _inFile)
{
	m_tData.pTexture = CTexture::Read(m_pDevice, m_pContext, _inFile);
	if (nullptr == m_tData.pTexture)
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fDistortionPower),	sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

shared_ptr<CEffectRegular_Distortion> CEffectRegular_Distortion::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CEffectRegular_Distortion> pInstance = make_private_shared(CEffectRegular_Distortion, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CEffectRegular_Distortion", "Failed to Initialize_Prototype");
    }

    return pInstance;
}

shared_ptr<CGameObject> CEffectRegular_Distortion::Clone(any)
{
    shared_ptr<CEffectRegular_Distortion> pInstance = make_private_shared_copy(CEffectRegular_Distortion, *this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_RETURN(nullptr, "CEffectRegular_Distortion::Clone", "Failed to Initialize");
    }

    return pInstance;
}

shared_ptr<CEffectRegular_Distortion> CEffectRegular_Distortion::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectRegular_Distortion> pInstance = make_private_shared(CEffectRegular_Distortion, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Distortion::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Distortion::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Distortion::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectRegular_Distortion::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	if (FAILED(m_tData.pTexture->Export(outFile)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Export", "Failed to CTexture::Export");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fDistortionPower),	sizeof(_float));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Distortion::Export", "Failed to Write File: CEffectRegular_Distortion");
	}

	outFile.close();

	return S_OK;
}
#endif
