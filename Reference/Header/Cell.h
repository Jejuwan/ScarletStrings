#pragma once

#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)
class CNavigation;
class CVIBuffer_LineList;

class CCell : public ISystem
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	explicit CCell(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCell(const CCell&);
	virtual ~CCell() DEFAULT;

public:
	const _float3& Get_Point(POINT ePoint) const {
		return m_vPoints[ePoint];
	}

public:

	void SetUp_Neighbor(LINE eLine, shared_ptr<CCell> pNeighborCell) {

		m_iNeighborIndices[eLine] = pNeighborCell->m_iIndex;
	}

public:
	HRESULT Initialize(any Any);
	_bool Compare_Points(const _float3& vSourPoint, const _float3& vDestPoint);
	_bool is_In(_vectorf vPosition, _int* pNeighborIndex);

public:
	_float3 Get_FirstPos() { return m_vFirstPos; };
	_float3 Get_SecondPos() { return m_vSecondPos; };

private:
	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

private:
	_float3				m_vPoints[POINT_END];
	_int				m_iNeighborIndices[LINE_END] = { -1, -1, -1 };
	_int				m_iIndex = { 0 };

	_float3				m_vFirstPos = { 0.f,0.f,0.f };
	_float3				m_vSecondPos = { 0.f,0.f,0.f };

	shared_ptr<class CVIBuffer_LineList> m_pVIBuffer_LineList;


public:
	static shared_ptr<CCell> Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any Any);


};

END