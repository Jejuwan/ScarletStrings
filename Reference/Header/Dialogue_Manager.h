#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CDialogue_Manager final : public CSingleton<CDialogue_Manager>
{
public:
	typedef struct DialogueDesc
	{
		_uint		iScriptIndex;
		wstring		strName;
		wstring		strText1, strText2;
		_float		fLifeTime;

	} DIALOGUEDESC;

public:
	explicit CDialogue_Manager();
	virtual ~CDialogue_Manager() DEFAULT;

	HRESULT	Initialize(_uint iWinCX, _uint iWinCY);
	void	Tick(_float fTimeDelta);
	HRESULT	Render();

public:
	void							Play_Dialogue(_uint _iScriptIndex, const wstring& _strName, const wstring& _strText1 = L"", const wstring& _strText2 = L"", _float _fDuration = 0.f);
	void							Stop_Dialogue() { m_bDialogRendering = false; }

public:



private:
	_bool							m_bDialogRendering = { false };
	_float							m_fAccTime;

private:
	DIALOGUEDESC					m_tDialogues;
	_float2							m_vResolution;

private:
	friend	CSingleton<CDialogue_Manager>;

};

END