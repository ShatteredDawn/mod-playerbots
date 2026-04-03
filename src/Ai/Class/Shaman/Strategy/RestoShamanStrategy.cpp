/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "RestoShamanStrategy.h"
#include "CreateNextAction.h"
#include "MovementActions.h"
#include "ReachTargetActions.h"
#include "ShamanActions.h"
#include "UseItemAction.h"

RestoShamanStrategy::RestoShamanStrategy(PlayerbotAI* botAI) : GenericShamanStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

// ===== Trigger Initialization ===
void RestoShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
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
    triggers.push_back(
        new TriggerNode(
            "medium mana",
            {
                CreateNextAction<CastManaTideTotemAction>(ACTION_HIGH + 5.0f)
            }
        )
    );

    // Healing Triggers
    triggers.push_back(
        new TriggerNode(
            "group heal setting",
            {
                CreateNextAction<CastRiptideOnPartyAction>(27.0f),
                CreateNextAction<CastChainHealAction>(26.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(25.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(24.0f),
                CreateNextAction<CastLesserHealingWaveOnPartyAction>(23.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member low health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(19.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(18.0f),
                CreateNextAction<CastLesserHealingWaveOnPartyAction>(17.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member medium health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(16.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(15.0f),
                CreateNextAction<CastLesserHealingWaveOnPartyAction>(14.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member almost full health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(12.0f),
                CreateNextAction<CastLesserHealingWaveOnPartyAction>(11.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "earth shield on main tank",
            {
                CreateNextAction<CastEarthShieldOnMainTankAction>(ACTION_HIGH + 7.0f)
            }
        )
    );

    // Dispel Triggers
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit poison",
            {
                CreateNextAction<CastCleanseSpiritPoisonOnPartyAction>(ACTION_DISPEL + 2.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit disease",
            {
                CreateNextAction<CastCleanseSpiritDiseaseOnPartyAction>(ACTION_DISPEL + 2.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit curse",
            {
                CreateNextAction<CastCleanseSpiritCurseOnPartyAction>(ACTION_DISPEL + 2.0f)
            }
        )
    );

    // Range/Mana Triggers
    triggers.push_back(
        new TriggerNode(
            "enemy too close for spell",
            {
                CreateNextAction<FleeAction>(ACTION_MOVE + 9.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member to heal out of spell range",
            {
                CreateNextAction<ReachPartyMemberToHealAction>(ACTION_CRITICAL_HEAL + 1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "water shield",
            {
                CreateNextAction<CastWaterShieldAction>(19.5f)
            }
        )
    );
}

void ShamanHealerDpsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "healer should attack",
            {
                CreateNextAction<CastFlameShockAction>(ACTION_DEFAULT + 0.2f),
                CreateNextAction<CastLavaBurstAction>(ACTION_DEFAULT + 0.1f),
                CreateNextAction<CastLightningBoltAction>(ACTION_DEFAULT)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "medium aoe and healer should attack",
            {
                CreateNextAction<CastChainLightningAction>(ACTION_DEFAULT + 0.3f)
            }
        )
    );
}
