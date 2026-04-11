/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericHunterNonCombatStrategy.h"
#include "EquipAction.h"
#include "GenericActions.h"
#include "HunterActions.h"
#include "ImbueAction.h"

GenericHunterNonCombatStrategy::GenericHunterNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

void GenericHunterNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "trueshot aura",
            {
                CreateNextAction<CastTrueshotAuraAction>(2.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "often",
            {
                CreateNextAction<ImbueWithStoneAction>(1.0f),
                CreateNextAction<ImbueWithOilAction>(1.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low ammo",
            {
                CreateNextAction<SayLowAmmoAction>(ACTION_NORMAL)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "no track",
            {
                CreateNextAction<CastTrackHumanoidsAction>(ACTION_NORMAL)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "no ammo",
            {
                CreateNextAction<EquipUpgradesPacketAction>(ACTION_HIGH + 1.0f)
            }
        )
    );
}

void HunterPetStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "no pet",
            {
                CreateNextAction<CastCallPetAction>(60.0f)
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
            "pet not happy",
            {
                CreateNextAction<FeedPetAction>(60.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "hunters pet medium health",
            {
                CreateNextAction<CastMendPetAction>(60.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "hunters pet dead",
            {
                CreateNextAction<CastRevivePetAction>(60.0f)
            }
        )
    );
}
