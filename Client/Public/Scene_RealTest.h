#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Engine)
class CObjectLayer;
class CRenderer;
END

BEGIN(Client)

class CScene_RealTest final : public CScene
{
private:
	explicit CScene_RealTest(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_RealTest() DEFAULT;

public:
	HRESULT							Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

#ifdef _DEBUG
	virtual void					Debug() override;
#endif

private:
	HRESULT							Ready_Camera();
	HRESULT							Ready_Lights();
	HRESULT							Ready_BackGround();
	HRESULT							Ready_Player();
	HRESULT							Ready_Character();
	HRESULT							Ready_TestObject(const wstring& wstrProto);
	HRESULT							Ready_TestUI();
	HRESULT							Ready_TestDynamic();

private:
	_float m_fEffect_Timer = 0.75f;

#ifdef _DEBUG
	shared_ptr<class CRenderer>	m_pRenderer;
#endif
	shared_ptr<CObjectLayer> m_pDynamicLayer;
public:
	static shared_ptr<CScene_RealTest>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);

private:
	_float m_EffectTimer = 0.f;
};

END