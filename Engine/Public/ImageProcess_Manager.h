#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CImageProcess_Manager final : public CSingleton<CImageProcess_Manager>
{
private:
	explicit CImageProcess_Manager();
	virtual ~CImageProcess_Manager() DEFAULT;


public:
	void Initialize(IMAGE_PROCESS, const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&,
		const ComPtr<ID3D11Texture2D>&, const ComPtr<ID3D11RenderTargetView>&,
		const _int iWidth, const _int iHeight, const _int iBloomLevels);

	void Initialize(IMAGE_PROCESS, const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&,
		const ComPtr<ID3D11Texture2D>&, const ComPtr<ID3D11RenderTargetView>&,
		const _int iWidth, const _int iHeight, const _int iBloomLevels, _float fBloomStrength, _float fExposure, _float fGamma);

	void Tick(_float _fTimeDelta);
	void Excute_ImageProcess(IMAGE_PROCESS eProcessType, _uint iFilterPass = 2, function<HRESULT(shared_ptr<class CShader>)> = nullptr);

public:
#pragma region Postprocess
	// Tonemapping Settings
public:
	void FadeInExposure(IMAGE_PROCESS, _float fEndExposure, _float fTimeScale);
	void FadeInGamma(IMAGE_PROCESS, _float fEndGamma, _float fTimeScale);
	void FadeOutExposure(IMAGE_PROCESS, _float fEndExposure, _float fTimeScale);
	void FadeOutGamma(IMAGE_PROCESS, _float fEndGamma, _float fTimeScale);

	void Set_Exposure(IMAGE_PROCESS, _float fExposure);
	void Set_Gamma(IMAGE_PROCESS, _float fGamma);
	void Set_BloomStrength(IMAGE_PROCESS, _float fStrength);

	_float Get_Exposure(IMAGE_PROCESS);
	_float Get_Gamma(IMAGE_PROCESS);
	_float Get_BloomStrength(IMAGE_PROCESS);
#pragma endregion

#pragma region LUT Filter
	void	Enable_LUTFilter(_bool bEnable) { m_bLUTFilter = bEnable; }
	_bool	Enable_LUTFilter() { return m_bLUTFilter; }
	void	Set_LUTIndex(_uint iLUTIndex) { m_iLUTIndex = iLUTIndex; }
	_uint	Get_LUTIndex() { return m_iLUTIndex; }

	void	MaskingLUT(_bool bMasking, shared_ptr<class CTexture> = nullptr);
#pragma endregion

#pragma region Bloom

#pragma endregion

#pragma region FXAA
	void Enable_FXAA(_bool);
	_bool Enable_FXAA();
#pragma endregion

#pragma region DOF
	void	Enable_DOF(_bool);
	_bool	Enable_DOF();

	void Set_DOF_DepthStart(_float fDepthStart);
	void Set_DOF_DepthRange(_float fDepthRange);

	_float Get_DOF_DepthStart();
	_float Get_DOF_DepthRange();
#pragma endregion

public:
	shared_ptr<class CShader>				Get_PostShader(IMAGE_PROCESS);

private:
	shared_ptr<class CPostprocess>			m_pImageProcessor[IDX(IMAGE_PROCESS::MAX)];

private:
	_bool									m_bLUTFilter = { false };
	_uint									m_iLUTIndex = { 0 };

	friend CSingleton<CImageProcess_Manager>;
};

END