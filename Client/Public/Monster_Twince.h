#pragma once
#include "Client_Define.h"
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMonster_Twince final : public CMonster
{
private:
	explicit CMonster_Twince(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Twince(const CMonster_Twince&);
	virtual ~CMonster_Twince() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"설라이버 산타"; };
			
	void								SetRenderBodyTrue();// { m_bRenderBody = true; }
	void								SetRenderBodyFalse();// { m_bRenderBody = false; }
	
	virtual void						Dissolve();

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

private :
	shared_ptr<CGameObject>				m_pTwinceBox = { nullptr };
	_bool								m_bRenderBody = { true };
	_bool								m_bEffect = { false };
public:
	static shared_ptr<CMonster_Twince>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
};

END
