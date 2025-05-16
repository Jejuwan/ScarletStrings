#include "ClientPCH.h"
#include "CurvePlot.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

CCurvePlot::CCurvePlot(const CCurvePlot& _rhs)
	: m_mapControlPoints(_rhs.m_mapControlPoints)
	, m_vecControlPointX(_rhs.m_vecControlPointX)
	, m_vecControlPointY(_rhs.m_vecControlPointY)
	, m_vecPointX(_rhs.m_vecPointX)
	, m_vecPointY(_rhs.m_vecPointY)
	, m_fPower(_rhs.m_fPower)
#if ACTIVATE_TOOL
	, m_vPlotSize(_rhs.m_vPlotSize)
#endif
	, m_eType(_rhs.m_eType)
{
	m_pCurves = _rhs.m_pCurves->Clone();
}

void CCurvePlot::Ready(const string& _strPlot)
{
	m_strPlot = _strPlot;

	m_vecPointX.resize(static_cast<size_t>(PLOT_PRECISION + 1.f));
	m_vecPointY.resize(static_cast<size_t>(PLOT_PRECISION + 1.f));
	for (size_t i = 0; i <= static_cast<size_t>(PLOT_PRECISION); ++i)
	{
		m_vecPointX[i]		= static_cast<_float>(i) / static_cast<_float>(PLOT_PRECISION);
		m_vecPointY[i]		= static_cast<_float>(i) / static_cast<_float>(PLOT_PRECISION);
	}

	Set_Curve(CEase::Create());
}

void CCurvePlot::Tick()
{
#if ACTIVATE_TOOL
	Options();
#endif
	Update_Curves();

#if ACTIVATE_TOOL
  	if (ImPlot::BeginPlot(m_strPlot.c_str(), m_vPlotSize))
	{
		if (!m_mapControlPoints.empty())
		{
			ImPlot::PlotScatter(string(string("Control##Scatter##") + m_strPlot).c_str(), m_vecControlPointX.data(), m_vecControlPointY.data(), static_cast<_int>(m_mapControlPoints.size()));
			ImPlot::PlotLine(string(string("Control##Line##") + m_strPlot).c_str(), m_vecControlPointX.data(), m_vecControlPointY.data(), static_cast<_int>(m_mapControlPoints.size()));
		}

		ImPlot::PlotLine(string(string("Curve##") + m_strPlot).c_str(), m_vecPointX.data(), m_vecPointY.data(), static_cast<_int>(PLOT_PRECISION + 1.f));
		
		Interaction();

		ImPlot::EndPlot();
	}

	if (ImGui::TreeNode("Control Point List##CCurvePlot::Tick"))
	{
		for (size_t i = 0; i < m_mapControlPoints.size(); ++i)
		{
			if (ImGui::Button(string("-##" + std::to_string(i)).c_str()))
			{
				Remove_Point(static_cast<_uint>(i));
				Refresh_ControlPoints();
				continue;
			}
			ImGui::SameLine();
			_float2 vControlPoint = _float2((*std::next(m_mapControlPoints.begin(), i)).first, (*std::next(m_mapControlPoints.begin(), i)).second);
			if (ImGui::InputFloat2(string("Control" + std::to_string(i)).c_str(), reinterpret_cast<_float*>(&vControlPoint), "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Remove_Point(static_cast<_uint>(i));
				Refresh_ControlPoints();
				Add_Point(vControlPoint);
				Refresh_ControlPoints();
			}
		}

		ImGui::TreePop();
	}
#endif
}

const _float4 CCurvePlot::Get_Value(_float _fRatio) const
{
	_float	fRatio	= _fRatio;
	any		param	= g_aNull;

	switch (m_eType)
	{
	case Curves::TYPE::SPLINE:
	{
		_uint iIndex = Find_Index(_fRatio) - 1u;
		if (iIndex == m_mapControlPoints.size() - 3)
		{
			--iIndex;
		}
		fRatio = Function::ProportionalRatio(m_vecControlPointX[iIndex + 1], m_vecControlPointX[iIndex + 2], fRatio);
		param = iIndex;
	}
	break;
	case Curves::TYPE::EASE:
		param = m_fPower;
		break;
	}

	return m_pCurves->Get_Value(fRatio, param);
}

void CCurvePlot::Set_Curve(shared_ptr<ICurves> _pCurves)
{
	m_mapControlPoints.clear();

	m_pCurves	= _pCurves;
	m_eType		= m_pCurves->Get_Type();

	switch (m_eType)
	{
	case Curves::TYPE::CURVE:
	case Curves::TYPE::EASE:
		m_mapControlPoints.emplace(0.f, 0.f);
		m_mapControlPoints.emplace(1.f, 1.f);
		break;
	case Curves::TYPE::SPLINE:
		m_mapControlPoints.emplace(-1.f, -1.f);
		m_mapControlPoints.emplace(0.f, 0.f);
		m_mapControlPoints.emplace(1.f, 1.f);
		m_mapControlPoints.emplace(2.f, 2.f);
		break;
	}

	Refresh_ControlPoints();
}

void CCurvePlot::Add_ControlPoint(_vectorf _vControlPoint)
{
	m_mapControlPoints.emplace(XMVectorGetX(_vControlPoint), XMVectorGetY(_vControlPoint));
	Refresh_ControlPoints(_vControlPoint);
}

void CCurvePlot::Remove_ControlPoint(_uint _iIndex)
{
	m_mapControlPoints.erase(std::next(m_mapControlPoints.begin(), _iIndex));
	Refresh_ControlPoints();
}

void CCurvePlot::Add_Point(_vectorf _vPoint)
{
	_uint iIndex = Find_Index(XMVectorGetX(_vPoint));
	if (static_cast<_uint>(m_mapControlPoints.size()) == iIndex)
	{
		m_pCurves->Add_Front(_vPoint);
	}
	else if (static_cast<_uint>(m_mapControlPoints.size()) + 1u == iIndex)
	{
		m_pCurves->Add_Back(_vPoint);
	}
	else
	{
		m_pCurves->Add_Point(_vPoint, iIndex + 1u);
	}
	Add_ControlPoint(_vPoint);
}

void CCurvePlot::Remove_Point(_uint _iIndex)
{
	m_pCurves->Remove_Point(_iIndex);
	Remove_ControlPoint(_iIndex);
}

_uint CCurvePlot::Find_Index(_float _fRatio) const
{
	auto iter = m_mapControlPoints.lower_bound(_fRatio);
	
	if (iter == m_mapControlPoints.end())
	{
		return static_cast<_uint>(m_mapControlPoints.size()) + 1u;
	}
	else if (iter != m_mapControlPoints.begin() && iter->first != _fRatio)
	{
		--iter;
	}
	else if (iter == m_mapControlPoints.begin())
	{
		return static_cast<_uint>(m_mapControlPoints.size());
	}

	return Get_Index(iter->first);
}

_uint CCurvePlot::Get_Index(_float _fRatio) const
{
	_uint iIndex(0);
	for (auto iter = m_mapControlPoints.begin(); iter != m_mapControlPoints.end(); ++iter, ++iIndex)
	{
		if (iter->first == _fRatio)
		{
			return iIndex;
		}
	}

	return static_cast<_uint>(m_mapControlPoints.size());
}

void CCurvePlot::Update_Curves()
{
	any param = g_aNull;

	for (size_t i = 0; i <= static_cast<size_t>(PLOT_PRECISION); ++i)
	{
		m_vecPointY[i] = Get_Value(static_cast<_float>(i) / PLOT_PRECISION).y;
	}
}

#if ACTIVATE_TOOL
void CCurvePlot::Options()
{
	static const _char* szType = "Ease";

	if (ImGui::BeginCombo("Type##Curves::TYPE##CCurvePlot::Options", szType))
	{
		const _char* items[] = { "Curve", "Spline", "Ease" };

		if (ImGui::Selectable(items[0], szType == items[0]))
		{
			szType = items[0];
			Set_Curve(CCurve::Create());
		}
		if (ImGui::Selectable(items[1], szType == items[1]))
		{
			szType = items[1];
			Set_Curve(CSpline::Create());
		}
		if (ImGui::Selectable(items[2], szType == items[2]))
		{
			szType = items[2];
			Set_Curve(CEase::Create());
		}

		ImGui::EndCombo();
	}

	switch (m_eType)
	{
	case Curves::TYPE::CURVE:
	{
		static Curves::CURVE eCurveType(Curves::CURVE::BEZIER);

		static const _char* szCurveType = "Bezier";

		if (ImGui::BeginCombo("Type##Curves::CURVE##CCurvePlot::Options", szCurveType))
		{
			const _char* items[] = { "Bezier" };

			if (ImGui::Selectable(items[0], szType == items[0]))
			{
				szCurveType = items[0];
				eCurveType = Curves::CURVE::BEZIER;
			}

			ImGui::EndCombo();
		}
	}
	break;
	case Curves::TYPE::SPLINE:
	{
		static Curves::SPLINE eSplineType(Curves::SPLINE::CATMULL_ROM);

		static const _char* szSplineType = "CatmullRom";

		if (ImGui::BeginCombo("Type##Curves::SPLINE##CCurvePlot::Options", szSplineType))
		{
			const _char* items[] = { "CatmullRom" };

			if (ImGui::Selectable(items[0], szSplineType == items[0]))
			{
				szSplineType = items[0];
				eSplineType = Curves::SPLINE::CATMULL_ROM;
			}

			ImGui::EndCombo();
		}
	}
	break;
	case Curves::TYPE::EASE:
	{
		static Curves::EASE_TYPE	eEaseType(Curves::EASE_TYPE::TYPE_IN);
		static Curves::EASE			eEaseFunction(Curves::EASE::LINEAR);

		static const _char* szEaseType		= "In";
		static const _char* szEaseFunction	= "Linear";

		if (ImGui::BeginCombo("Type##Curves::EASE##CCurvePlot::Options", szEaseType))
		{
			const _char* items[] = { "In", "Out", "InOut" };

			if (ImGui::Selectable(items[0], szEaseType == items[0]))
			{
				szEaseType = items[0];
				eEaseType = Curves::EASE_TYPE::TYPE_IN;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Type(eEaseType);
			}
			if (ImGui::Selectable(items[1], szEaseType == items[1]))
			{
				szEaseType = items[1];
				eEaseType = Curves::EASE_TYPE::TYPE_OUT;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Type(eEaseType);
			}
			if (ImGui::Selectable(items[2], szEaseType == items[2]))
			{
				szEaseType = items[2];
				eEaseType = Curves::EASE_TYPE::TYPE_INOUT;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Type(eEaseType);
			}

			ImGui::EndCombo();
		}
		if (ImGui::BeginCombo("Function##Curves::EASE##CCurvePlot::Options", szEaseFunction))
		{
			const _char* items[] = { "Linear", "Sine", "Polynomial", "Sinusodial", "Exponential", "Circular", "Elastic", "Back", "Bounce", "Overshoot" };

			if (ImGui::Selectable(items[0], szEaseFunction == items[0]))
			{
				szEaseFunction = items[0];
				eEaseFunction = Curves::EASE::LINEAR;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[1], szEaseFunction == items[1]))
			{
				szEaseFunction = items[1];
				eEaseFunction = Curves::EASE::SINE;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[2], szEaseFunction == items[2]))
			{
				szEaseFunction = items[2];
				eEaseFunction = Curves::EASE::POLYNOMIAL;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[3], szEaseFunction == items[3]))
			{
				szEaseFunction = items[3];
				eEaseFunction = Curves::EASE::SINUSOIDAL;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[4], szEaseFunction == items[4]))
			{
				szEaseFunction = items[4];
				eEaseFunction = Curves::EASE::EXPONENTIAL;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[5], szEaseFunction == items[5]))
			{
				szEaseFunction = items[5];
				eEaseFunction = Curves::EASE::CIRCULAR;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[6], szEaseFunction == items[6]))
			{
				szEaseFunction = items[6];
				eEaseFunction = Curves::EASE::ELASTIC;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[7], szEaseFunction == items[7]))
			{
				szEaseFunction = items[7];
				eEaseFunction = Curves::EASE::BACK;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[8], szEaseFunction == items[8]))
			{
				szEaseFunction = items[8];
				eEaseFunction = Curves::EASE::BOUNCE;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}
			if (ImGui::Selectable(items[9], szEaseFunction == items[9]))
			{
				szEaseFunction = items[9];
				eEaseFunction = Curves::EASE::OVERSHOOT;
				dynamic_pointer_cast<CEase>(m_pCurves)->Set_Function(eEaseFunction);
			}

			ImGui::EndCombo();
		}
		if (ImGui::InputFloat("Power##Curves::EASE##CCurvePlot::Options", &m_fPower))
		{
		}
	}
	break;
	}
}

void CCurvePlot::Interaction()
{
	if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl)
	{
		Add_Point(_float2(static_cast<_float>(ImPlot::GetPlotMousePos().x), static_cast<_float>(ImPlot::GetPlotMousePos().y)));
	}
}
#endif

void CCurvePlot::Refresh_ControlPoints(_vectorf _vInsert)
{
	m_vecControlPointX.clear();
	m_vecControlPointY.clear();
	
	m_vecControlPointX.reserve(m_mapControlPoints.size());
	for (const auto& pair : m_mapControlPoints)
	{
		m_vecControlPointX.emplace_back(pair.first);
	}
	m_vecControlPointY.resize(m_mapControlPoints.size());
	for (size_t i = 0; i < m_mapControlPoints.size(); ++i)
	{
		m_vecControlPointY[i] = m_mapControlPoints[m_vecControlPointX[i]];
	}
}

shared_ptr<CCurvePlot> CCurvePlot::Create(const string& _strPlot)
{
	shared_ptr<CCurvePlot> pInstance = make_private_shared(CCurvePlot);
	pInstance->Ready(_strPlot);
	return pInstance;
}

shared_ptr<CCurvePlot> CCurvePlot::Clone(const string& _strPlot)
{
	shared_ptr<CCurvePlot> pInstance = make_private_shared_copy(CCurvePlot, *this);
	pInstance->m_strPlot = _strPlot.empty() ? m_strPlot + string(" Clone") : _strPlot;
	return pInstance;
}

shared_ptr<CCurvePlot> CCurvePlot::Read(std::ifstream& _inFile)
{
	shared_ptr<CCurvePlot> pInstance = make_private_shared(CCurvePlot);

	size_t nSize(0);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		pair<_float, _float> pairControlPoint;
		_inFile.read(reinterpret_cast<_byte*>(&pairControlPoint), sizeof(pair<const _float, _float>));
		pInstance->m_mapControlPoints.emplace(pairControlPoint);
	}

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecControlPointX.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecControlPointX.data()), sizeof(_float) * nSize);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecControlPointY.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecControlPointY.data()), sizeof(_float) * nSize);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecPointX.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecPointX.data()), sizeof(_float) * nSize);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecPointY.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecPointY.data()), sizeof(_float) * nSize);

	pInstance->m_pCurves = ICurves::Read(_inFile);

	_inFile.read(reinterpret_cast<_byte*>(&pInstance->m_fPower), sizeof(_float));

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_strPlot.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_strPlot.data()), sizeof(_char) * nSize);

#if ACTIVATE_TOOL
	_inFile.read(reinterpret_cast<_byte*>(&pInstance->m_vPlotSize), sizeof(ImVec2));
#else
	_inFile.seekg(sizeof(_float2), std::ios::cur);
#endif
	_inFile.read(reinterpret_cast<_byte*>(&pInstance->m_eType), sizeof(Curves::TYPE));

	return pInstance;
}

#if ACTIVATE_TOOL
void CCurvePlot::Export(std::ofstream& _outFile)
{
	size_t nSize(0);

	nSize = m_mapControlPoints.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	for (auto& iter : m_mapControlPoints)
	{
		_outFile.write(reinterpret_cast<const _byte*>(&iter), sizeof(pair<const _float, _float>));
	}

	nSize = m_vecControlPointX.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecControlPointX.data()), sizeof(_float) * nSize);

	nSize = m_vecControlPointY.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecControlPointY.data()), sizeof(_float) * nSize);

	nSize = m_vecPointX.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecPointX.data()), sizeof(_float) * nSize);

	nSize = m_vecPointY.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecPointY.data()), sizeof(_float) * nSize);

	m_pCurves->Export(_outFile);

	_outFile.write(reinterpret_cast<const _byte*>(&m_fPower), sizeof(_float));

	nSize = m_strPlot.length();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_strPlot.c_str()), sizeof(_char) * nSize);

	_outFile.write(reinterpret_cast<const _byte*>(&m_vPlotSize), sizeof(ImVec2));
	_outFile.write(reinterpret_cast<const _byte*>(&m_eType), sizeof(Curves::TYPE));
}
#endif
