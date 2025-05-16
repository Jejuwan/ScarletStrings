#pragma once

#include "Client_Define.h"

BEGIN(Client)

class CBattleField_Manager : public CSingleton<CBattleField_Manager>
{
private:
	explicit CBattleField_Manager();
	virtual ~CBattleField_Manager() DEFAULT;

#ifdef _DEBUG
public:
	void											Debug_Render();
#endif

public:
	void											Bind_BattleField(SCENE eScene, shared_ptr<class CBattleField>);
	void											Enable_BattleField(SCENE eScene, _uint iIndex, _bool bEnable);

private:
	vector<shared_ptr<class CBattleField>>			m_vecBattleField[IDX(SCENE::MAX)];

private:
	ComPtr<ID3D11Device>							m_pDevice;
	ComPtr<ID3D11DeviceContext>						m_pContext;

	friend		CSingleton<CBattleField_Manager>;
};

END