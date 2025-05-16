#pragma once
#include "PlayerState.h"

BEGIN(Client)

class CPlayerBrainFieldStartState : public CPlayerState
{
public:
	CPlayerBrainFieldStartState() DEFAULT;
	virtual ~CPlayerBrainFieldStartState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

private:
	vector<shared_ptr<class CCutsceneTentacle>> m_vecCutsceneTentacle;

private:
	_bool										m_bCamZoomIn = { false };

private:


#pragma region Event Callback
private:
	void Execute_TentacleEffect();
	void Create_Tentacle();
	void Tentacle_Anim();
	void ZoomIn();
#pragma endregion
};

END