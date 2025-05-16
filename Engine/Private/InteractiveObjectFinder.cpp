#include "EnginePCH.h"
#include "InteractiveObjectFinder.h"
#include "InteractiveObject.h"
#include "GameInstance.h"

void CInteractiveObjectFinder::Initialize(_uint _iWinSizeX, _uint _iWinSizeY)
{
	m_iWinSizeX = _iWinSizeX;
	m_iWinSizeY = _iWinSizeY;

	m_vScreenCenterPosition = _float2(0, 0);
}

std::shared_ptr<class CInteractiveObject> CInteractiveObjectFinder::Get_FocusedObject()
{
	return m_pFocusedObject;
}

std::shared_ptr<class CInteractiveObject> CInteractiveObjectFinder::Get_FocusedSpecialObject()
{
	return m_FocusedSpecialObject;
}

HRESULT CInteractiveObjectFinder::Append_InteractiveObject(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(_eScene, _wstrLayerTag);

	for (CObjectLayer::ObjectIterator it = pLayer->Begin(); it != pLayer->End(); ++it) 
	{
		std::shared_ptr<CInteractiveObject> pInteractiveObject = dynamic_pointer_cast<CInteractiveObject>(*it);
		m_vecInteractiveObject.push_back(pInteractiveObject);
	}

	return S_OK;
}

void CInteractiveObjectFinder::Add_InteractiveObject(const std::shared_ptr<CInteractiveObject>& _pGameObject)
{
	m_vecInteractiveObject.push_back(_pGameObject);
}

HRESULT CInteractiveObjectFinder::Append_InteractiveSpecialObject(const SCENE _eScene, const wstring& _wstrLayerTag)
{ 
	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(_eScene, _wstrLayerTag);

	for (CObjectLayer::ObjectIterator it = pLayer->Begin(); it != pLayer->End(); ++it)
	{
		std::shared_ptr<CInteractiveObject> pInteractiveObject = dynamic_pointer_cast<CInteractiveObject>(*it);
		m_vecSpecialInteractiveObject.push_back(pInteractiveObject);
	}

	return S_OK;
}

void CInteractiveObjectFinder::Add_InteractiveSpecialObject(const std::shared_ptr<class CInteractiveObject>& _pGameObject)
{
	m_vecSpecialInteractiveObject.push_back(_pGameObject);
}

void CInteractiveObjectFinder::Tick()
{
	if (!m_bSpecialOnly)
	{
		Tick_Objects();
	}
	Tick_SpecialObjects();
}

void CInteractiveObjectFinder::Clear()
{
	m_pFocusedObject = nullptr;
	m_FocusedSpecialObject = nullptr;

	m_vecInteractiveObject.clear();
	m_vecSpecialInteractiveObject.clear();
}

void CInteractiveObjectFinder::Tick_Objects()
{
	auto pPipeline = CPipeLine::Get_Instance();
	_float4x4 mViewProjMatrix = pPipeline->Get_Transform(PIPELINE::VIEW) * pPipeline->Get_Transform(PIPELINE::PROJECTION);
	_float4x4 mCamWorld = pPipeline->Get_Transform(PIPELINE::WORLD);

	m_pFocusedObject = nullptr;
	_float fNearestDistance = FLT_MAX;
	for (auto& pInteractiveObject : m_vecInteractiveObject)
	{
		if (!pInteractiveObject || false == pInteractiveObject->Has_Component(COMPONENT::TRANSFORM))
			continue;

		auto pTransform = dynamic_pointer_cast<CTransform>(pInteractiveObject->Get_Component(COMPONENT::TRANSFORM));

		_matrix mProjectedMatrix = pTransform->Get_Matrix() * mViewProjMatrix;
		_vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		_vector vProjPosition = XMVector3TransformCoord(vPosition, mProjectedMatrix);

		_float fScreenX = XMVectorGetX(vProjPosition);
		_float fScreenY = XMVectorGetY(vProjPosition);

		_float2 vScreenPos;
		vScreenPos.x = (fScreenX) * 0.5f * m_iWinSizeX;
		vScreenPos.y = (fScreenY) * 0.5f * m_iWinSizeY;

		_vector vCamLook = XMLoadFloat4((_float4*)mCamWorld.m[2]);
		_float3 vPos = pTransform->Get_State(TRANSFORM::POSITION);
		_vector vTargetDir = _float4(vPos.x,vPos.y,vPos.z,1) - XMLoadFloat4((_float4*)mCamWorld.m[3]);

		//if (XMVectorGetX(XMVector3Dot(XMVector3Normalize(vCamLook), XMVector3Normalize(vTargetDir))) < 0) // Behind Pass
		//{
		//	continue;
		//}

		_float vScreenDistance = abs((m_vScreenCenterPosition - vScreenPos).length());
		_float vWorldDistance = abs(XMVectorGetX(XMVector3Length(vTargetDir)));
		_float fDistancePoint;
		if(pInteractiveObject->Get_CalculateScreenDistance())
			fDistancePoint = vScreenDistance + vWorldDistance * 2.f;
		else
			fDistancePoint = vWorldDistance * 2.f;

		if (fDistancePoint < fNearestDistance && pInteractiveObject->Get_Interactivable() &&
			vWorldDistance <= m_fFocusingDistanceThreshold && pInteractiveObject->Get_Enable())
		{
			fNearestDistance = fDistancePoint;
		/*	if (vWorldDistance <= m_fFocusingDistanceThreshold)
			{
				if (pInteractiveObject->Get_Enable())	*/
					m_pFocusedObject = pInteractiveObject;
			//}
		}
	}
	//cout << fNearestDistance << endl;
	if (m_pFocusedObject)
		m_pFocusedObject->On_Focusing();
}


void CInteractiveObjectFinder::Tick_SpecialObjects()
{
	auto pPipeline = CPipeLine::Get_Instance();
	_float4x4 mViewProjMatrix = pPipeline->Get_Transform(PIPELINE::VIEW) * pPipeline->Get_Transform(PIPELINE::PROJECTION);
	_float4x4 mCamWorld = pPipeline->Get_Transform(PIPELINE::WORLD);

	m_FocusedSpecialObject = nullptr;
	_float fNearestDistance = FLT_MAX;
	for (auto& pInteractiveObject : m_vecSpecialInteractiveObject)
	{
		if (!pInteractiveObject || false == pInteractiveObject->Has_Component(COMPONENT::TRANSFORM))
			continue;

		auto pTransform = dynamic_pointer_cast<CTransform>(pInteractiveObject->Get_Component(COMPONENT::TRANSFORM));

		_matrix mProjectedMatrix = pTransform->Get_Matrix() * mViewProjMatrix;
		_vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		_vector vProjPosition = XMVector3TransformCoord(vPosition, mProjectedMatrix);

		_float fScreenX = XMVectorGetX(vProjPosition);
		_float fScreenY = XMVectorGetY(vProjPosition);

		_float2 vScreenPos;
		vScreenPos.x = (fScreenX) * 0.5f * m_iWinSizeX;
		vScreenPos.y = (fScreenY) * 0.5f * m_iWinSizeY;

		_vector vCamLook = XMLoadFloat4((_float4*)mCamWorld.m[2]);
		_vector vTargetDir = pTransform->Get_State(TRANSFORM::POSITION) - XMLoadFloat4((_float4*)mCamWorld.m[3]);

		if (XMVectorGetX(XMVector3Dot(XMVector3Normalize(vCamLook), XMVector3Normalize(vTargetDir))) < 0) // Behind Pass
		{
			continue;
		}

		_float vScreenDistance = abs((m_vScreenCenterPosition - vScreenPos).length());
		_float vWorldDistance = abs(XMVectorGetX(XMVector3Length(vTargetDir)));
		_float fDistancePoint = vScreenDistance + vWorldDistance * 2.f;
		if (fDistancePoint < fNearestDistance)
		{
			fNearestDistance = fDistancePoint;
			if (vWorldDistance <= pInteractiveObject->Get_PickWorldDistance() &&
				pInteractiveObject->Get_CanPick())
			{
				if (vScreenDistance <= m_fScreenSpecialFocusingDistanceThreshold)
				{
					if (pInteractiveObject->Get_Enable() && pInteractiveObject->Get_Interactivable())
						m_FocusedSpecialObject = pInteractiveObject;
				}
				else if (m_bSpecialOnly)
				{
					if (pInteractiveObject->Get_Enable() && pInteractiveObject->Get_Interactivable())
						m_vecCapturedObjects.emplace_back(pInteractiveObject);
				}
			}
		}
	}
	if (m_bSpecialOnly)
	{
		_float fNearest = FLT_MAX;
		for (auto& p : m_vecCapturedObjects)
		{
			if (auto pTransform = p->Get_Component<CTransform>(COMPONENT::TRANSFORM))
			{
				_float fDist = _float3(_float3(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION)) - pTransform->Get_State(TRANSFORM::POSITION)).length();
				if (fDist < fNearest)
				{
					fNearest = fDist;
					m_FocusedSpecialObject = p;
				}
			}
		}
		m_vecCapturedObjects.clear();
	}

	if (m_FocusedSpecialObject)
		m_FocusedSpecialObject->On_Focusing();
}

void CInteractiveObjectFinder::EraseObject(shared_ptr<CInteractiveObject> _obj)
{
	auto it = std::find(m_vecInteractiveObject.begin(), m_vecInteractiveObject.end(), _obj);
	if (it != m_vecInteractiveObject.end())
	{
		m_vecInteractiveObject.erase(it);
	}
}

