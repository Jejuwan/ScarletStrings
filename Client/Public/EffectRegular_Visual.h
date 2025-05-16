#pragma once
#include "Client_Define.h"
#include "EffectRegular.h"

BEGIN(Client)

class CEffectRegular_Visual final : public CEffectRegular
{
private:
	explicit CEffectRegular_Visual(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectRegular_Visual(const CEffectRegular_Visual&);
	virtual ~CEffectRegular_Visual() DEFAULT;
	
public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(any = g_aNull) override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

	virtual HRESULT								Render_Bloom() override;

	virtual HRESULT								Fetch(any vPosition3) override;
	virtual _bool								Return() override;
	virtual HRESULT								Release() override;

private:
	HRESULT										Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void										Set_Data(const VFX_Visual _tData)		{ m_tData = _tData; }

	const VFX_Visual							Get_Data() const						{ return m_tData; }
#endif

private:
	virtual HRESULT								Ready_Components() override;

private:
	_int										m_iSpriteIndex	= 0;

	_float										m_fFadeRatio	= 0.f;

	VFX_Visual									m_tData{};

#if ACTIVATE_TOOL
	_float4x4									m_mFetch		= g_mUnit;
#endif

public:
	static shared_ptr<CEffectRegular_Visual>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>				Clone(any = g_aNull) override;

	static shared_ptr<CEffectRegular_Visual>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT								Export(const wstring& wstrFilePath) override;
#endif
};

END
