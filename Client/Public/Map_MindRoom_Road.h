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

class CMap_MindRoom_Road : public CGameObject
{
private:
	explicit CMap_MindRoom_Road(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_MindRoom_Road(const CMap_MindRoom_Road&);
	virtual ~CMap_MindRoom_Road() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype(_uint iIndex);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

public:
	void								Set_SingleColor_Lerp(_float3 vColor);
	void								Set_SingleColor(_float3 vColor);

	void								Set_SingleColorMode(_bool bSingleColorMode);

	void								Effect_SwipeClear();

public:
	void								Set_Visible(_bool bVisible);

public:
	void								Execute_RadialSperad();
	void								Reset_Option();

private:
	virtual HRESULT						Ready_Components() override;
	void	Using_Map_Tool();
private:
	void	Add_ModelCom();

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	_bool								m_bSingleColorMode = { false };
	_float3								m_vSingleColor;

	_float								m_fColorFactor = { 0.f };

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

private:
	_bool								m_bRadialSpread = { false };

	_float								m_fMaxSampleRadius = { 2.f };
	_float								m_fVisualRadius = { 0.f };

	_float								m_fSpreadSpeed = { 0.f };

private:
	_int	m_iIndex = { 0 };

	_bool	m_bSceneMapTool = { false };

	_float	m_fTimer = { 0.f };

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
	_bool m_bDead = { false };  // 나중에 지워야함@!@!

private:
	_bool m_bVisible = { true };

public:
	static shared_ptr<CMap_MindRoom_Road>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
