#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Model final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Model(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Model(const CVIBufferInstance_Model&);
	virtual ~CVIBufferInstance_Model() DEFAULT;

public:
	HRESULT										Initialize_Prototype(_uint iNumInstance);
	HRESULT										Initialize(any = g_aNull);

public:
	HRESULT										Update_InstanceBuffer(const vector<shared_ptr<class CTransform>>&);
public:
	void										Bind_InstanceBuffer(_uint iSlot);

private:
	_char										m_szName[MAX_PATH]	= "";

	_uint										m_iMaterialIndex = 0;
	unique_ptr<VTXMESH[]>						m_pVertices_NonAnim;

private:
	vector<_float4x4>							m_vecInstanceMatrix;

public:
	static shared_ptr<CVIBufferInstance_Model>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;
};

END
