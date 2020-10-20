#include "pch-il2cpp.h"
#include "cheat-manager.h"

#include "ImGUI/imgui.h"
#include "GUI/CheatModule/DebugModule.h"
static bool isShowMain = false;
static bool isBlocked = false;
static bool isShowOptions = false;
static std::vector<std::shared_ptr<ICheatModule>> cheatModules{};
static DebugModule dmodule{};

void Render()
{
	if (!isShowMain)
		return;
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Cheat Manager"))
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Block game keyboard, mouse", &isBlocked);

	ImGui::Spacing();
	for (auto cmIter = cheatModules.begin(); cmIter < cheatModules.end(); cmIter++) {
		auto pModule = *cmIter;
		ImGui::PushID(pModule->GetName());
		pModule->OnRender();
		ImGui::PopID();
	}

	ImGui::End();

	if (!ImGui::Begin("Debug"))
	{
		dmodule.OnRender();
		ImGui::End();
		return;
	}
	
	dmodule.OnRender();
	ImGui::End();
	
}

short OnKeyUp(DWORD key)
{
	if (key == VK_F1) {
		isShowMain = !isShowMain;
	}

	short flag = 0;
	if (isShowMain) {
		flag |= BType_SendImGUI;
		if (isBlocked)
			flag |= BType_Blocked;
	}

	return flag;
}

void AddModule(std::shared_ptr<ICheatModule> cheatModule) {
	cheatModules.push_back(cheatModule);
}