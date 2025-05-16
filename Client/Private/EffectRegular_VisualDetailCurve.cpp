#include "ClientPCH.h"
#include "EffectRegular_VisualDetailCurve.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectRegular_VisualDetailCurve::CEffectRegular_VisualDetailCurve(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectRegular(_pDevice, _pContext, EFFECT::VFX_VisualDetailCurve)
{
}

CEffectRegular_VisualDetailCurve::CEffectRegular_VisualDetailCurve(const CEffectRegular_VisualDetailCurve& _rhs)
	: CEffectRegular(_rhs)
	, m_tData(_rhs.m_tData)
{
	if (_rhs.m_pCCScale)		m_pCCScale			= _rhs.m_pCCScale->Clone();
	if (_rhs.m_pCCBloom)		m_pCCBloom			= _rhs.m_pCCBloom->Clone();
	if (_rhs.m_pCCFade)			m_pCCFade			= _rhs.m_pCCFade->Clone();
	if (_rhs.m_pCCSprite)		m_pCCSprite			= _rhs.m_pCCSprite->Clone();
	if (_rhs.m_pCCDistortion)	m_pCCDistortion		= _rhs.m_pCCDistortion->Clone();
	if (_rhs.m_pCCDiffuse)		m_pCCDiffuse		= _rhs.m_pCCDiffuse->Clone();
	if (_rhs.m_pCCEmissive)		m_pCCEmissive		= _rhs.m_pCCEmissive->Clone();
	if (_rhs.m_pCCTranslation)	m_pCCTranslation	= _rhs.m_pCCTranslation->Clone();
	if (_rhs.m_pCCRotation)		m_pCCRotation		= _rhs.m_pCCRotation->Clone();
}

HRESULT CEffectRegular_VisualDetailCurve::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEXVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	m_pCCScale			= CCurveContainer::Create();
	m_pCCBloom			= CCurveContainer::Create();
	m_pCCFade			= CCurveContainer::Create();
	m_pCCSprite			= CCurveContainer::Create();
	m_pCCDistortion		= CCurveContainer::Create();
	m_pCCDiffuse		= CCurveContainer::Create();
	m_pCCEmissive		= CCurveContainer::Create();
	m_pCCTranslation	= CCurveContainer::Create();
	m_pCCRotation		= CCurveContainer::Create();

	return S_OK;
}

HRESULT CEffectRegular_VisualDetailCurve::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Initialize", "Failed to CEffectRegular::Initialize");
	}

	return S_OK;
}

void CEffectRegular_VisualDetailCurve::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	_float4 vTranslation = m_pCCTranslation->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
	vTranslation = _float4(_float3(vTranslation) * vTranslation.w, 0.f);
	if (m_pBone)
	{
		if (m_tData.bLocal)
		{
		//	m_pTransform->Set_State(TRANSFORM::POSITION,
		//		XMVector3TransformCoord(vTranslation, *m_pBone * m_mPivot * m_pTarget->Get_Matrix() * m_mOffset));
			m_pTransform->Set_Matrix(g_mUnit);
			m_pTransform->Rotate(m_vQuat);
			m_pTransform->Multiply(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
			m_pTransform->Set_State(TRANSFORM::POSITION, m_pTransform->Get_State(TRANSFORM::POSITION)
				+ vTranslation.x * m_pTransform->Get_State(TRANSFORM::RIGHT)
				+ vTranslation.y * m_pTransform->Get_State(TRANSFORM::UP)
				+ vTranslation.z * m_pTransform->Get_State(TRANSFORM::LOOK));
		}
		else
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, m_vBasePosition
				+ vTranslation.x * m_pTransform->Get_State(TRANSFORM::RIGHT)
				+ vTranslation.y * m_pTransform->Get_State(TRANSFORM::UP)
				+ vTranslation.z * m_pTransform->Get_State(TRANSFORM::LOOK));
		}
	}
	else
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, m_vBasePosition
			+ vTranslation.x * m_pTransform->Get_State(TRANSFORM::RIGHT)
			+ vTranslation.y * m_pTransform->Get_State(TRANSFORM::UP)
			+ vTranslation.z * m_pTransform->Get_State(TRANSFORM::LOOK));
	}
	
	m_pTransform->Set_Scale(_float3(m_pCCScale->Get_Value(m_pCCScale->Get_Total() * m_fTimeAcc / m_tData.fLifeTime)) * m_vBaseScale);

	_float4 vRotation = m_pCCRotation->Get_Value(m_fTimeAcc / m_tData.fLifeTime) * _fTimeDelta;
//	m_pTransform->Rotate(XMQuaternionRotationRollPitchYaw(vRotation.x * _fTimeDelta, vRotation.y * _fTimeDelta, vRotation.z * _fTimeDelta));
	_vector vQuatX	= XMQuaternionRotationAxis(m_pTransform->Get_State(TRANSFORM::RIGHT),	vRotation.x);
	_vector vQuatY	= XMQuaternionRotationAxis(m_pTransform->Get_State(TRANSFORM::UP),		vRotation.y);
	_vector vQuatZ	= XMQuaternionRotationAxis(m_pTransform->Get_State(TRANSFORM::LOOK),	vRotation.z);
	_vector vQuat	= XMQuaternionMultiply(vQuatX, XMQuaternionMultiply(vQuatY, vQuatZ));
	m_pTransform->Rotate(vQuat);

	m_tMaterialDesc.vDiffuse = m_pCCDiffuse->Get_Value(m_fTimeAcc / m_tData.fLifeTime);
	m_tMaterialDesc.vEmissive = m_pCCEmissive->Get_Value(m_fTimeAcc / m_tData.fLifeTime);

	m_pTransform->Translate(m_vOffset);

//	if (m_pTarget)
//	{
//		m_pTransform->Set_State(TRANSFORM::POSITION, m_pTarget->Get_State(TRANSFORM::POSITION));
//	}

	m_bRelease = m_fTimeAcc > m_tData.fLifeTime;
	
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

void CEffectRegular_VisualDetailCurve::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	Add_RenderObject(RENDER_GROUP::BLOOM);
}

HRESULT CEffectRegular_VisualDetailCurve::Render()
{
	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (m_tData.pMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXMASK, m_tData.pMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXMASK");
		}
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, std::clamp(
	static_cast<_int>(floorf(m_pCCSprite->Get_Value(m_pCCSprite->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x *
	static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y))),
	0, m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y - 1)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_pCCFade->Get_Value(m_pCCFade->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_pCCBloom->Get_Value(m_pCCBloom->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_pCCDistortion->Get_Value(m_pCCDistortion->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Float: SHADER_DISTORTION_POWER");
	}

	_float fDifTiling[8], fEmsTiling[8];
	std::fill(fDifTiling, fDifTiling + 8, m_tData.fDiffuseTiling);
	std::fill(fEmsTiling, fEmsTiling + 8, m_tData.fEmissiveTiling);
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fDifTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_FloatArray");
	}
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_EMISSIVE, fEmsTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_FloatArray");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to CEffectRegular::Render");
	}

	return S_OK;
}

HRESULT CEffectRegular_VisualDetailCurve::Render_Bloom()
{
//	MATERIALDESC	tOriginalMaterial	= m_tMaterialDesc;
//	m_tMaterialDesc.vDiffuse.w			= m_pCCBloom->Get_Value(m_fTimeAcc).x;

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}
	if (m_tData.pMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
		if (FAILED(m_pShader->Bind_ShaderResourceView(SHADER_TEXMASK, m_tData.pMask->Get_ShaderResourceView())))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXMASK");
		}
	}
	if (FAILED(m_pShader->Bind_RawValue(SHADER_SPRITESHEET_SIZE, &m_tData.vSpriteSheetSize, sizeof(_int2))))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_IntArray: SHADER_SPRITESHEET_SIZE");
	}
	if FAILED(m_pShader->Bind_Int(SHADER_SPRITESHEET_INDEX, std::clamp(
	static_cast<_int>(floorf(m_pCCSprite->Get_Value(m_pCCSprite->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x *
	static_cast<_float>(m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y))),
	0, m_tData.vSpriteSheetSize.x * m_tData.vSpriteSheetSize.y - 1)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Int: SHADER_SPRITESHEET_INDEX");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_FADERATIO, m_pCCFade->Get_Value(m_pCCFade->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Float: SHADER_FADERATIO");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_pCCBloom->Get_Value(m_pCCBloom->Get_Total() * m_fTimeAcc / m_tData.fLifeTime).x)))
//	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, tOriginalMaterial.vDiffuse.w)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	_float fDifTiling[8], fEmsTiling[8];
	std::fill(fDifTiling, fDifTiling + 8, m_tData.fDiffuseTiling);
	std::fill(fEmsTiling, fEmsTiling + 8, m_tData.fEmissiveTiling);
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fDifTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_FloatArray");
	}
	if (FAILED(m_pShader->Bind_FloatArray(SHADER_TILING_EMISSIVE, fEmsTiling, 8)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to Bind_FloatArray");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Render", "Failed to CEffectRegular::Render");
	}

//	m_tMaterialDesc = tOriginalMaterial;

	return S_OK;
}

HRESULT CEffectRegular_VisualDetailCurve::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Fetch", "Failed to CEffectRegular::Fetch");
	}

	if (_arg.has_value() && _arg.type() == typeid(_float3))
	{
		_float3 vPosition = any_cast<_float3>(_arg);
		m_pTransform->Set_State(TRANSFORM::POSITION, vPosition);
		m_vBasePosition = vPosition;
		m_vBaseScale = _float3(1.f, 1.f, 1.f);
	}
	else if (_arg.has_value() && _arg.type() == typeid(shared_ptr<CTransform>))
	{
		m_pTarget = any_cast<shared_ptr<CTransform>>(_arg);
		m_vBaseScale = m_pTarget->Get_Scale();
		m_vBasePosition = m_pTarget->Get_State(TRANSFORM::POSITION);
		m_pTransform->Set_Matrix(m_pTarget->Get_Matrix());
	}
	else if (_arg.has_value() && _arg.type() == typeid(_float4x4))
	{
		_float4x4 m		= any_cast<_float4x4>(_arg);
		m_vBaseScale	= _float3(m.row(0).length(), m.row(1).length(), m.row(2).length());
		m_vBasePosition = _float3(m.row(3));
		m_pTransform->Set_Matrix(m);
	}
	else if (_arg.has_value() && _arg.type() == typeid(pair<shared_ptr<CGameObject>, const _char*>))
	{
		pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_arg);
		shared_ptr<CGameObject> pOwner = arg.first;
		m_pTarget = pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		shared_ptr<CModel> pModel = pOwner->Get_Component<CModel>(COMPONENT::MODEL);
		m_pBone = pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();
		m_mPivot = pModel->Get_Pivot();
		m_pTransform->Set_Matrix(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_vBasePosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_vBaseScale = _float3(1.f, 1.f, 1.f);
	}
	else if (_arg.has_value() && _arg.type() == typeid(tuple<shared_ptr<CGameObject>, const _char*, _float4>))
	{
		tuple<shared_ptr<CGameObject>, const _char*, _float4> arg = any_cast<tuple<shared_ptr<CGameObject>, const _char*, _float4>>(_arg);
		shared_ptr<CGameObject> pOwner = std::get<0>(arg);
		shared_ptr<CModel> pModel = pOwner->Get_Component<CModel>(COMPONENT::MODEL);
		m_pTarget = pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		m_mPivot = pModel->Get_Pivot();
		m_pBone = pModel->Get_Bone(std::get<1>(arg))->Get_CombinedTransformationPointer();
		m_vQuat = std::get<2>(arg);
		m_pTransform->Set_Matrix(g_mUnit);
		m_pTransform->Rotate(m_vQuat);
		m_pTransform->Multiply(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_vBasePosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_vBaseScale = _float3(1.f, 1.f, 1.f);
	}
	else if (_arg.has_value() && _arg.type() == typeid(tuple<shared_ptr<CGameObject>, _float4x4, _float4>))
	{
		tuple<shared_ptr<CGameObject>, _float4x4, _float4> arg = any_cast<tuple<shared_ptr<CGameObject>, _float4x4, _float4>>(_arg);
		shared_ptr<CGameObject> pOwner = std::get<0>(arg);
		shared_ptr<CModel> pModel = pOwner->Get_Component<CModel>(COMPONENT::MODEL);
		m_pTarget = pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		m_mPivot = pModel->Get_Pivot();
		m_mBone = std::get<1>(arg);
		m_pBone = &m_mBone;
		m_vQuat = std::get<2>(arg);
		m_pTransform->Set_Matrix(g_mUnit);
		m_pTransform->Rotate(m_vQuat);
		m_pTransform->Multiply(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_vBasePosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_vBaseScale = _float3(1.f, 1.f, 1.f);
	}
	else if (_arg.has_value() && _arg.type() == typeid(tuple<shared_ptr<CGameObject>, _float4x4, _float4, _float3>))
	{
		tuple<shared_ptr<CGameObject>, _float4x4, _float4, _float3> arg = any_cast<tuple<shared_ptr<CGameObject>, _float4x4, _float4, _float3>>(_arg);
		shared_ptr<CGameObject> pOwner = std::get<0>(arg);
		shared_ptr<CModel> pModel = pOwner->Get_Component<CModel>(COMPONENT::MODEL);
		m_pTarget = pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		m_mPivot = pModel->Get_Pivot();
		m_mBone = std::get<1>(arg);
		m_pBone = &m_mBone;
		m_vQuat = std::get<2>(arg);
		m_pTransform->Set_Matrix(g_mUnit);
		m_pTransform->Rotate(m_vQuat);
		m_pTransform->Multiply(*m_pBone * m_mPivot * m_pTarget->Get_Matrix());
		m_vBasePosition = m_pTransform->Get_State(TRANSFORM::POSITION);
		m_vBaseScale = _float3(1.f, 1.f, 1.f);
		m_vOffset = std::get<3>(arg);
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
	if (m_tData.pMask)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
	}
#endif

	return S_OK;
}

_bool CEffectRegular_VisualDetailCurve::Return()
{
	return m_bRelease;
}

HRESULT CEffectRegular_VisualDetailCurve::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Release", "Failed to CGameObject::Release");
	}

	m_pTarget = nullptr;
	m_pBone = nullptr;
	m_mPivot = g_mUnit;
	m_mBone = g_mUnit;
	m_vOffset = _float4();
	m_vQuat = XMQuaternionIdentity();

	return S_OK;
}

HRESULT CEffectRegular_VisualDetailCurve::Initialize(std::ifstream& _inFile)
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXNORMAL;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fDistortionPower), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fDiffuseTiling), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fEmissiveTiling), sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bLocal), sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.vSpriteSheetSize), sizeof(_int2));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass), sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass), sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	m_pCCScale = CCurveContainer::Read(_inFile);
	m_pCCBloom = CCurveContainer::Read(_inFile);
	m_pCCFade = CCurveContainer::Read(_inFile);
	m_pCCSprite = CCurveContainer::Read(_inFile);
	m_pCCDistortion = CCurveContainer::Read(_inFile);
	m_pCCDiffuse = CCurveContainer::Read(_inFile);
	m_pCCEmissive = CCurveContainer::Read(_inFile);
	m_pCCTranslation = CCurveContainer::Read(_inFile);
	m_pCCRotation = CCurveContainer::Read(_inFile);

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectRegular_VisualDetailCurve::Set_Data(const VFX_VisualDetailCurve _tData)
{
	m_tData = _tData;
}
#endif

shared_ptr<CEffectRegular_VisualDetailCurve> CEffectRegular_VisualDetailCurve::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectRegular_VisualDetailCurve> pInstance = make_private_shared(CEffectRegular_VisualDetailCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualDetailCurve::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectRegular_VisualDetailCurve::Clone(any)
{
	shared_ptr<CEffectRegular_VisualDetailCurve> pInstance = make_private_shared_copy(CEffectRegular_VisualDetailCurve, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualDetailCurve::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectRegular_VisualDetailCurve> CEffectRegular_VisualDetailCurve::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectRegular_VisualDetailCurve> pInstance = make_private_shared(CEffectRegular_VisualDetailCurve, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualDetailCurve::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualDetailCurve::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectRegular_VisualDetailCurve::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectRegular_VisualDetailCurve::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect), sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fDistortionPower), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fDiffuseTiling), sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fEmissiveTiling), sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bLocal), sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.vSpriteSheetSize), sizeof(_int2));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass), sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass), sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc), sizeof(MATERIALDESC));

	m_pCCScale->Export(outFile);
	m_pCCBloom->Export(outFile);
	m_pCCFade->Export(outFile);
	m_pCCSprite->Export(outFile);
	m_pCCDistortion->Export(outFile);
	m_pCCDiffuse->Export(outFile);
	m_pCCEmissive->Export(outFile);
	m_pCCTranslation->Export(outFile);
	m_pCCRotation->Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectRegular_VisualDetailCurve::Export", "Failed to Write File: CEffectRegular_Visual");
	}

	outFile.close();

	return S_OK;
}
#endif
