#pragma once

#include "Spell.h"
#include "AiObjectContext.h"
#include "MovementActions.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class BloodlordMandokirWhirlwindAction : public MovementAction
{
public:
    BloodlordMandokirWhirlwindAction(
        PlayerbotAI* botAI,
        const std::string name = "bloodlord mandokir whirlwind"
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
        const BloodlordMandokirAssistant& bloodlordMandokirAssistant = raidLeader.getBloodlordMandokirAssistant();
        Unit* const bloodlordMandokir = bloodlordMandokirAssistant.findActiveBoss(*this->bot);

        if (bloodlordMandokir == nullptr)
        {
            return false;
        }

        const float safeDistance = float(BloodlordMandokirEnum::SPELL_WHIRLWIND_SAFE_DISTANCE) - this->bot->GetDistance2d(bloodlordMandokir);

        if (safeDistance <= 0.0f)
        {
            return false;
        }

        this->MoveAway(bloodlordMandokir, safeDistance);

        return true;
    }
};
