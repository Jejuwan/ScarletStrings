#pragma once
#include "Client_Define.h"
#include "ScreenEffect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CScreenRippleEffect final : public CScreenEffect
{
public:
	typedef struct tagRippleDesc
	{
		_float fRippleSpeed;
		_float fRippleStrength;
	} RIPPLEDESC;
private:
	explicit CScreenRippleEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CScreenRippleEffect(const CScreenRippleEffect&);
	virtual ~CScreenRippleEffect() DEFAULT;

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
	_float									m_fRippleSpeed = { 0.f };
	_float									m_fRippleStrength = { 0.f };

public:
	static shared_ptr<CScreenRippleEffect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any = g_aNull);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END