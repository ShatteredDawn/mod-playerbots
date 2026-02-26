#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"
#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1VenoxisPositioningTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1VenoxisPositioningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 venoxis positioning") {}

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

        if (this->botAI->IsMainTank(this->bot) == false)
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

        if (highPriestVenoxisAssistant.isInPhase1(*this->bot) == false)
        {
            return false;
        }

        const Unit* const boss = highPriestVenoxisAssistant.findActiveBoss(*this->bot);

        if (boss == nullptr)
        {
            return false;
        }

        const Position bossIdealPosition = highPriestVenoxisAssistant.getVenoxisPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        const float maxDistance = highPriestVenoxisAssistant.getVenoxisMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        return true;
    }
};
