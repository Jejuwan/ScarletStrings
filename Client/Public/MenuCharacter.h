#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMenuCharacter final : public CGameObject
{
public:
	typedef struct tagMenuCharacterDesc
	{
		const _wchar* wszModelTag;
		_float4x4					matTransform;
		_float4						vMtrlDiffuse;
		_uint						iAnimIndex;

		vector<pair<_uint, _uint>> vecMeshPassIndexPair;
	} MENUCHARACTERDESC;
private:
	explicit CMenuCharacter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMenuCharacter(const CMenuCharacter&);
	virtual ~CMenuCharacter() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Fade_In();
	void								Fade_Out();

public:
	const _wchar*						Get_ModelTag() { return m_tMenuCharacterDesc.wszModelTag; }

private:
	virtual HRESULT						Ready_Components() override;

private:
	void								On_RenderFinishedCallback();

private:
	shared_ptr<CRenderer>				m_pRenderer;
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	shared_ptr<CShader>					m_pUIShader;
	shared_ptr<CTransform>				m_pUITransform;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;

private:
	MENUCHARACTERDESC					m_tMenuCharacterDesc;

private:
	vector<pair<_uint, _uint>>			m_vecMeshPassIndexPair;

private:
	_bool								m_bRenderUI = { false };

private:
	_bool								m_bFading = { false };
	_bool								m_bFadeIn = { false };

	_float								m_fFadeFactor = { 1.f };

private:
	_float4x4							m_matProjection;

public:
	static shared_ptr<CMenuCharacter>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any = g_aNull);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END