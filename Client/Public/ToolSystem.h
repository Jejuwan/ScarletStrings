#pragma once
#include "Client_Define.h"
#include "System.h"

#if ACTIVATE_TOOL

BEGIN(Client)

class IToolSystem abstract : public ISystem
{
protected:
	explicit IToolSystem(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
		: m_pDevice(pDevice), m_pContext(pContext)
	{}
	virtual ~IToolSystem() DEFAULT;

public:
	virtual void					Tool_MenuBar()		PURE;
	virtual void					Tool_System()		PURE;
	virtual void					Tool_Properties()	PURE;
	virtual void					Tool_MainControl()	PURE;
	virtual void					Tool_SubControl()	PURE;

protected:
	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;
};

END

#endif
