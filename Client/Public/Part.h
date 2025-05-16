#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CPart final : public CGameObject
{
public :
	struct PARTDESC {
		const _float4x4*	pWeakBoneMatrix = { nullptr };
		_float4x4			PivotMatrix;
		const _float4x4*	pParentMatrix = { nullptr };
		_float4x4			PlusMatrix = g_mUnit;
		_bool				bGlitch = { false };
		_bool				bNeon = { false };
	};
private:
	explicit CPart(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPart(const CPart&);
	virtual ~CPart();

public:
	virtual HRESULT						Initialize_Prototype(const wstring& ModelTag);
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Neon();
	virtual HRESULT						Render_Screen_Effect() override;

	virtual HRESULT						Render_OutLine() override;

	void								Dissolve();
	void BlackMaskPivotChance();

public:
	void								Set_WeakBoneMatrix(const _float4x4* pWeakBoneMatrix) { m_pWeakBoneMatrix = pWeakBoneMatrix; }
	void								Set_PlusMatrix(_float4x4& matAdditivePivot) { m_PlusMatrix = matAdditivePivot; }

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;


	const _float4x4*					m_pWeakBoneMatrix = { nullptr };
	_float4x4							m_PivotMatrix;
	const _float4x4*					m_pParentMatrix = { nullptr };
	_float4x4							m_PlusMatrix;

private:
	shared_ptr<CTexture>				m_pTexDissolve;
	_bool								m_bDissolve = false;
	_float								m_fDissolveThreshold = 0.f;
	_float								m_fDissolveDuration = 1.f;

private:
	_bool								m_bGlitch = { false };
	_bool								m_bNeon = { false };

	shared_ptr<CTexture>				m_pGlitchNoise;

	_float								m_fGlitchSeed = { 0.f };

	_float								m_fSeedAccTime = { 0.f };
	_float								m_fSeedInterval = { 0.1f };

public:
	static shared_ptr<CPart>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& ModelTag);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	void Free();
};

END
