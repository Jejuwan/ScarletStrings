#pragma once

#include "Client_Define.h"
#include "Scene.h"

BEGIN(Engine)
class CObjectLayer;
class CRenderer;
class CModelRenderer;
END

BEGIN(Client)

class CScene_Suoh_Battle final : public CScene
{
private:
	explicit CScene_Suoh_Battle(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Suoh_Battle() DEFAULT;

public:
	HRESULT							Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

#ifdef _DEBUG
	virtual void					Debug() override;
#endif

	static HRESULT					Apply_PostProcessing();

private:
	HRESULT							Ready_Camera();
	HRESULT							Ready_Player();
	HRESULT							Ready_Lights();
	HRESULT							Ready_BackGround();

	HRESULT							Ready_Character();
	HRESULT							Ready_Monsters();
	HRESULT							Ready_UI();
	HRESULT							Ready_UI_Late();

#pragma region Map
private:
	HRESULT		Ready_Map();
	HRESULT		Load_SuohMapFile(wstring strObjName, _int iIndex);
private:
	shared_ptr<CObjectLayer> m_pStaticLayer;
	shared_ptr<CObjectLayer> m_pRoadLayer;
	shared_ptr<CObjectLayer> m_pDynamicLayer;
	shared_ptr<CObjectLayer> m_pBuildingLayer;
	shared_ptr<CObjectLayer> m_pSpecialDynamicLayer;
	shared_ptr<CObjectLayer> m_pETCLayer;
	shared_ptr<class CWave> m_pWave;
	wstring	m_strObjName;
	wstring	m_strPrototypeName;
#pragma endregion
#ifdef _DEBUG
	shared_ptr<class CRenderer>	m_pRenderer;
	shared_ptr<CModelRenderer> m_pModelRenderer;
#endif

public:
	static shared_ptr<CScene_Suoh_Battle>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END