#pragma once
#include "Engine_Define.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"

BEGIN(Engine)

class CObject_Manager final : public CSingleton<CObject_Manager>
{
private:
	explicit CObject_Manager() DEFAULT;
	virtual ~CObject_Manager() DEFAULT;

public:
	HRESULT															Reserve_Manager(const SCENE);

	void															Tick(_float fTimeDelta);
	void															Late_Tick(_float fTimeDelta);

public:
	shared_ptr<CObjectLayer>										Find_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<CObjectPool>											Find_Pool(const SCENE, const wstring& wstrPoolTag);

public:
	HRESULT															Set_TimeScale(const SCENE, const wstring& wstrLayerTag, _float fTimeScale);
	_float															Get_TimeScale(const SCENE, const wstring& wstrLayerTag);

public:
	HRESULT															Add_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<class CGameObject> pPrototype);

	shared_ptr<CObjectLayer>										Add_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<CObjectPool>											Add_Pool(const SCENE, const wstring& wstrPoolTag, const wstring& wstrPrototypeTag, _uint iPoolSize, any = g_aNull);
	shared_ptr<CObjectPool>											Add_Pool(const SCENE, const wstring& wstrPoolTag, shared_ptr<CGameObject> pPrototype, _uint iPoolSize, any = g_aNull);

	shared_ptr<CGameObject>											Clone_GameObject(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);

	HRESULT															Clear_Scene_Object(const SCENE);

	void															Iterate_Layers(const SCENE, function<_bool(pair<wstring, shared_ptr<CObjectLayer>>)>);
	void															Iterate_Pools(const SCENE, function<_bool(pair<wstring, shared_ptr<CObjectPool>>)>);

public:
	void															Loading(_bool bLoading, SCENE _eScene) { m_bOnLoading = bLoading; m_eLoadingScene = _eScene; }

	//void															Execute_Lag(_float fLagTime);
private:
	shared_ptr<CGameObject>											Find_Prototype(const SCENE, const wstring& wstrPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CGameObject>>			Prototypes;
	typedef unordered_map<wstring, shared_ptr<CObjectLayer>>		Layers;

	typedef unordered_map<wstring, shared_ptr<CObjectPool>>			Pools;

	unique_ptr<Prototypes[]>										m_arrPrototypes;
	unique_ptr<Layers[]>											m_arrLayers;
	unique_ptr<Pools[]>												m_arrPools;

	SCENE															m_eSceneMax	= static_cast<SCENE>(0);

private:
	_bool															m_bOnLoading = { false };
	SCENE															m_eLoadingScene;

private:
	_float m_fMaxLagCoolTime = 0.8f;
	_float m_fLagCoolTime = m_fMaxLagCoolTime;
	_float m_fLagTime = 0.1f;
	_float m_fEffectTimeScale = 1.f;
	_bool m_bHitLag = false;

public:
	void Set_HitLag(_bool b) { m_bHitLag = b; }
	_bool Get_HitLag() { return m_bHitLag; }
	void Set_LagCoolTime(_float f) { m_fMaxLagCoolTime = f; }
	void Set_LagTime(_float f) { m_fLagTime = f; }
	_float Get_Effect_TimeScale() { return m_fEffectTimeScale; }
	void Set_Effect_TimeScale(_float f) { m_fEffectTimeScale = f; }

	friend CSingleton<CObject_Manager>;
};

END
