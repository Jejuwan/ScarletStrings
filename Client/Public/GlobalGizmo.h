#pragma once
#include "Client_Define.h"
#include "GameObject.h"

#ifdef ACTIVATE_TOOL

BEGIN(Engine)
class CVIBufferInstance_Rect;
END

BEGIN(Client)

class CGlobalGizmo final : public CGameObject
{
private:
	explicit CGlobalGizmo(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CGlobalGizmo(const CGlobalGizmo&);
	virtual ~CGlobalGizmo() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Initialize_Gizmo(_int2 vGridSize);
	void								Set_Color(_color);
	void								Set_RenderGroup(RENDER_GROUP);
	void								Show(_bool bShow = true)	{ m_bShow = bShow; }

private:
	RENDER_GROUP						m_eGroup	= RENDER_GROUP::BLEND;
	_bool								m_bShow		= true;

public:
	static shared_ptr<CGlobalGizmo>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END

#endif
