#include "Engine_Function.h"
#include "Engine_Curve.h"

namespace Curves
{
#pragma region Ease
	inline _float Ease_In(EASE _eFunction, _float _fRatio, _float _fPower)
	{
		if (0.f == _fRatio)
		{
			return 0.f;
		}
		if (1.f == _fRatio)
		{
			return 1.f;
		}
		switch (_eFunction)
		{
		case EASE::LINEAR:
			return _fRatio;
		case EASE::SINE:
			return 1.f - cosf(XM_PI * _fRatio / (_fPower ? _fPower : 2.f));
		case EASE::POLYNOMIAL:
			return powf(_fRatio, _fPower ? _fPower : 2.f);
		case EASE::SINUSOIDAL:
			return 1.f - powf(cosf(XM_PIDIV2 * _fRatio), 1.f / (_fPower ? _fPower : 2.f));
		case EASE::EXPONENTIAL:
			return powf(2.f, (_fPower ? _fPower : 10.f) * (_fRatio - 1.f));
		case EASE::CIRCULAR:
			return 1.f - sqrtf(1.f - powf(_fRatio, _fPower ? _fPower : 2.f));
		case EASE::ELASTIC:
			return -powf(2.f, (_fPower ? _fPower : 10.f) * (_fRatio - 1.f)) * sinf((_fPower ? _fPower : 10.f) * (_fRatio - 1.075f) * XM_2PI / 3.f);
		case EASE::BACK:
			return 2.70158f * powf(_fRatio, _fPower ? _fPower * 1.5f : 3.f) - 1.70158f * powf(_fRatio, _fPower ? _fPower : 2.f);
		case EASE::BOUNCE:
			return 1.f - Ease_Out(EASE::BOUNCE, 1.f - _fRatio, _fPower);
		case EASE::OVERSHOOT:
			return 1.f - cosf((_fPower ? _fPower : 4.f) * XM_PI * _fRatio) * (1.f - _fRatio);
		}
		MSG_RETURN(0.f, "Curves::Ease_In", "Invalid Ease Function");
	}

	inline _float Ease_Out(EASE _eEaseFunction, _float _fRatio, _float _fPower)
	{
		if (0.f == _fRatio)
		{
			return 0.f;
		}
		if (1.f == _fRatio)
		{
			return 1.f;
		}
		switch (_eEaseFunction)
		{
		case EASE::LINEAR:
			return _fRatio;
		case EASE::SINE:
			return sinf(XM_PI * _fRatio / (_fPower ? _fPower : 2.f));
		case EASE::POLYNOMIAL:
			return 1.f - powf(1.f - _fRatio, _fPower ? _fPower : 2.f);
		case EASE::SINUSOIDAL:
			return powf(sinf(XM_PIDIV2 * _fRatio), 1.f / (_fPower ? _fPower : 2.f));
		case EASE::EXPONENTIAL:
			return 1.f - powf(2.f, _fRatio * -(_fPower ? _fPower : 10.f));
		case EASE::CIRCULAR:
			return sqrtf(1.f - powf(_fRatio - 1.f, _fPower ? _fPower : 2.f));
		case EASE::ELASTIC:
			return 1.f + powf(2.f, _fRatio  * -(_fPower ? _fPower : 10.f)) * sinf((_fRatio - 0.075f) * (_fPower ? _fPower : 10.f) * XM_2PI / 3.f);
		case EASE::BACK:
			return 1.f + 2.70158f * powf(_fRatio - 1.f, _fPower ? _fPower * 1.5f : 3.f) + 1.70158f * powf(_fRatio - 1.f, _fPower ? _fPower : 2.f);
		case EASE::BOUNCE:
			return 1.f - (1.f - _fRatio) * fabsf(sinf(XM_PI / ((_fRatio - 1.f) * _fPower ? _fPower : 2.f)));
		case EASE::OVERSHOOT:
			return 1.f - cosf(XM_PI * _fRatio * (_fPower ? _fPower : 4.f)) * (1.f - _fRatio);
		}
		MSG_RETURN(0.f, "Curves::Ease_Out", "Invalid Ease Function");
	}

	inline _float Ease_InOut(EASE _eEaseFunction, _float _fRatio, _float _fPower)
	{
		if (0.f == _fRatio)
		{
			return 0.f;
		}
		if (1.f == _fRatio)
		{
			return 1.f;
		}
		switch (_eEaseFunction)
		{
		case EASE::LINEAR:
			return _fRatio;
		case EASE::SINE:
			return 0.5f - cosf(XM_PI * _fRatio) * 0.5f;
		case EASE::POLYNOMIAL:
			return _fRatio < 0.5f ? 2.f * powf(_fRatio, _fPower ? _fPower : 2.f) : 1.f - powf(2.f - 2.f * _fRatio, _fPower ? _fPower : 2.f) * 0.5f;
		case EASE::SINUSOIDAL:
			return 0.5f - cosf(XM_PI * _fRatio) * 0.5f;
		case EASE::EXPONENTIAL:
			return _fRatio < 0.5f ? powf(2.f, (_fPower ? _fPower : 10.f) * (_fRatio * 2.f - 1.f)) * 0.5f : (2.f - powf(2.f, (_fPower ? _fPower : 10.f) * (1.f - _fRatio * 2.f))) * 0.5f;
		case EASE::CIRCULAR:
			return _fRatio < 0.5f ? (1.f - sqrtf(1.f - powf(2.f * _fRatio, _fPower ? _fPower : 2.f))) * 0.5f : (1.f + sqrtf(1.f - powf(2.f - 2.f * _fRatio, _fPower ? _fPower : 2.f))) * 0.5f;
		case EASE::ELASTIC:
			return _fRatio < 0.5f ? -(powf(2.f, (_fPower ? _fPower : 10.f) * (_fRatio * 2.f - 1.f)) * sinf((_fPower ? _fPower : 10.f) * (_fRatio * 2.f - 1.125f) * XM_2PI / 4.5f)) * 0.5f : (powf(2.f, (_fPower ? _fPower : 10.f) * (1.f - _fRatio * 2.f)) * sinf((_fPower ? _fPower : 10.f) * (_fRatio * 2.f - 1.125f) * XM_2PI / 4.5f)) * 0.5f + 1.f;
		case EASE::BACK:
			return _fRatio < 0.5f ? (powf(2.f * _fRatio, (_fPower ? _fPower : 2.f)) * (5.40316f * _fRatio - 2.60040f)) * 0.5f : (powf(2.f * _fRatio - 2.f, (_fPower ? _fPower : 2.f)) * ((2.f * _fRatio - 2.f) * 2.70158f) + 2.f) * 0.5f;
		case EASE::BOUNCE:
			return _fRatio < 0.5f ? (1.f - Ease_Out(EASE::BOUNCE, 1.f - 2.f * _fRatio, _fPower)) * 0.5f : (1.f + Ease_Out(EASE::BOUNCE, 2.f * _fRatio - 1.f, _fPower)) * 0.5f;
		case EASE::OVERSHOOT:
			return 1.f - cosf(XM_PI * _fRatio * (_fPower ? _fPower : 4.f)) * (1.f - _fRatio);
		}
		MSG_RETURN(0.f, "Curves::Ease_InOut", "Invalid Ease Function");
	}
#pragma endregion
}
