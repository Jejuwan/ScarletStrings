#pragma once
#include "PlayerState.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)

class CPlayerBrainFieldCutsceneFinalState : public CPlayerState
{
public:
	CPlayerBrainFieldCutsceneFinalState() DEFAULT;
	virtual ~CPlayerBrainFieldCutsceneFinalState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

#pragma region Event Callback
private:
	void Event1();
	void Event2();
	void Event3();
	void Event4();
	void Event5();
	void Event6();
#pragma endregion

private:
	shared_ptr<CRenderer> m_pRenderer;

private:
	_bool					m_bCutsceneFinished = { false };
};

END