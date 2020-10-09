#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"

#include "hack.h"
#include "utils.h"

using namespace app;

const char* sLocalPlayerFailed = "failed: Player doesn't exist";

void SetUnlimitSabbotage(bool state)
{
    auto handle = GetCurrentProcess();
    
    // Check isDoorCanUsed
    WriteProcessMemory(handle, (char*)MapRoom_SabotageDoors + 0x3d, state ? "\x90\x90\x90\x90\x90\x90": "\x0f\x84\xac\x00\x00\x00", 6, NULL);
    // Check is timer end
    WriteProcessMemory(handle, (char*)MapRoom_SabotageDoors + 0xcf, state ? "\x90\x90" : "\x77\x1e", 2, NULL);

    // For other sabotages
    WriteProcessMemory(handle, (char*)MapRoom_SabotageLights + 0x38,    state ? "\x90\x90" : "\x74\x1d", 2, NULL);
    WriteProcessMemory(handle, (char*)MapRoom_SabotageComms + 0x38,     state ? "\x90\x90" : "\x74\x1d", 2, NULL);
    WriteProcessMemory(handle, (char*)MapRoom_SabotageOxygen + 0x38,    state ? "\x90\x90" : "\x74\x1d", 2, NULL);
    WriteProcessMemory(handle, (char*)MapRoom_SabotageReactor + 0x38,   state ? "\x90\x90" : "\x74\x1d", 2, NULL);
    WriteProcessMemory(handle, (char*)MapRoom_SabotageSeismic + 0x38,   state ? "\x90\x90" : "\x74\x1d", 2, NULL);

    CloseHandle(handle);
}

void SetGhostState(bool state)
{
    if (!IsLocalPlayerExist()) {
        printf("Error: Fake Impostor %s\n", sLocalPlayerFailed);
        return;
    }
    auto playerInfo = GetLocalPlayer()->fields.FMDMBBNEAHH;
    playerInfo->fields.DMFDFKEJHLH = !playerInfo->fields.DMFDFKEJHLH;
}

void SetInfectedState(bool state)
{
    if (!IsLocalPlayerExist()) {
        printf("Error: Fake Impostor %s\n", sLocalPlayerFailed);
        return;
    }

    // FMDMBBNEAHH -> PlayerData
    auto playerInfo = GetLocalPlayer()->fields.FMDMBBNEAHH;

    // LODLBBJNGKB -> InfectedState
    playerInfo->fields.LODLBBJNGKB = state;
    
    printf("State: Fake Impostor %s\n", state ? "enabled" : "disabled");
}

void CompleteAllTasks()
{
    if (!IsLocalPlayerExist()) {
        printf("Error: Complete all tasks %s\n", sLocalPlayerFailed);
        return;
    }

    auto localPlayer = GetLocalPlayer();
    auto playerInfo = localPlayer->fields.FMDMBBNEAHH;
    auto tasks = localPlayer->fields.myTasks->fields;
    for (uint32_t i = 0; i < tasks._size; i++) {
        auto taskInfo = FindTask(playerInfo, i);
        if (!taskInfo->fields.MEAOHEIBBKN) {
            std::cout << "Completing task #" << i << std::endl;
            PlayerControl_RpcCompleteTask(localPlayer, i, NULL);
        }
    }
}

