#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ICurves abstract
{
protected:
	explicit ICurves() DEFAULT;
	explicit ICurves(const ICurves&) DEFAULT;
	virtual ~ICurves() DEFAULT;

public:
	virtual void				Ready(_vectorf vInitial, _vectorf vFinal)				PURE;

private:
	virtual void				Ready(std::ifstream&)									PURE;

public:
	virtual inline void			Add_Front(_vectorf)										PURE;
	virtual inline void			Add_Back(_vectorf)										PURE;
	virtual inline void			Add_Point(_vectorf, _uint iIndex = 0)					PURE;
	virtual inline void			Remove_Point(_uint iIndex)								PURE;

	virtual const _float4		Get_InitialPoint() const								PURE;
	virtual const _float4		Get_FinalPoint() const									PURE;

	virtual const _uint			Get_Size(_bool = true) const							PURE;

	virtual const _float4		Get_Value(_float fRatio, any param = g_aNull) const		PURE;

	virtual const Curves::TYPE	Get_Type() const										PURE;

public:
	static shared_ptr<ICurves>	Read(std::ifstream&);
	virtual shared_ptr<ICurves>	Clone()													PURE;
#if ACTIVATE_TOOL
	virtual void				Export(std::ofstream&)									PURE;
#endif
};

END
