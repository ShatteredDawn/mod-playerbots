#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1RazzashiCobrasPositioningAction : public AttackAction
{
public:
    HighPriestVenoxisPhase1RazzashiCobrasPositioningAction(
        PlayerbotAI* botAI,
        const std::string name = "high priest venoxis phase 1 razzashi cobras positioning"
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

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        const std::vector<Unit*> razzashiCobras = highPriestVenoxisAssistant.findRazzashiCobras(*this->bot);
        for (Unit* const cobra : razzashiCobras)
        {
            if (cobra == nullptr)
            {
                return false;
            }

            Unit* const target = cobra->GetVictim();

            if (target == nullptr)
            {
                this->Attack(cobra);

                return false;
            }

            Player* const playerTarget = dynamic_cast<Player*>(target);

            if (playerTarget == nullptr)
            {
                this->Attack(cobra);

                return false;
            }

            const bool isProperlyTanked = this->botAI->IsTank(playerTarget) == true && this->botAI->IsMainTank(playerTarget) == false;

            if (isProperlyTanked == false)
            {
                this->Attack(cobra);

                return false;
            }
        }

        const Position cobrasIdealPosition = highPriestVenoxisAssistant.getRazzashiCobrasPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(cobrasIdealPosition.GetPositionX(), cobrasIdealPosition.GetPositionY());
        const float maxDistance = highPriestVenoxisAssistant.getRazzashiCobrasMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        const float botPositionX = this->bot->GetPositionX();
        const float botPositionY = this->bot->GetPositionY();
        const float dX = cobrasIdealPosition.GetPositionX() - botPositionX;
        const float dY = cobrasIdealPosition.GetPositionY() - botPositionY;
        const float moveX = botPositionX + (dX / distanceToIdealPosition) * maxDistance;
        const float moveY = botPositionY + (dY / distanceToIdealPosition) * maxDistance;

        return this->MoveTo(
            MAP_ZUL_GURUB,
            moveX,
            moveY,
            cobrasIdealPosition.GetPositionZ(),
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
