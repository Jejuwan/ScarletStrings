#include "ClientPCH.h"
#include "EffectInstance_ParticleRect.h"
#include "GameInstance.h"

CEffectInstance_ParticleRect::CEffectInstance_ParticleRect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticleRect)
{
}

CEffectInstance_ParticleRect::CEffectInstance_ParticleRect(const CEffectInstance_ParticleRect& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
{
}

HRESULT CEffectInstance_ParticleRect::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT, g_aNull);

	return S_OK;
}

HRESULT CEffectInstance_ParticleRect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticleRect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (!m_tData.bLoop)
	{
		m_bRelease = m_fTimeAcc > m_tData.fLifeTime;
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (m_bToolRepeat && m_bRelease)
		{
		//	Fetch(_float3(0.f, 0.f, 0.f));
		}
	}
#endif
}

void CEffectInstance_ParticleRect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_ParticleRect::Render()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleRect::Render_Bloom()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleRect::Fetch(any)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Fetch", "Failed to CEffectInstance::Fetch");
	}

	return S_OK;
}

_bool CEffectInstance_ParticleRect::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_ParticleRect::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Release", "Failed to CGameObject::Release");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleRect::Initialize(std::ifstream& _inFile)
{
	_bool bTexture(false);

	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pDiffuse = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pDiffuse)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pEmission = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pEmission)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bLoop),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bFade),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bPhysX),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bGravity),			sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstanceLifeTime),	sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fSpeed),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpeedScope),		sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fScale),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vScaleScope),		sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vHorizontalScope),	sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vVerticalScope),		sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vRotationScope),		sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference),	sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),		sizeof(_uint));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticleRect::Set_Data(const VFX_ParticleRect _tData)
{
	m_tData = _tData;
	m_iActivateInstances = m_tData.iMaxInstance;
}
#endif

void CEffectInstance_ParticleRect::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);
}

void CEffectInstance_ParticleRect::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{

	}
}

shared_ptr<CEffectInstance_ParticleRect> CEffectInstance_ParticleRect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticleRect> pInstance = make_private_shared(CEffectInstance_ParticleRect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleRect::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticleRect::Clone(any)
{
	shared_ptr<CEffectInstance_ParticleRect> pInstance = make_private_shared_copy(CEffectInstance_ParticleRect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleRect::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticleRect> CEffectInstance_ParticleRect::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticleRect> pInstance = make_private_shared(CEffectInstance_ParticleRect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleRect::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleRect::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleRect::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticleRect::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect), sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if (m_tData.pEmission)
	{

		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pEmission->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bLoop),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bFade),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bPhysX),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bGravity),			sizeof(_bool));
																				
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
																				
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInstanceLifeTime),	sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fSpeed),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpeedScope),			sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fScale),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vScaleScope),			sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vHorizontalScope),	sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vVerticalScope),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vRotationScope),		sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTheta),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fPhi),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vForceReference),		sizeof(_float3));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances),		sizeof(_uint));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleRect::Export", "Failed to Write File: CEffectInstance_ParticleRect");
	}

	outFile.close();

	return S_OK;
}
#endif
