#include "Engine_Function.h"

namespace Function
{
	_float Lerp(_float _fStart, _float _fEnd, _float _fRatio, _float _fWeight)
	{
		_float fRatio(powf(_fRatio, _fWeight));
		return (_fStart * (1.f - fRatio)) + (_fEnd * fRatio);
	}

	XMVECTOR Lerp(FXMVECTOR _vStart, FXMVECTOR _vEnd, _float _fRatio, _float _fWeight)
	{
		_float fRatio(powf(_fRatio, _fWeight));
		return XMVectorLerp(_vStart, _vEnd, fRatio);
	}

	XMMATRIX Lerp(FXMMATRIX _mStart, CXMMATRIX _mEnd, _float _fRatio, _float _fWeight, _bool _bScale, _bool _bRotation, _bool _bTranslation)
	{
		if (0.f == _fRatio) return _mStart;
		if (1.f == _fRatio) return _mEnd;

		XMVECTOR vStartScale, vStartRotation, vStartTranslation;
		XMVECTOR vEndScale,	vEndRotation, vEndTranslation;

		if (!XMMatrixDecompose(&vStartScale, &vStartRotation, &vStartTranslation, _mStart))
		{
			XMMATRIX mRotation;
			mRotation.r[0] = XMVector3Normalize(_mStart.r[0]);
			mRotation.r[1] = XMVector3Normalize(_mStart.r[1]);
			mRotation.r[2] = XMVector3Normalize(_mStart.r[2]);

			vStartScale			= XMVectorSet(
				XMVectorGetX(XMVector3Length(_mStart.r[0])),
				XMVectorGetY(XMVector3Length(_mStart.r[1])),
				XMVectorGetZ(XMVector3Length(_mStart.r[2])),
				1.f
			);
			vStartRotation		= XMQuaternionRotationMatrix(mRotation);
			vStartTranslation	= _mStart.r[3];
		}

		if (!XMMatrixDecompose(&vEndScale, &vEndRotation, &vEndTranslation, _mEnd))
		{
			XMMATRIX mRotation;
			mRotation.r[0] = XMVector3Normalize(_mEnd.r[0]);
			mRotation.r[1] = XMVector3Normalize(_mEnd.r[1]);
			mRotation.r[2] = XMVector3Normalize(_mEnd.r[2]);

			vEndScale			= XMVectorSet(
				XMVectorGetX(XMVector3Length(_mEnd.r[0])),
				XMVectorGetY(XMVector3Length(_mEnd.r[1])),
				XMVectorGetZ(XMVector3Length(_mEnd.r[2])),
				1.f
			);
			vEndRotation		= XMQuaternionRotationMatrix(mRotation);
			vEndTranslation		= _mEnd.r[3];
		}

		_float fRatio(powf(_fRatio, _fWeight));
		XMVECTOR vScale			= !_bScale			? vStartScale		: XMVectorLerp(vStartScale, vEndScale, fRatio);
		XMVECTOR vRotation		= !_bRotation		? vStartRotation	: XMQuaternionSlerp(vStartRotation, vEndRotation, fRatio);
		XMVECTOR vTranslation	= !_bTranslation	? vStartTranslation	: XMVectorLerp(vStartTranslation, vEndTranslation, fRatio);

		return XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	}

	XMVECTOR Slerp(FXMVECTOR _vQuaternion, _float _fRatio, _float _fWeight)
	{
		_float fRatio(fmodf(powf(fabsf(_fRatio), _fWeight), 1.f));
		XMVECTOR vQuaternion = _fRatio >= 0.f ? _vQuaternion : XMQuaternionInverse(_vQuaternion);

		return XMQuaternionSlerp(XMQuaternionIdentity(), XMQuaternionNormalize(vQuaternion), fRatio);
	}

	_float ProportionalRatio(_float _fMin, _float _fMax, _float _fValue)
	{
		if (_fMin == _fMax) return 0.f;
		return (_fValue - _fMin) / (_fMax - _fMin);
	}

	_bool NearZero(_float _fValue)
	{
		return fabsf(_fValue) <= g_fTolorance;
	}

	_bool NearZero2(FXMVECTOR _fVector2)
	{
		return XMVector2NearEqual(XMVectorZero(), _fVector2, XMVectorSet(g_fTolorance, g_fTolorance, 0.f, 0.f));
	}

	_bool NearZero3(FXMVECTOR _fVector3)
	{
		return XMVector3NearEqual(XMVectorZero(), _fVector3, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, 0.f));
	}

	_bool NearZero4(FXMVECTOR _fVector4)
	{
		return XMVector4NearEqual(XMVectorZero(), _fVector4, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, g_fTolorance));
	}

	_uint Probability(initializer_list<_float> _il)
	{
		_uint	iSize	= 0;
		_float	fAcc	= 0.f;
		_float	fTotal	= 0.f;
		_float	fRandom	= static_cast<_float>(rand()) / RAND_MAX;

		for (_float fPercentage : _il)
		{
			fTotal += fPercentage;
		}

		for (_float fPercentage : _il)
		{
			fAcc += fPercentage / fTotal;
			if (fRandom < fAcc)
			{
				return iSize;
			}
			++iSize;
		}

		return static_cast<_uint>(rand() % iSize);
	}

	_float RandomFloat(_float _fMin, _float _fMax)
	{
		static std::random_device	device;
		static std::mt19937			generator(device());

		if (_fMin > _fMax)
		{
			std::swap(_fMin, _fMax);
		}

		std::uniform_real_distribution<_float> value(_fMin, _fMax);

		return value(generator);
	}

	XMVECTOR RandomVector(FXMVECTOR _vMin, FXMVECTOR _vMax)
	{
		return XMVectorSet(
			Function::RandomFloat(XMVectorGetX(_vMin), XMVectorGetX(_vMax)),
			Function::RandomFloat(XMVectorGetY(_vMin), XMVectorGetY(_vMax)),
			Function::RandomFloat(XMVectorGetZ(_vMin), XMVectorGetZ(_vMax)),
			Function::RandomFloat(XMVectorGetW(_vMin), XMVectorGetW(_vMax)));
	}

	_float QuaternionToAngle(FXMVECTOR _vQuaternion, _bool _bToRadians)
	{
		_float fTheta = acosf(XMVectorGetW(XMQuaternionNormalize(_vQuaternion))) * 2.f;
		return _bToRadians ? fTheta : XMConvertToDegrees(fTheta);
	}

	XMVECTOR QuaternionToEuler(FXMVECTOR _vQuaternion, _bool _bToRadians)
	{
		XMFLOAT4 vQuaternion;
		XMStoreFloat4(&vQuaternion, XMQuaternionNormalize(_vQuaternion));

		_float fSquareX = vQuaternion.x * vQuaternion.x;
		_float fSquareY	= vQuaternion.y * vQuaternion.y;
		_float fSquareZ	= vQuaternion.z * vQuaternion.z;
		_float fSquareW	= vQuaternion.w * vQuaternion.w;

		_float fPitch	= asinf(2.f * (vQuaternion.w * vQuaternion.x - vQuaternion.y * vQuaternion.z));
		_float fYaw		= atan2f(2.f * (vQuaternion.x * vQuaternion.z + vQuaternion.w * vQuaternion.y), (-fSquareX - fSquareY + fSquareZ + fSquareW));
		_float fRoll	= atan2f(2.f * (vQuaternion.x * vQuaternion.y + vQuaternion.w * vQuaternion.z), (-fSquareX + fSquareY - fSquareZ + fSquareW));

		return _bToRadians ? XMVectorSet(fPitch, fYaw, fRoll, 0.f) : RadiansToDegrees(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
	}

	XMVECTOR DegreesToRadians(FXMVECTOR _vDegrees)
	{
		return _vDegrees * (XM_PI / 180.0f);
	}

	XMVECTOR RadiansToDegrees(FXMVECTOR _vRadians)
	{
		return _vRadians * (180.0f / XM_PI);
	}

	string ConvertString(const wstring& _wstr)
	{
		_uint	strSize	= WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), -1, NULL, 0, NULL, NULL);
		_char*	str		= new _char[strSize];

		WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), -1, str, strSize, NULL, NULL);
		string strResult(str);
		delete[] str;

		return strResult;
	}

	wstring ConvertString(const string& _str)
	{
		_uint	wstrSize	= MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, NULL, 0);
		_wchar*	wstr		= new _wchar[wstrSize];

		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, wstr, wstrSize);
		wstring wstrResult(wstr);
		delete[] wstr;

		return wstrResult;
	}

	string ModString(const string& _str, const string& _strTarget, const string& _strReplace)
	{
		string	strResult(_str);
		size_t	nPos(0);

		while ((nPos = strResult.find(_strTarget, nPos)) != string::npos)
		{
			strResult.replace(nPos, _strTarget.length(), _strReplace);
			nPos += _strReplace.length();
		}
			
		return strResult;
	}

	wstring ModString(const wstring& _wstr, const wstring& _wstrTarget, const wstring& _wstrReplace)
	{
		wstring	wstrResult(_wstr);
		size_t	nPos(0);

		while ((nPos = wstrResult.find(_wstrTarget, nPos)) != wstring::npos)
		{
			wstrResult.replace(nPos, _wstrTarget.length(), _wstrReplace);
			nPos += _wstrReplace.length();
		}

		return wstrResult;
	}

	void SplitPath(_In_ const string& _str, _Out_opt_ string* _pDrive, _Out_opt_ string* _pDirectory, _Out_opt_ string* _pFileName, _Out_opt_ string* _pExtension)
	{
		_char	szDrive[MAX_PATH]			= "";
		_char	szDirectory[MAX_PATH]		= "";
		_char	szFileName[MAX_PATH]		= "";
		_char	szExt[MAX_PATH]				= "";
	
		_splitpath_s(_str.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		
		if (_pDrive)		*_pDrive		= szDrive;
		if (_pDirectory)	*_pDirectory	= szDirectory;
		if (_pFileName)		*_pFileName		= szFileName;
		if (_pExtension)	*_pExtension	= szExt;
	}

	void SplitPath(_In_ const wstring& _wstr, _Out_opt_ wstring* _pDrive, _Out_opt_ wstring* _pDirectory, _Out_opt_ wstring* _pFileName, _Out_opt_ wstring* _pExtension)
	{
		_wchar	szDrive[MAX_PATH]			= TEXT("");
		_wchar	szDirectory[MAX_PATH]		= TEXT("");
		_wchar	szFileName[MAX_PATH]		= TEXT("");
		_wchar	szExt[MAX_PATH]				= TEXT("");
	
		_wsplitpath_s(_wstr.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		
		if (_pDrive)		*_pDrive		= szDrive;
		if (_pDirectory)	*_pDirectory	= szDirectory;
		if (_pFileName)		*_pFileName		= szFileName;
		if (_pExtension)	*_pExtension	= szExt;
	}

	XMVECTOR ComputeNormal(FXMVECTOR _vA, FXMVECTOR _vB, FXMVECTOR _vC)
	{
		XMVECTOR vAB = _vB - _vA;
		XMVECTOR vAC = _vC - _vA;

		return XMVector3Normalize(XMVector3Cross(vAB, vAC));
	}

	XMVECTOR ComputeSlideVector(FXMVECTOR _vDirection, FXMVECTOR _vNormal, _bool _bReturnReflection)
	{
		XMVECTOR vNormalProjection = _vNormal * XMVectorGetX(XMVector3Dot(_vDirection, _vNormal));

		return _bReturnReflection ? vNormalProjection : _vDirection - vNormalProjection;
	}

	_uint Choose(_uint _iSize, _uint _iNum)
	{
		return _iNum ? Choose(_iSize - 1, _iNum - 1) * _iSize / _iNum : 1;
	}

	_float Bernstein(_uint _iSize, _uint _iNum, _float _fRatio)
	{
		return Choose(_iSize, _iNum) * powf(_fRatio, static_cast<_float>(_iNum)) * powf(1.f - _fRatio, static_cast<_float>(_iSize - _iNum));
	}

	_float XZPlaneRadians(FXMVECTOR _vDirection)
	{
		XMVECTOR	vDirection	= XMVector3Normalize(_vDirection);
		XMVECTOR	vProjection	= XMVector3Normalize(XMVectorSet(XMVectorGetX(vDirection), 0.f, XMVectorGetZ(vDirection), 0.f));
		_float		fDot		= XMVectorGetX(XMVector3Dot(vDirection, vProjection));
		
		return XMVectorGetY(vDirection) >= 0.f ? acosf(fDot) : -acosf(fDot);
	}

	XMMATRIX MatrixRandomRotation(FXMMATRIX _m)
	{
		_float fX		= static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX);
		_float fY		= static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX);
		_float fZ		= static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX);
		_float fAngle	= static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX) * XM_2PI;

		XMVECTOR vAxis			= XMVector3Normalize(XMVectorSet(fX, fY, fZ, 0.f));
		XMVECTOR vQuaternion	= XMQuaternionRotationAxis(vAxis, fAngle);
		XMMATRIX mRotation		= XMMatrixRotationQuaternion(vQuaternion);

		return mRotation * _m;
	}

	XMMATRIX HorizontalRotationMaxtrix(FXMMATRIX _mWorld)
	{
		XMVECTOR	vRight		= _mWorld.r[0];
		_float		fAngle		= -atan2(XMVectorGetY(vRight), XMVectorGetZ(vRight));
		XMMATRIX	mRotation	= XMMatrixRotationY(fAngle);

		return mRotation * _mWorld;
	}

	XMVECTOR Cone_DiffusionAngle(FXMVECTOR _vReference, _float _fTheta, _float _fPhi)
	{
		XMFLOAT3	vVector;

		_float		fAlpha	= _fTheta + RandomFloat(0.f, 1.f) * (_fPhi - _fTheta);
		_float		fBeta	= 2.f * RandomFloat(0.f, 1.f) * XM_PI;

		vVector.x			= sin(fAlpha) * cos(fBeta);
		vVector.y			= sin(fAlpha) * sin(fBeta);
		vVector.z			= cos(fAlpha);

		XMVECTOR	vAxis	= XMVector3Cross(XMVectorSet(0.f, 0.f, 1.f, 0.f), _vReference);
		_float		fAngle	= XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.f, 0.f, 1.f, 0.f), _vReference));

		return XMVector3Transform(XMLoadFloat3(&vVector), XMMatrixRotationAxis(vAxis, fAngle));
	}
}

namespace DirectX
{
	XMVECTOR QuaternionBetweenAxis(FXMVECTOR _vAxisA, FXMVECTOR _vAxisB)
	{
		XMVECTOR vAxisA	= XMVector3Normalize(_vAxisA);
		XMVECTOR vAxisB = XMVector3Normalize(_vAxisB);

		XMVECTOR vCross	= XMVector3Cross(vAxisA, vAxisB);
		_float fDot		= XMVectorGetX(XMVector3Dot(vAxisA, vAxisB));
		
		if (fDot > 1.f)		fDot = 1.f;
		if (fDot < -1.f)	fDot = -1.f;

		if (XMVector3Equal(vCross, XMVectorZero()))
		{
			return fDot > 0 ? XMQuaternionIdentity() : XMQuaternionRotationAxis(vAxisA, XM_PI);
		}

		return XMQuaternionRotationAxis(vCross, acosf(fDot));
	}
}
