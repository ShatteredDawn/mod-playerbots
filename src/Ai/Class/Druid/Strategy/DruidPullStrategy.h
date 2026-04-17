/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DRUID_PULL_STRATEGY_H
#define _PLAYERBOT_DRUID_PULL_STRATEGY_H

#include "CreateNextAction.h"
#include "DruidActions.h"
#include "DruidShapeshiftActions.h"
#include "PullStrategy.h"

class DruidPullStrategy : public PullStrategy
{
public:
    DruidPullStrategy(PlayerbotAI* botAI) : PullStrategy(botAI, "faerie fire", CreateNextAction<CastFaerieFireFeralAction>(1.0f).factory, "dire bear form", CreateNextAction<CastDireBearFormAction>(1.0f).factory) {}

    std::string GetPullActionName() const override;
    std::string GetPreActionName() const override;
};

#endif
