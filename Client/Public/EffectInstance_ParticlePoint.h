#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Client)

class CEffectInstance_ParticlePoint final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticlePoint(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticlePoint(const CEffectInstance_ParticlePoint&);
	virtual ~CEffectInstance_ParticlePoint() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any = g_aNull) override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

	virtual HRESULT										Render_Bloom() override;

	virtual HRESULT										Fetch(any = g_aNull) override;
	virtual _bool										Return() override;
	virtual HRESULT										Release() override;

private:
	HRESULT												Initialize(std::ifstream&);

protected:
	virtual HRESULT										Ready_Components() override;

public:
#if ACTIVATE_TOOL
	void												Set_Data(const VFX_ParticlePoint);
	const VFX_ParticlePoint								Get_Data() const												{ return m_tData; }

	shared_ptr<class CCurveContainer>					Get_FadeCurve()													{ return m_pCCFade; }
	shared_ptr<class CCurveContainer>					Get_DiffuseCurve()												{ return m_pCCDiffuse; }
	void												Set_FadeCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCFade = pCC; }
	void												Set_DiffuseCurve(shared_ptr<class CCurveContainer> pCC)			{ m_pCCDiffuse = pCC; }
#endif

private:
	virtual void										Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void										Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	const INSTDESC										Instance_Restart(_uint);
	const INSTDESC										Instance_End(_uint);

private:
	VFX_ParticlePoint									m_tData{};

	vector<INSTDESC>									m_vecInstDescs;
	vector<_float4>										m_vecDiffuse;
	vector<_float>										m_vecRotation;

	shared_ptr<class CCurveContainer>					m_pCCFade;
	shared_ptr<class CCurveContainer>					m_pCCDiffuse;

public:
	static shared_ptr<CEffectInstance_ParticlePoint>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticlePoint>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT										Export(const wstring& wstrFilePath) override;
#endif
};

END
