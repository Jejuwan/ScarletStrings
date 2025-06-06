#include "EnginePCH.h"
#include "Channel.h"
#include "Model.h"
#include "Animation.h"
#include "Bone.h"

#if ACTIVATE_TOOL
HRESULT CChannel::Initialize_FromAssimp(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	m_iBoneIndex = _pModel->Get_BoneIndex(_pAIChannel->mNodeName.C_Str());
	m_iNumKeyFrames = Function::Max(_pAIChannel->mNumScalingKeys, _pAIChannel->mNumRotationKeys, _pAIChannel->mNumPositionKeys);

	m_vecKeyFrames.reserve(m_iNumKeyFrames);

	_float3	vScale;
	_float4	vRotation;
	_float3	vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME tKeyFrame{};

		if (i < _pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &_pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mScalingKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumRotationKeys)
		{
			vRotation.x = _pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = _pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = _pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = _pAIChannel->mRotationKeys[i].mValue.w;
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mRotationKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumPositionKeys)
		{
			memcpy(&vTranslation, &_pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mPositionKeys[i].mTime);
		}

		memcpy(&tKeyFrame.vScale, &vScale, sizeof(_float3));
		memcpy(&tKeyFrame.vRotation, &vRotation, sizeof(_float4));
		memcpy(&tKeyFrame.vTranslation, &vTranslation, sizeof(_float3));
		tKeyFrame.vTranslation.w = 1.f;

		m_vecKeyFrames.push_back(tKeyFrame);
	}

	m_mBase = XMMatrixAffineTransformation(
		XMLoadFloat4(&m_vecKeyFrames[0].vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&m_vecKeyFrames[0].vRotation),
		XMLoadFloat4(&m_vecKeyFrames[0].vTranslation)
	);

	return S_OK;
}
#endif

HRESULT CChannel::Initialize_FromBinary(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(&m_iBoneIndex), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumKeyFrames), sizeof(_uint));

	m_vecKeyFrames.resize(m_iNumKeyFrames);

	_inFile.read(reinterpret_cast<_byte*>(m_vecKeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);

	m_mBase = XMMatrixAffineTransformation(
		XMLoadFloat4(&m_vecKeyFrames[0].vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&m_vecKeyFrames[0].vRotation),
		XMLoadFloat4(&m_vecKeyFrames[0].vTranslation)
	);

	return S_OK;
}

void CChannel::Update_Transformation(vector<shared_ptr<CBone>>::iterator _itBegin, _uint& _iCurrentKeyFrame, _float _fTrackPosition)
{
	if (0.f == _fTrackPosition)
	{
		_iCurrentKeyFrame = 0;
	}

	_float	fRatio;
	_vector	vScale, vRotation, vTranslation;

	if (_fTrackPosition < m_vecKeyFrames.back().fTime)
	{
		while (_fTrackPosition > m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime)
		{
			++_iCurrentKeyFrame;
		}

		fRatio = Function::ProportionalRatio(m_vecKeyFrames[_iCurrentKeyFrame].fTime, m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime, _fTrackPosition);

		vScale = XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vScale), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vRotation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vRotation), fRatio);
		vTranslation = XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vTranslation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vTranslation), fRatio);
	}
	else
	{
		vScale = _float4(m_vecKeyFrames.back().vScale);
		vRotation = _float4(m_vecKeyFrames.back().vRotation);
		vTranslation = _float4(m_vecKeyFrames.back().vTranslation);
	}

	//_itBegin[m_iBoneIndex]->Set_Transformation(vScale, vRotation, vTranslation);

	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation	);

	//4:"References"��
	if (m_iBoneIndex == m_iRootBoneIndex)
	{
		m_mRootMatrix = TransformationMatrix;
		XMStoreFloat4(&m_vRootPos, TransformationMatrix.r[3]);
		//TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	if (m_iBoneIndex == m_iRootBoneAddIndex)
	{
		//XMStoreFloat4(&m_vRootPosAdd, TransformationMatrix.r[3]);
		TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	if (m_iBoneIndex == m_iReferenceBoneIndex)
	{
		//XMStoreFloat4(&m_vRootPos, TransformationMatrix.r[3]);
		TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	_itBegin[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Blend_Transformation(vector<shared_ptr<CBone>>::iterator _itBegin, _uint _iKeyFrame, _float _fTrackPosition, shared_ptr<CAnimation> _pBase)
{
	_float	fRatio;
	_vector	vScale, vRotation, vTranslation;

	if (_fTrackPosition < m_vecKeyFrames.back().fTime)
	{
		fRatio = Function::ProportionalRatio(m_vecKeyFrames[_iKeyFrame].fTime, m_vecKeyFrames[_iKeyFrame + 1].fTime, _fTrackPosition);

		vScale = XMVectorLerp(_float4(m_vecKeyFrames[_iKeyFrame].vScale), _float4(m_vecKeyFrames[_iKeyFrame + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(_float4(m_vecKeyFrames[_iKeyFrame].vRotation), _float4(m_vecKeyFrames[_iKeyFrame + 1].vRotation), fRatio);
		vTranslation = XMVectorLerp(_float4(m_vecKeyFrames[_iKeyFrame].vTranslation), _float4(m_vecKeyFrames[_iKeyFrame + 1].vTranslation), fRatio);
	}
	else
	{
		vScale = _float4(m_vecKeyFrames.back().vScale);
		vRotation = _float4(m_vecKeyFrames.back().vRotation);
		vTranslation = _float4(m_vecKeyFrames.back().vTranslation);
	}

	_itBegin[m_iBoneIndex]->Blend_Transformation(vScale, vRotation, vTranslation, _pBase->Get_Channel(m_iBoneIndex, true)->m_mBase);
}

void CChannel::Interpolate_Transformation(shared_ptr<CChannel> pChannel, vector<shared_ptr<CBone>>& itBegin, _uint iKeyFrame, _float fTrackPosition, _float InterpolateTime)
{
	
	_vector			vScale;
	_vector			vRotation;
	_vector			vPosition;

	if (fTrackPosition >= InterpolateTime)
	{
		vScale = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vScale);
		vRotation = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vRotation);
		vPosition = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vTranslation);
	}
	else 
	{
		_float		fRatio = fTrackPosition / InterpolateTime;

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourTranslation, vDestTranslation;

		vSourScale = XMLoadFloat4(&m_vecKeyFrames[iKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_vecKeyFrames[iKeyFrame].vRotation);
		vSourTranslation = XMLoadFloat4(&m_vecKeyFrames[iKeyFrame].vTranslation);

		vDestScale = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vScale);
		vDestRotation = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vRotation);
		vDestTranslation = XMLoadFloat4(&pChannel->Get_KeyFrameRef().front().vTranslation);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
	}

	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (m_iBoneIndex == m_iRootBoneIndex)
	{
		m_mRootMatrix = TransformationMatrix;
		XMStoreFloat4(&m_vRootPos, TransformationMatrix.r[3]);
		//TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	if (-1 != m_iRootBoneAddIndex &&m_iBoneIndex == m_iRootBoneAddIndex)
	{
		XMStoreFloat4(&m_vRootPosAdd, TransformationMatrix.r[3]);
		TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	if (m_iBoneIndex == m_iReferenceBoneIndex)
	{
		//XMStoreFloat4(&m_vRootPos, TransformationMatrix.r[3]);
		TransformationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}

	itBegin[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::SetRootBoneIndex(_uint Reference, _uint RootBone)
{
	m_iReferenceBoneIndex = Reference;
	m_iRootBoneIndex = RootBone;
}

void CChannel::SetRootBoneAddIndex(_uint RootBoneAdd)
{
	m_iRootBoneAddIndex = RootBoneAdd;
}

#if ACTIVATE_TOOL
shared_ptr<CChannel> CChannel::Create(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	shared_ptr<CChannel> pInstance = make_private_shared(CChannel);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAIChannel, _pModel)))
	{
		MSG_RETURN(nullptr, "CChannel::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CChannel> CChannel::Clone()
{
	return make_private_shared_copy(CChannel, *this);
}

shared_ptr<CChannel> CChannel::Read(std::ifstream& _inFile)
{
	shared_ptr<CChannel> pInstance = make_private_shared(CChannel);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile)))
	{
		MSG_RETURN(nullptr, "CChannel::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
void CChannel::Export(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_iBoneIndex), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumKeyFrames), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecKeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);
}

void CChannel::Export_Keyframe(std::ofstream& _outFile)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumKeyFrames), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(m_vecKeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);
}
#endif