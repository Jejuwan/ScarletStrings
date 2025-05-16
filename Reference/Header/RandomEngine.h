#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CRandomEngine final : public CSingleton<CRandomEngine>
{
private:
	CRandomEngine();
	virtual ~CRandomEngine() DEFAULT;

public:
	_float Get_RandomUNORM();

private: /* Random Engine */
	std::mt19937_64								m_RandomEngine;
	std::uniform_real_distribution<_float>		m_RandomUNORM;

	friend CSingleton<CRandomEngine>;
};

END