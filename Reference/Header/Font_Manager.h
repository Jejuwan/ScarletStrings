#pragma once
#include "Engine_Define.h"

BEGIN(Engine)
class CCustomFont;
class CFont_Manager final : public CSingleton<CFont_Manager>
{
private:
	explicit CFont_Manager() DEFAULT;
	virtual ~CFont_Manager() DEFAULT;

public:
	HRESULT											Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT											Render_Font(const wstring& strFontTag, const wstring& strText, const _float2 vPos, _vectorf vColor, _float fScale, const _float fRotation);

private:
	map<const wstring, shared_ptr<CCustomFont>>		m_Fonts;

private	:
	shared_ptr<CCustomFont>							Find_Font(const wstring& strFontTag);

private:
	friend CSingleton<CFont_Manager>;
};

END