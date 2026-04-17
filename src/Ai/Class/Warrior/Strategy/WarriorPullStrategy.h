/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_WARRIOR_PULL_STRATEGY_H
#define _PLAYERBOT_WARRIOR_PULL_STRATEGY_H

#include "CreateNextAction.h"
#include "GenericSpellActions.h"
#include "PullStrategy.h"

class WarriorPullStrategy : public PullStrategy
{
public:
    WarriorPullStrategy(PlayerbotAI* botAI) : PullStrategy(botAI, "shoot", CreateNextAction<CastShootAction>(1.0f).factory) {}

    std::string GetPullActionName() const override;
};

#endif
