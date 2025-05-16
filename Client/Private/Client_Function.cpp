#include "ClientPCH.h"
#include "Client_Function.h"
#include "Client_Define.h"
#include "GameInstance.h"
#include "Player.h"
#include "CurveContainer.h"

_float2 Function::Clip_To_Screen(_float3 _vClipSpace, _bool _bCenterSpace)
{
	if (_bCenterSpace)
	{
		return _float2(_vClipSpace) * 0.5f * _float2(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY));
	}
	else
	{
		return (_float2(_vClipSpace) * _float2(0.5f, -0.5f) + _float2(0.5f, 0.5f)) * _float2(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY));
	}
}

_float4 Function::Screen_To_World(_float2 _vScreenSpace, _float _fZ)
{
	_float	fX		= (2.f * _vScreenSpace.x) / static_cast<_float>(g_iWinCX) - 1.f;
	_float	fY		= 1.f - (2.f * _vScreenSpace.y) / static_cast<_float>(g_iWinCY);

	_vector vNDC	= XMVectorSet(fX, fY, 1.f, 1.f);


	XMVECTOR vWorld	= XMVector3TransformCoord(vNDC, XMMatrixInverse(
		nullptr, XMMatrixMultiply(CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW), CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))));

	XMVECTOR vDir	= XMVector3Normalize(XMVectorSubtract(vWorld, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION)));

	return XMVectorAdd(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION), XMVectorScale(vDir, _fZ));
}

_float3 Function::Rotate_By_Transform(shared_ptr<CTransform> _pTransform, _float3 _vTranslation)
{
	_vector vScale, vQuaternion, vTranslation;
	
	if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, _pTransform->Get_Matrix()))
	{
		return XMVector3Rotate(XMLoadFloat3(&_vTranslation), vQuaternion);
	}

	return _vTranslation;
}

_float Function::MonteCarloRandom(shared_ptr<CCurveContainer> _pCC, _float _fMaxValue)
{
	while (true)
	{
		_float x = static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX) * _pCC->Get_Total();
		_float y = static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX) * _fMaxValue;

		if (y <= _pCC->Get_CurveValue(x))
		{
			return x;
		}
	}
}

_float Function::MonteCarloMaxValue(shared_ptr<CCurveContainer> _pCC, _uint _iSampleState)
{
	_float fTotal			= _pCC->Get_Total();
	_float fSampleInterval	= fTotal / static_cast<_float>(_iSampleState);
	_float fMaxValue		= 0.f;

	for (_float f = 0.f; f < fTotal; f += fSampleInterval)
	{
		if (fMaxValue < _pCC->Get_CurveValue(f))
		{
			fMaxValue = _pCC->Get_CurveValue(f);
		}
	}

	return fMaxValue;
}

_float Function::PlaneAcos2(_float3 _vNormal, _float3 _vA, _float3 _vB)
{
	_vector vCross	= XMVector3Cross(_vA, _vB);
	_float	fDot	= XMVectorGetX(XMVector3Dot(_vA, _vB));

	return atan2(XMVectorGetX(XMVector3Length(vCross)) * (XMVectorGetX(XMVector3Dot(vCross, _vNormal)) < 0 ? -1 : 1), fDot);
}

shared_ptr<CGameObject> Function::Find_Player()
{
	shared_ptr<CGameObject> pPlayer;

	for (_uint i = 0; i < IDX(SCENE::MAX); ++i)
	{
		CGameInstance::Get_Instance()->Iterate_Layers(static_cast<SCENE>(i),
			[&](pair<wstring, shared_ptr<CObjectLayer>> _pair)->_bool
			{
				_pair.second->Iterate_Objects(
					[&](shared_ptr<CGameObject> _pObject)->_bool
					{
						if (dynamic_pointer_cast<CPlayer>(_pObject))
						{
							pPlayer = _pObject;
							return false;
						}
						return true;
					}
				);
				return !pPlayer;
			}
		);
	}
	
	return pPlayer;
}

_bool Function::Is_Cursor_In(_float2 _vPosition, _float2 _vSize)
{
	POINT	ptCursor = CGameInstance::Get_Instance()->Get_CursorPos();

	return Function::InRange(ptCursor.x, static_cast<_long>(_vPosition.x - _vSize.x * 0.5f), static_cast<_long>(_vPosition.x + _vSize.x * 0.5f))
		&& Function::InRange(ptCursor.y, static_cast<_long>(_vPosition.y - _vSize.y * 0.5f), static_cast<_long>(_vPosition.y + _vSize.y * 0.5f));
}

_float Function::Distance_Camera(_float3 _vPosition)
{
	return _float3(_vPosition - CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION)).length();
}

_float Function::Distance(shared_ptr<CTransform> _pTransformA, shared_ptr<CTransform> _pTransformB)
{
	return _float3(_pTransformA->Get_State(TRANSFORM::POSITION) - _pTransformB->Get_State(TRANSFORM::POSITION)).length();
}

_float Function::RelativeAngle(shared_ptr<CTransform> _pOwner, shared_ptr<CTransform> _pOpponent)
{
	_float3 vOwner		= _pOwner->Get_State(TRANSFORM::POSITION);
	_float3 vOpponent	= _pOpponent->Get_State(TRANSFORM::POSITION);

	_float3 vLook		= _pOwner->Get_State(TRANSFORM::LOOK).normalize();
	_float3 vRelative	= _float3(vOpponent.x - vOwner.x, 0.f, vOpponent.z - vOwner.z).normalize();

	_float3	vCross		= XMVector3Cross(vLook, vRelative);
	_float	fDot		= XMVectorGetX(XMVector3Dot(vLook, vRelative));
	_float	fAngle		= acosf(Function::Clamp(-1.f, 1.f, fDot));

	if (vCross.y < 0.f)	fAngle = -fAngle;

	return fAngle;
}

_float Function::RelativeAngle(_float3 _vOwnerLook, _float3 _vOwnerPos, _float3 _vOpponentPos)
{
	_float3 vLook		= _vOwnerLook.normalize();
	_float3 vRelative	= _float3(_vOpponentPos.x - _vOwnerPos.x, 0.f, _vOpponentPos.z - _vOwnerPos.z).normalize();

	_float3	vCross		= XMVector3Cross(vLook, vRelative);
	_float	fDot		= XMVectorGetX(XMVector3Dot(vLook, vRelative));
	_float	fAngle		= acosf(Function::Clamp(-1.f, 1.f, fDot));

	if (vCross.y < 0.f)	fAngle = -fAngle;

	return fAngle;
}

/*
_float2 Function::Compute_RelativeAngle(shared_ptr<CTransform> _pCriteria, shared_ptr<CTransform> _pTarget)
{
	_float3	vLook		= _pCriteria->Get_State(TRANSFORM::LOOK).normalize();
	_float3	vRelative	= _float3(_pTarget->Get_State(TRANSFORM::POSITION) - _pCriteria->Get_State(TRANSFORM::POSITION)).normalize();

	_float	fHorizontal	= atan2(vLook.z, vLook.x) - atan2(vRelative.z, vRelative.x);
	_float	fVertical	= asin(vRelative.y) - asin(vLook.y);

	return _float2(fHorizontal, fVertical);
}
*/
