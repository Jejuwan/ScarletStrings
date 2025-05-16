#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Client)

class CEffectInstance_ParticlePointVectorFieldCurve final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticlePointVectorFieldCurve(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticlePointVectorFieldCurve(const CEffectInstance_ParticlePointVectorFieldCurve&);
	virtual ~CEffectInstance_ParticlePointVectorFieldCurve() DEFAULT;

public:
	virtual HRESULT														Initialize_Prototype() override;
	virtual HRESULT														Initialize(any = g_aNull) override;
	virtual void														Tick(_float fTimeDelta) override;
	virtual void														Late_Tick(_float fTimeDelta) override;
	virtual HRESULT														Render() override;

	virtual HRESULT														Render_Bloom() override;

	virtual HRESULT														Fetch(any vec_mat_transform_pair_object_bone = g_aNull) override;
	virtual _bool														Return() override;
	virtual HRESULT														Release() override;

private:
	HRESULT																Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void																Set_Data(const VFX_ParticlePointVectorFieldCurve);
	const VFX_ParticlePointVectorFieldCurve								Get_Data() const												{ return m_tData; }

	shared_ptr<class CCurveContainer>									Get_InstLifeTimeCurve()											{ return m_pCCInstLifeTime; }
	shared_ptr<class CCurveContainer>									Get_ScaleCurve()												{ return m_pCCScale; }
	shared_ptr<class CCurveContainer>									Get_BloomCurve()													{ return m_pCCBloom; }
	shared_ptr<class CCurveContainer>									Get_FadeCurve()													{ return m_pCCFade; }
	shared_ptr<class CCurveContainer>									Get_VelocityXCurve()											{ return m_pCCVelocityX; }
	shared_ptr<class CCurveContainer>									Get_VelocityYCurve()											{ return m_pCCVelocityY; }
	shared_ptr<class CCurveContainer>									Get_VelocityZCurve()											{ return m_pCCVelocityZ; }
	shared_ptr<class CCurveContainer>									Get_RotationCurve()												{ return m_pCCRotation; }

	void																Set_InstLifeTimeCurve(shared_ptr<class CCurveContainer> pCC)	{ m_pCCInstLifeTime = pCC; }
	void																Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCScale = pCC; }
	void																Set_BloomCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCBloom = pCC; }
	void																Set_FadeCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCFade = pCC; }
	void																Set_VectorXCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCVelocityX = pCC; }
	void																Set_VectorYCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCVelocityY = pCC; }
	void																Set_VectorZCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCVelocityZ = pCC; }
	void																Set_RotationCurve(shared_ptr<class CCurveContainer> pCC)		{ m_pCCRotation = pCC; }
#endif

private:
	virtual void														Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void														Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	void																Initiate_Instance(VTXPOSSIZEINSTTRANSCOLORARG*, _uint iInstanceIndex, _float fTimeDelta = 0.f);
	void																Tick_Instance(VTXPOSSIZEINSTTRANSCOLORARG*, _uint iInstanceIndex, _float fTimeDelta = 0.f);
	void																Exit_Instance(VTXPOSSIZEINSTTRANSCOLORARG*, _float fTimeDelta = 0.f);
	void																Terminate_Instance(VTXPOSSIZEINSTTRANSCOLORARG*, _uint iInstanceIndex, _float fTimeDelta = 0.f);

private:
	VFX_ParticlePointVectorFieldCurve									m_tData{};

	vector<_float>														m_vecInstTimeAcc;
	vector<_float>														m_vecInstLifeTime;
	vector<_float2>														m_vecInstRotation;
	vector<_float3>														m_vecInstVelocity;
	
	vector<_float3>														m_vecInstNoise;

	vector<_int>														m_vecInstTerminateIndex;

	shared_ptr<class CCurveContainer>									m_pCCInstLifeTime;
	shared_ptr<class CCurveContainer>									m_pCCScale;
	shared_ptr<class CCurveContainer>									m_pCCBloom;
	shared_ptr<class CCurveContainer>									m_pCCFade;
	shared_ptr<class CCurveContainer>									m_pCCVelocityX;
	shared_ptr<class CCurveContainer>									m_pCCVelocityY;
	shared_ptr<class CCurveContainer>									m_pCCVelocityZ;
	shared_ptr<class CCurveContainer>									m_pCCRotation;

	_int																m_iMaxTerminateInstance	= 0;
	_bool																m_bTerminate			= false;
	_bool																m_bSpeedBase			= false;

	shared_ptr<CTransform>												m_pTarget;
	_float4x4															m_mPivot				= g_mUnit;
	const _float4x4*													m_pBone					= nullptr;

public:
	static shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>										Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticlePointVectorFieldCurve>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT														Export(const wstring& wstrFilePath) override;
#endif
};

END
