#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1RazzashiCobrasPositioningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 razzashi cobras positioning") {}

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

        if (this->botAI->IsTank(this->bot) == false)
        {
            return false;
        }

        if (this->botAI->IsMainTank(this->bot) == true)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        if (highPriestVenoxisAssistant.isInCombatWithVenoxis(*this->bot) == false)
        {
            return false;
        }

        const std::vector<Unit*> razzashiCobras = highPriestVenoxisAssistant.findRazzashiCobras(*this->bot);

        if (razzashiCobras.empty())
        {
            return false;
        }

        Unit* const firstCobra = razzashiCobras.front();

        if (firstCobra == nullptr)
        {
            return false;
        }

        if (firstCobra->IsAlive() == false)
        {
            return false;
        }

        const Position cobrasIdealPosition = highPriestVenoxisAssistant.getRazzashiCobrasPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(cobrasIdealPosition.GetPositionX(), cobrasIdealPosition.GetPositionY());
        const float maxDistance = highPriestVenoxisAssistant.getRazzashiCobrasMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        return true;
    }
};
