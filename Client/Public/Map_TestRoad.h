#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap_TestRoad : public CGameObject
{
private:
	explicit CMap_TestRoad(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_TestRoad(const CMap_TestRoad&);
	virtual ~CMap_TestRoad() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	shared_ptr<class CMap_Tool>	m_pMap_Tool;

private:
	_float m_fTimer = { 0.f };
	_float m_fTemp = { 1.f };

public:
	static shared_ptr<CMap_TestRoad>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
