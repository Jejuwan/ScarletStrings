#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CSceneCapture final : public CSingleton<CSceneCapture>
{
private:
	explicit CSceneCapture() DEFAULT;
	virtual ~CSceneCapture() DEFAULT;

public:
	HRESULT	Initialize(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&,
		const ComPtr<ID3D11Texture2D>&, const ComPtr<ID3D11RenderTargetView>&);
	void Tick(_float fTimeDelta);
	void Render();

public:
	void Ready(shared_ptr<class CRenderer> pRenderer);

	void Capture(CUTSCENE_CHANNEL eChannel);

	void Record(CUTSCENE_CHANNEL eChannel);
	void Stop_Record();

public:
	HRESULT Bind_ShaderResourceView(const shared_ptr<class CShader> pShader, const _char* pConstantName, CUTSCENE_CHANNEL eChannel);
	ComPtr<ID3D11ShaderResourceView> Get_Channel_ShaderResourceView(CUTSCENE_CHANNEL eChannel);

public:
	void Subscribe_Capture_Success_Callback(const std::function<void()>& fpCaptureSuccess);
private:
	std::function<void()> m_fpOnCaptureSuccess = { nullptr };

private:
	void OnRenderFinished();

private:
	pair<ComPtr<ID3D11ShaderResourceView>, ComPtr<ID3D11RenderTargetView>> m_pCapturedTectures[IDX(CUTSCENE_CHANNEL::MAX)];

	_bool									m_bCaptureTrigger;
	CUTSCENE_CHANNEL						m_eCaptureTarget;

	_bool									m_bRecording;
	CUTSCENE_CHANNEL						m_eRecordTarget;

private:
	ComPtr<ID3D11Device>					m_pDevice;
	ComPtr<ID3D11DeviceContext>				m_pContext;

private:
	ComPtr<ID3D11Texture2D>		   m_pBackBuffer;
	ComPtr<ID3D11RenderTargetView> m_pBackbufferRenderTargetView;

	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	friend CSingleton<CSceneCapture>;
};

END