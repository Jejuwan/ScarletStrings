#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCutsceneTentacle : public CGameObject
{
public:
	enum class ANIMATION
	{
		BRAIN_START01,
		BRAIN_START02,
		BRAIN_START03,
		BRAIN_START04,
		BRAIN_START05,
		BRAIN_START06,
		BRAIN_TRANS01,
		BRAIN_TRANS02,
		BRAIN_TRANS03,
		BRAIN_TRANS04,
		BRAIN_TRANS05,
		BRAIN_TRANS06,
	};
private:
	explicit CCutsceneTentacle(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCutsceneTentacle(const CCutsceneTentacle&);
	virtual ~CCutsceneTentacle() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Neon() override;
	virtual HRESULT						Render_Screen_Effect() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	_float3								m_vStripeColor = float3(2.f, 0.2f, 0.2f);

public:
	static shared_ptr<CCutsceneTentacle>Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any eScene = g_aNull) override;
};

END