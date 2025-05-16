#include "EnginePCH.h"
#include "Keyframe.h"

#include "Transform.h"

CKeyframe::CKeyframe(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::KEYFRAME)
{
}

CKeyframe::CKeyframe(const CKeyframe& _rhs)
	: CComponent(_rhs)
{
}

HRESULT CKeyframe::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKeyframe::Initialize()
{
	return S_OK;
}

void CKeyframe::Tick(shared_ptr<class CTransform> pTransform, _float fTimeDelta)
{
	if (m_bPlaying)
	{
		const std::vector<std::pair<_float4x4, _float>>& vecKeyframe = m_mapKeyframePair[m_wstrCurrentKeyframeTag];
		if (false == vecKeyframe.empty())
		{
			if (vecKeyframe.size() <= m_iCurrentPlayingKeyframe)
			{
				m_iCurrentPlayingKeyframe = 0;
				m_bCurrentKeyframeAccTime = 0.f;

				if (m_fpOnAnimFinish)
				{
					m_fpOnAnimFinish(m_wstrCurrentKeyframeTag);
				}

				m_bPlaying = false;
				return;
			}

			const std::pair<_float4x4, _float>& tKeyframe = vecKeyframe[m_iCurrentPlayingKeyframe];
			if (m_bCurrentKeyframeAccTime >= tKeyframe.second)
			{
				m_bCurrentKeyframeAccTime = 0.f;
				++m_iCurrentPlayingKeyframe;
			}

			Interpolate_Keyframe(m_iCurrentPlayingKeyframe, m_bCurrentKeyframeAccTime, vecKeyframe, pTransform);
			m_bCurrentKeyframeAccTime += fTimeDelta;
		}
	}
}

void CKeyframe::Append_Keyframe(const wstring& wstrFilePath, const wstring& wstrAnimTag)
{
	std::ifstream inFile(wstrFilePath, std::ios::binary);

	if (!inFile.is_open())
	{
		assert(false && "CKeyframe::Read_Keyframe : Failed to Open File");
	}

	_uint iNumKeyframe = 0;
	inFile.read(reinterpret_cast<char*>(&iNumKeyframe), sizeof(_uint));

	std::vector<std::pair<_float4x4, _float>> vecKeyframes;
	vecKeyframes.reserve(iNumKeyframe);

	for (_uint i = 0; i < iNumKeyframe; ++i)
	{
		std::pair<_float4x4, _float> tKeyframe;
		inFile.read(reinterpret_cast<char*>(&tKeyframe), sizeof(std::pair<_float4x4, _float>));

		vecKeyframes.push_back(tKeyframe);
	}

	m_mapKeyframePair.emplace(wstrAnimTag, vecKeyframes);

	inFile.close();
}

void CKeyframe::Clear_Keyframe()
{
	m_mapKeyframePair.clear();
}

_bool CKeyframe::Play_Animation(const wstring& wstrAnimTag, const _float4x4& matPivot)
{
	if (m_mapKeyframePair.find(wstrAnimTag) == m_mapKeyframePair.end())
	{
		return false;
	}

	m_bPlaying = true;

	m_iCurrentPlayingKeyframe = 0;
	m_bCurrentKeyframeAccTime = 0.f;

	m_wstrCurrentKeyframeTag = wstrAnimTag;

	m_matPivot = matPivot;

	return true;
}

void CKeyframe::Pause_Animation()
{
	m_bPlaying = false;
}

void CKeyframe::Continue_Animation()
{
	m_bPlaying = true;
}

void CKeyframe::Subscribe_Animation_Finish_Callback(const std::function<void(const wstring&)>& fpCallback)
{
	m_fpOnAnimFinish = fpCallback;
}

void CKeyframe::Interpolate_Keyframe(_uint iKeyframeIndex, _float fKeyframeAccTime, 
	const std::vector<std::pair<_float4x4, _float>>& vecKeyframe,
	shared_ptr<class CTransform> pTransform)
{
	if (vecKeyframe.size() <= iKeyframeIndex + 1)
		return;

	const std::pair<_float4x4, _float>& tCurrentKeyframe = vecKeyframe[iKeyframeIndex];
	const std::pair<_float4x4, _float>& tNextKeyframe = vecKeyframe[iKeyframeIndex + 1];

	_vector vCurrentTranslation, vCurrentScale, vCurrentRotationQuat;
	XMMatrixDecompose(&vCurrentScale, &vCurrentRotationQuat, &vCurrentTranslation, XMLoadFloat4x4(&tCurrentKeyframe.first));

	_vector vNextTranslation, vNextScale, vNextRotationQuat;
	XMMatrixDecompose(&vNextScale, &vNextRotationQuat, &vNextTranslation, XMLoadFloat4x4(&tNextKeyframe.first));

	_float fInterpolatedTime = fKeyframeAccTime / tCurrentKeyframe.second;

	_vector vInterpolatedRotationQuat = XMQuaternionSlerp(vCurrentRotationQuat, vNextRotationQuat, fInterpolatedTime);
	_vector vInterpolatedTranslation = XMVectorLerp(vCurrentTranslation, vNextTranslation, fInterpolatedTime);
	_vector vInterpolatedScale = XMVectorLerp(vCurrentScale, vNextScale, fInterpolatedTime);

	_matrix matInterpolatedMatrix = XMMatrixScalingFromVector(vInterpolatedScale);
	matInterpolatedMatrix *= XMMatrixRotationQuaternion(vInterpolatedRotationQuat);
	matInterpolatedMatrix *= XMMatrixTranslationFromVector(vInterpolatedTranslation);

	pTransform->Set_Matrix(matInterpolatedMatrix * m_matPivot);
}

shared_ptr<CKeyframe> CKeyframe::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CKeyframe> pInstance = make_private_shared(CKeyframe, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CKeyframe::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CKeyframe::Clone(any)
{
	shared_ptr<CKeyframe> pInstance = make_private_shared_copy(CKeyframe, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CKeyframe::Clone", "Failed to Initialize");
	}

	return pInstance;
}
