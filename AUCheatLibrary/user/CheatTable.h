#pragma once
#include <Windows.h>
#include <map> 

using namespace std;
class CheatTable
{
private:
	map<DWORD, void*> mKeyFunc{};
	map<DWORD, bool> mKeyIsStateExist{};
	map<DWORD, bool> mKeyState{};
public:
	
	struct CheatInfo {
		DWORD VKey;
		const char* name;
		void* function;
		bool isState;
	};

	CheatTable();
	CheatTable(initializer_list<CheatInfo> cheatList);

	void RunCheat(DWORD vk);
	void AddCheat(CheatInfo cheatInfo);
};

