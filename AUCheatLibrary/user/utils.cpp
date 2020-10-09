#include "pch-il2cpp.h"
#include "utils.h"

PlayerControl__StaticFields* playerStatic;

bool IsLocalPlayerExist()
{
	return GetLocalPlayer() != nullptr;
}

PlayerControl* GetLocalPlayer()
{
    if (playerStatic == nullptr)
        playerStatic = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
	return playerStatic->LocalPlayer;
}

GameData_CBOMPDNBEIF* FindTask(GameData_IHEKEPMDGIJ* playerInfo, uint32_t id) {
    auto tasks = playerInfo->fields.IHACFCJPFCF;
    for (int i = 0; i < tasks->fields._size; i++) {
        auto taskInfo = tasks->fields._items->vector[i];
        auto taskId = taskInfo->fields.AKLEDCMKHMC;
        if (taskId == id)
            return taskInfo;
    }
    return NULL;
}