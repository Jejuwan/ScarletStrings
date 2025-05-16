#include "EnginePCH.h"
#include "VIBufferInstance_Mesh.h"
#include "Mesh.h"

CVIBufferInstance_Mesh::CVIBufferInstance_Mesh(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBufferInstance(_pDevice, _pContext, COMPONENT::VIBUFFER_INSTANCE_MESH)
{
}

CVIBufferInstance_Mesh::CVIBufferInstance_Mesh(const CVIBufferInstance_Mesh& _rhs)
	: CVIBufferInstance(_rhs)
{
}

HRESULT CVIBufferInstance_Mesh::Initialize_Prototype(const wstring& _wstrPath, _uint _iNumInstance)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);
	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to Open File");
	}

	m_iNumInstances		= _iNumInstance;
	m_iInstanceStride	= sizeof(VTXPOSNORTANTEXINSTTRANSCOLORARG);

    m_iNumVB			= 2;
	m_iVertexStride		= sizeof(VTXMESH);
	m_iIndexStride		= sizeof(_uint);
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eTopology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	inFile.read(reinterpret_cast<_byte*>(m_szName),						MAX_PATH);
	inFile.seekg(sizeof(_uint), std::ios::cur);
//	inFile.read(reinterpret_cast<_byte*>(&m_iMaterialIndex),			sizeof(_uint));
	inFile.seekg(sizeof(_uint), std::ios::cur);
//	inFile.read(reinterpret_cast<_byte*>(&m_iNumVB),					sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumVertices),				sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iIndicesCountPerInstance),	sizeof(_uint));

	m_iNumIndices		= m_iNumInstances * m_iIndicesCountPerInstance;

	m_pVertices			= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices			= Function::CreateDynamicArray<_uint>(m_iNumIndices);

	inFile.read(reinterpret_cast<_byte*>(m_pVertices.get()),			sizeof(_float3)	* m_iNumVertices);
	inFile.read(reinterpret_cast<_byte*>(m_pIndices.get()),				sizeof(_uint)	* m_iIndicesCountPerInstance);
	
#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	inFile.read(reinterpret_cast<_byte*>(pVertices.get()), m_iVertexStride * m_iNumVertices);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion
#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iIndexStride * m_iNumIndices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		memcpy(&pIndices[i * m_iIndicesCountPerInstance], m_pIndices.get(), sizeof(_uint) * m_iIndicesCountPerInstance);
	}

	m_tInitializeData.pSysMem = pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to Read File");
	}

	inFile.close();

    return S_OK;
}

HRESULT CVIBufferInstance_Mesh::Initialize_Prototype(MODEL _eType, shared_ptr<CMesh> _pMesh, _uint _iNumInstance)
{
	switch (_eType)
	{
	case MODEL::ANIM:
		m_iInstanceStride	= sizeof(VTXMESHANIMINSTTRANSCOLORARG);
		m_iVertexStride		= sizeof(VTXMESHANIM);
		break;
	case MODEL::NONANIM:
		m_iInstanceStride	= sizeof(VTXMESHINSTTRANSCOLORARG);
		m_iVertexStride		= sizeof(VTXMESH);
		break;

	default:
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Invalid MODEL");
	}

	m_iNumInstances		= _iNumInstance;

    m_iNumVB			= 2;
	m_iIndexStride		= sizeof(_uint);
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eTopology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	strcpy_s(m_szName, _pMesh->Get_Name());
	m_iNumVertices				= _pMesh->Get_NumVertices();
	m_iIndicesCountPerInstance	= _pMesh->Get_NumIndices();
	m_iNumIndices				= m_iNumInstances * m_iIndicesCountPerInstance;

	m_pVertices					= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices					= Function::CreateDynamicArray<_uint>(m_iNumIndices);
	_pMesh->Copy_Vertices(m_pVertices.get());
	_pMesh->Copy_Indices(m_pIndices.get());
	
#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	switch (_eType)
	{
	case MODEL::ANIM:
	{
		m_iNumBones = _pMesh->Get_NumBones();
		m_vecBoneIndices.reserve(m_iNumBones);
		m_vecBoneOffsets.reserve(m_iNumBones);
		m_pVertices_Anim = Function::CreateDynamicArray<VTXMESHANIM>(m_iNumVertices);
		_pMesh->Copy_BoneIndices(m_vecBoneIndices.data());
		_pMesh->Copy_BoneOffsets(m_vecBoneOffsets.data());
		_pMesh->Copy_AnimVertices(m_pVertices_Anim.get());

		ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
		m_tInitializeData.pSysMem = m_pVertices_Anim.get();

		if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
		}
	}
	break;
	case MODEL::NONANIM:
	{
		m_pVertices_NonAnim = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);
		_pMesh->Copy_NonAnimVertices(m_pVertices_NonAnim.get());

		ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
		m_tInitializeData.pSysMem = m_pVertices_NonAnim.get();

		if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
		{
			MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
		}
	}
	break;
	}

#pragma endregion
#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iIndexStride * m_iNumIndices;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		memcpy(&pIndices[i * m_iIndicesCountPerInstance], m_pIndices.get(), sizeof(_uint) * m_iIndicesCountPerInstance);
	}

	m_tInitializeData.pSysMem = pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion

	return S_OK;
}

HRESULT CVIBufferInstance_Mesh::Initialize_Prototype(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumInstances), sizeof(_uint));
	m_iInstanceStride = sizeof(VTXPOSNORTANTEXINSTTRANSCOLORARG);

	m_iNumVB = 2;
	m_iVertexStride = sizeof(VTXMESH);
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_inFile.read(reinterpret_cast<_byte*>(m_szName), MAX_PATH);
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumVertices), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iIndicesCountPerInstance), sizeof(_uint));

	m_iNumIndices = m_iNumInstances * m_iIndicesCountPerInstance;

	m_pVertices = Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	_inFile.read(reinterpret_cast<_byte*>(m_pVertices.get()), sizeof(_float3) * m_iNumVertices);
	_inFile.read(reinterpret_cast<_byte*>(m_pIndices.get()), sizeof(_uint) * m_iIndicesCountPerInstance);

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	m_pVertices_NonAnim = Function::CreateDynamicArray<VTXMESH>(m_iNumVertices);

	_inFile.read(reinterpret_cast<_byte*>(m_pVertices_NonAnim.get()), sizeof(VTXMESH) * m_iNumVertices);

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = m_pVertices_NonAnim.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
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

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);
	for (_uint i = 0; i < m_iNumInstances; ++i)
	{
		memcpy(&pIndices[i * m_iIndicesCountPerInstance], m_pIndices.get(), sizeof(_uint) * m_iIndicesCountPerInstance);
	}

	m_tInitializeData.pSysMem = pIndices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to CreateBuffer");
	}

#pragma endregion

	if (_inFile.fail() || _inFile.eof())
	{
		_inFile.clear();
		_inFile.close();
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize_Prototype", "Failed to Read File");
	}

	return S_OK;
}

HRESULT CVIBufferInstance_Mesh::Initialize(any)
{
	#pragma region INSTANCE_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumInstances * m_iInstanceStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	switch (m_iVertexStride)
	{
		case sizeof(VTXMESHANIM):
		{
			auto pInstanceVertices = Function::CreateDynamicArray<VTXMESHANIMINSTTRANSCOLORARG>(m_iNumInstances);
			m_tInitializeData.pSysMem = pInstanceVertices.get();

			if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pInstanceVB.GetAddressOf())))
			{
				MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize", "Failed to CreateBuffer");
			}
		}
		break;
		case sizeof(VTXMESH):
		{
			auto pInstanceVertices = Function::CreateDynamicArray<VTXMESHINSTTRANSCOLORARG>(m_iNumInstances);
			m_tInitializeData.pSysMem = pInstanceVertices.get();

			if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pInstanceVB.GetAddressOf())))
			{
				MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize", "Failed to CreateBuffer");
			}
		}
		break;
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Initialize", "Failed to CVIBuffer::Initialize");
	}

    return S_OK;
}

shared_ptr<CVIBufferInstance_Mesh> CVIBufferInstance_Mesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrPath, _uint _iNumInstance)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared(CVIBufferInstance_Mesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_wstrPath, _iNumInstance)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CVIBufferInstance_Mesh> CVIBufferInstance_Mesh::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, MODEL _eType, shared_ptr<CMesh> _pMesh, _uint _iNumInstance)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared(CVIBufferInstance_Mesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_eType, _pMesh, _iNumInstance)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBufferInstance_Mesh::Clone(any _arg)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared_copy(CVIBufferInstance_Mesh, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Clone", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CVIBufferInstance_Mesh> CVIBufferInstance_Mesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrPath)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Read", "Failed to Open File");
	}

	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared(CVIBufferInstance_Mesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(inFile)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Read", "Failed to Initialize_Prototype");
	}

	inFile.close();

	return pInstance;
}

shared_ptr<CVIBufferInstance_Mesh> CVIBufferInstance_Mesh::Read(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, std::ifstream& _inFile)
{
	shared_ptr<CVIBufferInstance_Mesh> pInstance = make_private_shared(CVIBufferInstance_Mesh, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_inFile)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Mesh::Read", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
void CVIBufferInstance_Mesh::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumInstances), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(m_szName), MAX_PATH);
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumVertices), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iIndicesCountPerInstance), sizeof(_uint));

	_outFile.write(reinterpret_cast<const _byte*>(m_pVertices.get()), sizeof(_float3) * m_iNumVertices);
	_outFile.write(reinterpret_cast<const _byte*>(m_pIndices.get()), sizeof(_uint) * m_iIndicesCountPerInstance);

	_outFile.write(reinterpret_cast<const _byte*>(m_pVertices_NonAnim.get()), sizeof(VTXMESH) * m_iNumVertices);
}

HRESULT CVIBufferInstance_Mesh::Export(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Export", "Failed to Open File");
	}

	Export(outFile);

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CVIBufferInstance_Mesh::Export", "Failed to Export");
	}

	outFile.close();

	return S_OK;
}
#endif
