#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CPlayerTentacle final : public CGameObject
{
private:
	enum class VFX_STATE { STATIC, FADEIN, TRAIL, FADEOUT, MAX };

private:
	explicit CPlayerTentacle(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPlayerTentacle(const CPlayerTentacle&);
	virtual ~CPlayerTentacle();

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Set_Transparency(_bool bTansparency);
	void								Set_BrainFieldMode(_bool bBrainField) { m_bBrainField = bBrainField; }

private:
	virtual HRESULT						Ready_Components() override;


private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

	//unique_ptr<class CPlayerState>		m_eCurState;
	TENTACLEDESC							m_TentacleDesc;
	class CPlayer* m_pPlayer;

	_bool								m_bExecuted[10] = { false };
	_bool								m_bFly = false;
	_bool								m_bCommon = false;
	_bool								m_bRender = false;

private:
	_bool								m_bBrainField = { false };

private:
	_bool								m_bTransparency = { false };

private:
	_float								m_fGradientFactor = { 0.f };
	_bool								m_bKaren = { false };
private:
	_float3									m_vRootColor = { 1.f, 0.79f, 0.09f };
	_float3									m_vTipColor = { 2.5f, 0.f, 0.f };

	_float									m_fZFactor = { 1.6f };
	_bool									m_bDead = { false };
public:
	static shared_ptr<CPlayerTentacle>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

	void Set_Fly(_bool b) { m_bFly = b; }
	void Set_Render(_bool b) { m_bRender = b; }
	void Reset() { m_bRender = false; m_bCommon = false; m_fGradientFactor = 0.f; }
	void Set_Karen(_bool b) { m_bKaren = b; }
	void Set_Dead(_bool b) { m_bDead = b; }

	virtual _bool Return() override;

};

END
