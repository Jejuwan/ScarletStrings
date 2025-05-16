#pragma once

#include "Client_Define.h"
#include "GameObject.h"
#include "Map_Dynamic.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CModelRenderer;
END

BEGIN(Client)

class CMap_MindRoom_Dynamic : public CMap_Dynamic
{
private:
	explicit CMap_MindRoom_Dynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_MindRoom_Dynamic(const CMap_MindRoom_Dynamic&);
	virtual ~CMap_MindRoom_Dynamic() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype(_uint iIndex);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
	void								Using_Map_Tool();
	void								Add_ModelCom();
	void								Add_DissolveModel();
	virtual void						Duplicate_Model(_float fTimeDelta);

	void	Hide_Mesh(wstring MeshName);
	HRESULT	Mesh_Render(wstring MeshName);
	void	Mesh_Index(wstring MeshName);

private:
	//shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

	INSTMODELDESC						m_tInstModelShadowDesc;
	INSTMODELDESC						m_tInstModelOutlineDesc;

	_bool								m_bRot = true;
	_bool								m_bLagEnable = true;
protected:
	_int								m_iDynamicIndex = { 0 };

private:
	shared_ptr<CTransform>				m_pTargetTransform = nullptr;

public:
	static shared_ptr<CMap_MindRoom_Dynamic>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Set_Rot(_bool b) { m_bRot = b; }
	void Set_LagEnable(_bool b) { m_bLagEnable = b; }

};

END
