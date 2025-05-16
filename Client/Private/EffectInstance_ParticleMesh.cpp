#include "ClientPCH.h"
#include "EffectInstance_ParticleMesh.h"
#include "GameInstance.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

CEffectInstance_ParticleMesh::CEffectInstance_ParticleMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticleMesh)
{
}

CEffectInstance_ParticleMesh::CEffectInstance_ParticleMesh(const CEffectInstance_ParticleMesh& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
	, m_bLinearStart(_rhs.m_bLinearStart)
{
	if (_rhs.m_pCCInstLifeTime)		m_pCCInstLifeTime = _rhs.m_pCCInstLifeTime->Clone();
	if (_rhs.m_pCCGravityScale)		m_pCCGravityScale = _rhs.m_pCCGravityScale->Clone();
	if (_rhs.m_pCCStart)			m_pCCStart = _rhs.m_pCCStart->Clone();
	if (_rhs.m_pCCSpeed)			m_pCCSpeed = _rhs.m_pCCSpeed->Clone();
	if (_rhs.m_pCCScale)			m_pCCScale = _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCBloom)			m_pCCBloom = _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)				m_pCCFade = _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCRotation)			m_pCCRotation = _rhs.m_pCCRotation->Clone();

#ifdef ACTIVATE_TOOL
	if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene())
	{
		Add_Component(COMPONENT::VIBUFFER_INSTANCE_MESH, _rhs.m_umapComponent.find(COMPONENT::VIBUFFER_INSTANCE_MESH)->second);
	}
#endif
}

HRESULT CEffectInstance_ParticleMesh::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHVFX, g_aNull);
	
	m_pCCInstLifeTime		= CCurveContainer::Create();
	m_pCCGravityScale		= CCurveContainer::Create();
	m_pCCSpeed				= CCurveContainer::Create();
	m_pCCStart				= CCurveContainer::Create();
	m_pCCScale				= CCurveContainer::Create();
	m_pCCBloom				= CCurveContainer::Create();
	m_pCCFade				= CCurveContainer::Create();
	m_pCCRotation			= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticleMesh::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
	
	m_bRelease = m_fTimeAcc > m_tData.fLifeTime;

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (m_bToolRepeat && m_bRelease)
		{
		//	Fetch(m_mFetch);
		}
	}
#endif
}

void CEffectInstance_ParticleMesh::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(m_tData.bBlend ? RENDER_GROUP::BLEND : RENDER_GROUP::NONBLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_ParticleMesh::Render()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Render_Bloom()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Fetch(any _arg)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Fetch", "Failed to CEffectInstance::Fetch");
	}

	if (_arg.has_value() && _arg.type() == typeid(_float3))
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, any_cast<_float3>(_arg));
	}
	else if (_arg.has_value() && _arg.type() == typeid(_float4x4))
	{
		m_pTransform->Set_Matrix(any_cast<_float4x4>(_arg));
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_bLinearStart = m_pCCStart->Get_Value(0.f).w && (m_pCCStart->Get_Value(0.f).x == m_pCCStart->Get_Value(1.f).x);
	}
#endif

	return S_OK;
}

_bool CEffectInstance_ParticleMesh::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_ParticleMesh::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Release", "Failed to CGameObject::Release");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Initialize(std::ifstream& _inFile)
{
	m_tData.pMesh = CMesh::Read(m_pDevice, m_pContext, MODEL::NONANIM, _inFile, g_mUnit);
	if (nullptr == m_tData.pMesh)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CMesh::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime), sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference), sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bBlend), sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance), sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass), sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass), sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances), sizeof(_uint));

	m_pCCInstLifeTime		= CCurveContainer::Read(_inFile);
	m_pCCStart				= CCurveContainer::Read(_inFile);
	m_pCCSpeed				= CCurveContainer::Read(_inFile);
	m_pCCGravityScale		= CCurveContainer::Read(_inFile);
	m_pCCScale				= CCurveContainer::Read(_inFile);
	m_pCCBloom				= CCurveContainer::Read(_inFile);
	m_pCCFade				= CCurveContainer::Read(_inFile);
	m_pCCRotation			= CCurveContainer::Read(_inFile);

	m_bLinearStart			= m_pCCStart->Get_Value(0.f).w && (m_pCCStart->Get_Value(0.f).x == m_pCCStart->Get_Value(1.f).x);

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CTexture::Read");
	}

	if (FAILED(Add_Component(COMPONENT::VIBUFFER_INSTANCE_MESH, CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, MODEL::NONANIM, m_tData.pMesh, m_tData.iMaxInstance))))
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to Add_Component");
	}
	if (FAILED(Get_Component<CVIBufferInstance_Mesh>(COMPONENT::VIBUFFER_INSTANCE_MESH)->Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Initialize", "Failed to CVIBufferInstance_Mesh::Initialize");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Ready_Components", "Failed to CEffect::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Ready_Components", "Nullptr Exception: m_pShader");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticleMesh::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (COMPONENT::VIBUFFER_INSTANCE_MESH == _eComponent && !_pComponent)
	{
#if ACTIVATE_TOOL
		if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene())
#endif
		return S_OK;
	}

	if (FAILED(__super::Add_Component(_eComponent, _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Ready_Components", "Failed to CGameObject::Add_Component");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticleMesh::Set_Data(const VFX_ParticleMesh _tData)
{
	_bool bSwapMesh = m_tData.pMesh != _tData.pMesh;

	m_tData = _tData;
	m_iActivateInstances = _tData.iMaxInstance;

	if (bSwapMesh)
	{
		Delete_Component(COMPONENT::VIBUFFER_INSTANCE_MESH);

		if (FAILED(Add_Component(COMPONENT::VIBUFFER_INSTANCE_MESH, CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, MODEL::NONANIM, m_tData.pMesh, m_tData.iMaxInstance))))
		{
			MSG_RETURN(, "CEffectInstance_ParticleMesh::Initialize", "Failed to Add_Component");
		}
		if (FAILED(Get_Component<CVIBufferInstance_Mesh>(COMPONENT::VIBUFFER_INSTANCE_MESH)->Initialize()))
		{
			MSG_RETURN(, "CEffectInstance_ParticleMesh::Initialize", "Failed to CVIBufferInstance_Mesh::Initialize");
		}
	}
}
#endif

void CEffectInstance_ParticleMesh::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	m_vecInstTimeAcc.resize(m_tData.iMaxInstance);
	m_vecInstLifeTime.resize(m_tData.iMaxInstance);
	m_vecInstRotation.resize(m_tData.iMaxInstance);
	m_vecInstVelocity.resize(m_tData.iMaxInstance);
	m_vecInstFetch.resize(m_tData.iMaxInstance);

	VTXMESHINSTTRANSCOLORARG* pData = reinterpret_cast<VTXMESHINSTTRANSCOLORARG*>(_pData);

	if (m_bLinearStart)
	{
		_float fMaxDelay = max(m_pCCStart->Get_Value(0.f).x, m_pCCStart->Get_Value(1.f).x);
		for (_int i = 0; i < m_tData.iMaxInstance; ++i)
		{
			m_vecInstTimeAcc[i] = -static_cast<_float>(i) / static_cast<_float>(m_tData.iMaxInstance) * fMaxDelay;
		}
	}
	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		if (!m_bLinearStart)
		{
			m_vecInstTimeAcc[i] = -m_pCCStart->Get_Value(Function::RandomFloat(0.f, m_pCCStart->Get_Total())).x;
		}
		m_vecInstLifeTime[i]	= m_pCCInstLifeTime->Get_Value(Function::RandomFloat(0.f, m_pCCInstLifeTime->Get_Total())).x;
		m_vecInstVelocity[i]	= Function::Cone_DiffusionAngle(m_tData.vForceReference, m_tData.fTheta, m_tData.fPhi);
		m_vecInstRotation[i]	= Function::RandomVector(m_pCCRotation->Get_Value(0.f), m_pCCRotation->Get_Value(m_pCCRotation->Get_Total()));

		ZeroMemory(&pData[i], sizeof(_float4x4));

		pData[i].vColor = float4(1.f, 1.f, 1.f, 1.f);
	}
}

void CEffectInstance_ParticleMesh::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXMESHINSTTRANSCOLORARG* pData = reinterpret_cast<VTXMESHINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstTimeAcc[i] += _fTimeDelta;
		if (m_vecInstTimeAcc[i] <= 0.f)
		{
			m_vecInstFetch[i] = false;
			continue;
		}
		else if (m_vecInstTimeAcc[i] >= m_vecInstLifeTime[i])
		{
			m_vecInstFetch[i] = false;
			continue;
		}
		else if (!m_vecInstFetch[i])
		{
			m_vecInstFetch[i] = true;

			_matrix m = Function::MatrixRandomRotation(g_mUnit);
			pData[i].vRight			= _float4(m.r[0]);
			pData[i].vUp			= _float4(m.r[1]);
			pData[i].vLook			= _float4(m.r[2]);
			pData[i].vTranslation	= _float4(m.r[3]);
		}
		else
		{
			_matrix mData			= _float4x4(pData[i].vRight, pData[i].vUp, pData[i].vLook, pData[i].vTranslation);
			mData					= XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(m_vecInstRotation[i] * m_vecInstRotation[i].w * _fTimeDelta)) * mData;

			_float4 vScale			= m_pCCScale->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]);
			mData.r[0]				= XMVector3Normalize(mData.r[0]) * vScale.x;
			mData.r[1]				= XMVector3Normalize(mData.r[1]) * vScale.y;
			mData.r[2]				= XMVector3Normalize(mData.r[2]) * vScale.z;

			pData[i].vRight			= _float4(mData.r[0]);
			pData[i].vUp			= _float4(mData.r[1]);
			pData[i].vLook			= _float4(mData.r[2]);

			_float4 vPrevPos		= _float4(pData[i].vTranslation);
			pData[i].vTranslation	= _float4(pData[i].vTranslation) + _float4(m_vecInstVelocity[i], 0.f).normalize() * m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x * _fTimeDelta;
			pData[i].vTranslation	= _float4(pData[i].vTranslation) - _float4(0.f, g_fGravity, 0.f, 0.f) * m_pCCGravityScale->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x * _fTimeDelta;
			m_vecInstVelocity[i]	= _float4(pData[i].vTranslation) - vPrevPos;

			pData[i].vArgument.x	= m_pCCFade->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x;
			pData[i].vArgument.y	= m_pCCBloom->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x;
		}
	}
}

shared_ptr<CEffectInstance_ParticleMesh> CEffectInstance_ParticleMesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticleMesh> pInstance = make_private_shared(CEffectInstance_ParticleMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleMesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticleMesh::Clone(any)
{
	shared_ptr<CEffectInstance_ParticleMesh> pInstance = make_private_shared_copy(CEffectInstance_ParticleMesh, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleMesh::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticleMesh> CEffectInstance_ParticleMesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticleMesh> pInstance = make_private_shared(CEffectInstance_ParticleMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleMesh::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleMesh::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticleMesh::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticleMesh::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect), sizeof(EFFECT));

	m_tData.pMesh->Export(outFile, MODEL::NONANIM);

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime), sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTheta), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fPhi), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vForceReference), sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bBlend), sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance), sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass), sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass), sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances), sizeof(_uint));

	m_pCCInstLifeTime->Export(outFile);
	m_pCCStart->Export(outFile);
	m_pCCSpeed->Export(outFile);
	m_pCCGravityScale->Export(outFile);
	m_pCCScale->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCRotation->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticleMesh::Export", "Failed to Write File: CEffectRegular_VisualMesh");
	}

	outFile.close();

	return S_OK;
}
#endif
