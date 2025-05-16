#pragma once

#include "Client_Define.h"

BEGIN(Client)

enum class SCREEN_EFFECT
{
	DAMAGE_OVERAY,
	RIPPLE,
	RIPPLE_TINY,
	RGB_SHIFT,
	COLOR_REVERSE,
	SUNNY_FILTER,
	PURPLE_FILTER,
};

class CScreenEffectManager final : public CSingleton<CScreenEffectManager>
{
private:
	explicit CScreenEffectManager() DEFAULT;
	virtual ~CScreenEffectManager() DEFAULT;

public:
	HRESULT														Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	void														Tick(_float fTimeDelta);
	void														Late_Tick(_float fTimeDelta);

public:
	void														Add_ScreenEffect(SCREEN_EFFECT eEffectType, shared_ptr<CGameObject> pScreenEffect);
	void														Execute_ScreenEffect(SCREEN_EFFECT eEffectType);
	void														Stop_ScreenEffect(SCREEN_EFFECT eEffectType);

private:
	unordered_map<SCREEN_EFFECT, shared_ptr<CGameObject>>		m_mapScreenEffect;

private:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;

	friend CSingleton<CScreenEffectManager>;
};

END