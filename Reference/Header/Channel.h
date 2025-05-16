#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CChannel final
{
private:
	explicit CChannel() DEFAULT;
	virtual ~CChannel() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT						Initialize_FromAssimp(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	HRESULT						Initialize_FromBinary(std::ifstream&);

public:
	_uint						Get_BoneIndex() const { return m_iBoneIndex; }
	_uint						Get_NumKeyFrames() const { return m_iNumKeyFrames; }
	KEYFRAME					Get_KeyFrame(_uint iIndex) { return m_vecKeyFrames[iIndex]; }
	vector<KEYFRAME>&			Get_KeyFrameRef() { return m_vecKeyFrames; }
#if ACTIVATE_TOOL
	void						Set_KeyFrame(_uint iIndex, KEYFRAME tKeyFrame) { m_vecKeyFrames[iIndex] = tKeyFrame; }
#endif

public:
	void						Update_Transformation(vector<shared_ptr<class CBone>>::iterator itBegin, _uint& iCurrentKeyFrame, _float fTrackPosition);
	void						Blend_Transformation(vector<shared_ptr<class CBone>>::iterator itBegin, _uint iKeyFrame, _float fTrackPosition, shared_ptr<class CAnimation> pBase);
	void						Interpolate_Transformation(shared_ptr<class CChannel> nextChannel,vector<shared_ptr<class CBone>>& itBegin, _uint iKeyFrame, _float fTrackPosition, _float InterpolateTime);
	void						SetRootBoneIndex(_uint Reference, _uint RootBone);
	void						SetRootBoneAddIndex(_uint RootBoneAdd);
private:
	_uint						m_iBoneIndex = g_iMaxBones;
	_uint						m_iRootBoneIndex = { 0 };
	_int						m_iRootBoneAddIndex = { -1 };
	_uint						m_iReferenceBoneIndex = { 0 };
	_uint						m_iNumKeyFrames = 0;
	vector<KEYFRAME>			m_vecKeyFrames;

	_float4x4					m_mBase = g_mUnit;
	_float4x4					m_mRootMatrix;

	_float4						m_vRootPos;
	_float4						m_vRootPosAdd;

public:
#if ACTIVATE_TOOL
	static shared_ptr<CChannel>	Create(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	shared_ptr<CChannel>		Clone();

	static shared_ptr<CChannel>	Read(std::ifstream&);

#if ACTIVATE_TOOL
	void						Export(std::ofstream&);
	void						Export_Keyframe(std::ofstream&);
#endif

	_float4						Get_RootPos() { return m_vRootPos; }
	_float4						Get_RootPosAdd() { 
		return m_vRootPosAdd; 
	}
	_float4x4					Get_RootMatrix() { return m_mRootMatrix; }
};

END