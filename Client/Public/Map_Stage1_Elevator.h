#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap_Stage1_Elevator : public CGameObject
{
private:
	explicit CMap_Stage1_Elevator(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Stage1_Elevator(const CMap_Stage1_Elevator&);
	virtual ~CMap_Stage1_Elevator() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
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
	shared_ptr<CRenderer>				m_pRenderer;

private:
	class CPlayer*						m_pPlayer;
	_float								m_fOriginGravity;

	_bool								m_bStop = { false };

private:
	_bool	m_bApplyPx = { true };
	_bool	m_bSceneMapTool = { false };
	_bool	m_bElevatorDown = { false };

	_float m_fTimer = { 0.f };
	_float m_fTemp = { 1.f };

	_int	m_iIndex = { 0 };

	wstring m_strName;
	wstring m_strSelectedName;
	PxController* m_pController = { nullptr };
	PxController* m_pControllerLeft = { nullptr };
	PxController* m_pControllerRight = { nullptr };
	PxController* m_pControllerBack = { nullptr };
	shared_ptr<class CPhysXCollider>	m_pCollider;
	ACTORDESC m_pActorDesc;

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
	shared_ptr<class CCCTV>			m_pCCTV;
	_bool							m_bEnableCCTV = { false };

public:
	static shared_ptr<CMap_Stage1_Elevator>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
