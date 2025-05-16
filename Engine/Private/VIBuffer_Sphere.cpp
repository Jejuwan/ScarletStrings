#include "EnginePCH.h"
#include "VIBuffer_Sphere.h"

#include "GeometryGenerator.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_SPHERE)
{
}

HRESULT CVIBuffer_Sphere::Initialize(any _iResolution)
{
	auto iRes = any_cast<_int>(_iResolution);
	auto [vecVertices, vecIndices] = CGeometryGenerator::Create_Sphere(iRes, iRes);

	m_iNumVB = 1;
	m_iNumVertices = _uint(vecVertices.size());
	m_iVertexStride = sizeof(VTXPOSNORTEX);
	m_iNumIndices = (_uint)vecIndices.size();
	m_iIndexStride = sizeof(_ulong);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = vecVertices.data();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem = vecIndices.data();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

#pragma endregion

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to Initialize");
	}

	return S_OK;
}

shared_ptr<CVIBuffer_Sphere> CVIBuffer_Sphere::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any _iResolution)
{
	shared_ptr<CVIBuffer_Sphere> pInstance = make_private_shared(CVIBuffer_Sphere, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_iResolution)))
	{
		MSG_RETURN(nullptr, "CVIBuffer_Sphere::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_Sphere::Clone(any)
{
	return shared_from_componenet();
}
