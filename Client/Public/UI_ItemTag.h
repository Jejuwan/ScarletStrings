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

class CUI_ItemTag final : public CGameObject
{
private:
	explicit CUI_ItemTag(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_ItemTag() DEFAULT;

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(any _aDesc = g_aNull)	override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual HRESULT							Ready_Components() override;

private:
	void									Split_ItemCount();

private:
	shared_ptr<CTransform>					m_pItemTag;

	shared_ptr<CTexture>					m_pTexture;

	shared_ptr<CShader>						m_pShader;
	shared_ptr<CVIBuffer_Rect>				m_pVIBuffer_Rect;

private:
	wstring									m_strItemTag = L"";


#pragma region ItemCount
private:
	shared_ptr<CTexture>					m_pTexture_Zero;
	shared_ptr<CTexture>					m_pTexture_One;
	shared_ptr<CTexture>					m_pTexture_Two;
	shared_ptr<CTexture>					m_pTexture_Three;
	shared_ptr<CTexture>					m_pTexture_Four;
	shared_ptr<CTexture>					m_pTexture_Five;
	shared_ptr<CTexture>					m_pTexture_Six;
	shared_ptr<CTexture>					m_pTexture_Seven;
	shared_ptr<CTexture>					m_pTexture_Eight;
	shared_ptr<CTexture>					m_pTexture_Nine;

	shared_ptr<CTransform>					m_pOnes;
	shared_ptr<CTransform>					m_pTens;


	_uint									m_iItemCount;
	_uint									m_iOnes;		// 1의 자리
	_uint									m_iTens;		// 10의 자리

	_float3									m_vScale_ItemCount_Tens;
	_float3									m_vScale_ItemCount_Ones;
	_float4									m_vPosition_ItemCount_Tens;
	_float4									m_vPosition_ItemCount_Ones;
#pragma endregion



public:
	static shared_ptr<CUI_ItemTag>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>			Clone(any = g_aNull) override;
};

END