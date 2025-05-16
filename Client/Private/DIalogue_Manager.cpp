#include "ClientPCH.h"
#include "DIalogue_Manager.h"

CDialogue_Manager::CDialogue_Manager()
{
}

CDialogue_Manager::~CDialogue_Manager()
{
}

void CDialogue_Manager::Add_Dialogue(_int _iNumber, const wstring& _strName, const wstring& _strText)
{





	Dialogues.push_back({ _iNumber, _strText });
}

wstring CDialogue_Manager::Get_Dialogue(_int _iNumber)
{
	for (const DIALOGUEDESC& dialogue : Dialogues)
	{
		if (_iNumber == dialogue.iNumber)
			return dialogue.strText;
	}
	return L"Not Exist Dialogue";
}