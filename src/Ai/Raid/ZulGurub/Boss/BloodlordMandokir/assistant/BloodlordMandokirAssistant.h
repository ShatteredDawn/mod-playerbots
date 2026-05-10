#pragma once

#include "Unit.h"
#include "Player.h"
#include "Spell.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "Cell.h"
#include "CellImpl.h"

#include "../definition/enum/BloodlordMandokirEnum.h"


#include "PlayerbotAIConfig.h"

class BloodlordMandokirAssistant
{
public:
    BloodlordMandokirAssistant() = default;
    ~BloodlordMandokirAssistant() = default;

    BloodlordMandokirAssistant(const BloodlordMandokirAssistant&) = default;
    BloodlordMandokirAssistant& operator=(const BloodlordMandokirAssistant&) = default;

    BloodlordMandokirAssistant(BloodlordMandokirAssistant&&) = default;
    BloodlordMandokirAssistant& operator=(BloodlordMandokirAssistant&&) = default;

    [[nodiscard]] bool isInCombatWithBloodlordMandokir(Player& bot) const noexcept
    {
        const Unit* const BloodlordMandokir = this->findActiveBoss(bot);

        if (BloodlordMandokir == nullptr)
        {
            return false;
        }

        return bot.IsInCombatWith(BloodlordMandokir) || BloodlordMandokir->IsInCombatWith(&bot);
    }

    [[nodiscard]] Creature* findActiveBoss(Player& bot) const noexcept
    {
        return bot.FindNearestCreature(uint32_t(BloodlordMandokirEnum::ENTRY), PlayerbotAIConfig::instance().sightDistance, true);
    }

    [[nodiscard]] bool isAtSafeDistanceFromBloodlordMandokir(Player& bot) const noexcept
    {
        const Unit* const BloodlordMandokir = this->findActiveBoss(bot);

        if (BloodlordMandokir == nullptr)
        {
            return true;
        }

        const float distanceToBloodlordMandokir = bot.GetExactDist2d(BloodlordMandokir->GetPositionX(), BloodlordMandokir->GetPositionY());

        return distanceToBloodlordMandokir > float(BloodlordMandokirEnum::SPELL_WHIRLWIND_SAFE_DISTANCE);
    };
};
