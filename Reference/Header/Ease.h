#pragma once
#include "Engine_Define.h"
#include "Curves.h"

BEGIN(Engine)

class ENGINE_DLL CEase final : public ICurves
{
private:
	explicit CEase() DEFAULT;
	explicit CEase(const CEase&);
	virtual ~CEase() DEFAULT;

public:
	virtual void				Ready(_vectorf vInitial = XMVectorZero(), _vectorf vFinal = XMVectorReplicate(1.f)) override;

private:
	virtual void				Ready(std::ifstream&) override;

public:
	virtual inline void			Add_Front(_vectorf) override									{};
	virtual inline void			Add_Back(_vectorf) override										{};
	virtual inline void			Add_Point(_vectorf, _uint iIndex) override;
	virtual inline void			Remove_Point(_uint) override									{}

	virtual const _float4		Get_InitialPoint() const override								{ return m_lstPoints.front(); }
	virtual const _float4		Get_FinalPoint() const override									{ return m_lstPoints.back(); }

	virtual const _uint			Get_Size(_bool = true) const override							{ return 0u; }

	virtual const _float4		Get_Value(_float fRatio, any fPower = g_aNull) const override;

	virtual const Curves::TYPE	Get_Type() const override										{ return Curves::TYPE::EASE; }

	void						Set_Type(Curves::EASE_TYPE eType)								{ m_eType = eType; }
	void						Set_Function(Curves::EASE eEase)								{ m_eEase = eEase; }

private:
	Curves::EASE_TYPE			m_eType		= Curves::EASE_TYPE::TYPE_IN;
	Curves::EASE				m_eEase		= Curves::EASE::LINEAR;

	list<_float4>				m_lstPoints;

public:
	static shared_ptr<CEase>	Create(_vectorf = XMVectorZero(), _vectorf = XMVectorReplicate(1.f));
	virtual shared_ptr<ICurves>	Clone() override;

	static shared_ptr<CEase>	Read(std::ifstream&);
#if ACTIVATE_TOOL
	virtual void				Export(std::ofstream&) override;
#endif
};

END
