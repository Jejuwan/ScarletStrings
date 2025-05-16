#pragma once
#include "Client_Define.h"
#include "GameObject.h"

#ifdef _DEBUG

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CDebugCamera : public CGameObject
{
private:
	explicit CDebugCamera(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CDebugCamera(const CDebugCamera&);
	virtual ~CDebugCamera() DEFAULT;

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
	shared_ptr<CModel>					m_pModel;

public:
	static shared_ptr<CDebugCamera>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END

#endif
