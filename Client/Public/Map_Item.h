#pragma once

#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMap_Item : public CGameObject
{
public:
	enum class ITEMSTATE { STATE_WAIT, STATE_GET, STATE_END };

private:
	explicit CMap_Item(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Item(const CMap_Item&);
	virtual ~CMap_Item() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Bloom();
	virtual HRESULT						Render_Neon();

	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;
	virtual HRESULT						Render_OutLine() override;

private:
	virtual HRESULT						Ready_Components() override;
	void	Using_Map_Tool();
	void	Item_State(_float fTimeDelta);
	virtual _bool						Return() override;
	void	Start_Effect();
private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CModel>					m_pModel;
	shared_ptr<CRenderer>				m_pRenderer;

private:
	_float4								m_vItemLineColor = { 3.5f, 3.5f, 3.5f, 1.f };
	_float2								m_vTilingFactor = { 10.f, 10.f };
	_float								m_fBloomStrengh = { 1.5f };
	_float								m_fTimeFactor = { 0.f };
	_float								m_fSeed0 = { 0.f };
	_float								m_fSeed1 = { 0.f };

private:
	shared_ptr<CShader>					m_pGlitchShader;
	shared_ptr<CTexture>				m_pGlitchTexture;
	shared_ptr<CTexture>				m_pMaskTexture;
	shared_ptr<CVIBuffer_Rect>			m_pGlitchBuffer;

	_float4x4							m_matPivot;

	_float4								m_fGlitchBaseColor = { 1.f, 6.4f, 1.f, 1.f };
	_float								m_fGlitchBloomStrength = { 0.3f };

private:
	_bool	m_bSceneMapTool = { false };

	_float m_fTimer = { 0.f };
	_float m_fEffectTimer = { 0.f };
	_float m_fTemp = { 1.f };

	_int	m_iIndex = { 0 };

	wstring m_strName;
	wstring m_strSelectedName;
	wstring m_strObjName;
	wstring m_strIndexName;
	wstring m_strNameNum;

	/* Scale, Rotation, Translation, matrix */
	tuple<wstring,_float3, _float3, _float3, _float4x4> m_tuplePivot;

	_float3 m_Scale = { 0.f,0.f,0.f };
	_float3 m_Rotation = { 0.f,0.f,0.f };
	_float3 m_Translation = { 0.f,0.f,0.f };
	_float4x4 m_Mat;

	_int m_iMeshIndex = { 0 };

	// TODO: Demo Code!#!@#$%#%^$%^$%#$%^$Y%$%^$@%^%$$&#$&^*
	_bool m_bDead = { false };  // 나중에 지워야함@!@!

	ITEMSTATE m_eItemState = { ITEMSTATE::STATE_END };

private:
	_bool								m_bNoticeItem;
	_uint								m_iSoundCounter;

public:
	static shared_ptr<CMap_Item>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

};

END
