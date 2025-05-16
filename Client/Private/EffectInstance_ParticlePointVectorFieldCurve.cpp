#include "ClientPCH.h"
#include "EffectInstance_ParticlePointVectorFieldCurve.h"
#include "GameInstance.h"
#include "ComputeAccelerator.h"
#include "Bone.h"

CEffectInstance_ParticlePointVectorFieldCurve::CEffectInstance_ParticlePointVectorFieldCurve(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticlePointVectorFieldCurve)
{
}

CEffectInstance_ParticlePointVectorFieldCurve::CEffectInstance_ParticlePointVectorFieldCurve(const CEffectInstance_ParticlePointVectorFieldCurve& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
	, m_bSpeedBase(_rhs.m_bSpeedBase)
{
	if (_rhs.m_pCCInstLifeTime)		m_pCCInstLifeTime	= _rhs.m_pCCInstLifeTime->Clone();
	if (_rhs.m_pCCScale)			m_pCCScale			= _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCBloom)			m_pCCBloom			= _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)				m_pCCFade			= _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCVelocityX)		m_pCCVelocityX		= _rhs.m_pCCVelocityX->Clone();
	if (_rhs.m_pCCVelocityY)		m_pCCVelocityY		= _rhs.m_pCCVelocityY->Clone();
	if (_rhs.m_pCCVelocityZ)		m_pCCVelocityZ		= _rhs.m_pCCVelocityZ->Clone();
	if (_rhs.m_pCCRotation)			m_pCCRotation		= _rhs.m_pCCRotation->Clone();
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);
	
	m_pCCInstLifeTime		= CCurveContainer::Create();
	m_pCCScale				= CCurveContainer::Create();
	m_pCCBloom				= CCurveContainer::Create();
	m_pCCFade				= CCurveContainer::Create();
	m_pCCVelocityX			= CCurveContainer::Create();
	m_pCCVelocityY			= CCurveContainer::Create();
	m_pCCVelocityZ			= CCurveContainer::Create();
	m_pCCRotation			= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CEffectInstance::Initialize");
	}

	if (m_tData.pNoise)
	{
		CComputeAccelerator::Get_Instance()->Register_ShaderResourceView(shared_from_this(), TEXT("Input"), sizeof(VTXPOSSIZEINSTTRANSCOLORARG), m_tData.iMaxInstance);
		CComputeAccelerator::Get_Instance()->Register_UnorderedAccessView(shared_from_this(), TEXT("Output"), sizeof(_float3), m_tData.iMaxInstance);
	}

	return S_OK;
}

void CEffectInstance_ParticlePointVectorFieldCurve::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (m_pTarget)
	{
		if (m_pBone)
		{
			_float4x4 m = *m_pBone * m_mPivot * m_pTarget->Get_Matrix();
			m_pTransform->Set_State(TRANSFORM::POSITION, _float3(m.row(3)));
		}
		else
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_pTarget->Get_State(TRANSFORM::POSITION));
		}
	}

	m_pTransform->Rotate(XMQuaternionRotationRollPitchYaw(m_tData.vRotationScale.x * _fTimeDelta, m_tData.vRotationScale.y * _fTimeDelta, m_tData.vRotationScale.z * _fTimeDelta));
//	m_bRelease = m_fTimeAcc > Function::MonteCarloMaxValue(m_pCCInstLifeTime, 100) + m_tData.fLifeTime;

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (CGameInstance::Get_Instance()->Key_Hold(VK_LEFT))
		{
			m_pTransform->Translate(_float3(-1.f, 0.f, 0.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold(VK_RIGHT))
		{
			m_pTransform->Translate(_float3(+1.f, 0.f, 0.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold(VK_UP))
		{
			m_pTransform->Translate(_float3(0.f, 0.f, +1.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold(VK_DOWN))
		{
			m_pTransform->Translate(_float3(0.f, 0.f, -1.f));
		}

		if (m_bToolRepeat && m_bRelease)
		{
			Fetch(_float3(0.f, 0.f, 0.f));
		}
	}
#endif
}

void CEffectInstance_ParticlePointVectorFieldCurve::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	Add_RenderObject(RENDER_GROUP::BLOOM);
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Render()
{
	_bool bDCAA(true);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Render_Bloom()
{
	_bool bDCAA(true);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render_Bloom", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Fetch(any _vec_mat_transform_pair_object_bone)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Fetch", "Failed to CEffectInstance::Fetch");
	}

	if (!_vec_mat_transform_pair_object_bone.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Fetch", "Invalid Argument");
	}
	else
	{
		if (_vec_mat_transform_pair_object_bone.type() == typeid(_float3))
		{
			_float3 vPosition = any_cast<_float3>(_vec_mat_transform_pair_object_bone);
			m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
		}
		else if (_vec_mat_transform_pair_object_bone.type() == typeid(_float4x4))
		{
			m_pTransform->Set_Matrix(any_cast<_float4x4>(_vec_mat_transform_pair_object_bone));
		}
		else if (_vec_mat_transform_pair_object_bone.type() == typeid(shared_ptr<CTransform>))
		{
			m_pTarget = any_cast<shared_ptr<CTransform>>(_vec_mat_transform_pair_object_bone);
			m_pTransform->Set_Matrix(m_pTarget->Get_Matrix());
		}
		else if (_vec_mat_transform_pair_object_bone.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
		{
			pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_vec_mat_transform_pair_object_bone);
			shared_ptr<CGameObject> pObject = arg.first;
			shared_ptr<CModel> pModel = pObject->Get_Component<CModel>(COMPONENT::MODEL);
			m_pTarget = pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			m_mPivot = pModel->Get_Pivot();
			m_pBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();
		}
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_bSpeedBase = m_pCCRotation->Get_Value(0.f).x == m_pCCRotation->Get_Value(1.f).x;
	}
#endif

	return S_OK;
}

_bool CEffectInstance_ParticlePointVectorFieldCurve::Return()
{
	return m_bRelease;
}                                                                                                                                                                                                                                                                                             

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Release", "Failed to CGameObject::Release");
	}

	m_vecInstTimeAcc.clear();
	m_vecInstLifeTime.clear();
	m_vecInstVelocity.clear();
	m_vecInstTerminateIndex.clear();
	if (m_tData.pNoise)
	{
		m_vecInstNoise.clear();
	}

	m_iMaxTerminateInstance	= 0;
	m_bTerminate			= false;
	m_pTarget				= nullptr;
	m_mPivot				= g_mUnit;
	m_pBone					= nullptr;

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Initialize(std::ifstream& _inFile)
{
	m_tData.pMesh = CMesh::Read(m_pDevice, m_pContext, MODEL::NONANIM, _inFile, XMMatrixRotationY(XM_PI));
	if (nullptr == m_tData.pMesh)
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CMesh::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}
	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pNoise = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pNoise)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CTexture::Read");
		}
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vVelocityScale),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpawnScale),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vRotationScale),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vNoiseScale),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vNoiseOffset),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vNoiseTiling),			sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bLocal),					sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iSpawnMethod),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fSpawnTerm),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fSpawnOffset),			sizeof(_float));
	
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iTerminateMethod),		sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTerminateTerm),			sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSize),					sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iNoisePass),				sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),				sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),			sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),			sizeof(_uint));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Initialize", "Failed to CTexture::Read");
	}

	m_pCCInstLifeTime		= CCurveContainer::Read(_inFile);
	m_pCCScale				= CCurveContainer::Read(_inFile);
	m_pCCBloom				= CCurveContainer::Read(_inFile);
	m_pCCFade				= CCurveContainer::Read(_inFile);
	m_pCCVelocityX			= CCurveContainer::Read(_inFile);
	m_pCCVelocityY			= CCurveContainer::Read(_inFile);
	m_pCCVelocityZ			= CCurveContainer::Read(_inFile);
	m_pCCRotation			= CCurveContainer::Read(_inFile);

	m_bSpeedBase			= m_pCCRotation->Get_Value(0.f).x == m_pCCRotation->Get_Value(1.f).x;

#ifdef _DEBUG
	if (m_tData.pNoise)
	{
		if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
		{
			CComputeAccelerator::Get_Instance()->Register_ShaderResourceView(shared_from_this(), TEXT("Input"), sizeof(VTXPOSSIZEINSTTRANSCOLORARG), m_tData.iMaxInstance);
			CComputeAccelerator::Get_Instance()->Register_UnorderedAccessView(shared_from_this(), TEXT("Output"), sizeof(_float3), m_tData.iMaxInstance);
		}
	}
#endif

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticlePointVectorFieldCurve::Set_Data(const VFX_ParticlePointVectorFieldCurve _tData)
{
	m_tData = _tData;
	m_iActivateInstances = m_tData.iMaxInstance;
}
#endif

void CEffectInstance_ParticlePointVectorFieldCurve::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
#ifdef _DEBUG
	if (m_tData.pNoise)
	{
		if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
		{
			CComputeAccelerator::Get_Instance()->Erase_System(shared_from_this());
			CComputeAccelerator::Get_Instance()->Register_ShaderResourceView(shared_from_this(), TEXT("Input"), sizeof(VTXPOSSIZEINSTTRANSCOLORARG), m_tData.iMaxInstance);
			CComputeAccelerator::Get_Instance()->Register_UnorderedAccessView(shared_from_this(), TEXT("Output"), sizeof(_float3), m_tData.iMaxInstance);
		}
	}
#endif

	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	if (m_tData.pNoise)
	{
		m_vecInstNoise.resize(m_tData.iMaxInstance);
	}
	m_vecInstTimeAcc.resize(m_tData.iMaxInstance, 0.f);
	m_vecInstLifeTime.resize(m_tData.iMaxInstance, 0.f);
	m_vecInstRotation.resize(m_tData.iMaxInstance, _float2(0.f, 0.f));
	m_vecInstVelocity.resize(m_tData.iMaxInstance, _float3(0.f, 0.f, 0.f));
	
	m_iMaxTerminateInstance	= 0;
	m_bTerminate			= false;

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

//	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
//	{
//		Initiate_Instance(pData, i);
//	}
}

void CEffectInstance_ParticlePointVectorFieldCurve::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	if (m_tData.pNoise)
	{
		_float3 vCenter = m_pTransform->Get_State(TRANSFORM::POSITION);
		if (FAILED(m_tData.pNoise->Bind_ShaderResourceView(CComputeAccelerator::Get_Instance()->Get_Shader(), aiTextureType_DIFFUSE, SHADER_TEXMASK)))
		{
			MSG_RETURN(, "CEffectInstance_ParticlePointVectorFieldCurve::Update_Instance", "Failed to Bind_ShaderResourceView");
		}
		if (FAILED(CComputeAccelerator::Get_Instance()->Get_Shader()->Bind_RawValue("g_vSphericalCenter", &vCenter, sizeof(_float3))))
		{
			MSG_RETURN(, "CEffectInstance_ParticlePointVectorFieldCurve::Update_Instance", "Failed to Bind_RawValue");
		}
		if (FAILED(CComputeAccelerator::Get_Instance()->Get_Shader()->Bind_RawValue("g_vTiling", &m_tData.vNoiseTiling, sizeof(_float2))))
		{
			MSG_RETURN(, "CEffectInstance_ParticlePointVectorFieldCurve::Update_Instance", "Failed to Bind_RawValue");
		}
		CComputeAccelerator::Get_Instance()->Update_Data(shared_from_this(), TEXT("Input"), TEXT("Output"), "g_inputTransColorArg", "g_outputFloat3", pData);
		CComputeAccelerator::Get_Instance()->Compute(m_tData.iNoisePass, 100, 1, 1);
		CComputeAccelerator::Get_Instance()->Receive_Data(shared_from_this(), TEXT("Output"), m_vecInstNoise.data());
	}

	if (m_fTimeAcc < m_tData.fLifeTime)
	{
		for (_int i = 0; i < m_tData.iMaxInstance * (m_fTimeAcc < m_tData.fSpawnTerm ? m_fTimeAcc / m_tData.fSpawnTerm : 1.f); ++i)
		{
			m_vecInstTimeAcc[i] += _fTimeDelta;
	
			if (m_vecInstTimeAcc[i] < m_vecInstLifeTime[i])
			{
				Tick_Instance(pData, i, _fTimeDelta);
			}
			else
			{
				Initiate_Instance(pData, i, _fTimeDelta);
			}
		}
	}
	else
	{
		Exit_Instance(pData, _fTimeDelta);
	}
}

void CEffectInstance_ParticlePointVectorFieldCurve::Initiate_Instance(VTXPOSSIZEINSTTRANSCOLORARG* _pData, _uint _i, _float _fTimeDelta)
{
	VTXMESH	tVertex;
	
	if (m_fTimeAcc < m_tData.fSpawnTerm)
	{
		switch (m_tData.iSpawnMethod)
		{
		case 1:
		{
			_float	fStartRatio	= max(m_fTimeAcc / m_tData.fSpawnTerm, 0.1f);
			_float	fThreshold	= fStartRatio * XM_PI;

			while (true)
			{
				tVertex			= m_tData.pMesh->Get_RandomVTXMESH();
				_float	fRadian	= Function::XZPlaneRadians(XMLoadFloat3(&tVertex.vPosition));

				if (fRadian >= 0.f)
				{
					if (tVertex.vPosition.x < 0.f)
					{
						fRadian = XM_PI - fRadian;
					}
				}
				else
				{
					if (tVertex.vPosition.x >= 0.f)
					{
						fRadian = XM_PI + fRadian;
					}
					else
					{
						fRadian = -fRadian;
					}
				}

				if (fRadian <= fThreshold)
				{
					break;
				}
			}
		}
		break;
		default:
		{
			tVertex				= m_tData.pMesh->Get_RandomVTXMESH();
		}
		break;
		}
	}
	else
	{
			tVertex				= m_tData.pMesh->Get_RandomVTXMESH();
	}

	m_vecInstTimeAcc[_i]		= 0.f;
	m_vecInstVelocity[_i]		= _float3(0.f, 0.f, 0.f);
	m_vecInstLifeTime[_i]		= m_pCCInstLifeTime->Get_Value(Function::RandomFloat(0.f, m_pCCInstLifeTime->Get_Total())).x;
	
	_pData[_i].vTranslation		= _float4(_float3(tVertex.vPosition) * m_tData.vSpawnScale, 1.f);
	if (!m_tData.bLocal)
	{
		_pData[_i].vTranslation = _float4(XMVector3TransformCoord(XMLoadFloat4(&_pData[_i].vTranslation), m_pTransform->Get_Matrix()));
	}
	
	_pData[_i].vColor			= _float4(0.f, 0.f, 0.f, 1.f);
	_pData[_i].vArgument.x		= 1.f;
	
	_float4 vMin			= m_pCCRotation->Get_Value(0.f);
	_float4 vMax			= m_pCCRotation->Get_Value(1.f);
	_float	fS				= m_bSpeedBase
							? min(fabsf(vMin.x - vMax.x), Function::RandomFloat(vMin.y, vMax.y))
							: min(fabsf(vMin.x - vMax.x), Function::RandomFloat(vMin.y, vMax.y));
	if (m_bSpeedBase)
	{
		m_vecInstRotation[_i].x = vMin.y * (rand() % 2 ? 1.f : -1.f);
		m_vecInstRotation[_i].y = vMax.y * (rand() % 2 ? 1.f : -1.f);
	}
	else
	{
		if (rand() % 2)
		{
			m_vecInstRotation[_i].x = Function::RandomFloat(vMin.x, vMax.x - fS);
			m_vecInstRotation[_i].y = m_vecInstRotation[_i].x + fS;
		}
		else
		{
			m_vecInstRotation[_i].x = Function::RandomFloat(vMax.x, vMin.x + fS);
			m_vecInstRotation[_i].y = m_vecInstRotation[_i].x - fS;
		}
	}
}

void CEffectInstance_ParticlePointVectorFieldCurve::Tick_Instance(VTXPOSSIZEINSTTRANSCOLORARG* _pData, _uint _i, _float _fTimeDelta)
{
	_float	fRatio			= m_vecInstTimeAcc[_i] / m_vecInstLifeTime[_i];
	m_vecInstVelocity[_i].x	= m_pCCVelocityX->Get_Value(m_pCCVelocityX->Get_Total() * fRatio).x;
	m_vecInstVelocity[_i].y	= m_pCCVelocityY->Get_Value(m_pCCVelocityY->Get_Total() * fRatio).x;
	m_vecInstVelocity[_i].z	= m_pCCVelocityZ->Get_Value(m_pCCVelocityZ->Get_Total() * fRatio).x;

	_pData[_i].vLook		= _float4(m_vecInstVelocity[_i], 0.f);
	_float3	vVelocity		= m_vecInstVelocity[_i] * m_tData.vVelocityScale;	
	vVelocity				= vVelocity.x * m_pTransform->Get_State(TRANSFORM::RIGHT)
							+ vVelocity.y * m_pTransform->Get_State(TRANSFORM::UP)
							+ vVelocity.z * m_pTransform->Get_State(TRANSFORM::LOOK);
	_pData[_i].vTranslation	= _float4(_pData[_i].vTranslation) + _float4(vVelocity * _fTimeDelta, 0.f);
	if (m_tData.pNoise)
	{
		_pData[_i].vTranslation = _float4(_pData[_i].vTranslation) + _float4(m_vecInstNoise[_i] * m_tData.vNoiseScale * _fTimeDelta, 0.f) + m_tData.vNoiseOffset * _fTimeDelta;
	}

	_pData[_i].vArgument.x	= m_pCCFade->Get_Value(m_pCCFade->Get_Total() * fRatio).x;
	_pData[_i].vArgument.y	= m_pCCBloom->Get_Value(m_pCCBloom->Get_Total() * fRatio).x;
	
	_float2	vScale			= _float2(m_pCCScale->Get_Value(fRatio));
	_pData[_i].vColor		= _float4(vScale.x, vScale.y, _pData[_i].vColor.z, _pData[_i].vColor.w);
	
	_float	fRotation		= m_vecInstRotation[_i].x * (1.f - m_vecInstTimeAcc[_i] / m_vecInstLifeTime[_i]) + m_vecInstRotation[_i].y * (m_vecInstTimeAcc[_i] / m_vecInstLifeTime[_i]);
	_pData[_i].vColor.z		= m_bSpeedBase ? _pData[_i].vColor.z + fRotation * _fTimeDelta : fRotation;

	_float4	vQuaternion		= XMQuaternionInverse(XMQuaternionRotationMatrix(m_pTransform->Get_Matrix()));
	_float4 vRelative		= XMVector3Rotate(_float4(_pData[_i].vTranslation), vQuaternion);
	_pData[_i].vUp			= vRelative;
}

void CEffectInstance_ParticlePointVectorFieldCurve::Exit_Instance(VTXPOSSIZEINSTTRANSCOLORARG* _pData, _float _fTimeDelta)
{	// TODO: SIMD Lib
	if (!m_bTerminate)
	{
		m_iMaxTerminateInstance = m_fTimeAcc < m_tData.fSpawnTerm ? m_tData.iMaxInstance * (m_fTimeAcc / m_tData.fSpawnTerm) : m_tData.iMaxInstance;
	}

	switch (m_tData.iTerminateMethod)
	{
	case 1:
	{
		_int iActiveInstance(0);
		for (_int i = 0; i < m_iMaxTerminateInstance; ++i)
		{
			m_vecInstTimeAcc[i] += _fTimeDelta;

			if (m_vecInstTimeAcc[i] < m_vecInstLifeTime[i])
			{
				++iActiveInstance;
				Tick_Instance(_pData, i, _fTimeDelta);
			}
			else
			{
				m_vecInstTerminateIndex.emplace_back(i);
			}
		}
		for (auto i : m_vecInstTerminateIndex)
		{
			if (iActiveInstance < m_iMaxTerminateInstance * (1.f - std::clamp((m_fTimeAcc - m_tData.fLifeTime) / m_tData.fTerminateTerm, 0.f, 1.f)))
			{
				++iActiveInstance;
				Terminate_Instance(_pData, i);
			}
			else
			{
				ZeroMemory(&_pData[i], sizeof(VTXPOSSIZEINSTTRANSCOLORARG));
			//	break;
			}
		}
		m_vecInstTerminateIndex.clear();
	}
	break;
	default:
	{
		for (_int i = 0; i < m_iMaxTerminateInstance; ++i)
		{
			m_vecInstTimeAcc[i] += _fTimeDelta;

			if (m_vecInstTimeAcc[i] < m_vecInstLifeTime[i])
			{
				Tick_Instance(_pData, i, _fTimeDelta);
			}
			else
			{
				ZeroMemory(&_pData[i], sizeof(VTXPOSSIZEINSTTRANSCOLORARG));
			}
		}
	}
	break;
	}

	_bool	bRelease = true;
	for (_int i = 0; i < m_iMaxTerminateInstance; ++i)
	{
		if (bRelease)
		{
			bRelease = m_vecInstTimeAcc[i] >= m_vecInstLifeTime[i];
		}
	}
	m_bRelease		= bRelease;
	m_bTerminate	= true;
}

void CEffectInstance_ParticlePointVectorFieldCurve::Terminate_Instance(VTXPOSSIZEINSTTRANSCOLORARG* _pData, _uint _i, _float _fTimeDelta)
{
	VTXMESH	tVertex;
	
	if (m_fTimeAcc > m_tData.fLifeTime)
	{
		switch (m_tData.iTerminateMethod)
		{
		case 1:
		{
			_float	fEndRatio	= max(1.f - ((m_fTimeAcc - m_tData.fLifeTime) / m_tData.fTerminateTerm), 0.f);
			_float	fThreshold	= fEndRatio * XM_PI;

			while (true)
			{
				tVertex			= m_tData.pMesh->Get_RandomVTXMESH();
				_float	fRadian	= Function::XZPlaneRadians(XMLoadFloat3(&tVertex.vPosition));

				if (fRadian >= 0.f)
				{
					if (tVertex.vPosition.x < 0.f)
					{
						fRadian = XM_PI - fRadian;
					}
				}
				else
				{
					if (tVertex.vPosition.x >= 0.f)
					{
						fRadian = XM_PI + fRadian;
					}
					else
					{
						fRadian = -fRadian;
					}
				}

				if (fRadian <= fThreshold)
				{
					break;
				}
			}
		}
		break;
		default:
		{
			tVertex				= m_tData.pMesh->Get_RandomVTXMESH();
		}
		break;
		}
	}
	else
	{
		tVertex					= m_tData.pMesh->Get_RandomVTXMESH();
	}

	m_vecInstTimeAcc[_i]		= 0.f;
	m_vecInstVelocity[_i]		= _float3(0.f, 0.f, 0.f);
	m_vecInstLifeTime[_i]		= m_pCCInstLifeTime->Get_Value(Function::RandomFloat(0.f, m_pCCInstLifeTime->Get_Total())).x;
	
	_pData[_i].vTranslation		= _float4(_float3(tVertex.vPosition) * m_tData.vSpawnScale, 1.f);
	if (!m_tData.bLocal)
	{
		_pData[_i].vTranslation	= _float4(XMVector3TransformCoord(XMLoadFloat4(&_pData[_i].vTranslation), m_pTransform->Get_Matrix()));
	}
	
	_pData[_i].vColor			= _float4(0.f, 0.f, 0.f, 1.f);
	_pData[_i].vArgument.x		= 1.f;
}
                                                                                                                                                                             
shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> CEffectInstance_ParticlePointVectorFieldCurve::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointVectorFieldCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointVectorFieldCurve::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticlePointVectorFieldCurve::Clone(any)
{
	shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pInstance = make_private_shared_copy(CEffectInstance_ParticlePointVectorFieldCurve, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointVectorFieldCurve::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> CEffectInstance_ParticlePointVectorFieldCurve::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointVectorFieldCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointVectorFieldCurve::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointVectorFieldCurve::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointVectorFieldCurve::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticlePointVectorFieldCurve::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect), sizeof(EFFECT));

	m_tData.pMesh->Export(outFile, MODEL::NONANIM);

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if(m_tData.pNoise)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pNoise->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vVelocityScale),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpawnScale),			sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vRotationScale),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vNoiseScale),			sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vNoiseOffset),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vNoiseTiling),		sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bLocal),				sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iSpawnMethod),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fSpawnTerm),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fSpawnOffset),		sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iTerminateMethod),	sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTerminateTerm),		sizeof(_float));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSize),				sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iNoisePass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances),		sizeof(_uint));

	m_pCCInstLifeTime->Export(outFile);
	m_pCCScale->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCVelocityX->Export(outFile);
	m_pCCVelocityY->Export(outFile);
	m_pCCVelocityZ->Export(outFile);
	m_pCCRotation->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointVectorFieldCurve::Export", "Failed to Write File: CEffectInstance_ParticlePointVectorFieldCurve");
	}

	outFile.close();

	return S_OK;
}
#endif
