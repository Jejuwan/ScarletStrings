#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CBattleField : public CGameObject
{
private:
	explicit CBattleField(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBattleField(const CBattleField&);
	virtual ~CBattleField() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

public:
	virtual void									Set_Enable(_bool bEnable) override;

private:
	virtual HRESULT									Ready_Components() override;

private:
	HRESULT											Import(const wstring& wstrFilePath);

private:
	std::vector<shared_ptr<class CBattleFieldWall>>	m_vecBattleField;

public:
	static shared_ptr<CBattleField>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END