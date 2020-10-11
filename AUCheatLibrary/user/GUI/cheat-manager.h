#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include "CheatModule/ICheatModule.h"

const char BType_Blocked = 1;
const char BType_SendImGUI = 2;


/// <summary>
/// This function render overlay and all cheat modules.
/// Must be called after ImGui::NewFrame and before ImGui::EndFrame.
/// </summary>
void Render();

/// <summary>
/// This function is callback to KeyUp event.
/// </summary>
/// <returns> BType flags. </returns> 
short OnKeyUp(DWORD key);

/// <summary>
/// Add cheat module to manager.
/// </summary>
/// <param name="cheatModule">Shared pointer to ICheatModule. To create it use std::make_shared</param>
void AddModule(std::shared_ptr<ICheatModule> cheatModule);