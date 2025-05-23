#pragma once
#include "Loader.h"
#include "EnginePCH.h"

UINT APIENTRY Thread_Loader_Main(void* _pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(_pArg);

	if (FAILED(CoInitializeEx(nullptr, 0)))
	{
		MSG_BOX("Thread_Loader_Main", "Failed: CoInitializeEx");
		assert(false);
	}

	if (FAILED(pLoader->Loading()))
	{
		MSG_BOX("Thread_Loader_Main", "Failed: Loading");
		assert(false);
	}

	return 0;
}
