#pragma once

#include <cmath>

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerMoveToSonOfHakkarTrigger : public Trigger
{
public:
    HakkarTheSoulflayerMoveToSonOfHakkarTrigger(PlayerbotAI* botAI) : Trigger(botAI, "hakkar the soulflayer move to son of hakkar") {}

    bool IsActive() override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();
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

        const Creature* const sonOfHakkar = sonOfHakkarAssistant.findAnyAliveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        if (sonOfHakkar->IsInCombatWith(this->bot) == true || this->bot->IsInCombatWith(sonOfHakkar) == true)
        {
            return false;
        }

        const HakkarTheSoulflayerMemory& hakkarTheSoulflayerMemory = hakkarTheSoulflayerAssistant.getMemory();

        const double bloodSiphonCooldown = double(HakkarTheSoulflayerEnum::BLOOD_SIPHON_COOL_DOWN);
        const double pullTimeAfterBloodSiphon = bloodSiphonCooldown - double(SonOfHakkarEnum::PULL_TIME_BEFORE_BLOOD_SIPHON);
        const double timeSinceLastBloodSiphon = hakkarTheSoulflayerMemory.getBloodSiphonWatch().getElapsedSeconds();

        if (timeSinceLastBloodSiphon <= pullTimeAfterBloodSiphon)
        {
            return false;
        }

        const float distanceToSonOfHakkar = sonOfHakkarAssistant.getDistanceToActiveSonOfHakkar(*this->bot);
        const Creature* const activeSonOfHakkar = sonOfHakkarMemory.getActiveSonOfHakkar();

        if (activeSonOfHakkar != nullptr && distanceToSonOfHakkar < 25.0f)
        {
            return false;
        }

        return true;
    }
};
