#pragma once

#include "Client_Define.h"
#include "Scene.h"

BEGIN(Engine)
class CObjectLayer;
class CRenderer;
class CModelRenderer;
END

BEGIN(Client)

class CScene_Title final : public CScene
{
private:
	explicit CScene_Title(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Title() DEFAULT;

public:
	HRESULT							Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	static HRESULT					Apply_PostProcessing();

private:
	HRESULT							Ready_Camera();
	HRESULT							Ready_BackGround();
	HRESULT							Ready_Character();
	HRESULT							Add_Light();

public:
	static shared_ptr<CScene_Title>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END