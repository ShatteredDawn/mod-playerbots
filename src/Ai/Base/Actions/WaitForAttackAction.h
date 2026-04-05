/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include "MovementActions.h"

class PlayerbotAI;

class WaitForAttackKeepSafeDistanceAction : public MovementAction
{
public:
    WaitForAttackKeepSafeDistanceAction(PlayerbotAI* botAI)
        : MovementAction(botAI, "wait for attack keep safe distance") {}

    bool Execute(Event event) override;

protected:
    [[nodiscard]] WorldPosition GetBestPoint(AiObjectContext& context, Player& bot, Unit& target, float minDistance, float maxDistance) const;
};

class SetWaitForAttackTimeAction : public Action
{
public:
    SetWaitForAttackTimeAction(PlayerbotAI* botAI)
        : Action(botAI, "wait for attack time") {}

    bool Execute(Event event) override;
};
