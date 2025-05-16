#include "EnginePCH.h"
#include "ConstantBuffer.h"

CConstantBuffer::CConstantBuffer(SHADER_STAGE _eShaderStage, const size_t _nLength)
	: m_eShaderStage(_eShaderStage)
	, m_nLength		(_nLength)
{
}

HRESULT CConstantBuffer::Initialize(ComPtr<ID3D11Device> _pDevice)
{
	D3D11_BUFFER_DESC tBufferDesc{};
	tBufferDesc.ByteWidth				= static_cast<UINT>(m_nLength);
	tBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	tBufferDesc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	tBufferDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.MiscFlags				= 0;
	tBufferDesc.StructureByteStride		= 0;

	if (FAILED(_pDevice->CreateBuffer(&tBufferDesc, nullptr, m_pBuffer.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CConstantBuffer::Initialize", "Failed to CreateBuffer");
	}

	return S_OK;
}

HRESULT CConstantBuffer::Update(ComPtr<ID3D11DeviceContext> _pContext, void* _pData, size_t _iLength, _uint _iOffset)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	if (SUCCEEDED(_pContext->Map(m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubResource)))
	{
		memcpy(static_cast<_byte*>(tSubResource.pData) + _iOffset, _pData, _iLength ? _iLength : m_nLength);
		_pContext->Unmap(m_pBuffer.Get(), 0);

		return S_OK;
	}

	MSG_RETURN(E_FAIL, "CConstantBuffer::Update", "Failed to Map D3D11_MAPPED_SUBRESOURCE");
}

HRESULT CConstantBuffer::Bind_ConstantBuffer(ComPtr<ID3D11DeviceContext> _pContext, _uint _iSlot)
{
	switch (m_eShaderStage)
	{
	case SHADER_STAGE::VS:
		_pContext->VSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	case SHADER_STAGE::HS:
		_pContext->HSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	case SHADER_STAGE::DS:
		_pContext->DSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	case SHADER_STAGE::GS:
		_pContext->GSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	case SHADER_STAGE::PS:
		_pContext->PSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	case SHADER_STAGE::CS:
		_pContext->CSSetConstantBuffers(_iSlot, 1, m_pBuffer.GetAddressOf());
		break;
	}

	return S_OK;
}

shared_ptr<CConstantBuffer> CConstantBuffer::Create(SHADER_STAGE _eShaderStage, ComPtr<ID3D11Device> _pDevice, const size_t _nLength)
{
	if (SHADER_STAGE::MAX == _eShaderStage || _nLength % 16)
	{
		MSG_RETURN(nullptr, "CConstantBuffer::Create", "Invalid Parameter");
	}

	shared_ptr<CConstantBuffer> pInstance = make_private_shared(CConstantBuffer, _eShaderStage, _nLength);

	if (FAILED(pInstance->Initialize(_pDevice)))
	{
		MSG_RETURN(nullptr, "CConstantBuffer::Create", "Failed to Initialize");
	}

	return pInstance;
}
