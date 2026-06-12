/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "DestructionWarlockStrategy.h"
#include "CreateNextAction.h"
#include "MovementActions.h"
#include "WarlockActions.h"

DestructionWarlockStrategy::DestructionWarlockStrategy(PlayerbotAI* botAI) : GenericWarlockStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> DestructionWarlockStrategy::getDefaultActions()
{
    return {
       CreateNextAction<CastImmolateAction>(5.9f),
       CreateNextAction<CastConflagrateAction>(5.8f),
       CreateNextAction<CastChaosBoltAction>(5.7f),
       CreateNextAction<CastIncinerateAction>(5.6f),
       CreateNextAction<CastCorruptionAction>(5.3f),      // Note: Corruption and Shadow Bolt won't be used after the character learns Incinerate at level 64
       CreateNextAction<CastShadowBoltAction>(5.2f),
       CreateNextAction<CastShootAction>(5.0f)
    };
}

// ===== Trigger Initialization ===
void DestructionWarlockStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericWarlockStrategy::InitTriggers(triggers);

    // Main DoT triggers for high uptime + high priority cooldowns
    triggers.push_back(
        new TriggerNode(
            "immolate",
            {
                CreateNextAction<CastImmolateAction>(20.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "conflagrate",
            {
                CreateNextAction<CastConflagrateAction>(19.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "chaos bolt",
            {
                CreateNextAction<CastChaosBoltAction>(19.0f)
            }
        )
    );

    // Note: Corruption won't be used after the character learns Incinerate at level 64
    triggers.push_back(
        new TriggerNode(
            "corruption on attacker",
            {
                CreateNextAction<CastCorruptionOnAttackerAction>(5.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "corruption",
            {
                CreateNextAction<CastCorruptionAction>(5.4f)
            }
        )
    );

    // Shadowburn as execute if target is low HP
    triggers.push_back(
        new TriggerNode(
            "target critical health",
            {
                CreateNextAction<CastShadowburnAction>(18.0f)
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
