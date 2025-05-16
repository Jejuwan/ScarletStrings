#pragma once
#include "Client_Define.h"

BEGIN(Client)

class CEffectInstance_ParticlePointVector final
{
private:
	explicit CEffectInstance_ParticlePointVector();
	explicit CEffectInstance_ParticlePointVector(const CEffectInstance_ParticlePointVector&);
	virtual ~CEffectInstance_ParticlePointVector() DEFAULT;

public:
	static shared_ptr<CEffectInstance_ParticlePointVector>	Create();
};

END
