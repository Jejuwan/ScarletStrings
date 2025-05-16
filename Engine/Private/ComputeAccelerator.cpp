#include "EnginePCH.h"
#include "ComputeAccelerator.h"
#include "System.h"
#include "Shader.h"

HRESULT CComputeAccelerator::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, shared_ptr<CShader> _pShader)
{
	m_pDevice	= _pDevice;
	m_pContext	= _pContext;
	m_pShader	= _pShader;

	return S_OK;
}

HRESULT CComputeAccelerator::Register_ShaderResourceView(shared_ptr<ISystem> _pSystem, const wstring& _wstrTag, _uint _iElementSize, _uint _iCount, void* _pData)
{
	D3D11_BUFFER_DESC tDesc{};
	ComPtr<ID3D11Buffer> pBuffer;
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	pBuffer = Create_StructedBuffer(D3D11_BIND_SHADER_RESOURCE, _iElementSize, _iCount, _pData);
	if (nullptr == pBuffer)
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Register_ShaderResourceView", "Failed to Create_StructedBuffer");
	}

	pShaderResourceView = Create_ShaderResourceView(pBuffer);
	if (nullptr == pShaderResourceView)
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Register_ShaderResourceView", "Failed to Create_ShaderResourceView");
	}

	m_mapSystemSRVBuffer[_pSystem].emplace(_wstrTag, pBuffer);
	m_mapSystemShaderResourceView[_pSystem].emplace(_wstrTag, pShaderResourceView);

	pBuffer->GetDesc(&tDesc);
	m_mapSystemSRVDesc[_pSystem].emplace(_wstrTag, tDesc);

	return S_OK;
}

HRESULT CComputeAccelerator::Register_UnorderedAccessView(shared_ptr<ISystem> _pSystem, const wstring& _wstrTag, _uint _iElementSize, _uint _iCount, void* _pData)
{
	D3D11_BUFFER_DESC tDesc{};
	ComPtr<ID3D11Buffer> pBuffer, pResultBuffer;
	ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;

	pBuffer = Create_StructedBuffer(D3D11_BIND_UNORDERED_ACCESS, _iElementSize, _iCount, _pData);
	if (nullptr == pBuffer)
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Register_UnorderedAccessView", "Failed to Create_StructedBuffer");
	}

	pUnorderedAccessView = Create_UnorderedAccessView(pBuffer);
	if (nullptr == pUnorderedAccessView)
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Register_UnorderedAccessView", "Failed to Create_UnorderedAccessView");
	}

	m_mapSystemUAVBuffer[_pSystem].emplace(_wstrTag, pBuffer);
	m_mapSystemUnorderedAccessView[_pSystem].emplace(_wstrTag, pUnorderedAccessView);

	pBuffer->GetDesc(&tDesc);
	tDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_READ;
	tDesc.Usage				= D3D11_USAGE_STAGING;
	tDesc.BindFlags			= 0;
	tDesc.MiscFlags			= 0;

	m_mapSystemUAVDesc[_pSystem].emplace(_wstrTag, tDesc);

	if (SUCCEEDED(m_pDevice->CreateBuffer(&tDesc, nullptr, pResultBuffer.GetAddressOf())))
	{
		m_mapSystemResultBuffer[_pSystem].emplace(_wstrTag, pResultBuffer);
		return S_OK;
	}

	MSG_RETURN(E_FAIL, "CComputeAccelerator::Register_UnorderedAccessView", "Failed to CreateBuffer");
}

HRESULT CComputeAccelerator::Erase_System(shared_ptr<ISystem> _pSystem)
{
	m_mapSystemSRVBuffer.erase(_pSystem);
	m_mapSystemUAVBuffer.erase(_pSystem);
	m_mapSystemResultBuffer.erase(_pSystem);
	m_mapSystemShaderResourceView.erase(_pSystem);
	m_mapSystemUnorderedAccessView.erase(_pSystem);
	m_mapSystemSRVDesc.erase(_pSystem);
	m_mapSystemUAVDesc.erase(_pSystem);

	return S_OK;
}

HRESULT CComputeAccelerator::Update_Data(shared_ptr<ISystem> _pSystem, const wstring& _wstrSRVTag, const wstring& _wstrUAVTag, const _char* _szSRV, const _char* _szUAV, void* _pSrc, size_t _nSize)
{
	if (m_mapSystemSRVBuffer.end() == m_mapSystemSRVBuffer.find(_pSystem))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Update_Data", "Invalid System");
	}
	if (m_mapSystemSRVBuffer[_pSystem].end() == m_mapSystemSRVBuffer[_pSystem].find(_wstrSRVTag))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Update_Data", "Invalid Tag");
	}

	D3D11_MAPPED_SUBRESOURCE tSubResource{};
	if (FAILED(m_pContext->Map(m_mapSystemSRVBuffer[_pSystem][_wstrSRVTag].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubResource)))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Update_Data", "Failed to Map");
	}
	memcpy(tSubResource.pData, _pSrc, _nSize ? _nSize : m_mapSystemSRVDesc[_pSystem][_wstrSRVTag].ByteWidth);
	m_pContext->Unmap(m_mapSystemSRVBuffer[_pSystem][_wstrSRVTag].Get(), 0);

	if (FAILED(m_pShader->Bind_ShaderResourceView(_szSRV, m_mapSystemShaderResourceView[_pSystem][_wstrSRVTag])))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Update_Data", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_UnorderedAccessView(_szUAV, m_mapSystemUnorderedAccessView[_pSystem][_wstrUAVTag])))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Update_Data", "Failed to Bind_UnorderedAccessView");
	}

	return S_OK;
}

HRESULT CComputeAccelerator::Receive_Data(shared_ptr<ISystem> _pSystem, const wstring& _wstrTag, void* _pDst, size_t _nSize)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};
	m_pContext->CopyResource(m_mapSystemResultBuffer[_pSystem][_wstrTag].Get(), m_mapSystemUAVBuffer[_pSystem][_wstrTag].Get());
	if (SUCCEEDED(m_pContext->Map(m_mapSystemResultBuffer[_pSystem][_wstrTag].Get(), 0, D3D11_MAP_READ, 0, &tSubResource)))
	{
		memcpy(_pDst, tSubResource.pData, _nSize ? _nSize : m_mapSystemUAVDesc[_pSystem][_wstrTag].ByteWidth);
		m_pContext->Unmap(m_mapSystemResultBuffer[_pSystem][_wstrTag].Get(), 0);

		return S_OK;
	}

	MSG_RETURN(E_FAIL, "CComputeAccelerator::Receive_Data", "Failed to Map");
}

ComPtr<ID3D11Buffer> CComputeAccelerator::Create_StructedBuffer(D3D11_BIND_FLAG _eFlag, _uint _iElementSize, _uint _iCount, void* _pData)
{
	ComPtr<ID3D11Buffer> pBuffer;

	D3D11_BUFFER_DESC           tDesc{};
	tDesc.ByteWidth             = _iElementSize * _iCount;
	tDesc.BindFlags             = _eFlag;
	tDesc.StructureByteStride   = _iElementSize;
	tDesc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	if (tDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		tDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage				= D3D11_USAGE_DYNAMIC;
	}

	if (_pData)
	{
		D3D11_SUBRESOURCE_DATA  tData{};
		tData.pSysMem			= _pData;
		if (FAILED(m_pDevice->CreateBuffer(&tDesc, &tData, pBuffer.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CComputeAccelerator::Create_StructedBuffer", "Failed to CreateBuffer");
		}
	}
	else
	{
		if (FAILED(m_pDevice->CreateBuffer(&tDesc, nullptr, pBuffer.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CComputeAccelerator::Create_StructedBuffer", "Failed to CreateBuffer");
		}
	}

	return pBuffer;
}

ComPtr<ID3D11ShaderResourceView> CComputeAccelerator::Create_ShaderResourceView(ComPtr<ID3D11Buffer> _pBuffer)
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	D3D11_BUFFER_DESC				tBufferDesc{};
	_pBuffer->GetDesc(&tBufferDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC	tSRVDesc{};
	tSRVDesc.ViewDimension				= D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement		= 0;

	if (tBufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		tSRVDesc.Format					= DXGI_FORMAT_R32_TYPELESS;
		tSRVDesc.BufferEx.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;
		tSRVDesc.BufferEx.NumElements	= tBufferDesc.ByteWidth / 4;
	}
	else if (tBufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		tSRVDesc.Format					= DXGI_FORMAT_UNKNOWN;
		tSRVDesc.BufferEx.NumElements	= tBufferDesc.ByteWidth / tBufferDesc.StructureByteStride;
	}
	else
	{
		return nullptr;
	}

	if (FAILED(m_pDevice->CreateShaderResourceView(_pBuffer.Get(), &tSRVDesc, pShaderResourceView.GetAddressOf())))
	{
		MSG_RETURN(nullptr, "CComputeAccelerator::Create_ShaderResourceView", "Failed to CreateShaderResourceView");
	}

	return pShaderResourceView;
}

ComPtr<ID3D11UnorderedAccessView> CComputeAccelerator::Create_UnorderedAccessView(ComPtr<ID3D11Buffer> _pBuffer)
{
	ComPtr<ID3D11UnorderedAccessView> pUnorderedAccessView;

	D3D11_BUFFER_DESC tBufferDesc{};
	_pBuffer->GetDesc(&tBufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC	tUAVDesc = {};
	tUAVDesc.ViewDimension				= D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.FirstElement		= 0;

	if (tBufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		tUAVDesc.Format					= DXGI_FORMAT_R32_TYPELESS;
		tUAVDesc.Buffer.Flags			= D3D11_BUFFER_UAV_FLAG_RAW;
		tUAVDesc.Buffer.NumElements		= static_cast<UINT>(tBufferDesc.ByteWidth / 4u);
	}
	else if (tBufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		tUAVDesc.Format					= DXGI_FORMAT_UNKNOWN;
		tUAVDesc.Buffer.NumElements		= tBufferDesc.ByteWidth / tBufferDesc.StructureByteStride;
	}
	else
	{
		return nullptr;
	}

	if (FAILED(m_pDevice->CreateUnorderedAccessView(_pBuffer.Get(), &tUAVDesc, pUnorderedAccessView.GetAddressOf())))
	{
		MSG_RETURN(nullptr, "CComputeAccelerator::Create_UnorderedAccessView", "Failed to CreateUnorderedAccessView");
	}

	return pUnorderedAccessView;
}

HRESULT CComputeAccelerator::Compute(_uint _iPass, _uint _iX, _uint _iY, _uint _iZ)
{
	if (FAILED(m_pShader->Dispatch(_iPass, _iX, _iY, _iZ)))
	{
		MSG_RETURN(E_FAIL, "CComputeAccelerator::Compute", "Failed to CShader::Dispatch");
	}

	return S_OK;
}
