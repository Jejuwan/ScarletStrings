#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_HP final : public CGameObject
{
private:
	explicit CUI_HP(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_HP() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	void									Initialize_UI();
	void									Tick_UI(_float fTimeDelta);
	void									Tick_SpriteTexture(_float fTimeDelta);

private:
	void									UIBounce_System(_float fTimeDelta);
	void									Calculate_Damage_System(_float fTimeDelta);
	void									Shake_HPBar_System(_float fTimeDetlta);
	void									Set_HpRenderFont();

public:
	void									Set_DamageToPlayer();


private:
	shared_ptr<CTransform>					m_pBlankHP;
	shared_ptr<CTransform>					m_pHP1;

	shared_ptr<CTransform>					m_pSlash;
	shared_ptr<CTransform>					m_pOnes;
	shared_ptr<CTransform>					m_pTens;
	shared_ptr<CTransform>					m_pHundreds;
	shared_ptr<CTransform>					m_pThousands;
	shared_ptr<CTransform>					m_pOnesMax;
	shared_ptr<CTransform>					m_pTensMax;
	shared_ptr<CTransform>					m_pHundredsMax;
	shared_ptr<CTransform>					m_pThousandsMax;

	shared_ptr<CTexture>					m_pTexture_BlankHP;
	shared_ptr<CTexture>					m_pTexture_HP1;
	shared_ptr<CTexture>					m_pTexture_HP80;
	shared_ptr<CTexture>					m_pTexture_HP60;
	shared_ptr<CTexture>					m_pTexture_HP40;

	shared_ptr<CTexture>					m_pTexture_Slash;
	shared_ptr<CTexture>					m_pTexture_Zero;
	shared_ptr<CTexture>					m_pTexture_One;
	shared_ptr<CTexture>					m_pTexture_Two;
	shared_ptr<CTexture>					m_pTexture_Three;
	shared_ptr<CTexture>					m_pTexture_Four;
	shared_ptr<CTexture>					m_pTexture_Five;
	shared_ptr<CTexture>					m_pTexture_Six;
	shared_ptr<CTexture>					m_pTexture_Seven;
	shared_ptr<CTexture>					m_pTexture_Eight;
	shared_ptr<CTexture>					m_pTexture_Nine;


	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	SPRITEUIDESC							m_tUIDesc, m_tUIDesc_80, m_tUIDesc_60;

private:
	_bool									m_bDash;
	_bool									m_bIsShake;
	_bool									m_bDamageEnd;
	_bool									m_bIsFinishCalculateDamage;

private:
#pragma region Bounce
	POINT									m_ptCursorMove;
	_bool									m_bUIBounce;
	_bool									m_bLeft;
	_bool									m_bRight;
	_bool									m_bTop;
	_bool									m_bBottom;
	_float									m_fMovedPositionX_Left;
	_float									m_fMovedPositionX_Right;
	_float									m_fMovedPositionY_Top;
	_float									m_fMovedPositionY_Bottom;
	_float									m_fOriginalPosX;
	_float									m_fOriginalPosY;
#pragma endregion


private:
	_uint									m_iMonsterPow;
	_uint									m_iShakeCount;

private:
	_float									m_fAccTime;
	_float									m_fShakeTime;
	_float									m_fAccDashTime;
	_float									m_fDamageDelayTime;

	_float									m_fHpBlank;
	_float									m_fHpBlankMax;
	_float									m_fRatio_Blank;
	_float									m_fPercentage_Blank;


	_float									m_fPrevHP;
	_float									m_fDamage;

	_float									m_fHp;
	_float									m_fHpMax;
	_float									m_fRatio;
	_float									m_fPercentage;

	_float									m_fBounceDistance;
private:
	_float3									m_vScale_HP;
	_float3									m_vScale_HPBlank;

	_float3									m_vScale_Slash;

	_float3									m_vScale_HPOnes;
	_float3									m_vScale_HPTens;
	_float3									m_vScale_HPHundreds;
	_float3									m_vScale_HPThousands;

	_float3									m_vScale_HPOnesMax;
	_float3									m_vScale_HPTensMax;
	_float3									m_vScale_HPHundredsMax;
	_float3									m_vScale_HPThousandsMax;
private:
	_float4									m_vPosition_HP;
	_float4									m_vPrevPosition_HP;
	_float4									m_vPosition_HP_Shake;

	_float4									m_vPosition_HPBlank;
	_float4									m_vPrevPosition_HPBlank;
	_float4									m_vPosition_HPBlank_Shake;

	_float4									m_vPosition_Slash;

	_float4									m_vPosition_HPOnes;
	_float4									m_vPosition_HPTens;
	_float4									m_vPosition_HPHundreds;
	_float4									m_vPosition_HPThousands;

	_float4									m_vPosition_HPOnesMax;
	_float4									m_vPosition_HPTensMax;
	_float4									m_vPosition_HPHundredsMax;
	_float4									m_vPosition_HPThousandsMax;
private:
	wstring									m_strName;
	wstring									m_strSeperator;

	_int									m_iOnes;
	_int									m_iTens;
	_int									m_iHundreds;
	_int									m_iThousands;

	_int									m_iOnesMax;
	_int									m_iTensMax;
	_int									m_iHundredsMax;
	_int									m_iThousandsMax;


public:
	static shared_ptr<CUI_HP>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;

};

END