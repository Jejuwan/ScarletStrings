#pragma once
#include "Engine_Define.h"
#include "PipeLine.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "System.h"
#include "CSound_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CSingleton<CGameInstance>
{
private:
	explicit CGameInstance();
	virtual ~CGameInstance() DEFAULT;

public:
#pragma region Engine

	HRESULT										Initialize_Engine(_Out_ ComPtr<ID3D11Device>&, _Out_ ComPtr<ID3D11DeviceContext>&, _In_ const GRAPHICDESC, _In_ const SCENE eStaticScene, _In_ const SCENE eMaxScene, _In_ const COLLISION_GROUP eMaxGroup, _In_ const SOUND_CHANNEL eMaxChannel);
	void										Tick_Engine(_float fTimeDelta);
#ifdef _DEBUG
	void										Debug_Engine();
#endif

	LRESULT										WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	_float										Get_ActivatedTime() const;
	GRAPHICDESC									Get_GraphicDesc() const;

	const string								AbsolutePath(const string& strRelativePath);
	const wstring								AbsolutePath(const wstring& wstrRelativePath);
	const string								RelativePath(const string& strAbsolutePath);
	const wstring								RelativePath(const wstring& wstrAbsolutePath);

#pragma endregion
#pragma region Graphic Device

	HRESULT										Clear_BackBuffer_View(_color vBackBuffer);
	HRESULT										Clear_DepthStencil_View();
	HRESULT										Present();

#pragma endregion
#pragma region RenderTarget Manager

	ComPtr<ID3D11ShaderResourceView>			Get_RenderTarget_ShaderResourceView(const wstring& wstrTargetTag);
	void										Copy_From_BackBuffer(const wstring& wstrTargetTag);
//	void										Enable_SSAO(_bool bEnable);

#pragma endregion
#pragma region ConstsantBuffer Manager

	HRESULT										Create_ConstantBuffer(SHADER_STAGE, _uint iSlot, const size_t nLength, _bool bReleaseBuffer = false);
	HRESULT										Update_ConstantBuffer(SHADER_STAGE, _uint iSlot, void* pData, size_t nLength = 0, _uint iOffset = 0);
	HRESULT										Bind_ConstantBuffer(SHADER_STAGE, _uint iSlot);
	HRESULT										Release_ConstantBuffer(SHADER_STAGE, _uint iSlot);

#pragma endregion
#pragma region LUT Manager



#pragma endregion
#pragma region Sound Manager

#pragma endregion
#pragma region Timer Manager

	void										Tick_Timer();

	HRESULT										Add_Timer(const _float fFPS);
	HRESULT										Delete_Timer(const _float fFPS);
	_bool										Check_Timer(const _float fFPS);
	shared_ptr<class CTimer>					Find_Timer(const _float fFPS);
	_float										Get_TimeDelta(const _float fFPS);

	HRESULT										Set_Default_Timer(const _float fFPS);
	
	HRESULT										Set_TimeScale(const _float fFPS, _float fTimeScale);
	HRESULT										Set_TimeScale(_float fTimeScale);
	_float										Get_PhysXTimeScale();
	HRESULT										Set_PhysXTimeScale(_float fTImeScale);
	_float										Get_TimeScale(const _float fFPS);
	_float										Get_Effect_TimeScale();
	void										Set_Effect_TimeScale(_float f);
#pragma endregion
#pragma region Mouse Manager

	POINT										Get_CursorPos();
	POINT										Get_CursorMove();
	POINTS										Get_CursorScroll();
	_bool										Is_CursorOn();

	void										Fix_Cursor(_bool bFix = true);
	void										Show_Cursor(_bool bShow = true);
	void										Real_Show_Cursor(_bool bShow = true);
	void										Toggle_Cursor();

	void										CheckFocus_OnMouse(_bool bCheck = true);

#pragma endregion
#pragma region Key Manager

	_bool										Key_None(_uint iKey = VK_MAX);
	_bool										Key_Down(_uint iKey = VK_MAX);
	_bool										Key_Hold(_uint iKey = VK_MAX);
	_bool										Key_Up(_uint iKey = VK_MAX);

	void										CheckFocus_OnKeyboard(_bool bCheck = true);

#pragma endregion
#pragma region Event Handler

	void										Register_OnTickListener(shared_ptr<void>, function<_bool(_float fTimeDelta)>);
	_bool										Exist_Event(shared_ptr<void>);

#pragma endregion
#pragma region Scene Manager

	HRESULT										Open_Scene(SCENE, shared_ptr<class CScene>, _bool bOnLoading = true);
	const SCENE									Current_Scene(_bool bSkipLoad = true);
	shared_ptr<CScene>							Get_Current_Scene_Ptr();
#pragma endregion
#pragma region Object Manager

	HRESULT										Add_Object_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<class CGameObject> pPrototype);
	shared_ptr<class CGameObject>				Clone_GameObject(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);
	template<typename T>
	shared_ptr<T>								Clone_GameObject(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);

	shared_ptr<class CObjectLayer>				Add_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<class CObjectPool>				Add_Pool(const SCENE, const wstring& wstrPoolTag, const wstring& wstrPrototypeTag, _uint iPoolSize, any = g_aNull);
	shared_ptr<class CObjectPool>				Add_Pool(const SCENE, const wstring& wstrPoolTag, shared_ptr<class CGameObject> pPrototype, _uint iPoolSize, any = g_aNull);

	shared_ptr<class CObjectLayer>				Find_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<class CObjectPool>				Find_Pool(const SCENE, const wstring& wstrPoolTag);

	void										Iterate_Layers(const SCENE, function<_bool(pair<wstring, shared_ptr<class CObjectLayer>>)>);
	void										Iterate_Pools(const SCENE, function<_bool(pair<wstring, shared_ptr<class CObjectPool>>)>);

	HRESULT										Set_TimeScale(const SCENE, const wstring& wstrLayerTag, _float fTimeScale);
	_float										Get_TimeScale(const SCENE, const wstring& wstrLayerTag);

	public:
		void									Set_HitLag(_bool b);
		_bool									Get_HitLag();
		void									Set_LagCoolTime(_float f);
		void									Set_LagTime(_float f);
		void									Execute_Lag(_float f);
#pragma endregion
#pragma region Collision Manager

	void										Reset_CollisionGroupCheck();
	void										Check_CollisionGroup(COLLISION_GROUP, COLLISION_GROUP, _bool bCheck = true);

	void										Register_Collider(COLLISION_GROUP, shared_ptr<class CGameObject>, shared_ptr<class CCollider>);
	void										Delete_Collider(shared_ptr<class CGameObject>, _float fTimeDelta = 0.f);

#pragma endregion
#pragma region Component Manager

	HRESULT										Add_Component_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<class CComponent> pPrototype);
	shared_ptr<class CComponent>				Clone_Component(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);
	template<typename T>
	shared_ptr<T>								Clone_Component(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);

#pragma endregion
#pragma region Grid Manager

	_float3										Get_GridSize();

	HRESULT										Register_VIBuffer(const SCENE, const wstring& wstrGridLayerTag, shared_ptr<class CGameObject>);
	HRESULT										Reset_Grids(const SCENE, const wstring& wstrGridLayerTag = wstring());

	_float3										Raycast(_vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX, _float3* pNormal = nullptr);
	_float3										Raycast(const wstring& wstrGridLayerTag, _vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX, _float3* pNormal = nullptr);

#pragma endregion
#pragma region Light Manager

	shared_ptr<class CLight>					Add_Light(const SCENE, LIGHTDESC, shared_ptr<class CTransform>, shared_ptr<class CGameObject> = nullptr);
	HRESULT										Erase_Light(shared_ptr<class CLight>);
	HRESULT										Clear_Lights(const SCENE);

	void										Enable_SSAO(_bool bEnable);
	_bool										Enable_SSAO();

	void										Enable_Lensflare(_bool bEnable);
	_bool										Enable_Lensflare();

	_float3										Get_SunPosition();
	void										Set_SunPosition(_float3 vPosition);

	_float3										Get_SunColor();
	void										Set_SunColor(_float3 vColor);

	_float										Get_LensflareStrength();
	void										Set_LensflareStrength(_float fStrength);

	void										Set_Reflection(shared_ptr<class CTexture> pReflection);
	HRESULT										Bind_Reflection(shared_ptr<class CShader> pShader);
	HRESULT										Bind_LightDirection(SCENE, shared_ptr<class CShader>);
#ifdef _DEBUG
	void										Iterate_Shadows(function<void(shared_ptr<class CLight>)>);
	ComPtr<ID3D11ShaderResourceView>			Get_Shadow(shared_ptr<class CLight>);
#endif

#pragma endregion
#pragma region Postprocess
	void										FadeInExposure(IMAGE_PROCESS, _float fEndExposure, _float fTimeScale);
	void										FadeInGamma(IMAGE_PROCESS, _float fEndGamma, _float fTimeScale);
	void										FadeOutExposure(IMAGE_PROCESS, _float fEndExposure, _float fTimeScale);
	void										FadeOutGamma(IMAGE_PROCESS, _float fEndGamma, _float fTimeScale);

	void										Set_Exposure(IMAGE_PROCESS, _float fExposure);
	void										Set_Gamma(IMAGE_PROCESS, _float fGamma);
	void										Set_BloomStrength(IMAGE_PROCESS, _float fStrength);

	_float										Get_Exposure(IMAGE_PROCESS);
	_float										Get_Gamma(IMAGE_PROCESS);
	_float										Get_BloomStrength(IMAGE_PROCESS);

	void										Enable_LUTFilter(_bool bEnable);
	_bool										Enable_LUTFilter();
	void										Set_LUTIndex(_uint iLUTIndex);
	_uint										Get_LUTIndex();

	void										MaskingLUT(_bool bMasking, shared_ptr<class CTexture> = nullptr);

	void										Enable_FXAA(_bool bEnable);
	_bool										Enable_FXAA();

	void										Enable_DOF(_bool);
	_bool										Enable_DOF();

	void										Set_DOF_DepthStart(_float fDepthStart);
	void										Set_DOF_DepthRange(_float fDepthRange);

	_float										Get_DOF_DepthStart();
	_float										Get_DOF_DepthRange();
#pragma endregion

#pragma region PhysX
	PxController* CreateController(PxCapsuleControllerDesc* desc);
	void ClearPhysX();
#ifdef _DEBUG
	HRESULT SetRenderer();
	void SetPhysxDrawStatus(_bool Render);
#endif // _DEBUG


#pragma region InteractiveObjectFinder
	std::shared_ptr<class CInteractiveObject>	Get_FocusedObject();
	std::shared_ptr<class CInteractiveObject>	Get_FocusedSpecialObject();
	void										Set_SpecialOnly(_bool b);

	void										Add_InteractiveObject(const std::shared_ptr<class CInteractiveObject>&);
	void										Add_InteractiveSpecialObject(const std::shared_ptr<class CInteractiveObject>&);
	HRESULT										Append_InteractiveObject(const SCENE _eScene, const wstring& _wstrLayerTag);
	HRESULT										Append_InteractiveSpecialObject(const SCENE _eScene, const wstring& _wstrLayerTag);

	void										Clear_InteractiveObjects();
	void										Erase_InteractiveObject(shared_ptr<CInteractiveObject> _pobj);
#pragma endregion
#pragma region Sound Manager
	void										PlaySound(const wstring& pSoundKey, float fVolume);
	void										PlaySound_BrainTalk(CSound_Manager::CHANNELID eId, const wstring& pSoundKey, float fVolume);
	void										PlaySoundeffect(const wstring& pSoundKey, CSound_Manager::CHANNELID eID, float fVolume);
	void										PlayBGM(const wstring& pSoundKey, float fVolume);
	void										StopSound(CSound_Manager::CHANNELID eID);
	void										StopAll();
	void										SetChannelVolume(CSound_Manager::CHANNELID eID, float fVolume);
	bool										isplaying(CSound_Manager::CHANNELID eid);
	void										SetPitch(_uint eid, _float fPitch);
#ifdef _DEBUG
	void										Mute(_bool = true);
#endif
#pragma endregion
#pragma region Font Manager
	HRESULT										Add_Font(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT										Render_Font(const wstring& strFontTag, const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation = 0.f);
#pragma endregion
#pragma region Dialogue Mananger
	void										Play_Dialogue(_uint _iScriptIndex, const wstring& _strName, const wstring& _strText1 = L"", const wstring& _strText2 = L"", _float _fDuration = 0.f);

#pragma endregion


private:
	shared_ptr<class CGraphicDevice>			m_pGraphic_Device;
	shared_ptr<class CRenderTarget_Manager>		m_pRenderTarget_Manager;
	shared_ptr<class CConstantBuffer_Manager>	m_pConstantBuffer_Manager;
	shared_ptr<class CSound_Manager>			m_pSound_Manager;
	shared_ptr<class CTimer_Manager>			m_pTimer_Manager;
	shared_ptr<class CMouse_Manager>			m_pMouse_Manager;
	shared_ptr<class CKey_Manager>				m_pKey_Manager;
	shared_ptr<class CEvent_Handler>			m_pEvent_Handler;
	shared_ptr<class CScene_Manager>			m_pScene_Manager;
	shared_ptr<class CObject_Manager>			m_pObject_Manager;
	shared_ptr<class CCollision_Manager>		m_pCollision_Manager;
	shared_ptr<class CComponent_Manager>		m_pComponent_Manager;
	shared_ptr<class CGrid_Manager>				m_pGrid_Manager;
	shared_ptr<class CLight_Manager>			m_pLight_Manager;
	shared_ptr<class CPicker>					m_pPicker;
	shared_ptr<class CPhysXEngine>				m_pPhysXEngine;
	shared_ptr<class CInteractiveObjectFinder>	m_pInteractiveObjectFinder;
	shared_ptr<class CResourceManager>			m_pResource_Manager;
	shared_ptr<class CSceneCapture>				m_pSceneCapture;
	shared_ptr<class CFont_Manager>				m_pFont_Manager;
	shared_ptr<class CDialogue_Manager>			m_pDialogue_Manager;
	shared_ptr<class CImageProcess_Manager>		m_pImageProcess;
	shared_ptr<class CFrustom>					m_pFrustom;

private:
	GRAPHICDESC									m_tGraphicDesc{};

	_float										m_fTimeAcc	= 0.f;

public:
	static void									Release_Engine();

private:
	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

public:
	ComPtr<ID3D11Device>				Get_Device() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext>			Get_Context() { return m_pContext; }
	shared_ptr<CPhysXEngine>			Get_PhysXEngine() { return m_pPhysXEngine; }


	private:
		CGameObject* m_pPlayer = { nullptr };

	public:
		CGameObject* Get_Player() { return m_pPlayer; }
		void	     Set_Player(class CGameObject* pGameObj) { m_pPlayer = pGameObj; }

	friend CSingleton<CGameInstance>;
};

#include "GameInstance.inl"

END
