#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSound_Manager;
END

BEGIN(Client)

class CUI_Indicator_BrainTalk_Mizuha final : public CGameObject
{
public:
	enum BRAINTALK_CONVERSATION
	{
		BT_FIRST, BT_SECOND, BT_THIRD, BT_FOURTH, BT_FIFTH, BT_SIXTH, BT_SEVENTH, 
		BT_BOSSSTAGE, BT_BOSSSTAGE_HALF, BT_BOSSSTAGE_END, BT_CROWS, BT_MAX
	};
	enum BRAINTALK_SPECIFIC_SITUATION
	{
		// Haruka
		BT_HARUKA_PLAYER_DAMAGED_BY_ELECTRIC, BT_HARUKA_PLAYER_DAMAGED_BY_WATER,
		// Kasane
		BT_KASANE_LEVELUP, BT_KASANE_GET_ITEM,
		// Use My SAS
		BT_USE_MY_SAS_ARASI, BT_USE_MY_SAS_SIDEN, BT_USE_MY_SAS_KYOKA, BT_USE_MY_SAS_KAGERO,
		// Siden
		BT_SIDEN_MONSTER_GET_WET,
		// Kagero
		BT_KAGERO_MONSTER_LOST_US,
		// Notice Items
		BT_NOTICE_ITEM,

		BT_SPEMAX
	};

private:
	explicit CUI_Indicator_BrainTalk_Mizuha(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CUI_Indicator_BrainTalk_Mizuha() DEFAULT;

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
	void													Set_BrainTalk_Test();
	void													Shuffle_RandomNumber();

public:
	void													Set_BrainTalk_Conversation(_uint iBrainTalkIndex);
	void													Set_BrainTalk_SpecialSituation(_uint iBrainTalkIndex);

private:
	void													Conversation_First(_float fTimeDelta);
	void													Conversation_Second(_float fTimeDelta);
	void													Conversation_Third(_float fTimeDelta);
	void													Conversation_Fourth(_float fTimeDelta);
	void													Conversation_Fifth(_float fTimeDelta);
	void													Conversation_Sixth(_float fTimeDelta);
	void													Conversation_Seventh(_float fTimeDelta);
	void													Conversation_BossStage(_float fTimeDelta);
	void													Conversation_BossStage_Half(_float fTimeDelta);
	void													Conversation_BossStage_End(_float fTimeDelta);
	void													Conversation_Crows(_float fTimeDelta);

private:
	// Haruka
	void													Situation_Haruka_Player_Damaged_By_Electric(_float fTimeDelta);
	void													Situation_Haruka_Player_Damaged_By_Water(_float fTimeDelta);

	// Kasane
	void													Situation_Kasane_LevelUp(_float fTimeDelta);
	void													Situation_Kasane_Get_Item(_float fTimeDelta);

	// Use My SAS
	void													Situation_Arasi_Use_My_SAS(_float fTimeDelta);
	void													Situation_Siden_Use_My_SAS(_float fTimeDelta);
	void													Situation_Kyoka_Use_My_SAS(_float fTimeDelta);
	void													Situation_Kagero_Use_My_SAS(_float fTimeDelta);

	// Monster Wet
	void													Situation_Siden_Monster_Get_Wet(_float fTimeDelta);

	// Monster Lost Us
	void													Situation_Kagero_Monster_Lost_Us(_float fTimeDelta);

	// Notice Items
	void													Situation_Siden_Notice_Items(_float fTimeDelta);
	void													Situation_Arasi_Notice_Items(_float fTimeDelta);
	void													Situation_Kyoka_Notice_Items(_float fTimeDelta);
	void													Situation_Kagero_Notice_Items(_float fTimeDelta);

private:
#pragma region Transform
	shared_ptr<CTransform>									m_pBrainTalkFrame;
	shared_ptr<CTransform>									m_pBrainTalkFrame_Noise;
	shared_ptr<CTransform>									m_pFaceFrame;
	shared_ptr<CTransform>									m_pFaceFrame_Noise;
	shared_ptr<CTransform>									m_pBrainTalk_LightFrame;
	shared_ptr<CTransform>									m_pBrainTalk_LightFrame_Noise;
	shared_ptr<CTransform>									m_pBrainTalk_Light;
	shared_ptr<CTransform>									m_pFace_Kasane;
	shared_ptr<CTransform>									m_pFace_Siden;
	shared_ptr<CTransform>									m_pFace_Arasi;
	shared_ptr<CTransform>									m_pFace_Kyoka;
	shared_ptr<CTransform>									m_pFace_Kagero;
	shared_ptr<CTransform>									m_pFace_Haruka;
	shared_ptr<CTransform>									m_pFace_Gemma;
	shared_ptr<CTransform>									m_pFace_Naomi;
#pragma endregion

private:
#pragma region Texture
	shared_ptr<CTexture>									m_pTexture_BrainTalkFrame;
	shared_ptr<CTexture>									m_pTexture_BrainTalkFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_FaceFrame;
	shared_ptr<CTexture>									m_pTexture_FaceFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_LightFrame;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_LightFrame_Noise;
	shared_ptr<CTexture>									m_pTexture_BrainTalk_Light;
	shared_ptr<CTexture>									m_pTexture_Face_Kasane;
	shared_ptr<CTexture>									m_pTexture_Face_Siden;
	shared_ptr<CTexture>									m_pTexture_Face_Arasi;
	shared_ptr<CTexture>									m_pTexture_Face_Kyoka;
	shared_ptr<CTexture>									m_pTexture_Face_Kagero;
	shared_ptr<CTexture>									m_pTexture_Face_Haruka;
	shared_ptr<CTexture>									m_pTexture_Face_Gemma;
	shared_ptr<CTexture>									m_pTexture_Face_Naomi;
#pragma endregion

private:
#pragma region The Other Pointers
	shared_ptr<CShader>										m_pShader;
	shared_ptr<CVIBuffer_Rect>								m_pVIBuffer_Rect;
#pragma endregion

private:
#pragma region Scale
	_float3													m_vScale_BrainTalkFrame;
	_float3													m_vScale_BrainTalkFrame_Noise;
	_float3													m_vScale_FaceFrame;
	_float3													m_vScale_FaceFrame_Noise;
	_float3													m_vScale_BrainTalk_LightFrame;
	_float3													m_vScale_BrainTalk_LightFrame_Noise;
	_float3													m_vScale_BrainTalk_Light;
	_float3													m_vScale_Face_Kasane;
	_float3													m_vScale_Face_Siden;
	_float3													m_vScale_Face_Arasi;
	_float3													m_vScale_Face_Kyoka;
	_float3													m_vScale_Face_Kagero;
	_float3													m_vScale_Face_Haruka;
	_float3													m_vScale_Face_Gemma;
	_float3													m_vScale_Face_Naomi;

#pragma endregion

private:
#pragma region Position
	_float4													m_vPosition_BrainTalkFrame;
	_float4													m_vPosition_BrainTalkFrame_Noise;
	_float4													m_vPosition_FaceFrame;
	_float4													m_vPosition_FaceFrame_Noise;
	_float4													m_vPosition_BrainTalk_LightFrame;
	_float4													m_vPosition_BrainTalk_LightFrame_Noise;
	_float4													m_vPosition_BrainTalk_Light;
	_float4													m_vPosition_Face_Kasane;
	_float4													m_vPosition_Face_Siden;
	_float4													m_vPosition_Face_Arasi;
	_float4													m_vPosition_Face_Kyoka;
	_float4													m_vPosition_Face_Kagero;
	_float4													m_vPosition_Face_Haruka;
	_float4													m_vPosition_Face_Gemma;
	_float4													m_vPosition_Face_Naomi;
#pragma endregion

private:
	SPRITEUIDESC											m_tUIDesc;

private:
	BRAINTALK_CONVERSATION									m_eBrainTalk_Conversation;
	BRAINTALK_SPECIFIC_SITUATION							m_eBrainTalk_Specific_Situation;

private:
	// For. Render End (Master-Key)
	_bool													m_bDisappear;

private:
	// _bool
#pragma region For. Trigger
	_bool													m_bBrainTalk_First;
	_bool													m_bBrainTalk_Second;
	_bool													m_bBrainTalk_Third;
	_bool													m_bBrainTalk_Fourth;
	_bool													m_bBrainTalk_Fifth;
	_bool													m_bBrainTalk_Sixth;
	_bool													m_bBrainTalk_Seventh;
	_bool													m_bBrainTalk_BossStage;
	_bool													m_bBrainTalk_BossStage_Half;
	_bool													m_bBrainTalk_BossStage_End;
	_bool													m_bBrainTalk_Crows;
#pragma endregion
#pragma region For. Specific Situation
	// Haruka
	_bool													m_bBrainTalk_Haruka_Player_Damaged_By_Electric;
	_bool													m_bBrainTalk_Haruka_Player_Damaged_By_Water;

	// Kasane
	_bool													m_bBrainTalk_Kasane_LevelUp;
	_bool													m_bBrainTalk_Kasane_Get_Item;

	// Use My SAS
	_bool													m_bBrainTalk_Arasi_Use_My_SAS;
	_bool													m_bBrainTalk_Siden_Use_My_SAS;
	_bool													m_bBrainTalk_Kyoka_Use_My_SAS;
	_bool													m_bBrainTalk_Kagero_Use_My_SAS;

	// Monster Wet
	_bool													m_bBrainTalk_Siden_Monster_Get_Wet;

	// Monster Lost Us
	_bool													m_bBrainTalk_Kagero_Monster_Lost_Us;

	// Notice Items
	_bool													m_bBrainTalk_Siden_Notice_Item;
	_bool													m_bBrainTalk_Arasi_Notice_Item;
	_bool													m_bBrainTalk_Kyoka_Notice_Item;
	_bool													m_bBrainTalk_Kagero_Notice_Item;

	_bool													m_bRender_Kasane_Get_Item;
	_bool													m_bIsBrainTalkNow;
#pragma endregion
#pragma region For. Render
	_bool													m_bRender_BTLF;
	_bool													m_bRender_BTLFN;
	_bool													m_bRender_Noise;
	_bool													m_bRender_FaceFrame;
	_bool													m_bRender_BTF;
	_bool													m_bRender_BTL;

	_bool													m_bRender_Face;
	_bool													m_bRender_Face_Kasane;
	_bool													m_bRender_Face_Siden;
	_bool													m_bRender_Face_Arasi;
	_bool													m_bRender_Face_Kyoka;
	_bool													m_bRender_Face_Kagero;
	_bool													m_bRender_Face_Haruka;
	_bool													m_bRender_Face_Gemma;
	_bool													m_bRender_Face_Naomi;
#pragma endregion


private:
	_uint													m_iRandomNumber;
	_uint													m_iCheckSoundIndex;

	_uint													m_iFaceIndex_Kasane;
	_uint													m_iFaceIndex_Siden;
	_uint													m_iFaceIndex_Arasi;
	_uint													m_iFaceIndex_Kyoka;
	_uint													m_iFaceIndex_Kagero;
	_uint													m_iFaceIndex_Haruka;
	_uint													m_iFaceIndex_Gemma;
	_uint													m_iFaceIndex_Naomi;

private:
	_float													m_fAccTime;
	_float													m_fAccTime_Face_Kasane;
	_float													m_fAccTime_Face_Siden;
	_float													m_fAccTime_Face_Arasi;
	_float													m_fAccTime_Face_Kyoka;
	_float													m_fAccTime_Face_Kagero;
	_float													m_fAccTime_Face_Haruka;
	_float													m_fAccTime_Face_Gemma;
	_float													m_fAccTime_Face_Naomi;
	_float													m_fCheckSoundTime;

	_float													m_fBTLFN;
	_float													m_fBTLF;
	_float													m_fNoiseAlpha;
	_float													m_fFaceFrame;
	_float													m_fBTF;
	_float													m_fFace_KasaneAlpha;
	_float													m_fFace_SidenAlpha;
	_float													m_fFace_ArasiAlpha;
	_float													m_fFace_KyokaAlpha;
	_float													m_fFace_KageroAlpha;
	_float													m_fFace_HarukaAlpha;
	_float													m_fFace_GemmaAlpha;
	_float													m_fFace_NaomiAlpha;

private:
	wstring													m_strText1;
	wstring													m_strText2;

public:
	static shared_ptr<CUI_Indicator_BrainTalk_Mizuha>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

};

END