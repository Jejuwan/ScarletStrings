#pragma once
#include "Client_Define.h"
#include "System.h"

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CModel;
class CAnimation;
class CBone;
class CChannel;
class CMesh;
class CTexture;
class CObjectLayer;
END

BEGIN(Client)


class CMap_Tool : public ISystem
{
public:
	enum STAGE { TUTORIAL, MIZUHA, SUOH, SUOH_BATTLE, MINDROOM, HIDEOUT, STAGE_END };

private:
	explicit CMap_Tool(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CMap_Tool() DEFAULT;

public:
	virtual HRESULT							Initialize();
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;



private:
	void		Menu();
	void		PivotWindow();
	void		MenuWindow();
	void		ObjectListWindow();
	void		EmplaceListWindow();
	void		ChangePivotListWindow();
	void		Check_UI();

	/* Maptool_Create_Tutorial.h */
	void		Create_Tutorial();
	/* Maptool_Create_Mizuha.h */
	void		Create_Mizuha();
	/* Maptool_Create_Suoh.h */
	void		Create_NeonLine();
	void		Create_Suoh();
	/* Maptool_Create_MindRoom.h */
	void		Create_MindRoom();
	/* Maptool_Create_Hideout.h */
	void		Create_Hideout();

	vector<wstring>			mdlFileNames(const wstring& strPath);
	void		Use_FileList(wstring strFilePath, string HeaderName,_bool bCheck);
	wstring		Object_Numbering(wstring& baseName, list<wstring>& ListNames);

public:
	wstring		Get_FileName() { return m_strFileName; };
	tuple<wstring, _float3, _float3, _float3, _float4x4> Get_TuplePivot() { return m_tuplePivot; };

public:
	_float3		Get_PickingPos() { return m_PickingPos; }

private: 
	void		Change_Save_Load_Button(_int iIndex);
	void		Change_Scene();
	void		Mouse_Ray();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	shared_ptr<CGameObject> m_pDummyObject;
	shared_ptr<CObjectLayer> m_RoadLayer;

private:
	_bool	m_bObject_Menu = { true };
	_bool	m_bObjectList_Menu = { true };
	_bool	m_bPivot_Menu = { true };
	_bool	m_bChangePivotList_Menu = { true };
	_bool	m_bEmplaceList_Menu = { true };
	_bool	m_bPhysxRender = { true };
	_bool	m_bApplyPickingPos = { false };
	_bool	m_bPickingObjectMode = { false };
	_bool	m_bCheck_UI = { false };
	_bool	m_bCheck[5] = { false };
	_bool	m_bLoadWindow = { false };
	_bool	m_bScene_Change[10] = { false };

	_bool	m_bCreate = { false };
	_bool	m_bDelete = { false };
	_bool	m_bSave = { false };
	_bool	m_bLoad = { false };
	_bool	m_bEmplace = { false };
	_bool	m_bApplyPos = { false };

	_float	m_fTimer = { 0.f };

	_vector	m_vMouseRay, m_vMouseRayPos = { XMVectorSet(0.f,0.f,0.f,0.f) };

	_float3 m_MousePos = { 0.f,0.f,0.f };
	_float3 m_PickingPos = { 0.f,0.f,0.f };

	wstring	m_strFileName;
	wstring	m_strSelectedName;
	wstring	m_strObjName;

	list<wstring> m_listObjName;
	list<wstring> m_listChangePivotName;
	list<wstring> m_listEmplaceName;

	/* Scale, Rotation, Translation, matrix */
	tuple<wstring, _float3, _float3, _float3, _float4x4> m_tuplePivot;
	
	STAGE	m_eStage;


public:
	static shared_ptr<CMap_Tool>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
