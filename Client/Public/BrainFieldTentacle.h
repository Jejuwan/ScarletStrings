#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CBrainFieldTentacle : public CGameObject
{
public:
	enum class ANIMATION
	{
		BRAIN_FIELD_OPEN_C01_1,
		BRAIN_FIELD_OPEN_C01_2,
		BRAIN_FIELD_OPEN_C01_3,
		BRAIN_FIELD_OPEN_C01_CH0200_1,
		BRAIN_FIELD_OPEN_C01_CH0200_2,
		BRAIN_FIELD_OPEN_C01_CH0200_3,
		BRAIN_FIELD_OPEN_C02_1,
		BRAIN_FIELD_OPEN_C02_2,
		BRAIN_FIELD_OPEN_C02_3,
		BRAIN_FIELD_OPEN_C02_CH0200_1,
		BRAIN_FIELD_OPEN_C02_CH0200_2,
		BRAIN_FIELD_OPEN_C02_CH0200_3,
		CURE_START01,
		START01,
		START02,
		START03,
		START04,
		START05,
		SIDE_SHAKE,
		RT_LOOP,
		ENPC_BC_FIN_C04_1,
		ENPC_BC_FIN_C04_2,
		ENPC_BC_FIN_C04_3,
		ENPC_BC_FIN_C06_1,
		ENPC_BC_FIN_C06_2,
		ENPC_BC_FIN_C06_3,
		F05S00470C20_0,
		F05S00470C20_1,
		F05S00470C20_2,
		F05S00470C21_0,
		F05S00470C21_1,
		F05S00470C21_2,
		F05S00470C22_0,
		F05S00470C22_1,
		F05S00470C22_2,
		M04S00470C20_1,
		M04S00470C20_2,
		M04S00470C20_3,
		M04S00470C21_1,
		M04S00470C21_2,
		M04S00470C21_3,
		M04S00470C22_1,
		M04S00470C22_2,
		M04S00470C22_3,
	};
private:
	explicit CBrainFieldTentacle(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBrainFieldTentacle(const CBrainFieldTentacle&);
	virtual ~CBrainFieldTentacle() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	virtual HRESULT							Render_Neon() override;
	virtual HRESULT							Render_Screen_Effect() override;

public:
	void									Dissolve_Stripe();
	void									Set_Pivot(const _float4x4& matPivot);

public:
	void									EmissiveMode();

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CModel>						m_pModel;

	_uint									m_iPassIndex = { 22 };

private:
	shared_ptr<CTexture>					m_pNoiseTexture;

	_bool									m_bDissolve = { false };
	_float									m_fDissolveFactor = { 0.f };

	_bool									m_bEmissiveMode = { false };

private:
	const _float4x4*						m_pCombineBone;
	_float4x4								m_matPivot;

private:
	_float3									m_vStripeColor = float3(2.f, 0.2f, 0.2f);

private:
	_float3									m_vRootColor = { 1.f, 0.79f, 0.09f };
	_float3									m_vTipColor = { 2.5f, 0.f, 0.f };

	_float									m_fZFactor = { 0.8f };

public:
	static shared_ptr<CBrainFieldTentacle>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any eScene = g_aNull) override;
};

END