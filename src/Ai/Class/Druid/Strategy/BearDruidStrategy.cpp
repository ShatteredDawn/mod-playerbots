/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "BearDruidStrategy.h"

#include "CreateNextAction.h"
#include "ActionNode.h"
#include "GenericActions.h"
#include "ReachTargetActions.h"
#include "DruidActions.h"
#include "DruidShapeshiftActions.h"
#include "DruidBearActions.h"

class BearDruidStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    BearDruidStrategyActionNodeFactory()
    {
        creators["feral charge - bear"] = &feral_charge_bear;
        creators["swipe (bear)"] = &swipe_bear;
        creators["bear form"] = &bear_form;
        creators["dire bear form"] = &dire_bear_form;
        creators["mangle (bear)"] = &mangle_bear;
        creators["maul"] = &maul;
        creators["bash"] = &bash;
        creators["swipe"] = &swipe;
        creators["lacerate"] = &lacerate;
        creators["demoralizing roar"] = &demoralizing_roar;
        creators["taunt spell"] = &growl;
    }

private:
    static ActionNode* feral_charge_bear([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<ReachMeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* swipe_bear([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* bear_form([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* dire_bear_form([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ { CreateNextAction<CastCasterFormAction>(1.0f) },
            /*A*/ { CreateNextAction<CastBearFormAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* mangle_bear([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* maul([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<MeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* bash([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<MeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* swipe([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<MeleeAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* lacerate([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastMaulAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* growl([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }

    static ActionNode* demoralizing_roar([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ {},
            /*C*/ {}
        );
    }
};

BearDruidStrategy::BearDruidStrategy(PlayerbotAI* botAI) : FeralDruidStrategy(botAI)
{
    actionNodeFactories.Add(new BearDruidStrategyActionNodeFactory());
}

std::vector<NextAction> BearDruidStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastMaulAction>(5.2f),
        CreateNextAction<CastEnrageAction>(5.1f),
        CreateNextAction<MeleeAction>(5.0f)
    };
}

void BearDruidStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    FeralDruidStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "bear form",
            {
                CreateNextAction<CastDireBearFormAction>(28.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "medium health",
            {
                CreateNextAction<CastFrenziedRegenerationAction>(27.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "mangle (bear)",
            {
                CreateNextAction<CastMangleBearAction>(17.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "faerie fire (feral)",
            {
                CreateNextAction<CastFaerieFireFeralAction>(17.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "lacerate",
            {
                CreateNextAction<CastLacerateAction>(16.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "demoralizing roar",
            {
                CreateNextAction<CastDemoralizingRoarAction>(15.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "high aoe",
            {
                CreateNextAction<CastChallengingRoarAction>(26.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "lose aggro",
            {
                CreateNextAction<CastGrowlAction>(26.0f),
                CreateNextAction<CastFaerieFireFeralAction>(25.5f)
            }
        ));
    triggers.push_back(
        new TriggerNode(
            "berserk active",
            {
                CreateNextAction<CastMangleBearAction>(25.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "medium aoe",
            {
                CreateNextAction<CastDemoralizingRoarAction>(24.5f),
                CreateNextAction<CastSwipeBearAction>(24.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "light aoe",
            {
                CreateNextAction<CastSwipeBearAction>(24.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "bash",
            {
                CreateNextAction<CastBashAction>(42.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "bash on enemy healer",
            {
                CreateNextAction<CastBashOnEnemyHealerAction>(41.0f)
            }
        )
    );
}
