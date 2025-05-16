#pragma once
#include "Client_Define.h"
#include "ScreenEffect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CColorReverseEffect final : public CScreenEffect
{
private:
	explicit CColorReverseEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CColorReverseEffect(const CColorReverseEffect&);
	virtual ~CColorReverseEffect() DEFAULT;

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

public:
	static shared_ptr<CColorReverseEffect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END