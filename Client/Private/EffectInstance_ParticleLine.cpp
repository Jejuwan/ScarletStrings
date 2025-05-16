#include "ClientPCH.h"
#include "EffectInstance_ParticleLine.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectInstance_ParticleLine::CEffectInstance_ParticleLine(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticleLine)
{
}

CEffectInstance_ParticleLine::CEffectInstance_ParticleLine(const CEffectInstance_ParticleLine& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
	, m_vReference(_rhs.m_vReference)
	, m_vNormal(_rhs.m_vNormal)
{
	if (_rhs.m_pCCInstLifeTime)		m_pCCInstLifeTime = _rhs.m_pCCInstLifeTime;
	if (_rhs.m_pCCStart)			m_pCCStart = _rhs.m_pCCStart;
	if (_rhs.m_pCCLength)			m_pCCLength = _rhs.m_pCCLength;
	if (_rhs.m_pCCDelay)			m_pCCDelay = _rhs.m_pCCDelay;
	if (_rhs.m_pCCBloom)			m_pCCBloom = _rhs.m_pCCBloom;
	if (_rhs.m_pCCFade)				m_pCCFade = _rhs.m_pCCFade;
	if (_rhs.m_pCCSprite)			m_pCCSprite = _rhs.m_pCCSprite;
}

HRESULT CEffectInstance_ParticleLine::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	m_pCCInstLifeTime = CCurveContainer::Create();
	m_pCCStart = CCurveContainer::Create();
	m_pCCLength = CCurveContainer::Create();
	m_pCCDelay = CCurveContainer::Create();
	m_pCCBloom = CCurveContainer::Create();
	m_pCCFade = CCurveContainer::Create();
	m_pCCSprite = CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectInstance_ParticleLine::Initialize(any _arg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticleLine::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_bRelease = m_fTimeAcc >= m_tData.fLifeTime + Function::MonteCarloMaxValue(m_pCCDelay, 100) + Function::MonteCarloMaxValue(m_pCCInstLifeTime, 100);

#if ACTIVATE_TOOL
#endif
}

void CEffectInstance_ParticleLine::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	Add_RenderObject(RENDER_GROUP::BLOOM);
}

HRESULT CEffectInstance_ParticleLine::Render()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue("g_vLineNormal", &m_vNormal, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: g_vLineNormal");
	}
	if (FAILED(m_pShader->Bind_Float("g_fLineThickness", m_tData.fThickness)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: g_fLineThickness");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleLine::Render_Bloom()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue("g_vLineNormal", &m_vNormal, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: g_vLineNormal");
	}
	if (FAILED(m_pShader->Bind_Float("g_fLineThickness", m_tData.fThickness)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: g_fLineThickness");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleLine::Fetch(any _arg)
{
	if (!_arg.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Fetch", "Invalid Argument");
	}
	else
	{
		if (_arg.type() == typeid(_float3))
		{
			_float3 vPosition = any_cast<_float3>(_arg);
			m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
		}
		else if (_arg.type() == typeid(_float4x4))
		{
			m_pTransform->Set_Matrix(any_cast<_float4x4>(_arg));
		}
		else if (_arg.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
		{
			pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_arg);
			shared_ptr<CTransform> pTransform = arg.first->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			shared_ptr<CModel> pModel = arg.first->Get_Component<CModel>(COMPONENT::MODEL);
			m_pTarget = pTransform;
			m_mPivot = pModel->Get_Pivot();
			m_pBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();
			m_pTransform->Set_Matrix(m_pTarget->Get_Matrix());
			m_pTransform->Set_State(TRANSFORM::POSITION, (*m_pBone * m_mPivot * m_pTarget->Get_Matrix()).row(3));
			m_vReference = XMVector3TransformNormal(m_tData.vForceReference, m_pTarget->Get_Matrix());
		}
		else if (_arg.type() == typeid(tuple<shared_ptr<CGameObject>, const _char*, _float>))
		{
			tuple<shared_ptr<CGameObject>, const _char*, _float> arg = any_cast<tuple<shared_ptr<CGameObject>, const _char*, _float>>(_arg);
			shared_ptr<CTransform> pTransform = std::get<0>(arg)->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			shared_ptr<CModel> pModel = std::get<0>(arg)->Get_Component<CModel>(COMPONENT::MODEL);
			m_pTarget = pTransform;
			m_mPivot = pModel->Get_Pivot();
			m_pBone = pModel->Get_Bone(std::get<1>(arg))->Get_CombinedTransformationPointer();
			m_pTransform->Set_Matrix(m_pTarget->Get_Matrix());
			m_pTransform->Set_State(TRANSFORM::POSITION, (*m_pBone * m_mPivot * m_pTarget->Get_Matrix()).row(3));
			m_vReference = XMVector3TransformNormal(m_tData.vForceReference, m_pTarget->Get_Matrix());
			m_vNormal = m_tData.vNormal.normalize() * std::get<2>(arg);
		}
	}

	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Fetch", "Failed to CEffectInstance::Fetch");
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
	}
#endif

	return S_OK;
}

_bool CEffectInstance_ParticleLine::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_ParticleLine::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Release", "Failed to CGameObject::Release");
	}

	m_vReference = m_tData.vForceReference;
	m_vNormal = m_tData.vNormal;

	m_pTarget = nullptr;
	m_mPivot = g_mUnit;
	m_pBone = nullptr;

	return S_OK;
}

HRESULT CEffectInstance_ParticleLine::Initialize(std::ifstream& _inFile)
{
	_bool bExist(false);

	_inFile.read(reinterpret_cast<_byte*>(&bExist), sizeof(_bool));
	if (bExist)
	{
		m_tData.pDiffuse = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pDiffuse)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
	}
	_inFile.read(reinterpret_cast<_byte*>(&bExist), sizeof(_bool));
	if (bExist)
	{
		m_tData.pEmission = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pEmission)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fThickness),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpawnRange),			sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fReversePower),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bReverse),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bPreserveDirection),		sizeof(_bool));
	
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference),		sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vNormal),				sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSize),					sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpriteSheetSize),		sizeof(_int2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),				sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),			sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),			sizeof(_uint));

	m_pCCInstLifeTime		= CCurveContainer::Read(_inFile);
	m_pCCStart				= CCurveContainer::Read(_inFile);
	m_pCCLength				= CCurveContainer::Read(_inFile);
	m_pCCDelay				= CCurveContainer::Read(_inFile);
	m_pCCBloom				= CCurveContainer::Read(_inFile);
	m_pCCFade				= CCurveContainer::Read(_inFile);
	m_pCCSprite				= CCurveContainer::Read(_inFile);

	m_vReference			= m_tData.vForceReference;
	m_vNormal				= m_tData.vNormal;

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticleLine::Set_Data(const VFX_ParticleLine _tData)
{
	m_tData = _tData;
	m_iActivateInstances = m_tData.iMaxInstance;

	m_vReference = m_tData.vForceReference;
	m_vNormal = m_tData.vNormal;
}
#endif

void CEffectInstance_ParticleLine::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	m_vecInstFetch.resize(m_tData.iMaxInstance);
	m_vecInstTimeAcc.resize(m_tData.iMaxInstance);
	m_vecInstLifeTime.resize(m_tData.iMaxInstance);
	m_vecReverseAngles.resize(m_tData.iMaxInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstTimeAcc[i] = -m_pCCStart->Get_Value(Function::RandomFloat(0.f, m_pCCStart->Get_Total())).x;
		m_vecInstLifeTime[i] = m_pCCInstLifeTime->Get_Value(Function::RandomFloat(0.f, m_pCCInstLifeTime->Get_Total())).x;
		pData[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);

		m_vecReverseAngles[i] = Function::Cone_DiffusionAngle(m_vReference, m_tData.fTheta, m_tData.fPhi);
	}

	_float3 vCriteria = XMVector3Cross(XMVector3Cross(XMVector3Normalize(m_vReference), XMVector3Normalize(m_vecReverseAngles.front())), XMVector3Normalize(m_vReference));
	std::sort(m_vecReverseAngles.begin(), m_vecReverseAngles.end(),
		[&](const _vectorf& vA, const _vectorf& vB)
		{
			_float fAngleA = Function::PlaneAcos2(m_vReference, vCriteria,
			XMVector3Cross(XMVector3Cross(XMVector3Normalize(m_vReference), XMVector3Normalize(vA)), XMVector3Normalize(m_vReference)));
			_float fAngleB = Function::PlaneAcos2(m_vReference, vCriteria,
			XMVector3Cross(XMVector3Cross(XMVector3Normalize(m_vReference), XMVector3Normalize(vB)), XMVector3Normalize(m_vReference)));
			return fAngleA < fAngleB;
		}
	);
}

void CEffectInstance_ParticleLine::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstTimeAcc[i] += _fTimeDelta;

		_float fRatio = powf(fmodf(m_vecInstTimeAcc[i], m_vecInstLifeTime[i]) / m_vecInstLifeTime[i], m_tData.fReversePower);
		if (pData[i].vRight.y && pData[i].vRight.x > fRatio)
		{
			pData[i].vColor.w		= 0.f;
			pData[i].vRight.y		= 0.f;
			if (m_fTimeAcc < m_tData.fLifeTime)
			{
				m_vecInstTimeAcc[i]	= -m_pCCDelay->Get_Value(Function::RandomFloat(0.f, m_pCCDelay->Get_Total())).x;
				m_vecInstFetch[i]	= false;
			}
		}
		pData[i].vRight.x = fRatio;

		if (m_vecInstTimeAcc[i] <= 0.f)
		{
			pData[i].vColor.w	= 0.f;
			m_vecInstFetch[i]	= false;
			continue;
		}
		else if (fRatio >= 1.f)
		{
			pData[i].vColor.w	= 0.f;
			m_vecInstFetch[i]	= false;
			continue;
		}
		else if (!m_vecInstFetch[i])
		{
			m_vecInstFetch[i] = true;
			pData[i].vColor.w = 1.f;
			if (!m_tData.bPreserveDirection || (m_tData.bPreserveDirection && !pData[i].vRight.z))
			{
				pData[i].vRight.z = 1.f;
				if (!m_tData.bReverse)
				{
					pData[i].vTranslation	= _float4(m_pTransform->Get_State(TRANSFORM::POSITION) + Function::RandomVector(-m_tData.vSpawnRange, m_tData.vSpawnRange), 1.f);
					pData[i].vLook			= _float4(_float3(Function::Cone_DiffusionAngle(m_vReference, m_tData.fTheta, m_tData.fPhi)), 0.f);
				}
				else
				{
					pData[i].vTranslation	= _float4(m_vecReverseAngles[i], 0.f);
					pData[i].vLook			= _float4(m_vecReverseAngles[i == m_tData.iMaxInstance - 1? 0 : i + 1], 0.f);
					pData[i].vUp			= _float4(m_pTransform->Get_State(TRANSFORM::POSITION) + Function::RandomVector(-m_tData.vSpawnRange, m_tData.vSpawnRange), 1.f);

					pData[i].vArgument.z	= rand() % (static_cast<_int>(m_tData.vSpriteSheetSize.x) * static_cast<_int>(m_tData.vSpriteSheetSize.y));
				}
			}
		}
		else
		{
			pData[i].vArgument.x		= m_pCCFade->Get_Value(fRatio).x;
			pData[i].vArgument.y		= m_pCCBloom->Get_Value(fRatio).x;
			pData[i].vArgument.w		= fRatio;
			pData[i].vRight.y			= 1.f;
			pData[i].vRight.w			= m_vecInstTimeAcc[i] / m_vecInstLifeTime[i];
			if (m_tData.bReverse)
			{
				pData[i].vUp.w			= m_pCCLength->Get_Value(fRatio).x;
			}
			else
			{
				pData[i].vArgument.z	= floorf(m_pCCSprite->Get_Value(fRatio).x * static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y));
				pData[i].vArgument.z	= std::clamp(pData[i].vArgument.z, 0.f, static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y - 1.f));
				pData[i].vUp			= _float4(m_pCCLength->Get_Value(fRatio));
			}
		}
	}
}

shared_ptr<CEffectInstance_ParticleLine> CEffectInstance_ParticleLine::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticleLine> pInstance = make_private_shared(CEffectInstance_ParticleLine, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleLine::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticleLine::Clone(any)
{
	shared_ptr<CEffectInstance_ParticleLine> pInstance = make_private_shared_copy(CEffectInstance_ParticleLine, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleLine::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticleLine> CEffectInstance_ParticleLine::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticleLine> pInstance = make_private_shared(CEffectInstance_ParticleLine, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleLine::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleLine::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleLine::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticleLine::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect), sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fThickness),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpawnRange),			sizeof(_float3));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fReversePower),		sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bReverse),			sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bPreserveDirection),	sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTheta),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fPhi),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vForceReference),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vNormal),				sizeof(_float3));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSize),				sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances), sizeof(_uint));

	m_pCCInstLifeTime->Export(outFile);
	m_pCCStart->Export(outFile);
	m_pCCLength->Export(outFile);
	m_pCCDelay->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCSprite->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleLine::Export", "Failed to Write File: CEffectInstance_ParticleLine");
	}

	outFile.close();

	return S_OK;
}
#endif
