#pragma once

#include "AiObjectContext.h"
#include "MovementActions.h"
#include "Multiplier.h"
#include "PlayerbotAI.h"
#include "Value.h"

// #include "HighPriestVenoxisPhase1HolyWrathAction.h"
// #include "../VenoxisPositioning/HighPriestVenoxisPhase1VenoxisPositioningAction.h"
// #include "../RazzashiCobrasPositioning/HighPriestVenoxisPhase1RazzashiCobrasPositioningAction.h"
#include "GenericSpellActions.h"
#include "raid/leader/RaidLeaderRegistry.h"


class HighPriestVenoxisPhase1HolyWrathMultiplier : public Multiplier
{
public:
    HighPriestVenoxisPhase1HolyWrathMultiplier(PlayerbotAI* botAI) : Multiplier(botAI, "high priest venoxis phase 1 holy wrath") {}

    float GetValue(Action& action) override
    {
        if (this->bot == nullptr)
        {
            return 0.0f;
        }

        if (this->botAI->IsTank(this->bot) == true)
        {
            return 1.0f;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();

        if (highPriestVenoxisAssistant.isInPhase1(*this->bot) == false)
        {
            return 1.0f;
        }

        if (highPriestVenoxisAssistant.isAtSafeDistanceFromVenoxis(*this->bot) == true)
        {
            return 1.0f;
        }

        const std::vector<Unit*> cobras = highPriestVenoxisAssistant.findRazzashiCobras(*this->bot);

        if (!cobras.empty())
        {
            return 1.0f;
        }

        // const HighPriestVenoxisPhase1HolyWrathAction* const holyWrathAction = dynamic_cast<const HighPriestVenoxisPhase1HolyWrathAction*>(&action);

        // if (holyWrathAction != nullptr)
        // {
        //     return 1.0f;
        // }

        // const HighPriestVenoxisPhase1VenoxisPositioningAction* const venoxisPositioningAction = dynamic_cast<const HighPriestVenoxisPhase1VenoxisPositioningAction*>(&action);

        // if (venoxisPositioningAction != nullptr)
        // {
        //     return 1.0f;
        // }

        // const HighPriestVenoxisPhase1RazzashiCobrasPositioningAction* const razzashiCobrasPositioningAction = dynamic_cast<const HighPriestVenoxisPhase1RazzashiCobrasPositioningAction*>(&action);

        // if (razzashiCobrasPositioningAction != nullptr)
        // {
        //     return 1.0f;
        // }

        const CastSpellAction* const castSpellAction = dynamic_cast<const CastSpellAction*>(&action);

        if (castSpellAction != nullptr)
        {
            return 1.0f;
        }

        return 0.0f;
    }
};
