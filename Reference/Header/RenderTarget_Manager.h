#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CRenderTarget_Manager final : public CSingleton<CRenderTarget_Manager>
{
private:
	explicit CRenderTarget_Manager() DEFAULT;
	virtual ~CRenderTarget_Manager();

public:
	HRESULT															Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);

public:
	ComPtr<ID3D11Texture2D>											Get_Texture2D(const wstring& wstrTargetTag);
	ComPtr<ID3D11ShaderResourceView>								Get_ShaderResourceView(const wstring& wstrTargetTag);
	ComPtr<ID3D11RenderTargetView>									Get_RenderTargetView(const wstring& wstrTargetTag);

public:
	HRESULT															Add_RenderTarget(const wstring& wstrTargetTag, const _uint2 vResolution, const DXGI_FORMAT, const _color vClear);
	HRESULT															Add_DepthStencil(const wstring& wstrDepthStencilTag, const _uint2 vResolution);

	HRESULT															Push_RenderTarget(const wstring& wstrMultiTargetTag, const wstring& wstrTargetTag);
	HRESULT															Push_DepthStencil(const wstring& wstrMultiTargetTag, const wstring& wstrDepthStencilTag);
	HRESULT															Erase_RenderTarget(const wstring& wstrMultiTargetTag, const wstring& wstrTargetTag);

	HRESULT															Begin_MultiRenderTaget(const wstring& wstrMultiTargetTag, _uint iCopyResource = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, initializer_list<_uint> ilPreserve = {});
	HRESULT															End_MultiRenderTarget(const wstring& wstrTargetTag = wstring());

	HRESULT															Bind_RenderTarget(const wstring& wstrTargetTag, shared_ptr<class CShader>, const _char* szConstantName);

public:
	HRESULT															Copy_From_BackBuffer(const wstring& wstrTargetTag);
	HRESULT															Copy_To_BackBuffer(const wstring& wstrTargetTag);

	HRESULT															Copy_RenderTarget(const wstring& wstrDestTargetTag, const wstring& wstrSrcTargetTag);

private:
	inline _bool													Exist_MultiRenderTarget(const wstring& wstrMultiTargetTag);
	inline _bool													Exist_MultiDepthStencil(const wstring& wstrMultiDepthStencilTag);
	inline _bool													Exist_RenderTarget(const wstring& wstrTargetTag);
	inline _bool													Exist_DepthStencil(const wstring& wstrDepthStencilTag);

private:
	unordered_map<wstring, list<shared_ptr<class CRenderTarget>>>	m_umapMultiRenderTarget;
	unordered_map<wstring, ComPtr<ID3D11DepthStencilView>>			m_umapMultiDepthStencil;
	unordered_map<wstring, shared_ptr<class CRenderTarget>>			m_umapRenderTarget;
	unordered_map<wstring, ComPtr<ID3D11DepthStencilView>>			m_umapDepthStencil;

	ComPtr<ID3D11RenderTargetView>									m_pBackBufferView;
	ComPtr<ID3D11DepthStencilView>									m_pDepthStencilView;

	ComPtr<ID3D11Device>											m_pDevice;
	ComPtr<ID3D11DeviceContext>										m_pContext;

	_bool															m_bOriginalTarget	= true;

	friend CSingleton<CRenderTarget_Manager>;
};

END
