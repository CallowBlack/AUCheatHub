#include "pch-il2cpp.h"
#include "PlayersModule.h"
#include "il2cpp-appdata.h"
#include "ImGUI/imgui.h"

#include "utils.h"
#include "hack.h"

#include <string>
#include <sstream>
#include <vector>
#include <codecvt> 
#include "detours.h"

using namespace app;

struct LocalInfo {
    uint32_t netId;
    int32_t clientId;

    uint8_t currentPlayerId;

    int32_t clientIdToKick;

    int32_t clientIdToKill;

    bool vote;
    uint8_t playerIdToVote;
    
    bool voteAll;
    uint8_t playerIdToAllVote;
};

static AmongUsClient__StaticFields* clientStatic = nullptr;
static const ImVec4 IMPOSTER_COLOR = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
static const ImVec4 CHOSEN_COLOR = ImVec4(0.1f, 1.0f, 0.1f, 1.0f);
static const ImVec4 HOST_COLOR = ImVec4(0.1f, 0.1f, 1.0f, 1.0f);
static const ImVec4 GHOST_COLOR = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

static Color32__Array* playerColors;
static MeetingHud__StaticFields* meetingHudStatic = nullptr;
static std::shared_ptr<LocalInfo> localeInfo = std::make_shared<LocalInfo>(LocalInfo{0});

void __cdecl PlayerCreate(PlayerControl* player, MethodInfo* method) {
    auto staticClient = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    if (player->fields._.OwnerId == staticClient->Instance->fields._.ClientId) {
        localeInfo->netId = player->fields._.NetId;
        localeInfo->clientId = staticClient->Instance->fields._.ClientId;
        localeInfo->currentPlayerId = player->fields.PlayerId;
    }
    PlayerControl_Start(player, method);
}

int32_t __cdecl GetPing(InnerNetClient* client, MethodInfo* method) {
    // Kick player while ping is requested
    if (localeInfo->clientIdToKick != 0){
        KickPlayer(localeInfo->clientIdToKick);
        localeInfo->clientIdToKick = 0;
    }

    if (localeInfo->clientIdToKill != 0) {
        auto localPlayer = GetLocalPlayer();
        auto tempNetId = localPlayer->fields._.NetId;
        localPlayer->fields._.NetId = localeInfo->netId;

        KillPlayer(localeInfo->clientIdToKill);

        localPlayer->fields._.NetId = tempNetId;
        localeInfo->clientIdToKill = 0;
    }

    if (localeInfo->vote) {
        VoteByPlayer(localeInfo->currentPlayerId, localeInfo->playerIdToVote);
        localeInfo->vote = false;
    }

    return InnerNetClient_get_Ping(client, method);
}

PlayersModule::PlayersModule()
{
    playerColors = reinterpret_cast<KMGFBENDNFO__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*KMGFBENDNFO__TypeInfo))->FOJPMGJFKMB;

    clientStatic = reinterpret_cast<AmongUsClient__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*AmongUsClient__TypeInfo));
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)PlayerControl_Start, (PBYTE)PlayerCreate);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)InnerNetClient_get_Ping, (PBYTE)GetPing);
    DetourTransactionCommit();
}

void PlayersModule::OnRender()
{
	auto gameClient = clientStatic->Instance;
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Players")) {
        if (IsGameStarted()) {
            if (meetingHudStatic == nullptr)
                meetingHudStatic = reinterpret_cast<MeetingHud__StaticFields*>(il2cpp_class_get_static_field_data((Il2CppClass*)*MeetingHud__TypeInfo));
            auto clients = gameClient->fields._.allClients;
            auto pcLocal = GetLocalPlayer();
            bool isMeeting = meetingHudStatic->Instance != nullptr && Object_1_op_Implicit((Object_1 *)meetingHudStatic->Instance, 0);

            bool playerVoted = false;
            if (isMeeting) {
                auto playerVotes = meetingHudStatic->Instance->fields.FALDLDJHDDJ;
                for (int i = 0; i < playerVotes->max_length; i++) {
                    auto playerVote = playerVotes->vector[i];
                    if (playerVote->fields.TargetPlayerId == localeInfo->currentPlayerId)
                        playerVoted = playerVote->fields.didVote;
                }
            }

            ImGui::Columns(4, "Players");
            ImGui::SetColumnWidth(0, 30);
            ImGui::SetColumnWidth(1, 120);
            ImGui::SetColumnWidth(2, 400);
            ImGui::SetColumnWidth(3, 320);
            ImGui::Text("ID"); ImGui::NextColumn();
            ImGui::Text("Name"); ImGui::NextColumn();
            ImGui::Text("Status"); ImGui::NextColumn();
            ImGui::Text("Actions"); ImGui::NextColumn();

            ImGui::Separator();

            for (int i = 0; i < clients->fields._size; i++) {
                auto cRemote = clients->fields._items->vector[i];
                auto pcRemote = cRemote->fields.Character;
                auto pDataRemote = pcRemote->fields.FMDMBBNEAHH;

                if (cRemote->fields.InScene) {

                    bool isLocal =    localeInfo->clientId == cRemote->fields.Id;
                    bool isHost =     gameClient->fields._.HostId == cRemote->fields.Id;

                    bool isImposter = pcRemote->fields.FMDMBBNEAHH->fields.LODLBBJNGKB;
                    bool isGhost =    pcRemote->fields.FMDMBBNEAHH->fields.DMFDFKEJHLH;

                    bool isSelected = (pcRemote->fields._.NetId == pcLocal->fields._.NetId && !isLocal) || 
                                      (isLocal && localeInfo->netId == pcRemote->fields._.NetId);

                    // Player id
                    {
                        ImGui::Text("%u", pcRemote->fields.PlayerId);
                        ImGui::NextColumn();
                    }

                    // Player name
                    {
                        std::u16string source((char16_t*)&pcRemote->fields.FMDMBBNEAHH->fields.EKHEPECKPKK->fields.m_firstChar);
                        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
                        ImGui::Text(convert.to_bytes(source).c_str());
                        ImGui::NextColumn();
                    }

                    // Player status
                    {
                        if (isLocal) {
                            ImGui::Text("Local"); ImGui::SameLine();
                        }

                        if (isHost)
                        {
                            ImGui::TextColored(IMPOSTER_COLOR, "Host"); ImGui::SameLine();
                        }

                        if (isImposter)
                        {
                            ImGui::TextColored(IMPOSTER_COLOR, "Imposter"); ImGui::SameLine();
                        }

                        if (isGhost)
                        {
                            ImGui::TextColored(GHOST_COLOR, "Ghost"); ImGui::SameLine();
                        }

                        if (isSelected)
                        {
                            ImGui::TextColored(CHOSEN_COLOR, "Selected"); ImGui::SameLine();
                        }
                        ImGui::Text("");

                        if (isMeeting) {
                            auto playerVotes = meetingHudStatic->Instance->fields.FALDLDJHDDJ;
                            bool hasVoted = false;
                            for (int i = 0; i < playerVotes->max_length; i++) {
                                auto playerVote = playerVotes->vector[i];
                                if (playerVote->fields.didVote && !playerVote->fields.isDead && playerVote->fields.votedFor == pcRemote->fields.PlayerId)
                                {
                                    if (!hasVoted) {
                                        ImGui::Text("Voted by:");
                                        ImGui::SameLine();
                                        hasVoted = true;
                                    }
                                    auto clientData = GetPlayerClientById(playerVote->fields.TargetPlayerId);
                                    if (clientData) {
                                        ImGui::Text("%u ", clientData->fields.Character->fields.PlayerId);
                                        ImGui::SameLine();
                                    }
                                }
                            }
                            if (hasVoted)
                                ImGui::Text("");
                        }

                        ImGui::NextColumn();
                    }

                    // Interaction buttons
                    {
                        ImGui::PushID(pcRemote->fields.PlayerId);
                        if (EnabledButton(!isSelected, "Select", ImVec2(70, 22))) {
                            pcLocal->fields._.NetId = isLocal ? localeInfo->netId : pcRemote->fields._.NetId;
                            localeInfo->currentPlayerId = pcRemote->fields.PlayerId;
                        }
                            
                        ImGui::SameLine();

                        if (!isMeeting) {
                            if (ImGui::Button("TpTo", ImVec2(70, 22))) {
                                SnapToPlayer(pcLocal, pcRemote);
                            }
                            ImGui::SameLine();

                            if (ImGui::Button("TpFrom", ImVec2(70, 22))) {
                                SnapToPlayer(pcRemote, pcLocal);
                            }
                            ImGui::SameLine();

                            if (EnabledButton(!isGhost, "Kill", ImVec2(70, 22))) {
                                localeInfo->clientIdToKill = pcRemote->fields.PlayerId;
                            }
                        }
                        else {
                            if (EnabledButton(!isLocal, "Kick", ImVec2(70, 22)))
                                localeInfo->clientIdToKick = cRemote->fields.Id;
                            ImGui::SameLine();

                            if (EnabledButton(!isGhost && !playerVoted, "Vote", ImVec2(70, 22))) {
                                localeInfo->vote = true;
                                localeInfo->playerIdToVote = pcRemote->fields.PlayerId;
                            }
                            ImGui::SameLine();

                            if (EnabledButton(!isGhost, "Vote All", ImVec2(70, 22))) {

                            }
                        }
                        ImGui::PopID();
                    }

                    ImGui::NextColumn();
                    ImGui::Separator();
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

