#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

#include "helpers.h"

// Cheat functionality
#include "utils.h"
#include "hack.h"

// GUI
#include "GUI/cheat-manager.h"
#include "GUI/game-overlay.h"
#include "GUI/CheatModule/SimpleItem.h"

using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

void Run()
{
    il2cppi_new_console();

	AddModule(std::make_shared<SimpleItem>((char*)"Infection State", SetInfectedState, IsLocalPlayerExist, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Ghost State", SetGhostState, IsLocalPlayerExist, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Sabbotage no-reload", SetUnlimitSabbotage, IsLocalPlayerExist, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Complete all task", CompleteAllTasks, IsLocalPlayerExist, false));
	AddModule(std::make_shared<SimpleItem>((char*)"Kill no-reload", SetKillNoReload, IsLocalPlayerExist, true));

	createOverlay();

}