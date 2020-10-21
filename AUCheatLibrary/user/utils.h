#pragma once
#include "il2cpp-appdata.h"
#include <Windows.h>
#include "ImGUI/imgui.h"
using namespace app;

bool IsLocalPlayerExist();
bool IsGameStarted();
int GetGameState();
PlayerControl* GetLocalPlayer();
GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ* playerInfo, uint32_t id);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
bool GetResourceMemory(HINSTANCE hInstance, int resId, LPBYTE& pDest, DWORD& size);
bool EnabledButton(bool enabled, const char* text, const ImVec2& size);
ClientData* GetPlayerClientById(int id);
ClientData* GetClientByClientId(int32_t clientId);
bool IsPlayerVoted(uint8_t playerId);