/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "WaitForAttackStrategy.h"

#include "Action.h"
#include "AiObjectContext.h"
#include "CreateNextAction.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "Strategy.h"
#include "WaitForAttackAction.h"

void WaitForAttackStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "wait for attack safe distance",
        {
            CreateNextAction<WaitForAttackKeepSafeDistanceAction>(ACTION_RAID)
        }
    ));
}

void WaitForAttackStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
    multipliers.push_back(new WaitForAttackMultiplier(botAI));
}

bool WaitForAttackStrategy::ShouldWait(PlayerbotAI& botAI)
{
    if (!botAI.HasStrategy("wait for attack", BOT_STATE_COMBAT))
    {
        return false;
    }

    const Player* const bot = botAI.GetBot();

    if (bot == nullptr)
    {
        return false;
    }

    if (!bot->GetGroup() || !botAI.HasActivePlayerMaster())
    {
        return false;
    }

    AiObjectContext* const context = botAI.GetAiObjectContext();

    if (context == nullptr)
    {
        return false;
    }

    Value<Unit*>* const currentTargetValue = context->GetValue<Unit*>("current target");

    if (currentTargetValue == nullptr)
    {
        return false;
    }

    // Don't wait if the current target is an enemy player
    const Unit* const  target = currentTargetValue->Get();

    if (target && target->IsPlayer())
    {
        return false;
    }

    Value<time_t>* const combatStartTimeValue = context->GetValue<time_t>("combat start time");

    if (combatStartTimeValue == nullptr)
    {
        return false;
    }

    time_t combatStartTime = combatStartTimeValue->Get();

    if (bot->IsInCombat())
    {
        if (combatStartTime == 0)
        {
            combatStartTime = time(nullptr);
            combatStartTimeValue->Set(combatStartTime);
        }

        const time_t elapsedTime = time(nullptr) - combatStartTime;

        return elapsedTime < WaitForAttackStrategy::GetWaitTime(botAI);
    }


    if (combatStartTime != 0)
    {
        combatStartTimeValue->Set(0);
    }

    return false;
}

uint8_t WaitForAttackStrategy::GetWaitTime(PlayerbotAI& botAI)
{
    AiObjectContext* const context = botAI.GetAiObjectContext();

    if (context == nullptr)
    {
        return 0;
    }

    Value<uint8_t>* const waitForAttackTimeValue = context->GetValue<uint8_t>("wait for attack time");

    if (waitForAttackTimeValue == nullptr)
    {
        return 0;
    }

    return waitForAttackTimeValue->Get();
}

float WaitForAttackStrategy::GetSafeDistance()
{
    return PlayerbotAIConfig::instance().spellDistance;
}

float WaitForAttackMultiplier::GetValue(Action& action)
{
    if (this->botAI == nullptr)
    {
        return 0.0f;
    }

    const std::string& actionName = action.getName();

    if (actionName != "wait for attack keep safe distance" &&
        actionName != "dps assist" &&
        actionName != "set facing" &&
        actionName != "pull my target" &&
        actionName != "pull rti target" &&
        actionName != "reach pull" &&
        actionName != "pull start" &&
        actionName != "pull action" &&
        actionName != "pull end")
    {
        return WaitForAttackStrategy::ShouldWait(*this->botAI) ? 0.0f : 1.0f;
    }

    return 1.0f;
}
