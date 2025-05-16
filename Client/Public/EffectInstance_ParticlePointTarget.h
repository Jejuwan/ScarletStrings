#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CEffectInstance_ParticlePointTarget final : public CEffectInstance
{
private:
	explicit CEffectInstance_ParticlePointTarget(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_ParticlePointTarget(const CEffectInstance_ParticlePointTarget&);
	virtual ~CEffectInstance_ParticlePointTarget() DEFAULT;

public:
	virtual HRESULT											Initialize_Prototype() override;
	virtual HRESULT											Initialize(any = g_aNull) override;
	virtual void											Tick(_float fTimeDelta) override;
	virtual void											Late_Tick(_float fTimeDelta) override;
	virtual HRESULT											Render() override;

	virtual HRESULT											Render_Bloom() override;

	virtual HRESULT											Fetch(any pObject) override;
	virtual _bool											Return() override;
	virtual HRESULT											Release() override;

private:
	HRESULT													Initialize(std::ifstream&);

protected:
	virtual HRESULT											Ready_Components() override;

public:
#if ACTIVATE_TOOL
	void													Set_Data(const VFX_ParticlePointTarget);

	const VFX_ParticlePointTarget							Get_Data() const { return m_tData; }
#endif

private:
	virtual void											Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void											Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	const INSTDESC											Instance_Restart(_uint);
	const INSTDESC											Instance_End(_uint);

private:
	VFX_ParticlePointTarget									m_tData{};

	vector<INSTDESC>										m_vecInstDescs;
	vector<_float>											m_vecRotation;
	vector<_int>											m_vecSpriteIndex;

	shared_ptr<CTransform>									m_pTargetTransform;
	shared_ptr<CModel>										m_pTargetModel;

	unique_ptr<_float4x4[]>									m_pBoneMatrices;

public:
	static shared_ptr<CEffectInstance_ParticlePointTarget>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>							Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_ParticlePointTarget>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT											Export(const wstring& wstrFilePath) override;
#endif
};

END
