#include "EnginePCH.h"
#include "Curve.h"

CCurve::CCurve(const CCurve& _rhs)
    : ICurves(_rhs)
    , m_lstPoints(_rhs.m_lstPoints)
{
}

void CCurve::Ready(_vectorf _vInitial, _vectorf _vFinal)
{
    m_lstPoints.emplace_front(_vInitial);
    m_lstPoints.emplace_back(_vFinal);
}

void CCurve::Ready(std::ifstream& _inFile)
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

inline void CCurve::Add_Front(_vectorf _vPoint)
{
    m_lstPoints.emplace_front(_vPoint);
}

inline void CCurve::Add_Back(_vectorf _vPoint)
{
	m_lstPoints.emplace_back(_vPoint);
}

inline void CCurve::Add_Point(_vectorf _vPoint, _uint _iIndex)
{
    m_lstPoints.emplace(std::next(m_lstPoints.begin(), _iIndex), _vPoint);
}

inline void CCurve::Remove_Point(_uint _iIndex)
{
    if (_iIndex >= m_lstPoints.size())
    {
        MSG_RETURN(, "CCurve::Remove_Point", "Invalid Index");
    }

    m_lstPoints.erase(std::next(m_lstPoints.begin(), _iIndex));
}

const _float4 CCurve::Get_Value(_float _fRatio, any) const
{
    if (m_lstPoints.size() < 2)
    {
       MSG_RETURN(_float4(), "CCurve::Get_Value", "CCurve Not Ready");
    }

    return Curves::Bezier_Curve(m_lstPoints, _fRatio);
}

shared_ptr<CCurve> CCurve::Create(_vectorf _vInitial, _vectorf _vFinal)
{
    shared_ptr<CCurve> pInstance = make_private_shared(CCurve);
    pInstance->Ready(_vInitial, _vFinal);
    return pInstance;
}

shared_ptr<ICurves> CCurve::Clone()
{
    return make_private_shared_copy(CCurve, *this);
}

shared_ptr<CCurve> CCurve::Read(std::ifstream& _inFile)
{
    shared_ptr<CCurve> pInstance = make_private_shared(CCurve);
    pInstance->Ready(_inFile);
    return pInstance;
}

#if ACTIVATE_TOOL
void CCurve::Export(std::ofstream& _outFile)
{
    const Curves::TYPE eType = Curves::TYPE::CURVE;
    _outFile.write(reinterpret_cast<const _byte*>(&eType), sizeof(Curves::TYPE));

    size_t nSize = m_lstPoints.size();
    _outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
    for (size_t i = 0; i < nSize; ++i)
    {
        _outFile.write(reinterpret_cast<const _byte*>(&m_lstPoints.front()), sizeof(_float4));
        m_lstPoints.pop_front();
    }
}
#endif
