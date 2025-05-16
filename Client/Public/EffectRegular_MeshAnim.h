#pragma once
#include "Client_Define.h"
#include "EffectRegular.h"

BEGIN(Client)

class CEffectRegular_MeshAnim final : public CEffectRegular
{
private:
	explicit CEffectRegular_MeshAnim(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectRegular_MeshAnim(const CEffectRegular_MeshAnim&);
	virtual ~CEffectRegular_MeshAnim() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

	virtual HRESULT									Render_Bloom() override;

	virtual HRESULT									Fetch(any vPosition3) override;
	virtual _bool									Return() override;
	virtual HRESULT									Release() override;

private:
	HRESULT											Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void											Set_Data(const VFX_MeshAnim _tData) { m_tData = _tData; }
	const VFX_MeshAnim								Get_Data() const { return m_tData; }

	shared_ptr<class CCurveContainer>				Get_ScaleCurve() { return m_pCCScale; }
	shared_ptr<class CCurveContainer>				Get_TranslationCurve() { return m_pCCTranslation; }
	shared_ptr<class CCurveContainer>				Get_BloomCurve() { return m_pCCBloom; }
	shared_ptr<class CCurveContainer>				Get_FadeCurve() { return m_pCCFade; }
	shared_ptr<class CCurveContainer>				Get_DiffuseCurve() { return m_pCCDiffuse; }
	shared_ptr<class CCurveContainer>				Get_EmissiveCurve() { return m_pCCEmissive; }
	shared_ptr<class CCurveContainer>				Get_SpriteCurve() { return m_pCCSprite; }

	void											Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCScale = pCC; }
	void											Set_TranslationCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCTranslation = pCC; }
	void											Set_BloomCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCBloom = pCC; }
	void											Set_FadeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCFade = pCC; }
	void											Set_DiffuseCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCDiffuse = pCC; }
	void											Set_EmissiveCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCEmissive = pCC; }
	void											Set_SpriteCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCSprite = pCC; }
#endif

private:
	virtual HRESULT									Ready_Components() override;

public:
	virtual HRESULT									Add_Component(const COMPONENT, shared_ptr<class CComponent> = nullptr) override;

private:
	_int											m_iMeshIndex	= 0;
	_float											m_fFadeRatio	= 1.f;
	_float											m_fBloomRatio	= 1.f;
	_float3											m_vFetchPosition;

	VFX_MeshAnim									m_tData{};

	shared_ptr<class CCurveContainer>				m_pCCScale;
	shared_ptr<class CCurveContainer>				m_pCCTranslation;
	shared_ptr<class CCurveContainer>				m_pCCBloom;
	shared_ptr<class CCurveContainer>				m_pCCFade;
	shared_ptr<class CCurveContainer>				m_pCCDiffuse;
	shared_ptr<class CCurveContainer>				m_pCCEmissive;
	shared_ptr<class CCurveContainer>				m_pCCSprite;

	_float3											m_vBaseScale	= _float3(1.f, 1.f, 1.f);

#if ACTIVATE_TOOL
	_float4x4										m_mFetch		= g_mUnit;
#endif

public:
	static shared_ptr<CEffectRegular_MeshAnim>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;

	static shared_ptr<CEffectRegular_MeshAnim>		Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT									Export(const wstring& wstrFilePath) override;
#endif
};

END
