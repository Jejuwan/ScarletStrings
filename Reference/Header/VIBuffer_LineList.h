#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_LineList final : public CVIBuffer
{
private:
	explicit CVIBuffer_LineList(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CVIBuffer_LineList() DEFAULT;

public:
	HRESULT								Initialize(any = g_aNull);
	HRESULT								Update(_float3* Position);
public:
	static shared_ptr<CVIBuffer_LineList>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any Any);
	virtual shared_ptr<CComponent>		Clone(any = g_aNull) override;
};

END
