#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CLoadingTip : public CGameObject
{
private:
	explicit CLoadingTip(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CLoadingTip() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	shared_ptr<CTexture>					m_pTexture;
	shared_ptr<CTexture>					m_pLineTexture;

private:
	_float									m_fTilingFactor = { 1.f };

private:
	_float2									m_vDescFontPosition;
	_float2									m_vTitleFontPosition;
	_float									m_fDescFontRotation;

private:
	SCENE									m_eLoadingScene;

private:
	static pair<wstring, wstring>			m_tLoadingTipPair[IDX(SCENE::MAX)];

public:
	static shared_ptr<CLoadingTip>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any = g_aNull);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END