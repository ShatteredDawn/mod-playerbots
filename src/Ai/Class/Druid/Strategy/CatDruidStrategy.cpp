/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "CatDruidStrategy.h"

#include "AiObjectContext.h"
#include "DruidActions.h"
#include "DruidCatActions.h"
#include "DruidShapeshiftActions.h"
#include "GenericActions.h"
#include "ReachTargetActions.h"

class CatDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    CatDruidStrategyActionNodeFactory()
    {
        creators["faerie fire (feral)"] = &faerie_fire_feral;
        creators["melee"] = &melee;
        creators["feral charge - cat"] = &feral_charge_cat;
        creators["cat form"] = &cat_form;
        creators["claw"] = &claw;
        creators["mangle (cat)"] = &mangle_cat;
        creators["rake"] = &rake;
        creators["ferocious bite"] = &ferocious_bite;
        creators["rip"] = &rip;
        creators["pounce"] = &pounce;
        creators["ravage"] = &ravage;
        creators["prowl"] = &prowl;
    }

private:
    static ActionNode* faerie_fire_feral([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* melee([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* feral_charge_cat([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<ReachMeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* cat_form([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ { CreateNextAction<CastBearFormAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* claw([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<MeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* mangle_cat([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* rake([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* ferocious_bite([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* rip([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* ravage([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastPounceAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* pounce([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastShredAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* prowl([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCatFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ {}
        );
    }

};

CatDruidStrategy::CatDruidStrategy(PlayerbotAI* botAI) : FeralDruidStrategy(botAI)
{
    actionNodeFactories.Add(new CatDruidStrategyActionNodeFactory());
}

std::vector<NextAction> CatDruidStrategy::getDefaultActions()
{
    return {
        CreateNextAction<MeleeAction>(ACTION_DEFAULT)
    };
}

void CatDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    FeralDruidStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "healer low mana",
            {
                CreateNextAction<CastInnervateOnHealerAction>(35.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "prowl",
            {
                CreateNextAction<CastProwlAction>(29.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "enemy out of melee",
            {
                CreateNextAction<CastDashAction>(28.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "cat form",
            {
                CreateNextAction<CastCatFormAction>(28.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low energy",
            {
                CreateNextAction<CastTigersFuryAction>(27.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "savage roar",
            {
                CreateNextAction<CastSavageRoarAction>(26.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "combo points 5 available",
            {
                CreateNextAction<CastRipAction>(23.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points 5 available",
            {
                CreateNextAction<CastMaimAction>(23.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "ferocious bite execute",
            {
                CreateNextAction<CastFerociousBiteAction>(24.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "clearcasting",
            {
                CreateNextAction<CastShredAction>(24.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "ferocious bite time",
            {
                CreateNextAction<CastFerociousBiteAction>(22.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "mangle (cat)",
            {
                CreateNextAction<CastMangleCatAction>(22.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "rake",
            {
                CreateNextAction<CastRakeAction>(21.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "medium threat",
            {
                CreateNextAction<CastCowerAction>(21.0f)
            }
        )
    );

    triggers.push_back(
        new TriggerNode(
            "almost full energy available",
            {
                CreateNextAction<CastRavageAction>(5.6f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points not full",
            {
                CreateNextAction<CastRavageAction>(5.6f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "almost full energy available",
            {
                CreateNextAction<CastPounceAction>(5.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points not full",
            {
                CreateNextAction<CastPounceAction>(5.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "almost full energy available",
            {
                CreateNextAction<CastShredAction>(5.4f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points not full",
            {
                CreateNextAction<CastShredAction>(5.4f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "almost full energy available",
            {
                CreateNextAction<CastMangleCatAction>(5.3f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points not full and high energy",
            {
                CreateNextAction<CastMangleCatAction>(5.3f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "almost full energy available",
            {
                CreateNextAction<CastClawAction>(5.2f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "combo points not full and high energy",
            {
                CreateNextAction<CastClawAction>(5.2f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "faerie fire (feral)",
            {
                CreateNextAction<CastFaerieFireFeralAction>(5.0f)
            }
        )
    );
}

// ============================================================
// CatOffhealStrategy
// Additive overlay — only the healing triggers. Designed to be
// stacked on top of "cat" so the bot stays in cat form for DPS
// but shifts out to heal when the party needs it.
// ============================================================

class CatOffhealStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    CatOffhealStrategyActionNodeFactory()
    {
        creators["healing touch on party"] = &healing_touch_on_party;
        creators["regrowth on party"]       = &regrowth_on_party;
        creators["rejuvenation on party"]   = &rejuvenation_on_party;
    }

private:
    // P: shift to caster form before casting   C: shift back to cat form afterwards
    static ActionNode* healing_touch_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ { CreateNextAction<CastCatFormAction>(1.0f) }
        );
    }

    static ActionNode* regrowth_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ { CreateNextAction<CastCatFormAction>(1.0f) }
        );
    }

    static ActionNode* rejuvenation_on_party([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ {},
            /*C*/ { CreateNextAction<CastCatFormAction>(1.0f) }
        );
    }
};

CatOffhealStrategy::CatOffhealStrategy(PlayerbotAI* botAI) : CombatStrategy(botAI)
{
    actionNodeFactories.Add(new CatOffhealStrategyActionNodeFactory());
}

void CatOffhealStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "party member critical health",
            {
                CreateNextAction<CastRegrowthOnPartyAction>(36.0f),
                CreateNextAction<CastHealingTouchOnPartyAction>(35.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member low health",
            {
                CreateNextAction<CastHealingTouchOnPartyAction>(25.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member medium health",
            {
                CreateNextAction<CastRejuvenationOnPartyAction>(18.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "party member to heal out of spell range",
            {
                CreateNextAction<ReachPartyMemberToHealAction>(93.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low mana",
            {
                CreateNextAction<CastInnervateAction>(24.0f)
            }
        )
    );
}
