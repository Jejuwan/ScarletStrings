#include "EnginePCH.h"
#include "Transform.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Event_Handler.h"

CTransform::CTransform()
	: CComponent(nullptr, nullptr, COMPONENT::MAX)
	, m_mWorld	(g_mUnit)
	, m_mAxis	(XMFLOAT3X3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f))
{
}

CTransform::CTransform(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::TRANSFORM)
	, m_mWorld	(g_mUnit)
	, m_mAxis	(XMFLOAT3X3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f))
{
}

CTransform::CTransform(const CTransform& _rhs)
	: CComponent(_rhs)
	, m_mWorld	(_rhs.m_mWorld)
	, m_mAxis	(_rhs.m_mAxis)
{
}

_float4x4 CTransform::Get_Matrix() const
{
	return m_mWorld;
}

_float4x4 CTransform::Get_WorldMatrix()
{
	return m_mWorld;
}

_float4x4* CTransform::Get_WorldMatrixPtr()
{
	return &m_mWorld;
}

_float3 CTransform::Get_State(const TRANSFORM _eState) const
{
	return *reinterpret_cast<const _float3*>(&m_mWorld.m[IDX(_eState)][0]);
}

_float3 CTransform::Get_Scale() const
{
	return _float3(
		XMVectorGetX(XMVector3Length(Get_State(TRANSFORM::RIGHT))),
		XMVectorGetY(XMVector3Length(Get_State(TRANSFORM::UP))),
		XMVectorGetZ(XMVector3Length(Get_State(TRANSFORM::LOOK)))
	);
}

void CTransform::Set_Matrix(const _matrixf _mMatrix)
{
	m_mWorld	= _mMatrix;
	Set_Axis();
}

void CTransform::Set_State(const TRANSFORM _eState, const _vectorf _vState)
{
	_matrix mWorld(m_mWorld);

	if (Function::InRange(_eState, TRANSFORM::RIGHT, TRANSFORM::LOOK, "[]"))
	{
		if (XMVector3Equal(XMVector3Normalize(mWorld.r[IDX(_eState)]), XMVector3Normalize(_vState)))
		{
			return;
		}
		else if (XMVector3Equal(XMVector3Cross(XMVector3Normalize(mWorld.r[IDX(_eState)]), XMVector3Normalize(_vState)), XMVectorZero()))
		{
			switch (_eState)
			{
			case TRANSFORM::RIGHT:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::UP)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::UP)]));
				break;
			case TRANSFORM::UP:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::LOOK)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::LOOK)]));
				break;
			case TRANSFORM::LOOK:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::RIGHT)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::RIGHT)]));
				break;
			}
		}
		m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], _vState));
		Set_State(TRANSFORM::POSITION, mWorld.r[IDX(TRANSFORM::POSITION)]);
	}
	else if(TRANSFORM::POSITION == _eState)
	{
		mWorld.r[IDX(_eState)] = _vState;
		m_mWorld = mWorld;
		m_mWorld.m[3][3] = 1.f;
	}
	else
	{
		return;
	}
	Set_Axis();
}

void CTransform::Set_Scale(const _vectorf _vScale)
{
	//_vectorf vPosition = XMLoadFloat4x4(&m_mWorld).r[IDX(TRANSFORM::POSITION)];

	//if (XMVectorGetX(XMVector3Length(_vScale)))
	//{
	//	_vectorf vScaleFactor	= _vScale / Get_Scale();
	//	m_mWorld *= XMMatrixScalingFromVector(vScaleFactor);
	//}
	//else
	//{
	//	m_mWorld *= XMMatrixScalingFromVector(XMVectorReplicate(g_fTolorance));
	//}

	//Set_State(TRANSFORM::POSITION, vPosition);
	m_mWorld._11 = m_mAxis._11 * XMVectorGetX(_vScale);
	m_mWorld._12 = m_mAxis._12 * XMVectorGetX(_vScale);
	m_mWorld._13 = m_mAxis._13 * XMVectorGetX(_vScale);
	m_mWorld._21 = m_mAxis._21 * XMVectorGetY(_vScale);
	m_mWorld._22 = m_mAxis._22 * XMVectorGetY(_vScale);
	m_mWorld._23 = m_mAxis._23 * XMVectorGetY(_vScale);
	m_mWorld._31 = m_mAxis._31 * XMVectorGetZ(_vScale);
	m_mWorld._32 = m_mAxis._32 * XMVectorGetZ(_vScale);
	m_mWorld._33 = m_mAxis._33 * XMVectorGetZ(_vScale);
}

void CTransform::Multiply(const _matrixf _mMatrix)
{
	m_mWorld *= _mMatrix;
	Set_Axis();
}

void CTransform::Translate(const _vectorf _vTranslation)
{
	m_mWorld *= XMMatrixTranslationFromVector(_vTranslation);
}

void CTransform::Turn(_vectorf vAxis, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis,fTimeDelta);

	_vector		vRight = Get_State(TRANSFORM::RIGHT);
	_vector		vUp = Get_State(TRANSFORM::UP);
	_vector		vLook = Get_State(TRANSFORM::LOOK);

	Set_State(TRANSFORM::RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(TRANSFORM::UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(TRANSFORM::LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Rotate(const _vectorf _vQuaternion)
{
	_matrix mWorld(m_mWorld);
	m_mWorld *= XMMatrixRotationQuaternion(XMQuaternionNormalize(_vQuaternion));
	Set_State(TRANSFORM::POSITION, mWorld.r[IDX(TRANSFORM::POSITION)]);
}

void CTransform::Rotate(const _vectorf _vAxis, const _float _fRadian)
{
	_matrix mWorld(m_mWorld);
	m_mWorld *= XMMatrixRotationQuaternion(XMQuaternionRotationAxis(_vAxis, _fRadian));
	Set_State(TRANSFORM::POSITION, mWorld.r[IDX(TRANSFORM::POSITION)]);
}

void CTransform::Rotate_Lerp(const _vectorf _vAxis, const _float _fRadian, const _float _fInterpolationDuration)
{
	_float	fAcc(0.f);
	CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				_float fDelta = _fTimeDelta / _fInterpolationDuration;
				fAcc += fDelta;
				if (fAcc > 1.f)
				{
					fDelta -= fAcc - 1.f;
				}

				Rotate(_vAxis, _fRadian * fDelta);
			}

			return fAcc < 1.f;
		}
	);
}

void CTransform::Rotate(const TRANSFORM _eState, const _float _fRadian)
{
	Rotate(Get_State(_eState), _fRadian);
}

void CTransform::Rotate_Lerp(const TRANSFORM _eState, const _float _fRadian, const _float _fInterpolationDuration)
{
	Rotate_Lerp(Get_State(_eState), _fRadian, _fInterpolationDuration);
}

void CTransform::LookAt(const _vectorf _vPosition, const _bool _bFixUp, const _bool _bPipeLineUp)
{
	Set_State(TRANSFORM::LOOK, _vPosition - Get_State(TRANSFORM::POSITION));
	if (_bFixUp)
	{
		Set_State(TRANSFORM::UP, _bPipeLineUp ? CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::UP) :_float4(0.f, 1.f, 0.f, 0.f));
	}
}

void CTransform::LookAt(shared_ptr<CTransform> _pTransform, const _bool _bFixUp, const _bool _bPipeLineUp)
{
	LookAt(_pTransform->Get_State(TRANSFORM::POSITION), _bFixUp, _bPipeLineUp);
}

void CTransform::LookAt_Interpolation(const _vectorf _vPosition, const _bool _bFixUp, const _bool _bPipeLineUp, const _float _fInterpolationDuration, const _float _fWeight)
{
	_float	fAcc(0.f);
	_vector	vLook = _vPosition - Get_State(TRANSFORM::POSITION);
	CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / _fInterpolationDuration;
				LookTo(Function::Lerp(Get_State(TRANSFORM::LOOK), vLook, Function::Clamp(0.f, 1.f, fAcc), _fWeight), _bFixUp, _bPipeLineUp);
			}

			return fAcc < 1.f;
		}
	);
}

void CTransform::LookAt_Interpolation(shared_ptr<CTransform> _pTransform, const _bool _bFixUp, const _bool _bPipeLineUp, const _float _fInterpolationDuration, const _float _fWeight)
{
	LookAt_Interpolation(_pTransform->Get_State(TRANSFORM::POSITION), _bFixUp, _bPipeLineUp, _fInterpolationDuration, _fWeight);
}

void CTransform::LookTo(const _vectorf _vDirection, const _bool _bFixUp, const _bool _bPipeLineUp)
{
	Set_State(TRANSFORM::LOOK, _vDirection);
	if (_bFixUp)
	{
		Set_State(TRANSFORM::UP, _bPipeLineUp ? CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::UP) : _float4(0.f, 1.f, 0.f, 0.f));
	}
}

void CTransform::LookAtDir(_vectorf lookDir,_bool bFix)
{
	_vector		vLook = lookDir;

	if (bFix)
		vLook = XMVectorSetY(vLook, 0.f);

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);


	_vector		vUp = XMVector3Cross(vLook, vRight);

	//if (bFix)
	//	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_float3		vScaled = Get_Scale();

	Set_State(TRANSFORM::RIGHT, vRight);
	Set_State(TRANSFORM::UP, vUp);
	Set_State(TRANSFORM::LOOK, vLook);

	Set_Scale(vScaled);
}

void CTransform::LookTo_Interpolation(const _vectorf _vDirection, const _bool _bFixUp, const _bool _bPipeLineUp, const _float _fInterpolationDuration, const _float _fWeight)
{
	_float	fAcc(0.f);
	CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / _fInterpolationDuration;
				LookTo(Function::Lerp(Get_State(TRANSFORM::LOOK), _vDirection, Function::Clamp(0.f, 1.f, fAcc), _fWeight), _bFixUp, _bPipeLineUp);
			}

			return fAcc < 1.f;
		}
	);
}

_float3 CTransform::Get_HorizontalDir(const TRANSFORM eState)
{
	_float3 vDir = Get_State(eState);
	vDir.y = 0;
	return XMVector3Normalize(vDir);
}

void CTransform::Rotation_By_RollPitchYao(const _matrixf RPY)
{
	XMStoreFloat4x4(&m_mWorld, RPY);
}

HRESULT CTransform::Bind_OnShader(shared_ptr<CShader> _pShader)
{
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATWORLD, m_mWorld)))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Vector");
	}

	return S_OK;
}

void CTransform::Set_Axis()
{
	_float3 vRight	= XMVector3Normalize(Get_State(TRANSFORM::RIGHT));
	_float3 vUp		= XMVector3Normalize(Get_State(TRANSFORM::UP));
	_float3 vLook	= XMVector3Normalize(Get_State(TRANSFORM::LOOK));
	m_mAxis			= XMFLOAT3X3(vRight.x, vRight.y, vRight.z, vUp.x, vUp.y, vUp.z, vLook.x, vLook.y, vLook.z);
}

shared_ptr<CTransform> CTransform::Create()
{
	return make_private_shared(CTransform);
}

shared_ptr<CTransform> CTransform::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	return make_private_shared(CTransform, _pDevice, _pContext);
}

shared_ptr<CComponent> CTransform::Clone(any)
{
	return make_private_shared_copy(CTransform, *this);
}
