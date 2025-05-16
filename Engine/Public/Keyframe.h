#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CKeyframe final : public CComponent
{
private:
	explicit CKeyframe(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CKeyframe(const CKeyframe&);
	virtual ~CKeyframe() DEFAULT;

public:
	HRESULT												Initialize_Prototype();
	HRESULT												Initialize();
	void												Tick(shared_ptr<class CTransform>, _float fTimeDelta);

public:
	void												Append_Keyframe(const wstring& wstrFilePath, const wstring& wstrAnimTag);
	void												Clear_Keyframe();

public:
	_bool												Play_Animation(const wstring& wstrAnimTag, const _float4x4& matPivot);
	void												Pause_Animation();
	void												Continue_Animation();

public:
	void												Subscribe_Animation_Finish_Callback(const std::function<void(const wstring&)>& fpCallback);
private:
	std::function<void(const wstring&)>					m_fpOnAnimFinish = { nullptr };


private:
	void												Interpolate_Keyframe(_uint iKeyframeIndex, _float fTime, 
																		const std::vector<std::pair<_float4x4, _float>>& vecKeyframe, 
																		shared_ptr<class CTransform> pTransform);

private:
	unordered_map<wstring, std::vector<std::pair<_float4x4, _float>>> m_mapKeyframePair;

	_bool												m_bPlaying = { false };
	_uint												m_iCurrentPlayingKeyframe = { 0 };
	_float												m_bCurrentKeyframeAccTime = { 0.f };

	wstring												m_wstrCurrentKeyframeTag;

	_float4x4											m_matPivot;

public:
	static shared_ptr<CKeyframe>						Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>						Clone(any) override;
};

END