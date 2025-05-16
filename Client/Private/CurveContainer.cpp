#include  "ClientPCH.h"
#include "CurveContainer.h"
#include "GameInstance.h"
#include "CurvePlot.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

CCurveContainer::CCurveContainer(const CCurveContainer& _rhs)
	: m_fTotal(_rhs.m_fTotal)
	, m_vecTimeTags(_rhs.m_vecTimeTags)
	, m_vecValues(_rhs.m_vecValues)
{
	for (auto& iter : _rhs.m_lstCurvePlots)
	{
		m_lstCurvePlots.emplace_back(iter->Clone());
	}
}

void CCurveContainer::Initialize()
{
	m_vecTimeTags.emplace_back(0.f);
	m_vecValues.emplace_back(XMVectorZero());

	Add_Curve(CCurvePlot::Create(), 0, 1.f, XMVectorReplicate(1.f));
}

void CCurveContainer::Add_Curve(shared_ptr<CCurvePlot> _pCurvePlot, _uint _iIndex, _float _fDuration, _float4 _vValue)
{
	m_fTotal += _fDuration;

	m_vecTimeTags.emplace(std::next(m_vecTimeTags.begin(), _iIndex + 1u), _fDuration);
	m_vecValues.emplace(std::next(m_vecValues.begin(), _iIndex + 1u), _vValue);
	m_lstCurvePlots.emplace(std::next(m_lstCurvePlots.begin(), _iIndex), _pCurvePlot);
}

void CCurveContainer::Remove_Curve(_uint _iIndex)
{
	m_fTotal -= m_vecTimeTags[_iIndex];
	
	m_vecTimeTags.erase(std::next(m_vecTimeTags.begin()));
	m_vecValues.erase(std::next(m_vecValues.begin()));
	m_lstCurvePlots.erase(std::next(m_lstCurvePlots.begin()));
}

const _float CCurveContainer::Get_CurveValue(_float _fRatio) const
{
	_uint	iIndex	= Find_Index(_fRatio);
	_float	fAcc	= std::accumulate(m_vecTimeTags.begin(), std::next(m_vecTimeTags.begin(), iIndex + 1), 0.f);
 	_float	fRatio	= (_fRatio - fAcc) / *std::next(m_vecTimeTags.begin(), iIndex + 1);
	_float	fValue	= (*std::next(m_lstCurvePlots.begin(), iIndex))->Get_Value(fRatio).y;

	return fValue;
}

const _float4 CCurveContainer::Get_Value(_float _fRatio) const
{
	_uint	iIndex	= Find_Index(_fRatio);
	if (m_vecTimeTags.size() - 1 == iIndex)
	{
		return m_vecValues[iIndex];
	}
	_float	fAcc	= std::accumulate(m_vecTimeTags.begin(), std::next(m_vecTimeTags.begin(), iIndex + 1), 0.f);
 	_float	fRatio	= (_fRatio - fAcc) / *std::next(m_vecTimeTags.begin(), iIndex + 1);
	_float	fValue	= (*std::next(m_lstCurvePlots.begin(), iIndex))->Get_Value(fRatio).y;
	_float4	vReturn	= m_vecValues[iIndex] * (1.f - fValue) + m_vecValues[iIndex + 1u] * fValue;

	return vReturn;
}

shared_ptr<CCurvePlot> CCurveContainer::Get_CurvePlot(_uint _iIndex) const
{
	return *std::next(m_lstCurvePlots.begin(), _iIndex);
}

shared_ptr<CCurvePlot> CCurveContainer::Get_CurvePlot(_float _fRatio) const
{
	return *std::next(m_lstCurvePlots.begin(), Find_Index(_fRatio));
}

void CCurveContainer::Set_Duration(_uint _iIndex, _float _fDuration)
{
	*std::next(m_vecTimeTags.begin(), _iIndex) = _fDuration;
	m_fTotal = std::accumulate(m_vecTimeTags.begin(), m_vecTimeTags.end(), 0.f);
}

void CCurveContainer::Set_Value(_uint _iIndex, _float4 _vValue)
{
	*std::next(m_vecValues.begin(), _iIndex) = _vValue;
}

void CCurveContainer::Set_CurvePlot(_uint _iIndex, shared_ptr<CCurvePlot> _pCurvePlot)
{
	*std::next(m_lstCurvePlots.begin(), _iIndex) = _pCurvePlot;
}

_uint CCurveContainer::Find_Index(_float _fRatio) const
{
//	auto iter = std::lower_bound(m_vecTimeTags.begin(), m_vecTimeTags.end(), _fRatio);

	_float fSum(0.f);
	for (size_t i = 0; i < m_vecTimeTags.size(); ++i)
	{
		if (i == m_vecTimeTags.size() - 1)
		{
			return static_cast<_uint>(i);
		}
		fSum += m_vecTimeTags[i + 1];
		if (fSum >= _fRatio)
		{
			return static_cast<_uint>(i);
		}
	}

	return static_cast<_uint>(m_vecTimeTags.size());
}

shared_ptr<CCurveContainer> CCurveContainer::Create()
{
	shared_ptr<CCurveContainer> pInstance = make_private_shared(CCurveContainer);
	pInstance->Initialize();
	return pInstance;
}

shared_ptr<CCurveContainer> CCurveContainer::Clone()
{
	return make_private_shared_copy(CCurveContainer, *this);
}

shared_ptr<CCurveContainer> CCurveContainer::Read(std::ifstream& _inFile)
{
	shared_ptr<CCurveContainer> pInstance = make_private_shared(CCurveContainer);

	size_t nSize(0);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecTimeTags.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecTimeTags.data()), sizeof(_float) * nSize);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	pInstance->m_vecValues.resize(nSize);
	_inFile.read(reinterpret_cast<_byte*>(pInstance->m_vecValues.data()), sizeof(_float4) * nSize);

	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		pInstance->m_lstCurvePlots.emplace_back(CCurvePlot::Read(_inFile));
	}

	_inFile.read(reinterpret_cast<_byte*>(&pInstance->m_fTotal), sizeof(_float));

	return pInstance;
}

#if ACTIVATE_TOOL
void CCurveContainer::Export(std::ofstream& _outFile)
{
	size_t nSize(0);

	nSize = m_vecTimeTags.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecTimeTags.data()), sizeof(_float) * nSize);

	nSize = m_vecValues.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecValues.data()), sizeof(_float4) * nSize);

	nSize = m_lstCurvePlots.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	for (auto& iter : m_lstCurvePlots)
	{
		iter->Export(_outFile);
	}

	_outFile.write(reinterpret_cast<const _byte*>(&m_fTotal), sizeof(_float));
}
#endif
