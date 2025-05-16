#pragma once
#include "Client_Define.h"

BEGIN(Engine)
class CObjectLayer;
END

BEGIN(Client)

class CMap_ToolManager final : public CSingleton<CMap_ToolManager>
{
public:
	typedef struct tagOBJECTDESC {
		wstring		strName;
		_float3		Scale = { 1.f, 1.f, 1.f };
		_float3		Rotation = { 0.f, 0.f, 0.f };
		_float3		Translation = { 0.f, 0.f, 0.f };
		_float4x4	matrix;
		_bool		bSave = { false };
	}OBJECTDESC;

private:
	explicit CMap_ToolManager() DEFAULT;
	virtual ~CMap_ToolManager() DEFAULT;

public:
	void		Tick(_float fTimeDelta);
	void		Late_Tick(_float fTimeDelta);

public:
	/* Erase All List */
	_bool		Clear_List(_bool bClear = false);
	void		Clear_Obj();
	HRESULT		NameNum_List(wstring NameNum);
	HRESULT		Emplace_Object(void* pArg = nullptr);
	HRESULT		Save_File(_int iStageNum);
	HRESULT		Delete_Object(wstring Name);

	HRESULT		Ready_TestMap();
	HRESULT		Ready_MizuhaMap();
	HRESULT		Ready_SuohMap(_int iIndex);
	HRESULT		Ready_MindRoom();
	HRESULT		Ready_Hideout();

	wstring		TutorialMap_Name(wstring strObjectName);
	wstring		MizuhaMap_Name(wstring strObjectName);
	wstring		SuohMap_Name(wstring strObjectName);
	wstring		MindRoom_Name(wstring strObjectName);
	wstring		Hideout_Name(wstring strObjectName);

private:
	HRESULT		Load_MapFile(wstring strObjName, _int iIndex);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public: /* Get */
	tuple<wstring,_float3, _float3, _float3, _float4x4> Get_TuplePivot() { return m_tuplePivot; };
	wstring	Get_SelectName() { return m_strSelectedName; };
	//wstring	Get_PickObjName() { return m_strPickingObjName; };
	_bool	Get_Save() { return m_bSave; };
	_bool	Get_Load() { return m_bLoad; };
	_bool	Get_Delete() { return m_bDelete; };
	_bool	Get_AllClear() { return m_bAllClear; };
	_bool	Get_ClearDone() { return m_bClearDone; };
	_bool	Get_Emplace() { return m_bEmplace; };
	list<wstring> Get_ListNameNum() { return m_listNameNum; };
	_vector Get_CameraPos() { return m_vCamerPos; };
	HWND	Get_Hwnd() { return m_hWnd; };
	_bool	Get_PickObjMode() { return m_bPickObjMode; };
	_bool	Get_PickingObject() { return m_bPickingObject; };
	_float3 Get_PickingPos() { return m_PickingPos; };

public:	/* Set */
	void	Set_TuplePivot(tuple<wstring, _float3, _float3, _float3, _float4x4> tuplePivot) { m_tuplePivot = tuplePivot; };
	void	Set_SelectName(wstring Name) { m_strSelectedName = Name; };
	//void	Set_PickObjName(wstring Name) { m_strPickingObjName = Name; };
	void	Set_Save(_bool bSave) { m_bSave = bSave; };
	void	Set_Load(_bool bLoad) { m_bLoad = bLoad; };
	void	Set_Delete(_bool bDelete) { m_bDelete = bDelete; };
	void	Set_Emplace(_bool bEmplace) { m_bEmplace = bEmplace; };
	void	Set_Layer(shared_ptr<CObjectLayer> pLayer) { m_pLayer = pLayer; };
	void	Set_CameraPos(_vector vCamPos) { m_vCamerPos = vCamPos; };
	void	Set_Hwnd(HWND hwnd) { m_hWnd = hwnd; };
	void	Set_StageNum(_int iStageNum) { m_iStageNum = iStageNum; };
	void	Set_PickObjMode(_bool bPick) { m_bPickObjMode = bPick; };
	void	Set_PickingObject(_bool bPick) { m_bPickingObject = bPick; };
	void	Set_PickingPos(_float3 Pos) { m_PickingPos = Pos; };
	void	Set_AllClear(_bool bAllClear) { m_bAllClear = bAllClear; };
	void	Set_ClearDone(_bool bClearDone) { m_bClearDone = bClearDone; };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	/* Scale, Rotation, Translation, matrix */
	tuple<wstring, _float3, _float3, _float3, _float4x4> m_tuplePivot;

	list<tuple<wstring, _float3, _float3, _float3, _float4x4>> m_listPivot;

	list<wstring>	m_listNameNum;

	list<_bool>		m_listClearObject;

/* bool */
	_bool	m_bSave = { false };
	_bool	m_bDelete = { false };
	_bool	m_bEmplace = { false };
	_bool	m_bLoad = { false };
	_bool	m_bPickObjMode = { false };
	_bool	m_bPickingObject = { false };
	_bool	m_bAllClear = { false };
	_bool	m_bClearDone = { false };

	_int	m_iCount = { 0 };
	_int	m_iStageNum = { 0 };

	OBJECTDESC m_ObjectDesc;

	set<wstring> m_setElement;  // 중복을 방지하기 위한 set

	_vector m_vCamerPos = { XMVectorSet(0.f,0.f,0.f,0.f) };

	HWND	m_hWnd = 0;

	shared_ptr<CObjectLayer> m_pLayer;

	wstring	m_strSelectedName;
	wstring	m_strObjName;
	wstring	m_strNameNum;
	wstring	m_strPrototypeName;

	_float3 m_PickingPos = _float3(0.f,0.f,0.f);

	//wstring	m_strPickingObjName;


////////////////////////////////////////////////////////////////////////////////////////////////
private:
	_bool	m_IsMindRoom = { false };

public:
	_bool	Is_MindRoom() { return m_IsMindRoom; };
	void	Set_Is_MindRoom(_bool IsMindRoom) { m_IsMindRoom = IsMindRoom; };

	friend CSingleton<CMap_ToolManager>;
};

END
