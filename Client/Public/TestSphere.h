#pragma once
#include "Client_Define.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CVIBuffer_Sphere;
END

BEGIN(Client)

class CTestSphere final : public CInteractiveObject
{
private:
	explicit CTestSphere(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTestSphere(const CTestSphere&);
	virtual ~CTestSphere() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CTexture>				m_pTexture;
	shared_ptr<CVIBuffer_Sphere>		m_pVIBuffer_Sphere;

public:
	static shared_ptr<CTestSphere>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
