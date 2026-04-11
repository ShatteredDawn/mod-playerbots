/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ActionNode.h"

#include "HunterBuffStrategies.h"
#include "CreateNextAction.h"
#include "HunterActions.h"
#include "Strategy.h"

class BuffHunterStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    BuffHunterStrategyActionNodeFactory()
    {
        creators["aspect of the dragonhawk"] = &aspect_of_the_dragonhawk;
        creators["aspect of the hawk"] = &aspect_of_the_hawk;
        creators["aspect of the pack"] = &aspect_of_the_pack;
    }

private:
    static ActionNode* aspect_of_the_dragonhawk(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastAspectOfTheHawkAction>(1.0f) },
            {}
        );
    }

    static ActionNode* aspect_of_the_hawk(PlayerbotAI*)
    {
        return new ActionNode(
            /*P*/ {},
            /*A*/ { CreateNextAction<CastAspectOfTheMonkeyAction>(1.0f) },
            /*C*/ {}
        );
    }
    static ActionNode* aspect_of_the_pack(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastAspectOfTheCheetahAction>(1.0f) },
            {}
        );
    }
};

HunterBuffDpsStrategy::HunterBuffDpsStrategy(PlayerbotAI* botAI) : Strategy(botAI)
{
    actionNodeFactories.Add(new BuffHunterStrategyActionNodeFactory());
}

void HunterBuffDpsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "aspect of the dragonhawk",
            {
                CreateNextAction<CastAspectOfTheDragonhawkAction>(ACTION_HIGH)
            }
        )
    );
}

void HunterNatureResistanceStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "aspect of the wild",
            {
                CreateNextAction<CastAspectOfTheWildAction>(ACTION_HIGH)
            }
        )
    );
}

void HunterBuffSpeedStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(
        new TriggerNode(
            "aspect of the pack",
            {
                CreateNextAction<CastAspectOfThePackAction>(ACTION_HIGH)
            }
        )
    );
}
