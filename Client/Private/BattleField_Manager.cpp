#include "ClientPCH.h"
#include "BattleField_Manager.h"

#include "BattleField.h"
#include "GameInstance.h"

CBattleField_Manager::CBattleField_Manager()
{
}

#ifdef _DEBUG
void CBattleField_Manager::Debug_Render()
{
#ifdef ACTIVATE_IMGUI
	SCENE eScene = CGameInstance::Get_Instance()->Current_Scene();
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("BattleField Manager"))
	{
	_uint iBattleFieldSize = (_uint)m_vecBattleField[IDX(eScene)].size();
	for (_uint i = 0; i < iBattleFieldSize; ++i)
	{
		string strLabel = "Enable BattleField #" + std::to_string(i);

		_bool bEnable = m_vecBattleField[IDX(eScene)][i]->Get_Enable();
		if (ImGui::Checkbox(strLabel.c_str(), &bEnable))
		{
			m_vecBattleField[IDX(eScene)][i]->Set_Enable(bEnable);
		}
	}

	static _int iEnableIndex = -1;
	auto fpSwitchBattleField = [&](_uint iIndex)
		{
			_uint iBattleFieldSize = (_uint)m_vecBattleField[IDX(CGameInstance::Get_Instance()->Current_Scene())].size();
			for (_uint i = 0; i < iBattleFieldSize; ++i)
			{
				if (i == iIndex)
					m_vecBattleField[IDX(CGameInstance::Get_Instance()->Current_Scene())][i]->Set_Enable(true);
				else
					m_vecBattleField[IDX(CGameInstance::Get_Instance()->Current_Scene())][i]->Set_Enable(false);
			}
		};
	ImGui::Text("Can Switch BattleField Index to (¡ç , ¡æ)");
	if (CGameInstance::Get_Instance()->Key_Down(VK_LEFT))
	{
		iEnableIndex = max(0, iEnableIndex - 1);
		fpSwitchBattleField(iEnableIndex);
	}
	if (CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
	{
		iEnableIndex = min(iBattleFieldSize - 1, iEnableIndex + 1);
		fpSwitchBattleField(iEnableIndex);
	}

	ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}
	#endif
}
#endif

void CBattleField_Manager::Bind_BattleField(SCENE eScene, shared_ptr<CBattleField> pBattleField)
{
	m_vecBattleField[IDX(eScene)].push_back(pBattleField);
	pBattleField->Set_Enable(false);
}

void CBattleField_Manager::Enable_BattleField(SCENE eScene, _uint iIndex, _bool bEnable)
{
	m_vecBattleField[IDX(eScene)][iIndex]->Set_Enable(bEnable);
}
