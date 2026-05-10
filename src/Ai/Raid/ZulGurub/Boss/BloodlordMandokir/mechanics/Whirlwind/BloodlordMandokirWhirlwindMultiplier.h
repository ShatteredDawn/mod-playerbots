#pragma once

#include "AiObjectContext.h"
#include "BloodlordMandokirWhirlwindAction.h"
#include "MovementActions.h"
#include "Multiplier.h"
#include "PlayerbotAI.h"
#include "Value.h"

#include "GenericSpellActions.h"
#include "raid/leader/RaidLeaderRegistry.h"


class BloodlordMandokirWhirlwindMultiplier : public Multiplier
{
public:
    BloodlordMandokirWhirlwindMultiplier(PlayerbotAI* botAI) : Multiplier(botAI, "bloodlord mandokir whirlwind") {}

    float GetValue(Action& action) override
    {
        if (this->bot == nullptr)
        {
            return 0.0f;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const BloodlordMandokirAssistant& bloodlordMandokirAssistant = raidLeader.getBloodlordMandokirAssistant();

		const Creature* const bloodlordMandokir = bloodlordMandokirAssistant.findActiveBoss(*this->bot);

		if (bloodlordMandokir == nullptr)
		{
			return 1.0f;
		}

		if (bloodlordMandokir->HasAura(uint32_t(BloodlordMandokirEnum::SPELL_WHIRLWIND)) == false)
		{
			return 1.0f;
		}

        if (bloodlordMandokirAssistant.isAtSafeDistanceFromBloodlordMandokir(*this->bot) == true)
        {
            return 1.0f;
        }

        const BloodlordMandokirWhirlwindAction* const movementAction = dynamic_cast<const BloodlordMandokirWhirlwindAction*>(&action);

        if (movementAction != nullptr)
        {
            return 1.0f;
        }

        return 0.0f;
    }
};
