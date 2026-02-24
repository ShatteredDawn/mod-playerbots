#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerHakkarPositioningTrigger : public Trigger
{
public:
    HakkarTheSoulflayerHakkarPositioningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer hakkar positioning") {}

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

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return false;
        }

        const Unit* const boss = hakkarTheSoulflayerAssistant.findActiveBoss(*this->bot);

        if (boss == nullptr)
        {
            return false;
        }

        if (boss->GetVictim() != this->bot)
        {
            return false;
        }

        const Position bossIdealPosition = hakkarTheSoulflayerAssistant.getHakkarPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        const float maxDistance = hakkarTheSoulflayerAssistant.getHakkarMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        return true;
    }
};
