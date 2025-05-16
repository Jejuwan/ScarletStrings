#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CEffectInstance_TrailQuadDiffuse final : public CEffectInstance
{
#if ACTIVATE_TOOL
public:
#else
private:
#endif
	enum class QUADPOINT	{ BONE, LOCALPIVOT, MAX };

private:
	explicit CEffectInstance_TrailQuadDiffuse(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_TrailQuadDiffuse(const CEffectInstance_TrailQuadDiffuse&);
	virtual ~CEffectInstance_TrailQuadDiffuse() DEFAULT;

public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any = g_aNull) override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;

	virtual HRESULT											Render_Bloom() override;

	virtual HRESULT											Fetch(any tuple_pTarget_szBones_tuple_pTarget_szBone_mLocalPivots) override;
	virtual _bool											Return() override;
	virtual HRESULT											Release() override;

private:
	HRESULT													Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void													Set_Data(const VFX_TrailQuad);

	const VFX_TrailQuad										Get_Data() const	{ return m_tData; }
#endif

private:
	virtual void											Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void											Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	_float													m_fInstanceAcc			= 0.f;
	deque<pair<_float4, _float4>>							m_deqQuad;
	vector<_float>											m_vecInstanceWeight;	

	_bool													m_bUpdate				= false;
	_bool													m_bItinerant			= false;

	_uint													m_iFinalInstance		= 0;
	_bool													m_bFinalInstance		= false;

	shared_ptr<CTransform>									m_pTargetTransform;
	_float4x4												m_mTargetPivot			= g_mUnit;
	pair<const _float4x4*, const _float4x4*>				m_pTargetBones			= make_pair(nullptr, nullptr);
	const _float4x4*										m_pTargetBone			= nullptr;
	pair<_float4x4, _float4x4>								m_mTargetLocalPivots	= make_pair(_float4x4{}, _float4x4{});

	VFX_TrailQuad											m_tData{};

	QUADPOINT												m_eType					= QUADPOINT::MAX;

public:
	static shared_ptr<CEffectInstance_TrailQuadDiffuse>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_TrailQuadDiffuse>		Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT											Export(const wstring& wstrFilePath) override;
#endif
};

END
