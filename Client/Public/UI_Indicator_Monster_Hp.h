#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CObjectLayer;
END

BEGIN(Client)

class CUI_Indicator_Monster_Hp final : public CGameObject
{
private:
	explicit CUI_Indicator_Monster_Hp(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_Monster_Hp() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any _aDesc = g_aNull)	override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

private:
	virtual HRESULT									Ready_Components() override;

public:
	void											Set_MonsterHp(_float fHp, _float fHpMax);
	void											Calculate_Damage_System(_float fTimeDelta);
	void											Set_IndicatorPosition(_vectorf _vWorldPosition, _matrixf _WorldMatrix);
	void											Set_MonsterNameTag(wstring _strMonsterNameTag);
	void											Set_MonsterNameTagRemove_Execution();

private:
	void											Initialize_UI();
	void											Tick_UI(_float fTimeDelta);
	void											LockOn(_float fTimeDelta);
	void											Set_DamageToMonster();

private:
	shared_ptr<CTransform>							m_pIndicator_Hp_Back;
	shared_ptr<CTransform>							m_pIndicator_Hp_Front;
	shared_ptr<CTransform>							m_pIndicator_Hp_Background;

	shared_ptr<CTexture>							m_pTexture_Indicator_Hp_Back;
	shared_ptr<CTexture>							m_pTexture_Indicator_Hp_Front;
	shared_ptr<CTexture>							m_pTexture_Indicator_Hp_Background;

	shared_ptr<CShader>								m_pShader;
	shared_ptr<CVIBuffer_Rect>						m_pVIBuffer_Rect;

	shared_ptr<CTransform>							m_pLockOn;
	shared_ptr<class CCamera_Player>				m_pCamera_Player;
	
private:
	_float3											m_vScale_Indicator_Hp_Back;
	_float3											m_vScale_Indicator_Hp_Front;
	_float3											m_vScale_Indicator_Hp_Background;

private:
	_float4											m_vPosition_Indicator_Hp_Back;
	_float4											m_vPosition_Indicator_Hp_Front;
	_float4											m_vPosition_Indicator_Hp_Background;

private:
	_bool											m_bDamageEnd;
	_bool											m_bIsFinishCalculateDamage;
	_bool											m_bRenderDegree;
	_bool											m_bExecution;

private:	// Time
	_float											m_fTime;
	_float											m_fDamageDelayTime;

private:	// For. Monster
	_float											m_fHp;
	_float											m_fHpMax;
	_float											m_fRatio;
	_float											m_fPercentage;

	_float											m_fHpBlank;
	_float											m_fHpBlankMax;
	_float											m_fRatio_Blank;
	_float											m_fPercentage_Blank;

	_float											m_fPrevHP;

private:
	_float											m_fDistanceMonsterToCamera;
	_float2											m_vPosition_RenderMonster_Name;
	wstring											m_strMonsterName;

public:
	static shared_ptr<CUI_Indicator_Monster_Hp>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;

};

END