/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericDKStrategy.h"

#include "DKActions.h"
#include "DKAiObjectContext.h"

class GenericDKStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericDKStrategyActionNodeFactory()
    {
        creators["killing machine"] = &killing_machine;
        creators["anti magic zone"] = &anti_magic_zone;
        creators["death grip"] = &death_grip;
    }

private:
    static ActionNode* death_grip([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastIcyTouchAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* killing_machine([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastImprovedIcyTalonsAction>(1.0f) },
            /*C*/ {}
        );
    }

    static ActionNode* anti_magic_zone([[maybe_unused]] PlayerbotAI* botAI)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastAntiMagicShellAction>(1.0f) },
            /*C*/ {}
        );
    }
};

GenericDKStrategy::GenericDKStrategy(PlayerbotAI* botAI) : MeleeCombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericDKStrategyActionNodeFactory());
}

void GenericDKStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    MeleeCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "mind freeze",
            {
                CreateNextAction<CastMindFreezeAction>(ACTION_HIGH + 1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "mind freeze on enemy healer",
            {
                CreateNextAction<CastMindFreezeOnEnemyHealerAction>(ACTION_HIGH + 1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "horn of winter",
            {
                CreateNextAction<CastHornOfWinterAction>(ACTION_NORMAL + 1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "critical health",
            {
                CreateNextAction<CastRaiseDeadAction>(ACTION_HIGH + 6.0f),
                CreateNextAction<CastDeathPactAction>(ACTION_HIGH + 5.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low health",
            {
                CreateNextAction<CastIceboundFortitudeAction>(ACTION_HIGH + 5.0f),
                CreateNextAction<CastRuneTapAction>(ACTION_HIGH + 4.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "medium aoe",
            {
                CreateNextAction<CastDeathAndDecayAction>(ACTION_HIGH + 9.0f),
                CreateNextAction<CastPestilenceAction>(ACTION_NORMAL + 4.0f),
                CreateNextAction<CastBloodBoilAction>(ACTION_NORMAL + 3.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "pestilence glyph",
            {
                CreateNextAction<CastPestilenceAction>(ACTION_HIGH + 9.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode("no rune",
            {
                CreateNextAction<CastEmpowerRuneWeaponAction>(ACTION_HIGH + 1.0f)
            }
        )
    );
}
