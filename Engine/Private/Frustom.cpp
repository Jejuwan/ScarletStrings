#include "EnginePCH.h"
#include "Frustom.h"

#include "PipeLine.h"

void CFrustom::Tick()
{
	m_vFrustomVertex[0].x = -1.0f;	m_vFrustomVertex[0].y = -1.0f;	m_vFrustomVertex[0].z = 0.0f;
	m_vFrustomVertex[1].x = 1.0f;	m_vFrustomVertex[1].y = -1.0f;	m_vFrustomVertex[1].z = 0.0f;
	m_vFrustomVertex[2].x = 1.0f;	m_vFrustomVertex[2].y = -1.0f;	m_vFrustomVertex[2].z = 1.0f;
	m_vFrustomVertex[3].x = -1.0f;	m_vFrustomVertex[3].y = -1.0f;	m_vFrustomVertex[3].z = 1.0f;
	m_vFrustomVertex[4].x = -1.0f;	m_vFrustomVertex[4].y = 1.0f;	m_vFrustomVertex[4].z = 0.0f;
	m_vFrustomVertex[5].x = 1.0f;	m_vFrustomVertex[5].y = 1.0f;	m_vFrustomVertex[5].z = 0.0f;
	m_vFrustomVertex[6].x = 1.0f;	m_vFrustomVertex[6].y = 1.0f;	m_vFrustomVertex[6].z = 1.0f;
	m_vFrustomVertex[7].x = -1.0f;	m_vFrustomVertex[7].y = 1.0f;	m_vFrustomVertex[7].z = 1.0f;

	auto pPipeLine = CPipeLine::Get_Instance();
	_matrix mViewProj = pPipeLine->Get_Transform(PIPELINE::VIEW) * pPipeLine->Get_Transform(PIPELINE::PROJECTION);
	_matrix mViewProjInv = XMMatrixInverse(nullptr, mViewProj);

	for (_uint i = 0; i < 8; ++i)
	{
		_vector vFrustomWorld = XMVector3TransformCoord(XMLoadFloat3(&m_vFrustomVertex[i]), mViewProjInv);
		XMStoreFloat3(&m_vFrustomVertex[i], vFrustomWorld);
	}

	XMStoreFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_FAR)], XMPlaneFromPoints(XMLoadFloat3(m_vFrustomVertex + 2), XMLoadFloat3(m_vFrustomVertex + 6), XMLoadFloat3(m_vFrustomVertex + 7)));
	XMStoreFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_LEFT)], XMPlaneFromPoints(XMLoadFloat3(m_vFrustomVertex + 0), XMLoadFloat3(m_vFrustomVertex + 3), XMLoadFloat3(m_vFrustomVertex + 7)));
	XMStoreFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_RIGHT)], XMPlaneFromPoints(XMLoadFloat3(m_vFrustomVertex + 1), XMLoadFloat3(m_vFrustomVertex + 5), XMLoadFloat3(m_vFrustomVertex + 6)));
}

_bool CFrustom::Culling_Point(_vector vPosition)
{
	float fDistance = 0.f;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_FAR)]), vPosition));
	if (fDistance > PLANE_EPSILON)
		return false;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_LEFT)]), vPosition));
	if (fDistance > PLANE_EPSILON)
		return false;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_RIGHT)]), vPosition));
	if (fDistance > PLANE_EPSILON)
		return false;

	return true;
}

_bool CFrustom::Culling_Bounding_Sphere(_vector vPosition, _float fRadius)
{
	float fDistance = 0.f;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_FAR)]), vPosition));
	if (fDistance > (PLANE_EPSILON + fRadius))
		return false;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_LEFT)]), vPosition));
	if (fDistance > (PLANE_EPSILON + fRadius))
		return false;
	fDistance = XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane[IDX(FRUSTOM_TYPE::FRUSTOM_RIGHT)]), vPosition));
	if (fDistance > (PLANE_EPSILON + fRadius))
		return false;

	return true;
}
