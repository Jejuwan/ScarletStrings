#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CObjectPool final
{
private:
	explicit CObjectPool(const SCENE, const _uint iPoolSize);
	virtual ~CObjectPool() DEFAULT;

public:
	HRESULT											Initialize(const wstring& wstrPrototype, any = g_aNull);
	HRESULT											Initialize(shared_ptr<class CGameObject> pPrototype, any = g_aNull);
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CGameObject>					Pop(any aFetchArg = g_aNull, _bool PoolOut = false);
	HRESULT											Push(shared_ptr<class CGameObject>);

	void											Iterate_Objects(function<_bool(shared_ptr<class CGameObject>)>);

private:
	void											Add();

private:
 	unordered_set<shared_ptr<class CGameObject>>	m_usetPop;
	deque<shared_ptr<class CGameObject>>			m_deqPool;
	list<shared_ptr<class CGameObject>>				m_lstPop;
	list<shared_ptr<class CGameObject>>				m_lstPush;

	_uint											m_iPoolSize;

	shared_ptr<class CGameObject>					m_pPrototype;
	any												m_arg;

	const SCENE										m_eScene;

public:
	static shared_ptr<CObjectPool>					Create(const SCENE, const _uint iPoolSize, const wstring& wstrPrototypeTag, any = g_aNull);
	static shared_ptr<CObjectPool>					Create(const SCENE, const _uint iPoolSize, shared_ptr<class CGameObject> pPrototype, any = g_aNull);
};

END
