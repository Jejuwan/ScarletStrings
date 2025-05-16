#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_SkillSlot final : public CGameObject
{
private:
	explicit CUI_SkillSlot(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_SkillSlot() DEFAULT;

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
	void									UIBounce_System(_float fTimeDelta);
	void									Blink_Skill_Icon_Electric(_float fTimeDelta);
	void									Blink_Skill_Icon_Acceleration(_float fTimeDelta);
	void									Blink_Skill_Icon_Clone(_float fTimeDelta);
	void									Blink_Skill_Icon_Stealth(_float fTimeDelta);


public:
	void									Activate_Skill_Electric();
	void									Activate_Skill_Acceleration();
	void									Activate_Skill_Clone();
	void									Activate_Skill_Stealth();
	void									SkillStop_Electic();
	void									SkillStop_Acceleration();
	void									SkillStop_Replication();
	void									SkillStop_Stealth();

private:
	shared_ptr<CTransform>					m_pSkillSlot;
	shared_ptr<CTransform>					m_pSkill_ElectricOff;
	shared_ptr<CTransform>					m_pSkill_ElectricOn;
	shared_ptr<CTransform>					m_pSkill_AccelerationOff;
	shared_ptr<CTransform>					m_pSkill_AccelerationOn;
	shared_ptr<CTransform>					m_pSkill_CloneOff;
	shared_ptr<CTransform>					m_pSkill_CloneOn;
	shared_ptr<CTransform>					m_pSkill_StealthOff;
	shared_ptr<CTransform>					m_pSkill_StealthOn;
	shared_ptr<CTransform>					m_pSkill_Cool_Electric;
	shared_ptr<CTransform>					m_pSkill_Cool_Acceleration;
	shared_ptr<CTransform>					m_pSkill_Cool_Clone;
	shared_ptr<CTransform>					m_pSkill_Cool_Stealth;


	shared_ptr<CTexture>					m_pTexture;
	shared_ptr<CTexture>					m_pTexture_ElectricOff;
	shared_ptr<CTexture>					m_pTexture_ElectricOn;
	shared_ptr<CTexture>					m_pTexture_AccelerationOff;
	shared_ptr<CTexture>					m_pTexture_AccelerationOn;
	shared_ptr<CTexture>					m_pTexture_CloneOff;
	shared_ptr<CTexture>					m_pTexture_CloneOn;
	shared_ptr<CTexture>					m_pTexture_StealthOff;
	shared_ptr<CTexture>					m_pTexture_StealthOn;
	shared_ptr<CTexture>					m_pTexture_SkillCool_Electric;
	shared_ptr<CTexture>					m_pTexture_SkillCool_Acceleration;
	shared_ptr<CTexture>					m_pTexture_SkillCool_Clone;
	shared_ptr<CTexture>					m_pTexture_SkillCool_Stealth;

	
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

#pragma region Bounce
	POINT									m_ptCursorMove;
	_bool									m_bUIBounce;
	_bool									m_bLeft;
	_bool									m_bRight;
	_bool									m_bTop;
	_bool									m_bBottom;

	// Electric Icon
	_float									m_fMovedPosition_ElectricX_Left;
	_float									m_fMovedPosition_ElectricX_Right;
	_float									m_fMovedPosition_ElectricY_Top;
	_float									m_fMovedPosition_ElectricY_Bottom;
	_float									m_fOriginalPos_ElectricX;
	_float									m_fOriginalPos_ElectricY;

	// Acceleration Icon
	_float									m_fMovedPosition_AccelerationX_Left;
	_float									m_fMovedPosition_AccelerationX_Right;
	_float									m_fMovedPosition_AccelerationY_Top;
	_float									m_fMovedPosition_AccelerationY_Bottom;
	_float									m_fOriginalPos_AccelerationX;
	_float									m_fOriginalPos_AccelerationY;

	// Clone Icon
	_float									m_fMovedPosition_CloneX_Left;
	_float									m_fMovedPosition_CloneX_Right;
	_float									m_fMovedPosition_CloneY_Top;
	_float									m_fMovedPosition_CloneY_Bottom;
	_float									m_fOriginalPos_CloneX;
	_float									m_fOriginalPos_CloneY;

	//Stealth Icon
	_float									m_fMovedPosition_StealthX_Left;
	_float									m_fMovedPosition_StealthX_Right;
	_float									m_fMovedPosition_StealthY_Top;
	_float									m_fMovedPosition_StealthY_Bottom;
	_float									m_fOriginalPos_StealthX;
	_float									m_fOriginalPos_StealthY;

	// Electric SkillCool
	_float									m_fMovedPosition_ElectricCoolX_Left;
	_float									m_fMovedPosition_ElectricCoolX_Right;
	_float									m_fMovedPosition_ElectricCoolY_Top;
	_float									m_fMovedPosition_ElectricCoolY_Bottom;
	_float									m_fOriginalPos_ElectricCoolX;
	_float									m_fOriginalPos_ElectricCoolY;

	// Acceleration SkillCool
	_float									m_fMovedPosition_AccelerationCoolX_Left;
	_float									m_fMovedPosition_AccelerationCoolX_Right;
	_float									m_fMovedPosition_AccelerationCoolY_Top;
	_float									m_fMovedPosition_AccelerationCoolY_Bottom;
	_float									m_fOriginalPos_AccelerationCoolX;
	_float									m_fOriginalPos_AccelerationCoolY;

	// Clone SkillCool
	_float									m_fMovedPosition_CloneCoolX_Left;
	_float									m_fMovedPosition_CloneCoolX_Right;
	_float									m_fMovedPosition_CloneCoolY_Top;
	_float									m_fMovedPosition_CloneCoolY_Bottom;
	_float									m_fOriginalPos_CloneCoolX;
	_float									m_fOriginalPos_CloneCoolY;

	// Stealth SkillCool
	_float									m_fMovedPosition_StealthCoolX_Left;
	_float									m_fMovedPosition_StealthCoolX_Right;
	_float									m_fMovedPosition_StealthCoolY_Top;
	_float									m_fMovedPosition_StealthCoolY_Bottom;
	_float									m_fOriginalPos_StealthCoolX;
	_float									m_fOriginalPos_StealthCoolY;

#pragma endregion

private:
	_bool									m_bCanActivate_Electric;
	_bool									m_bCanActivate_Acceleration;
	_bool									m_bCanActivate_Clone;
	_bool									m_bCanActivate_Stealth;

	_bool									m_bCanChargeGauge_Electric;
	_bool									m_bCanChargeGauge_Acceleration;
	_bool									m_bCanChargeGauge_Clone;
	_bool									m_bCanChargeGauge_Stealth;

	_bool									m_bRenderIcon_Electric;
	_bool									m_bRenderIcon_Acceleration;
	_bool									m_bRenderIcon_Replication;
	_bool									m_bRenderIcon_Stealth;


private:
	_float									m_fAccTime;
	_float									m_fBlink_Electric;
	_float									m_fBlink_Acceleration;
	_float									m_fBlink_Clone;
	_float									m_fBlink_Stealth;
	_float									m_fBlinkIndex;
	_float									m_fBlinkIndex_Electric;
	_float									m_fBlinkIndex_Acceleration;
	_float									m_fBlinkIndex_Clone;
	_float									m_fBlinkIndex_Stealth;
	
	_float									m_fBlinkTime_Electric;
	_float									m_fBlinkTime_Acceleration;
	_float									m_fBlinkTime_Clone;
	_float									m_fBlinkTime_Stealth;

	_float									m_fRatio_Electric;
	_float									m_fRatio_Acceleration;
	_float									m_fRatio_Clone;
	_float									m_fRatio_Stealth;

	_float									m_fGauge_Electric;
	_float									m_fGauge_Acceleration;
	_float									m_fGauge_Clone;
	_float									m_fGauge_Stealth;

	_float									m_fGaugeMax_Electric;
	_float									m_fGaugeMax_Acceleration;
	_float									m_fGaugeMax_Clone;
	_float									m_fGaugeMax_Stealth;


private:
	_float3									m_vScale_Electric_Off;
	_float3									m_vScale_Acceleration_Off;
	_float3									m_vScale_Clone_Off;
	_float3									m_vScale_Stealth_Off;
	_float3									m_vScale_Electric_On;
	_float3									m_vScale_Acceleration_On;
	_float3									m_vScale_Clone_On;
	_float3									m_vScale_Stealth_On;
	_float3									m_vScale_SkillCool_Electric;
	_float3									m_vScale_SkillCool_Acceleration;
	_float3									m_vScale_SkillCool_Clone;
	_float3									m_vScale_SkillCool_Stealth;


	_float4									m_vPosition_Electric_Off;
	_float4									m_vPosition_Acceleration_Off;
	_float4									m_vPosition_Clone_Off;
	_float4									m_vPosition_Stealth_Off;
	_float4									m_vPosition_Electric_On;
	_float4									m_vPosition_Acceleration_On;
	_float4									m_vPosition_Clone_On;
	_float4									m_vPosition_Stealth_On;
	_float4									m_vPosition_SkillCool_Electric;
	_float4									m_vPosition_SkillCool_Acceleration;
	_float4									m_vPosition_SkillCool_Clone;
	_float4									m_vPosition_SkillCool_Stealth;

	_float4									m_vPrevPosition_Electric_Icon;
	_float4									m_vPrevPosition_Acceleration_Icon;
	_float4									m_vPrevPosition_Clone_Icon;
	_float4									m_vPrevPosition_Stealth_Icon;

	_float4									m_vPrevPosition_Electric_SkillCool;
	_float4									m_vPrevPosition_Acceleration_SkillCool;
	_float4									m_vPrevPosition_Clone_SkillCool;
	_float4									m_vPrevPosition_Stealth_SkillCool;

public:
	static shared_ptr<CUI_SkillSlot>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END