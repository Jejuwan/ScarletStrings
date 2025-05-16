#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class ICurves;
END

BEGIN(Client)

class CCurvePlot final
{
private:
	explicit CCurvePlot() DEFAULT;
	explicit CCurvePlot(const CCurvePlot&);
	virtual ~CCurvePlot() DEFAULT;

public:
	void							Ready(const string& strPlot);
	void							Tick();

public:
	const _float4					Get_Value(_float fRatio) const;
	shared_ptr<ICurves>				Get_Curve() const						{ return m_pCurves; }
	const string					Get_Name() const						{ return m_strPlot; }

	void							Set_Curve(shared_ptr<ICurves>);
#if ACTIVATE_TOOL
	void							Set_Size(_float fSize)					{ m_vPlotSize = ImVec2(fSize, fSize); }
#endif
	void							Set_Name(const string& strPlot)			{ m_strPlot = strPlot; }

private:
	void							Add_ControlPoint(_vectorf);
	void							Remove_ControlPoint(_uint iIndex);

	void							Add_Point(_vectorf);
	void							Remove_Point(_uint iIndex);

	_uint							Find_Index(_float fRatio) const;
	_uint							Get_Index(_float fRatio) const;

	void							Update_Curves();
#if ACTIVATE_TOOL
	void							Options();
	void							Interaction();
#endif
	void							Refresh_ControlPoints(_vectorf = XMVectorZero());

private:
	map<_float, _float>				m_mapControlPoints;
	vector<_float>					m_vecControlPointX;
	vector<_float>					m_vecControlPointY;

	vector<_float>					m_vecPointX;
	vector<_float>					m_vecPointY;

	shared_ptr<ICurves>				m_pCurves;
	_float							m_fPower		= 0.f;

	string							m_strPlot;
	Curves::TYPE					m_eType			= Curves::TYPE::EASE;
#if ACTIVATE_TOOL
	ImVec2							m_vPlotSize		= ImVec2(400.f, 400.f);
#endif

public:
	static shared_ptr<CCurvePlot>	Create(const string& strPlot = string());
	shared_ptr<CCurvePlot>			Clone(const string& strPlot = string());

	static shared_ptr<CCurvePlot>	Read(std::ifstream&);
#if ACTIVATE_TOOL
	void							Export(std::ofstream&);
#endif
#pragma region Overload
//	shared_ptr<ICurves>				operator->() const	{ return m_pCurves; }
#pragma endregion
};

END
