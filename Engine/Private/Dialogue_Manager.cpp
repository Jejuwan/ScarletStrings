#include "EnginePCH.h"
#include "Dialogue_Manager.h"
#include "GameInstance.h"

CDialogue_Manager::CDialogue_Manager()
{
}

HRESULT CDialogue_Manager::Initialize(_uint iWinCX, _uint iWinCY)
{
	m_vResolution = { _float(iWinCX), _float(iWinCY) };

	return S_OK;
}

void CDialogue_Manager::Tick(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if (m_tDialogues.fLifeTime <= m_fAccTime)
	{
		m_bDialogRendering = false;
	}
}

HRESULT CDialogue_Manager::Render()
{
	if (m_bDialogRendering)
	{
		_float fXPivot = m_vResolution.x / 3.f;
		_float fYPivot = m_vResolution.y / 1.2f;

		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_tDialogues.strName,  _float2(fXPivot, fYPivot), XMVectorReplicate(1.f), 0.5f, 0.f);
		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_tDialogues.strText1, _float2(fXPivot, fYPivot + 30.f), XMVectorReplicate(1.f), 0.5f, 0.f);
		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_tDialogues.strText2, _float2(fXPivot, fYPivot + 60.f), XMVectorReplicate(1.f), 0.5f, 0.f);
	}

	return S_OK;
}

void CDialogue_Manager::Play_Dialogue(_uint _iScriptIndex , const wstring& _strName, const wstring& _strText1, const wstring& _strText2, _float _fLifeTime)
{
	m_bDialogRendering = true;
	m_fAccTime = 0.f;
	m_tDialogues = DIALOGUEDESC{ _iScriptIndex, _strName, _strText1, _strText2, _fLifeTime };
}