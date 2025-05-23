#pragma once

namespace Function
{
	inline _float						Lerp(_float fStart, _float fEnd, _float fRatio, _float fWeight = 1.f);
	inline XMVECTOR						Lerp(FXMVECTOR vStart, FXMVECTOR vEnd, _float fRatio, _float fWeight = 1.f);
	inline XMMATRIX						Lerp(FXMMATRIX mStart, CXMMATRIX mEnd, _float fRatio, _float fWeight = 1.f, _bool bScale = true, _bool bRotation = true, _bool bTranslation = true);
	inline XMVECTOR						Slerp(FXMVECTOR vQuaternion, _float fRatio, _float fWeight = 1.f);

	inline _float						ProportionalRatio(_float fMin, _float fMax, _float fValue);

	inline _bool						NearZero(_float fValue);
	inline _bool						NearZero2(FXMVECTOR fVector2);
	inline _bool						NearZero3(FXMVECTOR fVector3);
	inline _bool						NearZero4(FXMVECTOR fVector4);

	inline _uint						Probability(initializer_list<_float>);
	inline _float						RandomFloat(_float fMin = -FLT_MAX, _float fMax = FLT_MAX);
	inline XMVECTOR						RandomVector(FXMVECTOR vMin = XMVectorReplicate(-FLT_MAX), FXMVECTOR vMax = XMVectorReplicate(FLT_MAX));

	inline _float						QuaternionToAngle(FXMVECTOR vQuaternion, _bool bToRadians = true);
	inline XMVECTOR						QuaternionToEuler(FXMVECTOR vQuaternion, _bool bToRadians = true);
	inline XMVECTOR						DegreesToRadians(FXMVECTOR vDegrees);
	inline XMVECTOR						RadiansToDegrees(FXMVECTOR vRadians);

	inline string						ConvertString(const wstring& wstr);
	inline wstring						ConvertString(const string& str);
	inline string						ModString(const string&, const string& strTarget, const string& strReplace = string());
	inline wstring						ModString(const wstring&, const wstring& wstrTarget, const wstring& wstrReplace = wstring());
	inline void							SplitPath(_In_ const string& _str, _Out_opt_ string* _pDrive, _Out_opt_ string* _pDirectory, _Out_opt_ string* _pFileName, _Out_opt_ string* _pExtension);
	inline void							SplitPath(_In_ const wstring& _wstr, _Out_opt_ wstring* _pDrive, _Out_opt_ wstring* _pDirectory, _Out_opt_ wstring* _pFileName, _Out_opt_ wstring* _pExtension);

	inline XMVECTOR						ComputeNormal(FXMVECTOR, FXMVECTOR, FXMVECTOR);
	inline XMVECTOR						ComputeSlideVector(FXMVECTOR vDirection, FXMVECTOR vNormal, _bool bReturnReflection = false);

	inline XMVECTOR						Cone_DiffusionAngle(FXMVECTOR, _float fTheta, _float fPhi);

	inline _uint						Choose(_uint iSize, _uint iNum);
	inline _float						Bernstein(_uint iSize, _uint iNum, _float fRatio);

	inline _float						XZPlaneRadians(FXMVECTOR);

	inline XMMATRIX						MatrixRandomRotation(FXMMATRIX);
	inline XMMATRIX						HorizontalRotationMaxtrix(FXMMATRIX);

#pragma region Template

	template<typename T>
	void Swap(T& lhs, T& rhs)
	{
		T temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Min(T value)
	{
		return value;
	}
	template <typename T, typename... Args>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Min(T first, Args&&... args)
	{
		return min(first, Min(args...));
	}
	
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Max(T value)
	{
		return value;
	}
	template <typename T, typename... Args>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Max(T first, Args&&... args)
	{
		return max(first, Max(args...));
	}

	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value, _bool>::type
	InRange(T value, T low, T high, const string& range = "[)")
	{
		if (range == "()") {
			return value >	low && value <	high;
		}
		else if (range == "(]") {
			return value >	low && value <=	high;
		}
		else if (range == "[)") {
			return value >=	low && value <	high;
		}
		else if (range == "[]") {
			return value >=	low && value <=	high;
		}
		else
		{
			throw std::invalid_argument("Function::InRange: Invalid Range Option");
		}
	}

	template<typename T>
	T Random(initializer_list<T> _il)
	{
		assert(0 < _il.size());

		auto it = _il.begin();
		std::advance(it, rand() % _il.size());

		return *it;
	}

	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	Clamp(T low, T high, T value)
	{
		return min(max(value, low), high);
	}

	template<typename T>
	vector<T*> ConvertToRawPtrVector(const vector<ComPtr<T>>& comPtrVector)
	{
		std::vector<T*> rawPtrVector;
		rawPtrVector.reserve(comPtrVector.size());

		for (const auto& comPtr : comPtrVector) {
			rawPtrVector.emplace_back(comPtr.Get());
		}

		return rawPtrVector;
	}

	template<typename T>
	unique_ptr<T[], std::default_delete<T[]>> CreateDynamicArray(size_t arrSize, _bool zeroMemory = true)
	{
		T* rawDynamicArray = new T[arrSize];
		if (zeroMemory) ZeroMemory(rawDynamicArray, arrSize * sizeof(T));
		return move(unique_ptr<T[], std::default_delete<T[]>>(rawDynamicArray));
	}

#pragma endregion
}

namespace DirectX
{
	inline XMVECTOR QuaternionBetweenAxis(FXMVECTOR, FXMVECTOR);
}

template <typename T>
void Safe_Delete(T& _p)
{
	if (nullptr != _p)
	{
		delete _p;
		_p = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& _p)
{
	if (nullptr != _p)
	{
		delete[] _p;
		_p = nullptr;
	}
}

#include "Engine_Function.inl"
