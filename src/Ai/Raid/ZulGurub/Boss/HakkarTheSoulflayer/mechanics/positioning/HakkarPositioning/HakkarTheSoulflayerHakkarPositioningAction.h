#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"
#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerHakkarPositioningAction : public AttackAction
{
public:
    HakkarTheSoulflayerHakkarPositioningAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer hakkar positioning"
    ) : AttackAction(botAI, name) {}

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

        if (this->botAI->IsMainTank(this->bot) == false)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HakkarTheSoulflayerAssistant& hakkarTheSoulflayerAssistant = raidLeader.getHakkarTheSoulflayerAssistant();

        Unit* const boss = hakkarTheSoulflayerAssistant.findActiveBoss(*this->bot);

        if (boss == nullptr)
        {
            return false;
        }

        if (this->bot->GetVictim() != boss)
        {
            this->Attack(boss);

            return false;
        }

        if (boss->GetTarget() != this->bot->GetGUID())
        {
            this->Attack(boss);

            return false;
        }

        const Position bossIdealPosition = hakkarTheSoulflayerAssistant.getHakkarPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        const float maxDistance = hakkarTheSoulflayerAssistant.getHakkarMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        const float botPositionX = this->bot->GetPositionX();
        const float botPositionY = this->bot->GetPositionY();
        const float dX = bossIdealPosition.GetPositionX() - botPositionX;
        const float dY = bossIdealPosition.GetPositionY() - botPositionY;
        const float moveX = botPositionX + (dX / distanceToIdealPosition) * maxDistance;
        const float moveY = botPositionY + (dY / distanceToIdealPosition) * maxDistance;

        return this->MoveTo(
            MAP_ZUL_GURUB,
            moveX,
            moveY,
            bossIdealPosition.GetPositionZ(),
            false,
            false,
            false,
            false,
            MovementPriority::MOVEMENT_COMBAT,
            true,
            true
        );

        return true;
    }
};
