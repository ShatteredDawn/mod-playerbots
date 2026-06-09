/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ShamanNonCombatStrategy.h"
#include "AiFactory.h"
#include "CreateNextAction.h"
#include "GenericActions.h"
#include "ShamanActions.h"
#include "shaman/RefillShamanReagentsAction.h"

class ShamanNonCombatStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    ShamanNonCombatStrategyActionNodeFactory()
    {
        creators["flametongue weapon main hand"] = &flametongue_weapon_main_hand;
        // creators["frostbrand weapon off hand"] = &frostbrand_weapon_off_hand;
        creators["windfury weapon main hand"] = &windfury_weapon_main_hand;
        creators["earthliving weapon main hand"] = &earthliving_weapon_main_hand;
        creators["cleanse spirit"] = &cleanse_spirit;
        creators["cleanse spirit poison on party"] = &cleanse_spirit_poison_on_party;
        creators["cleanse spirit disease on party"] = &cleanse_spirit_disease_on_party;
    }

private:
    static ActionNode* flametongue_weapon_main_hand(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastRockbiterWeaponMainHandAction>(1.0f) },
            {}
        );
    }
    // static ActionNode* frostbrand_weapon_off_hand(PlayerbotAI*)
    // {
    //     return new ActionNode("frostbrand weapon off hand",
    //                           /*P*/ {},
    //                           /*A*/ { NextAction("flametongue weapon off hand") },
    //                           /*C*/ {});
    // }
    static ActionNode* earthliving_weapon_main_hand(PlayerbotAI*)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastFlametongueWeaponMainHandAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* windfury_weapon_main_hand(PlayerbotAI*)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastFlametongueWeaponMainHandAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* cleanse_spirit(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsActionSham>(1.0f) },
            {});
    }
    static ActionNode* cleanse_spirit_poison_on_party(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsPoisonOnPartyActionSham>(1.0f) },
            {});
    }
    static ActionNode* cleanse_spirit_disease_on_party(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsDiseaseOnPartyActionSham>(1.0f) },
            {});
    }
};

ShamanNonCombatStrategy::ShamanNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    actionNodeFactories.Add(new ShamanNonCombatStrategyActionNodeFactory());
}

void ShamanNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    // Totemic Recall
    triggers.push_back(
        new TriggerNode(
            "totemic recall",
            {
                CreateNextAction<CastTotemicRecallAction>(60.0f),
            }
        )
    );

    // Healing/Resurrect Triggers
    triggers.push_back(
        new TriggerNode(
            "party member dead",
            {
                CreateNextAction<CastAncestralSpiritAction>(ACTION_CRITICAL_HEAL + 10.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(31.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(30.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member low health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(29.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(28.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member medium health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(27.0f),
                CreateNextAction<CastHealingWaveOnPartyAction>(26.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member almost full health",
            {
                CreateNextAction<CastRiptideOnPartyAction>(25.0f),
                CreateNextAction<CastLesserHealingWaveOnPartyAction>(24.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "group heal setting",
            {
                CreateNextAction<CastChainHealAction>(27.0f)
            }
        )
    );

    // Cure Triggers
    triggers.push_back(
        new TriggerNode(
            "cleanse spirit poison",
            {
                CreateNextAction<CastCleanseSpiritAction>(24.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit poison",
            {
                CreateNextAction<CastCleanseSpiritPoisonOnPartyAction>(23.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "cleanse spirit disease",
            {
                CreateNextAction<CastCleanseSpiritAction>(24.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit disease",
            {
                CreateNextAction<CastCleanseSpiritDiseaseOnPartyAction>(23.0f),
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "cleanse spirit curse",
            {
                CreateNextAction<CastCleanseSpiritAction>(24.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member cleanse spirit curse",
            {
                CreateNextAction<CastCleanseSpiritCurseOnPartyAction>(23.0f),
            }
        )
    );


    triggers.emplace_back(
        new TriggerNode(
            "seldom",
            {
                CreateNextAction<RefillShamanReagentsAction>(ACTION_HIGH)
            }
        )
    );


    // Out of Combat Buff Triggers
    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == SHAMAN_TAB_ELEMENTAL)
    {
        triggers.push_back(
            new TriggerNode(
                "main hand weapon no imbue",
                {
                    CreateNextAction<CastFlametongueWeaponMainHandAction>(22.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "water shield",
                {
                    CreateNextAction<CastWaterShieldAction>(21.0f),
                }
            )
        );
    }
    else if (tab == SHAMAN_TAB_ENHANCEMENT)
    {
        triggers.push_back(
            new TriggerNode(
                "main hand weapon no imbue",
                {
                    CreateNextAction<CastWindfuryWeaponMainHandAction>(22.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "off hand weapon no imbue",
                {
                    CreateNextAction<CastFlametongueWeaponMainHandAction>(21.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "lightning shield",
                {
                    CreateNextAction<CastLightningShieldAction>(20.0f),
                }
            )
        );
    }
    else if (tab == SHAMAN_TAB_RESTORATION)
    {
        triggers.push_back(
            new TriggerNode(
                "main hand weapon no imbue",
                {
                    CreateNextAction<CastEarthlivingWeaponMainHandAction>(22.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "water shield",
                {
                    CreateNextAction<CastWaterShieldAction>(20.0f),
                }
            )
        );
    }

    // Buff Triggers while swimming
    triggers.push_back(
        new TriggerNode(
            "water breathing",
            {
                CreateNextAction<CastWaterBreathingAction>(12.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "water walking",
            {
                CreateNextAction<CastWaterWalkingAction>(12.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "water breathing on party",
            {
                CreateNextAction<CastWaterBreathingOnPartyAction>(11.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "water walking on party",
            {
                CreateNextAction<CastWaterWalkingOnPartyAction>(11.0f),
            }
        )
    );

    // Pet Triggers
    triggers.push_back(
        new TriggerNode(
            "has pet",
            {
                CreateNextAction<TogglePetSpellAutoCastAction>(60.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "new pet",
            {
                CreateNextAction<SetPetStanceAction>(65.0f),
            }
        )
    );
}

void ShamanNonCombatStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    NonCombatStrategy::InitMultipliers(multipliers);
}
