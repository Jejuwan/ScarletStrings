#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Drive_SideGauge final : public CGameObject
{
private:
	explicit CUI_Drive_SideGauge(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Drive_SideGauge() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;
	 
private:
	virtual HRESULT							Ready_Components() override;

public:
	void									Initialize_UI();

private:
	shared_ptr<CTransform>					m_pDrive_SideGauge;
	shared_ptr<CTexture>					m_pTexture;	
	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	SPRITEUIDESC							m_tUIDesc;

public:
	static shared_ptr<CUI_Drive_SideGauge>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;

};

END