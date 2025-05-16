#pragma once

#include "Client_Define.h"
#include "Character.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNaomi final : public CCharacter
{
private:
	explicit CNaomi(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CNaomi(const CNaomi&);
	virtual ~CNaomi() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Move_Mouth() override;
	void								Stop_Mouth() override;

private:
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	static shared_ptr<CNaomi>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END