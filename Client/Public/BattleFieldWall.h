#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModelRenderer;
END

BEGIN(Client)

class CBattleFieldWall final : public CGameObject
{
private:
	explicit CBattleFieldWall(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBattleFieldWall(const CBattleFieldWall&);
	virtual ~CBattleFieldWall() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Enable_Physicx(_bool bEnable);

public:
	void								Bake_Physics();

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModelRenderer>			m_pModelRenderer;

	shared_ptr<CTexture>				m_pNoiseTexture;

private:
	vector<PxRigidStatic*>				m_vecActor;
	ACTORDESC							m_ActorDesc;

private:
	_bool								m_bOnScene = { false };

private:
	INSTMODELDESC						m_tInstBattleFieldAsciiDesc;

private:
	_float								m_fAccTime = { 0.f };
	
public:
	static shared_ptr<CBattleFieldWall>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END