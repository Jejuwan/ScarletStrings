#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CInteractiveObjectFinder final : public CSingleton<CInteractiveObjectFinder>
{
private:
	explicit CInteractiveObjectFinder() DEFAULT;
	virtual ~CInteractiveObjectFinder() DEFAULT;

public:
	void		Initialize(_uint _iWinSizeX, _uint _iWinSizeY);
	
public:
	std::shared_ptr<class CInteractiveObject>	Get_FocusedObject();
	std::shared_ptr<class CInteractiveObject>	Get_FocusedSpecialObject();

	void										Set_SpecialOnly(_bool b) { m_bSpecialOnly = b; }
	
public:
	HRESULT										Append_InteractiveObject(const SCENE _eScene, const wstring& _wstrLayerTag);
	void										Add_InteractiveObject(const std::shared_ptr<class CInteractiveObject>&);
	HRESULT										Append_InteractiveSpecialObject(const SCENE _eScene, const wstring& _wstrLayerTag);
	void										Add_InteractiveSpecialObject(const std::shared_ptr<class CInteractiveObject>&);
	
public:
	void										Tick();
	void										Clear();
	
private:
	std::shared_ptr<class CInteractiveObject> m_pFocusedObject;
	std::shared_ptr<class CInteractiveObject> m_FocusedSpecialObject;
	std::vector<std::shared_ptr<class CInteractiveObject>> m_vecInteractiveObject;
	std::vector<std::shared_ptr<class CInteractiveObject>> m_vecSpecialInteractiveObject;

	vector<shared_ptr<class CInteractiveObject>> m_vecCapturedObjects;

private:
	_bool m_bSpecialOnly = false;
	
	_uint m_iWinSizeX = { 0 };
	_uint m_iWinSizeY = { 0 };

	_float2 m_vScreenCenterPosition = {};

private:
	_float m_fFocusingDistanceThreshold = { 30.f };
	_float m_fSpecialFocusingDistanceThreshold = { 30.f };
	_float m_fScreenSpecialFocusingDistanceThreshold = { 200.f };
private:
	void Tick_Objects();
	void Tick_SpecialObjects();

public:
	friend CSingleton<CInteractiveObjectFinder>;
	void EraseObject(shared_ptr<CInteractiveObject> _obj);
};

END