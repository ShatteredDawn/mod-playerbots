/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "MarksmanshipHunterStrategy.h"
#include "CreateNextAction.h"
#include "HunterActions.h"

MarksmanshipHunterStrategy::MarksmanshipHunterStrategy(PlayerbotAI* botAI) : GenericHunterStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> MarksmanshipHunterStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastKillCommandAction>(5.8f),
        CreateNextAction<CastKillShotAction>(5.7f),
        CreateNextAction<CastSerpentStingAction>(5.6f),
        CreateNextAction<CastChimeraShotAction>(5.5f),
        CreateNextAction<CastAimedShotAction>(5.4f),
        CreateNextAction<CastArcaneShotAction>(5.3f),
        CreateNextAction<CastSteadyShotAction>(5.2f),
        CreateNextAction<CastAutoShotAction>(5.1f)
    };
}

// ===== Trigger Initialization ===
void MarksmanshipHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericHunterStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "silencing shot",
            {
                CreateNextAction<CastSilencingShotAction>(40.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "kill command",
            {
                CreateNextAction<CastKillCommandAction>(18.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "target critical health",
            {
                CreateNextAction<CastKillShotAction>(18.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low mana",
            {
                CreateNextAction<CastViperStingAction>(17.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "no stings",
            {
                CreateNextAction<CastSerpentStingAction>(17.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "serpent sting on attacker",
            {
                CreateNextAction<CastSerpentStingOnAttackerAction>(16.5f)
            }
        )
    );
}
