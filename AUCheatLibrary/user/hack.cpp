#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"

#include "hack.h"
#include "utils.h"

using namespace app;

const char* sLocalPlayerFailed = "failed: Player doesn't exist";

int localNetId = 0;

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
    auto gameBase = GetModuleBaseAddress(GetCurrentProcessId(), L"GameAssembly.dll");
    String* layerName;
    if (state) {
        layerName = *(String**)(gameBase + 0x14779D4); // app::String* "Ghost"
    }
    else {
        layerName = *(String**)(gameBase + 0x147759C); // app::String* "Players"
    }
    auto gameObject = Component_get_gameObject((Component*)GetLocalPlayer(), 0);
    auto layer = LayerMask_NameToLayer(layerName, 0);
    GameObject_set_layer(gameObject, layer, 0);
    auto playerInfo = GetLocalPlayer()->fields.FMDMBBNEAHH;
    playerInfo->fields.DMFDFKEJHLH = state;
    printf("Status: Ghost state %s\n", state ? "enabled" : "disabled");
}

void SetInfectedState(bool state)
{
    if (!IsLocalPlayerExist()) {
        printf("Error: Fake Impostor %s\n", sLocalPlayerFailed);
        return;
    }

    // FMDMBBNEAHH -> PlayerData
    auto playerInfo = GetLocalPlayer()->fields.FMDMBBNEAHH;

    localNetId = GetLocalPlayer()->fields._.NetId;
    if (state)
    {
        auto playerStatic = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
        auto allControls = playerStatic->AllPlayerControls;
        for (int i = 0; i < allControls->fields._size; i++) {
            auto player = allControls->fields._items->vector[i];
            auto playerNetId = player->fields._.NetId;
            if (player->fields.FMDMBBNEAHH->fields.LODLBBJNGKB && playerNetId != localNetId) {
                GetLocalPlayer()->fields._.NetId = playerNetId;
            }
        }
    }
    else {
        GetLocalPlayer()->fields._.NetId = localNetId;
    }
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
        if (taskInfo != nullptr && !taskInfo->fields.MEAOHEIBBKN) {
            PlayerControl_RpcCompleteTask(localPlayer, i, NULL);
        }
    }
}

void SetKillNoReload(bool state) {
    if (!IsLocalPlayerExist()) {
        printf("Error: Kill no reload %s\n", sLocalPlayerFailed);
        return;
    }
    auto handle = GetCurrentProcess();
    WriteProcessMemory(handle, (char*)PlayerControl_MurderPlayer + 0x238, state ? "\x90\x90\x90\x90\x90" : "\xe8\xa3\x2d\x00\x00", 5, NULL);
    printf("Status: Kill no-reload was %s", state ? "enabled" : "disabled");
    CloseHandle(handle);
}

void GetAllPlayersNetID() {
    auto playerStatic = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
    auto allControls = playerStatic->AllPlayerControls;
    for (int i = 0; i < allControls->fields._size; i++) {
        auto player = allControls->fields._items->vector[i];
        printf("Player with id %d have netid %d\n", player->fields.PlayerId, player->fields._.NetId);
    }
}