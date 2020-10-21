#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "helpers.h"

// Cheat functionality
#include "utils.h"
#include "hack.h"

// GUI
#include "GUI/cheat-manager.h"
#include "GUI/game-overlay.h"
#include "GUI/CheatModule/SimpleItem.h"
#include "GUI/CheatModule/PlayersModule.h"

using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";


void WINAPI Run(LPVOID phModule)
{
	setlocale(LC_ALL, "");
	il2cppi_new_console();

	AddModule(std::make_shared<SimpleItem>((char*)"Infection State", SetInfectedState, IsGameStarted, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Ghost State", SetGhostState, IsGameStarted, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Complete all task", CompleteAllTasks, IsGameStarted, false));

	AddModule(std::make_shared<SimpleItem>((char*)"Sabbotage no-reload", SetUnlimitSabbotage, nullptr, true));
	AddModule(std::make_shared<SimpleItem>((char*)"Kill no-reload", SetKillNoReload, nullptr, true));

	AddModule(std::make_shared<PlayersModule>());
	
	LPBYTE pFont = 0;
	DWORD dFontSize = 0;
	GetResourceMemory(*static_cast<HMODULE*>(phModule), 105, pFont, dFontSize);
	createOverlay(pFont, dFontSize);

}

