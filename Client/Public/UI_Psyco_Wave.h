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

class CUI_Psyco_Wave final : public CGameObject
{
private:
	explicit CUI_Psyco_Wave(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Psyco_Wave() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

private:
	void									Initialize_UI();
	void									Tick_UI(_float fTimeDelta);
	void									UIBounce_System(_float fTimeDelta);
	void									Tick_SpriteTexture(_float fTimeDelta);
	void									Consume_System(_float fTimeDelta);

public:
	void									Set_Psyco_Wave();
	void									Set_Psyco_Disonance();

private:
#pragma region Transform
	shared_ptr<CTransform>					m_pWave;
	shared_ptr<CTransform>					m_pWave_Background;
	shared_ptr<CTransform>					m_pDisonance;
	shared_ptr<CTransform>					m_pDisonance_Background;
#pragma endregion
#pragma region Texture
	shared_ptr<CTexture>					m_pTexture_Wave;
	shared_ptr<CTexture>					m_pTexture_Disonance;
#pragma endregion
#pragma region Others
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;
#pragma endregion

private:
#pragma region Sprite
	SPRITEUIDESC							m_tUIDesc_Wave;
	SPRITEUIDESC							m_tUIDesc_Disonance;
#pragma endregion
#pragma region _bool
	_bool									m_bIsBattle;
	_bool									m_bIsWave;
	_bool									m_bIsWave_Background;
	_bool									m_bIsDisonance;
	_bool									m_bIsDisonance_Background;
#pragma endregion
#pragma region _int

#pragma endregion
#pragma region _float
	_float									m_fAccTime;
///////////////////////////////////////////////////////////////////////////
// 고정값
	_float									m_fPsycoGaugeOrigin;
	_float									m_fPsycoGaugeMaxOrigin;
	_float									m_fPsycoGaugeRatioOrigin;
	_float									m_fPsycoGaugePercentageOrigin;
// 변하는 값
	_float									m_fPsycoGauge_Wave;
	_float									m_fPsycoGaugeMax_Wave;
	_float									m_fPsycoGaugeRatio_Wave;
	_float									m_fPsycoGaugePercentage_Wave;

	_float									m_fPsycoGauge_Background;
	_float									m_fPsycoGaugeMax_Background;
	_float									m_fPsycoGaugeRatio_Background;
	_float									m_fPsycoGaugePercentage_Background;
///////////////////////////////////////////////////////////////////////////

#pragma endregion
#pragma region _float3
	_float3									m_vScale_Wave;
	_float3									m_vScale_Disonance;
#pragma endregion
#pragma region _float4
	_float4									m_vPosition_Wave;
	_float4									m_vPrevPosition_Wave;
	_float4									m_vPosition_Disonance;
#pragma endregion
#pragma region POINT
	POINT									m_ptCursorMove;
#pragma endregion

private:
#pragma region Bounce
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

public:
	static shared_ptr<CUI_Psyco_Wave>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;

};

END