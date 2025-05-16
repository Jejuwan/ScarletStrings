#include "ClientPCH.h"
#include "EffectRegular_MeshAnim.h"
#include "GameInstance.h"
#include "CurveContainer.h"
#include "Curves.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

CEffectRegular_MeshAnim::CEffectRegular_MeshAnim(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectRegular(_pDevice, _pContext, EFFECT::VFX_MeshAnim)
{
}

CEffectRegular_MeshAnim::CEffectRegular_MeshAnim(const CEffectRegular_MeshAnim& _rhs)
	: CEffectRegular(_rhs)
	, m_tData(_rhs.m_tData)
{
	if (_rhs.m_pCCScale)		m_pCCScale = _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCTranslation)	m_pCCTranslation = _rhs.m_pCCTranslation->Clone();
	if (_rhs.m_pCCBloom)			m_pCCBloom = _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)			m_pCCFade = _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCDiffuse)		m_pCCDiffuse = _rhs.m_pCCDiffuse->Clone();
	if (_rhs.m_pCCEmissive)		m_pCCEmissive = _rhs.m_pCCEmissive->Clone();
	if (_rhs.m_pCCSprite)		m_pCCSprite = _rhs.m_pCCSprite->Clone();
}

HRESULT CEffectRegular_MeshAnim::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHVFX, g_aNull);

	m_pCCScale			= CCurveContainer::Create();
	m_pCCTranslation	= CCurveContainer::Create();
	m_pCCBloom			= CCurveContainer::Create();
	m_pCCFade			= CCurveContainer::Create();
	m_pCCDiffuse		= CCurveContainer::Create();
	m_pCCEmissive		= CCurveContainer::Create();
	m_pCCSprite			= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Initialize", "Failed to CEffectRegular::Initialize");
	}

	return S_OK;
}

void CEffectRegular_MeshAnim::Tick(_float _fTimeDelta)
{
	m_fTimeAcc += _fTimeDelta;

	m_fFadeRatio = m_pCCFade->Get_Value(m_fTimeAcc / m_tData.fLifeTime).x;
	m_fBloomRatio = m_pCCBloom->Get_Value(m_fTimeAcc / m_tData.fLifeTime).x;
	
	m_pTransform->Set_Scale(_float3(m_pCCScale->Get_Value(m_fTimeAcc / m_tData.fLifeTime)) * m_vBaseScale);
	m_pTransform->Set_State(TRANSFORM::POSITION, m_vFetchPosition + m_pCCTranslation->Get_Value(m_fTimeAcc / m_tData.fLifeTime));

	m_tMaterialDesc.vDiffuse = m_pCCDiffuse->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
	m_tMaterialDesc.vEmissive = m_pCCEmissive->Get_Value(m_fTimeAcc / m_tData.fLifeTime);

//	m_iMeshIndex = std::clamp(static_cast<_int>(floorf(m_pCCSprite->Get_Value(m_fTimeAcc).x)), 0, static_cast<_int>(m_tData.vecMesh.size() - 1));
	if (m_iMeshIndex != std::clamp(static_cast<_int>(floorf(m_pCCSprite->Get_Value(m_fTimeAcc).x)), 0, static_cast<_int>(m_tData.vecMesh.size() - 1)))
	{
		Delete_Component(COMPONENT::MESH);
		Add_Component(COMPONENT::MESH, m_tData.vecMesh[m_iMeshIndex]);
		m_iMeshIndex = std::clamp(static_cast<_int>(floorf(m_pCCSprite->Get_Value(m_fTimeAcc).x)), 0, static_cast<_int>(m_tData.vecMesh.size() - 1) );
	}

	__super::Tick(_fTimeDelta);

	m_bRelease = m_fTimeAcc > m_tData.fLifeTime;

#if ACTIVATE_TOOL
//	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
//	{
//		if (m_bToolRepeat && m_bRelease)
//		{
//			Fetch(m_mFetch);
//		}
//	}
#endif
}

void CEffectRegular_MeshAnim::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_fBloomRatio)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectRegular_MeshAnim::Render()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_fFadeRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float2 vUVOffset = m_tData.vUVOffsetSpeed * m_fTimeAcc;
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET, &vUVOffset, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float fDifTiling[8], fEmsTiling[8];
	std::fill(fDifTiling, fDifTiling + 8, m_tData.fDiffuseTiling);
	std::fill(fEmsTiling, fEmsTiling + 8, m_tData.fEmissiveTiling);
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fDifTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_FloatArray");
	}
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_EMISSIVE, fEmsTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to Bind_FloatArray");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Render_Bloom()
{
//	MATERIALDESC	tOriginalMaterial	= m_tMaterialDesc;
//	_float			fOriginalBloom		= m_fBloomRatio;
//	std::swap(m_tMaterialDesc.vDiffuse.w, m_fBloomRatio);

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_fBloomRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_fFadeRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float2 vUVOffset = m_tData.vUVOffsetSpeed * m_fTimeAcc;
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET, &vUVOffset, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float fDifTiling[8], fEmsTiling[8];
	std::fill(fDifTiling, fDifTiling + 8, m_tData.fDiffuseTiling);
	std::fill(fEmsTiling, fEmsTiling + 8, m_tData.fEmissiveTiling);
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fDifTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_FloatArray");
	}
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_EMISSIVE, fEmsTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to Bind_FloatArray");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Render_Bloom", "Failed to CEffectRegular::Render");
	}

//	m_tMaterialDesc	= tOriginalMaterial;
//	m_fBloomRatio	= fOriginalBloom;

	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Fetch", "Failed to CEffectRegular::Fetch");
	}

	m_fFadeRatio		= 0.f;
	m_vFetchPosition	= m_pTransform->Get_State(TRANSFORM::POSITION);

	if (_arg.has_value() && _arg.type() == typeid(_float4x4))
	{
		_float4x4	m = any_cast<_float4x4>(_arg);
		m_vBaseScale = _float3(m.row(0).length(), m.row(1).length(), m.row(2).length());
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_mFetch = any_cast<_float4x4>(_arg);
	}
#endif

	return S_OK;
}

_bool CEffectRegular_MeshAnim::Return()
{
	return m_bRelease;
}

HRESULT CEffectRegular_MeshAnim::Release()
{
	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Initialize(std::ifstream& _inFile)
{
	size_t nSize(0);
	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));

	for (size_t i = 0; i < nSize; ++i)
	{
		m_tData.vecMesh.emplace_back(CMesh::Read(m_pDevice, m_pContext, MODEL::NONANIM, _inFile, XMMatrixRotationY(XM_PI)));
	}

	if (FAILED(Add_Component(COMPONENT::MESH, m_tData.vecMesh[0])))
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Initialize", "Failed to Add_Component");
	}

	_bool bTexture(false);

	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pDiffuse = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pDiffuse)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffsetSpeed),		sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fDiffuseTiling),		sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fEmissiveTiling),	sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	m_pCCScale			= CCurveContainer::Read(_inFile);
	m_pCCTranslation	= CCurveContainer::Read(_inFile);
	m_pCCBloom			= CCurveContainer::Read(_inFile);
	m_pCCFade			= CCurveContainer::Read(_inFile);
	m_pCCDiffuse		= CCurveContainer::Read(_inFile);
	m_pCCEmissive		= CCurveContainer::Read(_inFile);
	m_pCCSprite			= CCurveContainer::Read(_inFile);

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Ready_Components", "Failed to CEffect::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Ready_Components", "Nullptr Exception: m_pShader");
	}
	
	return S_OK;
}

HRESULT CEffectRegular_MeshAnim::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (FAILED(__super::Add_Component(_eComponent, COMPONENT::MESH == _eComponent && !_pComponent ? m_tData.vecMesh[0] : _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Ready_Components", "Failed to CGameObject::Add_Component");
	}
	
	return S_OK;
}

shared_ptr<CEffectRegular_MeshAnim> CEffectRegular_MeshAnim::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectRegular_MeshAnim> pInstance = make_private_shared(CEffectRegular_MeshAnim, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_MeshAnim::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectRegular_MeshAnim::Clone(any)
{
	shared_ptr<CEffectRegular_MeshAnim> pInstance = make_private_shared_copy(CEffectRegular_MeshAnim, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_MeshAnim::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectRegular_MeshAnim> CEffectRegular_MeshAnim::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectRegular_MeshAnim> pInstance = make_private_shared(CEffectRegular_MeshAnim, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_MeshAnim::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_MeshAnim::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectRegular_MeshAnim::Read", "Failed to Initialize");
	}

	return pInstance;
}

#ifdef _DEBUG
HRESULT CEffectRegular_MeshAnim::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	size_t nSize = m_tData.vecMesh.size();
	outFile.write(reinterpret_cast<const _byte*>(&nSize),						sizeof(size_t));
	for (auto& pMesh : m_tData.vecMesh)
	{
		pMesh->Export(outFile, MODEL::NONANIM);
	}

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffsetSpeed),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fDiffuseTiling),		sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fEmissiveTiling),		sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	m_pCCScale->Export(outFile);
	m_pCCTranslation->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCDiffuse->Export(outFile);
	m_pCCEmissive->Export(outFile);
	m_pCCSprite->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_MeshAnim::Export", "Failed to Write File: CEffectRegular_MeshAnim");
	}

	outFile.close();

	return S_OK;
}
#endif
