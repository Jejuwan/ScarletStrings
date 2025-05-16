#include "EnginePCH.h"
#include "ImageProcess_Manager.h"

#include "Postprocess.h"
#include "Engine_Macro.h"

CImageProcess_Manager::CImageProcess_Manager()
{
	for (_uint i = 0; i < IDX(IMAGE_PROCESS::MAX); ++i)
	{
		m_pImageProcessor[i] = make_private_shared(CPostprocess);
	}
}

void CImageProcess_Manager::Initialize(IMAGE_PROCESS eProcessType, const ComPtr<ID3D11Device>& _pDevice, const ComPtr<ID3D11DeviceContext>& _pContext,
	const ComPtr<ID3D11Texture2D>& _pTexture, const ComPtr<ID3D11RenderTargetView>& _pRenderTargetView, 
	const _int iWidth, const _int iHeight, const _int iBloomLevels)
{
	m_pImageProcessor[IDX(eProcessType)]->Initialize(_pDevice, _pContext, _pTexture, _pRenderTargetView, iWidth, iHeight, iBloomLevels, eProcessType);
}

void CImageProcess_Manager::Initialize(IMAGE_PROCESS eProcessType, const ComPtr<ID3D11Device>& pDevice, const ComPtr<ID3D11DeviceContext>& pContext, 
	const ComPtr<ID3D11Texture2D>& pTexture, const ComPtr<ID3D11RenderTargetView>& pRenderTargetView, 
	const _int iWidth, const _int iHeight, const _int iBloomLevels, _float fBloomStrength, _float fExposure, _float fGamma)
{
	Initialize(eProcessType, pDevice, pContext, pTexture, pRenderTargetView, iWidth, iHeight, iBloomLevels);

	Set_BloomStrength(eProcessType, fBloomStrength);
	Set_Exposure(eProcessType, fExposure);
	Set_Gamma(eProcessType, fGamma);
}

void CImageProcess_Manager::Tick(_float _fTimeDelta)
{
	for (_uint i = 0; i < IDX(IMAGE_PROCESS::MAX); ++i)
	{
		if (m_pImageProcessor[i])
			m_pImageProcessor[i]->Tick(_fTimeDelta);
	}
}

void CImageProcess_Manager::Excute_ImageProcess(IMAGE_PROCESS eProcessType, _uint iFilterPass, function<HRESULT(shared_ptr<CShader>)> _fpListner)
{
	if (eProcessType == IMAGE_PROCESS::PROCESS_TONEMAPPING
		&& m_bLUTFilter)
	{
		m_pImageProcessor[IDX(eProcessType)]->Render(eProcessType, 5, m_iLUTIndex);
	}
	else
	{
		m_pImageProcessor[IDX(eProcessType)]->Render(eProcessType, iFilterPass, _fpListner);
	}
}

void CImageProcess_Manager::FadeInExposure(IMAGE_PROCESS eProcessType, _float fEndExposure, _float fTimeScale)
{
	m_pImageProcessor[IDX(eProcessType)]->FadeInExposure(fEndExposure, fTimeScale);
}

void CImageProcess_Manager::FadeInGamma(IMAGE_PROCESS eProcessType, _float fEndGamma, _float fTimeScale)
{
	m_pImageProcessor[IDX(eProcessType)]->FadeInGamma(fEndGamma, fTimeScale);
}

void CImageProcess_Manager::FadeOutExposure(IMAGE_PROCESS eProcessType, _float fEndExposure, _float fTimeScale)
{
	m_pImageProcessor[IDX(eProcessType)]->FadeOutExposure(fEndExposure, fTimeScale);
}

void CImageProcess_Manager::FadeOutGamma(IMAGE_PROCESS eProcessType, _float fEndGamma, _float fTimeScale)
{
	m_pImageProcessor[IDX(eProcessType)]->FadeOutGamma(fEndGamma, fTimeScale);
}

void CImageProcess_Manager::Set_Exposure(IMAGE_PROCESS eProcessType, _float fExposure)
{
	m_pImageProcessor[IDX(eProcessType)]->Set_Exposure(fExposure);
}

void CImageProcess_Manager::Set_Gamma(IMAGE_PROCESS eProcessType, _float fGamma)
{
	m_pImageProcessor[IDX(eProcessType)]->Set_Gamma(fGamma);
}

void CImageProcess_Manager::Set_BloomStrength(IMAGE_PROCESS eProcessType, _float fStrength)
{
	m_pImageProcessor[IDX(eProcessType)]->Set_BloomStrength(fStrength);
}

_float CImageProcess_Manager::Get_Exposure(IMAGE_PROCESS eProcessType)
{
	return m_pImageProcessor[IDX(eProcessType)]->Get_Exposure();
}

_float CImageProcess_Manager::Get_Gamma(IMAGE_PROCESS eProcessType)
{
	return m_pImageProcessor[IDX(eProcessType)]->Get_Gamma();
}

_float CImageProcess_Manager::Get_BloomStrength(IMAGE_PROCESS eProcessType)
{
	return m_pImageProcessor[IDX(eProcessType)]->Get_BloomStrength();
}

shared_ptr<class CShader> CImageProcess_Manager::Get_PostShader(IMAGE_PROCESS eProcessType)
{
	return m_pImageProcessor[IDX(eProcessType)]->Get_Shader();
}

void CImageProcess_Manager::MaskingLUT(_bool bMasking, shared_ptr<CTexture> pMaskingTexture)
{
	m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->MaskingLUT(bMasking, pMaskingTexture);
}

void CImageProcess_Manager::Enable_FXAA(_bool _bEnable)
{
	m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Enable_FXAA(_bEnable);
}

_bool CImageProcess_Manager::Enable_FXAA()
{
	return m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Enable_FXAA();
}

void CImageProcess_Manager::Enable_DOF(_bool _bEnable)
{
	m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Enable_DOF(_bEnable);
}

_bool CImageProcess_Manager::Enable_DOF()
{
	return m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Enable_DOF();
}

void CImageProcess_Manager::Set_DOF_DepthStart(_float fDepthStart)
{
	m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Set_DepthStart(fDepthStart);
}

void CImageProcess_Manager::Set_DOF_DepthRange(_float fDepthRange)
{
	m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Set_DepthRange(fDepthRange);
}

_float CImageProcess_Manager::Get_DOF_DepthStart()
{
	return m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Get_DepthStart();
}

_float CImageProcess_Manager::Get_DOF_DepthRange()
{
	return m_pImageProcessor[IDX(IMAGE_PROCESS::PROCESS_TONEMAPPING)]->Get_DepthRange();
}
