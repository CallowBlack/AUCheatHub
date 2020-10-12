#include "pch-il2cpp.h"
#include "PlayersModule.h"
#include "il2cpp-appdata.h"
#include "ImGUI/imgui.h"
#include "utils.h"

#include <string>
#include <sstream>
#include <vector>
#include <codecvt> 
#include "detours.h"

using namespace app;
static AmongUsClient__StaticFields* clientStatic = nullptr;
static const ImVec4 IMPOSTER_COLOR = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
static const ImVec4 CHOSEN_COLOR = ImVec4(0.1f, 1.0f, 0.1f, 1.0f);
static const ImVec4 HOST_COLOR = ImVec4(0.1f, 0.1f, 1.0f, 1.0f);
static const ImVec4 GHOST_COLOR = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

static std::shared_ptr<uint32_t> localInetId = std::make_shared<uint32_t>();

void __cdecl OnVoteKick(BanMenu* menu, bool JIBLHOACKOC, MethodInfo* method) {
    auto selected = menu->fields.selected;
    auto staticClient = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    auto local_client = staticClient->Instance->fields._.ClientId;
    auto allClients = staticClient->Instance->fields._.allClients;
    for (int i = 1; i < allClients->fields._size; i++) {
        staticClient->Instance->fields._.ClientId = allClients->fields._items->vector[i]->fields.Id;
        menu->fields.selected = selected;
        BanMenu_Kick(menu, JIBLHOACKOC, method);
    }
    staticClient->Instance->fields._.ClientId = local_client;
}

void __cdecl PlayerCreate(PlayerControl* player, MethodInfo* method) {
    auto staticClient = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    if (player->fields._.OwnerId == staticClient->Instance->fields._.ClientId) {
        *localInetId = player->fields._.NetId;
    }
    PlayerControl_Start(player, method);
}

PlayersModule::PlayersModule()
{
    clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)PlayerControl_Start, (PBYTE)PlayerCreate);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)BanMenu_Kick, (PBYTE)OnVoteKick);
    DetourTransactionCommit();
}

void PlayersModule::OnRender()
{
	auto auClient = clientStatic->Instance;
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Players")) {
        if (IsGameStarted()) {
            auto clients = auClient->fields._.allClients;
            auto localClientId = auClient->fields._.ClientId;
            auto localPlayer = GetLocalPlayer();
            ImGui::Columns(4, "Players");
            ImGui::SetColumnWidth(0, 30);
            ImGui::Text("ID"); ImGui::NextColumn();
            ImGui::Text("Name"); ImGui::NextColumn();
            ImGui::Text("Status"); ImGui::NextColumn();
            ImGui::Text("Actions"); ImGui::NextColumn();
            ImGui::Separator();
            for (int i = 0; i < clients->fields._size; i++) {
                auto remoteClient = clients->fields._items->vector[i];
                auto playerControl = remoteClient->fields.Character;
                auto clientId = remoteClient->fields.Id;
                bool selected = false;
                if (remoteClient->fields.InScene) {
                    // Player id
                    {
                        ImGui::Text("%u", playerControl->fields.PlayerId);
                        ImGui::NextColumn();
                    }

                    // Player name
                    {
                        std::u16string source((char16_t*)&playerControl->fields.FMDMBBNEAHH->fields.EKHEPECKPKK->fields.m_firstChar);
                        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
                        ImGui::Text(convert.to_bytes(source).c_str());
                        ImGui::NextColumn();
                    }

                    // Player status
                    {
                        if (i == 0) {
                            ImGui::Text("Local"); ImGui::SameLine();
                        }

                        if (auClient->fields._.HostId == remoteClient->fields.Id)
                        {
                            ImGui::TextColored(IMPOSTER_COLOR, "Host"); ImGui::SameLine();
                        }

                        if (playerControl->fields.FMDMBBNEAHH->fields.LODLBBJNGKB)
                        {
                            ImGui::TextColored(IMPOSTER_COLOR, "Imposter"); ImGui::SameLine();
                        }

                        if (playerControl->fields.FMDMBBNEAHH->fields.DMFDFKEJHLH)
                        {
                            ImGui::TextColored(GHOST_COLOR, "Ghost"); ImGui::SameLine();
                        }

                        if ((playerControl->fields._.NetId == localPlayer->fields._.NetId && i != 0) || 
                            (i == 0 && *localInetId == playerControl->fields._.NetId))
                        {
                            ImGui::TextColored(CHOSEN_COLOR, "Chosen"); ImGui::SameLine();
                            selected = true;
                        }
                        ImGui::Text("");
                        ImGui::NextColumn();
                    }

                    // Interaction buttons
                    {
                        ImGui::PushID(playerControl->fields.PlayerId);
                        if (selected)
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

                        if (ImGui::Button("Sel") && !selected) {
                            
                            localPlayer->fields._.NetId = (i == 0) ? *localInetId : playerControl->fields._.NetId;
                        }

                        ImGui::SameLine();
                        if (selected)
                            ImGui::PopStyleVar();


                        if (i == 0)
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

                        if (ImGui::Button("Kick") && i != 0) {

                        }

                        if (i == 0)
                            ImGui::PopStyleVar();
                        ImGui::PopID();
                    }


                    ImGui::NextColumn();
                }
            }
        }
        else {
            ImGui::Text("Players list empty. \nYou must start game to activate it.");
        }
    }


}

char* PlayersModule::GetName()
{
	return (char*)"Player List";
}
