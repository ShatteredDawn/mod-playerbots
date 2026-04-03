/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ElementalShamanStrategy.h"
#include "CreateNextAction.h"
#include "ShamanActions.h"

ElementalShamanStrategy::ElementalShamanStrategy(PlayerbotAI* botAI) : GenericShamanStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> ElementalShamanStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastLavaBurstAction>(5.2f),
        CreateNextAction<CastLightningBoltAction>(5.0f)
    };
}

// ===== Trigger Initialization ===
void ElementalShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericShamanStrategy::InitTriggers(triggers);

    // Totem Triggers
    triggers.push_back(
        new TriggerNode(
            "call of the elements",
            {
                CreateNextAction<CastCallOfTheElementsAction>(60.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low health",
            {
                CreateNextAction<CastStoneclawTotemAction>(40.0f)
            }
        )
    );

    // Cooldown Trigger
    triggers.push_back(
        new TriggerNode(
            "elemental mastery",
            {
                CreateNextAction<CastElementalMasteryAction>(29.0f)
            }
        )
    );

    // Damage Triggers
    triggers.push_back(
        new TriggerNode(
            "earth shock execute",
            {
                CreateNextAction<CastEarthShockAction>(5.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "flame shock",
            {
                CreateNextAction<CastFlameShockAction>(5.3f)
            }
        )
    );

    // Mana Triggers
    triggers.push_back(
        new TriggerNode(
            "water shield",
            {
                CreateNextAction<CastWaterShieldAction>(19.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "high mana",
            {
                CreateNextAction<CastThunderstormAction>(19.0f)
            }
        )
    );

    // Range Triggers
    triggers.push_back(
        new TriggerNode(
            "enemy is close",
            {
                CreateNextAction<CastThunderstormAction>(19.0f)
            }
        )
    );
}
