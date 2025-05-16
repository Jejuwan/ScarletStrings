#pragma once
#include "Client_Define.h"
#include "InteractiveObject.h"

BEGIN(Client)

class CTestBarrigate final : public CInteractiveObject
{
private:
	explicit CTestBarrigate(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTestBarrigate(const CTestBarrigate&);
	virtual ~CTestBarrigate() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render();

	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	PxRigidDynamic*						m_pActor;
public:
	static shared_ptr<CTestBarrigate>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull);
};

END