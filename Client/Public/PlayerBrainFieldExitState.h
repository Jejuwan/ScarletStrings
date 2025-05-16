#pragma once
#include "PlayerState.h"
class CPlayerBrainFieldExitState :
    public CPlayerState
{
public:
    CPlayerBrainFieldExitState() DEFAULT;
    ~CPlayerBrainFieldExitState() DEFAULT;

    virtual shared_ptr<CPlayerState>    Transition(class CPlayer& _player);
    virtual void                        Entry(class CPlayer& _player);
    virtual void                        Update(class CPlayer& _player, _float fTimeDelta);
    virtual void                        Exit(class CPlayer& _player);

private:
    void                                ExitBrainField(class CPlayer& _player);

private:
    _bool                               m_bExitStart = { false };

    _float                              m_fAccTime = { 0.f };
    _float                              m_fExitDelay = { 15.f };
    _float                              m_fScreenEffect = { 10.f };

};

