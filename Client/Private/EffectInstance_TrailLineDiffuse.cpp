#include "ClientPCH.h"
#include "EffectInstance_TrailLineDiffuse.h"
#include "GameInstance.h"
#include "Bone.h"

CEffectInstance_TrailLineDiffuse::CEffectInstance_TrailLineDiffuse(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_TrailLineDiffuse)
{
}

CEffectInstance_TrailLineDiffuse::CEffectInstance_TrailLineDiffuse(const CEffectInstance_TrailLineDiffuse& _rhs)
	: CEffectInstance	(_rhs)
	, m_tData			(_rhs.m_tData)
{
}

HRESULT CEffectInstance_TrailLineDiffuse::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_INSTANCE_LINE);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINEVFX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_LINE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE, g_aNull);

	return S_OK;
}

HRESULT CEffectInstance_TrailLineDiffuse::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Initialize", "Failed to __super::Initialize");
	}
	
	return S_OK;
}

void CEffectInstance_TrailLineDiffuse::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fInstanceAcc	+= _fTimeDelta;

	if (m_fInstanceAcc >= m_tData.fInterval)
	{
		m_fInstanceAcc	= fmodf(m_fInstanceAcc, m_tData.fInterval);
		m_bUpdate		= true;

		m_deqLine.emplace_front((*m_pTargetBone * m_mTargetPivot * m_pTargetTransform->Get_Matrix()).row(3));
		m_deqLine.pop_back();

		if (m_fTimeAcc >= m_tData.fLifeTime)
		{
			for (_float& fWeight : m_vecInstanceWeight)
			{
				fWeight	= std::clamp(fWeight - 1.f / static_cast<_float>(m_tData.iMaxInstance), 0.f, 1.f);
			}
			m_bRelease	= std::all_of(m_vecInstanceWeight.begin(), m_vecInstanceWeight.end(), [](_float fWeight) { return !fWeight; });
		}
	}
	else
	{
		m_bUpdate = false;
	}
}

void CEffectInstance_TrailLineDiffuse::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_TrailLineDiffuse::Render()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (FAILED(m_pShader->Bind_Int(SHADER_MAXINSTANCE, m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render", "Failed to Bind_Int: SHADER_MAXINSTANCE");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_THICKNESS, m_tData.fThickness)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render", "Failed to Bind_Float: SHADER_THICKNESS");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	if (FAILED(__super::Render(m_tData.iShaderPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_TrailLineDiffuse::Render_Bloom()
{
	_bool bDCAA(false);
	if (FAILED(m_pShader->Bind_RawValue("g_bDeclareColorAsArguement", &bDCAA, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render_Bloom", "Failed to Bind_RawValue: g_bDeclareColorAsArguement");
	}

	if (FAILED(m_pShader->Bind_Int(SHADER_MAXINSTANCE, m_tData.iMaxInstance)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render_Bloom", "Failed to Bind_Int: SHADER_MAXINSTANCE");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_THICKNESS, m_tData.fThickness)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render_Bloom", "Failed to Bind_Float: SHADER_THICKNESS");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render_Bloom", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

	if (FAILED(__super::Render(m_tData.iBloomPass)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Render_Bloom", "Failed to CEffectInstance::Render");
	}

	return S_OK;
}

HRESULT CEffectInstance_TrailLineDiffuse::Fetch(any _pair_pObject_szBone)
{
	if (!_pair_pObject_szBone.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Fetch", "Invalid Argument");
	}

#if ACTIVATE_TOOL
	if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene())
	{
#endif
		pair<shared_ptr<CGameObject>, const _char*> arg = any_cast<pair<shared_ptr<CGameObject>, const _char*>>(_pair_pObject_szBone);
	
		shared_ptr<CGameObject>	pTarget		= arg.first;
		shared_ptr<CTransform>	pTransform	= pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		shared_ptr<CModel>		pModel		= pTarget->Get_Component<CModel>(COMPONENT::MODEL);
	
		m_pTargetTransform					= pTransform;
		m_pTargetBone						= pModel->Get_Bone(arg.second)->Get_CombinedTransformationPointer();
		m_mTargetPivot						= pModel->Get_Pivot();
#if ACTIVATE_TOOL
	}
	else
	{
		tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*> arg = any_cast<tuple<shared_ptr<CTransform>, shared_ptr<CModel>, _char*>>(_pair_pObject_szBone);

		m_pTargetTransform					= std::get<0>(arg);
		m_pTargetBone						= std::get<1>(arg)->Get_Bone(std::get<2>(arg))->Get_CombinedTransformationPointer();
		m_mTargetPivot						= std::get<1>(arg)->Get_Pivot();
	}
#endif

	m_deqLine.resize(m_tData.iMaxInstance + 3);
	m_vecInstanceWeight.reserve(m_tData.iMaxInstance);
	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstanceWeight.emplace_back(1.f - static_cast<_float>(i) / static_cast<_float>(m_tData.iMaxInstance));
	}

	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Fetch", "Failed to CEffectInstance::Fetch");
	}

	return S_OK;
}

_bool CEffectInstance_TrailLineDiffuse::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_TrailLineDiffuse::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDiffuse::Release", "Failed to CGameObject::Release");
	}

	m_fInstanceAcc			= 0.f;
	m_iInstanceCursor		= 0;

	m_bUpdate				= false;
	m_bItinerant			= false;
	m_bRelease				= false;

	m_pTargetTransform		= nullptr;
	m_pTargetBone			= nullptr;
	m_mTargetPivot			= g_mUnit;

//	m_iActivateInstances	= 0;

	m_deqLine.clear();
	m_vecInstanceWeight.clear();

	return S_OK;
}

HRESULT CEffectInstance_TrailLineDiffuse::Initialize(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInterval),			sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fThickness),			sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),		sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),		sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),			sizeof(MATERIALDESC));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDistortion::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_TrailLineDiffuse::Set_Data(const VFX_TrailLineDiffuse _tData)
{
	m_tData = _tData;
}
#endif

void CEffectInstance_TrailLineDiffuse::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXPOSSIZEINSTTRANSCOLORARG) * _iNumInstance);

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);
}

void CEffectInstance_TrailLineDiffuse::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	if (!m_bUpdate)
	{
		return;
	}

	VTXPOSSIZEINSTTRANSCOLORARG* pData = reinterpret_cast<VTXPOSSIZEINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		pData[i].vRight			= m_deqLine[i + 0];
		pData[i].vUp			= m_deqLine[i + 1];
		pData[i].vLook			= m_deqLine[i + 2];
		pData[i].vTranslation	= m_deqLine[i + 3];

		pData[i].vColor			= _color(1.f, 1.f, 1.f, m_vecInstanceWeight[i]);
	}
}

shared_ptr<CEffectInstance_TrailLineDiffuse> CEffectInstance_TrailLineDiffuse::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_TrailLineDiffuse> pInstance = make_private_shared(CEffectInstance_TrailLineDiffuse, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailLineDiffuse", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_TrailLineDiffuse::Clone(any)
{
	shared_ptr<CEffectInstance_TrailLineDiffuse> pInstance = make_private_shared_copy(CEffectInstance_TrailLineDiffuse, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailLineDiffuse", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_TrailLineDiffuse> CEffectInstance_TrailLineDiffuse::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_TrailLineDiffuse> pInstance = make_private_shared(CEffectInstance_TrailLineDiffuse, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailLineDiffuse::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailLineDiffuse::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_TrailLineDiffuse::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_TrailLineDiffuse::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInterval),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fThickness),			sizeof(_float));

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
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailLineDiffuse::Export", "Failed to Write File: CEffectInstance_TrailLineDiffuse");
	}

	outFile.close();

	return S_OK;
}
#endif
