/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ArcaneMageStrategy.h"
#include "CreateNextAction.h"
#include "MageActions.h"

ArcaneMageStrategy::ArcaneMageStrategy(PlayerbotAI* botAI) : GenericMageStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> ArcaneMageStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastArcaneBlastAction>(5.6f),
        CreateNextAction<CastArcaneMissilesAction>(5.5f),
        CreateNextAction<CastArcaneBarrageAction>(5.4f),   // cast while moving
        CreateNextAction<CastFireBlastAction>(5.3f),       // cast while moving if arcane barrage isn't available/learned
        CreateNextAction<CastFrostboltAction>(5.2f),        // for arcane immune targets
        CreateNextAction<CastShootAction>(5.1f)
    };
}

// ===== Trigger Initialization ===
void ArcaneMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMageStrategy::InitTriggers(triggers);

    // Proc Trigger
    triggers.push_back(
        new TriggerNode(
            "arcane blast 4 stacks and missile barrage",
            {
                CreateNextAction<CastArcaneMissilesAction>(15.0f)
            }
        )
    );
}
