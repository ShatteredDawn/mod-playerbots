#pragma once

#include "Creature.h"
#include "Player.h"
#include "PlayerScript.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerPlayerScript : public PlayerScript
{
public:
    HakkarTheSoulflayerPlayerScript() : PlayerScript("HakkarTheSoulflayerPlayerScript") {}

    void OnPlayerKilledByCreature(Creature*, Player* player) override
    {
        if (player == nullptr)
        {
            return;
        }

        const uint32_t mapId = player->GetMapId();

        if (mapId != MAP_ZUL_GURUB)
        {
            return;
        }

        const uint32_t instanceId = player->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, player->GetMapId());
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();

        const SonOfHakkarMemory& memory = sonOfHakkarAssistant.getMemory();

        if (player == memory.getSonOfHakkarDesignatedPuller())
        {
            sonOfHakkarAssistant.initialiseDesignatedPuller(*player);
        }
    }

};