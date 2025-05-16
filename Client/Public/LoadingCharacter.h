#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLoadingCharacter : public CGameObject
{
public:
	typedef struct tagLoadingCharacterDesc
	{
		const _wchar*			wszModelPath;
		const _wchar*			wszLineNoiseTexturePath;
		_float4x4		matTransform;

		_uint			iWalkAnimIndex;
	} LOADINGCHARACTERDESC;
private:
	explicit CLoadingCharacter(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CLoadingCharacter() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;

private:
	shared_ptr<CTexture>				m_pLineNoiseTexture;

private:
	LOADINGCHARACTERDESC				m_tLoadingCharcterDesc;

private:
	_float4x4							m_matView;
	_float4x4							m_matProjection;

public:
	static shared_ptr<CLoadingCharacter>Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, any = g_aNull);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END