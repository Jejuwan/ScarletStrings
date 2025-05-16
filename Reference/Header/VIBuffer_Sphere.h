#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
private:
	explicit CVIBuffer_Sphere(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CVIBuffer_Sphere() DEFAULT;

public:
	HRESULT								Initialize(any = g_aNull);

public:
	static shared_ptr<CVIBuffer_Sphere>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any iResolution = 64);
	virtual shared_ptr<CComponent>		Clone(any = g_aNull) override;
};

END