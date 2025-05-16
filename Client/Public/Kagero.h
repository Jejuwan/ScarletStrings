#pragma once
#include "Client_Define.h"
#include "Character.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CKagero final : public CCharacter
{
private:
	explicit CKagero(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CKagero(const CKagero&);
	virtual ~CKagero() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Move_Mouth() override;
	void								Stop_Mouth() override;

	virtual void						Execute_SAS(const std::function<void()>& fpFinished = nullptr) override;

private:
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

public:
	static shared_ptr<CKagero>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END