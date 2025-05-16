#include "EnginePCH.h"
#include "SSAO.h"

HRESULT CSSAO::Render()
{
	return S_OK;
}

shared_ptr<CSSAO> CSSAO::Create()
{
	shared_ptr<CSSAO> pInstance = make_private_shared(CSSAO);
	return pInstance;
}
