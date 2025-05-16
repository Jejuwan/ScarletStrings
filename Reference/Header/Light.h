#pragma once
#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class ENGINE_DLL CLight final : public ISystem
{
private:
	explicit CLight(const LIGHTDESC);
	virtual ~CLight() DEFAULT;

public:
	HRESULT											Initialize(const LIGHTDESC, shared_ptr<class CTransform>);
	void											Tick(_float fTimeDelta);

public:
	_bool											Is_Expired() const;
	const LIGHTDESC									Get_LightDesc() const					{ return m_tLightDesc; }
	const SHADOWDESC								Get_ShadowDesc() const					{ return m_tShadowDesc; }

	const _float4x4									Get_ShadowView() const					{ return m_mShadowView; }
	const _float4x4									Get_ShadowProjection() const			{ return m_mShadowProjection; }

	void											Set_LightDesc(LIGHTDESC tDesc);
	void											Set_ShadowDesc(SHADOWDESC tDesc)		{ m_tShadowDesc = tDesc; }

	HRESULT											Register_Layer(SCENE, const wstring& wstrLayerTag);

public:
	HRESULT											Render_ShadowDepth(shared_ptr<class CRenderer>);
	
	HRESULT											Bind_Light(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>, _bool bSSAO = false);
	HRESULT											Bind_ShadowMatrix(shared_ptr<class CShader>);

private:
	const LIGHTDESC									m_tInitialData{};
	LIGHTDESC										m_tLightDesc{};
	weak_ptr<class CTransform>						m_pTransform;

	_float4x4										m_mShadowView;
	_float4x4										m_mShadowProjection;

	list<weak_ptr<class CObjectLayer>>				m_lstLayer;
	SHADOWDESC										m_tShadowDesc{};

public:
	static shared_ptr<CLight>						Create(const LIGHTDESC, shared_ptr<class CTransform> = nullptr);

	friend class CLight_Manager;
};

END
