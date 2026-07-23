/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericShamanStrategy.h"
#include "AiFactory.h"
#include "CreateNextAction.h"
#include "GenericActions.h"
#include "ShamanActions.h"

class GenericShamanStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericShamanStrategyActionNodeFactory()
    {
        creators["totem of wrath"] = &totem_of_wrath;
        creators["flametongue totem"] = &flametongue_totem;
        creators["magma totem"] = &magma_totem;
        creators["strength of earth totem"] = &strength_of_earth_totem;
        creators["cleansing totem"] = &cleansing_totem;
        creators["wrath of air totem"] = &wrath_of_air_totem;
        creators["windfury totem"] = &windfury_totem;
        creators["cleanse spirit"] = &cleanse_spirit;
        creators["cleanse spirit poison on party"] = &cleanse_spirit_poison_on_party;
        creators["cleanse spirit disease on party"] = &cleanse_spirit_disease_on_party;
    }

private:
    // Passthrough totems are set up so lower level shamans will still cast totems.
    // Totem of Wrath -> Flametongue Totem -> Searing Totem
    // Magma Totem -> Searing Totem
    // Strength of Earth Totem -> Stoneskin Totem
    // Cleansing Totem -> Mana Spring Totem
    // Wrath of Air Totem -> Windfury Totem -> Grounding Totem

    static ActionNode* totem_of_wrath([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastFlametongueTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* flametongue_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastSearingTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* magma_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastSearingTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* searing_totem(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* strength_of_earth_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastStoneskinTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* stoneskin_totem(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* cleansing_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastManaSpringTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* mana_spring_totem(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* healing_stream_totem(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* wrath_of_air_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastWindfuryTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* windfury_totem([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            {},
            { CreateNextAction<SetGroundingTotemAction>(1.0f) },
            {}
        );
    }
    static ActionNode* grounding_totem(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* wind_shear(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* purge(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            {},
            {}
        );
    }
    static ActionNode* cleanse_spirit(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsActionSham>(1.0f) },
            {}
        );
    }
    static ActionNode* cleanse_spirit_poison_on_party(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsPoisonOnPartyActionSham>(1.0f) },
            {}
        );
    }
    static ActionNode* cleanse_spirit_disease_on_party(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastCureToxinsDiseaseOnPartyActionSham>(1.0f) },
            {}
        );
    }
};

GenericShamanStrategy::GenericShamanStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericShamanStrategyActionNodeFactory());
}

void GenericShamanStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    CombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "wind shear",
            {
                CreateNextAction<CastWindShearAction>(23.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "wind shear on enemy healer",
            {
                CreateNextAction<CastWindShearOnEnemyHealerAction>(23.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "purge",
            {
                CreateNextAction<CastPurgeAction>(ACTION_DISPEL),
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

void ShamanCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
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
}

void ShamanBoostStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "heroism",
            {
                CreateNextAction<CastHeroismAction>(30.0f),
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "bloodlust",
            {
                CreateNextAction<CastBloodlustAction>(30.0f),
            }
        )
    );

    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == SHAMAN_TAB_ELEMENTAL)
    {
        triggers.push_back(
            new TriggerNode(
                "fire elemental totem",
                {
                    CreateNextAction<CastFireElementalTotemAction>(23.0f),
                }
            )
        );
    }
    else if (tab == SHAMAN_TAB_ENHANCEMENT)
    {
        triggers.push_back(
            new TriggerNode(
                "fire elemental totem",
                {
                    CreateNextAction<CastFireElementalTotemMeleeAction>(24.0f),
                }
            )
        );
    }
}

void ShamanAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{

    Player* bot = botAI->GetBot();
    const uint8_t tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == SHAMAN_TAB_ELEMENTAL)
    {
        triggers.push_back(
            new TriggerNode(
                "medium aoe",{ CreateNextAction<CastFireNovaAction>(23.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "chain lightning no cd",
                {
                    CreateNextAction<CastChainLightningAction>(5.6f),
                }
            )
        );
    }

    if (tab == SHAMAN_TAB_ENHANCEMENT)
    {
        triggers.push_back(
            new TriggerNode(
                "maelstrom weapon 5 and medium aoe",
                {
                    CreateNextAction<CastChainLightningAction>(22.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "maelstrom weapon 4 and medium aoe",
                {
                    CreateNextAction<CastChainLightningAction>(21.0f),
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "enemy within melee",
                {
                    CreateNextAction<CastFireNovaAction>(5.1f),
                }
            )
        );
    }

    if (tab == SHAMAN_TAB_RESTORATION)
    {
        // Handled by "Healer DPS" Strategy
    }
}
