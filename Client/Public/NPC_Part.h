#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CNPC_Part : public CGameObject
{
public :
	struct NPC_PARTDESC {
		wstring ModelName;
		shared_ptr<CModel> ParentModel;
	};


public:
	enum class MANSTATE { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_END};
	enum class POINTPOS { POS_FIRST, POS_SECOND, POS_THIRD, POS_FOURTH,
		POS_FORTH, POS_FIFTH, POS_SIXTH, POS_END };
	enum class ROUNDTRIP { ROUNDTRIP_SECOND, ROUNDTRIP_THIRD, ROUNDTRIP_FOURTH, 
		ROUNDTRIP_FIFTH, ROUNDTRIP_SIXTH, ROUNDTRIP_END };

	typedef struct tagPos {
		_float3 CreatePos = {};
		_float3 Pos[4] = {};
	}ManPOS;

private:
	explicit CNPC_Part(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CNPC_Part(const CNPC_Part&);
	virtual ~CNPC_Part() DEFAULT;

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
	HRESULT								Ready_Parts();

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

	vector<shared_ptr<CGameObject>>			m_pParts;

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

	MANSTATE	m_eManState = {};
	POINTPOS	m_ePointPos = {};
	ROUNDTRIP	m_eRoundTrip = {};

	ManPOS	m_ManPos;

	_float3 m_Dir = { _float3(0.f,0.f,0.f) };

public:
	static shared_ptr<CNPC_Part>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
