#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CTitleCharacter final : public CGameObject
{
public:
	typedef struct tagTitleCharacterDesc
	{
		const _wchar*				wszModelTag;
		_float4x4					matTransform;
		_float4						vMtrlDiffuse;
		_uint						iAnimIndex;

		vector<pair<_uint, _uint>> vecMeshPassIndexPair;
	} TITLECHARACTERDESC;
private:
	explicit CTitleCharacter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTitleCharacter(const CTitleCharacter&);
	virtual ~CTitleCharacter() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	TITLECHARACTERDESC					m_tTitleCharacterDesc;

private:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

public:
	static shared_ptr<CTitleCharacter>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any = g_aNull);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END