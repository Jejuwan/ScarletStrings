#pragma once
#include "Client_Define.h"
#include "EffectRegular.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CEffectRegular_Distortion final : public CEffectRegular
{
private:
	explicit CEffectRegular_Distortion(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectRegular_Distortion(const CEffectRegular_Distortion&);
	virtual ~CEffectRegular_Distortion() DEFAULT;

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
	const VFX_Distortion							Get_Data() const				{ return m_tData; }

	void											Set_Data(VFX_Distortion tData)	{ m_tData = tData; }
#endif

private:
	_uint											m_iSpriteIndex	= 0;

	VFX_Distortion									m_tData{};

#if ACTIVATE_TOOL
	_float4x4										m_mFetch		= g_mUnit;
#endif

public:
	static shared_ptr<CEffectRegular_Distortion>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;

	static shared_ptr<CEffectRegular_Distortion>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT									Export(const wstring& wstrFilePath) override;
#endif
};

END
