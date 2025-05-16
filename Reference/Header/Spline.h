#pragma once
#include "Engine_Define.h"
#include "Curves.h"

BEGIN(Engine)

class ENGINE_DLL CSpline final : public ICurves
{
private:
	explicit CSpline() DEFAULT;
	explicit CSpline(const CSpline&);
	virtual ~CSpline() DEFAULT;

public:
	virtual void				Ready(_vectorf, _vectorf) override;

private:
	virtual void				Ready(std::ifstream&) override;

public:
	virtual inline void			Add_Front(_vectorf) override;
	virtual inline void			Add_Back(_vectorf) override;
	virtual inline void			Add_Point(_vectorf, _uint iIndex = 0) override;
	virtual inline void			Remove_Point(_uint iIndex) override;

	virtual const _float4		Get_InitialPoint() const override						{ return m_lstPoints.front(); }
	virtual const _float4		Get_FinalPoint() const override							{ return m_lstPoints.back(); }

	virtual const _uint			Get_Size(_bool bSegment = true) const override			{ return static_cast<_uint>(bSegment ? m_lstPoints.size() - 2 : m_lstPoints.size()); }

	virtual const _float4		Get_Value(_float fRatio, any iIndex) const override;

	virtual const Curves::TYPE	Get_Type() const override								{ return Curves::TYPE::SPLINE; }

private:
	list<_float4>				m_lstPoints;

public:
	static shared_ptr<CSpline>	Create(_vectorf = XMVectorReplicate(-1.f), _vectorf = XMVectorZero(), _vectorf = XMVectorReplicate(1.f), _vectorg = XMVectorReplicate(2.f));
	virtual shared_ptr<ICurves>	Clone() override;

	static shared_ptr<CSpline>	Read(std::ifstream&);
#if ACTIVATE_TOOL
	virtual void				Export(std::ofstream&) override;
#endif
};

END
