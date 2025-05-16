#include "ClientPCH.h"
#include "EffectInstance_MeshAfterImage.h"
#include "GameInstance.h"
#include "LookUpTable.h"

//	LUT
//	Instance
//		Mesh
//		...
//	...

CEffectInstance_MeshAfterImage::CEffectInstance_MeshAfterImage(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffectInstance(_pDevice, _pContext, EFFECT::VFX_MeshAfterImage)
{
}

CEffectInstance_MeshAfterImage::CEffectInstance_MeshAfterImage(const CEffectInstance_MeshAfterImage& _rhs)
	: CEffectInstance	(_rhs)
	, m_tData			(_rhs.m_tData)
{
}

HRESULT CEffectInstance_MeshAfterImage::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER);

	m_umapComponentArg[COMPONENT::RENDERER]		= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]		= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESHANIMVFX, g_aNull);
	
	return S_OK;
}

HRESULT CEffectInstance_MeshAfterImage::Initialize(any _wstrPath)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CEffectInstance_MeshAfterImage::Tick(_float _fTimeDelta)
{
#ifdef LUT_SKINNING
#if ACTIVATE_IMGUI
	ImGui::Begin("After Image LUT");
//	ImGui::Image(m_pBoneInstanceLUT->Get_ShaderResourceView().Get(), ImVec2(400, 400));
	ImGui::Image(m_pBoneInstanceLUT->Get_ShaderResourceView().Get(), ImVec2(static_cast<_float>(4 * g_iMaxBones), static_cast<_float>(m_pTargetModel->Get_NumMeshes() * m_tData.iMaxInstance * 10)));
	ImGui::End();
#endif
#endif
	__super::Tick(_fTimeDelta);

	m_fInstanceAcc		+= _fTimeDelta;
	m_fAfterImageAcc	+= _fTimeDelta;

	if (m_fInstanceAcc >= m_tData.fInterval)
	{
		m_fInstanceAcc	= fmodf(m_fInstanceAcc, m_tData.fInterval);
		m_bUpdate		= true;

		if (m_fTimeAcc < m_tData.fLifeTime)
		{
			if (m_fAfterImageAcc >= m_tData.fAfterImageInterval)
			{
				m_vecMatrices[m_iInstanceCursor] = m_pTargetTransform->Get_WorldMatrix();
				for (_uint i = 0; i < m_pTargetModel->Get_NumMeshes(); ++i)
				{
					if (FAILED(Update_BoneData(m_iInstanceCursor, i)))
					{
						MSG_CONTINUE("CEffectInstance_MeshAfterImage::Tick", "Failed to Update_BoneData");
					}
				}

				m_vecInstanceWeight[m_iInstanceCursor] = 1.f;
				m_iInstanceCursor = m_iInstanceCursor == m_tData.iMaxInstance - 1 ? 0 : m_iInstanceCursor + 1;
				m_fAfterImageAcc = fmodf(m_fAfterImageAcc, m_tData.fAfterImageInterval);
			}
		}

		for (_float& fWeight : m_vecInstanceWeight)
		{
			fWeight = std::clamp(fWeight - _fTimeDelta / m_tData.fFadeOut, 0.f, 1.f);
		}

		if (m_fTimeAcc >= m_tData.fLifeTime)
		{
			m_bRelease = std::all_of(m_vecInstanceWeight.begin(), m_vecInstanceWeight.end(), [](_float fWeight) { return !fWeight; });
		}
	}
	else
	{
		m_bUpdate = false;
	}
	
#pragma region Instance
	if (m_pTargetModel)
	{
		m_pTargetModel->Update_Instance([&](void* pData, _uint iNumInstance) { Update_Instance(pData, iNumInstance, _fTimeDelta); });
	}
#pragma endregion
}

void CEffectInstance_MeshAfterImage::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
//	Add_RenderObject(RENDER_GROUP::NONBLEND);
	if (m_tData.fBloom)
	{
		Add_RenderObject(RENDER_GROUP::BLOOM);
	}
}

HRESULT CEffectInstance_MeshAfterImage::Render()
{
//	ComPtr<ID3D11ShaderResourceView> pDumpSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
//	m_pContext->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pDumpSRV->GetAddressOf());

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTargetTransform->Get_WorldMatrix())))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_MTRLSHN, m_tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

#ifdef LUT_SKINNING
	if (FAILED(m_pBoneInstanceLUT->Bind_ShaderResourceView(m_pShader, SHADER_LUT_INSTANCED_BONES)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CLookUpTable::Bind_ShaderResourceView: SHADER_LUT_INSTANCED_BONES");
	}
#elif !defined(TEMP_AFTERIMAGE)
	if (FAILED(m_pShader->Bind_ShaderResourceView("g_sbBoneInstances", m_pBoneInstanceSRV)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CShader::Bind_ShaderResourceView");
	}
#endif

	for (_uint i = 0; i < m_pTargetModel->Get_NumMeshes(); ++i)
	{
		m_pTargetModel->Update_Instance([&](void* pData, _uint iNumInstance) { Update_Instance(pData, iNumInstance, 0.f); });
		if (FAILED(m_pShader->Bind_Int("g_iCurrentMeshIndex", i)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to Bind_Int");
		}
		if (FAILED(m_pTargetModel->Render_Instance(i, m_pShader, m_tData.iShaderPass)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render", "Failed to CModel::Render_Instance");
		}
	}

	return S_OK;
}

HRESULT CEffectInstance_MeshAfterImage::Render_Bloom()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_pTargetTransform->Get_WorldMatrix())))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, m_tMaterialDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLAMB, m_tMaterialDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLSPC, m_tMaterialDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLEMS, m_tMaterialDesc.vEmissive)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
	}
	if (FAILED(m_pShader->Bind_Float(SHADER_MTRLSHN, m_tMaterialDesc.fShininess)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_RawValue: SHADER_MTRLSHN");
	}

	if (FAILED(m_pShader->Bind_Float(SHADER_BLOOMSTRENGTH, m_tData.fBloom)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to Bind_Float: SHADER_BLOOMSTRENGTH");
	}

#ifdef LUT_SKINNING
	if (FAILED(m_pBoneInstanceLUT->Bind_ShaderResourceView(m_pShader, SHADER_LUT_INSTANCED_BONES)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CLookUpTable::Bind_ShaderResourceView: SHADER_LUT_INSTANCED_BONES");
	}
#elif !defined(TEMP_AFTERIMAGE)
	if (FAILED(m_pShader->Bind_ShaderResourceView("g_sbBoneInstances", m_pBoneInstanceSRV)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CShader::Bind_ShaderResourceView");
	}
#endif

	for (_uint i = 0; i < m_pTargetModel->Get_NumMeshes(); ++i)
	{
		m_pTargetModel->Update_Instance([&](void* pData, _uint iNumInstance) { Update_Instance(pData, iNumInstance, 0.f); });
		if (FAILED(m_pShader->Bind_Int("g_iCurrentMeshIndex", i)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to Bind_Int");
		}
		if (FAILED(m_pTargetModel->Render_Instance(i, m_pShader, m_tData.iBloomPass)))
		{
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Render_Bloom", "Failed to CModel::Render_Instance");
		}
	}

	return S_OK;
}

HRESULT CEffectInstance_MeshAfterImage::Fetch(any _pObject)
{
	m_bRelease = false;
	if (!_pObject.has_value())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Fetch", "Invalid Argument");
	}
#if ACTIVATE_TOOL
	if (SCENE::TOOL != CGameInstance::Get_Instance()->Current_Scene())
	{
#endif
		m_pTarget			= any_cast<shared_ptr<CGameObject>>(_pObject);
		m_pTargetTransform	= m_pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
		m_pTargetModel		= m_pTarget->Get_Component<CModel>(COMPONENT::MODEL);
#if ACTIVATE_TOOL
	}
	else
	{
		m_pTargetTransform	= any_cast<pair<shared_ptr<CTransform>, shared_ptr<CModel>>>(_pObject).first;
		m_pTargetModel		= any_cast<pair<shared_ptr<CTransform>, shared_ptr<CModel>>>(_pObject).second;
	}
#endif
#if ACTIVATE_TOOL
	if (SCENE::TOOL == CGameInstance::Get_Instance()->Current_Scene())
	{
		if (FAILED(m_pTargetModel->Ready_Instances(100)))
		{
			m_bRelease = true;
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Fetch", "Not Enough Instances");
		}
	}
	else
#endif
	{
		if (static_cast<_int>(m_pTargetModel->Get_NumInstances()) < m_tData.iMaxInstance)
		{
			m_bRelease = true;
			MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Fetch", "Not Enough Instances");
		}
	}
	m_pBoneData			= Function::CreateDynamicArray<_float4x4>(g_iMaxBones * m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes());
#ifdef LUT_SKINNING
	m_pBoneInstanceLUT	= CLookUpTable::Create(m_pDevice, _uint2(g_iMaxBones * 4, m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes()), DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (nullptr == m_pBoneInstanceLUT)
	{
		m_bRelease = true;
		MSG_RETURN(E_FAIL, "CEffectInstance-MeshAfterImage::Fetch", "Failed to CLookUpTable::Create");
	}
#endif
	m_vecMatrices.resize(m_tData.iMaxInstance);
	m_vecInstanceWeight.resize(m_tData.iMaxInstance);
	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		m_vecInstanceWeight.emplace_back(1.f - static_cast<_float>(i) / static_cast<_float>(m_tData.iMaxInstance));
	}
#if !defined(TEMP_AFTERIMAGE) && !defined(LUT_SKINNING)
	D3D11_BUFFER_DESC							tBufferDesc{};
    tBufferDesc.ByteWidth						= sizeof(_float4x4) * g_iMaxBones * m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes();
    tBufferDesc.Usage							= D3D11_USAGE_DYNAMIC;
    tBufferDesc.BindFlags						= D3D11_BIND_SHADER_RESOURCE;
    tBufferDesc.CPUAccessFlags					= D3D11_CPU_ACCESS_WRITE;
    tBufferDesc.MiscFlags						= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    tBufferDesc.StructureByteStride				= sizeof(_float4x4);

	D3D11_SHADER_RESOURCE_VIEW_DESC				tShaderResourceViewDesc{};
	tShaderResourceViewDesc.Format				= DXGI_FORMAT_UNKNOWN;
	tShaderResourceViewDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;
	tShaderResourceViewDesc.Buffer.FirstElement	= 0;
	tShaderResourceViewDesc.Buffer.NumElements	= g_iMaxBones * m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes();

	if (FAILED(m_pDevice->CreateBuffer(&tBufferDesc, nullptr, m_pBoneInstanceBuffer.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Initialize", "Failed to CreateBuffer");
	}
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pBoneInstanceBuffer.Get(), &tShaderResourceViewDesc, m_pBoneInstanceSRV.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Initialize", "Failed to CreateShaderResourceView");
	}
#endif
#pragma region Instance
	if (m_pTargetModel)
	{
		m_pTargetModel->Update_Instance([&](void* pData, _uint iNumInstance) { Fetch_Instance(pData, iNumInstance); });
	}
#pragma endregion
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Fetch", "Failed to CEffectInstance::Fetch");
	}

	return S_OK;
}

_bool CEffectInstance_MeshAfterImage::Return()
{
	return m_bRelease;
}

HRESULT CEffectInstance_MeshAfterImage::Release()
{
	if (FAILED(__super::Release()))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Release", "Failed to CGameObject::Release");
	}

	m_fInstanceAcc			= 0.f;
	m_fAfterImageAcc		= 0.f;
	m_iInstanceCursor		= 0;

	m_bUpdate				= false;
	m_bRelease				= false;

	m_pTarget				= nullptr;
	m_pTargetModel			= nullptr;
	m_pTargetTransform		= nullptr;
	m_mTargetPivot			= g_mUnit;

	m_pBoneData				= nullptr;
#ifdef LUT_SKINNING
	m_pBoneInstanceLUT		= nullptr;
#endif
//	m_iActivateInstances	= 0;

#if !defined(TEMP_AFTERIMAGE) && !defined(LUT_SKINNING)
	m_pBoneInstanceBuffer	= nullptr;
	m_pBoneInstanceSRV		= nullptr;
#endif

	m_vecMatrices.clear();
	m_vecInstanceWeight.clear();

	return S_OK;
}

HRESULT CEffectInstance_MeshAfterImage::Initialize(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fLifeTime),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fInterval),				sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fAfterImageInterval),	sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fFadeOut),				sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iMaxInstance),			sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.fBloom),					sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iBloomPass),				sizeof(_int));
	_inFile.read(reinterpret_cast<_byte*>(&m_tData.iShaderPass),			sizeof(_int));

	_inFile.read(reinterpret_cast<_byte*>(&m_tMaterialDesc),				sizeof(MATERIALDESC));

	_inFile.read(reinterpret_cast<_byte*>(&m_iActivateInstances),			sizeof(_uint));

	if (_inFile.fail())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CEffectInstance_TrailQuadDistortion::Initialize", "Failed to CTexture::Read");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
void CEffectInstance_MeshAfterImage::Set_Data(const VFX_MeshAfterImage _tData)
{
	m_tData = _tData;
}
void CEffectInstance_MeshAfterImage::Set_ToolInformation(shared_ptr<CTransform> _pTransform, shared_ptr<CModel> _pModel)
{
	m_pTargetTransform = _pTransform;
	m_pTargetModel = _pModel;
	m_mTargetPivot = m_pTargetModel->Get_Pivot();
}
#endif

void CEffectInstance_MeshAfterImage::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	ZeroMemory(_pData, sizeof(VTXMESHANIMINSTTRANSCOLORARG) * _iNumInstance);

	VTXMESHANIMINSTTRANSCOLORARG* pData = reinterpret_cast<VTXMESHANIMINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		pData[i].vColor = _color(1.f, 1.f, 1.f, 1.f);
	}
}

void CEffectInstance_MeshAfterImage::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	if (!m_bUpdate)
	{
		return;
	}

	VTXMESHANIMINSTTRANSCOLORARG* pData = reinterpret_cast<VTXMESHANIMINSTTRANSCOLORARG*>(_pData);

	for (_int i = 0; i < m_tData.iMaxInstance; ++i)
	{
		memcpy(&pData[i], &m_vecMatrices[i], sizeof(_float4x4));

		pData[i].vColor.w		= m_vecInstanceWeight[i];

	//	pData[i].vArgument.x	= i;
	//	pData[i].vArgument.y	= i;
	//	pData[i].vArgument.z	= m_tData.iMaxInstance;
	//	pData[i].vArgument.w	= m_pTargetModel->Get_NumMeshes();

	//	_float4x4	mBone		= m_pBoneData[i * m_pTargetModel->Get_NumMeshes() + m_iMeshIndex];
	//	pData[i].vRight			= mBone.row(0);
	//	pData[i].vUp			= mBone.row(1);
	//	pData[i].vLook			= mBone.row(2);
	//	pData[i].vTranslation	= mBone.row(3);

		pData[i].vArgument.x	= static_cast<_float>(i);
		pData[i].vArgument.y	= static_cast<_float>(i);
		pData[i].vArgument.z	= static_cast<_float>(m_tData.iMaxInstance);
		pData[i].vArgument.w	= static_cast<_float>(m_pTargetModel->Get_NumMeshes());
	}
}

HRESULT CEffectInstance_MeshAfterImage::Update_BoneData(_uint _iInstanceIndex, _uint _iMeshIndex)
{
//	cout << "Update_BoneData " << _iInstanceIndex << " " << _iMeshIndex << endl;
	memcpy(
		m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones,
		m_pTargetModel->Get_BoneMatrices(_iMeshIndex),
		sizeof(_float4x4) * m_pTargetModel->Get_NumBones());

//	m_pShader->Bind_MatrixArray(SHADER_BONE, m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones, g_iMaxBones);

#ifdef LUT_SKINNING
	if (FAILED(m_pBoneInstanceLUT->Update(m_pContext, m_pBoneData.get(), sizeof(_float4x4) * m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes() * g_iMaxBones)))
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Update_BoneData", "Failed to CLookUpTable::Update");
	}
#elif defined(TEMP_AFTERIMAGE)
	switch (_iInstanceIndex)
	{
	case 0:
		m_pShader->Bind_MatrixArray("mBone0", m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones, m_pTargetModel->Get_NumBones());
		break;
	case 1:
		m_pShader->Bind_MatrixArray("mBone1", m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones, m_pTargetModel->Get_NumBones());
		break;
	case 2:
		m_pShader->Bind_MatrixArray("mBone2", m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones, m_pTargetModel->Get_NumBones());
		break;
	case 3:
		m_pShader->Bind_MatrixArray("mBone3", m_pBoneData.get() + (m_pTargetModel->Get_NumMeshes() * _iInstanceIndex + _iMeshIndex) * g_iMaxBones, m_pTargetModel->Get_NumBones());
		break;
	}
#else
	D3D11_MAPPED_SUBRESOURCE tSubResource{};
	if (SUCCEEDED(m_pContext->Map(m_pBoneInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubResource)))
	{
		memcpy(tSubResource.pData, m_pBoneData.get(), sizeof(_float4x4) * g_iMaxBones * m_tData.iMaxInstance * m_pTargetModel->Get_NumMeshes());
		m_pContext->Unmap(m_pBoneInstanceBuffer.Get(), 0);
	}
	else
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Update_BoneData", "Failed to ID3D11DeviceContext::Map");
	}
#endif

	return S_OK;
}

shared_ptr<CEffectInstance_MeshAfterImage> CEffectInstance_MeshAfterImage::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffectInstance_MeshAfterImage> pInstance = make_private_shared(CEffectInstance_MeshAfterImage, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_MeshAfterImage::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffectInstance_MeshAfterImage::Clone(any)
{
	shared_ptr<CEffectInstance_MeshAfterImage> pInstance = make_private_shared_copy(CEffectInstance_MeshAfterImage, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_MeshAfterImage::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CEffectInstance_MeshAfterImage> CEffectInstance_MeshAfterImage::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CEffectInstance_MeshAfterImage> pInstance = make_private_shared(CEffectInstance_MeshAfterImage, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_MeshAfterImage::Read", "Failed to Initialize_Prototype");
	}
	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffectInstance_MeshAfterImage::Read", "Failed to Initialize");
	}
	if (FAILED(pInstance->Initialize(_inFile)))
	{
		MSG_RETURN(nullptr, "CEffectInstance_MeshAfterImage::Read", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
HRESULT CEffectInstance_MeshAfterImage::Export(const wstring& _wstrFilePath)
{
	std::ofstream outFile(_wstrFilePath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CEffectInstance_MeshAfterImage::Export", "Failed to Open File");
	}

	outFile.write(reinterpret_cast<const _byte*>(&m_eEffect),					sizeof(EFFECT));

	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fLifeTime),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fInterval),			sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fAfterImageInterval),	sizeof(_float));
	outFile.write(reinterpret_cast<const _byte*>(&m_tData.fFadeOut),			sizeof(_float));

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
