#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CResourceManager final : public CSingleton<CResourceManager>
{
private:
	explicit CResourceManager() DEFAULT;
	virtual ~CResourceManager() DEFAULT;

public:
	_bool Is_Exist_Texture(const wstring wstrTextureTag);

	std::shared_ptr<class CTexture> Get_Texture(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const wstring pTextureTag, const wstring pTexturePath);

private:
	map<wstring, std::shared_ptr<class CTexture>>		m_mapTextures;

	friend CSingleton<CResourceManager>;
};

END