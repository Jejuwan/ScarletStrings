#pragma once
#include "Client_Define.h"
#include "EffectRegular.h"

BEGIN(Client)

class CEffectRegular_VisualDetailCurve final : public CEffectRegular
{
private:
	explicit CEffectRegular_VisualDetailCurve(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectRegular_VisualDetailCurve(const CEffectRegular_VisualDetailCurve&);
	virtual ~CEffectRegular_VisualDetailCurve() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any = g_aNull) override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

	virtual HRESULT										Render_Bloom() override;

	virtual HRESULT										Fetch(any pTransform = g_aNull) override;
	virtual _bool										Return() override;
	virtual HRESULT										Release() override;

private:
	HRESULT												Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void												Set_Data(const VFX_VisualDetailCurve);
	const VFX_VisualDetailCurve							Get_Data() const { return m_tData; }

	shared_ptr<class CCurveContainer>					Get_ScaleCurve() { return m_pCCScale; }
	shared_ptr<class CCurveContainer>					Get_BloomCurve() { return m_pCCBloom; }
	shared_ptr<class CCurveContainer>					Get_FadeCurve() { return m_pCCFade; }
	shared_ptr<class CCurveContainer>					Get_SpriteCurve() { return m_pCCSprite; }
	shared_ptr<class CCurveContainer>					Get_DistortionCurve() { return m_pCCDistortion; }
	shared_ptr<class CCurveContainer>					Get_DiffuseCurve() { return m_pCCDiffuse; }
	shared_ptr<class CCurveContainer>					Get_EmissiveCurve() { return m_pCCEmissive; }
	shared_ptr<class CCurveContainer>					Get_TranslationCurve() { return m_pCCTranslation; }
	shared_ptr<class CCurveContainer>					Get_RotationCurve() { return m_pCCRotation; }

	void												Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCScale = pCC; }
	void												Set_BloomCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCBloom = pCC; }
	void												Set_FadeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCFade = pCC; }
	void												Set_SpriteCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCSprite = pCC; }
	void												Set_DistortionCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCDistortion = pCC; }
	void												Set_DiffuseCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCDiffuse = pCC; }
	void												Set_EmissiveCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCEmissive= pCC; }
	void												Set_TranslationCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCTranslation= pCC; }
	void												Set_RotationCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCRotation = pCC; }
#endif

private:
	VFX_VisualDetailCurve								m_tData{};

	shared_ptr<class CCurveContainer>					m_pCCScale;
	shared_ptr<class CCurveContainer>					m_pCCBloom;
	shared_ptr<class CCurveContainer>					m_pCCFade;
	shared_ptr<class CCurveContainer>					m_pCCSprite;
	shared_ptr<class CCurveContainer>					m_pCCDistortion;
	shared_ptr<class CCurveContainer>					m_pCCDiffuse;
	shared_ptr<class CCurveContainer>					m_pCCEmissive;
	shared_ptr<class CCurveContainer>					m_pCCTranslation;
	shared_ptr<class CCurveContainer>					m_pCCRotation;

	_float3												m_vBaseScale = _float3(1.f, 1.f, 1.f);
	_float3												m_vBasePosition = _float3(0.f, 0.f, 0.f);

	shared_ptr<CTransform>								m_pTarget;
	_float4x4											m_mPivot	= g_mUnit;
	const _float4x4*									m_pBone		= nullptr;
	_float4x4											m_mBone		= g_mUnit;
	_float3												m_vOffset	= _float3();
	_float4												m_vQuat		= XMQuaternionIdentity();

public:
	static shared_ptr<CEffectRegular_VisualDetailCurve>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

	static shared_ptr<CEffectRegular_VisualDetailCurve>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT										Export(const wstring& wstrFilePath) override;
#endif
};

END
