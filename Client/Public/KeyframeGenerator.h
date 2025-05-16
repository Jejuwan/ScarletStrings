#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

class CKeyframeGenerator abstract : public CGameObject
{
protected:
	explicit CKeyframeGenerator(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CKeyframeGenerator(const CKeyframeGenerator&);
	virtual ~CKeyframeGenerator() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

protected:
	virtual HRESULT									Ready_Components() override;

protected:
	virtual _bool									Interpolate_Keyframe(_uint iKeyframeIndex, _float fKeyframeAccTime, _float4x4& mInterpolatedMatrix);

protected:
	virtual HRESULT									Import(const wstring& wstrImportPath);
	virtual HRESULT									Export(const wstring& wstrExportPath);

#if ACTIVATE_IMGUI
	void											Render_File_Dialog();
#endif

protected:
	_uint											m_iKeyframeViewLine = { 5 };
	std::vector<std::pair<_float4x4, _float>>		m_vecKeyframes; // Transformation / Interpolation Time

protected:
	_uint											m_iCurrentSelectedKeyframe = { 0 };

protected:
	_bool											m_bPlaying = { false };
	_uint											m_iCurrentPlayingKeyframe = { 0 };
	_float											m_bCurrentKeyframeAccTime = { 0.f };

private:
#if ACTIVATE_IMGUI
	ImGuiFileDialog									m_imfdEmbed_Load;
	ImGuiFileDialog									m_imfdEmbed_Export;
#endif
};

END