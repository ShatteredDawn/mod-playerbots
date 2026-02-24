#pragma once

#include <cmath>

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerPullSonOfHakkarTrigger : public Trigger
{
public:
    HakkarTheSoulflayerPullSonOfHakkarTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer pull son of hakkar") {}

    bool IsActive() override
    {
        if (this->bot == nullptr)
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

        const SonOfHakkarMemory& sonOfHakkarMemory = sonOfHakkarAssistant.getMemory();

        if (sonOfHakkarMemory.getSonOfHakkarKilledForCurrentBloodSiphon() == true)
        {
            return false;
        }

        const Unit* const sonOfHakkarDesignatedPuller = sonOfHakkarMemory.getSonOfHakkarDesignatedPuller();

        if (sonOfHakkarDesignatedPuller == nullptr)
        {
            return false;
        }

        if (sonOfHakkarDesignatedPuller->GetGUID() != this->bot->GetGUID())
        {
            return false;
        }

        const Creature* const sonOfHakkar = sonOfHakkarMemory.getActiveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        if (sonOfHakkar->IsInCombatWith(this->bot) == true || this->bot->IsInCombatWith(sonOfHakkar) == true)
        {
            return false;
        }

        const Position sonOfHakkarPosition = sonOfHakkar->GetPosition();
        const float distanceToSonOfHakkar = this->bot->GetExactDist2d(sonOfHakkarPosition.GetPositionX(), sonOfHakkarPosition.GetPositionY());

        const bool meleeBot = PlayerbotAI::IsMelee(this->bot);

        const float minimumDistance = meleeBot ? 5.0f : 25.0f;

        return distanceToSonOfHakkar < minimumDistance;
    }
};
