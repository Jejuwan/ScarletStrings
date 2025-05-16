#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CKeyframe;
class CCamera;
END

BEGIN(Client)

class CCCTV final : public CGameObject
{
private:
	explicit CCCTV(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCCTV(const CCCTV&);
	virtual ~CCCTV() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Lock_Camera(_bool bLock);

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;
	shared_ptr<CKeyframe>				m_pKeyframe;

	shared_ptr<CTexture>				m_pLineTexture;

	shared_ptr<CRenderer>				m_pRenderer;

private:
	_float4x4							m_mProjection;

private:
	_float								m_fAccTime = { 0.f };

private:
	shared_ptr<CTransform>				m_pParentTransform;
	shared_ptr<CCamera>					m_pCamera;

	shared_ptr<CTransform>				m_pCCTVPivotTransform;

private:
	_bool								m_bActiveCCTV = { false };

public:
	static shared_ptr<CCCTV>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END