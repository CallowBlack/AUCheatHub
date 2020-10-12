#include "pch-il2cpp.h"
#include "utils.h"
#include <TlHelp32.h>
#include <iostream>

PlayerControl__StaticFields* playerStatic;

bool IsLocalPlayerExist()
{
	return GetLocalPlayer() != nullptr;
}

bool IsGameStarted()
{
    auto clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    return clientStatic->Instance->fields._.GameState == InnerNetClient_IFLBIJFJPMK__Enum_Started;
}

int GetGameState()
{
    auto clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    return (int)clientStatic->Instance->fields._.GameState;
}

PlayerControl* GetLocalPlayer()
{
    if (playerStatic == nullptr)
        playerStatic = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
	return playerStatic->LocalPlayer;
}

GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ* playerInfo, uint32_t id) {
    auto tasks = playerInfo->fields.IHACFCJPFCF;
    for (int i = 0; i < tasks->fields._size; i++) {
        auto taskInfo = tasks->fields._items->vector[i];
        auto taskId = taskInfo->fields.AKLEDCMKHMC;
        if (taskId == id)
            return taskInfo;
    }
    return NULL;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}
