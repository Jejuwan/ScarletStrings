#include "EnginePCH.h"
#include "GameInstance.h"
#include "GraphicDevice.h"
#include "RenderTarget_Manager.h"
#include "ConstantBuffer_Manager.h"
#include "Timer_Manager.h"
#include "Mouse_Manager.h"
#include "Key_Manager.h"
#include "Event_Handler.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"
#include "Component_Manager.h"
#include "Grid_Manager.h"
#include "Light_Manager.h"
#include "Picker.h"
#include "PhysXEngine.h"
#include "ImageProcess_Manager.h"
#include "InteractiveObjectFinder.h"
#include "InteractiveObject.h"
#include "ResourceManager.h"
#include "SceneCapture.h"
#include "Font_Manager.h"
#include "Dialogue_Manager.h"
#include "Frustom.h"
#include "ComputeAccelerator.h"

CGameInstance::CGameInstance()
	: m_pGraphic_Device			(CGraphicDevice::Get_Instance())
	, m_pRenderTarget_Manager	(CRenderTarget_Manager::Get_Instance())
	, m_pConstantBuffer_Manager	(CConstantBuffer_Manager::Get_Instance())
	, m_pSound_Manager			(CSound_Manager::Get_Instance())
	, m_pTimer_Manager			(CTimer_Manager::Get_Instance())
	, m_pMouse_Manager			(CMouse_Manager::Get_Instance())
	, m_pKey_Manager			(CKey_Manager::Get_Instance())
	, m_pEvent_Handler			(CEvent_Handler::Get_Instance())
	, m_pScene_Manager			(CScene_Manager::Get_Instance())
	, m_pObject_Manager			(CObject_Manager::Get_Instance())
	, m_pCollision_Manager		(CCollision_Manager::Get_Instance())
	, m_pComponent_Manager		(CComponent_Manager::Get_Instance())
	, m_pGrid_Manager			(CGrid_Manager::Get_Instance())
	, m_pLight_Manager			(CLight_Manager::Get_Instance())
	, m_pPicker					(CPicker::Get_Instance())
	, m_pPhysXEngine			(CPhysXEngine::Get_Instance())
	, m_pImageProcess			(CImageProcess_Manager::Get_Instance())
	, m_pInteractiveObjectFinder(CInteractiveObjectFinder::Get_Instance())
	, m_pResource_Manager		(CResourceManager::Get_Instance())
	, m_pSceneCapture			(CSceneCapture::Get_Instance())
	, m_pFont_Manager			(CFont_Manager::Get_Instance())
	, m_pDialogue_Manager		(CDialogue_Manager::Get_Instance())
	, m_pFrustom				(CFrustom::Get_Instance())
{
}

#pragma region Engine

HRESULT CGameInstance::Initialize_Engine(_Out_ ComPtr<ID3D11Device>& _pDevice, _Out_ ComPtr<ID3D11DeviceContext>& _pContext, _In_ const GRAPHICDESC _tGraphicDesc, _In_ const SCENE _eStaticScene, _In_ const SCENE _eMaxScene, _In_ const COLLISION_GROUP _eMaxGroup, _In_ const SOUND_CHANNEL _eMaxChannel)
{
	if (FAILED(m_pGraphic_Device->Ready_GraphicDevice(_tGraphicDesc, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGraphic_Device->Ready_GraphicDevice");
	}
	m_pDevice = _pDevice;
	m_pContext = _pContext;
	if (FAILED(m_pPhysXEngine->Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_PhysXEngine->Initialize");
	}

	if (FAILED(m_pLight_Manager->Initialize(_pDevice, _pContext, _tGraphicDesc, _eMaxScene)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pLight_Manager->Reserve_Manager");
	}

	if (FAILED(m_pRenderTarget_Manager->Initialize(_pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pRenderTarget_Manager->Initialize");
	}

	if (FAILED(m_pPicker->Initialize(_tGraphicDesc.hWnd)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pPicker->Initialize");
	}

	if (FAILED(m_pSound_Manager->Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pSound_Manager->Initialize");
	}

	if (FAILED(m_pMouse_Manager->Initialize(_tGraphicDesc.hWnd, POINT{static_cast<LONG>(_tGraphicDesc.iWinCX), static_cast<LONG>(_tGraphicDesc.iWinCY)})))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pMouse_Manager->Initialize");
	}

	if (FAILED(m_pKey_Manager->Initialize(_tGraphicDesc.hWnd)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pKey_Manager->Initialize");
	}

	if (FAILED(m_pScene_Manager->Initialize(_eStaticScene, _eMaxScene)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pScene_Manager->Initialize");
	}

	if (FAILED(m_pObject_Manager->Reserve_Manager(_eMaxScene)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pObject_Manager->Reserve_Manager");
	}

	if (FAILED(m_pCollision_Manager->Reserve_Manager(_eMaxGroup)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pCollision_Manager->Reserve_Manager");
	}

	if (FAILED(m_pComponent_Manager->Reserve_Manager(_eMaxScene)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pComponent_Manager->Reserve_Manager");
	}

	if (FAILED(m_pGrid_Manager->Reserve_Manager(_eMaxScene, g_vGridSize)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGrid_Manager->Reserve_Manager");
	}
	m_pInteractiveObjectFinder->Initialize(_tGraphicDesc.iWinCX, _tGraphicDesc.iWinCY);

	if (FAILED(m_pDialogue_Manager->Initialize(_tGraphicDesc.iWinCX, _tGraphicDesc.iWinCY)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pDialogue_Manager->Initialize");
	}

#if defined(_DEBUG)
#if ACTIVATE_CONSOLE
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif
#elif FORCE_CONSOLE
#if ACTIVATE_CONSOLE
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif
#endif

	m_tGraphicDesc = _tGraphicDesc;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float _fTimeDelta)
{
	if (!m_pMouse_Manager
	||	!m_pKey_Manager
	||	!m_pScene_Manager
	||	!m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Tick_Engine", "Null Exception");
	}

	m_fTimeAcc += _fTimeDelta;

	m_pMouse_Manager->Tick();
	m_pKey_Manager->Tick();
	m_pInteractiveObjectFinder->Tick();

	m_pScene_Manager->Tick(_fTimeDelta);
	m_pObject_Manager->Tick(_fTimeDelta);

	m_pCollision_Manager->Tick(_fTimeDelta);
	m_pImageProcess->Tick(_fTimeDelta);

	m_pFrustom->Tick();

	m_pScene_Manager->Late_Tick(_fTimeDelta);
	m_pObject_Manager->Late_Tick(_fTimeDelta);
	
	m_pPicker->Tick();
	
	m_pLight_Manager->Tick(_fTimeDelta);

	m_pEvent_Handler->Tick(_fTimeDelta);

	m_pPhysXEngine->Tick(_fTimeDelta);

	m_pDialogue_Manager->Tick(_fTimeDelta);
}

#ifdef _DEBUG
void CGameInstance::Debug_Engine()
{
	m_pScene_Manager->Debug();
}
#endif

LRESULT CGameInstance::WndProcHandler(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	if (nullptr != m_pMouse_Manager)
	{
		m_pMouse_Manager->Handle_MessageProc(_hWnd, _message, _wParam, _lParam);
	}

	return 1;
}

_float CGameInstance::Get_ActivatedTime() const
{
	return m_fTimeAcc;
}

GRAPHICDESC CGameInstance::Get_GraphicDesc() const
{
	return m_tGraphicDesc;
}

const string CGameInstance::AbsolutePath(const string& _strRelativePath)
{
	return filesystem::absolute(_strRelativePath).string();
}

const wstring CGameInstance::AbsolutePath(const wstring& _wstrRelativePath)
{
	return filesystem::absolute(_wstrRelativePath).wstring();
}

const string CGameInstance::RelativePath(const string& _strAbsolutePath)
{
	filesystem::path directory	= filesystem::current_path();
	filesystem::path absolute	= _strAbsolutePath;
	filesystem::path relative	= filesystem::relative(absolute, directory);

	return relative.string();
}

const wstring CGameInstance::RelativePath(const wstring& _wstrAbsolutePath)
{
	filesystem::path directory	= filesystem::current_path();
	filesystem::path absolute	= _wstrAbsolutePath;
	filesystem::path relative	= filesystem::relative(absolute, directory);

	return relative.wstring();
}

#pragma endregion
#pragma region Graphic Device

HRESULT CGameInstance::Clear_BackBuffer_View(_color _cBackBuffer)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_BackBuffer_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_BackBuffer_View(_cBackBuffer);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_DepthStencil_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Present", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Present();
}

#pragma endregion
#pragma region RenderTarget Manager

ComPtr<ID3D11ShaderResourceView> CGameInstance::Get_RenderTarget_ShaderResourceView(const wstring& _wstrTargetTag)
{
	if (nullptr == m_pRenderTarget_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Get_RenderTarget_ShaderResourceView", "Null Exception: m_pRenderTarget_Manager");
	}

	return m_pRenderTarget_Manager->Get_ShaderResourceView(_wstrTargetTag);
}

void CGameInstance::Copy_From_BackBuffer(const wstring& _wstrTargetTag)
{
	if (nullptr == m_pRenderTarget_Manager)
	{
		MSG_RETURN(, "CGameInstance::Copy_From_BackBuffer", "Null Exception: m_pRenderTarget_Manager");
	}

	m_pRenderTarget_Manager->Copy_From_BackBuffer(_wstrTargetTag);
}

//void CGameInstance::Enable_SSAO(_bool bEnable)
//{
//	assert(m_pRenderTarget_Manager && "CGameInstance::Get_RenderTarget_ShaderResourceView Null Exception: m_pRenderTarget_Manager");
//
//	return m_pRenderTarget_Manager->Enable_SSAO(bEnable);
//}

#pragma endregion
#pragma region ConstantBuffer Manager

HRESULT CGameInstance::Create_ConstantBuffer(SHADER_STAGE _eShaderStage, _uint _iSlot, const size_t _nLength, _bool _bReleaseBuffer)
{
	if (nullptr == m_pConstantBuffer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Create_ConstantBuffer", "Null Exception: m_pConstantBuffer_Manager");
	}

	return m_pConstantBuffer_Manager->Create_Buffer(_eShaderStage, _iSlot, _nLength, _bReleaseBuffer);
}

HRESULT CGameInstance::Update_ConstantBuffer(SHADER_STAGE _eShaderStage, _uint _iSlot, void* _pData, size_t _nLength, _uint _iOffset)
{
	if (nullptr == m_pConstantBuffer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Update_ConstantBuffer", "Null Exception: m_pConstantBuffer_Manager");
	}

	return m_pConstantBuffer_Manager->Update_Buffer(_eShaderStage, _iSlot, _pData, _nLength, _iOffset);
}

HRESULT CGameInstance::Bind_ConstantBuffer(SHADER_STAGE _eShaderStage, _uint _iSlot)
{
	if (nullptr == m_pConstantBuffer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Bind_ConstantBuffer", "Null Exception: m_pConstantBuffer_Manager");
	}

	return m_pConstantBuffer_Manager->Bind_Buffer(_eShaderStage, _iSlot);
}

HRESULT CGameInstance::Release_ConstantBuffer(SHADER_STAGE _eShaderStage, _uint _iSlot)
{
	if (nullptr == m_pConstantBuffer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Create_ConstantBuffer", "Null Exception: m_pConstantBuffer_Manager");
	}

	return m_pConstantBuffer_Manager->Release_Buffer(_eShaderStage, _iSlot);
}

#pragma endregion
#pragma region Sound Manager

#pragma endregion
#pragma region Timer Manager

void CGameInstance::Tick_Timer()
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Tick();
}

HRESULT CGameInstance::Add_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Add_Timer(_fFPS);
}

HRESULT CGameInstance::Delete_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Delete_Timer(_fFPS);
}

_bool CGameInstance::Check_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Check_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Check_Timer(_fFPS);
}

shared_ptr<class CTimer> CGameInstance::Find_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Find_Timer(_fFPS);
}

_float CGameInstance::Get_TimeDelta(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeDelta", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Get_TimeDelta(_fFPS);
}

HRESULT CGameInstance::Set_Default_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Set_Default_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Set_Default_Timer(_fFPS);
}

HRESULT CGameInstance::Set_TimeScale(const _float _fFPS, _float fTimeScale)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Get_TimeDelta", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Set_TimeScale(_fFPS, fTimeScale);
}

HRESULT CGameInstance::Set_TimeScale(_float fTimeScale)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Get_TimeDelta", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Set_TimeScale(fTimeScale);
}

_float CGameInstance::Get_PhysXTimeScale()
{
	if (nullptr == m_pPhysXEngine)
	{
		MSG_RETURN(0.f, "CGameInstance::Set_PhysXTimeScale", "Null Exception: PhysXEngine");
	}
	return	m_pPhysXEngine->Get_TimeScale();
}

HRESULT CGameInstance::Set_PhysXTimeScale(_float fTImeScale)
{
	if (nullptr == m_pPhysXEngine)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Set_PhysXTimeScale", "Null Exception: PhysXEngine");
	}
	m_pPhysXEngine->Set_TimeScale(fTImeScale);

	return S_OK;
}

_float CGameInstance::Get_TimeScale(const _float fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeScale", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Get_TimeScale(fFPS);
}

_float CGameInstance::Get_Effect_TimeScale()
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeScale", "Null Exception: m_pTimer_Manager");
	}

	return m_pObject_Manager->Get_Effect_TimeScale();
}

void CGameInstance::Set_Effect_TimeScale(_float f)
{
	m_pObject_Manager->Set_Effect_TimeScale(f);
}

#pragma endregion
#pragma region Mouse Manager

POINT CGameInstance::Get_CursorPos()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(POINT{}, "CGameInstance::Get_CursorPos", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Get_CursorPos();
}

POINT CGameInstance::Get_CursorMove()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(POINT{}, "CGameInstance::Get_CursorMove", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Get_CursorMove();
}

POINTS CGameInstance::Get_CursorScroll()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(POINTS{}, "CGameInstance::Get_CursorScroll", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Get_CursorScroll();
}

_bool CGameInstance::Is_CursorOn()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Is_CursorOn", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Is_CursorOn();
}

void CGameInstance::Fix_Cursor(_bool _bFix)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Fix_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Fix_Cursor(_bFix);
}

void CGameInstance::Show_Cursor(_bool _bShow)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Show_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Show_Cursor(_bShow);
}

void CGameInstance::Real_Show_Cursor(_bool _bShow)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Show_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Real_Show_Cursor(_bShow);
}

void CGameInstance::Toggle_Cursor()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Toggle_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Toggle_Cursor();
}

void CGameInstance::CheckFocus_OnMouse(_bool _bCheck)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::CheckFocus_OnMouse", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->CheckFocus(_bCheck);
}

#pragma endregion
#pragma region Key Manager

_bool CGameInstance::Key_None(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_None() : m_pKey_Manager->Key_None(_iKey);
}

_bool CGameInstance::Key_Down(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Down() : m_pKey_Manager->Key_Down(_iKey);
}

_bool CGameInstance::Key_Hold(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Hold", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Hold() : m_pKey_Manager->Key_Hold(_iKey);
}

_bool CGameInstance::Key_Up(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Up() : m_pKey_Manager->Key_Up(_iKey);
}

void CGameInstance::CheckFocus_OnKeyboard(_bool _bCheck)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(, "CGameInstance::CheckFocus_OnKeyboard", "Null Exception: m_pKey_Manager");
	}

	return m_pKey_Manager->CheckFocus(_bCheck);
}

#pragma endregion
#pragma region Event Handler

void CGameInstance::Register_OnTickListener(shared_ptr<void> _pSubscriber, function<_bool(_float)> _funcListener)
{
	if (nullptr == m_pEvent_Handler)
	{
		MSG_RETURN(, "CGameInstance::Register_OnTickListener", "Null Exception: m_pEvent_Handler");
	}

	return m_pEvent_Handler->Register_OnTickListener(_pSubscriber, _funcListener);
}

_bool CGameInstance::Exist_Event(shared_ptr<void> _pSubscriber)
{
	if (nullptr == m_pEvent_Handler)
	{
		MSG_RETURN(false, "CGameInstance::Exist_Event", "Null Exception: m_pEvent_Handler");
	}

	return m_pEvent_Handler->Exist_Event(_pSubscriber);
}

#pragma endregion
#pragma region Scene Manager

HRESULT CGameInstance::Open_Scene(SCENE _eScene, shared_ptr<class CScene> _pScene, _bool bOnLoading)
{
	if (nullptr == m_pScene_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Open_Scene", "Null Exception: m_pScene_Manager");
	}
	
	return m_pScene_Manager->Open_Scene(_eScene, _pScene, bOnLoading);
}

const SCENE CGameInstance::Current_Scene(_bool _bSkipLoad)
{
	if (nullptr == m_pScene_Manager)
	{
		MSG_RETURN(m_pScene_Manager->Max_Scene(), "CGameInstance::Current_Scene", "Null Exception: m_pScene_Manager");
	}

	return m_pScene_Manager->Current_Scene(_bSkipLoad);
}

shared_ptr<CScene> CGameInstance::Get_Current_Scene_Ptr()
{
	return m_pScene_Manager->Get_Current_Scene_Ptr();
}

#pragma endregion
#pragma region Object Manager

HRESULT CGameInstance::Add_Object_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<class CGameObject> _pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Object_Prototype", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Prototype(_eScene, _wstrPrototypeTag, _pPrototype);
}

shared_ptr<class CGameObject> CGameInstance::Clone_GameObject(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_GameObject", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Clone_GameObject(_eScene, _wstrPrototypeTag, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Add_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Layer(_eScene, _wstrLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Add_Pool(const SCENE _eScene, const wstring& _wstrPoolTag, const wstring& _wstrPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Pool(_eScene, _wstrPoolTag, _wstrPrototypeTag, _iPoolSize, _arg);
}

shared_ptr<class CObjectPool> CGameInstance::Add_Pool(const SCENE _eScene, const wstring& _wstrPoolTag, shared_ptr<class CGameObject> _pPrototype, _uint _iPoolSize, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Pool(_eScene, _wstrPoolTag, _pPrototype, _iPoolSize, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Find_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Layer(_eScene, _wstrLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Find_Pool(const SCENE _eScene, const wstring& _wstrPoolTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Pool(_eScene, _wstrPoolTag);
}

void CGameInstance::Iterate_Layers(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<class CObjectLayer>>)> _funcCallback)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Iterate_Layers(_eScene, _funcCallback);
}

void CGameInstance::Iterate_Pools(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<class CObjectPool>>)> _funcCallback)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Iterate_Pools(_eScene, _funcCallback);
}

HRESULT CGameInstance::Set_TimeScale(const SCENE _eScene, const wstring& wstrLayerTag, _float fTimeScale)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Set_TimeScale", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Set_TimeScale(_eScene, wstrLayerTag, fTimeScale);
}

_float CGameInstance::Get_TimeScale(const SCENE _eScene, const wstring& wstrLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeScale", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Get_TimeScale(_eScene, wstrLayerTag);
}

void CGameInstance::Set_HitLag(_bool b)
{
	return m_pObject_Manager->Set_HitLag(b);
}

_bool CGameInstance::Get_HitLag()
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeScale", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Get_HitLag();
}

void CGameInstance::Set_LagCoolTime(_float f)
{
	m_pObject_Manager->Set_LagCoolTime(f);
}

void CGameInstance::Set_LagTime(_float f)
{
	m_pObject_Manager->Set_LagTime(f);
}

void CGameInstance::Execute_Lag(_float f)
{
	//m_pObject_Manager->Execute_Lag(f);
}

#pragma endregion
#pragma region Collision Manager

void CGameInstance::Reset_CollisionGroupCheck()
{
	if (nullptr == m_pCollision_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pCollision_Manager");
	}

	return m_pCollision_Manager->Reset_GroupCheck();
}

void CGameInstance::Check_CollisionGroup(COLLISION_GROUP _eGroupA, COLLISION_GROUP _eGroupB, _bool _bCheck)
{
	if (nullptr == m_pCollision_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pCollision_Manager");
	}

	return m_pCollision_Manager->Check_Group(_eGroupA, _eGroupB, _bCheck);
}

void CGameInstance::Register_Collider(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pObject, shared_ptr<CCollider> _pCollider)
{
	if (nullptr == m_pCollision_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pCollision_Manager");
	}

	return m_pCollision_Manager->Register_Collider(_eGroup, _pObject, _pCollider);
}

void CGameInstance::Delete_Collider(shared_ptr<CGameObject> _pObject, _float _fTimeDelta)
{
	if (nullptr == m_pCollision_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pCollision_Manager");
	}

	return m_pCollision_Manager->Delete_Collider(_pObject, _fTimeDelta);
}

#pragma endregion
#pragma region Component Manager

HRESULT CGameInstance::Add_Component_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Component_Prototype", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Add_Prototype(_eScene, _wstrPrototypeTag, _pPrototype);
}

shared_ptr<CComponent> CGameInstance::Clone_Component(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Clone_Component(_eScene, _wstrPrototypeTag, _arg);
}

#pragma endregion
#pragma region Grid Manager

_float3 CGameInstance::Get_GridSize()
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_float3{}, "CGameInstance::Get_GridSize", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Get_GridSize();
}

HRESULT CGameInstance::Register_VIBuffer(const SCENE _eScene, const wstring& _wstrGridLayerTag, shared_ptr<CGameObject> _pObject)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Register_VIBuffer", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Register_VIBuffer(_eScene, _wstrGridLayerTag, _pObject);
}

HRESULT CGameInstance::Reset_Grids(const SCENE _eScene, const wstring& _wstrGridLayerTag)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Reset_Grids", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Reset_Grids(_eScene, _wstrGridLayerTag);
}

_float3 CGameInstance::Raycast(_vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange, _float3* _pNormal)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_vRayOrigin, "CGameInstance::Raycast", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Raycast(_vRayOrigin, _vRayDirection, _fRange, _pNormal);
}

_float3 CGameInstance::Raycast(const wstring& _wstrGridLayerTag, _vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange, _float3* _pNormal)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_vRayOrigin, "CGameInstance::Raycast", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Raycast(_wstrGridLayerTag, _vRayOrigin, _vRayDirection, _fRange, _pNormal);
}

#pragma endregion
#pragma region Light Manager

shared_ptr<CLight> CGameInstance::Add_Light(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform, shared_ptr<CGameObject> _pObject)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Light", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Add_Light(_eScene, _tLightDesc, _pTransform, _pObject);
}

HRESULT CGameInstance::Erase_Light(shared_ptr<CLight> _pLight)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Erase_Light", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Erase_Light(_pLight);
}

HRESULT CGameInstance::Clear_Lights(const SCENE _eScene)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_Light", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Clear_Lights(_eScene);
}

void CGameInstance::Enable_SSAO(_bool bEnable)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	m_pLight_Manager->Enable_SSAO(bEnable);
}

_bool CGameInstance::Enable_SSAO()
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Enable_SSAO();
}

void CGameInstance::Enable_Lensflare(_bool bEnable)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	m_pLight_Manager->Enable_Lensflare(bEnable);
}

_bool CGameInstance::Enable_Lensflare()
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Enable_Lensflare();
}

_float3 CGameInstance::Get_SunPosition()
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Get_SunPosition();
}

void CGameInstance::Set_SunPosition(_float3 vPosition)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	m_pLight_Manager->Set_SunPosition(vPosition);
}

_float3 CGameInstance::Get_SunColor()
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Get_SunColor();
}

void CGameInstance::Set_SunColor(_float3 vColor)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	m_pLight_Manager->Set_SunColor(vColor);
}

_float CGameInstance::Get_LensflareStrength()
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Get_LensflareStrength();
}

void CGameInstance::Set_LensflareStrength(_float fStrength)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Set_LensflareStrength(fStrength);
}

void CGameInstance::Set_Reflection(shared_ptr<class CTexture> pReflection)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	m_pLight_Manager->Set_Reflection(pReflection);
}

HRESULT CGameInstance::Bind_Reflection(shared_ptr<class CShader> pShader)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Bind_Reflection(pShader);
}

HRESULT CGameInstance::Bind_LightDirection(SCENE eScene, shared_ptr<class CShader> pShader)
{
	assert(m_pLight_Manager && "CGameInstance::Clear_Light Null Exception: m_pLight_Manager");

	return m_pLight_Manager->Bind_LightDirection(eScene, pShader);
}

#ifdef _DEBUG
void CGameInstance::Iterate_Shadows(function<void(shared_ptr<CLight>)> _fpListener)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Shadows", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Iterate_Shadows(_fpListener);
}

ComPtr<ID3D11ShaderResourceView> CGameInstance::Get_Shadow(shared_ptr<CLight> _pLight)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Iterate_Shadows", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Get_Shadow(_pLight);
}
#endif

#pragma endregion

#pragma region Postprocess
void CGameInstance::FadeInExposure(IMAGE_PROCESS eImageProcess, _float fEndExposure, _float fTimeScale)
{
	assert(m_pImageProcess && "CGameInstance::FadeInExposure Null Exception: m_pImageProcess");

	return m_pImageProcess->FadeInExposure(eImageProcess, fEndExposure, fTimeScale);
}

void CGameInstance::FadeInGamma(IMAGE_PROCESS eImageProcess, _float fEndGamma, _float fTimeScale)
{
	assert(m_pImageProcess && "CGameInstance::FadeInGamma Null Exception: m_pImageProcess");

	return m_pImageProcess->FadeInGamma(eImageProcess, fEndGamma, fTimeScale);
}

void CGameInstance::FadeOutExposure(IMAGE_PROCESS eImageProcess, _float fEndExposure, _float fTimeScale)
{
	assert(m_pImageProcess && "CGameInstance::FadeOutExposure Null Exception: m_pImageProcess");

	return m_pImageProcess->FadeOutExposure(eImageProcess, fEndExposure, fTimeScale);
}

void CGameInstance::FadeOutGamma(IMAGE_PROCESS eImageProcess, _float fEndGamma, _float fTimeScale)
{
	assert(m_pImageProcess && "CGameInstance::FadeOutGamma Null Exception: m_pImageProcess");

	return m_pImageProcess->FadeOutGamma(eImageProcess, fEndGamma, fTimeScale);
}

void CGameInstance::Set_Exposure(IMAGE_PROCESS eImageProcess, _float fExposure)
{
	assert(m_pImageProcess && "CGameInstance::Set_Exposure Null Exception: m_pImageProcess");

	return m_pImageProcess->Set_Exposure(eImageProcess, fExposure);
}

void CGameInstance::Set_Gamma(IMAGE_PROCESS eImageProcess, _float fGamma)
{
	assert(m_pImageProcess && "CGameInstance::Set_Gamma Null Exception: m_pImageProcess");

	return m_pImageProcess->Set_Gamma(eImageProcess, fGamma);
}

void CGameInstance::Set_BloomStrength(IMAGE_PROCESS eImageProcess, _float fStrength)
{
	assert(m_pImageProcess && "CGameInstance::Set_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Set_BloomStrength(eImageProcess, fStrength);
}

_float CGameInstance::Get_Exposure(IMAGE_PROCESS eImageProcess)
{
	assert(m_pImageProcess && "CGameInstance::Get_Exposure Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_Exposure(eImageProcess);
}

_float CGameInstance::Get_Gamma(IMAGE_PROCESS eImageProcess)
{
	assert(m_pImageProcess && "CGameInstance::Get_Gamma Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_Gamma(eImageProcess);
}

_float CGameInstance::Get_BloomStrength(IMAGE_PROCESS eImageProcess)
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_BloomStrength(eImageProcess);
}

void CGameInstance::Enable_LUTFilter(_bool bEnable)
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Enable_LUTFilter(bEnable);
}

_bool CGameInstance::Enable_LUTFilter()
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Enable_LUTFilter();
}

void CGameInstance::Set_LUTIndex(_uint iLUTIndex)
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Set_LUTIndex(iLUTIndex); 
}

_uint CGameInstance::Get_LUTIndex()
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_LUTIndex();
}

void CGameInstance::MaskingLUT(_bool bMasking, shared_ptr<class CTexture> pMaskingTexture)
{
	assert(m_pImageProcess && "CGameInstance::Get_BloomStrength Null Exception: m_pImageProcess");

	return m_pImageProcess->MaskingLUT(bMasking, pMaskingTexture);
}

void CGameInstance::Enable_FXAA(_bool bEnable)
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	m_pImageProcess->Enable_FXAA(bEnable);
}

_bool CGameInstance::Enable_FXAA()
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	return m_pImageProcess->Enable_FXAA();
}

void CGameInstance::Enable_DOF(_bool bEnable)
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	m_pImageProcess->Enable_DOF(bEnable);
}

_bool CGameInstance::Enable_DOF()
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	return m_pImageProcess->Enable_DOF();
}

void CGameInstance::Set_DOF_DepthStart(_float fDepthStart)
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	m_pImageProcess->Set_DOF_DepthStart(fDepthStart);
}

void CGameInstance::Set_DOF_DepthRange(_float fDepthRange)
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	m_pImageProcess->Set_DOF_DepthRange(fDepthRange);
}

_float CGameInstance::Get_DOF_DepthStart()
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_DOF_DepthStart();
}

_float CGameInstance::Get_DOF_DepthRange()
{
	assert(m_pImageProcess && "CGameInstance::Enable_FXAA Null Exception: m_pImageProcess");

	return m_pImageProcess->Get_DOF_DepthRange();
}

#pragma endregion

PxController* CGameInstance::CreateController(PxCapsuleControllerDesc* desc)
{
	return m_pPhysXEngine->CreateController(desc);
}
void CGameInstance::ClearPhysX()
{
	return m_pPhysXEngine->Clear();
}
#ifdef _DEBUG
HRESULT CGameInstance::SetRenderer()
{
	return m_pPhysXEngine->SetRenderer();
}

void CGameInstance::SetPhysxDrawStatus(_bool Render)
{
	return m_pPhysXEngine->SetDrawStatus(Render);
}
#endif // DEBUG

#pragma region InteractiveObjectFinder
std::shared_ptr<CInteractiveObject> CGameInstance::Get_FocusedObject()
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Get_FocusedObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Get_FocusedObject();
}

std::shared_ptr<class CInteractiveObject> CGameInstance::Get_FocusedSpecialObject()
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Get_FocusedSpecialObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Get_FocusedSpecialObject();
}
void CGameInstance::Set_SpecialOnly(_bool b)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Get_FocusedSpecialObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Set_SpecialOnly(b);
}
void CGameInstance::Add_InteractiveObject(const std::shared_ptr<CInteractiveObject>& _pInteractiveObject)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Add_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Add_InteractiveObject(_pInteractiveObject);
}
void CGameInstance::Add_InteractiveSpecialObject(const std::shared_ptr<class CInteractiveObject>& _pInteractiveObject)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Add_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Add_InteractiveSpecialObject(_pInteractiveObject);
}
HRESULT CGameInstance::Append_InteractiveObject(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Append_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Append_InteractiveObject(_eScene, _wstrLayerTag);
}
HRESULT CGameInstance::Append_InteractiveSpecialObject(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Append_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Append_InteractiveSpecialObject(_eScene, _wstrLayerTag);
}

void CGameInstance::Clear_InteractiveObjects()
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Append_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->Clear();
}

void CGameInstance::Erase_InteractiveObject(shared_ptr<CInteractiveObject> _pobj)
{
	assert(m_pInteractiveObjectFinder && "CGameInstance::Append_InteractiveObject Null Exception: m_pInteractiveObjectFinder");

	return m_pInteractiveObjectFinder->EraseObject(_pobj);
}

void CGameInstance::PlaySound(const wstring& pSoundKey, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::PlaySound", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->PlaySound(pSoundKey, fVolume);
}

void CGameInstance::PlaySound_BrainTalk(CSound_Manager::CHANNELID eId, const wstring& pSoundKey, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::PlaySound_BrainTalk", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->PlaySound_BrainTalk(eId, pSoundKey, fVolume);
}

void CGameInstance::PlaySoundeffect(const wstring& pSoundKey, CSound_Manager::CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::PlaySoundeffect", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->PlaySoundeffect(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(const wstring& pSoundKey, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::PlayBGM", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(CSound_Manager::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::StopSound", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::StopAll", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CSound_Manager::CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::SetChannelVolume", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->SetChannelVolume(eID, fVolume);
}

bool CGameInstance::isplaying(CSound_Manager::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(false, "CGameInstance::isplaying", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->isplaying(eID);
}

void CGameInstance::SetPitch(_uint eID, _float fPitch)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::SetPitch", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->SetPitch(eID, fPitch);
}

#ifdef _DEBUG

void CGameInstance::Mute(_bool _bMute)
{
	if (nullptr == m_pSound_Manager)
	{
		MSG_RETURN(, "CGameInstance::Mute", "Null Exception: m_pSound_Manager");
	}
	return m_pSound_Manager->Mute(_bMute);
}
#endif
HRESULT CGameInstance::Add_Font(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const wstring& strFontTag, const wstring& strFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice.Get(), pContext.Get(), strFontTag, strFontFilePath);
}
HRESULT CGameInstance::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(strFontTag, strText, vPos, vColor, fScale, fRotation);
}

void CGameInstance::Play_Dialogue(_uint _iScriptIndex, const wstring& _strName, const wstring& _strText1, const wstring& _strText2, _float _fDuration)
{
	if (nullptr == m_pDialogue_Manager)
		MSG_RETURN(, "CGameInstance::Add_Dialogue", "Null Exception : m_pDialogue_Manager");

	return m_pDialogue_Manager->Play_Dialogue(_iScriptIndex, _strName, _strText1, _strText2, _fDuration);
}


#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Destroy_Instance();
	CPipeLine::Destroy_Instance();
	CPicker::Destroy_Instance();
	CLight_Manager::Destroy_Instance();
	CGrid_Manager::Destroy_Instance();
	CSound_Manager::Destroy_Instance();
	CComponent_Manager::Destroy_Instance();
	CObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
	CRenderTarget_Manager::Destroy_Instance();
	CKey_Manager::Destroy_Instance();
	CMouse_Manager::Destroy_Instance();
	CTimer_Manager::Destroy_Instance();
	CPhysXEngine::Destroy_Instance();
	CImageProcess_Manager::Destroy_Instance();
	CInteractiveObjectFinder::Destroy_Instance();
	CResourceManager::Destroy_Instance();
	CSceneCapture::Destroy_Instance();
	CFont_Manager::Destroy_Instance();
	CDialogue_Manager::Destroy_Instance();
	CFrustom::Destroy_Instance();
	CComputeAccelerator::Destroy_Instance();

#ifdef _DEBUG
#if ACTIVATE_CONSOLE
	FreeConsole();
#endif
#elif FORCE_CONSOLE
#if ACTIVATE_CONSOLE
	FreeConsole();
#endif
#endif
}
