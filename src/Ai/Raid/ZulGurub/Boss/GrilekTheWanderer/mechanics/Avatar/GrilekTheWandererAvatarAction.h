#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class GrilekTheWandererAvatarAction : public MovementAction
{
public:
    GrilekTheWandererAvatarAction(
        PlayerbotAI* botAI,
        const std::string name = "grilek the wanderer avatar"
    ) : MovementAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const GrilekTheWandererAssistant& grilekTheWandererAssistant = raidLeader.getGrilekTheWandererAssistant();
        Unit* const grilekTheWanderer = grilekTheWandererAssistant.findActiveBoss(*this->bot);

        if (grilekTheWanderer == nullptr)
        {
            return false;
        }

        const float safeDistance = float(GrilekTheWandererEnum::AVATAR_SAFE_DISTANCE) - this->bot->GetDistance2d(grilekTheWanderer);

        if (safeDistance <= 0.0f)
        {
            return false;
        }

        this->MoveAway(grilekTheWanderer, safeDistance);

        return true;
    }
};
