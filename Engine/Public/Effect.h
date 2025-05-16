#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, EFFECT);
	explicit CEffect(const CEffect&);
	virtual ~CEffect() DEFAULT;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(any = g_aNull) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			Late_Tick(_float fTimeDelta) override;

	virtual HRESULT			Fetch(any = g_aNull) override;

public:
	const EFFECT			Get_Type() const					{ return m_eEffect; }

#if ACTIVATE_TOOL
	const MATERIALDESC		Get_Material() const				{ return m_tMaterialDesc; }

	const _bool				Is_Release() const					{ return m_bRelease; };

	void					Set_Material(const MATERIALDESC tMtrlDesc) { m_tMaterialDesc = tMtrlDesc; }

	void					Set_ToolRepeat(_bool bRepeat)		{ m_bToolRepeat = bRepeat; }
#endif

protected:
	const EFFECT			m_eEffect		= static_cast<EFFECT>(0);

	_float					m_fTimeAcc		= 0.f;
	_bool					m_bRelease		= false;
#if ACTIVATE_TOOL
	_bool					m_bToolRepeat	= false;
#endif
	_flags					m_iShaderFlag	= 0;

public:
#if ACTIVATE_TOOL
	virtual HRESULT			Export(const wstring& wstrFilePath)	PURE;
#endif
};

END
