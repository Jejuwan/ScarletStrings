#pragma once
#include "Client_Define.h"
#include "Scene.h"

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)

enum class OBJ { OBJ_STATIC, OBJ_DYNAMIC, OBJ_END};

class CScene_MapTool : public CScene
{
private:
	explicit CScene_MapTool(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CScene_MapTool() DEFAULT;

public:
	virtual HRESULT			Initialize() override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;
#ifdef _DEBUG
	virtual void			Debug() override;
#endif

private:
	HRESULT					Ready_Components();
	HRESULT					Ready_GameObjects();
	HRESULT					Ready_Lights();
private:
	shared_ptr<class CCamera_MapTool>				m_pCamera;
	shared_ptr<class CGlobalGizmo>				m_pGizmo;
	shared_ptr<CRenderer>						m_pRenderer;

	_bool	m_bshow_Menu_window = { false };
	_bool	m_bGizmo = { false };
	_bool	m_bCreate = { false };

	wstring	m_strFileName;

	shared_ptr<class CMap_Tool>	m_pMap_Tool;

public:
	static shared_ptr<CScene_MapTool>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
