#include "ClientPCH.h"
#include "GameInstance.h"
#include "$safeitemname$.h"

C$safeitemname$::C$safeitemname$()
#ifdef _DEBUG
	: CDebugParallel(POLICY::MAX, POLICY::MAX)
#else
	: CParallel(POLICY::MAX, POLICY::MAX)
#endif
{
}

HRESULT C$safeitemname$::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
#ifdef _DEBUG
	m_strNodeName = "Parallel";
#endif

	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "C$safeitemname$::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void C$safeitemname$::Activate()
{
	__super::Activate();
}

STATUS C$safeitemname$::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void C$safeitemname$::Terminate()
{
	__super::Terminate();
}

#if ACTIVATE_IMGUI
void C$safeitemname$::DebugLog()
{
}
#endif

shared_ptr<C$safeitemname$> C$safeitemname$::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<C$safeitemname$> pInstance = make_private_shared(C$safeitemname$);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "C$safeitemname$::Create", "Failed to Initialize");
	}

	return pInstance;
}
