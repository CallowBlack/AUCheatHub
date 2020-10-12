#pragma once
#include "GUI/CheatModule/ICheatModule.h"
class PlayersModule :
    public ICheatModule
{
public:
    PlayersModule();
    virtual void OnRender() override;
    virtual char* GetName() override;
};

