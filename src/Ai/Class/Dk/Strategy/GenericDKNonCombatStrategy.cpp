/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericDKNonCombatStrategy.h"
#include "CreateNextAction.h"
#include "DKActions.h"
#include "GenericActions.h"
#include "Strategy.h"
#include "Trigger.h"
#include "death-knight/RefillDeathKnightReagentsAction.h"
#include "ImbueAction.h"

GenericDKNonCombatStrategy::GenericDKNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

void GenericDKNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "often",
            {
                CreateNextAction<ImbueWithStoneAction>(1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "horn of winter",
            {
                CreateNextAction<CastHornOfWinterAction>(21.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "bone shield",
            {
                CreateNextAction<CastBoneShieldAction>(21.0f)
            }
        )
    );
    triggers.emplace_back(
        new TriggerNode(
            "seldom",
            {
                CreateNextAction<RefillDeathKnightReagentsAction>(ACTION_HIGH)
            }
        )
    );
}
