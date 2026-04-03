/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "SurvivalHunterStrategy.h"
#include "CreateNextAction.h"
#include "HunterActions.h"

// ===== Action Node Factory =====
class SurvivalHunterStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    SurvivalHunterStrategyActionNodeFactory()
    {
        creators["explosive shot rank 4"] = &explosive_shot_rank_4;
        creators["explosive shot rank 3"] = &explosive_shot_rank_3;
        creators["explosive shot rank 2"] = &explosive_shot_rank_2;
    }

private:
    static ActionNode* explosive_shot_rank_4(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastExplosiveShotRank3Action>(1.0f) },
            {}
        );
    }
    static ActionNode* explosive_shot_rank_3(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastExplosiveShotRank2Action>(1.0f) },
            {}
        );
    }
    static ActionNode* explosive_shot_rank_2(PlayerbotAI*)
    {
        return new ActionNode(
            {},
            { CreateNextAction<CastExplosiveShotRank1Action>(1.0f) },
            {}
        );
    }
};

SurvivalHunterStrategy::SurvivalHunterStrategy(PlayerbotAI* botAI) : GenericHunterStrategy(botAI)
{
    actionNodeFactories.Add(new SurvivalHunterStrategyActionNodeFactory());
}

// ===== Default Actions =====
std::vector<NextAction> SurvivalHunterStrategy::getDefaultActions()
{
    return {
        CreateNextAction<CastKillCommandAction>(5.9f),
        CreateNextAction<CastKillShotAction>(5.8f),
        // @TODO: Needs to be made universal
        // CreateNextAction<CastExplosiveShotAction>(5.7f),
        CreateNextAction<CastBlackArrowAction>(5.6f),
        CreateNextAction<CastSerpentStingAction>(5.5f),
        CreateNextAction<CastAimedShotAction>(5.4f),
        CreateNextAction<CastArcaneShotAction>(5.3f),
        CreateNextAction<CastSteadyShotAction>(5.2f),
        CreateNextAction<CastAutoShotAction>(5.1f)
    };
}

// ===== Trigger Initialization ===
void SurvivalHunterStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    GenericHunterStrategy::InitTriggers(triggers);

    triggers.push_back(
        new TriggerNode(
            "lock and load",
            {
                CreateNextAction<CastExplosiveShotRank4Action>(28.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "kill command",
            {
                CreateNextAction<CastKillCommandAction>(18.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "target critical health",
            {
                CreateNextAction<CastKillShotAction>(18.0f)
            }
        )
    );
    // triggers.push_back(
    //     new TriggerNode(
    //         "explosive shot",
    //         {
    //             CreateNextAction<CastExplosiveShotAction>(17.5f)
    //         }
    //     )
    // );
    triggers.push_back(
        new TriggerNode(
            "black arrow",
            {
                CreateNextAction<CastBlackArrowAction>(16.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "low mana",
            {
                CreateNextAction<CastViperStingAction>(16.0f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "no stings",
            {
                CreateNextAction<CastSerpentStingAction>(15.5f)
            }
        )
    );
    triggers.push_back(
        new TriggerNode(
            "serpent sting on attacker",
            {
                CreateNextAction<CastSerpentStingOnAttackerAction>(15.0f)
            }
        )
    );
}
