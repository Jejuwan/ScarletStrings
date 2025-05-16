#pragma once

#include "Client_Define.h"
#include "Map_Dynamic.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CModelRenderer;
END

BEGIN(Client)

class CMap_Tutorial_Dynamic : public CMap_Dynamic
{
protected:
	explicit CMap_Tutorial_Dynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Tutorial_Dynamic(const CMap_Tutorial_Dynamic&);
	virtual ~CMap_Tutorial_Dynamic() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype(_uint iIndex);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_PBR();

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
	virtual void						Duplicate_Model(_float fTimeDelta);

private:
	void								Add_DissolveModel();

protected:
	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

protected:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

	INSTMODELDESC						m_tInstModelShadowDesc;
	INSTMODELDESC						m_tInstModelOutlineDesc;

protected:
	_int								m_iDynamicIndex = { 0 };

private:
	_float3	m_vPos = { 0.f,0.f,0.f };

private:
	virtual void	Add_ModelCom();
	virtual void	Hide_Mesh(wstring MeshName);
	virtual HRESULT	Mesh_Render(wstring MeshName);
	virtual void	Mesh_Index(wstring MeshName);

public:
	static shared_ptr<CMap_Tutorial_Dynamic>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
