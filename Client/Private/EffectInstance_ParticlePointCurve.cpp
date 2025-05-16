#include "ClientPCH.h"
#include "EffectInstance_ParticlePointCurve.h"
#include "GameInstance.h"
#include "ComputeAccelerator.h"

CEffectInstance_ParticlePointCurve::CEffectInstance_ParticlePointCurve(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticlePointCurve)
{
}

CEffectInstance_ParticlePointCurve::CEffectInstance_ParticlePointCurve(const CEffectInstance_ParticlePointCurve& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
{
	if (_rhs.m_pCCInstLifeTime)		m_pCCInstLifeTime = _rhs.m_pCCInstLifeTime->Clone();
	if (_rhs.m_pCCGravityScale)		m_pCCGravityScale = _rhs.m_pCCGravityScale->Clone();
	if (_rhs.m_pCCStart)			m_pCCStart = _rhs.m_pCCStart->Clone();
	if (_rhs.m_pCCSpeed)			m_pCCSpeed = _rhs.m_pCCSpeed->Clone();
	if (_rhs.m_pCCScale)			m_pCCScale = _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCBloom)				m_pCCBloom = _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)				m_pCCFade = _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCDiffuse)			m_pCCDiffuse = _rhs.m_pCCDiffuse->Clone();
	if (_rhs.m_pCCSprite)			m_pCCSprite = _rhs.m_pCCSprite->Clone();
}

HRESULT CEffectInstance_ParticlePointCurve::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);
	
	m_pCCInstLifeTime		= CCurveContainer::Create();
	m_pCCGravityScale		= CCurveContainer::Create();
	m_pCCSpeed				= CCurveContainer::Create();
	m_pCCStart				= CCurveContainer::Create();
	m_pCCScale				= CCurveContainer::Create();
	m_pCCBloom				= CCurveContainer::Create();
	m_pCCFade				= CCurveContainer::Create();
	m_pCCDiffuse			= CCurveContainer::Create();
	m_pCCSprite				= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointCurve::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticlePointCurve::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_bRelease = m_fTimeAcc > m_pCCInstLifeTime->Get_Total() * m_tData.fInstLifeTimeScale + m_pCCStart->Get_Total() * m_tData.fInstStartTimeScale;

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (m_bToolRepeat && m_bRelease)
		{
			Fetch(_float3(0.f, 0.f, 0.f));
		}
	}
#endif
}

void CEffectInstance_ParticlePointCurve::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	Add_RenderObject(RENDER_GROUP::BLOOM);
}

HRESULT CEffectInstance_ParticlePointCurve::Render()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointCurve::Render_Bloom()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointCurve::Fetch(any _arg)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Fetch", "Failed to CEffectInstance::Fetch");
	}

	if (!_arg.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Fetch", "Invalid Argument");
	}
	else
	{
		if (_arg.type() == typeid(_float3))
		{
			_float3 vPosition = any_cast<_float3>(_arg);
			m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
		}
		if (_arg.type() == typeid(_float4x4))
		{
			m_pTransform->Set_Matrix(any_cast<_float4x4>(_arg));
		}
		if (_arg.type() == typeid(shared_ptr<CTransform>))
		{
			m_pTransform = any_cast<shared_ptr<CTransform>>(_arg);
		}
	}

	return S_OK;
}

_bool CEffectInstance_ParticlePointCurve::Return()
{
	return m_bRelease;
}                                                                                                                                                                                                                                                                                             

HRESULT CEffectInstance_ParticlePointCurve::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Release", "Failed to CGameObject::Release");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointCurve::Initialize(std::ifstream& _inFile)
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointCurve::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference),		sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstLifeTimeScale),		sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstStartTimeScale),	sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fVelocityTail),			sizeof(_float));
	
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpriteSheetSize),		sizeof(_int2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSize),					sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),				sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),			sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),			sizeof(_uint));

	m_pCCInstLifeTime		= CCurveContainer::Read(_inFile);
	m_pCCStart				= CCurveContainer::Read(_inFile);
	m_pCCSpeed				= CCurveContainer::Read(_inFile);
	m_pCCGravityScale		= CCurveContainer::Read(_inFile);
	m_pCCScale				= CCurveContainer::Read(_inFile);
	m_pCCBloom				= CCurveContainer::Read(_inFile);
	m_pCCFade				= CCurveContainer::Read(_inFile);
	m_pCCDiffuse			= CCurveContainer::Read(_inFile);
	m_pCCSprite				= CCurveContainer::Read(_inFile);

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticlePointCurve::Set_Data(const VFX_ParticlePointCurve _tData)
{
	m_tData = _tData;
	m_iActivateInstances = m_tData.iMaxInstance;
}
#endif

void CEffectInstance_ParticlePointCurve::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	m_vecInstTimeAcc.resize(m_tData.iMaxInstance);
	m_vecInstLifeTime.resize(m_tData.iMaxInstance);
	m_vecInstInitialLook.resize(m_tData.iMaxInstance);
	m_vecInstVelocity.resize(m_tData.iMaxInstance);
	m_vecInstTransform.resize(m_tData.iMaxInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

//	if (!m_tData.fVelocityTail)
//	{
//		for (_int i = 0; i < m_tData.iMaxInstance; ++i)
//		{
//			m_vecInstTransform[i] = CTransform::Create();
//			m_vecInstTransform[i]->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION));
//			_float fMaxValue = 0.f;
//			fMaxValue = Function::MonteCarloMaxValue(m_pCCStart, 100);
//			m_vecInstTimeAcc[i] = Function::MonteCarloRandom(m_pCCStart, fMaxValue);
//			fMaxValue = Function::MonteCarloMaxValue(m_pCCInstLifeTime, 100);
//			m_vecInstLifeTime[i] = Function::MonteCarloRandom(m_pCCInstLifeTime, fMaxValue);
//			m_vecInstInitialLook[i] = Function::Cone_DiffusionAngle(m_tData.vForceReference, m_tData.fTheta, m_tData.fPhi);
//			m_vecInstTransform[i]->LookTo(m_vecInstInitialLook[i], false, false);
//		}
//	}
//	else
	{
		for (_int i = 0; i < m_tData.iMaxInstance; ++i)
		{
		//	_float fMaxValue = 0.f;
		//	fMaxValue = Function::MonteCarloMaxValue(m_pCCStart, 100);
		//	m_vecInstTimeAcc[i] = -Function::MonteCarloRandom(m_pCCStart, fMaxValue) * m_tData.fInstStartTimeScale;
		//	fMaxValue = Function::MonteCarloMaxValue(m_pCCInstLifeTime, 100);
		//	m_vecInstLifeTime[i] = Function::MonteCarloRandom(m_pCCInstLifeTime, fMaxValue) * m_tData.fInstLifeTimeScale;
			m_vecInstTimeAcc[i] = -m_pCCStart->Get_Value(Function::RandomFloat(0.f, m_pCCStart->Get_Total())).x;
			m_vecInstLifeTime[i] = m_pCCInstLifeTime->Get_Value(Function::RandomFloat(0.f, m_pCCInstLifeTime->Get_Total())).x;
			m_vecInstVelocity[i] = Function::Cone_DiffusionAngle(m_tData.vForceReference, m_tData.fTheta, m_tData.fPhi);

			pData[i].vRight = _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);
			pData[i].vUp = _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);

			pData[i].vColor = float4(1.f, 1.f, 1.f, 1.f);
		}
	}
}

void CEffectInstance_ParticlePointCurve::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

//	if (!m_tData.fVelocityTail)
//	{
//		for (_int i = 0; i < m_iActivateInstances; ++i)
//		{
//			m_vecInstTimeAcc[i] += _fTimeDelta;
//			if (m_vecInstTimeAcc[i] <= 0.f)
//			{
//				continue;
//			}
//			m_vecInstTransform[i]->Set_Scale(m_pCCScale->Get_Value(m_vecInstTimeAcc[i]));
//			_float3 vPosition = m_vecInstTransform[i]->Get_State(TRANSFORM::POSITION);
//			m_vecInstTransform[i]->Translate(m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i]) * m_vecInstTransform[i]->Get_State(TRANSFORM::LOOK).normalize());
//			m_vecInstTransform[i]->Translate(_float3(0.f, m_pCCGravityScale->Get_Value(m_vecInstTimeAcc[i]).x, 0.f));
//			m_vecInstTransform[i]->LookTo(m_vecInstTransform[i]->Get_State(TRANSFORM::POSITION) - vPosition, false, false);
//			
//			pData[i].vRight			= _float4(m_vecInstTransform[i]->Get_State(TRANSFORM::RIGHT), 0.f);
//			pData[i].vUp			= _float4(m_vecInstTransform[i]->Get_State(TRANSFORM::UP), 0.f);
//			pData[i].vLook			= _float4(m_vecInstTransform[i]->Get_State(TRANSFORM::LOOK), 0.f);
//			pData[i].vTranslation	= _float4(m_vecInstTransform[i]->Get_State(TRANSFORM::POSITION), 1.f);
//
//			pData[i].vArgument.x	= m_pCCFade->Get_Value(m_vecInstTimeAcc[i]).x;
//			pData[i].vArgument.y	= m_pCCBloom->Get_Value(m_vecInstTimeAcc[i]).x;
//			pData[i].vArgument.z	= floorf(m_pCCSprite->Get_Value(m_vecInstTimeAcc[i]).x * static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y));
//			pData[i].vArgument.w	= m_vecInstTimeAcc[i];
//		}
//	}
//	else
	{
		for (_int i = 0; i < m_tData.iMaxInstance; ++i)
		{
			m_vecInstTimeAcc[i] += _fTimeDelta;
			if (m_vecInstTimeAcc[i] <= 0.f)
			{
				continue;
			}
			else if (m_vecInstTimeAcc[i] > m_vecInstLifeTime[i])
			{
				pData[i].vColor.w = 0.f;
				continue;
			}

			pData[i].vRight = pData[i].vUp;
			pData[i].vUp = _float4(pData[i].vUp) + _float4(m_vecInstVelocity[i], 0.f).normalize() * m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i]).x * _fTimeDelta;
			pData[i].vUp = _float4(pData[i].vUp) - _float4(0.f, g_fGravity, 0.f, 0.f) * m_pCCGravityScale->Get_Value(m_vecInstTimeAcc[i]).x * _fTimeDelta;
			m_vecInstVelocity[i] = _float4(pData[i].vUp) - _float4(pData[i].vRight);

			pData[i].vColor = _float4(m_pCCDiffuse->Get_Value(m_vecInstTimeAcc[i]));

			pData[i].vArgument.x = m_pCCFade->Get_Value(m_vecInstTimeAcc[i]).x;
			pData[i].vArgument.y = m_pCCBloom->Get_Value(m_vecInstTimeAcc[i]).x;
			pData[i].vArgument.z = floorf(m_pCCSprite->Get_Value(m_vecInstTimeAcc[i]).x * static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y));
			pData[i].vArgument.z = std::clamp(pData[i].vArgument.z, 0.f, static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y - 1.f));
			pData[i].vArgument.w = m_tData.fVelocityTail;
		}
	}
}
                                                                                                                                                                             
shared_ptr<CEffectInstance_ParticlePointCurve> CEffectInstance_ParticlePointCurve::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticlePointCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointCurve::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticlePointCurve::Clone(any)
{
	shared_ptr<CEffectInstance_ParticlePointCurve> pInstance = make_private_shared_copy(CEffectInstance_ParticlePointCurve, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointCurve::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticlePointCurve> CEffectInstance_ParticlePointCurve::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticlePointCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointCurve::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointCurve::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointCurve::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticlePointCurve::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePoint::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePoint::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if(m_tData.pEmission)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pEmission->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePoint::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTheta),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fPhi),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vForceReference),		sizeof(_float3));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInstLifeTimeScale),	sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInstStartTimeScale),	sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fVelocityTail),		sizeof(_float));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSize),				sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances),		sizeof(_uint));

	m_pCCInstLifeTime->Export(outFile);
	m_pCCStart->Export(outFile);
	m_pCCSpeed->Export(outFile);
	m_pCCGravityScale->Export(outFile);
	m_pCCScale->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCDiffuse->Export(outFile);
	m_pCCSprite->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePoint::Export", "Failed to Write File: CEffectInstance_ParticlePoint");
	}

	outFile.close();

	return S_OK;
}
#endif
