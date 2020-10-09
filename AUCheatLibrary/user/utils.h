#pragma once
#include "il2cpp-appdata.h"

using namespace app;

bool IsLocalPlayerExist();
bool IsGameStarted();

PlayerControl* GetLocalPlayer();
GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ* playerInfo, uint32_t id);