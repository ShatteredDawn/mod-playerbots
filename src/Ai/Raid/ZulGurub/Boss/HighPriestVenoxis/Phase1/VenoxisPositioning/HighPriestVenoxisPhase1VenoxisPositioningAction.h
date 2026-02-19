#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "../../facade/HighPriestVenoxisFacade.h"

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

        Unit* const boss = HighPriestVenoxisFacade::FindActiveBoss(*this->bot);

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

        const Position bossIdealPosition = HighPriestVenoxisFacade::GetVenoxisPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        static constexpr float maxDistance = HighPriestVenoxisFacade::GetVenoxisMaxPositionDistance();

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

        return MoveTo(
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
