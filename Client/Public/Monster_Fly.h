#pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_Fly final : public CMonster
{
private:
	explicit CMonster_Fly(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Fly(const CMonster_Fly&);
	virtual ~CMonster_Fly() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual wstring						Get_TypeName() { return L"레벨 05 반도 펜두"; };

	virtual HRESULT						Render_OutLine() override;
	virtual void						Update_Motion(_float fTimeDelta) override;
	virtual void						RootMotion(_float fTimeDelta, _bool HeightFix = false) override;
	virtual void						RootMotionRotate(_float fTimeDelta, _bool HeightFix = false) ;
	virtual void						InitMovePos(_float fTimeDelta);
	virtual void						MoveRootPos();

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

public :

	shared_ptr<class CPhysXCollider>	m_pDodgeCollider;
	ACTORDESC*							m_pDodgeActorDesc = { nullptr };

public:
	static shared_ptr<CMonster_Fly>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
