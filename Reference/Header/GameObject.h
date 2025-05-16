#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public ISystem
{
protected:
	explicit CGameObject(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGameObject(const CGameObject&);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT												Initialize_Prototype();
	virtual HRESULT												Initialize(any = g_aNull);
	virtual void												Tick(_float fTimeDelta);
	virtual void												Late_Tick(_float fTimeDelta);
	virtual HRESULT												Render()				PURE;

public:
	virtual HRESULT												Fetch(any = g_aNull);
	virtual _bool												Return();
	virtual HRESULT												Release();

	virtual HRESULT												Render_ShadowDepth(shared_ptr<class CLight>);
	virtual HRESULT												Render_OutLine();
	virtual HRESULT												Render_Bloom();
	virtual HRESULT												Render_Neon();
	virtual HRESULT												Render_Screen_Effect();

	virtual void												OnCollisionEnter(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta);
	virtual void												OnCollision(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta);
	virtual void												OnCollisionExit(COLLISION_GROUP, shared_ptr<CGameObject>, _float fTimeDelta);

	_bool														Is_CheckOut() const					{ return m_bIsCheckOut; }
	void														CheckOut(_bool bCheckOut = true)	{ m_bIsCheckOut = bCheckOut; }

public:
	template <typename T>
	shared_ptr<T>												Get_Component(const COMPONENT);
	shared_ptr<class CComponent>								Get_Component(const COMPONENT);
	template <typename T>
	shared_ptr<T>												Get_Behavior(const BEHAVIOR);
	shared_ptr<class CBehavior>									Get_Behavior(const BEHAVIOR);

	_bool														Has_Component(const COMPONENT);

public:
	virtual _bool												Get_Enable() { return m_bEnable; }
	virtual void												Set_Enable(_bool bEnable) { m_bEnable = bEnable; }
#ifdef _DEBUG
	void														Set_Material(MATERIALDESC _tMaterialDesc)	{ m_tMaterialDesc = _tMaterialDesc; }
#endif

protected:
	virtual HRESULT												Render(_uint iPassIndex, _bool bOrthographic = false);

	virtual HRESULT												Ready_Components();
	virtual HRESULT												Ready_Behaviors();

	virtual HRESULT												Add_Component(const COMPONENT, shared_ptr<class CComponent> = nullptr);
	virtual HRESULT												Add_Behavior(const BEHAVIOR, shared_ptr<class CBehavior> = nullptr);
	virtual HRESULT												Delete_Component(const COMPONENT);
	virtual HRESULT												Delete_Behavior(const BEHAVIOR);

	HRESULT														Add_RenderObject(const RENDER_GROUP);
	HRESULT														Add_RenderObject(const UI_LAYER);

protected:
	inline shared_ptr<CGameObject>								shared_from_gameobject();

protected:
	virtual HRESULT												Bind_Matarial(shared_ptr<class CShader> pShader);
	virtual HRESULT												Bind_Transform(shared_ptr<class CShader> pShader);
	virtual HRESULT												Bind_Transform_Orthographic(shared_ptr<class CShader> pShader);
	virtual HRESULT												Bind_Texture(shared_ptr<class CShader> pShader);
	virtual HRESULT												Bind_VIBuffer(shared_ptr<class CShader> pShader, _uint _iPassIndex);
	virtual HRESULT												Bind_Model(shared_ptr<class CShader> pShader, _uint _iPassIndex);

protected:
	ENTITYDESC													m_tEntityDesc{};
	MATERIALDESC												m_tMaterialDesc{};

	unordered_map<COMPONENT, shared_ptr<class CComponent>>		m_umapComponent;
	unordered_map<COMPONENT, pair<wstring, any>>				m_umapComponentArg;
	bitset<IDX(COMPONENT::MAX)>									m_bitComponent;

	unordered_map<BEHAVIOR, shared_ptr<class CBehavior>>		m_umapBehavior;
	unordered_map<BEHAVIOR, pair<wstring, any>>					m_umapBehaviorArg;
	bitset<IDX(BEHAVIOR::MAX)>									m_bitBehavior;

private:
	weak_ptr<class CRenderer>									m_pWeakRenderer;
	weak_ptr<class CTransform>									m_pWeakTransform;
	weak_ptr<class CShader>										m_pWeakShader;
	weak_ptr<class CModel>										m_pWeakModel;
	weak_ptr<class CVIBuffer>									m_pWeakVIBuffer;
	weak_ptr<class CTexture>									m_pWeakTexture;
	weak_ptr<class CCollider>									m_pWeakCollider;
	weak_ptr<class CKeyframe>									m_pWeakKeyframe;

protected:
	_bool														m_bEnable = { true };
	_bool														m_bAlive = { true };
private:
	_bool														m_bIsCheckOut			= false;
protected:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;

public:
	virtual shared_ptr<CGameObject>								Clone(any = g_aNull)	PURE;
	wstring m_strLayerName;
	void														Set_Alive(_bool b) { m_bAlive = b; }
};

#include "GameObject.inl"

END
