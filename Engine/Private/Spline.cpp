#include "EnginePCH.h"
#include "Spline.h"

CSpline::CSpline(const CSpline& _rhs)
	: ICurves(_rhs)
	, m_lstPoints(_rhs.m_lstPoints)
//	, m_eSpline(_rhs.m_eSpline)
{
}

void CSpline::Ready(_vectorf _vA, _vectorf _vB)
{
	m_lstPoints.emplace_back(_vA);
	m_lstPoints.emplace_back(_vB);
}

void CSpline::Ready(std::ifstream& _inFile)
{
	size_t nSize(0);
	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		_float4 vPoint;
		_inFile.read(reinterpret_cast<_byte*>(&vPoint), sizeof(_float4));
		m_lstPoints.emplace_back(vPoint);
	}
}

inline void CSpline::Add_Front(_vectorf _vPoint)
{
	m_lstPoints.emplace_front(_vPoint);
}

inline void CSpline::Add_Back(_vectorf _vPoint)
{
	m_lstPoints.emplace_back(_vPoint);
}

void CSpline::Add_Point(_vectorf _vPoint, _uint _iIndex)
{
	m_lstPoints.emplace(std::next(m_lstPoints.begin(), _iIndex), _vPoint);
}

inline void CSpline::Remove_Point(_uint _iIndex)
{
	if (_iIndex >= m_lstPoints.size())
	{
		MSG_RETURN(, "CSpline::Remove_Point", "Invalid Index");
	}

	m_lstPoints.erase(std::next(m_lstPoints.begin(), _iIndex));
}

const _float4 CSpline::Get_Value(_float _fRatio, any _iIndex) const
{
	_uint iIndex = any_cast<_uint>(_iIndex);

	if (m_lstPoints.size() < 4)
	{
		MSG_RETURN(_float4(), "CSpline::Get_Value", "Spline Not Ready");
	}
	if (iIndex > m_lstPoints.size() - 4)
	{
		MSG_RETURN(_float4(), "CSpline::Get_Value", "Invalid Index");
	}

	return XMVectorCatmullRom(
		*(std::next(m_lstPoints.begin(), iIndex + 0)),
		*(std::next(m_lstPoints.begin(), iIndex + 1)),
		*(std::next(m_lstPoints.begin(), iIndex + 2)),
		*(std::next(m_lstPoints.begin(), iIndex + 3)),
		_fRatio);
}

shared_ptr<CSpline> CSpline::Create(_vectorf _vA, _vectorf _vB, _vectorf _vC, _vectorg _vD)
{
	shared_ptr<CSpline> pInstance = make_private_shared(CSpline);
	pInstance->Ready(_vA, _vB);
	pInstance->Ready(_vC, _vD);
	return pInstance;
}

shared_ptr<ICurves> CSpline::Clone()
{
	return make_private_shared_copy(CSpline, *this);
}

shared_ptr<CSpline> CSpline::Read(std::ifstream& _inFile)
{
	shared_ptr<CSpline> pInstance = make_private_shared(CSpline);
	pInstance->Ready(_inFile);
	return pInstance;
}

#if ACTIVATE_TOOL
void CSpline::Export(std::ofstream& _outFile)
{
	const Curves::TYPE eType = Curves::TYPE::SPLINE;
	_outFile.write(reinterpret_cast<const _byte*>(&eType), sizeof(Curves::TYPE));

	size_t nSize = m_lstPoints.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		_float4 vPoint = *std::next(m_lstPoints.begin(), i);
		_outFile.write(reinterpret_cast<const _byte*>(&vPoint), sizeof(_float4));
	}
}
#endif
