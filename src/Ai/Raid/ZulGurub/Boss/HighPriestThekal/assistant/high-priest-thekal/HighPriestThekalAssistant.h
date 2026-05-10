#pragma once

#include <algorithm>
#include <array>

#include "HighPriestThekal/definition/enum/ZealotLorkhanEnum.h"
#include "HighPriestThekal/definition/enum/ZealotZathEnum.h"
#include "Unit.h"
#include "Player.h"
#include "Spell.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "../../definition/enum/HighPriestThekalEnum.h"
#include "domain/core/raid/assistant/base-assistant/BaseAssistant.h"


class HighPriestThekalAssistant : public BaseAssistant
{
public:
    HighPriestThekalAssistant() = default;
    ~HighPriestThekalAssistant() = default;

    HighPriestThekalAssistant(const HighPriestThekalAssistant&) = default;
    HighPriestThekalAssistant& operator=(const HighPriestThekalAssistant&) = default;

    HighPriestThekalAssistant(HighPriestThekalAssistant&&) = default;
    HighPriestThekalAssistant& operator=(HighPriestThekalAssistant&&) = default;

    [[nodiscard]] bool isInCombatWithHighPriestThekal(Player& bot) const noexcept
    {
        const Creature* const highPriestThekal = this->findActiveEnemyByDBGUID(bot, uint32_t(HighPriestThekalEnum::GUID), uint32_t(HighPriestThekalEnum::ENTRY));

        if (highPriestThekal == nullptr)
        {
            return false;
        }

        return bot.IsInCombatWith(highPriestThekal) || highPriestThekal->IsInCombatWith(&bot);
    }

    [[nodiscard]] std::array<Creature*, 3> findActiveBosses(Player& bot) const noexcept
    {
        Creature* const highPriestThekal = this->findActiveEnemyByDBGUID(bot, uint32_t(HighPriestThekalEnum::GUID), uint32_t(HighPriestThekalEnum::ENTRY));
        Creature* const zealotLorkhan = this->findActiveEnemyByDBGUID(bot, uint32_t(ZealotLorkhanEnum::GUID), uint32_t(ZealotLorkhanEnum::ENTRY));
        Creature* const zealotZath = this->findActiveEnemyByDBGUID(bot, uint32_t(ZealotZathEnum::GUID), uint32_t(ZealotZathEnum::ENTRY));

        return { highPriestThekal, zealotLorkhan, zealotZath };
    }

    [[nodiscard]] std::array<Creature*, 3> getActiveBossesSortedByHealth(Player& bot) const noexcept
    {
        std::array<Creature*, 3> bosses = this->findActiveBosses(bot);

        if (bosses[0] == nullptr || bosses[1] == nullptr || bosses[2] == nullptr)
        {
            return bosses;
        }

        if (bosses[0]->GetHealth() < bosses[1]->GetHealth())
        {
            std::swap(bosses[0], bosses[1]);
        }

        if (bosses[1]->GetHealth() < bosses[2]->GetHealth())
        {
            std::swap(bosses[1], bosses[2]);
        }

        if (bosses[0]->GetHealth() < bosses[1]->GetHealth())
        {
            std::swap(bosses[0], bosses[1]);
        }

        return bosses;
    }

    [[nodiscard]] bool isInPhase1(Player& bot) const noexcept
    {
        const Creature* const unit = this->findActiveEnemyByDBGUID(bot, uint32_t(HighPriestThekalEnum::GUID), uint32_t(HighPriestThekalEnum::ENTRY));

        if (unit == nullptr)
        {
            return false;
        }

        if (unit->GetMapId() != MAP_ZUL_GURUB)
        {
            return false;
        }

        if (!unit->IsInCombat())
        {
            return false;
        }

        return unit->HasAura(uint32_t(HighPriestThekalEnum::SPELL_TIGER_FORM)) == false;
    };
};
