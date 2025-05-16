#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public std::enable_shared_from_this<CAnimation>
{
private:
	explicit CAnimation() DEFAULT;
	explicit CAnimation(const CAnimation&);
	virtual ~CAnimation() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT								Initialize_FromAssimp(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
#endif
	HRESULT								Initialize_FromBinary(std::ifstream&);
	HRESULT								Initialize();
	void								Tick(_float fTimeDelta, vector<shared_ptr<class CBone>>::iterator itBegin, _bool bReverse = false, _bool bLoop = true);


public:
	_bool								Is_ChannelExist(_uint iBoneIndex) const;
	_bool								Is_Finished() const { return !m_bReverse ? m_fTrackPosition >= m_fDuration : m_fTrackPosition <= 0.f; }
	_float								Get_Duration() const { return m_fDuration; }
	void								AddEvent(_uint TrackPosition, shared_ptr<class CEvent> _pEvent);
	void								Bind_Event(EVENTTYPE EventType, function<void(void)> Function);

#if ACTIVATE_TOOL
	_uint								Get_NumChannels() const { return static_cast<_uint>(m_vecChannels.size()); }
#endif

	const _char* Get_Name() const { return m_szName; }
	shared_ptr<class CChannel>			Get_Channel(_uint iChannelIndex, _bool bBoneIndex = false) const;
#if ACTIVATE_TOOL
	_uint								Get_ChannelIndex(_uint iBoneIndex) const;
#endif
	map<_uint, vector<shared_ptr<class CEvent>>>& Get_Events() { return m_mapEvents; }
public:
	void								Set_AnimationIndex(vector<_uint>::iterator itBegin, _uint iAnimationIndex);

	void								Fix(vector<shared_ptr<class CBone>>::iterator itBegin, _float fRatio);
	void								Blend(vector<shared_ptr<CAnimation>>::iterator itAnimationBegin, vector<shared_ptr<class CBone>>::iterator itBoneBegin, vector<_uint>::iterator itBoneAnimationBegin, _float fRatio);
	void								Reset();
	void								Interpolate_TransformationMatrices(_float fTimeDelta, shared_ptr<CAnimation> pAnim, vector<shared_ptr<class CBone>>& itBoneBegin, _float InterpolateTime);
	

#if ACTIVATE_TOOL
	HRESULT								Remove_Channel(_uint iChannelIndex);
#endif
	void								SetRootBoneIndex(_uint Reference, _uint RootBone);
	void								SetRootBoneAddIndex(_uint RootBoneAdd);
	void								MinusEventCount() { --m_iNumEvents; };

private:
	_char								m_szName[MAX_PATH] = "";

	_bool								m_bReverse = false;
	_bool								m_bInterpolateEnd = false;
	_bool								m_bTrash = false;

	_float								m_fDuration = 0.f;
	_float								m_fTrackPosition = 0.f;
	_float								m_fTicksPerSecond = 0.f;
	_float								m_fAnimSpeed = 1.f;

	_uint								m_iRootBoneIndex = { 0 };
	_int								m_iRootBoneAddIndex = { -1 };
	_uint								m_iNumChannels = 0;
	vector<shared_ptr<class CChannel>>	m_vecChannels;
	vector<_uint>						m_vecChannelKeyFrames;

	_float4								m_vRootPos;
	_float4								m_vRootPosAdd;
	_float4x4							m_mRootMatrix;
	_float								m_fProportion = 0.f; //0~1
	_float								m_fInterpolatePosition = 0;

	_uint														m_iNumEvents = 0;
	map<_uint, vector<shared_ptr<class CEvent>>>				m_mapEvents;
	map<_uint, vector<shared_ptr<class CEvent>>>::iterator		m_EventIter;
public:
#if ACTIVATE_TOOL
	static shared_ptr<CAnimation>		Create(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
#endif
	static shared_ptr<CAnimation>		Read(std::ifstream&);
	shared_ptr<CAnimation>				Clone();

#if ACTIVATE_TOOL
	void								Export(std::ofstream&);
	void								Export(std::ofstream&, _uint iChannelIndex);
#endif

	_float4								Get_RootPos() { return m_vRootPos; }
	_float4								Get_RootPosAdd() { return m_vRootPosAdd; }
	_float4x4							Get_RootMatrix() { return m_mRootMatrix; }
	_float								Get_Proportion() { return m_fProportion; }
	_float								Get_TrackPosition() { return m_fTrackPosition; }
	_float								Get_Duration() { return m_fDuration; }
	_bool								Get_InterpolateEnd() { return m_bInterpolateEnd; }
	void								Set_InterpolateEnd(_bool b) { m_bInterpolateEnd = b; }
	void								Set_AnimSpeed(_float f) { m_fAnimSpeed = f; };
	void								Set_TrackPosition(_float f);
	_bool								Get_Trash() { return m_bTrash; }
	void								Set_Trash(_bool b) { m_bTrash = b; }
};

END