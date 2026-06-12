/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TankWarlockStrategy.h"
#include "CreateNextAction.h"
#include "WarlockActions.h"

TankWarlockStrategy::TankWarlockStrategy(PlayerbotAI* botAI) : GenericWarlockStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

std::vector<NextAction> TankWarlockStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastShadowWardAction>(27.5f),
        CreateNextAction<CastSearingPainAction>(27.0f)
    };
}

void TankWarlockStrategy::InitTriggers(std::vector<TriggerNode*>&)
{
}
