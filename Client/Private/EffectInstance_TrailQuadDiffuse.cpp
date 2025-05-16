#include "ClientPCH.h"
#include "EffectInstance_TrailQuadDiffuse.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectInstance_TrailQuadDiffuse::CEffectInstance_TrailQuadDiffuse(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_TrailQuad)
{
}

CEffectInstance_TrailQuadDiffuse::CEffectInstance_TrailQuadDiffuse(const CEffectInstance_TrailQuadDiffuse& _rhs)
	: CEffectInstance	(_rhs)
	, m_tData			(_rhs.m_tData)
{
}

HRESULT CEffectInstance_TrailQuadDiffuse::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINTVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	return S_OK;
}

HRESULT CEffectInstance_TrailQuadDiffuse::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Initialize", "Failed to CEffectInstance::Initialize");
	}

	return S_OK;
}

void CEffectInstance_TrailQuadDiffuse::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fInstanceAcc	+= _fTimeDelta;

	if (m_fInstanceAcc >= m_tData.fInterval)
	{
		m_fInstanceAcc		= fmodf(m_fInstanceAcc, m_tData.fInterval);
		m_bUpdate			= true;

		switch (m_eType)
		{
		case QUADPOINT::BONE:
			m_deqQuad.emplace_front(make_pair(
				(*m_pTargetBones.first * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3),
				(*m_pTargetBones.second	* m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3)));
			//	(*m_pTargetBones.first * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3) + _float3(0.f, 1.f, 0.f)));
			break;
		case QUADPOINT::LOCALPIVOT:
			m_deqQuad.emplace_front(make_pair(
				(*m_pTargetBone * m_mTargetLocalPivots.first * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3),
				(*m_pTargetBone * m_mTargetLocalPivots.second * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3)));
			break;
		}
		
		m_deqQuad.pop_back();

		if (m_fTimeAcc >= m_tData.fLifeTime)
		{
			for (_float& fWeight : m_vecInstanceWeight)
			{
				fWeight = std::clamp(fWeight - 1.f / static_cast<_float>(m_tData.iMaxInstance), 0.f, 1.f);
			}
			m_bRelease = std::all_of(m_vecInstanceWeight.begin(), m_vecInstanceWeight.end(), [](_float fWeight) { return !fWeight; });
		}
	}
	else
	{
		m_bUpdate = false;
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (m_fTimeAcc > m_tData.fLifeTime)
		{
		//	m_fTimeAcc = 0.f;
		}
	}
#endif
}

void CEffectInstance_TrailQuadDiffuse::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_TrailQuadDiffuse::Render()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_Int(SHADER_MAXINSTANCE, m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_Int: SHADER_MAXINSTANCE");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_tData.fDistortionPower)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to Bind_Int: SHADER_DISTORTION_POWER");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_TrailQuadDiffuse::Render_Bloom()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (m_tData.pDiffuse)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXDIFFUSE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXDIFFUSE, m_tData.pDiffuse->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXDIFFUSE");
		}
	}
	if (m_tData.pEmission)
	{
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
		if (FAILED(m_pShader->Bind_ShaderResourceViews(SHADER_TEXEMISSION, m_tData.pEmission->Get_ShaderResourceView().GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_ShaderResourceViews: SHADER_TEXEMISSION");
		}
	}

	if (FAILED(m_pShader->Bind_Int(SHADER_MAXINSTANCE, m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_Int: SHADER_MAXINSTANCE");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_DISTORTION_POWER, m_tData.fDistortionPower)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to Bind_Int: SHADER_DISTORTION_POWER");
	}

	m_pShader->Set_Flag(m_iShaderFlag);
	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Render_Bloom", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_TrailQuadDiffuse::Fetch(any _tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots)
{
#ifdef _DEBUG
#endif
	if (!_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Fetch", "Invalid Argument");
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene())
	{
#endif
		if (_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots.type() == typeid(tuple<shared_ptr<CGameObject>, const _char*, const _char*>))
		{
			m_eType = QUADPOINT::BONE;

			tuple<shared_ptr<CGameObject>, const _char*, const _char*> arg = any_cast<tuple<shared_ptr<CGameObject>, const _char*, const _char*>>(_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots);

			shared_ptr<CGameObject> pTarget		= std::get<0>(arg);
			shared_ptr<CTransform>	pTransform	= pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			shared_ptr<CModel>		pModel		= pTarget->Get_Component<CModel>(COMPONENT::MODEL);
			
			m_pTargetTransform					= pTransform;
			m_mTargetPivot						= pModel->Get_Pivot();
			m_pTargetBones.first				= pModel->Get_Bone(std::get<1>(arg))->Get_CombinedTransformationPointer();
			m_pTargetBones.second				= pModel->Get_Bone(std::get<2>(arg))->Get_CombinedTransformationPointer();
		}
		else if (_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots.type() == typeid(tuple<shared_ptr<CGameObject>, const _char*, _float4x4, _float4x4>))
		{
			m_eType = QUADPOINT::LOCALPIVOT;

			tuple<shared_ptr<CGameObject>, const _char*, _float4x4, _float4x4> arg = any_cast<tuple<shared_ptr<CGameObject>, const _char*, _float4x4, _float4x4>>(_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots);

			shared_ptr<CGameObject> pTarget		= std::get<0>(arg);
			shared_ptr<CTransform>	pTransform	= pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
			shared_ptr<CModel>		pModel		= pTarget->Get_Component<CModel>(COMPONENT::MODEL);

			m_pTargetTransform					= pTransform;
			m_mTargetPivot						= pModel->Get_Pivot();
			m_pTargetBone						= pModel->Get_Bone(std::get<1>(arg))->Get_CombinedTransformationPointer();
			m_mTargetLocalPivots.first			= std::get<2>(arg);
			m_mTargetLocalPivots.second			= std::get<3>(arg);
		}
#if ACTIVATE_TOOL
	}
	else
	{
		if (_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots.type() == typeid(tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _char*>))
		{
			m_eType = QUADPOINT::BONE;

			tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _char*> arg = any_cast<tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _char*>>(_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots);

			m_pTargetTransform					= std::get<0>(arg);
			m_mTargetPivot						= std::get<1>(arg)->Get_Pivot();
			m_pTargetBones.first				= std::get<1>(arg)->Get_Bone(std::get<2>(arg))->Get_CombinedTransformationPointer();
			m_pTargetBones.second				= std::get<1>(arg)->Get_Bone(std::get<3>(arg))->Get_CombinedTransformationPointer();
		}
		else if (_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots.type() == typeid(tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _float4x4, _float4x4>))
		{
			m_eType = QUADPOINT::LOCALPIVOT;

			tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _float4x4, _float4x4> arg = any_cast<tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*, _float4x4, _float4x4>>(_tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots);

			m_pTargetTransform					= std::get<0>(arg);
			m_mTargetPivot						= std::get<1>(arg)->Get_Pivot();
			m_pTargetBone						= std::get<1>(arg)->Get_Bone(std::get<2>(arg))->Get_CombinedTransformationPointer();
			m_mTargetLocalPivots.first			= std::get<3>(arg);
			m_mTargetLocalPivots.second			= std::get<4>(arg);
		}
	}
#endif

	m_deqQuad.resize(m_iActivateInstances + 1);
	m_vecInstanceWeight.reserve(m_tData.iMaxInstance);
	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstanceWeight.emplace_back(1.f - static_cast<_float>(i) / static_cast<_float>(m_tData.iMaxInstance));
	}

	switch (m_eType)
	{
	case QUADPOINT::BONE:
		if (FAILED(__super::Fetch(make_pair(
			(*m_pTargetBones.first * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3),
			(*m_pTargetBones.second * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3)))))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Fetch", "Failed to CEffectInstance::Fetch");
		}
		break;
	case QUADPOINT::LOCALPIVOT:
		if (FAILED(__super::Fetch(make_pair(
			(*m_pTargetBone * m_mTargetLocalPivots.first * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3),
			(*m_pTargetBone * m_mTargetLocalPivots.second * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3)))))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Fetch", "Failed to CEffectInstance::Fetch");
		}
		break;
	}

	return S_OK;
}

_bool CEffectInstance_TrailQuadDiffuse::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_TrailQuadDiffuse::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Release", "Failed to CGameObject::Release");
	}

	m_fInstanceAcc				= 0.f;

	m_bUpdate					= false;
	m_bItinerant				= false;
	m_bRelease					= false;

	m_pTargetTransform			= nullptr;
	m_mTargetPivot				= g_mUnit;
	m_pTargetBones.first		= nullptr;
	m_pTargetBones.second		= nullptr;
	m_pTargetBone				= nullptr;
	m_mTargetLocalPivots.first	= g_mUnit;
	m_mTargetLocalPivots.second	= g_mUnit;

//	m_iActivateInstances		= 0;
	m_iFinalInstance			= 0;
	m_bFinalInstance			= false;

	m_deqQuad.clear();
	m_vecInstanceWeight.clear();

	return S_OK;
}

HRESULT CEffectInstance_TrailQuadDiffuse::Initialize(std::ifstream& _inFile)
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
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDistortion::Initialize", "Failed to CTexture::Read");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDistortion::Initialize", "Failed to CTexture::Read");
		}
		m_iShaderFlag |= SHADER_FLAG_TEXEMISSIVE;
	}

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInterval),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fDistortionPower),	sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.bInstanceTexture),	sizeof(_bool));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),		sizeof(_uint));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDistortion::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_TrailQuadDiffuse::Set_Data(const VFX_TrailQuad _tData)
{
	m_tData					= _tData;
	m_iActivateInstances	= _tData.iMaxInstance;
}
#endif

void CEffectInstance_TrailQuadDiffuse::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);
}

void CEffectInstance_TrailQuadDiffuse::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	if (!m_bUpdate)
	{
		return;
	}

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		if (!m_bItinerant)
		{
			if (i >= m_deqQuad.size() - 1)
			{
				return;
			}
		}

		pData[i].vRight				= m_deqQuad[i].first;
		pData[i].vUp				= m_deqQuad[i].second;
		pData[i].vLook				= m_deqQuad[i + 1].first;
		pData[i].vTranslation		= m_deqQuad[i + 1].second;

		pData[i].vColor				= _color(1.f, 1.f, 1.f, m_vecInstanceWeight[i]);

		if (!m_tData.bInstanceTexture)
		{
			pData[i].vArgument.x	= 1.f - m_vecInstanceWeight[i];
			pData[i].vArgument.y	= 1.f - m_vecInstanceWeight[i];
			pData[i].vArgument.z	= 1.f - m_vecInstanceWeight[i] + (1.f / m_tData.iMaxInstance);
			pData[i].vArgument.w	= 1.f - m_vecInstanceWeight[i] + (1.f / m_tData.iMaxInstance);
		}
		else
		{
			pData[i].vArgument.x	= 0.f;
			pData[i].vArgument.y	= 0.f;
			pData[i].vArgument.z	= 1.f;
			pData[i].vArgument.w	= 1.f;
		}
	}
}

shared_ptr<CEffectInstance_TrailQuadDiffuse> CEffectInstance_TrailQuadDiffuse::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_TrailQuadDiffuse> pInstance = make_private_shared(CEffectInstance_TrailQuadDiffuse, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailQuadDiffuse::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_TrailQuadDiffuse::Clone(any)
{
	shared_ptr<CEffectInstance_TrailQuadDiffuse> pInstance = make_private_shared_copy(CEffectInstance_TrailQuadDiffuse, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailQuadDiffuse::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_TrailQuadDiffuse> CEffectInstance_TrailQuadDiffuse::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_TrailQuadDiffuse> pInstance = make_private_shared(CEffectInstance_TrailQuadDiffuse, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailQuadDiffuse::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailQuadDiffuse::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailQuadDiffuse::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_TrailQuadDiffuse::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	_bool bFalse(false), bTrue(true);
	if (m_tData.pDiffuse)
	{
		outFile.write(reinterpret_cast<const _byte*>(&bTrue), sizeof(_bool));
		if (FAILED(m_tData.pDiffuse->Export(outFile)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Export", "Failed to CTexture::Export");
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
			MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Export", "Failed to CTexture::Export");
		}
	}
	else
	{
		outFile.write(reinterpret_cast<const _byte*>(&bFalse), sizeof(_bool));
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInterval),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fDistortionPower),	sizeof(_float));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.bInstanceTexture),	sizeof(_bool));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fBloom),				sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iBloomPass),			sizeof(_int));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.iShaderPass),			sizeof(_int));

	outFile.write(reinterpret_cast<const _byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	outFile.write(reinterpret_cast<const _byte*>(&m_iActivateInstances),		sizeof(_uint));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Export", "Failed to Write File: CEffectInstance_TrailQuadDistortion");
	}

	outFile.close();

	return S_OK;
}
#endif
