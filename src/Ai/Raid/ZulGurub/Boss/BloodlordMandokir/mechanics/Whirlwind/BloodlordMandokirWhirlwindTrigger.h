#pragma once

#include "AiObjectContext.h"
#include "BloodlordMandokir/assistant/BloodlordMandokirAssistant.h"
#include "PlayerbotAI.h"
#include "Trigger.h"

#include "raid/leader/RaidLeaderRegistry.h"

class BloodlordMandokirWhirlwindTrigger : public Trigger
{
public:
    BloodlordMandokirWhirlwindTrigger(PlayerbotAI* botAI) : Trigger(botAI, "bloodlord mandokir whirlwind") {}

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
        const BloodlordMandokirAssistant& bloodlordMandokirAssistant = raidLeader.getBloodlordMandokirAssistant();

        if (bloodlordMandokirAssistant.isInCombatWithBloodlordMandokir(*this->bot) == false)
        {
            return false;
        }

		const Creature* const bloodlordMandokir = bloodlordMandokirAssistant.findActiveBoss(*this->bot);

		if (bloodlordMandokir == nullptr)
		{
			return false;
		}

		if (bloodlordMandokir->HasAura(uint32_t(BloodlordMandokirEnum::SPELL_WHIRLWIND)) == false)
		{
			return false;
		}

        if (bloodlordMandokirAssistant.isAtSafeDistanceFromBloodlordMandokir(*this->bot) == true)
        {
            return false;
        }

        return true;
    }
};
