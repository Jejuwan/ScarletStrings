#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Engine)
class CMesh;
class ICurves;
class CCurve;
class CSpline;
class CEase;
END

BEGIN(Client)

class CEffectInstance_ParticleMesh final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticleMesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticleMesh(const CEffectInstance_ParticleMesh&);
	virtual ~CEffectInstance_ParticleMesh() DEFAULT;

public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any = g_aNull) override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;

	virtual HRESULT											Render_Bloom() override;

	virtual HRESULT											Fetch(any = g_aNull) override;
	virtual _bool											Return() override;
	virtual HRESULT											Release() override;

private:
	HRESULT													Initialize(std::ifstream&);

protected:
	virtual HRESULT											Ready_Components() override;

public:
	virtual HRESULT											Add_Component(const COMPONENT, shared_ptr<class CComponent> = nullptr) override;

public:
#if ACTIVATE_TOOL
	void													Set_Data(const VFX_ParticleMesh);
	const VFX_ParticleMesh									Get_Data() const { return m_tData; }

	shared_ptr<class CCurveContainer>						Get_InstLifeTimeCurve() { return m_pCCInstLifeTime; }
	shared_ptr<class CCurveContainer>						Get_StartCurve() { return m_pCCStart; }
	shared_ptr<class CCurveContainer>						Get_Speed() { return m_pCCSpeed; }
	shared_ptr<class CCurveContainer>						Get_GravityScaleCurve() { return m_pCCGravityScale; }
	shared_ptr<class CCurveContainer>						Get_ScaleCurve() { return m_pCCScale; }
	shared_ptr<class CCurveContainer>						Get_BloomCurve() { return m_pCCBloom; }
	shared_ptr<class CCurveContainer>						Get_FadeCurve() { return m_pCCFade; }
	shared_ptr<class CCurveContainer>						Get_RotationCurve() { return m_pCCRotation; }

	void													Set_InstLifeTimeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCInstLifeTime = pCC; }
	void													Set_StartCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCStart = pCC; }
	void													Set_Speed(shared_ptr<class CCurveContainer> pCC) { m_pCCSpeed = pCC; }
	void													Set_GravityScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCGravityScale = pCC; }
	void													Set_ScaleCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCScale = pCC; }
	void													Set_BloomCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCBloom = pCC; }
	void													Set_FadeCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCFade = pCC; }
	void													Set_RotationCurve(shared_ptr<class CCurveContainer> pCC) { m_pCCRotation= pCC; }
#endif

private:
	virtual void											Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void											Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	VFX_ParticleMesh										m_tData{};

	vector<_bool>											m_vecInstFetch;
	vector<_float>											m_vecInstTimeAcc;
	vector<_float>											m_vecInstLifeTime;
	vector<_float3>											m_vecInstVelocity;
	vector<_float4>											m_vecInstRotation;

	shared_ptr<class CCurveContainer>						m_pCCInstLifeTime;
	shared_ptr<class CCurveContainer>						m_pCCStart;
	shared_ptr<class CCurveContainer>						m_pCCSpeed;
	shared_ptr<class CCurveContainer>						m_pCCGravityScale;
	shared_ptr<class CCurveContainer>						m_pCCScale;
	shared_ptr<class CCurveContainer>						m_pCCBloom;
	shared_ptr<class CCurveContainer>						m_pCCFade;
	shared_ptr<class CCurveContainer>						m_pCCRotation;

	_bool													m_bLinearStart	= false;

public:
	static shared_ptr<CEffectInstance_ParticleMesh>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticleMesh>			Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT											Export(const wstring& wstrFilePath) override;
#endif
};

END
