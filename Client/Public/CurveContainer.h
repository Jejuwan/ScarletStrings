#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class ICurves;
END

BEGIN(Client)

class CCurveContainer final
{
private:
	explicit CCurveContainer() DEFAULT;
	explicit CCurveContainer(const CCurveContainer&);
	virtual ~CCurveContainer() DEFAULT;

public:
	void								Initialize();

public:
	void								Add_Curve(shared_ptr<class CCurvePlot>, _uint iIndex, _float fDuration, _float4 vValue);
	void								Remove_Curve(_uint iIndex);

	const _uint							Get_CurveCount() const					{ return static_cast<_uint>(m_lstCurvePlots.size()); }
	const _float						Get_Total() const						{ return m_fTotal; }
	const _float						Get_Duration(_uint iIndex) const		{ return m_vecTimeTags[iIndex]; }
	const _float4						Get_ContainerValue(_uint iIndex) const	{ return m_vecValues[iIndex]; }
	const _float						Get_CurveValue(_float fRatio) const;
	const _float4						Get_Value(_float fRatio) const;
	shared_ptr<class CCurvePlot>		Get_CurvePlot(_uint iIndex) const;
	shared_ptr<class CCurvePlot>		Get_CurvePlot(_float fRatio) const;
	const _float4						Get_InitialValue() const				{ return m_vecValues.front(); }

	void								Set_Duration(_uint iIndex, _float fDuration);
	void								Set_Value(_uint iIndex, _float4 vValue);
	void								Set_CurvePlot(_uint iIndex, shared_ptr<class CCurvePlot>);
	void								Set_InitialValue(_float4 vValue)		{ m_vecValues.front() = vValue; }

	_uint								Find_Index(_float fRatio) const;

private:
	vector<_float>						m_vecTimeTags;
	vector<_float4>						m_vecValues;
	list<shared_ptr<class CCurvePlot>>	m_lstCurvePlots;

	_float								m_fTotal	= 0.f;

public:
	static shared_ptr<CCurveContainer>	Create();
	shared_ptr<CCurveContainer>			Clone();
	static shared_ptr<CCurveContainer>	Read(std::ifstream&);
#if ACTIVATE_TOOL
	void								Export(std::ofstream&);
#endif
};

END
