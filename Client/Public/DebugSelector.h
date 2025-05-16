#pragma once
#include "Client_Define.h"
#include "Selector.h"

#ifdef _DEBUG

BEGIN(Client)

class CDebugSelector abstract : public CSelector
{
protected:
	explicit CDebugSelector() DEFAULT;
	virtual ~CDebugSelector() DEFAULT;

public:
	virtual void	Activate() override;
	virtual void	Terminate() override;

public:
	virtual void	Debug(any = g_aNull) override;
	virtual void	DebugLog();

protected:
	string			m_strNodeName;

private:
	_bool			m_bEventCall	= false;
	_bool			m_bOpenItem		= false;
	_bool			m_bOpenMode		= false;
};

END

#endif
