#pragma once
#include "Engine_Define.h"
#include "Curves.h"

BEGIN(Engine)

class ENGINE_DLL CCurve final : public ICurves
{
private:
	explicit CCurve() DEFAULT;
	explicit CCurve(const CCurve&);
	virtual ~CCurve() DEFAULT;

public:
	virtual void				Ready(_vectorf vInitial = XMVectorZero(), _vectorf vFinal = XMVectorReplicate(1.f)) override;

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

	virtual const _float4		Get_Value(_float fRatio, any = g_aNull) const override;

	virtual const Curves::TYPE	Get_Type() const override								{ return Curves::TYPE::CURVE; }

private:
	list<_float4>				m_lstPoints;

public:
	static shared_ptr<CCurve>	Create(_vectorf = XMVectorZero(), _vectorf = XMVectorReplicate(1.f));
	virtual shared_ptr<ICurves>	Clone() override;

	static shared_ptr<CCurve>	Read(std::ifstream&);
#if ACTIVATE_TOOL
	virtual void				Export(std::ofstream&) override;
#endif
};

END
