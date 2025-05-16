#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Client)

class CEffectInstance_ParticlePointDynamicCurve final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticlePointDynamicCurve(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticlePointDynamicCurve(const CEffectInstance_ParticlePointDynamicCurve&);
	virtual ~CEffectInstance_ParticlePointDynamicCurve() DEFAULT;

public:
	virtual HRESULT													Initialize_Prototype() override;
	virtual HRESULT													Initialize(any = g_aNull) override;
	virtual void													Tick(_float fTimeDelta) override;
	virtual void													Late_Tick(_float fTimeDelta) override;
	virtual HRESULT													Render() override;

	virtual HRESULT													Render_Bloom() override;

	virtual HRESULT													Fetch(any vec_mat_transform_pair_object_bone = g_aNull) override;
	virtual _bool													Return() override;
	virtual HRESULT													Release() override;

private:
	HRESULT															Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void															Set_Data(const VFX_ParticlePointDynamicCurve);
	const VFX_ParticlePointDynamicCurve								Get_Data() const { return m_tData; }

	shared_ptr<class CCurveContainer>								Get_InstLifeTimeCurve() { return m_pCCInstLifeTime; }
	shared_ptr<class CCurveContainer>								Get_StartCurve() { return m_pCCStart; }
	shared_ptr<class CCurveContainer>								Get_Speed() { return m_pCCSpeed; }
	shared_ptr<class CCurveContainer>								Get_GravityScaleCurve() { return m_pCCGravityScale; }
	shared_ptr<class CCurveContainer>								Get_ScaleCurve() { return m_pCCScale; }
	shared_ptr<class CCurveContainer>								Get_BloomCurve() { return m_pCCBloom; }
	shared_ptr<class CCurveContainer>								Get_FadeCurve() { return m_pCCFade; }
	shared_ptr<class CCurveContainer>								Get_SpriteCurve() { return m_pCCSprite; }
	shared_ptr<class CCurveContainer>								Get_RotationCurve() { return m_pCCRotation; }

	void															Set_InstLifeTimeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCInstLifeTime = pCC; }
	void															Set_StartCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCStart = pCC; }
	void															Set_Speed(shared_ptr<class CCurveContainer> pCC) { m_pCCSpeed = pCC; }
	void															Set_GravityScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCGravityScale = pCC; }
	void															Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCScale = pCC; }
	void															Set_BloomCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCBloom = pCC; }
	void															Set_FadeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCFade = pCC; }
	void															Set_SpriteCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCSprite = pCC; }
	void															Set_RotationCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCRotation= pCC; }
#endif

private:
	virtual void													Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void													Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	VFX_ParticlePointDynamicCurve									m_tData{};

	vector<_bool>													m_vecInstFetch;
	vector<_float>													m_vecInstTimeAcc;
	vector<_float>													m_vecInstLifeTime;
	vector<_float2>													m_vecInstRotation;
	vector<_float3>													m_vecInstVelocity;
	vector<_float4>													m_vecInstInitialPosition;

	shared_ptr<class CCurveContainer>								m_pCCInstLifeTime;
	shared_ptr<class CCurveContainer>								m_pCCStart;
	shared_ptr<class CCurveContainer>								m_pCCSpeed;
	shared_ptr<class CCurveContainer>								m_pCCGravityScale;
	shared_ptr<class CCurveContainer>								m_pCCScale;
	shared_ptr<class CCurveContainer>								m_pCCBloom;
	shared_ptr<class CCurveContainer>								m_pCCFade;
	shared_ptr<class CCurveContainer>								m_pCCSprite;
	shared_ptr<class CCurveContainer>								m_pCCRotation;

	shared_ptr<CTransform>											m_pTarget;
	_float4x4														m_mPivot		= g_mUnit;
	const _float4x4*												m_pBone			= nullptr;

	_float4x4														m_mFetch;
	_float3															m_vPrevPos;

	_bool															m_bRandomSprite	= false;
	_bool															m_bLinearStart	= false;
	_bool															m_bSpeedBase	= false;
	_bool															m_bRotatePivot	= false;

public:
	static shared_ptr<CEffectInstance_ParticlePointDynamicCurve>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>									Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticlePointDynamicCurve>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT													Export(const wstring& wstrFilePath) override;
#endif
};

END
