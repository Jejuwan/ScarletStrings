#pragma once
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTransform;
END

BEGIN(Client)

class CMonster_Karen_Cloned final : public CMonster
{
public :
	struct CLONEDESC
	{
		_float4 InitPos = {};
		shared_ptr<CTransform> TargetTransform;
	};
private:
	explicit CMonster_Karen_Cloned(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Karen_Cloned(const CMonster_Karen_Cloned&);
	virtual ~CMonster_Karen_Cloned() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Fetch(any Any) override;

	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"레벨 ??? 카렌 트레버스"; }; 

private:
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;

private :
	shared_ptr<CTransform>				m_pTargetTransform;
	_float								m_fTimer = { 5.f };
	_bool								m_bStart = { false };
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

public:
	static shared_ptr<CMonster_Karen_Cloned>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	


};

END
