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

AmongUsClient* GetClient() {
    auto clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    return clientStatic->Instance;
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

bool GetResourceMemory(HINSTANCE hInstance, int resId, LPBYTE& pDest, DWORD &size) {
    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(resId), RT_RCDATA);
    if (hResource) {
        HGLOBAL hGlob = LoadResource(hInstance, hResource);
        if (hGlob) {
            size = SizeofResource(hInstance, hResource);
            pDest = (LPBYTE)LockResource(hGlob);
            if (size > 0 && pDest)
                return true;
        }
    }
    return false;
}

bool EnabledButton(bool enabled, const char* text, const ImVec2& size) {
    bool result = false;
    if (!enabled)
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

    if (ImGui::Button(text, size) && enabled)
        result = true;

    if (!enabled)
        ImGui::PopStyleVar();
    return result;
}


ClientData* GetPlayerClientById(int id) {
    auto clients = GetClient()->fields._.allClients;
    for (int i = 0; i < clients->fields._size; i++) {
        if (clients->fields._items->vector[i]->fields.Character->fields.PlayerId == id) {
            return clients->fields._items->vector[i];
        }
    }
    return NULL;
}

ClientData* GetClientByClientId(int32_t clientId) {
    auto clients = GetClient()->fields._.allClients;
    for (int i = 0; i < clients->fields._size; i++) {
        auto client = clients->fields._items->vector[i];
        if (client->fields.Id == clientId) {
            return client;
        }
    }
    return NULL;
}

bool IsPlayerVoted(uint8_t playerId) {
    auto meetingHudStatic = reinterpret_cast<MeetingHud__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*MeetingHud__TypeInfo));
    if (meetingHudStatic->Instance) {
        auto playerVotes = meetingHudStatic->Instance->fields.FALDLDJHDDJ;
        for (int i = 0; i < playerVotes->max_length; i++) {
            auto playerVote = playerVotes->vector[i];
            if (playerVote->fields.TargetPlayerId == playerId)
                return playerVote->fields.didVote;
        }
    }
    return false;
}