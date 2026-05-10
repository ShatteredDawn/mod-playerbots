#pragma once

#include <cmath>

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerBringBackSonOfHakkarTrigger : public Trigger
{
public:
    HakkarTheSoulflayerBringBackSonOfHakkarTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer bring back son of hakkar") {}

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
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
        const SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        if (hakkarTheSoulflayerAssistant.isInCombatWithHakkar(*this->bot) == false)
        {
            return false;
        }

        const SonOfHakkarMemory& memory = sonOfHakkarAssistant.getMemory();

        if (memory.getSonOfHakkarKilledForCurrentBloodSiphon() == true)
        {
            return false;
        }

        const Creature* const sonOfHakkar = memory.getActiveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        if (sonOfHakkar->GetVictim() != this->bot)
        {
            return false;
        }

        return true;
    }
};
