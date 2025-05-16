#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CConstantBuffer final
{
private:
	explicit CConstantBuffer(SHADER_STAGE, const size_t nLength);
	virtual ~CConstantBuffer() DEFAULT;

public:
	HRESULT								Initialize(ComPtr<ID3D11Device>);
	HRESULT								Update(ComPtr<ID3D11DeviceContext>, void* pData, size_t nLength = 0, _uint iOffset = 0);
	HRESULT								Bind_ConstantBuffer(ComPtr<ID3D11DeviceContext>, _uint iSlot);

private:
	ComPtr<ID3D11Buffer>				m_pBuffer;
	const SHADER_STAGE					m_eShaderStage = SHADER_STAGE::MAX;
	const size_t						m_nLength = 0;

public:
	static shared_ptr<CConstantBuffer>	Create(SHADER_STAGE, ComPtr<ID3D11Device>, const size_t nLength);
};

END