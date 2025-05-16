#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Engine)
class CObjectLayer;
class CRenderer;
class CLight;
class CModelRenderer;
END

BEGIN(Client)

class CScene_Test final : public CScene
{
private:
	explicit CScene_Test(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Test() DEFAULT;

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
	HRESULT							Ready_Lights();
	HRESULT							Ready_BackGround();
	HRESULT							Ready_Player();
	HRESULT							Ready_Monster();
	HRESULT							Ready_TestUI();
	HRESULT							Ready_TestUI_Late();

	HRESULT							Ready_Character();
	HRESULT							Ready_BattleField();
	HRESULT							Ready_EventCollider();

#pragma region Map
private:
	HRESULT		Load_TutorialMapFile(wstring strObjName, _int iIndex);
	HRESULT		Ready_TestMap();

private:
	shared_ptr<CObjectLayer> m_pStaticLayer;
	shared_ptr<CObjectLayer> m_pDynamicLayer;
	shared_ptr<CObjectLayer> m_pPlantLayer;
	shared_ptr<class CWave> m_pWave;
	wstring	m_strObjName;
	wstring	m_strPrototypeName;
#pragma endregion

private:
	_bool						m_bPMCCSuccess = { true };
	_bool						m_bLockOnSuccess = { true };
	_bool						m_bBattleStyleSuccess = { true };
	_bool						m_bSpecialAttackSuccess = { true };

#ifdef _DEBUG
	shared_ptr<class CRenderer>	m_pRenderer;
	shared_ptr<class CModelRenderer> m_pModelRenderer;
#endif

public:
	static shared_ptr<CScene_Test>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END