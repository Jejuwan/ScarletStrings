#pragma once
#include "Client_Define.h"
#include "GameObject.h"

#ifdef _DEBUG

BEGIN(Client)

class CTestModel final : public CGameObject
{
private:
	explicit CTestModel(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTestModel(const CTestModel&);
	virtual ~CTestModel() DEFAULT;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(any = g_aNull) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

	virtual HRESULT					Render_ShadowDepth(shared_ptr<class CLight>);

private:
	virtual HRESULT					Ready_Components() override;
	virtual HRESULT					Ready_Behaviors() override;

public:
	static shared_ptr<CTestModel>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = g_aNull);
};

END

#endif
