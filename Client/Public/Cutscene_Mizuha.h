#pragma once
#include "Client_Define.h"
#include "Scene.h"

BEGIN(Engine)
class CObjectLayer;
END

BEGIN(Client)

class CCutscene_Mizuha final : public CScene
{
private:
	explicit CCutscene_Mizuha(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCutscene_Mizuha() DEFAULT;

public:
	HRESULT							Initialize() override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

#ifdef _DEBUG
public:
	virtual void					Debug() override;

private:
	HRESULT							Ready_DebugObjects();
#endif

private:
	HRESULT							Ready_Camera();
	HRESULT							Ready_BackGround();
	HRESULT							Ready_TestObject();
	HRESULT							Ready_TestUI();
	HRESULT							Ready_Lights();

#pragma region Map
private:
	HRESULT		Ready_Map();
	HRESULT		Load_MizuhaMapFile(wstring strObjName, _int iIndex);
	void		Map_Name();
	void		Map_Name_Second();
private:
	shared_ptr<CObjectLayer> m_pStaticLayer;
	shared_ptr<CObjectLayer> m_pRoadLayer;
	shared_ptr<CObjectLayer> m_pDynamicLayer;
	shared_ptr<CObjectLayer> m_pBuildingLayer;
	shared_ptr<CObjectLayer> m_pSpecialDynamicLayer;
	shared_ptr<CObjectLayer> m_pETCLayer;
	wstring	m_strObjName;
	wstring	m_strPrototypeName;
#pragma endregion

private:
	std::shared_ptr<class CMizuha_Scenario> m_pScenario;

private:
	//std::unordered_map<wstring

public:
	static shared_ptr<CCutscene_Mizuha>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END