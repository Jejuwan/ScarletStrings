#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Engine)
class ICurves;
END

BEGIN(Client)

class CEffectInstance_ParticleRect final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticleRect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticleRect(const CEffectInstance_ParticleRect&);
	virtual ~CEffectInstance_ParticleRect() DEFAULT;

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

public:
#if ACTIVATE_TOOL
	void												Set_Data(const VFX_ParticleRect);
	const VFX_ParticleRect								Get_Data() const	{ return m_tData; }
#endif

private:
	virtual void										Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void										Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	VFX_ParticleRect									m_tData{};

	shared_ptr<ICurves>									m_pInitiateCurves;

public:
	static shared_ptr<CEffectInstance_ParticleRect>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticleRect>		Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT										Export(const wstring& wstrFilePath) override;
#endif
};

END
