#pragma once

#include "AiObjectContext.h"
#include "MovementActions.h"
#include "Multiplier.h"
#include "PlayerbotAI.h"
#include "Value.h"

#include "GenericSpellActions.h"
#include "raid/leader/RaidLeaderRegistry.h"


class GrilekTheWandererAvatarMultiplier : public Multiplier
{
public:
    GrilekTheWandererAvatarMultiplier(PlayerbotAI* botAI) : Multiplier(botAI, "grilek the wanderer avatar") {}

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
        const GrilekTheWandererAssistant& grilekTheWandererAssistant = raidLeader.getGrilekTheWandererAssistant();

		const Creature* const grilekTheWanderer = grilekTheWandererAssistant.findActiveBoss(*this->bot);

		if (grilekTheWanderer == nullptr)
		{
			return 1.0f;
		}

		if (grilekTheWanderer->HasAura(uint32_t(GrilekTheWandererEnum::SPELL_AVATAR)) == false)
		{
			return 1.0f;
		}

        if (grilekTheWandererAssistant.isAtSafeDistanceFromGrilekTheWanderer(*this->bot) == true)
        {
            return 1.0f;
        }

        const MovementAction* const movementAction = dynamic_cast<const MovementAction*>(&action);

        if (movementAction != nullptr)
        {
            return 1.0f;
        }

        return 0.0f;
    }
};
