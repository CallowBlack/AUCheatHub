#pragma once
#include <wchar.h>

class ICheatModule {
public:
	void virtual OnRender() = 0;
	virtual char* GetName() = 0;
};