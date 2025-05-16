#pragma once
#include "Client_Define.h"
#include "Character.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CKasane final : public CCharacter
{
private:
	explicit CKasane(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CKasane(const CKasane&);
	virtual ~CKasane() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Move_Mouth() override;
	void								Stop_Mouth() override;

public:
	void								Open_Drive_Mode(const std::function<void()>& fpFinished = nullptr);

private:
	HRESULT								Render_Drive_Cutscene();
	HRESULT								Render_Drive_Mask();

	virtual HRESULT						Render_Screen_Effect() override;

private:
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;

private:
	_bool								m_bDriveMode = { false };
	_bool								m_bMaskOn = { false };
	_bool								m_bScreenEffect = { false };

	shared_ptr<CTexture>				m_pMaskNoise;
	shared_ptr<CTexture>				m_pCircularMask;

private:
	_float								m_fOriginDOFStart;
	_float								m_fOriginDOFRange;

	_float								m_fOriginBloomStrength;
	_float								m_fOriginExposure;
	_float								m_fOriginGamma;

private: // Screen Effect
	shared_ptr<CShader>					m_pEffectShader;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;

	shared_ptr<CTransform>				m_pScreenBufferTransform;
	_float4x4							m_matOrthoProj;

	_float								m_fLineStripeTime;

private:
	_bool								m_bPlayMode = { false };

public:
	static shared_ptr<CKasane>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END