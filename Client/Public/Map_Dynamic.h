#pragma once

#include "Client_Define.h"
#include "InteractiveObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
END

BEGIN(Client)

class CMap_Dynamic abstract : public CInteractiveObject
{
protected:
	explicit CMap_Dynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Dynamic(const CMap_Dynamic&);
	virtual ~CMap_Dynamic() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() PURE;

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

public:
	virtual void						Interactive_PhychoKinesis(_bool bInteractive) override;

protected:
	virtual HRESULT						Ready_Components() override;
	virtual void						Duplicate_Model(_float fTimeDelta);
	virtual void						Using_Map_Tool();

protected:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

protected:
	shared_ptr<CTexture>				m_pDissolveTexture;
	_bool								m_bDissolve = { false };
	_bool								m_bCaptured = { false };
	_uint								m_iPassIndex = { 15 };
	_uint								m_iGlassPassIndex = { 5 };

	_float								m_fDissolveTime = { 0.f };

	shared_ptr<CTexture>				m_pAuraTexture;
	_bool								m_bAura = { false };

	_float3								m_vAuraColor = { 0.1f, 0.f, 0.1f };
	_float								m_fAuraBlendFactor = { 0.3f };
	_float								m_fAuraTimeFactor = { 0.f };

protected:

	_bool  m_bExecuted[100] = { false };
	_float m_fTimer = { 0.f };
	_float m_fTemp = { 1.f };

	_float m_fDuplicateTime = 0.1f;

	_int	m_iIndex = { 0 };

	_bool	m_bSceneMapTool = { false };

	wstring m_strName;
	wstring m_strSelectedName;

	shared_ptr<class CMap_Tool>	m_pMap_Tool;
	shared_ptr<class CMap_Dynamic> m_pDuplicatedObject;
	shared_ptr<class CMonster> m_pTargetMonster;
	shared_ptr<class CTransform> m_pTargetMonsterTransform;

	/* Scale, Rotation, Translation, matrix */
	tuple<wstring, _float3, _float3, _float3, _float4x4> m_tuplePivot;
	tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> m_copyTuplePivot;
	tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> m_duplicateTuplePivot;

	wstring m_strObjName;
	wstring m_strIndexName;
	wstring m_strNameNum;

	_float3 m_Scale = { 0.f,0.f,0.f };
	_float3 m_Rotation = { 0.f,0.f,0.f };
	_float3 m_Translation = { 0.f,0.f,0.f };
	_float4x4 m_Mat;

	const _float4x4* m_BoneMatrix = { nullptr };
	_float4x4* m_MonsterMatrix = nullptr;
	_float4x4 m_Pivot;

	_int m_iMeshIndex = { 0 };

	_bool m_bDead = { false };

	_bool m_bActivePhysX = { false };
	_bool m_bThrow = { false };
	_bool m_bThrowHit = { false };
	_bool m_bReadyDuplicate = { false };
	_bool m_bStrong = { false };

	PxShape*		m_pShape = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	_float3			m_vExtents;
	_float3			m_vLocalPose;
	_float3			m_vThrowLook;
	_float3			m_vHoodPos;

	_float			m_fMass = 1.f;
	_float			m_fThrowForce = 200000.f;
	_float			m_fThrowTime = 3.f;
	_float			m_ThrowHitTime = 0.1f;
	_float			m_fThrowHeight = 2.5f;
	ACTORDESC		m_ActorDesc;

	_int		   m_iTutorialDynamicIndex = -1;

protected:
	_bool			m_bInitPhysX = { false };

	_bool			m_bInitPhysXDelay = { true };
	_float			m_fInitPhysXDelay = { 3.f };
	_float			m_fInitPhysXAccTime = { 0.f };

protected :
	virtual void	Add_ModelCom() {};
	virtual void	Hide_Mesh(wstring MeshName) {};
	virtual HRESULT	Mesh_Render(wstring MeshName) { return S_OK; };
	virtual void	Mesh_Index(wstring MeshName) {};

public:
	void	SetPhysX(_bool b);
	virtual void	Throw();
	virtual _bool	Return() override;
	virtual void	ThrowHit();
	void	Cancel();
	void	Destroy();
	shared_ptr<CMap_Dynamic> Duplicate();
	void	Set_Target_Monster();
	void	Set_ThrowLook();

	PxRigidDynamic* Get_Actor() { return m_pActor; }
	void	Set_ActivePhysX(_bool b) { m_bActivePhysX = b; }
	void	Set_Gravity(_bool b);
	_bool Get_ThrowHit() { return m_bThrowHit; }
	void Set_ThrowHit(_bool b) { m_bThrowHit = b; }
	_bool Get_Throw() { return m_bThrow; }
	void Set_Throw(_bool b) { m_bThrow = b; }
	tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> Get_CopyTuplePivot()
	{
		return m_copyTuplePivot;
	}
	_bool Get_ReadyDuplicate() { return m_bReadyDuplicate; }
	shared_ptr<class CMap_Dynamic> Get_DuplicatedObject() { return m_pDuplicatedObject; }
	void Set_DuplicatedObjectPos();
	void Set_Strong(_bool b) {
		m_bStrong = b;
	}
	_float Get_ThrowHeight() { return m_fThrowHeight; }
	ACTORDESC& Get_ActorDesc() { return m_ActorDesc; }
	void Reset_Actor() { m_pActor = nullptr; }
	void Set_Aura(_bool b) { m_bAura = b; }
	void Set_Dissolve(_bool b) { m_bDissolve = b; }
	shared_ptr<CTransform> Get_TargetMonsterTransform() { return m_pTargetMonsterTransform; }
};

END
