#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CComputeAccelerator final : public CSingleton<CComputeAccelerator>
{
private:
	typedef map<wstring, ComPtr<ID3D11Buffer>>				SYSTEM_BUF;
	typedef map<wstring, ComPtr<ID3D11ShaderResourceView>>	SYSTEM_SRV;
	typedef map<wstring, ComPtr<ID3D11UnorderedAccessView>>	SYSTEM_UAV;
	typedef map<wstring, D3D11_BUFFER_DESC>					SYSTEM_BUFDESC;

private:
	explicit CComputeAccelerator() DEFAULT;
	virtual ~CComputeAccelerator() DEFAULT;

public:
	HRESULT																Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, shared_ptr<class CShader>);

public:
	shared_ptr<class CShader>											Get_Shader() const	{ return m_pShader; }

public:
	HRESULT																Register_ShaderResourceView(shared_ptr<class ISystem>, const wstring& wstrTag, _uint iElementSize, _uint iCount, void* pData = nullptr);
	HRESULT																Register_UnorderedAccessView(shared_ptr<class ISystem>, const wstring& wstrTag, _uint iElementSize, _uint iCount, void* pData = nullptr);

	HRESULT																Erase_System(shared_ptr<class ISystem>);

	HRESULT																Update_Data(shared_ptr<class ISystem>, const wstring& wstrSRVTag, const wstring& wstrUAVTag, const _char* szSRV, const _char* szUAV, void* pSrc, size_t nSize = 0);
	HRESULT																Receive_Data(shared_ptr<class ISystem>, const wstring& wstrTag, void* pDst, size_t nSize = 0);

	HRESULT																Compute(_uint iPass, _uint iX, _uint iY, _uint iZ);

private:
	ComPtr<ID3D11Buffer>												Create_StructedBuffer(D3D11_BIND_FLAG, _uint iElementSize, _uint iCount, void* pData = nullptr);
	ComPtr<ID3D11ShaderResourceView>									Create_ShaderResourceView(ComPtr<ID3D11Buffer>);
	ComPtr<ID3D11UnorderedAccessView>									Create_UnorderedAccessView(ComPtr<ID3D11Buffer>);

private:
	map<weak_ptr<class ISystem>, SYSTEM_BUF, std::owner_less<>>			m_mapSystemSRVBuffer;
	map<weak_ptr<class ISystem>, SYSTEM_BUF, std::owner_less<>>			m_mapSystemUAVBuffer;
	map<weak_ptr<class ISystem>, SYSTEM_BUF, std::owner_less<>>			m_mapSystemResultBuffer;
	map<weak_ptr<class ISystem>, SYSTEM_SRV, std::owner_less<>>			m_mapSystemShaderResourceView;
	map<weak_ptr<class ISystem>, SYSTEM_UAV, std::owner_less<>>			m_mapSystemUnorderedAccessView;
	map<weak_ptr<class ISystem>, SYSTEM_BUFDESC, std::owner_less<>>		m_mapSystemSRVDesc;
	map<weak_ptr<class ISystem>, SYSTEM_BUFDESC, std::owner_less<>>		m_mapSystemUAVDesc;

	shared_ptr<class CShader>											m_pShader;

private:
	ComPtr<ID3D11Device>												m_pDevice;
	ComPtr<ID3D11DeviceContext>											m_pContext;

	friend class CSingleton<CComputeAccelerator>;
};

END
