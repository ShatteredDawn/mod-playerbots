#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestThekalHealthBalanceTrigger : public Trigger
{
public:
    HighPriestThekalHealthBalanceTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest thekal phase 1 health balance") {}

    bool IsActive() override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        if (this->botAI == nullptr)
        {
            return false;
        }

        if (PlayerbotAI::IsDps(this->bot) == false)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        HighPriestThekalAssistant& highPriestThekalAssistant = raidLeader.getHighPriestThekalAssistant();

        if (highPriestThekalAssistant.isInCombatWithHighPriestThekal(*this->bot) == false)
        {
            return false;
        }

        if (highPriestThekalAssistant.isInPhase1(*this->bot) == false)
        {
            return false;
        }

        const std::array<Creature*, 3> bosses = highPriestThekalAssistant.getActiveBossesSortedByHealth(*this->bot);

        Creature* const highestHealthBoss = bosses[0];

        if (highestHealthBoss == nullptr)
        {
            return false;
        }

        const Creature* const secondHighestHealthBoss = bosses[1];

        if (secondHighestHealthBoss == nullptr)
        {
            return false;
        }

        const uint32_t healthDifference = highestHealthBoss->GetHealth() - secondHighestHealthBoss->GetHealth();

        if (healthDifference < uint32_t(HighPriestThekalEnum::MAXIMUM_HEALTH_DIFFERENCE))
        {
            return false;
        }

        if (this->bot->GetTarget() == highestHealthBoss->GetGUID())
        {
            return false;
        }

        Value<Unit*>* const rtiTargetValue = this->context->GetValue<Unit*>("rti target");

        if (rtiTargetValue == nullptr)
        {
            return false;
        }

        const Unit* const rtiTarget = rtiTargetValue->Get();

        if (rtiTarget != nullptr && rtiTarget->GetGUID() == highestHealthBoss->GetGUID())
        {
            return false;
        }

        return true;
    }
};
