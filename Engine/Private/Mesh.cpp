#include "EnginePCH.h"
#include "Mesh.h"
#include "Model.h"
#include "Animation.h"
#include "Bone.h"
#include "Event_Handler.h"
#include "PhysXEngine.h"
#include "Shader.h"

CMesh::CMesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::MESH)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_pTriangleMesh(rhs.m_pTriangleMesh)
	, m_vecBoneIndices(rhs.m_vecBoneIndices)
	, m_vecBoneOffsets(rhs.m_vecBoneOffsets)
	, m_vecBones(rhs.m_vecBones)
	, m_vecInterpolationMatrices(rhs.m_vecInterpolationMatrices)
	, m_iNumBones(rhs.m_iNumBones)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_fFarestPoint(rhs.m_fFarestPoint)
{
	strcpy_s(m_szName, MAX_PATH, rhs.m_szName);
	if (rhs.m_pVertices_Anim)
	{
		m_pVertices_Anim = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);
		memcpy(m_pVertices_Anim.get(), rhs.m_pVertices_Anim.get(), sizeof(VTXMESHANIM) * m_iNumVertices);
	}
	if (m_pVertices_NonAnim)
	{
		m_pVertices_NonAnim = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);
		memcpy(rhs.m_pVertices_NonAnim.get(), rhs.m_pVertices_NonAnim.get(), sizeof(VTXMESH) * m_iNumVertices);
	}
}

#if ACTIVATE_TOOL
HRESULT CMesh::Initialize_FromAssimp(MODEL _eType, const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel)
{
	strcpy_s(m_szName, _pAIMesh->mName.C_Str());

	m_iMaterialIndex = _pAIMesh->mMaterialIndex;

	m_iNumVB = 1;
	m_iNumVertices = _pAIMesh->mNumVertices;
	m_iNumIndices = _pAIMesh->mNumFaces * 3;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVertices = Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	switch (_eType)
	{
	case MODEL::ANIM:
		m_iVertexStride = sizeof(VTXMESHANIM);
		if (FAILED(Ready_VertexBuffer_Anim(_pAIMesh, _pModel)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize_FromAssimp", "Failed to Ready_VertexBuffer_Anim");
		}
		break;
	case MODEL::NONANIM:
		m_iVertexStride = sizeof(VTXMESH);
		if (FAILED(Ready_VertexBuffer_NonAnim(_pAIMesh)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize_FromAssimp", "Failed to Ready_VertexBuffer_NonAnim");
		}
		break;

	default:
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromAssimp", "Invalid MODEL");
	}

#pragma endregion
#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = 0;

	_uint iIndex = 0;
	for (_uint i = 0; i < _pAIMesh->mNumFaces; ++i)
	{
		m_pIndices[iIndex++] = _pAIMesh->mFaces[i].mIndices[0];
		m_pIndices[iIndex++] = _pAIMesh->mFaces[i].mIndices[1];
		m_pIndices[iIndex++] = _pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = m_pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromAssimp", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromAssimp", "Failed to __super::Initialize");
	}

	return S_OK;
}
#endif

HRESULT CMesh::Initialize_FromBinary(MODEL _eType, std::ifstream& _inFile, _matrixf PivotMat)
{
	if (!Function::InRange(_eType, static_cast<MODEL>(0), MODEL::MAX))
	{
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Invalid MODEL");
	}

	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_inFile.read(reinterpret_cast<_byte*>(m_szName), MAX_PATH);
	_inFile.read(reinterpret_cast<_byte*>(&m_iMaterialIndex), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumVB), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumVertices), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumIndices), sizeof(_uint));

	m_pVertices = Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	_inFile.read(reinterpret_cast<_byte*>(m_pVertices.get()), sizeof(_float3) * m_iNumVertices);
	_inFile.read(reinterpret_cast<_byte*>(m_pIndices.get()), sizeof(_uint) * m_iNumIndices);

#pragma region VERTEX_BUFFER

	switch (_eType)
	{
		break;
	case MODEL::ANIM:
		m_iVertexStride = sizeof(VTXMESHANIM);
		if (FAILED(Ready_VertexBuffer_Anim(_inFile)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Failed to Ready_VertexBuffer_Anim");
		}
		break;
	case MODEL::NONANIM:
		m_iVertexStride = sizeof(VTXMESH);
		if (FAILED(Ready_VertexBuffer_NonAnim(_inFile, PivotMat)))
		{
			MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Failed to Ready_VertexBuffer_NonAnim");
		}
		break;

	default:
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Invalid MODEL");
	}

#pragma endregion
#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = m_pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Failed to CreateBuffer");
	}

#pragma endregion

#pragma region COOK
	if (_eType == MODEL::NONANIM)
	{
		shared_ptr<CPhysXEngine> pPhysXEngine = CPhysXEngine::Get_Instance();

		shared_ptr<PxVec3[]> PxVertexBuffer(new PxVec3[m_iNumVertices]);
		for (_uint i = 0; i < m_iNumVertices; i++)
		{
			PxVertexBuffer[i] = { m_pVertices_NonAnim[i].vPosition.x,m_pVertices_NonAnim[i].vPosition.y,m_pVertices_NonAnim[i].vPosition.z };
			_float Distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_pVertices_NonAnim[i].vPosition)));
			if (Distance >= m_fFarestPoint)
				m_fFarestPoint = Distance;
		}

		shared_ptr<PxU32[]> PxIndexBuffer(new PxU32[m_iNumIndices]);
		for (_uint i = 0; i < m_iNumIndices; i++)
			PxIndexBuffer[i] = m_pIndices[i];

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = m_iNumVertices;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = PxVertexBuffer.get();

		meshDesc.triangles.count = m_iNumIndices / 3;
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		meshDesc.triangles.data = PxIndexBuffer.get();
		meshDesc.materialIndices = NULL;
		//meshDesc.flags = PxMeshFlag::eFLIPNORMALS;
		if (!meshDesc.isValid())
			int test = 0;
		PxTolerancesScale scale;
		PxCookingParams params(scale);
		PxDefaultMemoryOutputStream writeBuffer;
		PxTriangleMeshCookingResult::Enum result;
		bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
		if (!status)
			return E_FAIL;

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		m_pTriangleMesh = pPhysXEngine->GetPhysics()->createTriangleMesh(readBuffer);


	}
#pragma endregion COOK

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMesh::Initialize_FromBinary", "Failed to __super::Initialize");
	}

	return S_OK;
}

HRESULT CMesh::Initialize(any Any)
{
	return __super::Initialize(Any);
}

#if ACTIVATE_TOOL
HRESULT CMesh::Ready_VertexBuffer_Anim(const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel)
{
	m_iNumBones = _pAIMesh->mNumBones;
	m_vecBoneIndices.reserve(m_iNumBones);
	m_vecBoneOffsets.reserve(m_iNumBones);
	m_vecBones.resize(m_iNumBones, g_mUnit);
	m_vecInterpolationMatrices.resize(m_iNumBones, g_mUnit);

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		if (_pAIMesh->mVertices)
		{
			memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
			m_pVertices[i] = pVertices[i].vPosition;
		}
		if (_pAIMesh->mNormals)
		{
			memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
			pVertices[i].vNormal = _float3(pVertices[i].vNormal).normalize();
		}
		if (_pAIMesh->mTangents)
		{
			memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
			pVertices[i].vTangent = _float3(pVertices[i].vTangent).normalize();
		}
		if (_pAIMesh->mTextureCoords[0])
		{
			memcpy(&pVertices[i].vTexCoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		}
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = _pAIMesh->mBones[i];
		m_vecBoneIndices.emplace_back(_pModel->Get_BoneIndex(pAIBone->mName.C_Str()));

		_float4x4 mOffset;
		memcpy(&mOffset, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		m_vecBoneOffsets.emplace_back(XMMatrixTranspose(mOffset));

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
			{
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
				pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;
		m_vecBoneIndices.emplace_back(_pModel->Get_BoneIndex(m_szName));
		m_vecBoneOffsets.emplace_back(g_mUnit);
		m_vecBones.emplace_back(g_mUnit);
		m_vecInterpolationMatrices.emplace_back(g_mUnit);
	}

#if ACTIVATE_TOOL
	m_pVertices_Anim = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);
	memcpy(m_pVertices_Anim.get(), pVertices.get(), m_iVertexStride * m_iNumVertices);
#endif

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Ready_VertexBuffer_Anim", "Failed to CreateBuffer");
	}

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(const aiMesh* _pAIMesh)
{
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		if (_pAIMesh->mVertices)
		{
			memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
			m_pVertices[i] = pVertices[i].vPosition;
		}
		if (_pAIMesh->mNormals)
		{
			memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
			pVertices[i].vNormal = _float3(pVertices[i].vNormal).normalize();
		}
		if (_pAIMesh->mTangents)
		{
			memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
			pVertices[i].vTangent = _float3(pVertices[i].vTangent).normalize();
		}
		if (_pAIMesh->mTextureCoords[0])
		{
			memcpy(&pVertices[i].vTexCoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		}
	}
#if ACTIVATE_TOOL
	m_pVertices_NonAnim = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);
	memcpy(m_pVertices_NonAnim.get(), pVertices.get(), m_iVertexStride * m_iNumVertices);
#endif

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Ready_VertexBuffer_NonAnim", "Failed to CreateBuffer");
	}

	return S_OK;
}
#endif

HRESULT CMesh::Ready_VertexBuffer_Anim(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumBones), sizeof(_uint));

	m_vecBoneIndices.resize(m_iNumBones);
	m_vecBoneOffsets.resize(m_iNumBones);
	m_vecBones.resize(m_iNumBones, g_mUnit);
	m_vecInterpolationMatrices.resize(m_iNumBones, g_mUnit);

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);

	_inFile.read(reinterpret_cast<_byte*>(m_vecBoneIndices.data()), sizeof(_uint) * m_iNumBones);
	_inFile.read(reinterpret_cast<_byte*>(m_vecBoneOffsets.data()), sizeof(_float4x4) * m_iNumBones);
	_inFile.read(reinterpret_cast<_byte*>(pVertices.get()), m_iVertexStride * m_iNumVertices);

	m_pVertices_Anim = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);
	memcpy(m_pVertices_Anim.get(), pVertices.get(), m_iVertexStride * m_iNumVertices);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Ready_VertexBuffer_Anim", "Failed to CreateBuffer");
	}

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(std::ifstream& _inFile, _matrixf PivotMat)
{
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	_inFile.read(reinterpret_cast<_byte*>(pVertices.get()), m_iVertexStride * m_iNumVertices);

	m_pVertices_NonAnim = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices.get()[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices.get()[i].vPosition), PivotMat));
		XMStoreFloat3(&pVertices.get()[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices.get()[i].vNormal), PivotMat));
		XMStoreFloat3(&pVertices.get()[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices.get()[i].vTangent), PivotMat));
	}

	memcpy(m_pVertices_NonAnim.get(), pVertices.get(), m_iVertexStride * m_iNumVertices);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CMesh::Ready_VertexBuffer_NonAnim", "Failed to CreateBuffer");
	}


	return S_OK;
}

HRESULT CMesh::Render_Instance(_uint iDrawCount, shared_ptr<CShader> pShader, _uint iPassIndex)
{
	if (FAILED(pShader->BeginPass(iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CMesh::Render_Instance", "Failed to CShader::BeginPass");
	}

	_uint iOffset = 0;
	m_pContext->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &m_iVertexStride, &iOffset);
	m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->DrawIndexedInstanced(m_iIndicesCountPerInstance, iDrawCount, 0, 0, 0);

	return S_OK;
}

_float3 CMesh::Get_RandomVertex(const MODEL _eType) const
{
//	float4x4	mBone =
//		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
//		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
//		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
//		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));

	_uint iVtxIdx = rand() % m_iNumVertices;

	switch (_eType)
	{
	case MODEL::ANIM:
	{
		VTXMESHANIM	tVTX = m_pVertices_Anim[iVtxIdx];
		_float4x4	mBone
			= m_vecBones[tVTX.vBlendIndices.x] * tVTX.vBlendWeights.x
			+ m_vecBones[tVTX.vBlendIndices.y] * tVTX.vBlendWeights.y
			+ m_vecBones[tVTX.vBlendIndices.z] * tVTX.vBlendWeights.z
			+ m_vecBones[tVTX.vBlendIndices.w] * (1.f - (tVTX.vBlendWeights.x + tVTX.vBlendWeights.y + tVTX.vBlendWeights.z));
		return XMVector3TransformCoord(_float4(tVTX.vPosition, 1.f), mBone);
	}
	break;
	case MODEL::NONANIM:
	{
		return m_pVertices_NonAnim[iVtxIdx].vPosition;
	}
	break;

	default:
		MSG_RETURN(_float3(), "CMesh::Get_RandomVertex", "Invalid MODEL");
	}
}

VTXMESH CMesh::Get_RandomVTXMESH(_uint _iMinIdx, _uint _iMaxIdx) const
{
	return m_pVertices_NonAnim[_iMinIdx + rand() % ((_iMaxIdx == UINT_MAX ? m_iNumVertices : _iMaxIdx) - _iMinIdx)];
}

const _float4x4* CMesh::Get_BoneMatrices(vector<shared_ptr<CBone>>::iterator _itBegin, _matrixf _mPivot)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_matrix mInterpolation = Function::Lerp(m_vecInterpolationMatrices[i], _itBegin[m_vecBoneIndices[i]]->Get_CombinedTransformation(), m_fInterpolationRatio);
		m_vecBones[i] = m_vecBoneOffsets[i] * mInterpolation * _mPivot;
	}
	return m_vecBones.data();
}

void CMesh::Set_InterpolationMatrix(vector<shared_ptr<CBone>>::iterator _itBegin, _float _fDuration)
{
	if (0.f == _fDuration)
	{
		m_fInterpolationRatio = 1.f;
	}
	else
	{
		for (_uint i = 0; i < m_iNumBones; ++i)
		{
			m_vecInterpolationMatrices[i] = Function::Lerp(m_vecInterpolationMatrices[i], _itBegin[m_vecBoneIndices[i]]->Get_CombinedTransformation(), m_fInterpolationRatio);
		}

		m_fInterpolationRatio = 0.f;

		CEvent_Handler::Get_Instance()->Erase_OnTickListener(shared_from_this());
		CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
			[this, _fDuration](_float _fDeltaTime)->_bool
			{
				m_fInterpolationRatio += _fDeltaTime / _fDuration;

				if (m_fInterpolationRatio >= 1.f)
				{
					m_fInterpolationRatio = 1.f;
					return false;
				}

				return true;
			}
		);
	}
}

void CMesh::Copy_BoneIndices(void* _pDest)
{
	memcpy(_pDest, m_vecBoneIndices.data(), sizeof(_uint) * m_iNumBones);
}

void CMesh::Copy_BoneOffsets(void* _pDest)
{
	memcpy(_pDest, m_vecBoneOffsets.data(), sizeof(_float4x4) * m_iNumBones);
}

void CMesh::Copy_NonAnimVertices(void* _pDest)
{
	memcpy(_pDest, m_pVertices_NonAnim.get(), m_iVertexStride * m_iNumVertices);
}

void CMesh::Copy_AnimVertices(void* _pDest)
{
	memcpy(_pDest, m_pVertices_Anim.get(), m_iVertexStride * m_iNumVertices);
}

#if ACTIVATE_TOOL
shared_ptr<CMesh> CMesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MODEL _eType, const aiMesh* _pAIMesh, shared_ptr<CModel> _pModel)
{
	shared_ptr<CMesh> pInstance = make_private_shared(CMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_FromAssimp(_eType, _pAIMesh, _pModel)))
	{
		MSG_RETURN(nullptr, "CMesh::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CMesh> CMesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MODEL _eType, std::ifstream& _inFile, _matrixf PivotMat)
{
	shared_ptr<CMesh> pInstance = make_private_shared(CMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_FromBinary(_eType, _inFile, PivotMat)))
	{
		MSG_RETURN(nullptr, "CMesh::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CMesh> CMesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrPath)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(nullptr, "CMesh::Read", "Failed to Open File");
	}

	shared_ptr<CMesh> pInstance = make_private_shared(CMesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_FromBinary(MODEL::NONANIM, inFile)))
	{
		MSG_RETURN(nullptr, "CMesh::Read", "Failed to Initialize_FromBinary");
	}

	inFile.close();

	return pInstance;
}

shared_ptr<CComponent> CMesh::Clone(any Any)
{
	shared_ptr<CMesh> pInstance = make_private_shared_copy(CMesh, *this);

	if (FAILED(pInstance->Initialize(Any)))
	{
		MSG_RETURN(nullptr, "CModel::Clone", "Failed to Initialize");
	}
	return pInstance;
}

#if ACTIVATE_TOOL
void CMesh::Export(std::ofstream& _outFile, MODEL _eType)
{
	_outFile.write(reinterpret_cast<const _byte*>(m_szName), MAX_PATH);
	_outFile.write(reinterpret_cast<const _byte*>(&m_iMaterialIndex), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumVB), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumVertices), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumIndices), sizeof(_uint));

	_outFile.write(reinterpret_cast<const _byte*>(m_pVertices.get()), sizeof(_float3) * m_iNumVertices);
	_outFile.write(reinterpret_cast<const _byte*>(m_pIndices.get()), sizeof(_uint) * m_iNumIndices);

	switch (_eType)
	{
	case MODEL::ANIM:
		_outFile.write(reinterpret_cast<const _byte*>(&m_iNumBones), sizeof(_uint));
		_outFile.write(reinterpret_cast<const _byte*>(m_vecBoneIndices.data()), sizeof(_uint) * m_iNumBones);
		_outFile.write(reinterpret_cast<const _byte*>(m_vecBoneOffsets.data()), sizeof(_float4x4) * m_iNumBones);
		_outFile.write(reinterpret_cast<const _byte*>(m_pVertices_Anim.get()), sizeof(VTXMESHANIM) * m_iNumVertices);
		break;
	case MODEL::NONANIM:
		_outFile.write(reinterpret_cast<const _byte*>(m_pVertices_NonAnim.get()), sizeof(VTXMESH) * m_iNumVertices);
		break;
	}
}

HRESULT CMesh::Export(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CMesh::Export", "Failed to Open File");
	}

	Export(outFile, MODEL::NONANIM);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CMesh::Export", "Failed to Write File: Materials");
	}

	outFile.close();

	return S_OK;
}
#endif