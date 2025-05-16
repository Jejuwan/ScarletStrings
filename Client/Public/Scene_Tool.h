#pragma once
#include "Client_Define.h"
#include "Scene.h"

#define SYSTEM_WIDTH		540.f
#define PROPERTY_WIDTH		360.f
#define MAINCONTROL_WIDTH	600.f
#define SUBCONTROL_WIDTH	420.f
#define CONTROL_HEIGHT		320.f	//	215.f
#define DIALOG_HEIGHT		240.f

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)

enum class TOOL
{
	MODEL,
	EFFECT,
	MAX
};

class CScene_Tool final : public CScene
{
private:
	explicit CScene_Tool(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_Tool() DEFAULT;

public:
	virtual HRESULT								Initialize() override;
	virtual void								Tick(_float fTimeDelta) override;
	virtual void								Late_Tick(_float fTimeDelta) override;
	virtual HRESULT								Render() override;
#ifdef _DEBUG
	virtual void								Debug() override;
#endif

private:
	HRESULT										Ready_Components();
	HRESULT										Ready_GameObjects();
	HRESULT										Ready_ToolSystems();

private:
	void										Tool_MenuBar();
	void										Tool_System();
	void										Tool_Properties();
	void										Tool_MainControl();
	void										Tool_SubControl();

private:
	TOOL										m_eTool	= TOOL::MAX;
	vector<shared_ptr<class IToolSystem>>		m_vecTools;

	shared_ptr<class CCamera_Main>				m_pCamera;
	shared_ptr<class CGlobalGizmo>				m_pGizmo;

	shared_ptr<CRenderer>						m_pRenderer;

public:
	static shared_ptr<CScene_Tool>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
