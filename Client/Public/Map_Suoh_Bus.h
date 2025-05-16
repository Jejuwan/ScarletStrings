#pragma once

#include "Client_Define.h"
#include "Map_Special_Dynamic.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CModelRenderer;
END

BEGIN(Client)

class CMap_Suoh_Bus : public CMap_Special_Dynamic
{
private:
	explicit CMap_Suoh_Bus(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Suoh_Bus(const CMap_Suoh_Bus&);
	virtual ~CMap_Suoh_Bus() DEFAULT;

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
	void	Add_ModelCom();


private:
	PxShape* m_pKinematicShape = nullptr;
	PxRigidDynamic* m_pKinematicActor = nullptr;
	shared_ptr<class CPhysXCollider> m_pCollider = nullptr;
private:
	shared_ptr<CModelRenderer>			m_pModelRenderer;
	wstring								m_wstrModelTag;

private:
	INSTMODELDESC						m_tInstModelDesc;
	INSTMODELMESHDESC					m_tInstModelMeshDesc;

	INSTMODELDESC						m_tInstModelShadowDesc;



public:
	static shared_ptr<CMap_Suoh_Bus>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	shared_ptr<class CPhysXCollider> Get_Collider() { return m_pCollider; }
	PxRigidDynamic* Get_KinematicActor() { return m_pKinematicActor; }
};

END
