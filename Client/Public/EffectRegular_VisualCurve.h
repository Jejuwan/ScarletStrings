#pragma once
#include "Client_Define.h"
#include "EffectRegular.h"

BEGIN(Client)

class CEffectRegular_VisualCurve final : public CEffectRegular
{
private:
	explicit CEffectRegular_VisualCurve(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectRegular_VisualCurve(const CEffectRegular_VisualCurve&);
	virtual ~CEffectRegular_VisualCurve() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

	virtual HRESULT									Render_Bloom() override;

	virtual HRESULT									Fetch(any pTransform = g_aNull) override;
	virtual _bool									Return() override;
	virtual HRESULT									Release() override;

private:
	HRESULT											Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void											Set_Data(const VFX_VisualCurve);
	const VFX_VisualCurve							Get_Data() const { return m_tData; }

	shared_ptr<class CCurveContainer>				Get_ScaleCurve() { return m_pCCScale; }
	shared_ptr<class CCurveContainer>				Get_BloomCurve() { return m_pCCBloom; }
	shared_ptr<class CCurveContainer>				Get_FadeCurve() { return m_pCCFade; }
	shared_ptr<class CCurveContainer>				Get_SpriteCurve() { return m_pCCSprite; }
	shared_ptr<class CCurveContainer>				Get_DistortionCurve() { return m_pCCDistortion; }

	void											Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCScale = pCC; }
	void											Set_BloomCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCBloom = pCC; }
	void											Set_FadeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCFade = pCC; }
	void											Set_SpriteCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCSprite = pCC; }
	void											Set_DistortionCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCDistortion = pCC; }
#endif

private:
	VFX_VisualCurve									m_tData{};

	shared_ptr<CTransform>							m_pTarget;

	shared_ptr<class CCurveContainer>				m_pCCScale;
	shared_ptr<class CCurveContainer>				m_pCCBloom;
	shared_ptr<class CCurveContainer>				m_pCCFade;
	shared_ptr<class CCurveContainer>				m_pCCSprite;
	shared_ptr<class CCurveContainer>				m_pCCDistortion;

	_float3											m_vBaseScale	= _float3(1.f, 1.f, 1.f);

public:
	static shared_ptr<CEffectRegular_VisualCurve>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;

	static shared_ptr<CEffectRegular_VisualCurve>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT									Export(const wstring& wstrFilePath) override;
#endif
};

END
