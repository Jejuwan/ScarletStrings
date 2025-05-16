#include "ClientPCH.h"
#include "EffectInstance_ParticlePointTarget.h"
#include "GameInstance.h"

CEffectInstance_ParticlePointTarget::CEffectInstance_ParticlePointTarget(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CEffectInstance(_pDevice, _pContext, EFFECT::VFX_ParticlePointTarget)
{
}

CEffectInstance_ParticlePointTarget::CEffectInstance_ParticlePointTarget(const CEffectInstance_ParticlePointTarget& _rhs)
    : CEffectInstance	(_rhs)
	, m_tData			(_rhs.m_tData)
{
}

HRESULT CEffectInstance_ParticlePointTarget::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointTarget::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePoint::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_ParticlePointTarget::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_tData.fSpeed += _fTimeDelta * 5.f;

	m_bRelease = m_fTimeAcc >= m_tData.fStartGap + m_tData.fInstanceLifeTime;

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

void CEffectInstance_ParticlePointTarget::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_ParticlePointTarget::Render()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_RawValue: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_IntArray(SHADER_INSTANCEINDEX, m_vecSpriteIndex.data(), m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_IntArray: SHADER_INSTANCEINDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointTarget::Render_Bloom()
{
//	MATERIALDESC	tOriginalMaterial	= m_tMaterialDesc;
//	_float			fOriginalBloom		= m_tData.fBloom;
//	std::swap(m_tMaterialDesc.vDiffuse.w, m_tData.fBloom);
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to Bind_RawValue: SHADER_SPRITESHEET_SIZE");
	}
	if (FAILED(m_pShader->Bind_IntArray(SHADER_INSTANCEINDEX, m_vecSpriteIndex.data(), m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to Bind_IntArray: SHADER_INSTANCEINDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Render_Bloom", "Failed to CEffectInstance::Render");
	}

//	m_tMaterialDesc	= tOriginalMaterial;
//	m_tData.fBloom	= fOriginalBloom;

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointTarget::Fetch(any _pObject)
{
	if (_pObject.has_value())
	{
		if (_pObject.type() == typeid(shared_ptr<CGameObject>))
		{
			shared_ptr<CGameObject> pObject = any_cast<shared_ptr<CGameObject>>(_pObject);
			m_pTargetTransform = pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			m_pTargetModel = pObject->Get_Component<CModel>(COMPONENT::MODEL);
		}
#if ACTIVATE_TOOL
		else
		{
			pair<shared_ptr<CTransform>, shared_ptr<CModel>> pArg = any_cast<pair<shared_ptr<CTransform>, shared_ptr<CModel>>>(_pObject);
			m_pTargetTransform = pArg.first;
			m_pTargetModel = pArg.second;
		}
#endif
		m_pTransform->Set_Matrix(m_pTargetTransform->Get_Matrix());
	//	m_pTransform->LookTo(m_pTargetTransform->Get_State(TRANSFORM::LOOK));
	//	m_pTransform->Set_State(TRANSFORM::POSITION, m_pTargetTransform->Get_State(TRANSFORM::POSITION));
		m_pTransform->Set_Scale(_float3(1.f, 1.f, 1.f));
		
		m_pBoneMatrices = Function::CreateDynamicArray<_float4x4>(g_iMaxBones * m_pTargetModel->Get_NumMeshes());
		for (_uint i = 0; i < m_pTargetModel->Get_NumMeshes(); ++i)
		{
			m_pTargetModel->Copy_BoneMatrices(i, m_pBoneMatrices.get() + i * g_iMaxBones);
		}
	}

	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Fetch", "Failed to CEffectInstance::Fetch");
	}

	return S_OK;
}

_bool CEffectInstance_ParticlePointTarget::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_ParticlePointTarget::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Release", "Failed to CGameObject::Release");
	}

	m_pTargetTransform	= nullptr;
	m_pTargetModel		= nullptr;
	m_pBoneMatrices		= nullptr;
	m_tData.fSpeed		= -.5f;

	m_vecInstDescs.clear();
	m_vecRotation.clear();
	m_vecSpriteIndex.clear();

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointTarget::Initialize(std::ifstream& _inFile)
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInstanceLifeTime), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fStartGap), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fSpeed), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fVelocityScale), sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fTheta), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fPhi), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vForceReference), sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vHorizontalScope), sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vVerticalScope), sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vRotationScope), sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bGravity), sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bMaterialFade), sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vMaterialFadeScope), sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fMaterialFadeWeight), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bApplyAlpha), sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance), sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSize), sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpriteSheetSize), sizeof(_int2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass), sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass), sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances), sizeof(_uint));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

HRESULT CEffectInstance_ParticlePointTarget::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Ready_Components", "Failed to CEffectInstance::Ready_Components");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_ParticlePointTarget::Set_Data(const VFX_ParticlePointTarget _tData)
{
	m_tData = _tData;
	m_iActivateInstances = _tData.iMaxInstance;
}
#endif

void CEffectInstance_ParticlePointTarget::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	m_vecInstDescs.resize(m_tData.iMaxInstance);
	m_vecRotation.resize(m_tData.iMaxInstance);
	m_vecSpriteIndex.resize(m_tData.iMaxInstance);
	
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstDescs[i] = Instance_End(i);
		pData[i].vRight = m_vecInstDescs[i].mTransformation.row(0);
		pData[i].vUp = m_vecInstDescs[i].mTransformation.row(1);
		pData[i].vLook = m_vecInstDescs[i].mTransformation.row(2);
		pData[i].vTranslation = m_vecInstDescs[i].mTransformation.row(3);
	}
}

void CEffectInstance_ParticlePointTarget::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		_float fRatio = m_vecInstDescs[i].fTimeAcc / m_tData.fInstanceLifeTime;
#pragma region LifeTime
		if (Function::InRange(m_vecInstDescs[i].fTimeAcc, 0.f, _fTimeDelta, "[)"))
		{
			m_vecInstDescs[i] = Instance_Restart(i);
		}
		else if (m_vecInstDescs[i].fTimeAcc >= m_tData.fInstanceLifeTime)
		{
		//	m_vecInstDescs[i] = Instance_End(i);
		}
		else if (m_vecInstDescs[i].fTimeAcc < 0.f)
		{
			m_vecInstDescs[i].fTimeAcc += _fTimeDelta;
			continue;
		}
		m_vecInstDescs[i].fTimeAcc += _fTimeDelta;
#pragma endregion
#pragma region Velocity
#pragma region Gravity
		if (m_tData.bGravity)
		{
			m_vecInstDescs[i].vVelocity.y -= g_fGravity * m_tData.fVelocityScale * _fTimeDelta;
		}
#pragma endregion
		_float4 vTranslation = m_vecInstDescs[i].mTransformation.row(3);
		vTranslation += m_vecInstDescs[i].vVelocity * m_tData.fSpeed * _fTimeDelta;
		m_vecInstDescs[i].mTransformation = _float4x4(
			m_vecInstDescs[i].mTransformation.row(0),
			m_vecInstDescs[i].mTransformation.row(1),
			m_vecInstDescs[i].mTransformation.row(2),
			vTranslation);
#pragma endregion
#pragma region Rotation
		m_vecRotation[i] += m_vecInstDescs[i].fRotation * _fTimeDelta;
#pragma endregion
#pragma region Fade
		if (m_tData.bMaterialFade)
		{
			if (fRatio < m_tData.vMaterialFadeScope.x)
			{
				pData[i].vColor = _color(1.f, 1.f, 1.f, 1.f);
			}
			else if (fRatio < m_tData.vMaterialFadeScope.y)
			{
				pData[i].vColor = _color(1.f, 1.f, 1.f, 1.f)
					* (1.f - powf(Function::ProportionalRatio(m_tData.vMaterialFadeScope.x, m_tData.vMaterialFadeScope.y, fRatio), m_tData.fMaterialFadeWeight));
			}
			else
			{
				pData[i].vColor = _color(0.f, 0.f, 0.f, 0.f);
			}
			if (!m_tData.bApplyAlpha)
			{
				pData[i].vColor.w = 1.f;
			}
		}
		else
		{
			pData[i].vColor = _color(1.f, 1.f, 1.f, 1.f);
		}
#pragma endregion
		m_vecSpriteIndex[i]		= static_cast<_int>(std::floorf(std::clamp(fRatio, 0.f, 1.f) * m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y));

		pData[i].vRight			= m_vecInstDescs[i].mTransformation.row(0);
		pData[i].vUp			= m_vecInstDescs[i].mTransformation.row(1);
		pData[i].vLook			= m_vecInstDescs[i].mTransformation.row(2);
		pData[i].vTranslation	= m_vecInstDescs[i].mTransformation.row(3);

		pData[i].vTranslation	= (m_pTransform->Get_Matrix() * m_vecInstDescs[i].mTransformation).row(3);

		pData[i].vArgument		= _float4(m_tData.vSize.x, m_tData.vSize.y, static_cast<_float>(m_vecSpriteIndex[i]), m_vecRotation[i]);
	}
}

const INSTDESC CEffectInstance_ParticlePointTarget::Instance_Restart(_uint _iIndex)
{
//	_uint iVertexCount = 0;
//	for (_uint i = 0; i < m_pTargetModel->Get_NumMeshes(); ++i)
//	{
//		iVertexCount += m_pTargetModel->Get_Mesh(i)->Get_NumVertices();
//	}
//	_uint iPick = rand() % iVertexCount;
//
//	_uint iIndex = 0;
//	_float3 vVertex;
//	m_pTargetModel->Iterate_Meshes(
//		[&](shared_ptr<CMesh> _pMesh)->_bool
//		{
//			_pMesh->Iterate_Polygons(
//				[&](POLYGON arr)->_bool
//				{
//					if (iPick < iIndex + 3)
//					{
//						vVertex = arr[rand() % 3];
//						return false;
//					}
//					++iIndex;
//					return true;
//				}
//			);
//
//			return true;
//		}
//	);

	INSTDESC tInstDesc{};

	_uint		iMeshIdx		= rand() % m_pTargetModel->Get_NumMeshes();
	_float3		vVertex			= m_pTargetModel->Get_Mesh(iMeshIdx)->Get_RandomVertex(MODEL::ANIM);
	//_float3		vVertex = m_pTargetModel->Get_Mesh(iMeshIdx)->Get_RandomVertex(m_pTargetModel->Get_NumAnimations() == 0 ? MODEL::NONANIM : MODEL::ANIM);

	_float4x4	mMatrix			= g_mUnit;
	_float		fAngle			= Function::RandomFloat(0, XM_2PI);
	_float		fHorizontal		= Function::RandomFloat(m_tData.vHorizontalScope.x, m_tData.vHorizontalScope.y);
	_float		fVertical		= Function::RandomFloat(m_tData.vVerticalScope.x, m_tData.vVerticalScope.y);

	mMatrix.m[3][0] = vVertex.x + cosf(fAngle) * fHorizontal;
	mMatrix.m[3][1] = vVertex.y + (rand() % 2) * fVertical;
	mMatrix.m[3][2] = vVertex.z + sinf(fAngle) * fHorizontal;

	tInstDesc.mTransformation	= mMatrix;
	tInstDesc.vVelocity			= Function::Cone_DiffusionAngle(m_tData.vForceReference, m_tData.fTheta, m_tData.fPhi);
	tInstDesc.fRotation			= Function::RandomFloat(m_tData.vRotationScope.x, m_tData.vRotationScope.y);
	tInstDesc.vArgument			= _float4{};

	m_vecRotation[_iIndex]		= 0.f;

	return tInstDesc;
}

const INSTDESC CEffectInstance_ParticlePointTarget::Instance_End(_uint _iIndex)
{
	INSTDESC tInstDesc{};
	tInstDesc.mTransformation = _float4x4{};
	tInstDesc.fTimeAcc = Function::RandomFloat(-m_tData.fStartGap, 0.f);
	return tInstDesc;
}

shared_ptr<CEffectInstance_ParticlePointTarget> CEffectInstance_ParticlePointTarget::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_ParticlePointTarget> pInstance = make_private_shared(CEffectInstance_ParticlePointTarget, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointTarget::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_ParticlePointTarget::Clone(any)
{
    shared_ptr<CEffectInstance_ParticlePointTarget> pInstance = make_private_shared_copy(CEffectInstance_ParticlePointTarget, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointTarget::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_ParticlePointTarget> CEffectInstance_ParticlePointTarget::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_ParticlePointTarget> pInstance = make_private_shared(CEffectInstance_ParticlePointTarget, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointTarget::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointTarget::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_ParticlePointTarget::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_ParticlePointTarget::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInstanceLifeTime),	sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fStartGap),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fSpeed),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fVelocityScale),		sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fTheta),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fPhi),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vForceReference),		sizeof(_float3));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vHorizontalScope),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vVerticalScope),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vRotationScope),		sizeof(_float2));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bGravity),			sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bMaterialFade),		sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vMaterialFadeScope),	sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fMaterialFadeWeight),	sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bApplyAlpha),			sizeof(_bool));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSize),				sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize),	sizeof(_int2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances),		sizeof(_uint));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_ParticlePointTarget::Export", "Failed to Write File: CEffectInstance_ParticlePointTarget");
	}

	outFile.close();

	return S_OK;
}
#endif
