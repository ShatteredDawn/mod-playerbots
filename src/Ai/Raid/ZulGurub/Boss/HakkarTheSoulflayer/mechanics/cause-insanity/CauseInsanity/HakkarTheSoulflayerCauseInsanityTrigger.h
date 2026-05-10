#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerCauseInsanityTrigger : public Trigger
{
public:
    HakkarTheSoulflayerCauseInsanityTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer cause insanity") {}

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

        const uint8_t botClassInfo = this->bot->getClass();

        static constexpr std::array<uint8_t, 2> controllingClasses = { CLASS_MAGE, CLASS_WARLOCK };

        if (std::find(controllingClasses.begin(), controllingClasses.end(), botClassInfo) == controllingClasses.end())
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return false;
        }

        const Player* const insaneAlly = hakkarTheSoulflayerAssistant.findInsaneAlly(*this->bot);

        if (insaneAlly == nullptr)
        {
            return false;
        }

        if (insaneAlly->IsCrowdControlled())
        {
            return false;
        }

        return true;
    }
};
