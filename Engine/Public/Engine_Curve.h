#pragma once
#include "Engine_Define.h"

namespace Curves
{
	enum class TYPE
	{
		CURVE,
		SPLINE,
		EASE,
		MAX
	};

	enum class CURVE	// APPROXIMATE
	{
		BEZIER,			
		MAX
	};

	enum class SPLINE	// INTERPOLATE
	{
	//	BEZIOR,
		CATMULL_ROM,
		MAX
	};

	enum class EASE
	{
		LINEAR,
		SINE,
	//	QUADRATIC,
	//	CUBIC,
	//	QUARTIC,
	//	QUINTIC,
		POLYNOMIAL,
		SINUSOIDAL,
		EXPONENTIAL,
		CIRCULAR,
		ELASTIC,
		BACK,
		BOUNCE,
		OVERSHOOT,
		MAX
	};

	enum class EASE_TYPE	{ TYPE_IN, TYPE_OUT, TYPE_INOUT, MAX };

	_inline _float	Ease_In(EASE, _float fRatio, _float fPower = 0.f);
	_inline _float	Ease_Out(EASE, _float fRatio, _float fPower = 0.f);
	_inline _float	Ease_InOut(EASE, _float fRatio, _float fPower = 0.f);

	template<typename Container>
	inline XMVECTOR	Bezier_Curve(Container _points, _float _fRatio)
	{
		XMVECTOR	vResult = XMVectorZero();
		size_t		nSize(_points.size());

		for (size_t i = 0; i < nSize; ++i)
		{
			vResult += Function::Bernstein(static_cast<_uint>(nSize - 1), static_cast<_uint>(i), _fRatio) * *std::next(_points.begin(), i);
		}

		return vResult;
	}
}

#include "Engine_Curve.inl"
