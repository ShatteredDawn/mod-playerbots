/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "AfflictionWarlockStrategy.h"
#include "CreateNextAction.h"
#include "MovementActions.h"
#include "WarlockActions.h"

AfflictionWarlockStrategy::AfflictionWarlockStrategy(PlayerbotAI* botAI) : GenericWarlockStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> AfflictionWarlockStrategy::getDefaultActions()
{
    return {
       CreateNextAction<CastCorruptionAction>(5.5f),
       CreateNextAction<CastUnstableAfflictionAction>(5.4f),
       CreateNextAction<CastHauntAction>(5.3f),
       CreateNextAction<CastShadowBoltAction>(5.2f),
       CreateNextAction<CastShootAction>(5.0f)
    };
}

// ===== Trigger Initialization ===
void AfflictionWarlockStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericWarlockStrategy::InitTriggers(triggers);

    // Main DoT triggers for high uptime
    triggers.push_back(
        new TriggerNode(
            "corruption on attacker",
            {
                CreateNextAction<CastCorruptionOnAttackerAction>(19.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "unstable affliction on attacker",
            {
                CreateNextAction<CastUnstableAfflictionOnAttackerAction>(19.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "corruption",
            {
                CreateNextAction<CastCorruptionAction>(18.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "unstable affliction",
            {
                CreateNextAction<CastUnstableAfflictionAction>(17.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "haunt",
            {
                CreateNextAction<CastHauntAction>(16.5f)
            }
        )
    );

    // Drain Soul as execute if target is low HP // Shadow Trance for free casts
    triggers.push_back(
        new TriggerNode(
            "shadow trance",
            {
                CreateNextAction<CastShadowBoltAction>(16.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "target critical health",
            {
                CreateNextAction<CastDrainSoulAction>(15.5f)
            }
        )
    );

    // Life Tap glyph buff, and Life Tap as filler
    triggers.push_back(
        new TriggerNode(
            "life tap glyph buff",
            {
                CreateNextAction<CastLifeTapAction>(29.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "life tap",
            {
                CreateNextAction<CastLifeTapAction>(5.1f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "enemy too close for spell",
            {
                CreateNextAction<FleeAction>(39.0f)
            }
        )
    );
}
