#pragma once
#include "Client_Define.h"
#include "Node.h"

#ifdef _DEBUG

BEGIN(Client)

class CDebugNode abstract : public CNode
{
protected:
	explicit CDebugNode() DEFAULT;
	virtual ~CDebugNode() DEFAULT;

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
