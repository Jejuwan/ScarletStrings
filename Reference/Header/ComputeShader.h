#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CComputeShader final
{
private:
	explicit CComputeShader(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CComputeShader(const CComputeShader&);
	virtual ~CComputeShader() DEFAULT;

public:
	static shared_ptr<CComputeShader>	Create();
};

END
