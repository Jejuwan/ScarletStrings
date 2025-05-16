#include "ClientPCH.h"
#include "EffectRegular_VisualMesh.h"
#include "GameInstance.h"
#include "Bone.h"
#include "CurveContainer.h"
#include "Curves.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

CEffectRegular_VisualMesh::CEffectRegular_VisualMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectRegular(_pDevice, _pContext, EFFECT::VFX_VisualMesh)
{
}

CEffectRegular_VisualMesh::CEffectRegular_VisualMesh(const CEffectRegular_VisualMesh& _rhs)
	: CEffectRegular(_rhs)
	, m_tData(_rhs.m_tData)
{
	if (_rhs.m_pCCScale)		m_pCCScale = _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCTranslation)	m_pCCTranslation = _rhs.m_pCCTranslation->Clone();
	if (_rhs.m_pCCBloom)		m_pCCBloom = _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)			m_pCCFade = _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCDiffuse)		m_pCCDiffuse = _rhs.m_pCCDiffuse->Clone();
	if (_rhs.m_pCCEmissive)		m_pCCEmissive = _rhs.m_pCCEmissive->Clone();
	if (_rhs.m_pCCDistortion)	m_pCCDistortion = _rhs.m_pCCDistortion->Clone();
	if (_rhs.m_pCCRotation)		m_pCCRotation = _rhs.m_pCCRotation->Clone();
	if (_rhs.m_pCCSprite)		m_pCCSprite = _rhs.m_pCCSprite->Clone();
}

HRESULT CEffectRegular_VisualMesh::Initialize_Prototype()
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
	m_pCCDistortion		= CCurveContainer::Create();
	m_pCCRotation		= CCurveContainer::Create();
	m_pCCSprite			= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectRegular_VisualMesh::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CEffectRegular::Initialize");
	}

	return S_OK;
}

void CEffectRegular_VisualMesh::Tick(_float _fTimeDelta)
{
	_float4 vTranslation = m_pCCTranslation->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
	vTranslation = _float4(_float3(vTranslation) * vTranslation.w, 1.f);

	if (m_pBone)
	{
		m_pTransform->Set_Matrix(g_mUnit);
		m_pTransform->Multiply(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_pTransform->Set_Matrix(XMMatrixRotationRollPitchYawFromVector(m_tData.vRollPitchYaw) * m_pTransform->Get_Matrix());

		if (m_tData.bSRT)
		{
			_float4 vRotation = m_pCCRotation->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
			vRotation *= vRotation.w ? vRotation.w * _fTimeDelta : 1.f;
			_vector vQuatX = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(1.f, 0.f, 0.f) : m_pTransform->Get_State(TRANSFORM::RIGHT), vRotation.x);
			_vector vQuatY = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 1.f, 0.f) : m_pTransform->Get_State(TRANSFORM::UP), vRotation.y);
			_vector vQuatZ = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 0.f, 1.f) : m_pTransform->Get_State(TRANSFORM::LOOK), vRotation.z);
			_vector vQuat = XMQuaternionMultiply(vQuatX, XMQuaternionMultiply(vQuatY, vQuatZ));
			m_pTransform->Rotate(vQuat);

			m_pTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
				+ m_pTransform->Get_State(TRANSFORM::RIGHT) * vTranslation.x
				+ m_pTransform->Get_State(TRANSFORM::UP) * vTranslation.y
				+ m_pTransform->Get_State(TRANSFORM::LOOK) * vTranslation.z);
		}
		else
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
				+ m_pTransform->Get_State(TRANSFORM::RIGHT) * vTranslation.x
				+ m_pTransform->Get_State(TRANSFORM::UP) * vTranslation.y
				+ m_pTransform->Get_State(TRANSFORM::LOOK) * vTranslation.z);

			_float4 vRotation = m_pCCRotation->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
			vRotation *= vRotation.w ? vRotation.w * _fTimeDelta : 1.f;
			_vector vQuatX = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(1.f, 0.f, 0.f) : m_pTransform->Get_State(TRANSFORM::RIGHT), vRotation.x);
			_vector vQuatY = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 1.f, 0.f) : m_pTransform->Get_State(TRANSFORM::UP), vRotation.y);
			_vector vQuatZ = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 0.f, 1.f) : m_pTransform->Get_State(TRANSFORM::LOOK), vRotation.z);
			_vector vQuat = XMQuaternionMultiply(vQuatX, XMQuaternionMultiply(vQuatY, vQuatZ));
			m_pTransform->Rotate(vQuat);
		}
	}
	else
	{
		_float4 vRotation = m_pCCRotation->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
		vRotation *= vRotation.w ? vRotation.w * _fTimeDelta : 1.f;
		_vector vQuatX = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(1.f, 0.f, 0.f) : m_pTransform->Get_State(TRANSFORM::RIGHT), vRotation.x);
		_vector vQuatY = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 1.f, 0.f) : m_pTransform->Get_State(TRANSFORM::UP), vRotation.y);
		_vector vQuatZ = XMQuaternionRotationAxis(m_tData.bRotateWorldAxis ? _float3(0.f, 0.f, 1.f) : m_pTransform->Get_State(TRANSFORM::LOOK), vRotation.z);
		_vector vQuat = XMQuaternionMultiply(vQuatX, XMQuaternionMultiply(vQuatY, vQuatZ));
		m_pTransform->Rotate(vQuat);

		m_pTransform->Set_State(TRANSFORM::POSITION, m_vFetchPosition
			+ vTranslation.x * m_pTransform->Get_State(TRANSFORM::RIGHT)
			+ vTranslation.y * m_pTransform->Get_State(TRANSFORM::UP)
			+ vTranslation.z * m_pTransform->Get_State(TRANSFORM::LOOK));
	}

	m_pTransform->Set_Scale(m_pCCScale->Get_Value(m_fTimeAcc / m_tData.fLifeTime));

	__super::Tick(_fTimeDelta);

	m_fFadeRatio = m_pCCFade->Get_Value(m_fTimeAcc / m_tData.fLifeTime).x;
	m_fBloomRatio = m_pCCBloom->Get_Value(m_fTimeAcc / m_tData.fLifeTime).x;

	m_tMaterialDesc.vDiffuse = m_pCCDiffuse->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
	m_tMaterialDesc.vEmissive = m_pCCEmissive->Get_Value(m_fTimeAcc / m_tData.fLifeTime);

	if (m_tData.bBillboard)
	{
		m_pTransform->LookAt(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION));
		m_pTransform->Rotate(TRANSFORM::RIGHT, XM_PIDIV2);
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

void CEffectRegular_VisualMesh::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_fBloomRatio)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectRegular_VisualMesh::Render()
{
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
	if (m_tData.pMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXMASK, m_tData.pMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXMASK");
		}
	}
	if (m_tData.pExtraMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXSPECULAR;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXEXTRAMASK, m_tData.pExtraMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEXTRAMASK");
		}
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_fFadeRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_pCCDistortion->Get_Value(m_fTimeAcc / m_tData.fLifeTime).x)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	if (FAILED(m_pShader->Bind_RawValue("g_bReverseGrayScale0", &m_tData.bReverseGrayScale0, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue("g_bReverseGrayScale1", &m_tData.bReverseGrayScale1, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float2 vUVOffset	= m_tData.vUVOffset		+ m_tData.vUVOffsetSpeed	* m_fTimeAcc;
	_float2 vUVOffset1	= m_tData.vUVOffset1	+ m_tData.vUVOffsetSpeed1	* m_fTimeAcc;
	_float2 vUVOffset2	= m_tData.vUVOffset2	+ m_tData.vUVOffsetSpeed2	* m_fTimeAcc;
	_float2 vUVOffset3	= m_tData.vUVOffset3	+ m_tData.vUVOffsetSpeed3	* m_fTimeAcc;
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET, &vUVOffset, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET1, &vUVOffset1, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET2, &vUVOffset2, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET3, &vUVOffset3, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_DIFFUSE, &m_tData.vDiffuseTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_EMISSIVE, &m_tData.vEmissiveTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_SPECULAR, &m_tData.vSpecularTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_NORMAL, &m_tData.vMaskTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectRegular_VisualMesh::Render_Bloom()
{
//	MATERIALDESC	tOriginalMaterial	= m_tMaterialDesc;
//	_float			fOriginalBloom		= m_fBloomRatio;
//	std::swap(m_tMaterialDesc.vDiffuse.w, m_fBloomRatio);

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
	if (m_tData.pMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXMASK, m_tData.pMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXMASK");
		}
	}
	if (m_tData.pExtraMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXSPECULAR;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXEXTRAMASK, m_tData.pExtraMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEXTRAMASK");
		}
	}

 	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_fBloomRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_fFadeRatio)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	if (FAILED(m_pShader->Bind_RawValue("g_bReverseGrayScale0", &m_tData.bReverseGrayScale0, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue("g_bReverseGrayScale1", &m_tData.bReverseGrayScale1, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	_float2 vUVOffset	= m_tData.vUVOffset		+ m_tData.vUVOffsetSpeed	* m_fTimeAcc;
	_float2 vUVOffset1	= m_tData.vUVOffset1	+ m_tData.vUVOffsetSpeed1	* m_fTimeAcc;
	_float2 vUVOffset2	= m_tData.vUVOffset2	+ m_tData.vUVOffsetSpeed2	* m_fTimeAcc;
	_float2 vUVOffset3	= m_tData.vUVOffset3	+ m_tData.vUVOffsetSpeed3	* m_fTimeAcc;
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET, &vUVOffset, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET1, &vUVOffset1, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET2, &vUVOffset2, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_UVOFFSET3, &vUVOffset3, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_DIFFUSE, &m_tData.vDiffuseTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_EMISSIVE, &m_tData.vEmissiveTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_SPECULAR, &m_tData.vSpecularTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_TILING2_NORMAL, &m_tData.vMaskTiling, sizeof(_float2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to Bind_RawValue");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Render", "Failed to CEffectRegular::Render");
	}

//	m_tMaterialDesc	= tOriginalMaterial;
//	m_fBloomRatio	= fOriginalBloom;

	return S_OK;
}

HRESULT CEffectRegular_VisualMesh::Fetch(any _arg)
{
	m_pTransform->Set_Matrix(g_mUnit);

	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Fetch", "Failed to CEffectRegular::Fetch");
	}

	if (_arg.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
	{
		pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_arg);
		shared_ptr<CGameObject> pObject = arg.first;
		shared_ptr<CModel> pModel = pObject->Get_Component<CModel>(COMPONENT::MODEL);
		m_pTarget = pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		m_mPivot = pModel->Get_Pivot();
		m_pBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();
	}

	_float3 vPosition = m_pTransform->Get_State(TRANSFORM::POSITION);
	_float4x4 m = m_pTransform->Get_Matrix();
	m_pTransform->Set_Matrix(XMMatrixRotationRollPitchYawFromVector(m_tData.vRollPitchYaw) * m);

	m_fFadeRatio		= 0.f;
	m_vFetchPosition	= m_pTransform->Get_State(TRANSFORM::POSITION);

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		m_mFetch = any_cast<_float4x4>(_arg);

		if (FAILED(Add_Component(COMPONENT::MESH, m_tData.pMesh)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to Add_Component");
		}
	}
#endif

	return S_OK;
}

_bool CEffectRegular_VisualMesh::Return()
{
	return m_bRelease;
}

HRESULT CEffectRegular_VisualMesh::Release()
{
	m_pTarget = nullptr;
	m_mPivot = g_mUnit;
	m_pBone = nullptr;

	return S_OK;
}

HRESULT CEffectRegular_VisualMesh::Initialize(std::ifstream& _inFile)
{
	m_tData.pMesh = CMesh::Read(m_pDevice, m_pContext, MODEL::NONANIM, _inFile, g_mUnit);
	if (nullptr == m_tData.pMesh)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CMesh::Read");
	}

	if (FAILED(Add_Component(COMPONENT::MESH, m_tData.pMesh)))
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to Add_Component");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}
	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pMask = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pMask)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
	}
	_inFile.read(reinterpret_cast<_byte*>(&bTexture), sizeof(_bool));
	if (bTexture)
	{
		m_tData.pExtraMask = CTexture::Read(m_pDevice, m_pContext, _inFile);
		if (nullptr == m_tData.pExtraMask)
		{
			_inFile.clear();
			_inFile.close();
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXSPECULAR;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vRollPitchYaw),		sizeof(_float3));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffsetSpeed),		sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffsetSpeed1),	sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffsetSpeed2),	sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffsetSpeed3),	sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffset),			sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffset1),			sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffset2),			sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vUVOffset3),			sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vDiffuseTiling),		sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vEmissiveTiling),	sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpecularTiling),	sizeof(_float2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vMaskTiling),		sizeof(_float2));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bMaskAlpha),			sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bExtraMaskAlpha),	sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bReverseGrayScale0),	sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bReverseGrayScale1),	sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bSRT),				sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bRotateWorldAxis),	sizeof(_bool));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bBillboard),			sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	m_pCCScale			= CCurveContainer::Read(_inFile);
	m_pCCTranslation	= CCurveContainer::Read(_inFile);
	m_pCCBloom			= CCurveContainer::Read(_inFile);
	m_pCCFade			= CCurveContainer::Read(_inFile);
	m_pCCDiffuse		= CCurveContainer::Read(_inFile);
	m_pCCEmissive		= CCurveContainer::Read(_inFile);
	m_pCCDistortion		= CCurveContainer::Read(_inFile);
	m_pCCRotation		= CCurveContainer::Read(_inFile);
	m_pCCSprite			= CCurveContainer::Read(_inFile);

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_Visual::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectRegular_VisualMesh::Set_Bone(shared_ptr<CTransform> _pTransform, shared_ptr<CModel> _pModel, shared_ptr<CBone> _pBone)
{
	m_pTarget = _pTransform;
	m_mPivot = _pModel ? _pModel->Get_Pivot() : _float4x4(g_mUnit);
	m_pBone = _pBone ? _pBone->Get_CombinedTransformationPointer() : nullptr;
}

void CEffectRegular_VisualMesh::Set_Data(const VFX_VisualMesh _tData)
{
	if (m_tData.pMesh != _tData.pMesh)
	{
		Delete_Component(COMPONENT::MESH);
		CGameObject::Add_Component(COMPONENT::MESH, _tData.pMesh);
	}

	if (_tData.pDiffuse)
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
	else
		m_iShaderFlag &= ~SHADER_FLAG_TEXDIFFUSE;
	if (_tData.pEmission)
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	else
		m_iShaderFlag &= ~SHADER_FLAG_TEXEMISSIVE;
	if (_tData.pMask)
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
	else
		m_iShaderFlag &= ~SHADER_FLAG_TEXNORMAL;
	if (_tData.pExtraMask)
		m_iShaderFlag |= SHADER_FLAG_TEXSPECULAR;
	else
		m_iShaderFlag &= ~SHADER_FLAG_TEXSPECULAR;

	m_tData = _tData;
}
#endif

HRESULT CEffectRegular_VisualMesh::Ready_Components()
{
	if (FAILED(CGameObject::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Ready_Components", "Failed to CEffect::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Ready_Components", "Nullptr Exception: m_pShader");
	}
	
	return S_OK;
}

HRESULT CEffectRegular_VisualMesh::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (FAILED(__super::Add_Component(_eComponent, COMPONENT::MESH == _eComponent ? m_tData.pMesh : _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Ready_Components", "Failed to CGameObject::Add_Component");
	}

	return S_OK;
}

shared_ptr<CEffectRegular_VisualMesh> CEffectRegular_VisualMesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectRegular_VisualMesh> pInstance = make_private_shared(CEffectRegular_VisualMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualMesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectRegular_VisualMesh::Clone(any)
{
	shared_ptr<CEffectRegular_VisualMesh> pInstance = make_private_shared_copy(CEffectRegular_VisualMesh, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualMesh::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectRegular_VisualMesh> CEffectRegular_VisualMesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectRegular_VisualMesh> pInstance = make_private_shared(CEffectRegular_VisualMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualMesh::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualMesh::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualMesh::Read", "Failed to Initialize");
	}

	return pInstance;
}

#ifdef _DEBUG
HRESULT CEffectRegular_VisualMesh::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	m_tData.pMesh->Export(outFile, MODEL::NONANIM);

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if (m_tData.pMask)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pMask->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}
	if (m_tData.pExtraMask)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pExtraMask->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vRollPitchYaw),		sizeof(_float3));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffsetSpeed),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffsetSpeed1),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffsetSpeed2),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffsetSpeed3),		sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffset),			sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffset1),			sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffset2),			sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vUVOffset3),			sizeof(_float2));
	
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vDiffuseTiling),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vEmissiveTiling),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpecularTiling),		sizeof(_float2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vMaskTiling),			sizeof(_float2));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bMaskAlpha),			sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bExtraMaskAlpha),		sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bReverseGrayScale0),	sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bReverseGrayScale1),	sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bSRT),				sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bRotateWorldAxis),	sizeof(_bool));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bBillboard),			sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	m_pCCScale->Export(outFile);
	m_pCCTranslation->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCDiffuse->Export(outFile);
	m_pCCEmissive->Export(outFile);
	m_pCCDistortion->Export(outFile);
	m_pCCRotation->Export(outFile);
	m_pCCSprite->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualMesh::Export", "Failed to Write File: CEffectRegular_VisualMesh");
	}

	outFile.close();

	return S_OK;
}
#endif
