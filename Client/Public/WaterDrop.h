#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CPhysics;
END

BEGIN(Client)

class CWaterDrop final : public CGameObject
{
private:
	explicit CWaterDrop(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CWaterDrop(const CWaterDrop&);
	virtual ~CWaterDrop() DEFAULT;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(any = g_aNull) override;
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;
	virtual HRESULT					Render() override;
	virtual HRESULT					Render_Bloom() override;

public:
	virtual HRESULT					Fetch(any mWorld) override;
	virtual _bool					Return() override;
	virtual HRESULT					Release() override;

private:
	virtual HRESULT					Ready_Components() override;
	virtual HRESULT					Ready_Behaviors() override;

private:
	shared_ptr<CShader>				m_pShader;
	shared_ptr<CTexture>			m_pTexDiffuse;
	shared_ptr<CTexture>			m_pTexNormal;
	shared_ptr<CTexture>			m_pTexNoise;
	shared_ptr<class CPhysXCollider>			m_pAttackCollider;
	ACTORDESC*						m_pAttackActorDesc = nullptr;
	_float							m_fBloomStrength	= 4.f;

	static const _float				s_fForce;

	shared_ptr<CTransform>			m_pTransform;
	shared_ptr<CPhysics>			m_pPhysics;

	_float							m_fTimeAcc			= 0.f;

	shared_ptr<CTransform>			m_pTargetTransform;
	_vector							m_vDir = {};
	_bool							m_bRelease			= false;

public:
	static shared_ptr<CWaterDrop>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>	Clone(any = g_aNull) override;
};

END
