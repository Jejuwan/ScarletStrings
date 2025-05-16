#include "EnginePCH.h"
#include "Font_Manager.h"
#include "CustomFont.h"

HRESULT CFont_Manager::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strFontTag, const wstring & strFontFilePath)
{
	if (nullptr != Find_Font(strFontTag))
		return E_FAIL;

	shared_ptr<CCustomFont>	pFont = CCustomFont::Create(pDevice, pContext, strFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(strFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation)
{
	shared_ptr<CCustomFont> pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render_Font(strText, vPos, vColor, fScale, fRotation);
}

shared_ptr<CCustomFont> CFont_Manager::Find_Font(const wstring & strFontTag)
{
	auto	iter = m_Fonts.find(strFontTag);

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}