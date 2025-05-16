#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CCharacter abstract : public CGameObject
{
protected:
	explicit CCharacter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCharacter(const CCharacter&);
	virtual ~CCharacter() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() PURE;

	virtual HRESULT						Render_OutLine() override;
	virtual HRESULT						Ready_Components() override;

public:
	virtual void						Move_Mouth() PURE;
	virtual void						Stop_Mouth() PURE;

public:
	virtual void						Execute_SAS(const std::function<void()>& fpFinished = nullptr);

protected:
	virtual HRESULT						Render(_uint iPassIndex);

	HRESULT								Render_SAS_Cutscene();
	HRESULT								Render_SAS_Cutscene_Outline();

protected:
	void								Tick_SAS(_uint iAnimIndex, _float fTimeDelta);

protected:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

protected:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

protected:
	_float4x4							m_mView;

	_bool								m_bExecuteSAS = { false };

protected:
	_float3								m_vOutLineColor = { 0.f, 0.f, 0.f };
	_float3								m_vAdditiveColor = { 0.f, 0.f, 0.f };
	_float								m_fOutLineSize = { 0.001f };

protected:
	shared_ptr<CTexture>				m_pNoiseLine;
	shared_ptr<CTexture>				m_pTrinagleNoise;
	shared_ptr<CTexture>				m_pDissolveNoise;

protected:
	_bool								m_bDissolve = { false };
	_int								m_iDissolve = { 0 };

	_float								m_fDissolveTime = { 0.3f };
	_float								m_fDissolveAccTime = { 0.f };

protected:
	std::function<void()>				m_fpFinished;

#ifdef _DEBUG
	_uint m_iCurrentAnimIndex = { 0 };
#endif

public:
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) PURE;
};

END