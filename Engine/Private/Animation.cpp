#include "EnginePCH.h"
#include "Animation.h"
#include "Channel.h"
#include "Event.h"

CAnimation::CAnimation(const CAnimation& _rhs)
	: m_bReverse(_rhs.m_bReverse)
	, m_fDuration(_rhs.m_fDuration)
	, m_fTrackPosition(_rhs.m_fTrackPosition)
	, m_fTicksPerSecond(_rhs.m_fTicksPerSecond)
	, m_iNumChannels(_rhs.m_iNumChannels)
	, m_vecChannels(_rhs.m_vecChannels)
	, m_vecChannelKeyFrames(_rhs.m_vecChannelKeyFrames)
	, m_mapEvents(_rhs.m_mapEvents)
{
	strcpy_s(m_szName, MAX_PATH, _rhs.m_szName);

	

	/*for (auto pOriginal : _rhs.m_vecChannels)
	{
		m_vecChannels.push_back(pOriginal->Clone());
	}*/
}

#if ACTIVATE_TOOL
HRESULT CAnimation::Initialize_FromAssimp(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	HRESULT hr = S_OK;

	strcpy_s(m_szName, _pAIAnimation->mName.C_Str());

	m_fDuration = static_cast<_float>(_pAIAnimation->mDuration);
	m_fTicksPerSecond = static_cast<_float>(_pAIAnimation->mTicksPerSecond);

	m_iNumChannels = _pAIAnimation->mNumChannels;
	m_vecChannels.reserve(m_iNumChannels);
	m_vecChannelKeyFrames.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		shared_ptr<CChannel> pChannel = CChannel::Create(_pAIAnimation->mChannels[i], _pModel);
		if (nullptr == pChannel)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CAnimation::Initialize_FromAssimp", "Failed to CChannel::Create");
		}

		m_vecChannels.emplace_back(pChannel);
	}

	return hr;
}
#endif

HRESULT CAnimation::Initialize_FromBinary(std::ifstream& _inFile)
{
	_inFile.read(reinterpret_cast<_byte*>(m_szName), MAX_PATH);
	_inFile.read(reinterpret_cast<_byte*>(&m_fDuration), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_fTicksPerSecond), sizeof(_float));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumChannels), sizeof(_uint));
	_inFile.read(reinterpret_cast<_byte*>(&m_iNumEvents), sizeof(_uint));

	m_vecChannels.reserve(m_iNumChannels);
	m_vecChannelKeyFrames.resize(m_iNumChannels);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels.emplace_back(CChannel::Read(_inFile));

		if (_inFile.eof())
		{
			int a = 1;
			MSG_BOX("asdf", "Asdf");
		}
	}

	for (_uint i = 0; i < m_iNumEvents; ++i) 
	{
		_uint iKeyFrame = 0;
		_inFile.read(reinterpret_cast<_byte*>(&iKeyFrame), sizeof(_uint));
		auto iter = m_mapEvents.find(iKeyFrame);
		if (iter == m_mapEvents.end())
		{
			vector<shared_ptr<CEvent>> vecEvent;
			vecEvent.push_back(CEvent::Read(_inFile, shared_from_this()));
			m_mapEvents.emplace(iKeyFrame, vecEvent);
		}
		else
			iter->second.push_back(CEvent::Read(_inFile, shared_from_this()));
	}

	return S_OK;
}

HRESULT CAnimation::Initialize()
{
	map<_uint, vector<shared_ptr<class CEvent>>> newEvents;
	for (auto& MapList : m_mapEvents)
	{
		for (auto& Event : MapList.second)
		{
			auto iter = newEvents.find(MapList.first);
			if (iter == newEvents.end())
			{
				vector<shared_ptr<CEvent>> vecEvent;
				vecEvent.push_back(Event->Clone(weak_from_this()));
				newEvents.emplace(MapList.first, vecEvent);
			}
			else
				iter->second.push_back(Event->Clone(weak_from_this()));

		}
	}
	m_mapEvents.clear();
	m_mapEvents = newEvents;
	m_EventIter = m_mapEvents.begin();
	return S_OK;
}

void CAnimation::Tick(_float _fTimeDelta, vector<shared_ptr<class CBone>>::iterator _itBegin, _bool _bReverse, _bool _bLoop)
{
	m_bReverse = _bReverse;
	
	if (false == _bReverse)
	{
		if (!m_mapEvents.empty() && m_EventIter != m_mapEvents.end())
		{
			if (m_EventIter->first <= m_fTrackPosition)
			{
				for (size_t i = 0; i < m_EventIter->second.size(); i++)
				{
					m_EventIter->second[i]->Act();
				}
				++m_EventIter;
				/*if (m_EventIter == m_mapEvents.end())
					m_EventIter = m_mapEvents.begin();*/
			}
		}
		m_fTrackPosition += m_fTicksPerSecond * _fTimeDelta * m_fAnimSpeed;
		m_fTrackPosition = min(m_fTrackPosition, m_fDuration);

		if (m_bTrash)
		{
			if (m_fTrackPosition > 37.f && m_fTrackPosition < 39.f)
				m_fTrackPosition = 39.1f;
		}
		m_fProportion = m_fTrackPosition / m_fDuration;
		if (_bLoop && m_fTrackPosition >= m_fDuration)
		{
			m_fTrackPosition = 0.f;
			m_EventIter = m_mapEvents.begin();
			m_fAnimSpeed = 1.f;
		}
	}
	else
	{

		if (!m_mapEvents.empty() && m_EventIter != m_mapEvents.end())
		{
			if (m_EventIter->first <= m_fTrackPosition)
			{
				for (size_t i = 0; i < m_EventIter->second.size(); i++)
				{
					m_EventIter->second[i]->Act();
				}
				--m_EventIter;
				/*if (m_EventIter == m_mapEvents.end())
					m_EventIter = m_mapEvents.begin();*/
			}
		}
		m_fTrackPosition -= m_fTicksPerSecond * _fTimeDelta;
		m_fTrackPosition = max(m_fTrackPosition, 0.f);

		if (_bLoop && m_fTrackPosition <= 0.f)
		{
			m_fTrackPosition = m_fDuration;
			m_EventIter = m_mapEvents.end();
			m_fAnimSpeed = 1.f;
		}
	}


	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels[i]->Update_Transformation(_itBegin, m_vecChannelKeyFrames[i], m_fTrackPosition);
		if (m_vecChannels[i]->Get_BoneIndex() == m_iRootBoneIndex)
		{
			_float4 AddBone = m_vecChannels[i]->Get_RootPosAdd();
			m_vRootPos = m_vecChannels[i]->Get_RootPos() + m_vecChannels[i]->Get_RootPosAdd();
			m_mRootMatrix = m_vecChannels[i]->Get_RootMatrix();
		}
		if(m_vecChannels[i]->Get_BoneIndex() == m_iRootBoneAddIndex)
		{
			_float4 AddBone = m_vecChannels[i]->Get_RootPosAdd();
			m_vRootPosAdd = m_vecChannels[i]->Get_RootPosAdd();
		}

	}
}

_bool CAnimation::Is_ChannelExist(_uint _iBoneIndex) const
{
	for (auto& pChannel : m_vecChannels)
	{
		if (pChannel->Get_BoneIndex() == _iBoneIndex)
		{
			return true;
		}
	}

	return false;
}

void CAnimation::AddEvent(_uint TrackPosition, shared_ptr<CEvent> _pEvent)
{
	++m_iNumEvents;
	auto iter = m_mapEvents.find(TrackPosition);
	if (iter == m_mapEvents.end())
	{
		vector<shared_ptr<CEvent>> vecEvent;
		vecEvent.push_back(_pEvent);
		m_mapEvents.emplace(TrackPosition, vecEvent);
	}
	else
		iter->second.push_back(_pEvent);
	m_EventIter = m_mapEvents.begin();
}

void CAnimation::Bind_Event(EVENTTYPE EventType, function<void(void)> Function)
{
	for (auto& MapIter : m_mapEvents)
	{
		for (auto& pEvent : MapIter.second)
			pEvent->Bind_Event(EventType, Function);
	}
	m_EventIter = m_mapEvents.begin();
}

void CAnimation::Fix(vector<shared_ptr<CBone>>::iterator _itBegin, _float _fRatio)
{
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		for (_uint j = 0; j < m_vecChannels[i]->Get_NumKeyFrames() - 1; ++j)
		{
			if (Function::InRange(m_fDuration * _fRatio, m_vecChannels[i]->Get_KeyFrame(j).fTime, m_vecChannels[i]->Get_KeyFrame(j + 1).fTime))
			{
				m_vecChannelKeyFrames[i] = j;
				break;
			}
		}

		m_vecChannels[i]->Update_Transformation(_itBegin, m_vecChannelKeyFrames[i], m_fDuration * _fRatio);
	}
}

void CAnimation::Blend(vector<shared_ptr<CAnimation>>::iterator _itAnimationBegin, vector<shared_ptr<CBone>>::iterator _itBoneBegin, vector<_uint>::iterator _itBoneAnimationBegin, _float _fRatio)
{
	_uint iKeyFrameIndex(0);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		for (_uint j = 0; j < m_vecChannels[i]->Get_NumKeyFrames() - 1; ++j)
		{
			if (Function::InRange(m_fDuration * _fRatio, m_vecChannels[i]->Get_KeyFrame(j).fTime, m_vecChannels[i]->Get_KeyFrame(j + 1).fTime))
			{
				iKeyFrameIndex = j;
				break;
			}
		}

		m_vecChannels[i]->Blend_Transformation(_itBoneBegin, iKeyFrameIndex, m_fDuration * _fRatio, _itAnimationBegin[_itBoneAnimationBegin[m_vecChannels[i]->Get_BoneIndex()]]);
	}
}

void CAnimation::Interpolate_TransformationMatrices(_float fTimeDelta, shared_ptr<CAnimation> _pAnim, vector<shared_ptr<class CBone>>& itBoneBegin, _float InterpolateTime)
{

	m_fInterpolatePosition += fTimeDelta;
	if (m_fInterpolatePosition >= InterpolateTime)
	{
		m_bInterpolateEnd = true;
	}
	_uint		iIndex = 0;

	for (auto& pChannel : m_vecChannels)
	{
		pChannel->Interpolate_Transformation(_pAnim->Get_Channel(iIndex), itBoneBegin, m_vecChannelKeyFrames[iIndex], m_fInterpolatePosition, InterpolateTime);
		iIndex++;
	}
}

void CAnimation::Reset()
{
	m_bReverse = false;
	m_fTrackPosition = 0.f;
	m_EventIter = m_mapEvents.begin();
	m_fAnimSpeed = 1.f;
	m_bInterpolateEnd = false;
	m_fInterpolatePosition = 0.1f;
}

shared_ptr<CChannel> CAnimation::Get_Channel(_uint _iChannelIndex, _bool _bBoneIndex) const
{
	if (false == _bBoneIndex)
	{
		return m_vecChannels[_iChannelIndex];
	}
	else
	{
		for (auto& pChannel : m_vecChannels)
		{
			if (pChannel->Get_BoneIndex() == _iChannelIndex)
			{
				return pChannel;
			}
		}
	}

	return nullptr;
}

#if ACTIVATE_TOOL
_uint CAnimation::Get_ChannelIndex(_uint _iBoneIndex) const
{
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (m_vecChannels[i]->Get_BoneIndex() == _iBoneIndex)
		{
			return i;
		}
	}

	return m_iNumChannels;
}
#endif

void CAnimation::Set_AnimationIndex(vector<_uint>::iterator _itBegin, _uint _iAnimationIndex)
{
	for (auto& pChannel : m_vecChannels)
	{
		_itBegin[pChannel->Get_BoneIndex()] = _iAnimationIndex;
	}
}

#if ACTIVATE_TOOL
HRESULT CAnimation::Remove_Channel(_uint _iChannelIndex)
{
	if (_iChannelIndex >= m_iNumChannels)
	{
		MSG_RETURN(E_FAIL, "CAnimation::Remove_Channel", "Invalid Channel Index");
	}

	--m_iNumChannels;
	m_vecChannels.erase(m_vecChannels.begin() + _iChannelIndex);
	m_vecChannelKeyFrames.erase(m_vecChannelKeyFrames.begin() + _iChannelIndex);

	return S_OK;
}
#endif

void CAnimation::SetRootBoneIndex(_uint Reference, _uint RootBone)
{
	m_iRootBoneIndex = RootBone;
	for (auto pChannel : m_vecChannels)
		pChannel->SetRootBoneIndex(Reference,RootBone);
}

void CAnimation::SetRootBoneAddIndex(_uint RootBoneAdd)
{
	m_iRootBoneAddIndex = RootBoneAdd;
	for (auto pChannel : m_vecChannels)
		pChannel->SetRootBoneAddIndex(RootBoneAdd);
}

#if ACTIVATE_TOOL
shared_ptr<CAnimation> CAnimation::Create(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize_FromAssimp(_pAIAnimation, _pModel)))
	{
		MSG_RETURN(nullptr, "CAnimation::Create", "Failed to Initialize_FromAssimp");
	}

	return pInstance;
}
#endif

shared_ptr<CAnimation> CAnimation::Read(std::ifstream& _inFile)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize_FromBinary(_inFile)))
	{
		MSG_RETURN(nullptr, "CAnimation::Read", "Failed to Initialize_FromBinary");
	}

	return pInstance;
}

shared_ptr<CAnimation> CAnimation::Clone()
{
	shared_ptr<CAnimation>  pInstance =make_private_shared_copy(CAnimation, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CAnimation::Read", "Failed to Initialize_FromBinary");
	}
			
	return pInstance;
}

#if ACTIVATE_TOOL
void CAnimation::Export(std::ofstream& _outFile)
{
	//m_iNumEvents = 0;
	
	_outFile.write(reinterpret_cast<const _byte*>(m_szName), MAX_PATH);
	_outFile.write(reinterpret_cast<const _byte*>(&m_fDuration), sizeof(_float));
	_outFile.write(reinterpret_cast<const _byte*>(&m_fTicksPerSecond), sizeof(_float));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumChannels), sizeof(_uint));
	_outFile.write(reinterpret_cast<const _byte*>(&m_iNumEvents), sizeof(_uint));

	for (auto& pChannel : m_vecChannels)
	{
		pChannel->Export(_outFile);
	}
	
	for (auto Iter = m_mapEvents.begin(); Iter != m_mapEvents.end(); ++Iter)
	{
		_uint vecSize = Iter->second.size();
		for (size_t i = 0; i < vecSize; i++)
		{
			_outFile.write(reinterpret_cast<const _byte*>(&Iter->first), sizeof(_uint));
			Iter->second[i]->Export(_outFile);
		}
	}
}
void CAnimation::Export(std::ofstream& _outFile, _uint iChannelIndex)
{
	_outFile.write(reinterpret_cast<const _byte*>(&m_fDuration), sizeof(_float));
	_outFile.write(reinterpret_cast<const _byte*>(&m_fTicksPerSecond), sizeof(_float));
	m_vecChannels[iChannelIndex]->Export_Keyframe(_outFile);
}
#endif

void CAnimation::Set_TrackPosition(_float f)
{
	m_fTrackPosition = f;
	for (auto& KeyFrame : m_vecChannelKeyFrames)
		KeyFrame = 0;
}