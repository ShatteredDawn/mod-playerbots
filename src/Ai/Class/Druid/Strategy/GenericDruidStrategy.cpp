/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericDruidStrategy.h"

#include "CancelChannelAction.h"
#include "CreateNextAction.h"
#include "DruidActions.h"
#include "DruidCatActions.h"
#include "DruidShapeshiftActions.h"
#include "GenericActions.h"
#include "AiFactory.h"
#include "FeralDruidStrategy.h"
#include "MageActions.h"

class GenericDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericDruidStrategyActionNodeFactory()
    {
        creators["melee"] = &melee;
        creators["caster form"] = &caster_form;
        creators["cure poison"] = &cure_poison;
        creators["cure poison on party"] = &cure_poison_on_party;
        creators["abolish poison"] = &abolish_poison;
        creators["abolish poison on party"] = &abolish_poison_on_party;
        creators["rebirth"] = &rebirth;
        creators["entangling roots on cc"] = &entangling_roots_on_cc;
        creators["cyclone on cc"] = &cyclone_on_cc;
        creators["hibernate on cc"] = &hibernate_on_cc;
    }

private:
    static ActionNode* melee([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* caster_form([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* cure_poison([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* cure_poison_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* abolish_poison([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* abolish_poison_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* rebirth([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* entangling_roots_on_cc([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* cyclone_on_cc([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* hibernate_on_cc([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }
};

GenericDruidStrategy::GenericDruidStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericDruidStrategyActionNodeFactory());
}

void GenericDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    CombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "almost full health",
            {
                CreateNextAction<CastBarkskinAction>(40.0f)
            }
        )
    );

    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == DRUID_TAB_FERAL)
    {
        if (!bot->HasAura(16931) /*thick hide — bear spec*/)
        {
            triggers.push_back(
                new TriggerNode(
                    "predator's swiftness and combat party member dead",
                    {
                        CreateNextAction<CastRebirthAction>(29.0f)
                    }
                )
            );
            triggers.push_back(
                new TriggerNode(
                    "combat party member dead",
                    {
                        CreateNextAction<CastRebirthAction>(28.5f)
                    }
                )
            );
        }
    }
    else
    {
        triggers.push_back(
            new TriggerNode(
                "combat party member dead",
                {
                    CreateNextAction<CastRebirthAction>(29.0f)
                }
            )
        );
    }

    triggers.push_back(
        new TriggerNode(
            "being attacked",
            {
                CreateNextAction<CastNaturesGraspAction>(39.0f)
            }
        )
    );
}

void DruidCureStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "party member cure poison",
            {
                CreateNextAction<CastAbolishPoisonOnPartyAction>(51.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "party member remove curse",
            {
                CreateNextAction<CastRemoveCurseOnPartyAction>(57.0f)
            }
        )
    );

}

void DruidBoostStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == DRUID_TAB_BALANCE)
    {
        triggers.push_back(
            new TriggerNode(
                "force of nature",
                {
                    CreateNextAction<CastForceOfNatureAction>(29.0f)
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
    }

    if (tab == DRUID_TAB_FERAL)
    {
        triggers.push_back(
            new TriggerNode(
                "berserk",
                {
                    CreateNextAction<CastBerserkAction>(27.5f)
                }
            )
        );
    }
}

void DruidCcStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == DRUID_TAB_BALANCE || tab == DRUID_TAB_RESTORATION)
    {
        triggers.push_back(
            new TriggerNode(
                "cyclone",
                {
                    CreateNextAction<CastCycloneCcAction>(42.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "hibernate",
                {
                    CreateNextAction<CastHibernateCcAction>(41.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "entangling roots",
                {
                    CreateNextAction<CastEntanglingRootsCcAction>(40.0f)
                }
            )
        );
    }
    if (tab == DRUID_TAB_FERAL)
    {
        if (bot->HasSpell(SPELL_CAT_FORM) && !bot->HasAura(AURA_THICK_HIDE))
        {
            triggers.push_back(
                new TriggerNode(
                    "predator's swiftness and cyclone",
                    {
                        CreateNextAction<CastCycloneCcAction>(42.0f)
                    }
                )
            );
            triggers.push_back(
                new TriggerNode(
                    "predator's swiftness and hibernate",
                    {
                        CreateNextAction<CastHibernateCcAction>(41.0f)
                    }
                )
            );
            triggers.push_back(
                new TriggerNode(
                    "predator's swiftness and entangling roots",
                    {
                        CreateNextAction<CastEntanglingRootsCcAction>(40.0f)
                    }
                )
            );
        }
        else
        {
            triggers.push_back(
                new TriggerNode(
                    "cyclone",
                    {
                        CreateNextAction<CastCycloneCcAction>(42.0f)
                    }
                )
            );
            triggers.push_back(
                new TriggerNode(
                    "hibernate",
                    {
                        CreateNextAction<CastHibernateCcAction>(41.0f)
                    }
                )
            );
            triggers.push_back(
                new TriggerNode(
                    "entangling roots",
                    {
                        CreateNextAction<CastEntanglingRootsCcAction>(40.0f)
                    }
                )
            );
        }
    }
}

void DruidHealerDpsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "healer should attack",
            {
                CreateNextAction<CastCancelTreeFormAction>(5.4f),
                CreateNextAction<CastMoonfireAction>(5.3f),
                CreateNextAction<CastWrathAction>(5.2f),
                CreateNextAction<CastStarfireAction>(5.1f),
            }
        )
    );
}

void DruidAoeStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    Player* bot = botAI->GetBot();
    int tab = AiFactory::GetPlayerSpecTab(bot);

    if (tab == DRUID_TAB_BALANCE)
    {
        triggers.push_back(
            new TriggerNode(
                "hurricane channel check",
                {
                    CreateNextAction<CancelChannelAction>(22.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "starfall",
                {
                    CreateNextAction<CastStarfallAction>(28.5f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "medium aoe",
                {
                    CreateNextAction<CastHurricaneAction>(23.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "enemy within melee",
                {
                    CreateNextAction<CastTyphoonAction>(40.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "insect swarm on attacker",
                {
                    CreateNextAction<CastInsectSwarmOnAttackerAction>(5.2f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "moonfire on attacker",
                {
                    CreateNextAction<CastMoonfireOnAttackerAction>(5.1f)
                }
            )
        );
    }

    if (tab == DRUID_TAB_RESTORATION)
    {
        triggers.push_back(
            new TriggerNode(
                "hurricane channel check",
                {
                    CreateNextAction<CancelChannelAction>(22.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "medium aoe",
                {
                    CreateNextAction<CastHurricaneAction>(23.0f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "insect swarm on attacker",
                {
                    CreateNextAction<CastInsectSwarmOnAttackerAction>(5.2f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "moonfire on attacker",
                {
                    CreateNextAction<CastMoonfireOnAttackerAction>(5.1f)
                }
            )
        );
    }

    if (tab == DRUID_TAB_FERAL && bot->HasSpell(SPELL_CAT_FORM) && !bot->HasAura(AURA_THICK_HIDE))
    {
        triggers.push_back(
            new TriggerNode(
                "clearcasting and medium aoe",
                {
                    CreateNextAction<CastSwipeCatAction>(25.5f)
                }
            )
        );
        triggers.push_back(
            new TriggerNode(
                "medium aoe",
                {
                    CreateNextAction<CastSwipeCatAction>(25.0f)
                }
            )
        );
    }
}
