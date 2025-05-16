#pragma once

#include "Engine_Define.h"
#include "System.h"

BEGIN(Engine)

class CSSAO final : public ISystem
{
private:
	explicit CSSAO() DEFAULT;
	virtual ~CSSAO() DEFAULT;

public:
	HRESULT Render();

public:
	static shared_ptr<CSSAO> Create();
};

END