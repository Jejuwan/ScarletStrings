#include "EnginePCH.h"
#include "Ease.h"

CEase::CEase(const CEase& _rhs)
	: ICurves(_rhs)
	, m_eType(_rhs.m_eType)
	, m_eEase(_rhs.m_eEase)
	, m_lstPoints(_rhs.m_lstPoints)
{
}

void CEase::Ready(_vectorf _vInitial, _vectorf _vFinal)
{
	m_lstPoints.emplace_front(_vInitial);
	m_lstPoints.emplace_back(_vFinal);
}

void CEase::Ready(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_eType), sizeof(Curves::EASE_TYPE));
 	_inFile.read(reinterpret_cast<_byte*>(&m_eEase), sizeof(Curves::EASE));

	size_t nSize(0);
	_inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		_float4 vPoint;
		_inFile.read(reinterpret_cast<_byte*>(&vPoint), sizeof(_float4));
		m_lstPoints.emplace_back(vPoint);
	}
}

void CEase::Add_Point(_vectorf _vPoint, _uint _iIndex)
{
	if (0u == _iIndex)
	{
		m_lstPoints.front() = _vPoint;
	}
	else if (1u == _iIndex)
	{
		m_lstPoints.back() = _vPoint;
	}
}

const _float4 CEase::Get_Value(_float _fRatio, any _fPower) const
{
	_float fPower(_fPower.has_value() ? any_cast<_float>(_fPower) : 0.f);

	switch (m_eType)
	{
	case Curves::EASE_TYPE::TYPE_IN:
		return XMVectorReplicate(Curves::Ease_In(m_eEase, _fRatio, fPower));
	case Curves::EASE_TYPE::TYPE_OUT:
		return XMVectorReplicate(Curves::Ease_Out(m_eEase, _fRatio, fPower));
	case Curves::EASE_TYPE::TYPE_INOUT:
		return XMVectorReplicate(Curves::Ease_InOut(m_eEase, _fRatio, fPower));
	}

	MSG_RETURN(_float4(), "CEase::Get_Value", "Invalid Ease Type");
}

shared_ptr<CEase> CEase::Create(_vectorf _vInitial, _vectorf _vFinal)
{
	shared_ptr<CEase> pInstance = make_private_shared(CEase);
	pInstance->Ready(_vInitial, _vFinal);
	return pInstance;
}

shared_ptr<ICurves> CEase::Clone()
{
	return make_private_shared_copy(CEase, *this);
}

shared_ptr<CEase> CEase::Read(std::ifstream& _inFile)
{
	shared_ptr<CEase> pInstance = make_private_shared(CEase);
	pInstance->Ready(_inFile);
	return pInstance;
}

#if ACTIVATE_TOOL
void CEase::Export(std::ofstream& _outFile)
{
	const Curves::TYPE eType = Curves::TYPE::EASE;
	_outFile.write(reinterpret_cast<const _byte*>(&eType), sizeof(Curves::TYPE));

	_outFile.write(reinterpret_cast<const _byte*>(&m_eType), sizeof(Curves::EASE_TYPE));
	_outFile.write(reinterpret_cast<const _byte*>(&m_eEase), sizeof(Curves::EASE));

	size_t nSize = m_lstPoints.size();
	_outFile.write(reinterpret_cast<const _byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
		_outFile.write(reinterpret_cast<const _byte*>(&m_lstPoints.front()), sizeof(_float4));
		m_lstPoints.pop_front();
	}
}
#endif
