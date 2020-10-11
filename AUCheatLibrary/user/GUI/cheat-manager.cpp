#include "pch-il2cpp.h"
#include "cheat-manager.h"

#include "ImGUI/imgui.h"

static bool isShowMain = false;
static bool isBlocked = true;
static bool isShowOptions = false;
static std::vector<std::shared_ptr<ICheatModule>> cheatModules{};

void Render()
{
	if (!isShowMain)
		return;

	if (!ImGui::Begin("Cheat Manager"))
	{
		ImGui::End();
		return;
	}

	for (auto cmIter = cheatModules.begin(); cmIter < cheatModules.end(); cmIter++) {
		auto pModule = *cmIter;
		ImGui::PushID(pModule->GetName());
		pModule->OnRender();
		ImGui::PopID();
	}

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

void ShowOptionWindow() {
	
}
