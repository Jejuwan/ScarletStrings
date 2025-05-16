#pragma once
#include "Client_Define.h"
#include "CurveContainer.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CCurveContainer;
END

namespace Function
{
	shared_ptr<CGameObject>		Find_Player();

	_bool						Is_Cursor_In(_float2 vPosition, _float2 vSize);

	_float						Distance_Camera(_float3 vPosition);
	_float						Distance(shared_ptr<CTransform>, shared_ptr<CTransform>);
	_float						RelativeAngle(shared_ptr<CTransform> pOwner, shared_ptr<CTransform> pOpponent);
	_float						RelativeAngle(_float3 vOwnerLook, _float3 vOwnerPos, _float3 vOpponentPos);
	_float2						Clip_To_Screen(_float3 vClipSpace, _bool bCenterSpace = false);
	_float4						Screen_To_World(_float2 vScreenSpace, _float fZ = 0.f);

	_float3						Rotate_By_Transform(shared_ptr<CTransform>, _float3);

	_float						MonteCarloRandom(shared_ptr<CCurveContainer>, _float fMaxValue);
	_float						MonteCarloMaxValue(shared_ptr<CCurveContainer>, _uint iSampleState);

	_float						PlaneAcos2(_float3 vNormal, _float3 vA, _float3 vB);

//	_float2						Compute_RelativeAngle(shared_ptr<CTransform> pCriteria, shared_ptr<CTransform> pTarget);
}
