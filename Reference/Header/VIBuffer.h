#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const COMPONENT);
	explicit CVIBuffer(const CVIBuffer&);
	virtual ~CVIBuffer() DEFAULT;

public:
	HRESULT							Initialize(any = g_aNull);
	HRESULT							Render();
	HRESULT							Render(shared_ptr<class CShader>, _uint iPassIndex, _bool bResetFlag = true);
	HRESULT							Render(shared_ptr<class CShader>, shared_ptr<class CTransform>, _uint iPassIndex, _bool bResetFlag = true);

public:
	_uint							Get_NumVertices() const		{ return m_iNumVertices; }
	_uint							Get_NumIndices() const		{ return m_iNumIndices; }
	_uint							Get_NumInstances() const	{ return m_iNumInstances; }

public:
	_bool							Intersect(_In_opt_ const _float4x4 _mWorld = g_mUnit) const;
	_bool							Intersect(_Out_ _float3&, _In_opt_ const _float4x4 _mWorld = g_mUnit) const;

	void							Iterate_Polygons(function<_bool(POLYGON)>);
	void							Iterate_Indices(function<_bool(_uint i0, _uint i1, _uint i2)>);

	void							Copy_Vertices(void* pDest);
	void							Copy_Indices(void* pDest);

protected:
	ComPtr<ID3D11Buffer>			m_pVB;
	ComPtr<ID3D11Buffer>			m_pIB;

	D3D11_BUFFER_DESC				m_tBufferDesc{};
	D3D11_SUBRESOURCE_DATA			m_tInitializeData{};
	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology;
	DXGI_FORMAT						m_eIndexFormat;

	_uint							m_iNumVB					= 0;
	_uint 							m_iNumVertices				= 0;
	_uint							m_iVertexStride				= 0;
	_uint 							m_iNumIndices				= 0;
	_uint							m_iIndexStride				= 0;

	_uint							m_iNumInstances				= 0;
	_uint							m_iIndicesCountPerInstance	= 0;

	vector<ComPtr<ID3D11Buffer>>	m_vecVB;
	vector<_uint>					m_vecVertexStride;
	vector<_uint>					m_vecVertexOffset;

	unique_ptr<_float3[]>			m_pVertices;
	unique_ptr<_uint[]>				m_pIndices;

#if WIP_FRUSTRUM_CULLING
private:
	BoundingBox						m_tBoundingBox;
#endif

public:
	virtual shared_ptr<CComponent>	Clone(any = g_aNull) override	PURE;
};

END
