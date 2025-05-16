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

class CMap_Suoh_Dynamic : public CMap_Dynamic
{
public:
	enum class CARSTATE { STATE_IDLE, STATE_MOVE, STATE_END };
	enum class POINTPOS {
		POS_1, POS_2, POS_3, POS_4,
		POS_5, POS_6, POS_7, POS_8, 
		POS_9, POS_10, POS_11, POS_12,
		POS_13, POS_14, POS_END
	};
	enum class ROUNDTRIP {
		ROUNDTRIP_2, ROUNDTRIP_3, ROUNDTRIP_4,
		ROUNDTRIP_5, ROUNDTRIP_6, ROUNDTRIP_7,
		ROUNDTRIP_8, ROUNDTRIP_9, ROUNDTRIP_10, 
		ROUNDTRIP_11, ROUNDTRIP_12, ROUNDTRIP_END
	};
	enum class CARROOT {
		ROOT_A, ROOT_B, ROOT_C,
		ROOT_END
	};

	typedef struct tagPos {
		_float3 CreatePos = {};
		_float3 Pos[12] = {};
	}CARPOS;

private:
	explicit CMap_Suoh_Dynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Suoh_Dynamic(const CMap_Suoh_Dynamic&);
	virtual ~CMap_Suoh_Dynamic() DEFAULT;

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


	void	Car_Moving(_float fTimeDelta);
	void	Input_PointPos();
	void	Car_Root();

/////////////////////////////////////////////////////////////////////////////////////////////////
private:
	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

	INSTMODELDESC						m_tInstModelShadowDesc;
	INSTMODELDESC						m_tInstModelOutlineDesc;

private:
	_int								m_iDynamicIndex = { 0 };

private:
	CARSTATE	m_eCarState = {};
	POINTPOS	m_ePointPos = {};
	ROUNDTRIP	m_eRoundTrip = {};
	CARROOT		m_eCarRoot = {};

	CARPOS		m_CarPos;
	_float3 m_Dir = { _float3(0.f,0.f,0.f) };
	_float m_fSpeed = { 0.f };

	_bool	m_bSouhScene = { false };

public:
	static shared_ptr<CMap_Suoh_Dynamic>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
