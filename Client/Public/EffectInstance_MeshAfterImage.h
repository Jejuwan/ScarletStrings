#pragma once
#include "Client_Define.h"
#include "EffectInstance.h"

#define	TEMP_AFTERIMAGE
//#define	LUT_SKINNING

BEGIN(Engine)
class CLookUpTable;
END

BEGIN(Client)

class CEffectInstance_MeshAfterImage final : public CEffectInstance
{
private:
	explicit CEffectInstance_MeshAfterImage(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEffectInstance_MeshAfterImage(const CEffectInstance_MeshAfterImage&);
	virtual ~CEffectInstance_MeshAfterImage() DEFAULT;

public:
	virtual HRESULT										Initialize_Prototype() override;
	virtual HRESULT										Initialize(any = g_aNull) override;
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

	virtual HRESULT										Render_Bloom() override;

	virtual HRESULT										Fetch(any pObject) override;
	virtual _bool										Return() override;
	virtual HRESULT										Release() override;

private:
	HRESULT												Initialize(std::ifstream&);

public:
#if ACTIVATE_TOOL
	void												Set_Data(const VFX_MeshAfterImage);

	const VFX_MeshAfterImage							Get_Data() const	{ return m_tData; }

	void												Set_ToolInformation(shared_ptr<CTransform>, shared_ptr<CModel>);
#endif

private:
	virtual void										Fetch_Instance(void*, _uint iNumInstance, any arg = g_aNull);
	virtual	void										Update_Instance(void*, _uint iNumInstance, _float fTimeDelta);

private:
	HRESULT												Update_BoneData(_uint iInstanceIndex, _uint iMeshIndex);
	
private:
	_float												m_fInstanceAcc		= 0.f;
	_float												m_fAfterImageAcc	= 0.f;
	_uint												m_iInstanceCursor	= 0;

	vector<_float4x4>									m_vecMatrices;
	vector<_float>										m_vecInstanceWeight;

	_bool												m_bUpdate			= false;
	
	shared_ptr<CGameObject>								m_pTarget;
	shared_ptr<CModel>									m_pTargetModel;
	shared_ptr<CTransform>								m_pTargetTransform;
	_float4x4											m_mTargetPivot		= g_mUnit;

	VFX_MeshAfterImage									m_tData{};

#ifdef LUT_SKINNING
	shared_ptr<class CLookUpTable>						m_pBoneInstanceLUT;
#elif !defined(TEMP_AFTERIMAGE)
	ComPtr<ID3D11Buffer>								m_pBoneInstanceBuffer;
	ComPtr<ID3D11ShaderResourceView>					m_pBoneInstanceSRV;
#endif
	unique_ptr<_float4x4[]>								m_pBoneData;

public:
	static shared_ptr<CEffectInstance_MeshAfterImage>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>						Clone(any = g_aNull) override;

	static shared_ptr<CEffectInstance_MeshAfterImage>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	virtual HRESULT										Export(const wstring& wstrFilePath) override;
#endif
};

END
