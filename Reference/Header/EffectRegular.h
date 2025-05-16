#pragma once
#include "Engine_Define.h"
#include "Effect.h"

BEGIN(Engine)

class ENGINE_DLL CEffectRegular abstract : public CEffect
{
protected:
	explicit CEffectRegular(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, EFFECT);
	explicit CEffectRegular(const CEffectRegular&);
	virtual ~CEffectRegular() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any = g_aNull) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override							PURE;

	virtual HRESULT							Fetch(any vPosition3) override;

protected:
	virtual HRESULT							Ready_Components() override;

	virtual HRESULT							Render(_uint iPassIndex, _bool bOrthographic = false) override;

protected:
	shared_ptr<class CTransform>			m_pTransform;
	shared_ptr<class CShader>				m_pShader;
	shared_ptr<class CVIBuffer>				m_pVIBuffer;
};

END
