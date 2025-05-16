#pragma once
#include "Client_Define.h"
#include "Decorator.h"

#ifdef _DEBUG

BEGIN(Client)

class CDebugDecorator abstract : public CDecorator
{
protected:
	explicit CDebugDecorator(shared_ptr<CNode>);
	virtual ~CDebugDecorator() DEFAULT;

public:
	virtual void	Activate() override;
	virtual void	Terminate() override;

public:
	virtual void	Debug(any = g_aNull) override;
	virtual void	DebugLog();

protected:
	string			m_strNodeName;

protected:
	_bool			m_bEventCall	= false;
	_bool			m_bOpenItem		= false;
	_bool			m_bOpenMode		= false;
};

END

#endif
