#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CScreenEffect abstract : public CGameObject
{
protected:
	explicit CScreenEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CScreenEffect(const CScreenEffect&);
	virtual ~CScreenEffect() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

public:
	virtual void							Set_Enable(_bool bEnable);

protected:
	virtual HRESULT							Ready_Components() override;

protected:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

protected:
	_float4x4								m_matProjection;

	_float									m_fShaderTime = { 0.f };
};

END