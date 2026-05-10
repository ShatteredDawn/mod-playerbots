#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"
#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1VenoxisPositioningAction : public AttackAction
{
public:
    HighPriestVenoxisPhase1VenoxisPositioningAction(
        PlayerbotAI* botAI,
        const std::string name = "high priest venoxis phase 1 venoxis positioning"
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
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        Unit* const boss = highPriestVenoxisAssistant.findActiveBoss(*this->bot);

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

        const Position bossIdealPosition = highPriestVenoxisAssistant.getVenoxisPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        const float maxDistance = highPriestVenoxisAssistant.getVenoxisMaxPositionDistance();

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
