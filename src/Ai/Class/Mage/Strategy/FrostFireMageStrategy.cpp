/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "FrostFireMageStrategy.h"
#include "CreateNextAction.h"
#include "MageActions.h"

FrostFireMageStrategy::FrostFireMageStrategy(PlayerbotAI* botAI) : GenericMageStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> FrostFireMageStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastFrostfireBoltAction>(5.2f),
        CreateNextAction<CastFireBlastAction>(5.1f),  // cast during movement
        CreateNextAction<CastShootAction>(5.0f)
    };
}

// ===== Trigger Initialization =====
void FrostFireMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMageStrategy::InitTriggers(triggers);

    // Debuff Triggers
    triggers.push_back(
        new TriggerNode(
            "improved scorch",
            {
                CreateNextAction<CastScorchAction>(19.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "living bomb",
            {
                CreateNextAction<CastLivingBombAction>(18.5f)
            }
        )
    );

    // Proc Trigger
    triggers.push_back(
        new TriggerNode(
            "hot streak",
            {
                CreateNextAction<CastPyroblastAction>(25.0f)
            }
        )
    );
}
