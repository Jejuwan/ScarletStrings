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

class CUI_Indicator_BrainTalk_Tutorial final : public CGameObject
{
	enum BRAINTALK
	{
		BT_FIRST,
		BT_SECOND,
		BT_THIRD,
		BT_FOURTH,
		BT_FIFTH,
		BT_MAX
	};
private:
	explicit CUI_Indicator_BrainTalk_Tutorial(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_BrainTalk_Tutorial() DEFAULT;
public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any _aDesc = g_aNull)	override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;

private:
	virtual HRESULT											Ready_Components() override;

private:
	void													Initialize_UI();
	void													Tick_UI(_float fTimeDelta);
	void													OpenBrainTalk(_float fTimeDelta);
	void													BrainTalk_Reset();
	void													Tick_SpriteTexture(_float fTimeDelta);

private:
	void													Conversation_First(_float fTimeDelta);
	void													Conversation_Second(_float fTimeDelta);
	void													Conversation_Third(_float fTimeDelta);
	void													Conversation_Fourth(_float fTimeDelta);
	void													Conversation_Fifth(_float fTimeDelta);

private:
	void													Set_BrainTalk_Test();

public:
	void													Set_BrainTalk(_uint iBrainTalkIndex);

private:
	// Transform
	shared_ptr<CTransform>									m_pBrainTalkFrame;
	shared_ptr<CTransform>									m_pBrainTalkFrame_Noise;
	shared_ptr<CTransform>									m_pFaceFrame;
	shared_ptr<CTransform>									m_pFaceFrame_Noise;
	shared_ptr<CTransform>									m_pBrainTalk_LightFrame;
	shared_ptr<CTransform>									m_pBrainTalk_LightFrame_Noise;
	shared_ptr<CTransform>									m_pBrainTalk_Light;
	shared_ptr<CTransform>									m_pFace_Kasane;

private:
	// Texture
	shared_ptr<CTexture>									m_pTexture_BrainTalkFrame;
	shared_ptr<CTexture>									m_pTexture_BrainTalkFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_FaceFrame;
	shared_ptr<CTexture>									m_pTexture_FaceFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_LightFrame;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_LightFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_Light;
	shared_ptr<CTexture>									m_pTexture_Face_Kasane;

private:
	shared_ptr<CShader>										m_pShader;
	shared_ptr<CVIBuffer_Rect>								m_pVIBuffer_Rect;

private:
	// Scale
	_float3													m_vScale_BrainTalkFrame;
	_float3													m_vScale_BrainTalkFrame_Noise;
	_float3													m_vScale_FaceFrame;
	_float3													m_vScale_FaceFrame_Noise;
	_float3													m_vScale_BrainTalk_LightFrame;
	_float3													m_vScale_BrainTalk_LightFrame_Noise;
	_float3													m_vScale_BrainTalk_Light;
	_float3													m_vScale_Face_Kasane;

private:
	// Position
	_float4													m_vPosition_BrainTalkFrame;
	_float4													m_vPosition_BrainTalkFrame_Noise;
	_float4													m_vPosition_FaceFrame;
	_float4													m_vPosition_FaceFrame_Noise;
	_float4													m_vPosition_BrainTalk_LightFrame;
	_float4													m_vPosition_BrainTalk_LightFrame_Noise;
	_float4													m_vPosition_BrainTalk_Light;
	_float4													m_vPosition_Face_Kasane;

private:
	SPRITEUIDESC											m_tUIDesc;

private:
	BRAINTALK												m_eBrainTalk;

private:
	// For. Trigger
	_bool													m_bBrainTalk_First;
	_bool													m_bBrainTalk_Second;
	_bool													m_bBrainTalk_Third;
	_bool													m_bBrainTalk_Fourth;
	_bool													m_bBrainTalk_Fifth;

	// For. Render End
	_bool													m_bDisappear;

	// For. Render
	_bool													m_bRender_BTLF;
	_bool													m_bRender_BTLFN;
	_bool													m_bRender_Noise;
	_bool													m_bRender_FaceFrame;
	_bool													m_bRender_BTF;
	_bool													m_bRender_Face_Kasane;
	_bool													m_bRender_BTL;

private:
	_uint													m_iCheckSoundIndex;

private:
	_float													m_fAccTime;
	_float													m_fAccTime_Face_Kasane;
	_float													m_fCheckSoundTime;

	_float													m_fBTLFN;
	_float													m_fBTLF;
	_float													m_fNoiseAlpha;
	_float													m_fFaceFrame;
	_float													m_fBTF;
	_float													m_fFace_KasaneAlpha;

private:
	wstring													m_strText1;
	wstring													m_strText2;

public:
	static shared_ptr<CUI_Indicator_BrainTalk_Tutorial>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;
};

END