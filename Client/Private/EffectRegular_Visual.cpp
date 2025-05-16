#include "ClientPCH.h"
#include "EffectRegular_Visual.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectRegular_Visual::CEffectRegular_Visual(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectRegular(_pDevice, _pContext, EFFECT::VFX_Visual)
{
}

CEffectRegular_Visual::CEffectRegular_Visual(const CEffectRegular_Visual& _rhs)
	: CEffectRegular(_rhs)
	, m_tData		(_rhs.m_tData)
{
}

HRESULT CEffectRegular_Visual::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CEffectRegular_Visual::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CEffectRegular::Initialize");
	}

	return S_OK;
}

void CEffectRegular_Visual::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	_float fInterval = m_tData.fLifeTime / (m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y);
	m_iSpriteIndex = static_cast<_int>(m_fTimeAcc / fInterval);

	if (m_fTimeAcc / m_tData.fLifeTime < m_tData.vFadeScope.x)
	{
		m_fFadeRatio = std::clamp(Function::ProportionalRatio(0.f, m_tData.vFadeScope.x, m_fTimeAcc / m_tData.fLifeTime), 0.f, 1.f);
	}
	else if (m_fTimeAcc / m_tData.fLifeTime < m_tData.vFadeScope.y)
	{
		m_fFadeRatio = 1.f;
	}
	else
	{
		m_fFadeRatio = std::clamp(Function::ProportionalRatio(1.f,  m_tData.vFadeScope.y, m_fTimeAcc / m_tData.fLifeTime), 0.f, 1.f);
	}

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

void CEffectRegular_Visual::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectRegular_Visual::Render()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, m_iSpriteIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_tData.bFade ? m_fFadeRatio : 1.f)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectRegular_Visual::Render_Bloom()
{
//	MATERIALDESC	tOriginalMaterial	= m_tMaterialDesc;
//	_float			fOriginalBloom		= m_tData.fBloom;
//	std::swap(m_tMaterialDesc.vDiffuse.w, m_tData.fBloom);

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, m_iSpriteIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_tData.bFade ? m_fFadeRatio : 1.f)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Render_Bloom", "Failed to CEffectRegular::Render");
	}

//	m_tMaterialDesc	= tOriginalMaterial;
//	m_tData.fBloom	= fOriginalBloom;

	return S_OK;
}

HRESULT CEffectRegular_Visual::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch(_vPosition3)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Fetch", "Failed to CEffectRegular::Fetch");
	}

	if (_vPosition3.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
	{
		pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_vPosition3);
		shared_ptr<CGameObject> pOwner = arg.first;
		shared_ptr<CTransform> pTransform = pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		shared_ptr<CModel> pModel = pOwner->Get_Component<CModel>(COMPONENT::MODEL);
		_float4x4 mBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformation();
		_float4x4 mPivot = pModel->Get_Pivot();
		m_pTransform->Set_State(TRANSFORM::POSITION, _float4x4(mBone * mPivot * pTransform->Get_Matrix()).row(3));
	}

	m_iShaderFlag = 0;
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_mFetch = any_cast<_float4x4>(_vPosition3);
	}
#endif

	return S_OK;
}

_bool CEffectRegular_Visual::Return()
{
	return m_bRelease;
}

HRESULT CEffectRegular_Visual::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Release", "Failed to CGameObject::Release");
	}

	return S_OK;
}

HRESULT CEffectRegular_Visual::Initialize(std::ifstream& _inFile)
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
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bFade),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vFadeScope),			sizeof(_float2));

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

HRESULT CEffectRegular_Visual::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CEffectRegular_Visual> CEffectRegular_Visual::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectRegular_Visual> pInstance = make_private_shared(CEffectRegular_Visual, _pDevice, _pContext);

	if(FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Visual::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectRegular_Visual::Clone(any)
{
	shared_ptr<CEffectRegular_Visual> pInstance = make_private_shared_copy(CEffectRegular_Visual, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Visual::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectRegular_Visual> CEffectRegular_Visual::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectRegular_Visual> pInstance = make_private_shared(CEffectRegular_Visual, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Visual::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Visual::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectRegular_Visual::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectRegular_Visual::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bFade),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vFadeScope),			sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Export", "Failed to Write File: CEffectRegular_Visual");
	}

	outFile.close();

	return S_OK;
}
#endif
