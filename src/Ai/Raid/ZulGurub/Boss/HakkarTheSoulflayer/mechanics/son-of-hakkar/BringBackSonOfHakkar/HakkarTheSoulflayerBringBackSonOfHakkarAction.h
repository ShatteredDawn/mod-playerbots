#pragma once

#include "HunterActions.h"
#include "MageActions.h"
#include "PaladinActions.h"
#include "PriestActions.h"
#include "RogueActions.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"


class HakkarTheSoulflayerBringBackSonOfHakkarAction : public MovementAction
{
public:
    HakkarTheSoulflayerBringBackSonOfHakkarAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer bring back son of hakkar"
    ) : MovementAction(botAI, name) {}

    bool Execute(Event) override
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
        SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();
        const SonOfHakkarMemory& memory = sonOfHakkarAssistant.getMemory();

        Creature* const sonOfHakkar = memory.getActiveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        const Position hakkarPosition = hakkarTheSoulflayerAssistant.getHakkarPosition();

        const float distanceToIdealPosition = this->bot->GetExactDist(hakkarPosition.GetPositionX(), hakkarPosition.GetPositionY(), hakkarPosition.GetPositionZ());
        const float maxDistance = hakkarTheSoulflayerAssistant.getHakkarMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        const float sonOfHakkarPositionX = sonOfHakkar->GetPositionX();
        const float sonOfHakkarPositionY = sonOfHakkar->GetPositionY();
        const float dX = hakkarPosition.GetPositionX() - sonOfHakkarPositionX;
        const float dY = hakkarPosition.GetPositionY() - sonOfHakkarPositionY;
        const float moveX = sonOfHakkarPositionX + (dX / distanceToIdealPosition) * maxDistance;
        const float moveY = sonOfHakkarPositionY + (dY / distanceToIdealPosition) * maxDistance;

        return this->MoveTo(
            MAP_ZUL_GURUB,
            moveX,
            moveY,
            hakkarPosition.GetPositionZ(),
            false,
            false,
            false,
            false,
            MovementPriority::MOVEMENT_FORCED,
            true,
            false
        );

    }
};
