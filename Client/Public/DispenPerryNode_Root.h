#pragma once
#include "Client_Define.h"
#ifdef _DEBUG
#include "DebugSelector.h"
#else
#include "Selector.h"
#endif

#define ATK_ARMSWIPE_COOLTIME		3.f
#define ATK_ARMSWIPE_RANGE_MIN		0.f
#define ATK_ARMSWIPE_RANGE_MAX		15.f

#define ATK_WATERSHOT_COOLTIME		6.f
#define ATK_WATERSHOT_RANGE_MIN		10.f
#define ATK_WATERSHOT_RANGE_MAX		60.f

#define ATK_LEAP_COOLTIME			30.f
#define ATK_LEAP_RANGE_MIN			25.f
#define ATK_LEAP_RANGE_MAX			35.f
#define ATK_LEAP_RANGE_FORCE		2.f

BEGIN(Client)

class CDispenPerryNode_Root final
#ifdef _DEBUG
	: public CDebugSelector
#else
	: public CSelector
#endif
{
private:
	explicit CDispenPerryNode_Root() DEFAULT;
	virtual ~CDispenPerryNode_Root() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;
#ifdef _DEBUG
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;
#endif

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

private:
#ifdef _DEBUG
	virtual void										Debug(any = g_aNull) override;
#if ACTIVATE_IMGUI
	virtual void										DebugLog() override;
#endif
#endif
	HRESULT												Ready_Skills();

private:
	weak_ptr<class CMonster_DispenPerry>				m_pOwner;
	DP_STATE*											m_pState		= nullptr;
	const DP_PHASE*										m_pDPPhase		= nullptr;

	_bool												m_bResetTree	= false;
#ifdef _DEBUG
	const ENTITYDESC*									m_pEntityDesc	= nullptr;

	shared_ptr<class CWaterTank>						m_pWaterTank;

	map<wstring, shared_ptr<class CTestAlphaBuffer>>	m_mapAlphaBuffer;

	shared_ptr<CRenderer>								m_pRenderer;
#endif

	shared_ptr<class CSkillContainer>					m_pSkillContainer;

public:
	static shared_ptr<CDispenPerryNode_Root>			Create(shared_ptr<class CBlackBoard>);
};

END

#pragma region Note
/*
	MEMO:
	STORAGE HP
	STORAGE HIT:	90-	:	DAMAGE-I
					80-	:	DAMAGE-I
					70-	:	DAMAGE-M
					60-	:	DAMAGE-I
					50-	:	DAMAGE-I
					40-	:	DAMAGE-M
					30-	:	DOWN
					20-	:	DAMAGE-I
					10-	:	DAMAGE-M
	
	PHASE1->PHASE2		:	THREAT
	
	MELEE				:	FAS->SAS
	RANGE				:	WS->MWS
	
	LEAP-TRUCK			:	DOWN-SHOCK
	ELSE					BACKRUN
	
	SAS->BACKRUN
	
	[ROOT]
	[SELECTOR] BT
		[DECORATOR-HIT]
		[LEAF] HIT
		[SELECTOR] PHASE
			[DECORATOR:HP>60]
			[SELECTOR]	PHASE1
				[DECORATOR:RANGE:~10]
				[DECORATOR:SKILL]			FAS
				[DECORATOR:RANGE:20~]
				[DECORATOR:SKILL]			WS
				[DECORATOR:RANGE:10~20]
				[DECORATOR:SKILL]			LEAP
				[DECORATOR:REPEAT:1.5F]		WALK
			[DECORATOR:HP<60]
			[SELECTOR]	PHASE2
				[DECORATOR:RANGE:~10]
				[DECORATOR:SKILL]			SAS
				[DECORATOR:RANGE:20~]
				[DECORATOR:SKILL]			MWS
				[DECORATOR:RANGE:10~20]
				[DECORATOR:SKILL]			LEAP
				[DECORATOR:REPEAT:1.5F]		WALK
*/
#pragma endregion
