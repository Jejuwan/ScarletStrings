#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CObjectLayer final
{
private:
	explicit CObjectLayer(const SCENE);
	virtual ~CObjectLayer() DEFAULT;

public:
	void											Tick(_float fTimeDelta);
	void											Late_Tick(_float fTimeDelta);

public:
	void											Set_TimeScale(_float fTimeScale) { m_fTimeScale = fTimeScale; }
	_float											Get_TimeScale() { return m_fTimeScale; }

public:
	void											Set_Enable(_bool bEnable) { m_bEnable = bEnable; }
	_bool											Get_Enable() { return m_bEnable; }

public:
	HRESULT											Add(shared_ptr<class CGameObject>);
	HRESULT											Delete(shared_ptr<class CGameObject>);

	void											Iterate_Objects(function<_bool(shared_ptr<class CGameObject>)>);
	shared_ptr<class CGameObject>					Get_GameObject(_uint idx);

	_bool											Contains(shared_ptr<class CGameObject>);

public:
	using ObjectIterator = std::unordered_set<std::shared_ptr<CGameObject>>::iterator;
	inline ObjectIterator							Begin() { return m_usetObject.begin(); }
	inline ObjectIterator							End() { return m_usetObject.end(); }

private:
	_float											m_fTimeScale = { 1.f };

private:
	unordered_set<shared_ptr<class CGameObject>>	m_usetObject;

	const SCENE										m_eScene;

private:
	_bool											m_bEnable = { true };

public:
	static shared_ptr<CObjectLayer>					Create(const SCENE);
};

END
