#include "EnginePCH.h"
#include "Curves.h"
#include "Curve.h"
#include "Spline.h"
#include "Ease.h"

shared_ptr<ICurves> ICurves::Read(std::ifstream& _inFile)
{
	Curves::TYPE eType(Curves::TYPE::MAX);
	_inFile.read(reinterpret_cast<_byte*>(&eType), sizeof(Curves::TYPE));

	switch (eType)
	{
	case Curves::TYPE::CURVE:
		return CCurve::Read(_inFile);
	case Curves::TYPE::SPLINE:
		return CSpline::Read(_inFile);
	case Curves::TYPE::EASE:
		return CEase::Read(_inFile);
	}

	MSG_RETURN(nullptr, "ICurves::Read", "Invalid Type");
}
