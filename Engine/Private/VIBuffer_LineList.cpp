#include "EnginePCH.h"
#include "VIBuffer_LineList.h"

CVIBuffer_LineList::CVIBuffer_LineList(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_LINELIST)
{
}

HRESULT CVIBuffer_LineList::Initialize(any Any)
{
	m_iNumVB		= 1;
	m_iNumVertices	= any_cast<_uint>(Any)*2;
	m_iVertexStride	= sizeof(VTXPOS);
	m_iNumIndices	= m_iNumVertices;
	m_iIndexStride	= sizeof(_uint);
	m_eIndexFormat	= DXGI_FORMAT_R32_UINT;
	m_eTopology		= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	m_pVertices		= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices		= Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	auto pVertices = Function::CreateDynamicArray<VTXPOS>(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition = _float3(0.f, 0.f, 0.f);
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_LineList::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumIndices * m_iIndexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		pIndices[i] = i;
	}

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pIndices.get();

	memcpy(m_pIndices.get(), pIndices.get(), m_iNumIndices * m_iIndexStride);

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_LineList::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_LineList::Initialize", "Failed to Initialize");
	}

	return S_OK;
}

HRESULT CVIBuffer_LineList::Update(_float3* Position)
{
	D3D11_MAPPED_SUBRESOURCE MapResource;
	if (SUCCEEDED(m_pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MapResource)))
	{
		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			((VTXPOS*)(MapResource.pData))[i].vPosition = Position[i];
		}
		m_pContext->Unmap(m_pVB.Get(), 0);
	}

	return S_OK;
}

shared_ptr<CVIBuffer_LineList> CVIBuffer_LineList::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any Any)
{
	shared_ptr<CVIBuffer_LineList> pInstance = make_private_shared(CVIBuffer_LineList, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(Any)))
	{
		MSG_RETURN(nullptr, "CVIBuffer_LineList::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_LineList::Clone(any _arg)
{
	return shared_from_componenet();
}
