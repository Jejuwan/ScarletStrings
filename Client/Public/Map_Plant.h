#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CModelRenderer;
END

BEGIN(Client)

class CMap_Plant : public CGameObject
{
private:
	explicit CMap_Plant(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Plant(const CMap_Plant&);
	virtual ~CMap_Plant() DEFAULT;
	
public:
	virtual HRESULT						Initialize_Prototype(_int iPlantIndex);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
	void	Using_Map_Tool();

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

	INSTMODELDESC						m_tInstModelShadowDesc;

private:
	_int		m_iPlantIndex = { 0 };

private:
	_float m_fTimer = { 0.f };
	_float m_fTemp = { 1.f };

	_int	m_iIndex = { 0 };

	_bool	m_bSceneMapTool = { false };

	wstring m_strName;
	wstring m_strSelectedName;

	shared_ptr<class CMap_Tool>	m_pMap_Tool;

	/* Scale, Rotation, Translation, matrix */
	tuple<wstring,_float3, _float3, _float3, _float4x4> m_tuplePivot;

	wstring m_strObjName;
	wstring m_strIndexName;
	wstring m_strNameNum;

	_float3 m_Scale = { 0.f,0.f,0.f };
	_float3 m_Rotation = { 0.f,0.f,0.f };
	_float3 m_Translation = { 0.f,0.f,0.f };
	_float4x4 m_Mat;

	_int m_iMeshIndex = { 0 };

	// TODO: Demo Code!#!@#$%#%^$%^$%#$%^$Y%$%^$@%^%$$&#$&^*
	_bool m_bDead = { false };  // ���߿� ��������@!@!

private:
	void	Add_ModelCom();
	void	Hide_Mesh(wstring MeshName);
	HRESULT	Mesh_Render(wstring MeshName);
	void	Mesh_Index(wstring MeshName);

public:
	static shared_ptr<CMap_Plant>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
