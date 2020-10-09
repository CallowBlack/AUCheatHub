#include "pch-il2cpp.h"
#include "CheatTable.h"

CheatTable::CheatTable() { }

CheatTable::CheatTable(initializer_list<CheatInfo> cheatList)
{
	printf("Cheat keyboard key list:\n");
	for (auto pCInfo = cheatList.begin(); pCInfo < cheatList.end(); pCInfo++) {
		printf("[%c] %s\n", (char)pCInfo->VKey, pCInfo->name);
		AddCheat(*pCInfo);
	}
}

void CheatTable::RunCheat(DWORD vk)
{
	if (mKeyFunc.find(vk) != mKeyFunc.end()) {
		if (mKeyIsStateExist[vk]) {
			mKeyState[vk] = !mKeyState[vk];
			((void(*)(bool))mKeyFunc[vk])(mKeyState[vk]);
		}
		else
			((void(*)())mKeyFunc[vk])();
	}
}

void CheatTable::AddCheat(CheatInfo cheatInfo)
{
	if (mKeyFunc.find(cheatInfo.VKey) == mKeyFunc.end()) {
		mKeyFunc.insert(pair<DWORD, void*>(cheatInfo.VKey, cheatInfo.function));
		mKeyIsStateExist.insert(pair<DWORD, bool>(cheatInfo.VKey, cheatInfo.isState));
		mKeyState.insert(pair<DWORD, bool>(cheatInfo.VKey, false));
	}
}
