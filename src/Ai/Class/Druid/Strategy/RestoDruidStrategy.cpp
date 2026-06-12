/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "RestoDruidStrategy.h"
#include "DruidActions.h"
#include "DruidShapeshiftActions.h"
#include "MovementActions.h"
#include "ReachTargetActions.h"

RestoDruidStrategy::RestoDruidStrategy(PlayerbotAI* botAI) : GenericDruidStrategy(botAI)
{
    // No custom ActionNodeFactory needed
}

void RestoDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericDruidStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "no healer dps strategy",
            {
                CreateNextAction<CastTreeFormAction>(5.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
        "party member to heal out of spell range",
        {
            CreateNextAction<ReachPartyMemberToHealAction>(39.0f)
        }
    )
);

    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastTreeFormAction>(34.1f),
                CreateNextAction<CastPartySwiftmendAction>(34.0f),
                CreateNextAction<CastWildGrowthOnPartyAction>(33.0f),
                CreateNextAction<CastPartyNourishAction>(32.0f),
                CreateNextAction<CastRegrowthOnPartyAction>(31.0f),
                CreateNextAction<CastHealingTouchOnPartyAction>(30.0f),
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastNaturesSwiftnessAction>(58.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "nature's swiftness active",
            {
                CreateNextAction<CastHealingTouchOnPartyAction>(55.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "clearcasting",
            {
                CreateNextAction<CastLifebloomOnMainTankAction>(13.0f)
            }
        )
    );

    // LOW
    triggers.push_back(
        new TriggerNode(
            "party member low health",
            {
                CreateNextAction<CastTreeFormAction>(21.5f),
                CreateNextAction<CastPartySwiftmendAction>(21.4f),
                CreateNextAction<CastWildGrowthOnPartyAction>(21.3f),
                CreateNextAction<CastPartyNourishAction>(21.2f),
                CreateNextAction<CastRegrowthOnPartyAction>(21.1f),
                CreateNextAction<CastHealingTouchOnPartyAction>(21.0f),
            }
        )
    );

    // MEDIUM
    triggers.push_back(
        new TriggerNode(
            "party member medium health",
                        {
                            CreateNextAction<CastTreeFormAction>(20.5f),
                            CreateNextAction<CastPartySwiftmendAction>(20.4f),
                            CreateNextAction<CastWildGrowthOnPartyAction>(20.3f),
                            CreateNextAction<CastPartyNourishAction>(20.2f),
                            CreateNextAction<CastRegrowthOnPartyAction>(20.1f),
                            CreateNextAction<CastHealingTouchOnPartyAction>(20.0f),
                        }
                    )
                );

    // ALMOST FULL
    triggers.push_back(
        new TriggerNode(
            "party member almost full health",
                        {
                            CreateNextAction<CastWildGrowthOnPartyAction>(10.3f),
                            CreateNextAction<CastRejuvenationOnPartyAction>(10.2f),
                            CreateNextAction<CastRegrowthOnPartyAction>(10.1f),
                        }
                    )
                );

    triggers.push_back(
        new TriggerNode(
            "medium mana",
            {
                CreateNextAction<CastInnervateAction>(25.0f)
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

void DruidTranquilityStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "medium group heal setting",
            {
                CreateNextAction<CastTreeFormAction>(30.6f),
                CreateNextAction<CastTranquilityAction>(30.5f)
            }
        )
    );
}

void DruidBlanketStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "wild growth blanket",
            {
                CreateNextAction<CastTreeFormAction>(8.1f),
                CreateNextAction<CastWildGrowthBlanketAction>(8.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "rejuvenation blanket",
            {
                CreateNextAction<CastTreeFormAction>(6.1f),
                CreateNextAction<CastRejuvenationBlanketAction>(6.0f)
            }
        )
    );
}
