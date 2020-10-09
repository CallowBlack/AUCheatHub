// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";
HHOOK hkb;
PlayerControl__StaticFields *control;
bool enabled = false;
// Custom injected code entry point
void UpdateInfo();

GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ *playerInfo, uint32_t id) {
    auto tasks = playerInfo->fields.IHACFCJPFCF;
    for (int i = 0; i < tasks->fields._size; i++) {
        auto taskInfo = tasks->fields._items->vector[i];
        auto taskId = taskInfo->fields.AKLEDCMKHMC;
        if (taskId == id)
            return taskInfo;
    }
    return NULL;
}


void WriteMemory(void* pBase, char* pBuffer, size_t szBuffer) {
    auto chBase = (char*)pBase;
    printf("Start changing");
    for (size_t i = 0; i < szBuffer; i++) {
        try {
            chBase[i] = pBuffer[i];
        }
        catch (...) {
            printf("exception");
        }
        
    }
}


LRESULT __declspec(dllexport)__stdcall CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (HC_ACTION == nCode && wParam == WM_KEYDOWN)
    {
        auto key = ((tagKBDLLHOOKSTRUCT*)lParam)->vkCode;
        if (enabled) {
            if (control->LocalPlayer != nullptr) {
                if (key == '1')
                {
                    std::cout << "Fake Impostor state changed." << std::endl;
                    auto playerInfo = control->LocalPlayer->fields.FMDMBBNEAHH;
                    playerInfo->fields.LODLBBJNGKB = !playerInfo->fields.LODLBBJNGKB;
                }
                else if (key == '2')
                {
                    std::cout << "Ghost state changed." << std::endl;
                    auto playerInfo = control->LocalPlayer->fields.FMDMBBNEAHH;
                    playerInfo->fields.DMFDFKEJHLH = !playerInfo->fields.DMFDFKEJHLH;
                }
                else if (key == '6') {
                    auto handle = GetCurrentProcess();
                    // \x0f\x84\xac\x00\x00\x00
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageDoors + 0x3d, (char*)"\x90\x90\x90\x90\x90\x90", 6, NULL);

                    // \x77\x1e
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageDoors + 0xcf, (char*)"\x90\x90", 2, NULL);

                    // \x74\x1d
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageLights + 0x38, (char*)"\x90\x90", 2, NULL);

                    // \x74\x1d
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageComms + 0x38, (char*)"\x90\x90", 2, NULL);

                    // \x74\x1d
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageOxygen + 0x38, (char*)"\x90\x90", 2, NULL);

                    // \x74\x1d
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageReactor + 0x38, (char*)"\x90\x90", 2, NULL);

                    // \x74\x1d
                    WriteProcessMemory(handle, (char*)MapRoom_SabotageSeismic + 0x38, (char*)"\x90\x90", 2, NULL);

                    CloseHandle(handle);
                }
                else if (key == '7') {

                }
                else if (key == '8') {
                    auto localPlayer = control->LocalPlayer;
                    auto playerInfo = localPlayer->fields.FMDMBBNEAHH;
                    auto tasks = localPlayer->fields.myTasks->fields;
                    for (uint32_t i = 0; i < tasks._size; i++) {
                        auto taskInfo = FindTask(playerInfo, i);
                        if (!taskInfo->fields.MEAOHEIBBKN) {
                            std::cout << "Completing task #" << i << std::endl;
                            PlayerControl_RpcCompleteTask(control->LocalPlayer, i, NULL);
                        }
                    }
                }
                else if (key == '9') {
                    std::cout << "Method: " << PlayerControl_Revive << std::endl;
                }
                else if (key == '0') {
                    
                    std::cout << "Complete task..." << std::endl;
                }
            }
            if (key == '5') {
                UpdateInfo();
            }
        }
        if (key == VK_F1) {
            enabled = !enabled;
        }
    }

    LRESULT RetVal = CallNextHookEx(hkb, nCode, wParam, lParam);
    return  RetVal;
}

void UpdateInfo() {
    auto client = *reinterpret_cast<AmongUsClient**>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
   
    std::cout << "AmongUsClient:  " << client << std::endl;

    auto shipPrefabs = client->fields.ShipPrefabs;
    std::cout << "ShipPrefabs:    " << shipPrefabs << std::endl;

    control = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
    std::cout << "PlayerControl:  " << control->LocalPlayer << std::endl;

    std::cout << "Kill method: " << PlayerControl_RpcMurderPlayer << std::endl;
    std::cout << "InnerNet:    " << InnerNetClient_StartRpcImmediately << std::endl;
    if (control->LocalPlayer != NULL) {
        auto shipStaticAddress = il2cpp_class_get_static_field_data((Il2CppClass*)*ShipStatus__TypeInfo);
        std::cout << "Ship static address calculated." << std::endl;
        if (shipStaticAddress == NULL) {
            std::cout << "Ship doesn't exist." << std::endl;
        }
        else {
            auto shipStatic = reinterpret_cast<ShipStatus__StaticFields*>(shipStaticAddress);
            auto ship = shipStatic->Instance;
            std::cout << "Ship:     " << ship << std::endl;

            auto mapBehavior = reinterpret_cast<MapBehaviour__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*MapBehaviour__TypeInfo))->Instance;
            std::cout << "Map behavior: " << mapBehavior << std::endl;
        }
    }
}

void Run()
{
    // If you would like to write to a log file, specify the name above and use il2cppi_log_write()
    // il2cppi_log_write("Startup");

    il2cppi_new_console();
    UpdateInfo();

    hkb = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, NULL, 0);
    if (hkb == NULL) {
        std::cout << "Keyboard hook failed!" << std::endl;
        std::cout << GetLastError() << std::endl;
    }

    BOOL bRet;
    MSG msg;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            std::cout << "Handle message error." << std::endl;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}