#include "EnginePCH.h"
#include "ResourceManager.h"

#include "Texture.h"

_bool CResourceManager::Is_Exist_Texture(const wstring wstrTextureTag)
{
	auto iter = m_mapTextures.find(wstrTextureTag);
	if (iter == m_mapTextures.end())
		return false;

	return true;
}

std::shared_ptr<class CTexture> CResourceManager::Get_Texture(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring pTextureTag, const wstring pTexturePath)
{
	std::shared_ptr<class CTexture> pTexture;;
	if (Is_Exist_Texture(pTextureTag))
	{
		pTexture = m_mapTextures[pTextureTag];
	}
	else
	{
		pTexture = CTexture::Create(_pDevice, _pContext, pTexturePath, 1, true);
		m_mapTextures.emplace(pTextureTag, pTexture);
	}

	return pTexture;
}
