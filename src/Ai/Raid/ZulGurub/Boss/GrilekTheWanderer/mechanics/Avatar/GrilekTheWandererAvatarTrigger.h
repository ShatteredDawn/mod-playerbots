#pragma once

#include "AiObjectContext.h"
#include "GrilekTheWanderer/assistant/GrilekTheWandererAssistant.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "raid/leader/RaidLeaderRegistry.h"

class GrilekTheWandererAvatarTrigger : public Trigger
{
public:
    GrilekTheWandererAvatarTrigger(PlayerbotAI* botAI) : Trigger(botAI, "grilek the wanderer avatar") {}

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

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const GrilekTheWandererAssistant& grilekTheWandererAssistant = raidLeader.getGrilekTheWandererAssistant();

        if (grilekTheWandererAssistant.isInCombatWithGrilekTheWanderer(*this->bot) == false)
        {
            return false;
        }

		const Creature* const grilekTheWanderer = grilekTheWandererAssistant.findActiveBoss(*this->bot);

		if (grilekTheWanderer == nullptr)
		{
			return false;
		}

		if (grilekTheWanderer->HasAura(uint32_t(GrilekTheWandererEnum::SPELL_AVATAR)) == false)
		{
			return false;
		}

        if (grilekTheWandererAssistant.isAtSafeDistanceFromGrilekTheWanderer(*this->bot) == true)
        {
            return false;
        }

        return true;
    }
};
