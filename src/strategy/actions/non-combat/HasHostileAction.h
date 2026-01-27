/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <sstream>

#include "Action.h"
#include "GlobalPlayerInspector.h"

class PlayerbotAI;

class HasHostileAction : public Action
{
public:
    HasHostileAction(PlayerbotAI* botAI) : Action(botAI, "has_hostile") {}

    bool Execute(Event event) override
    {
        const GlobalPlayerInspector inspector(this->bot->GetGUID().GetRawValue());

        const bool hasHostile = inspector.hasHostileUnitsInRange(40.0f);

        const Player* const master = this->botAI->GetMaster();

        if  (master == nullptr)
            return true;

        std::ostringstream message;

        message << "I have hostile: ";
        message << std::to_string(hasHostile);

        this->botAI->TellMaster(message);

        return true;
    }
};
