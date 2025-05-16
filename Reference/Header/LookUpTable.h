#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLookUpTable final
{
private:
	explicit CLookUpTable(DXGI_FORMAT, _uint2 vResolution);
	virtual ~CLookUpTable() DEFAULT;

public:
	HRESULT								Initialize(ComPtr<ID3D11Device>);
	HRESULT								Update(ComPtr<ID3D11DeviceContext>, const void* pData,  size_t nLength);
	HRESULT								Bind_ShaderResourceView(shared_ptr<class CShader>, const _char* szConstantName);

public:
	ComPtr<ID3D11ShaderResourceView>	Get_ShaderResourceView() const	{ return m_pShaderResourceView; }

private:
	ComPtr<ID3D11Texture2D>				m_pTexture2D;
	ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView;
	const DXGI_FORMAT					m_eFormat		= DXGI_FORMAT_FORCE_UINT;
	const _uint2						m_vResolution	= _uint2(0u, 0u);

public:
	static shared_ptr<CLookUpTable>		Create(ComPtr<ID3D11Device>, _uint2 vResolution, DXGI_FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT);
};

END
