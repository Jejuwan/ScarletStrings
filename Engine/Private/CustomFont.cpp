#include "EnginePCH.h"
#include "CustomFont.h"

CCustomFont::CCustomFont(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : m_pDevice(_pDevice)
    , m_pContext(_pContext)
{
}

CCustomFont::~CCustomFont()
{
	delete m_pBatch;
	m_pBatch = nullptr;
	 
	delete m_pFont;
	m_pFont = nullptr;
}

HRESULT CCustomFont::Initialize(const wstring& strFontFilePath)
{
	m_pBatch    = new SpriteBatch(m_pContext.Get());

	m_pFont     = new SpriteFont(m_pDevice.Get(), strFontFilePath.c_str());

	return S_OK;
}

HRESULT CCustomFont::Render_Font(const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation)
{
	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, strText.c_str(), vPos, vColor, fRotation, _float2(0.0f, 0.f), fScale);

	m_pBatch->End();

	return S_OK;
}

shared_ptr<CCustomFont> CCustomFont::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& strFontFilePath)
{
	shared_ptr<CCustomFont>     pInstance = shared_ptr<CCustomFont>(new CCustomFont(_pDevice, _pContext), [](CCustomFont* p) { delete p; });

    if (FAILED(pInstance->Initialize(strFontFilePath)))
    {
		MSG_RETURN(nullptr, "CCustomFont::Create", "Failed to Create : CCustomFont");
    }

    return pInstance;
}
