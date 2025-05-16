#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Client)

class CEffectInstance_TrailLineDiffuse final : public CEffectInstance
{
private:
	explicit CEffectInstance_TrailLineDiffuse(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_TrailLineDiffuse(const CEffectInstance_TrailLineDiffuse&);
	virtual ~CEffectInstance_TrailLineDiffuse() DEFAULT;
	
public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any = g_aNull) override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

	virtual HRESULT										Render_Bloom() override;

	virtual HRESULT										Fetch(any pair_pObject_szBone) override;
	virtual _bool										Return() override;
	virtual HRESULT										Release() override;

private:
	HRESULT												Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void												Set_Data(const VFX_TrailLineDiffuse);

	const VFX_TrailLineDiffuse							Get_Data() const						{ return m_tData; }
#endif

private:
	virtual void										Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void										Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

	
private:
	_float												m_fInstanceAcc		= 0.f;

	_uint												m_iInstanceCursor	= 0;
	deque<_float4>										m_deqLine;
	vector<_float>										m_vecInstanceWeight;

	_bool												m_bUpdate			= false;
	_bool												m_bItinerant		= false;
	
	shared_ptr<CGameObject>								m_pTarget;
	shared_ptr<CTransform>								m_pTargetTransform;
	const _float4x4*									m_pTargetBone		= nullptr;
	_float4x4											m_mTargetPivot		= g_mUnit;

	VFX_TrailLineDiffuse								m_tData{};

public:
	static shared_ptr<CEffectInstance_TrailLineDiffuse>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_TrailLineDiffuse>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT										Export(const wstring& wstrFilePath) override;
#endif
};

END
