#pragma once
#include "Client_Define.h"
#include "ScreenEffect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPurpleFilterEffect final : public CScreenEffect
{
private:
	explicit CPurpleFilterEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPurpleFilterEffect(const CPurpleFilterEffect&);
	virtual ~CPurpleFilterEffect() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

public:
	virtual void							Set_Enable(_bool bEnable);

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTexture>					m_pLUTTexture;
	_float									m_fLUTFactor;

public:
	static shared_ptr<CPurpleFilterEffect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END