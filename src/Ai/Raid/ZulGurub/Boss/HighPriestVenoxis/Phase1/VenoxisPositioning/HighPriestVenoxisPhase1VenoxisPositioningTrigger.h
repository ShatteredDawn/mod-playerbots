#pragma once

#include "AiObjectContext.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "../../facade/HighPriestVenoxisFacade.h"

class HighPriestVenoxisPhase1VenoxisPositioningTrigger : public Trigger
{
public:
    HighPriestVenoxisPhase1VenoxisPositioningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "high priest venoxis phase 1 venoxis positioning") {}

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

        if (this->botAI->IsMainTank(this->bot) == false)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsInCombatWithVenoxis(*this->bot) == false)
        {
            return false;
        }

        if (HighPriestVenoxisFacade::IsInPhase1(*this->bot) == false)
        {
            return false;
        }

        const Unit* const boss = HighPriestVenoxisFacade::FindActiveBoss(*this->bot);

        if (boss == nullptr)
        {
            return false;
        }

        const Position bossIdealPosition = HighPriestVenoxisFacade::GetVenoxisPosition();
        const float distanceToIdealPosition = this->bot->GetExactDist2d(bossIdealPosition.GetPositionX(), bossIdealPosition.GetPositionY());
        static constexpr float maxDistance = HighPriestVenoxisFacade::GetVenoxisMaxPositionDistance();

        if (distanceToIdealPosition < maxDistance)
        {
            return false;
        }

        return true;
    }
};
