#pragma once
#include "GUI/CheatModule/ICheatModule.h"
class DebugModule : public ICheatModule
{
	// Унаследовано через ICheatModule
public:
	void OnRender() override;
	char* GetName() override;
};

