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

class CLoadingLine : public CGameObject
{
private:
	explicit CLoadingLine(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CLoadingLine() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual HRESULT							Ready_Components() override;

private:
	shared_ptr<CTransform>					m_pBackground;
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	shared_ptr<CTexture>					m_pLineTexture;

public:
	static shared_ptr<CLoadingLine>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END