#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSound_Manager;
END

BEGIN(Client)

class CUI_Indicator_InstKill final : public CGameObject
{
private:
	explicit CUI_Indicator_InstKill(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_InstKill() DEFAULT;

public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any _aDesc = g_aNull)	override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;
	virtual HRESULT											Render_Bloom() override;
private:
	virtual HRESULT											Ready_Components() override;

private:
	void													Initialize_UI();
	void													Tick_UI(_float fTimeDelta);
	
private:
	void													TestKey();
	void													System(_float fTimeDelta);
	void													TagBounce(_float fTimeDelta);
	void													LineScaling(_float fTimeDelta);

public:
	void													Set_InstKillRenderStart();
	void													Set_InstKillRenderEnd();
	void													Set_IndicatorPosition(_matrixf _WorldMatrix);

private:
#pragma region CTransform
	shared_ptr<CTransform>									m_pLine;
	shared_ptr<CTransform>									m_pFillter_Square;
	shared_ptr<CTransform>									m_pFillter_Rhombus;
	shared_ptr<CTransform>									m_pIndicator;
	shared_ptr<CTransform>									m_pIndicator_Bloom;
	shared_ptr<CTransform>									m_pIndicator_Scaling;
	shared_ptr<CTransform>									m_pTag;

#pragma endregion
#pragma region CTexture
	shared_ptr<CTexture>									m_pTexture_Line;
	shared_ptr<CTexture>									m_pTexture_Fillter_Square;
	shared_ptr<CTexture>									m_pTexture_Fillter_Rhombus;
	shared_ptr<CTexture>									m_pTexture_Indicator;
	shared_ptr<CTexture>									m_pTexture_Tag;

#pragma endregion
#pragma region Others
	shared_ptr<CShader>										m_pShader;
	shared_ptr<CVIBuffer_Rect>								m_pVIBuffer_Rect;
#pragma endregion
#pragma region _bool
	_bool													m_bRenderSwitch;
	_bool													m_bTagScaling;
	_bool													m_bLineScaling;
	_bool													m_bRenderSwitch_ScalingIndicator;
	_bool													m_bIndicator_Scaling;

	_bool													m_bRenderDegree;
#pragma endregion
#pragma region _int
	_int													m_iTagIndex;
	_int													m_iTagNumber;
	_int													m_iLinePhase;
#pragma endregion
#pragma region _float
	_float													m_fShader_Line;
	_float													m_fShader_Indicator;
	_float													m_fShader_Indicator_Bloom;
	_float													m_fShader_Indicator_Scaling;
	_float													m_fShader_Tag;
	_float													m_fAccTagSetScaleTime;
	_float													m_fLineScalingTime;

#pragma endregion
#pragma region _float3
	_float3													m_vScale_Line;
	_float3													m_vScale_Fillter_Square;
	_float3													m_vScale_Fillter_Rhombus;
	_float3													m_vScale_Indicator;
	_float3													m_vScale_Indicator_Scaling;
	_float3													m_vScale_Tag;
#pragma endregion
#pragma region _float4
	_float4													m_vPosition_Line;
	_float4													m_vPosition_Fillter_Square;
	_float4													m_vPosition_Fillter_Rhombus;
	_float4													m_vPosition_Indicator;
	_float4													m_vPosition_Tag;
#pragma endregion

public:
	static shared_ptr<CUI_Indicator_InstKill>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

};

END