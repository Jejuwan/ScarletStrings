#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CMonster_Weak final : public CGameObject
{
public :
	struct WEAKDESC {
		const _float4x4*	pWeakBoneMatrix = { nullptr };
		_float4x4			PivotMatrix;
		const _float4x4*	pParentMatrix = { nullptr };
		_float4				ControllerPivot = { 0.f,0.f,0.f,0.f };
	};
private:
	explicit CMonster_Weak(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Weak(const CMonster_Weak&);
	virtual ~CMonster_Weak();

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Neon();

	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;


	const _float4x4*					m_pWeakBoneMatrix = { nullptr };
	_float4x4							m_PivotMatrix;
	const _float4x4*					m_pParentMatrix = { nullptr };
	_float4								m_ControllerPivot;

public:
	static shared_ptr<CMonster_Weak>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Free();
};

END
