/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GenericWarriorNonCombatStrategy.h"
#include "CreateNextAction.h"
#include "ImbueAction.h"
#include "WarriorActions.h"

class GenericWarriorNonCombatStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    GenericWarriorNonCombatStrategyActionNodeFactory() { creators["berserker rage"] = &berserker_rage; }

private:
    static ActionNode* berserker_rage(PlayerbotAI*)
    {
        return new ActionNode(
            { CreateNextAction<CastBerserkerStanceAction>(1.0f) },
            {},
            {}
        );
    }
};

GenericWarriorNonCombatStrategy::GenericWarriorNonCombatStrategy(PlayerbotAI* botAI) : NonCombatStrategy(botAI)
{
    actionNodeFactories.Add(new GenericWarriorNonCombatStrategyActionNodeFactory());
}

void GenericWarriorNonCombatStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    NonCombatStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "often",
            {
                CreateNextAction<ImbueWithStoneAction>(1.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "fear sleep sap",
            {
                CreateNextAction<CastBerserkerRageAction>(ACTION_EMERGENCY + 1.0f)
            }
        )
    );
}
