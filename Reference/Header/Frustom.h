#pragma once

#include "Engine_Define.h"

#define PLANE_EPSILON	5.0f

BEGIN(Engine)

class ENGINE_DLL CFrustom final : public CSingleton<CFrustom>
{
private:
	enum class FRUSTOM_TYPE { FRUSTOM_FAR, FRUSTOM_LEFT, FRUSTOM_RIGHT, MAX};

private:
	explicit CFrustom() DEFAULT;
	virtual ~CFrustom() DEFAULT;

public:
	void Tick();

public:
	_bool Culling_Point(_vector vPosition);
	_bool Culling_Bounding_Sphere(_vector vPosition, _float fRadius);

private:
	_float3 m_vFrustomVertex[8];
	_float4 m_vPlane[IDX(FRUSTOM_TYPE::MAX)];


	friend CSingleton<CFrustom>;
};

END