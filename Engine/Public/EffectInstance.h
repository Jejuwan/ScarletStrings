#pragma once
#include "Engine_Define.h"
#include "Effect.h"

BEGIN(Engine)

class ENGINE_DLL CEffectInstance abstract : public CEffect
{
protected:
	explicit CEffectInstance(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, EFFECT);
	explicit CEffectInstance(const CEffectInstance&);
	virtual ~CEffectInstance() DEFAULT;
	
public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override												PURE;

	virtual HRESULT							Fetch(any = g_aNull) override;

protected:
	virtual HRESULT							Ready_Components() override;

	virtual HRESULT							Render(_uint iPassIndex, _bool bOrthographic = false) override;

	virtual void							Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull)	PURE;
	virtual	void							Update_Instance(void*, _uint iNumInstance, _float fTimeDelta)	PURE;

	virtual HRESULT							Add_Component(const COMPONENT, shared_ptr<class CComponent>) override;

public:
#if ACTIVATE_TOOL
	const _uint								Get_ActivateInstances() const									{ return m_iActivateInstances; }

	void									Set_ActivateInstances(const _uint iNumInstance)					{ m_iActivateInstances = iNumInstance; }
#endif

protected:
	shared_ptr<class CTransform>			m_pTransform;
	shared_ptr<class CShader>				m_pShader;
	shared_ptr<class CVIBufferInstance>		m_pVIBufferInstance;

	_uint									m_iActivateInstances	= 0;
};

END
