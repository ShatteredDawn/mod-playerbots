#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"


class HakkarTheSoulflayerGoToPoisonousCloudTrigger : public Trigger
{
public:
    HakkarTheSoulflayerGoToPoisonousCloudTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer go to poisonous cloud") {}

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
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
        const SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return false;
        }

        if (this->bot->HasAura(uint32_t(PoisonousCloudEnum::AURA_POISONOUS_BLOOD)) == true)
        {
            return false;
        }

        const Unit* const poisonousCloud = sonOfHakkarAssistant.findPoisonousCloud(*this->bot);

        if (poisonousCloud == nullptr)
        {
            return false;
        }

        const Position poisonousCloudPosition = poisonousCloud->GetPosition();
        const float distanceToPoisonousCloud = this->bot->GetExactDist2d(poisonousCloudPosition.GetPositionX(), poisonousCloudPosition.GetPositionY());
        static constexpr float maxDistance = 5.0f;

        if (distanceToPoisonousCloud < maxDistance)
        {
            return false;
        }

        return true;
    }
};
