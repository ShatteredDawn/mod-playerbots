#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestVenoxisPhase1HolyWrathAction : public MovementAction
{
public:
    HighPriestVenoxisPhase1HolyWrathAction(
        PlayerbotAI* botAI,
        const std::string name = "high priest venoxis phase 1 holy wrath"
    ) : MovementAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, this->bot->GetMapId());
        const HighPriestVenoxisAssistant& highPriestVenoxisAssistant = raidLeader.getHighPriestVenoxisAssistant();
        Unit* const venoxis = highPriestVenoxisAssistant.findActiveBoss(*this->bot);

        if (venoxis == nullptr)
        {
            return false;
        }

        const float safeDistance = float(HighPriestVenoxisEnum::PHASE_1_SAFE_DISTANCE) - this->bot->GetDistance2d(venoxis);

        if (safeDistance <= 0.0f)
        {
            return false;
        }

        this->MoveAway(venoxis, safeDistance);

        return true;
    }
};
