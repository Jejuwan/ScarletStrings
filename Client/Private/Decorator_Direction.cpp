#include "ClientPCH.h"
#include "Decorator_Direction.h"
#include "GameInstance.h"

CDecorator_Direction::CDecorator_Direction(shared_ptr<CNode> pNode, DIRECTION Direction)
	: CDecorator(pNode)
	, m_eDirection(Direction)
{
}

HRESULT CDecorator_Direction::Initialize(shared_ptr<class CBlackBoard> pBlackBoard)
{
	if (FAILED(__super::Initialize(pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CDecorator_Direction::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CDecorator_Distance::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CDecorator_Distance::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CDecorator_Direction::Activate()
{
	__super::Activate();
}

STATUS CDecorator_Direction::Invoke(_float fTimeDelta)
{
	Begin_Invoke(fTimeDelta);
	
	_vector myLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_vector Direction = m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION);

	_float Radian = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(myLook), XMVector3Normalize(Direction))));
	_vector Cross = XMVector3Cross(myLook, Direction);

	if (XMVectorGetY(Cross) <= 0.f)
		Radian *= -1;

	_float Degree = XMConvertToDegrees(Radian);
	switch (m_eDirection)
	{
	case DIRECTION::FRONT:
		if (45.f >= Degree && -45.f <= Degree)
			m_eStatus = m_pNode->Invoke(fTimeDelta);
		else
			m_eStatus = STATUS::FAILURE;
		break;
	case DIRECTION::RIGHT:
		if (135.f >= Degree && 45 <= Degree)
			m_eStatus = m_pNode->Invoke(fTimeDelta);
		else
			m_eStatus = STATUS::FAILURE;
		break;
	case DIRECTION::BACK:
		if (-135.f >= Degree || 135.f <= Degree)
			m_eStatus = m_pNode->Invoke(fTimeDelta);
		else
			m_eStatus = STATUS::FAILURE;
		break;
	case DIRECTION::LEFT:
		if (-45.f >= Degree && -135.f <= Degree)
			m_eStatus = m_pNode->Invoke(fTimeDelta);
		else
			m_eStatus = STATUS::FAILURE;
		break;
	case DIRECTION::DIR_END:
		break;
	default:
		break;
	}

	return Return_Invoke();
}

void CDecorator_Direction::Terminate()
{
	m_pNode->Terminate();
	__super::Terminate();
}

shared_ptr<CDecorator_Direction> CDecorator_Direction::Create(shared_ptr<CBlackBoard> pBlackBoard, DIRECTION Direction, shared_ptr<CNode> _pNode)
{
	shared_ptr<CDecorator_Direction> pInstance = make_private_shared(CDecorator_Direction, _pNode , Direction);

	if (FAILED(pInstance->Initialize(pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CDecorator_Direction::Create", "Failed to Initialize");
	}

	return pInstance;
}
