/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PALADIN_PULL_STRATEGY_H
#define _PLAYERBOT_PALADIN_PULL_STRATEGY_H

#include "CreateNextAction.h"
#include "PaladinActions.h"
#include "PullStrategy.h"

class PaladinPullStrategy : public PullStrategy
{
public:
    PaladinPullStrategy(PlayerbotAI* botAI)
    : PullStrategy(
        botAI,
        "judgement",
        CreateNextAction<CastJudgementAction>(1.0f).factory,
        "seal of righteousness",
        CreateNextAction<CastSealOfRighteousnessAction>(1.0f).factory
    ) {}

    std::string GetPullActionName() const override;
    std::string GetPreActionName() const override;
};

#endif
