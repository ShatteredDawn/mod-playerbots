#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1HolyWrathTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1HolyWrathTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 holy wrath") {}

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

        if (PlayerbotAI::IsTank(this->bot) == true)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        if (highPriestVenoxisAssistant.isInCombatWithVenoxis(*this->bot) == false)
        {
            return false;
        }

        if (this->botAI->IsMainTank(this->bot) == true)
        {
            return false;
        }

        if (highPriestVenoxisAssistant.isInPhase1(*this->bot) == false)
        {
            return false;
        }

        if (highPriestVenoxisAssistant.isAtSafeDistanceFromVenoxis(*this->bot) == true)
        {
            return false;
        }

        return true;
    }
};
