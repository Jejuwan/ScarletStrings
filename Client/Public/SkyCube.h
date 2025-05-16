#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CSkyCube final : public CGameObject
{
private:
	explicit CSkyCube(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CSkyCube(const CSkyCube&);
	virtual ~CSkyCube() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any wstrTexturePath) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Set_LerpColor(_float3 vColorA, _float3 vColorB, _float fDuration);
	void								Set_GradationBackGround(_float3 vColorA, _float3 vColorB);
	void								GradientClear(_float3 vColorA);
	void								Reset();

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CTexture>				m_pTexture;

private:
	_bool								m_bSingleColorMode = { false };
	_float3								m_vColorA;
	_float3								m_vColorB;

	_float								m_fAccTime;
	_float								m_fDuration;

private:
	_bool								m_bGradationMode = { false };
	_bool								m_bGradationClearMode = { false };

public:
	static shared_ptr<CSkyCube>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any wstrTexturePath) override;
};

END
