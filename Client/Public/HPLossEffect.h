#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CHPLossEffect : public CGameObject
{
private:
	explicit CHPLossEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CHPLossEffect(const CHPLossEffect&);
	virtual ~CHPLossEffect() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	virtual void						Set_Enable(_bool bEnable) override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;
	shared_ptr<CTexture>				m_pTexture;

private:
	_float4x4							m_matProjection;

private:
	_float								m_fAccTime;
	_uint								m_iCycleCount;

public:
	static shared_ptr<CHPLossEffect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END