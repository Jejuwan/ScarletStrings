#include "EnginePCH.h"
#include "LookUpTable.h"
#include "Shader.h"

CLookUpTable::CLookUpTable(DXGI_FORMAT _eFormat, _uint2 _vResolution)
	: m_eFormat		(_eFormat)
	, m_vResolution	(_vResolution)
{
}

HRESULT CLookUpTable::Initialize(ComPtr<ID3D11Device> _pDevice)
{
	D3D11_TEXTURE2D_DESC tTexture2DDesc{};
	tTexture2DDesc.Width				= m_vResolution.x;
	tTexture2DDesc.Height				= m_vResolution.y;
	tTexture2DDesc.MipLevels			= 1;
	tTexture2DDesc.ArraySize			= 1;
	tTexture2DDesc.Format				= m_eFormat;

	tTexture2DDesc.SampleDesc.Count		= 1;
	tTexture2DDesc.SampleDesc.Quality	= 0;

	tTexture2DDesc.Usage				= D3D11_USAGE_DYNAMIC;
	tTexture2DDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	tTexture2DDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	tTexture2DDesc.MiscFlags			= 0;

	if (FAILED(_pDevice->CreateTexture2D(&tTexture2DDesc, nullptr, m_pTexture2D.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CLookUpTable::Initialize", "Failed to CreateTexture2D");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tShaderResourceViewDesc{};
	tShaderResourceViewDesc.Format						= tTexture2DDesc.Format;
	tShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	tShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	tShaderResourceViewDesc.Texture2D.MipLevels			= 1;

	if (FAILED(_pDevice->CreateShaderResourceView(m_pTexture2D.Get(), &tShaderResourceViewDesc, m_pShaderResourceView.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget::Initialize", "Failed to CreateShaderResourceView");
	}

	return S_OK;
}

HRESULT CLookUpTable::Update(ComPtr<ID3D11DeviceContext> _pContext, const void* _pData, size_t _nLength)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	if (SUCCEEDED(_pContext->Map(m_pTexture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubResource)))
	{
		memcpy(tSubResource.pData, _pData, _nLength);
		_pContext->Unmap(m_pTexture2D.Get(), 0);

#ifdef _DEBUG
		SaveDDSTextureToFile(_pContext.Get(), m_pTexture2D.Get(), TEXT("Bin/Resources/DebugLUT.dds"));
#endif
		return S_OK;
	}

	MSG_RETURN(E_FAIL, "CLookUpTable::Update", "Failed to Map");
}

HRESULT CLookUpTable::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, const _char* _szConstantName)
{
	if (FAILED(_pShader->Bind_ShaderResourceView(_szConstantName, m_pShaderResourceView.Get())))
	{
		MSG_RETURN(E_FAIL, "CLookUpTable::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

shared_ptr<CLookUpTable> CLookUpTable::Create(ComPtr<ID3D11Device> _pDevice, _uint2 _vResolution, DXGI_FORMAT _eFormat)
{
	shared_ptr<CLookUpTable> pInstance = make_private_shared(CLookUpTable, _eFormat, _vResolution);

	if (FAILED(pInstance->Initialize(_pDevice)))
	{
		MSG_RETURN(nullptr, "CLookUpTable::Create", "Failed to Initalize");
	}

	return pInstance;
}
