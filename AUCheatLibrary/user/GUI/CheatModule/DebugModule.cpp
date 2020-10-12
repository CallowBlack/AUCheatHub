#include "pch-il2cpp.h"
#include "DebugModule.h"
#include "ImGUI/imgui.h"

#include "utils.h"
#include <stdio.h>
#include <cstdarg>
#include <iostream>
#include "il2cpp-appdata.h"

#include <string>
#include <sstream>

using namespace ImGui;


void RenderLine(const char* name, std::string value) {
    PushID(name);
    ImGui::Text(name); SameLine(150);
    ImGui::Text(value.c_str()); SameLine(300);
    if (ImGui::Button("Copy"))
        SetClipboardText(value.c_str());
    PopID();
}

void DebugModule::OnRender()
{
    std::stringstream stream("");
    std::string temp;

    stream << GetGameState(); stream >> temp;
    RenderLine("Game state", temp); stream.clear();

    auto clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    stream << clientStatic; stream >> temp;
    RenderLine("AmongUsClient ", temp); stream.clear();

    auto playerStatic = reinterpret_cast<PlayerControl__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*PlayerControl__TypeInfo));
    stream << playerStatic; stream >> temp;
    RenderLine("PlayerControl ", temp); stream.clear();
}

char* DebugModule::GetName()
{
    return nullptr;
}
