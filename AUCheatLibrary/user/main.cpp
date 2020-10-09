#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#include "helpers.h"

#include "CheatTable.h"
#include "hack.h"

using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

// Hook for keyboard
HHOOK keyboardHook;

bool enabled = false;

CheatTable cheatTable;

LRESULT __declspec(dllexport)__stdcall CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (HC_ACTION == nCode && wParam == WM_KEYDOWN)
    {
        auto key = ((tagKBDLLHOOKSTRUCT*)lParam)->vkCode;
        if (enabled) {
            cheatTable.RunCheat(key);
        }
        if (key == VK_F1) {
            enabled = !enabled;
            printf("Status: Cheat keys was %s\n", enabled ? "enabled" : "disabled");
        }
    }

    LRESULT RetVal = CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    return  RetVal;
}


void Run()
{
    il2cppi_new_console();

    cheatTable = {
        {'1', "Infection State", SetInfectedState, true},
        {'2', "Ghost State",  SetGhostState, true},
        {'3', "Sabbotage no-reload", SetUnlimitSabbotage, true},
        {'4', "Complete all task", CompleteAllTasks, false}
    };
    // Setting low level keyboard hook
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
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