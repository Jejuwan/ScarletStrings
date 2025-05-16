#include "EnginePCH.h"
#include "SceneCapture.h"

#include "GameInstance.h"
#include "Renderer.h"

HRESULT CSceneCapture::Initialize(const ComPtr<ID3D11Device>& _pDevice, const ComPtr<ID3D11DeviceContext>& _pContext,
	const ComPtr<ID3D11Texture2D>& _pBackBuffer, const ComPtr<ID3D11RenderTargetView>& _pBackBufferRTV)
{
	m_pDevice = _pDevice;
	m_pContext = _pContext;

	m_pBackBuffer = _pBackBuffer;
	m_pBackbufferRenderTargetView = _pBackBufferRTV;

	D3D11_TEXTURE2D_DESC tCaptureTextureDesc;
	_pBackBuffer->GetDesc(&tCaptureTextureDesc);
	tCaptureTextureDesc.MipLevels = tCaptureTextureDesc.ArraySize = 1;
	tCaptureTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tCaptureTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	tCaptureTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tCaptureTextureDesc.MiscFlags = 0;
	tCaptureTextureDesc.CPUAccessFlags = 0;
	tCaptureTextureDesc.SampleDesc.Count = 1;
	tCaptureTextureDesc.SampleDesc.Quality = 0;

	for (_uint i = 0; i < (_uint)CUTSCENE_CHANNEL::MAX; ++i)
	{
		ComPtr<ID3D11Texture2D>				pCaptureTexture;
		ComPtr<ID3D11ShaderResourceView>	pCaptureShaderResourceView;
		ComPtr<ID3D11RenderTargetView>		pCaptureRenderTargetView;

		if (FAILED(m_pDevice->CreateTexture2D(&tCaptureTextureDesc, nullptr, pCaptureTexture.GetAddressOf())))
			assert(false && L"CreateTexture2D Failed");

		if (FAILED(m_pDevice->CreateShaderResourceView(pCaptureTexture.Get(), nullptr, pCaptureShaderResourceView.GetAddressOf())))
			assert(false && L"CreateShaderResourceView Failed");

		if (FAILED(m_pDevice->CreateRenderTargetView(pCaptureTexture.Get(), nullptr, pCaptureRenderTargetView.GetAddressOf())))
			assert(false && L"CreateRenderTargetView Failed");

		m_pCapturedTectures[i] = { pCaptureShaderResourceView, pCaptureRenderTargetView };
	}

	return S_OK;
}

void CSceneCapture::Tick(_float fTimeDelta)
{
}

void CSceneCapture::Render()
{
}

void CSceneCapture::Ready(shared_ptr<class CRenderer> pRenderer)
{
	pRenderer->Subscribe_Render_Finish_Callback(std::bind(&CSceneCapture::OnRenderFinished, this));
}

void CSceneCapture::Capture(CUTSCENE_CHANNEL eChannel)
{
	if (false == m_bCaptureTrigger)
	{
		m_eCaptureTarget = eChannel;
		m_bCaptureTrigger = true;
	}
}

void CSceneCapture::Record(CUTSCENE_CHANNEL eChannel)
{
	m_bRecording = true;
	m_eRecordTarget = eChannel;
}

void CSceneCapture::Stop_Record()
{
	m_bRecording = false;
}

HRESULT CSceneCapture::Bind_ShaderResourceView(const shared_ptr<class CShader> pShader, const _char* pConstantName, CUTSCENE_CHANNEL eChannel)
{
	auto& CaptureTexture = m_pCapturedTectures[_uint(eChannel)];
	if (FAILED(pShader->Bind_ShaderResourceView(pConstantName, CaptureTexture.first)))
	{
		return E_FAIL;
	}

	return S_OK;
}

ComPtr<ID3D11ShaderResourceView> CSceneCapture::Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL eChannel)
{
	return m_pCapturedTectures[_uint(eChannel)].first;
}

void CSceneCapture::Subscribe_Capture_Success_Callback(const std::function<void()>& fpCaptureSuccess)
{
	m_fpOnCaptureSuccess = fpCaptureSuccess;
}

void CSceneCapture::OnRenderFinished()
{
	if (m_bCaptureTrigger)
	{
		const pair<ComPtr<ID3D11ShaderResourceView>, ComPtr<ID3D11RenderTargetView>>& tCaptureTexturePair 
			= m_pCapturedTectures[(_uint)m_eCaptureTarget];

		ComPtr<ID3D11Resource> pBackBufferRTV, pCaptureRTV;
		m_pBackbufferRenderTargetView->GetResource(pBackBufferRTV.GetAddressOf());
		tCaptureTexturePair.second->GetResource(pCaptureRTV.GetAddressOf());
		m_pContext->CopyResource(pCaptureRTV.Get(), pBackBufferRTV.Get());

		if (m_fpOnCaptureSuccess)
			m_fpOnCaptureSuccess();

		m_bCaptureTrigger = false;
	}

	if (m_bRecording)
	{
		const pair<ComPtr<ID3D11ShaderResourceView>, ComPtr<ID3D11RenderTargetView>>& tCaptureTexturePair
			= m_pCapturedTectures[(_uint)m_eRecordTarget];

		ComPtr<ID3D11Resource> pBackBufferRTV, pRecordRTV;
		m_pBackbufferRenderTargetView->GetResource(pBackBufferRTV.GetAddressOf());
		tCaptureTexturePair.second->GetResource(pRecordRTV.GetAddressOf());
		m_pContext->CopyResource(pRecordRTV.Get(), pBackBufferRTV.Get());
	}
}
