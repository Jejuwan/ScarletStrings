#pragma once

#include "PlayerState.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)

class CPlayerBrainFieldCutsceneState : public CPlayerState
{
public:
	CPlayerBrainFieldCutsceneState() DEFAULT;
	virtual ~CPlayerBrainFieldCutsceneState() = default;

	virtual shared_ptr<CPlayerState> Transition(class CPlayer& _player);
	virtual void Entry(class CPlayer& _player);
	virtual void Update(class CPlayer& _player, _float fTimeDelta);
	virtual void Exit(class CPlayer& _player);

#pragma region Event Callback
private:
	void Event0();
	void Event1();
	void Event2();
	void Event3();
	void Event4();
	void Event5();
	void Event6();
	void Event7();
	void Event8();
	void Event9();
	void Event10();
	void Event11();
	void Event12();
	void Event13();
	void Event14();
	void Event15();
	void Event16();
	void Event17();
	void Event18();
	void Event19();
	void Event20();
	void Event21();
	void Event22();
	void Event23();
	void Event24();
	void Event25();
	void Event26();
#pragma endregion

private:
	_bool	m_bBrainFieldOpened = { false };

private:
	_float	m_fOriginBloomStrength = { 0.f };
	_float	m_fOriginExposure = { 0.f };
	_float	m_fOriginGamma = { 0.f };

private:
	shared_ptr<CRenderer> m_pRenderer;
};

END