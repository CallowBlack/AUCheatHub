#pragma once

void SetUnlimitSabbotage(bool state);
void SetGhostState(bool state);
void SetInfectedState(bool state);
void CompleteAllTasks();

void GetAllPlayersNetID();
void SetKillNoReload(bool state);

void KickPlayer(int32_t clientId);
void VoteByPlayer(uint8_t voterPlayerId, int8_t targetPlayerId);
void KillPlayer(uint8_t playerId);