#pragma once
#include "Client_Define.h"
#include "Sequence.h"

#ifdef _DEBUG

BEGIN(Client)

class CDebugSequence abstract : public CSequence
{
protected:
	explicit CDebugSequence() DEFAULT;
	virtual ~CDebugSequence() DEFAULT;

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
