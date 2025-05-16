#include "ClientPCH.h"
#include "EffectInstance_ParticlePointDynamicCurve.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectInstance_ParticlePointDynamicCurve::CEffectInstance_ParticlePointDynamicCurve(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticlePointDynamicCurve)
{
}

CEffectInstance_ParticlePointDynamicCurve::CEffectInstance_ParticlePointDynamicCurve(const CEffectInstance_ParticlePointDynamicCurve& _rhs)
	: CEffectInstance(_rhs)
	, m_tData(_rhs.m_tData)
	, m_bRandomSprite(_rhs.m_bRandomSprite)
	, m_bLinearStart(_rhs.m_bLinearStart)
	, m_bSpeedBase(_rhs.m_bSpeedBase)
	, m_bRotatePivot(_rhs.m_bRotatePivot)
{
	if (_rhs.m_pCCInstLifeTime)		m_pCCInstLifeTime = _rhs.m_pCCInstLifeTime->Clone();
	if (_rhs.m_pCCGravityScale)		m_pCCGravityScale = _rhs.m_pCCGravityScale->Clone();
	if (_rhs.m_pCCStart)			m_pCCStart = _rhs.m_pCCStart->Clone();
	if (_rhs.m_pCCSpeed)			m_pCCSpeed = _rhs.m_pCCSpeed->Clone();
	if (_rhs.m_pCCScale)			m_pCCScale = _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCBloom)				m_pCCBloom = _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)				m_pCCFade = _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCSprite)			m_pCCSprite = _rhs.m_pCCSprite->Clone();
	if (_rhs.m_pCCRotation)			m_pCCRotation = _rhs.m_pCCRotation->Clone();
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Initialize_Prototype()
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
	m_pCCSprite				= CCurveContainer::Create();
	m_pCCRotation			= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticlePointDynamicCurve::Tick(_float _fTimeDelta)
{
	if (m_pTarget)
	{
		m_vPrevPos = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_pTransform->Set_Matrix(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_pTransform->Rotate(TRANSFORM::RIGHT, XM_PIDIV2);
	}

	__super::Tick(_fTimeDelta);

	m_bRelease = m_fTimeAcc > Function::MonteCarloMaxValue(m_pCCInstLifeTime, 100) * m_tData.fInstLifeTimeScale + Function::MonteCarloMaxValue(m_pCCStart, 100) * m_tData.fInstStartTimeScale;

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_bRandomSprite = !m_pCCSprite->Get_Value(1.f).x;

		if (!m_tData.pMesh)
		{
			static _bool	bOnCursor(true);
			static _float	fOffsetZ(5.f);
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("NDC"))
				{
					ImGui::Checkbox("OnCursor", &bOnCursor);
					ImGui::InputFloat("OffsetZ", &fOffsetZ);
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			if (bOnCursor)
			{
				m_pTransform->Set_State(TRANSFORM::POSITION, Function::Screen_To_World(
					_float2(static_cast<_float>(CGameInstance::Get_Instance()->Get_CursorPos().x), static_cast<_float>(CGameInstance::Get_Instance()->Get_CursorPos().y)), fOffsetZ));
			}
			 
			if (m_bToolRepeat && m_bRelease)
			{
				if (bOnCursor)
				{
					Fetch(Function::Screen_To_World(
						_float2(static_cast<_float>(CGameInstance::Get_Instance()->Get_CursorPos().x), static_cast<_float>(CGameInstance::Get_Instance()->Get_CursorPos().y)), fOffsetZ));
				}
				else
				{
				//	Fetch(_float4x4(g_mUnit));
					_float4x4 m = _float4x4(
						_float4(1.f, 0.f, 0.f, 0.f),
						_float4(0.f, 1.f, 0.f, 0.f),
						_float4(0.f, 0.f, 1.f, 0.f),
						_float4(0.6f, 0.8f, 0.f, 1.f)
					);
					m_pTransform->Set_Matrix(m);
					Fetch(m);
				}
			}
		}
	}
#endif
}

void CEffectInstance_ParticlePointDynamicCurve::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	Add_RenderObject(RENDER_GROUP::BLOOM);

	_bool bReset(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bReset, sizeof(_bool))))
	{
		MSG_RETURN(, "CEffectInstance_ParticlePointDynamicCurve::Late_Tick", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Render()
{
	_bool bDCAA(true);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Render_Bloom()
{
	_bool bDCAA(true);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SIZE, &m_tData.vSize, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SIZE");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render_Bloom", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Fetch(any _vec_mat_transform_pair_object_bone)
{
	if (!_vec_mat_transform_pair_object_bone.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Fetch", "Invalid Argument");
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
			m_pTransform = any_cast<shared_ptr<CTransform>>(_vec_mat_transform_pair_object_bone);
		}
		else if (_vec_mat_transform_pair_object_bone.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
		{
			pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_vec_mat_transform_pair_object_bone);
			shared_ptr<CGameObject> pObject = arg.first;
			shared_ptr<CModel> pModel = pObject->Get_Component<CModel>(COMPONENT::MODEL);
			m_pTarget = pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			m_mPivot = pModel->Get_Pivot();
			m_pBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();

			m_vPrevPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			m_pTransform->Set_Matrix(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
			m_pTransform->Rotate(TRANSFORM::RIGHT, XM_PIDIV2);
		}
		else if (_vec_mat_transform_pair_object_bone.type() == typeid(tuple<shared_ptr<CGameObject>, const _char*, _float3>))
		{
			tuple<shared_ptr<CGameObject>, const _char*, _float3> arg = any_cast<tuple<shared_ptr<CGameObject>, const _char*, _float3>>(_vec_mat_transform_pair_object_bone);
			shared_ptr<CGameObject> pObject = std::get<0>(arg);
			shared_ptr<CModel> pModel = pObject->Get_Component<CModel>(COMPONENT::MODEL);
			m_pTarget = pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			m_mPivot = pModel->Get_Pivot();
			m_pBone = pModel->Get_Bone(std::get<1>(arg))->Get_CombinedTransformationPointer();
			m_tData.vForceReference = std::get<2>(arg);

			m_vPrevPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			m_pTransform->Set_Matrix(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
			m_pTransform->Rotate(TRANSFORM::RIGHT, XM_PIDIV2);
		}
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_bRandomSprite	= !m_pCCSprite->Get_Value(1.f).x;
		m_bLinearStart	= m_pCCStart->Get_Value(0.f).w && (m_pCCStart->Get_Value(0.f).x == m_pCCStart->Get_Value(m_pCCStart->Get_Total()).x);
		m_bSpeedBase	= m_pCCRotation->Get_Value(0.f).x == m_pCCRotation->Get_Value(m_pCCRotation->Get_Total()).x;
		m_bRotatePivot	= m_pCCRotation->Get_Value(0.f).w || m_pCCRotation->Get_Value(m_pCCRotation->Get_Total()).w;
	}
#endif

	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Fetch", "Failed to CEffectInstance::Fetch");
	}
	
	m_mFetch = m_pTransform->Get_Matrix();

	return S_OK;
}

_bool CEffectInstance_ParticlePointDynamicCurve::Return()
{
	return m_bRelease;
}                                                                                                                                                                                                                                                                                             

HRESULT CEffectInstance_ParticlePointDynamicCurve::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Release", "Failed to CGameObject::Release");
	}

	m_pTarget = nullptr;
	m_mPivot = g_mUnit;
	m_pBone = nullptr;
//	m_bRandomSprite = false;
//	m_bLinearStart = false;

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointDynamicCurve::Initialize(std::ifstream& _inFile)
{
	_bool bExist(false);

	_inFile.read(reinterpret_cast<_byte*>(&bExist), sizeof(_bool));
	if (bExist)
	{
		m_tData.pMesh = CMesh::Read(m_pDevice, m_pContext, MODEL::NONANIM, _inFile, XMMatrixRotationY(XM_PI));
		if (nullptr == m_tData.pMesh)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Initialize", "Failed to CMesh::Read");
		}
	}
	_inFile.read(reinterpret_cast<_byte*>(&bExist), sizeof(_bool));
	if (bExist)
	{
		m_tData.pDiffuse = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pDiffuse)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference),		sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstLifeTimeScale),		sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstStartTimeScale),	sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fVelocityTail),			sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bLocal),					sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bReverse),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vLocalSpaceMax),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vLocalSpaceMin),			sizeof(_float3));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpawnRange),			sizeof(_float3));

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
	m_pCCSprite				= CCurveContainer::Read(_inFile);
	m_pCCRotation			= CCurveContainer::Read(_inFile);

	m_bRandomSprite			= !m_pCCSprite->Get_Value(1.f).x;
	m_bLinearStart			= m_pCCStart->Get_Value(0.f).w && (m_pCCStart->Get_Value(0.f).x == m_pCCStart->Get_Value(m_pCCStart->Get_Total()).x);
	m_bSpeedBase			= m_pCCRotation->Get_Value(0.f).x == m_pCCRotation->Get_Value(m_pCCRotation->Get_Total()).x;
	m_bRotatePivot			= m_pCCRotation->Get_Value(0.f).w || m_pCCRotation->Get_Value(m_pCCRotation->Get_Total()).w;

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticlePointDynamicCurve::Set_Data(const VFX_ParticlePointDynamicCurve _tData)
{
	m_tData = _tData;
	m_iActivateInstances = m_tData.iMaxInstance;
}
#endif

void CEffectInstance_ParticlePointDynamicCurve::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	m_vecInstTimeAcc.resize(m_tData.iMaxInstance);
	m_vecInstLifeTime.resize(m_tData.iMaxInstance);
	m_vecInstInitialPosition.resize(m_tData.iMaxInstance);
	m_vecInstRotation.resize(m_tData.iMaxInstance);
	m_vecInstVelocity.resize(m_tData.iMaxInstance);
	m_vecInstFetch.resize(m_tData.iMaxInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

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
		
		if (!m_bRotatePivot)
		{
			_float4 vMin			= m_pCCRotation->Get_Value(0.f);
			_float4 vMax			= m_pCCRotation->Get_Value(m_pCCRotation->Get_Total());
			_float	fS				= m_bSpeedBase
									? min(fabsf(vMin.x - vMax.x), Function::RandomFloat(vMin.y, vMax.y))
									: min(fabsf(vMin.x - vMax.x), Function::RandomFloat(vMin.y, vMax.y));
			if (m_bSpeedBase)
			{
				m_vecInstRotation[i].x = vMin.y * (rand() % 2 ? 1.f : -1.f);
				m_vecInstRotation[i].y = vMax.y * (rand() % 2 ? 1.f : -1.f);
			}
			else
			{
				if (rand() % 2)
				{
					m_vecInstRotation[i].x = Function::RandomFloat(vMin.x, vMax.x - fS);
					m_vecInstRotation[i].y = m_vecInstRotation[i].x + fS;
				}
				else
				{
					m_vecInstRotation[i].x = Function::RandomFloat(vMax.x, vMin.x + fS);
					m_vecInstRotation[i].y = m_vecInstRotation[i].x - fS;
				}
			}
		}
		else
		{
			//
		}
		
		pData[i].vRight = _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);
		pData[i].vUp = _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);

		pData[i].vColor = float4(1.f, 1.f, 0.f, 0.f);
	}
}

void CEffectInstance_ParticlePointDynamicCurve::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	static _float4 a;
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstTimeAcc[i] += _fTimeDelta;
		if (m_vecInstTimeAcc[i] <= 0.f)
		{
			pData[i].vColor.w = 0.f;
			m_vecInstFetch[i] = false;
			continue;
		}
		else if (m_vecInstTimeAcc[i] >= m_vecInstLifeTime[i])
		{
			m_vecInstFetch[i] = false;
			pData[i].vColor.w = 0.f;
			continue;
		}
		else if (!m_vecInstFetch[i])
		{
			pData[i].vRight			= _float4(m_pTransform->Get_State(TRANSFORM::RIGHT), 0.f);
			pData[i].vUp			= _float4(m_pTransform->Get_State(TRANSFORM::UP), 0.f);
		//	pData[i].vLook			= _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);
		//	pData[i].vTranslation	= _float4(m_pTransform->Get_State(TRANSFORM::POSITION), 1.f);
		//	pData[i].vLook			= _float4(m_pTransform->Get_State(TRANSFORM::POSITION)
		//							+ (rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.x) * m_pTransform->Get_State(TRANSFORM::RIGHT)
		//							+ (rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.y) * m_pTransform->Get_State(TRANSFORM::UP)
		//							+ (rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.z) * m_pTransform->Get_State(TRANSFORM::LOOK), 1.f);
		//	do
		//	{
		//		vMin = _float3(
		//			(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMin.x),
		//			(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMin.y),
		//			(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMin.z));
		//	} while (fabsf(vMin.x) < m_tData.vLocalSpaceMin.x && fabsf(vMin.x) < m_tData.vLocalSpaceMin.y && fabsf(vMin.x) < m_tData.vLocalSpaceMin.z);
		//	_float3 vMin = m_tData.vLocalSpaceMin * _float3(rand() % 2 ? 1.f : -1.f, rand() % 2 ? 1.f : -1.f, rand() % 2 ? 1.f : -1.f);
		//	_float3 vMax = _float3(
		//		(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.x - m_tData.vLocalSpaceMin.x),
		//		(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.y - m_tData.vLocalSpaceMin.y),
		//		(rand() % 2 ? 1.f : -1.f) * Function::RandomFloat(0.f, m_tData.vLocalSpaceMax.z - m_tData.vLocalSpaceMin.z));
		//	_float3 vSpace = m_pTransform->Get_State(TRANSFORM::POSITION) + vMin + vMax;
			_float3 vCode	= _float3(rand() % 2 ? -1.f : 1.f, rand() % 2 ? -1.f : 1.f, rand() % 2 ? -1.f : 1.f);
			_float3 vMax	= Function::RandomVector(XMVectorReplicate(0.f), m_tData.vLocalSpaceMax);
			while (vMax.length() < m_tData.vLocalSpaceMin.x)
			{
				vMax = Function::RandomVector(XMVectorReplicate(0.f), m_tData.vLocalSpaceMax);
			}
			pData[i].vLook	= _float4(m_pTransform->Get_State(TRANSFORM::POSITION) + vMax * vCode, 1.f);

			if (m_tData.pMesh)
			{
				pData[i].vLook = _float4(pData[i].vLook) + m_tData.vSpawnRange * _float3(m_tData.pMesh->Get_RandomVTXMESH().vPosition);
			}
			pData[i].vTranslation = pData[i].vLook;
			m_vecInstFetch[i] = true;
			pData[i].vColor.w = 1.f;
			if (m_bRandomSprite)
			{
				pData[i].vArgument.z = static_cast<_float>(rand() % m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y);
			}
			m_vecInstInitialPosition[i] = _float4(pData[i].vTranslation);
			m_vecInstInitialPosition[i].w = _float3(m_vecInstInitialPosition[i] - m_pTransform->Get_State(TRANSFORM::POSITION)).length();
		}
		else
		{
			pData[i].vRight = _float4(m_pTransform->Get_State(TRANSFORM::RIGHT), 0.f);
			pData[i].vUp = _float4(m_pTransform->Get_State(TRANSFORM::UP), 0.f);
			pData[i].vLook = pData[i].vTranslation;
			if (!m_tData.bReverse)
			{
				pData[i].vTranslation = _float4(pData[i].vTranslation) + _float4(m_vecInstVelocity[i], 0.f).normalize() * m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x * _fTimeDelta;
			}
			else
			{
			//	pData[i].vTranslation = _float4(m_vecInstInitialPosition[i] * (1.f - m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x)
			//		+ m_pTransform->Get_State(TRANSFORM::POSITION) * m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x);
			
				pData[i].vTranslation = _float4(m_mFetch.row(3) +
					_float4(_float4(pData[i].vTranslation) - m_mFetch.row(3)).normalize() * m_vecInstInitialPosition[i].w * m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x);
				pData[i].vTranslation.w = 1.f;

			//	pData[i].vTranslation = _float4(m_vecInstInitialPosition[i] * (1.f - m_pCCSpeed->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x));
			}
			if (m_tData.bLocal)
			{
				pData[i].vTranslation = _float4(pData[i].vTranslation) + _float4(m_pTransform->Get_State(TRANSFORM::POSITION) - m_vPrevPos, 1.f);
			}

			pData[i].vArgument.x = m_pCCFade->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x;
			pData[i].vArgument.y = m_pCCBloom->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x;
			if (!m_bRandomSprite)
			{
				pData[i].vArgument.z = floorf(m_pCCSprite->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x * static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y));
				pData[i].vArgument.z = std::clamp(pData[i].vArgument.z, 0.f, static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y - 1.f));
			}

			_float2	vScale		= _float2(m_pCCScale->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]));
			pData[i].vColor		= _float4(vScale.x, vScale.y, pData[i].vColor.z, pData[i].vColor.w);
			
			if (!m_bRotatePivot)
			{
				_float	fRotation	= m_vecInstRotation[i].x * (1.f - m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]) + m_vecInstRotation[i].y * (m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]);
				pData[i].vColor.z	= m_bSpeedBase ? pData[i].vColor.z + fRotation * _fTimeDelta : fRotation;
			}
			else
			{
				_vector vRotation		= m_pCCRotation->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]);
				_vector vQuaternion		= XMQuaternionRotationRollPitchYawFromVector(XMVector3Normalize(XMVector3TransformNormal(vRotation, m_mFetch)) * XMVectorGetW(vRotation) * _fTimeDelta);
				pData[i].vTranslation	= _float4(XMVector3Rotate(_float4(pData[i].vTranslation) - m_mFetch.row(3), vQuaternion) + m_mFetch.row(3));
			}

			pData[i].vTranslation	= _float4(pData[i].vTranslation) - _float4(0.f, g_fGravity, 0.f, 0.f) * m_pCCGravityScale->Get_Value(m_vecInstTimeAcc[i] / m_vecInstLifeTime[i]).x * _fTimeDelta;
			pData[i].vTranslation.w = 1.f;

			m_vecInstVelocity[i] = _float4(_float4(pData[i].vTranslation) - _float4(pData[i].vLook));
			if (!m_tData.bReverse)
			{
				pData[i].vArgument.w = m_tData.fVelocityTail * m_vecInstVelocity[i].length();	
			}
			else
			{
				pData[i].vArgument.w = m_tData.fVelocityTail * vScale.x;
			}
		}
	}
}
                                                                                                                                                                             
shared_ptr<CEffectInstance_ParticlePointDynamicCurve> CEffectInstance_ParticlePointDynamicCurve::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointDynamicCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointDynamicCurve::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticlePointDynamicCurve::Clone(any)
{
	shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pInstance = make_private_shared_copy(CEffectInstance_ParticlePointDynamicCurve, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointDynamicCurve::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticlePointDynamicCurve> CEffectInstance_ParticlePointDynamicCurve::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticlePointDynamicCurve> pInstance = make_private_shared(CEffectInstance_ParticlePointDynamicCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointDynamicCurve::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointDynamicCurve::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointDynamicCurve::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticlePointDynamicCurve::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pMesh)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		m_tData.pMesh->Export(outFile, MODEL::NONANIM);
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Export", "Failed to CTexture::Export");
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

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bLocal),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bReverse),			sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vLocalSpaceMax),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vLocalSpaceMin),		sizeof(_float3));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpawnRange),			sizeof(_float3));
	
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
	m_pCCSprite->Export(outFile);
	m_pCCRotation->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointDynamicCurve::Export", "Failed to Write File: CEffectInstance_ParticlePointDynamicCurve");
	}

	outFile.close();

	return S_OK;
}
#endif
