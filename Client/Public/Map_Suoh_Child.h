#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap_Suoh_Child : public CGameObject
{
public:
	enum class CHILDSTATE { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_END};
	enum class POINTPOS { POS_FIRST, POS_SECOND, POS_THIRD, POS_FOURTH,
		POS_FORTH, POS_FIFTH, POS_SIXTH, POS_END };
	enum class ROUNDTRIP { ROUNDTRIP_SECOND, ROUNDTRIP_THIRD, ROUNDTRIP_FOURTH, 
		ROUNDTRIP_FIFTH, ROUNDTRIP_SIXTH, ROUNDTRIP_END };

	typedef struct tagPos {
		_float3 CreatePos = {};
		_float3 Pos[4] = {};
	}CHILDPOS;

private:
	explicit CMap_Suoh_Child(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Suoh_Child(const CMap_Suoh_Child&);
	virtual ~CMap_Suoh_Child() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype(_uint iIndex);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
	void	Using_Map_Tool();
	void	State_Animation(_float fTimeDelta);
	void	Go_Point();
	void	Input_PointPos();
private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;
	shared_ptr<CRenderer>				m_pRenderer;

private:
	_bool	m_bApplyPx = { true };

	_float m_fTimer = { 0.f };

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
	_bool m_bDead = { false };  // 나중에 지워야함@!@!

	CHILDSTATE	m_eChildState = {};
	POINTPOS	m_ePointPos = {};
	ROUNDTRIP	m_eRoundTrip = {};

	CHILDPOS	m_ChildPos;

	_float3 m_Dir = { _float3(0.f,0.f,0.f) };

public:
	static shared_ptr<CMap_Suoh_Child>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
