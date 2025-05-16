#include "EnginePCH.h"
#include "VIBufferInstance_Model.h"

#include "Transform.h"

CVIBufferInstance_Model::CVIBufferInstance_Model(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBufferInstance(_pDevice, _pContext, COMPONENT::VIBUFFER_INSTANCE_MODEL)
{
}

CVIBufferInstance_Model::CVIBufferInstance_Model(const CVIBufferInstance_Model& _rhs)
	: CVIBufferInstance(_rhs)
{
}

HRESULT CVIBufferInstance_Model::Initialize_Prototype(_uint _iNumInstance)
{
	m_iInstanceStride = sizeof(_float4x4);

	m_iNumInstances = _iNumInstance;
	m_vecInstanceMatrix.resize(_iNumInstance, XMMatrixIdentity());

	D3D11_BUFFER_DESC tDesc;
	ZeroMemory(&tDesc, sizeof(D3D11_BUFFER_DESC));
	tDesc.ByteWidth = m_iInstanceStride * m_iNumInstances;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSubResource;
	ZeroMemory(&tSubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	tSubResource.pSysMem = m_vecInstanceMatrix.data();

	m_pDevice->CreateBuffer(&tDesc, &tSubResource, m_pInstanceVB.GetAddressOf());
	return S_OK;
}

HRESULT CVIBufferInstance_Model::Initialize(any)
{
	return S_OK;
}

HRESULT CVIBufferInstance_Model::Update_InstanceBuffer(const vector<shared_ptr<class CTransform>>& vecInstanceTransform)
{
	// Update Instance Buffer
	for (size_t i = 0; i < vecInstanceTransform.size(); ++i)
	{
		const _float4x4& matWorld = vecInstanceTransform[i]->Get_Matrix();
		memcpy(&m_vecInstanceMatrix[i], &matWorld, sizeof(_float4x4));
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	m_pContext->Map(m_pInstanceVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, m_vecInstanceMatrix.data(), sizeof(_float4x4) * vecInstanceTransform.size());
	}
	m_pContext->Unmap(m_pInstanceVB.Get(), 0);

	return S_OK;
}

void CVIBufferInstance_Model::Bind_InstanceBuffer(_uint iSlot)
{
	_uint iOffset = 0;
	m_pContext->IASetVertexBuffers(iSlot, 1, m_pInstanceVB.GetAddressOf(), &m_iInstanceStride, &iOffset);
}

shared_ptr<CVIBufferInstance_Model> CVIBufferInstance_Model::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _uint _iNumInstance)
{
	shared_ptr<CVIBufferInstance_Model> pInstance = make_private_shared(CVIBufferInstance_Model, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_iNumInstance)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Model::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBufferInstance_Model::Clone(any _arg)
{
	shared_ptr<CVIBufferInstance_Model> pInstance = make_private_shared_copy(CVIBufferInstance_Model, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CVIBufferInstance_Model::Clone", "Failed to Initialize");
	}

	return pInstance;
}
