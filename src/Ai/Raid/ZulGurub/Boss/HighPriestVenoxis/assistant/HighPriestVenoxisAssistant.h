#pragma once

#include "Unit.h"
#include "Player.h"
#include "Spell.h"
#include "Position.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "Cell.h"
#include "CellImpl.h"


#include "PlayerbotAIConfig.h"

#include "../definition/enum/HighPriestVenoxisEnum.h"
#include "../definition/enum/RazzashiCobraEnum.h"


class HighPriestVenoxisAssistant
{
public:
    HighPriestVenoxisAssistant() = default;
    ~HighPriestVenoxisAssistant() = default;

    HighPriestVenoxisAssistant(const HighPriestVenoxisAssistant&) = default;
    HighPriestVenoxisAssistant& operator=(const HighPriestVenoxisAssistant&) = default;

    HighPriestVenoxisAssistant(HighPriestVenoxisAssistant&&) = default;
    HighPriestVenoxisAssistant& operator=(HighPriestVenoxisAssistant&&) = default;

    [[nodiscard]] const Position getVenoxisPosition() const noexcept
    {
        return Position{
            -12026.866f,
            -1665.5562f,
            33.59f,
        };
    }

    [[nodiscard]] constexpr float getVenoxisMaxPositionDistance() const noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] bool isInCombatWithVenoxis(Player& bot) const noexcept
    {
        const Unit* const venoxis = this->findActiveBoss(bot);

        if (venoxis == nullptr)
        {
            return false;
        }

        return bot.IsInCombatWith(venoxis) || venoxis->IsInCombatWith(&bot);
    }

    [[nodiscard]] const Position getRazzashiCobrasPosition() const noexcept
    {
        return Position{
            -11991.636f,
            -1698.4347f,
            32.284f,
        };
    }

    [[nodiscard]] constexpr float getRazzashiCobrasMaxPositionDistance() const noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] Unit* findActiveBoss(Player& bot) const noexcept
    {
        if (!bot.IsInCombat())
        {
            return nullptr;
        }

        return bot.FindNearestCreature(uint32_t(HighPriestVenoxisEnum::ENTRY), PlayerbotAIConfig::instance().sightDistance, true);
    };

    [[nodiscard]] std::vector<Unit*> findRazzashiCobras(Player& bot) const noexcept
    {
        if (!bot.IsInCombat())
        {
            return {};
        }

        std::list<Unit*> nearbyUnits{};

        Acore::AnyUnitInObjectRangeCheck unitChecker{&bot, PlayerbotAIConfig::instance().sightDistance};
        Acore::UnitListSearcher<Acore::AnyUnitInObjectRangeCheck> searcher{&bot, nearbyUnits, unitChecker};
        Cell::VisitObjects(&bot, searcher, PlayerbotAIConfig::instance().sightDistance);

        std::vector<Unit*> results{};

        for (Unit* const unit : nearbyUnits)
        {
            if (unit->GetEntry() != uint32_t(RazzashiCobraEnum::ENTRY))
            {
                continue;
            }

            if (unit->IsAlive() == false)
            {
                continue;
            }

            results.emplace_back(unit);
        }

        std::sort(
            results.begin(),
            results.end(),
            [](Unit* cobraA, Unit* cobraB)
            {
                return cobraA->GetGUID().GetHigh() < cobraB->GetGUID().GetHigh();
            }
        );

        return results;
    };

    [[nodiscard]] bool isAtSafeDistanceFromVenoxis(Player& bot) const noexcept
    {
        const Unit* const venoxis = this->findActiveBoss(bot);

        if (venoxis == nullptr)
        {
            return true;
        }

        const float distanceToVenoxis = bot.GetExactDist2d(venoxis->GetPositionX(), venoxis->GetPositionY());

        return distanceToVenoxis > float(HighPriestVenoxisEnum::PHASE_1_SAFE_DISTANCE);
    };

    [[nodiscard]] bool isInPhase1(Player& bot) const noexcept
    {
        const Unit* const unit = this->findActiveBoss(bot);

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

        return unit->GetHealthPct() > float(HighPriestVenoxisEnum::PHASE_2_THRESHOLD);
    };
};
