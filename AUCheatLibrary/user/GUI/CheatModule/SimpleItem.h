#pragma once
#include "ICheatModule.h"

class SimpleItem : public ICheatModule {
public:
	typedef bool (*isEnabledFunc)();
	SimpleItem(char* name, void* func, isEnabledFunc checkFunc, bool hasState);
	void OnRender() override;
	char* GetName() override;

private:
	typedef void (*stateFunc)(bool);
	typedef void (*nostateFunc)();

	void* cheatFunc;
	char* name;
	isEnabledFunc checkEnabledFunc;
	bool state = false;
	bool hasState = false;

};