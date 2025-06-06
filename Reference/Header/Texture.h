#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	explicit CTexture(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTexture(const CTexture&);
	virtual ~CTexture() DEFAULT;

public:
	HRESULT										Initialize(_uint iNumTextures = 1);
	HRESULT										Initialize(const wstring& wstrTexturePath, _bool bSRGB, _uint iNumTextures = 1);
#if ACTIVATE_TOOL
	HRESULT										Initialize(const wstring& wstrModelPath, _bool bSRGB, aiMaterial*, aiTextureType);
#endif
	HRESULT										Initialize(std::ifstream&);

public:
	ComPtr<ID3D11ShaderResourceView>			Get_ShaderResourceView(_uint iTextureIndex = 0) const;
#if ACTIVATE_TOOL
	wstring										Get_TexturePath(_uint iTextureIndex = 0) const	{ return m_vecTexturePath[iTextureIndex]; }
	_uint										Get_NumTextures() const							{ return static_cast<_uint>(m_vecTexture.size()); }
#endif
	ComPtr<ID3D11Texture2D>						Get_Texture2D(_uint iTextureIndex = 0) const;
	HRESULT										Set_Texture2D(ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC, _uint iTextureIndex = 0);

	HRESULT										Get_Desc(D3D11_TEXTURE2D_DESC& pDesc, _uint iTextureIndex = 0);

public:
	HRESULT										Push_ShaderResourceView(const wstring& wstrFullPath, _bool bSRGB);
#if ACTIVATE_TOOL
	HRESULT										Remove_ShaderResourceView(_uint iTextureIndex);
	_bool										Swap_ShaderResourceView(_uint iTextureIndex1, _uint iTextureIndex2);
#endif

	HRESULT										Bind_ShaderResourceView(shared_ptr<class CShader>, aiTextureType, const _char* szConstantName, _uint iTextureIndex = 0) const;
	HRESULT										Bind_ShaderResourceViews(shared_ptr<class CShader>, aiTextureType, const _char* szConstantName);

private:
	ComPtr<ID3D11ShaderResourceView>			Create_ShaderResourceView(const wstring& wstrFullPath, _bool bSRGB) const;

private:
	vector<ComPtr<ID3D11ShaderResourceView>>	m_vecTexture;
#if ACTIVATE_TOOL
	vector<wstring>								m_vecTexturePath;
#endif

public:
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint iNumTextures = 1);
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrTexturePath, _uint iNumTextures = 1, _bool bSRGB = false);
#if ACTIVATE_TOOL
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrModelPath, aiMaterial*, aiTextureType, _bool bSRGB = false);
#endif
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;

	static shared_ptr<CTexture>					Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	HRESULT										Export(std::ofstream&);
#endif
};

END
