/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "ServerFacade.h"
#include "Trigger.h"
#include "WaitForAttackStrategy.h"

class PlayerbotAI;

class WaitForAttackSafeDistanceTrigger : public Trigger
{
public:
    WaitForAttackSafeDistanceTrigger(PlayerbotAI* botAI)
        : Trigger(botAI, "wait for attack safe distance") {}

    bool IsActive() override
    {
        if (this->botAI == nullptr)
        {
            return false;
        }

        if (!WaitForAttackStrategy::ShouldWait(*this->botAI))
        {
            return false;
        }

        // Do not move if stay strategy is set
        if (this->botAI->HasStrategy("stay", this->botAI->GetState()))
        {
            return false;
        }

        // Do not move if currently being targeted
        if (!this->bot->getAttackers().empty())
        {
            return false;
        }

        Value<Unit*>* const currentTargetValue = this->context->GetValue<Unit*>("current target");

        if (currentTargetValue == nullptr)
        {
            return false;
        }

        Unit* const target = currentTargetValue->Get();

        if (target == nullptr)
        {
            return false;
        }

        const float safeDistance = WaitForAttackStrategy::GetSafeDistance();
        const float safeDistanceThreshold = WaitForAttackStrategy::GetSafeDistanceThreshold();
        const float distanceToTarget = ServerFacade::instance().GetDistance2d(this->bot, target);

        return (distanceToTarget > (safeDistance + safeDistanceThreshold)) ||
               (distanceToTarget < (safeDistance - safeDistanceThreshold));
    }
};
