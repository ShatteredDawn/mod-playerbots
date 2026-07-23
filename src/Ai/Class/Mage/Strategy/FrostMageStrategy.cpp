/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "FrostMageStrategy.h"
#include "GenericActions.h"
#include "MageActions.h"

FrostMageStrategy::FrostMageStrategy(PlayerbotAI* botAI) : GenericMageStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Default Actions =====
std::vector<NextAction> FrostMageStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastFrostboltAction>(5.4f),
        CreateNextAction<CastIceLanceAction>(5.3f),   // cast during movement
        CreateNextAction<CastFireBlastAction>(5.2f),  // cast during movement if ice lance is not learned
        CreateNextAction<CastShootAction>(5.1f),
        CreateNextAction<CastFireballAction>(5.0f)
    };
}

// ===== Trigger Initialization ===
void FrostMageStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericMageStrategy::InitTriggers(triggers);

    // Pet/Defensive triggers
    triggers.push_back(
        new TriggerNode(
            "no pet",
            {
                CreateNextAction<CastSummonWaterElementalAction>(30.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "has pet",
            {
                CreateNextAction<TogglePetSpellAutoCastAction>(60.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "new pet",
            {
                CreateNextAction<SetPetStanceAction>(60.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "medium health",
            {
                CreateNextAction<CastIceBarrierAction>(29.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "being attacked",
            {
                CreateNextAction<CastIceBarrierAction>(29.0f)
            }
        )
    );

    // Proc/Freeze triggers
    triggers.push_back(
        new TriggerNode(
            "brain freeze",
            {
                CreateNextAction<CastFrostfireBoltAction>(19.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "fingers of frost",
            {
                CreateNextAction<CastDeepFreezeAction>(19.0f),
                CreateNextAction<CastFrostboltAction>(18.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "frostbite on target",
            {
                CreateNextAction<CastDeepFreezeAction>(19.0f),
                CreateNextAction<CastFrostboltAction>(18.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "frost nova on target",
            {
                CreateNextAction<CastDeepFreezeAction>(19.0f),
                CreateNextAction<CastFrostboltAction>(18.0f)
            }
        )
    );
}
