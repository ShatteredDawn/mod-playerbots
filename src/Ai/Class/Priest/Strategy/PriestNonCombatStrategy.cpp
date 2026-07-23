/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "PriestNonCombatStrategy.h"

#include "CreateNextAction.h"
#include "GenericActions.h"
#include "ImbueAction.h"
#include "PriestActions.h"
#include "PriestNonCombatStrategyActionNodeFactory.h"
#include "priest/RefillPriestReagentsAction.h"

PriestNonCombatStrategy::PriestNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    actionNodeFactories.Add(new PriestNonCombatStrategyActionNodeFactory());
}

void PriestNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "inner fire",
            {
                CreateNextAction<CastInnerFireAction>(10.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "vampiric embrace",
            {
                CreateNextAction<CastVampiricEmbraceAction>(16.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member dead",
            {
                CreateNextAction<CastRemoveShadowformAction>(ACTION_CRITICAL_HEAL + 11.0f),
                CreateNextAction<CastResurrectionAction>(ACTION_CRITICAL_HEAL + 10.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "often",
            {
                CreateNextAction<ImbueWithOilAction>(1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastRenewOnPartyAction>(ACTION_CRITICAL_HEAL + 3.0f),
                CreateNextAction<CastPenanceOnPartyAction>(ACTION_CRITICAL_HEAL + 2.0f),
                CreateNextAction<CastGreaterHealOnPartyAction>(ACTION_CRITICAL_HEAL + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member low health",
            {
                CreateNextAction<CastRenewOnPartyAction>(ACTION_MEDIUM_HEAL + 3.0f),
                CreateNextAction<CastPenanceOnPartyAction>(ACTION_MEDIUM_HEAL + 2.0f),
                CreateNextAction<CastGreaterHealOnPartyAction>(ACTION_MEDIUM_HEAL + 1.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member medium health",
            {
                CreateNextAction<CastRenewOnPartyAction>(ACTION_LIGHT_HEAL + 9.0f),
                CreateNextAction<CastPenanceOnPartyAction>(ACTION_LIGHT_HEAL + 8.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member almost full health",
            {
                CreateNextAction<CastRenewOnPartyAction>(ACTION_LIGHT_HEAL + 3.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "group heal setting",
            {
                CreateNextAction<CastCircleOfHealingAction>(27.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "new pet",
            {
                CreateNextAction<SetPetStanceAction>(10.0f)
            }
        )
    );
    triggers.emplace_back(
        new TriggerNode(
            "seldom",
            {
                CreateNextAction<RefillPriestReagentsAction>(ACTION_HIGH)
            }
        )
    );

}

void PriestBuffStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "power word: fortitude on party",
            {
                CreateNextAction<CastPowerWordFortitudeOnPartyAction>(11.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "divine spirit on party",
            {
                CreateNextAction<CastDivineSpiritOnPartyAction>(13.0f)
            }
        )
    );
}

void PriestShadowResistanceStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "shadow protection",
            {
                CreateNextAction<CastShadowProtectionAction>(12.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "shadow protection on party",
            {
                CreateNextAction<CastShadowProtectionOnPartyAction>(11.0f)
            }
        )
    );
}
