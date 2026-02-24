#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerExcessiveThreatTrigger : public Trigger
{
public:
    HakkarTheSoulflayerExcessiveThreatTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer excessive threat") {}

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

        const uint8_t botClassInfo = this->bot->getClass();

        static constexpr std::array<uint8_t, 4> classesWithSpecialActions = {
            CLASS_PALADIN,
            CLASS_HUNTER,
            CLASS_ROGUE,
            CLASS_PRIEST,
        };

        if (std::find(classesWithSpecialActions.begin(), classesWithSpecialActions.end(), botClassInfo) != classesWithSpecialActions.end())
        {
            return false;
        }

        RaidTankAssistant& raidTankAssistant = raidLeader.getRaidTankAssistant();

        return hakkarTheSoulflayerAssistant.hasExcessiveThreat(
            *this->bot,
            {
                .raidTankAssistant = raidTankAssistant
            }
        );
    }
};
