#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Sphere;
END

BEGIN(Client)

class CPointGizmo final : public CGameObject
{
private:
	explicit CPointGizmo(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPointGizmo(const CPointGizmo&);
	virtual ~CPointGizmo() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;

public:
	void								On_Selected(_bool bSelected) { m_bSelected = bSelected; }

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CVIBuffer_Sphere>		m_pVIBuffer_Sphere;

private:
	_float3								m_vGizmoColor = { 1.f, 0.68f, 0.785f };
	_float3								m_vSelectedColor = { 1.f, 0.f, 0.f };

	_bool								m_bSelected = { false };

private:
	_bool								m_bOutLine = { false };
	_float3								m_vOutLineColor = { 0.768f, 1.f, 0.05f };
	_float								m_fOutLineSize = { 0.3f };

public:
	static shared_ptr<CPointGizmo>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END