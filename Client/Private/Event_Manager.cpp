#include "ClientPCH.h"
#include "Event_Manager.h"

#include "EventCollider.h"
#include "GameInstance.h"

CEvent_Manager::CEvent_Manager()
{
}

void CEvent_Manager::Bind_EventCollider(SCENE eScene, shared_ptr<class CEventCollider> pEventCollider)
{
	m_mapEventCollider[IDX(eScene)].emplace(pEventCollider->Get_EventTag(), pEventCollider);
}

void CEvent_Manager::Bind_EventCollider(SCENE eScene, const string& strEventTag, shared_ptr<class CEventCollider> pEventCollider)
{
	pEventCollider->Set_EventTag(strEventTag);
	Bind_EventCollider(eScene, pEventCollider);
}

void CEvent_Manager::Enable_EventCollider(SCENE eScene, const string& strEventTag, _bool bEnable)
{
	auto Iter = m_mapEventCollider[IDX(eScene)].find(strEventTag);
	if (Iter == m_mapEventCollider[IDX(eScene)].end())
	{
		MSG_RETURN(, "CEvent_Manager::Enable_EventCollider", "Event Collider Not Binded");
	}

	Iter->second->Set_Enable(bEnable);
}

void CEvent_Manager::Subscribe_Event(SCENE eScene, const string& strEventTag, function<void(EVENT_TYPE, string)> fpEventCallback)
{
	auto Iter = m_mapEventCollider[IDX(eScene)].find(strEventTag);
	if (Iter == m_mapEventCollider[IDX(eScene)].end())
	{
		MSG_RETURN(, "CEvent_Manager::Enable_EventCollider", "Event Collider Not Binded");
	}

	Iter->second->Subscribe_Enter_Callback(fpEventCallback);
}
