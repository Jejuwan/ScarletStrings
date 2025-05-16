#pragma once

#include "Engine_Define.h"

class CGeometryGenerator final
{
private:
	CGeometryGenerator() = delete;
	~CGeometryGenerator() = delete;

public:
	static std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ushort>> CreateCube_8();
	static std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ushort>> CreateCube_24();

public:
	static std::tuple<std::vector<VTXPOSNORTEX>, std::vector<_ulong>> Create_Sphere(const _uint _iNumSlices,
		const _uint _iNumStacks);

public:
	static std::tuple<std::vector<VTXPOSTEX>, std::vector<_ushort>> Create_Rect(const float _fScale = 1.f,
		const _float2 _vTexScale = { 1.f, 1.f });
};

