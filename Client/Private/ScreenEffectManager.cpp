#include "ClientPCH.h"
#include "ScreenEffectManager.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "HPLossEffect.h"
#include "ScreenRippleEffect.h"
#include "RGBShiftEffect.h"
#include "ColorReverseEffect.h"
#include "SunnyFliterEffect.h"
#include "PurpleFilterEffect.h"

HRESULT CScreenEffectManager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	m_pDevice = _pDevice;
	m_pContext = _pContext;

	auto pHPLossEffect = CHPLossEffect::Create(_pDevice, _pContext);
	pHPLossEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::DAMAGE_OVERAY, pHPLossEffect);

	CScreenRippleEffect::RIPPLEDESC tRippleDesc;
	tRippleDesc.fRippleSpeed = 10.f;
	tRippleDesc.fRippleStrength = 0.01f;
	auto pScreenRippleEffect = CScreenRippleEffect::Create(_pDevice, _pContext, tRippleDesc);
	pScreenRippleEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::RIPPLE, pScreenRippleEffect);

	tRippleDesc.fRippleSpeed = 30.f;
	tRippleDesc.fRippleStrength = 0.005f;
	auto pScreenRippleTinyEffect = CScreenRippleEffect::Create(_pDevice, _pContext, tRippleDesc);
	pScreenRippleTinyEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::RIPPLE_TINY, pScreenRippleTinyEffect);

	auto pRGBShiftEffect = CRGBShiftEffect::Create(_pDevice, _pContext);
	pRGBShiftEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::RGB_SHIFT, pRGBShiftEffect);

	auto pColorReverseEffect = CColorReverseEffect::Create(_pDevice, _pContext);
	pColorReverseEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::COLOR_REVERSE, pColorReverseEffect);

	auto pSunnyFilterEffect = CSunnyFliterEffect::Create(_pDevice, _pContext);
	pSunnyFilterEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::SUNNY_FILTER, pSunnyFilterEffect);

	auto pPurpleFilterEffect = CPurpleFilterEffect::Create(_pDevice, _pContext);
	pPurpleFilterEffect->Set_Enable(false);
	m_mapScreenEffect.emplace(SCREEN_EFFECT::PURPLE_FILTER, pPurpleFilterEffect);

	return S_OK;
}

void CScreenEffectManager::Tick(_float fTimeDelta)
{
	for (auto pScreenEffect : m_mapScreenEffect)
	{
		if (pScreenEffect.second->Get_Enable())
		{
			pScreenEffect.second->Tick(fTimeDelta);
		}
	}
}

void CScreenEffectManager::Late_Tick(_float fTimeDelta)
{
	for (auto pScreenEffect : m_mapScreenEffect)
	{
		if (pScreenEffect.second->Get_Enable())
		{
			pScreenEffect.second->Late_Tick(fTimeDelta);
		}
	}
}

void CScreenEffectManager::Add_ScreenEffect(SCREEN_EFFECT eEffectType, shared_ptr<CGameObject> pScreenEffect)
{
	m_mapScreenEffect.emplace(eEffectType, pScreenEffect);
}

void CScreenEffectManager::Execute_ScreenEffect(SCREEN_EFFECT eEffectType)
{
	m_mapScreenEffect[eEffectType]->Set_Enable(true);
}

void CScreenEffectManager::Stop_ScreenEffect(SCREEN_EFFECT eEffectType)
{
	m_mapScreenEffect[eEffectType]->Set_Enable(false);
}
