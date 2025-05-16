#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CConstantBuffer_Manager final : public CSingleton<CConstantBuffer_Manager>
{
private:
	typedef vector<shared_ptr<class CConstantBuffer>>	CONSTANTBUFFERS;

private:
	explicit CConstantBuffer_Manager() DEFAULT;
	virtual ~CConstantBuffer_Manager() DEFAULT;

public:
	HRESULT												Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);

public:		
	HRESULT												Create_Buffer(SHADER_STAGE, _uint iSlot, const size_t nLength, _bool bReleaseBuffer = false);
	HRESULT												Update_Buffer(SHADER_STAGE, _uint iSlot, void* pData, size_t nLength = 0, _uint iOffset = 0);
	HRESULT												Bind_Buffer(SHADER_STAGE, _uint iSlot);
	HRESULT												Release_Buffer(SHADER_STAGE, _uint iSlot);

private:		
	vector<CONSTANTBUFFERS>								m_vecConstantBuffers;

	ComPtr<ID3D11Device>								m_pDevice;
	ComPtr<ID3D11DeviceContext>							m_pContext;

	friend CSingleton<CConstantBuffer_Manager>;
};

END
