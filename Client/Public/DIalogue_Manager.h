#pragma once
#include "Client_Define.h"

BEGIN(Client)

class CDialogue_Manager : public CSingleton<CDialogue_Manager>
{
public:
	typedef struct DialogueDesc
	{
		_int		iNumber;
		wstring		strName;
		wstring		strText;
	} DIALOGUEDESC;

private:
	explicit CDialogue_Manager();
	virtual ~CDialogue_Manager() DEFAULT;

public:
	/* ��� �߰� */
	void						Add_Dialogue(_int _iNumber, const wstring& _strName, const wstring& _strText);

	/* ��� ȣ�� */
	wstring						Get_Dialogue(_int _iNumber);

private:
	vector<DIALOGUEDESC>		Dialogues;

private:
	friend						CSingleton<CDialogue_Manager>;
};

END