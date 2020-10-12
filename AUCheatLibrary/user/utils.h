#pragma once
#include "il2cpp-appdata.h"
#include <Windows.h>
using namespace app;

bool IsLocalPlayerExist();
bool IsGameStarted();

int GetGameState();

PlayerControl* GetLocalPlayer();
GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ* playerInfo, uint32_t id);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);