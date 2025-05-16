#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CCustomFont final
{
private:
	explicit CCustomFont(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CCustomFont();

public:
	HRESULT							Initialize(const wstring& strFontFilePath);
	HRESULT							Render_Font(const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation);

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };

	SpriteBatch*					m_pBatch = { nullptr };
	SpriteFont*						m_pFont = { nullptr };

public:
	static shared_ptr<CCustomFont>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& strFontFilePath);
};

END