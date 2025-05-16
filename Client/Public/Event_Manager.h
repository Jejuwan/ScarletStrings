#pragma once

#include "Client_Define.h"

BEGIN(Client)

class CEvent_Manager : public CSingleton<CEvent_Manager>
{
private:
	explicit CEvent_Manager();
	virtual ~CEvent_Manager() DEFAULT;

public:
	void														Bind_EventCollider(SCENE eScene, shared_ptr<class CEventCollider>);
	void														Bind_EventCollider(SCENE eScene, const string& strEventTag, shared_ptr<class CEventCollider>);
	void														Enable_EventCollider(SCENE eScene, const string& strEventTag, _bool bEnable);

	void														Subscribe_Event(SCENE eScene, const string& strEventTag, function<void(EVENT_TYPE, string)> fpEventCallback);

private:
	unordered_map<string, shared_ptr<class CEventCollider>>		m_mapEventCollider[IDX(SCENE::MAX)];

private:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;

	friend		CSingleton<CEvent_Manager>;
};

END