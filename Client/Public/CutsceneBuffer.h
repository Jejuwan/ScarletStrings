#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CKeyframe;
class CCamera;
END

BEGIN(Client)

class CCutsceneBuffer final : public CGameObject
{
private:
	explicit CCutsceneBuffer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCutsceneBuffer(const CCutsceneBuffer&);
	virtual ~CCutsceneBuffer() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Render_OutLine() override;

public:
	void								Bind_Camera(shared_ptr<CCamera> pCamera, _matrix mInitTransform);

public:
	void								Append_Keyframe(const wstring& wstrFilePath, const wstring& wstrAnimTag);
	void								Play_Animation(const wstring& wstrAnimTag, const std::function<void(const wstring& wstrAnimTag)>& fpFinished = nullptr);

public:
	void								Disappear(_float3 vVelocity, _float fDuration);

private:
	virtual HRESULT						Ready_Components() override;
	
private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;
	shared_ptr<CKeyframe>				m_pKeyframe;
	
	shared_ptr<CRenderer>				m_pRenderer;
	
	shared_ptr<CTexture>				m_pHorizontalMaskTexture;
	shared_ptr<CTexture>				m_pVerticalMaskTexture;

	shared_ptr<CTexture>				m_pNoiseGridTexture;

private:
	shared_ptr<CCamera>					m_pCamera;
	_float4x4							m_mProjection;
	
private:
	_bool								m_bDisappear = { false };
	_float3								m_vVelocity;

	_float								m_fDuration;
	_float								m_fAccTime;

private:
	CUTSCENE_CHANNEL					m_eCutsceneChannel;

public:
	static shared_ptr<CCutsceneBuffer>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END