#pragma once

#include "Pet.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "raid/leader/RaidLeaderRegistry.h"

class HighPriestThekalHealthBalanceAction : public AttackAction
{
public:
    HighPriestThekalHealthBalanceAction(
        PlayerbotAI* botAI,
        const std::string name = "high priest thekal phase 1 health balance"
    ) : AttackAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        HighPriestThekalAssistant& highPriestThekalAssistant = raidLeader.getHighPriestThekalAssistant();
        std::array<Creature*, 3> bosses = highPriestThekalAssistant.getActiveBossesSortedByHealth(*this->bot);

        Value<Unit*>* const rtiTargetValue = this->context->GetValue<Unit*>("rti target");

        if (rtiTargetValue == nullptr)
        {
            return false;
        }

        MarkTargetWithSkull(this->bot, bosses[0]);

        rtiTargetValue->Set(bosses[0]);

        return true;
    }
};
