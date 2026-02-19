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
#include "../../../facade/ZulGurubFacade.h"


class HighPriestVenoxisFacade
{
public:
    [[nodiscard]] static Position GetVenoxisPosition() noexcept
    {
        return Position{
            -12026.866f,
            -1665.5562f,
            33.59f,
        };
    }

    [[nodiscard]] static constexpr float GetVenoxisMaxPositionDistance() noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] static bool IsInCombatWithVenoxis(Player& bot) noexcept
    {
        const Unit* const venoxis = HighPriestVenoxisFacade::FindActiveBoss(bot);

        if (venoxis == nullptr)
        {
            return false;
        }

        // This method seems to not be working properly.
        // return bot.IsInCombatWith(venoxis);
        return bot.IsInCombat() && venoxis->IsInCombat();
    }

    [[nodiscard]] static Position GetRangedPosition() noexcept
    {
        return Position{
            -11985.107f,
            -1672.6897f,
            32.31f,
        };
    }

    [[nodiscard]] static constexpr float GetRangedMaxPositionDistance() noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] static Position GetRazzashiCobrasPosition() noexcept
    {
        return Position{
            -11991.636f,
            -1698.4347f,
            32.284f,
        };
    }

    [[nodiscard]] static constexpr float GetRazzashiCobrasMaxPositionDistance() noexcept
    {
        return 4.0f;
    }

    [[nodiscard]] static Unit* FindActiveBoss(Player& bot) noexcept
    {
        if (ZulGurubFacade::IsInInstance(bot) == false)
        {
            return nullptr;
        }

        if (!bot.IsInCombat())
        {
            return nullptr;
        }

        return bot.FindNearestCreature(uint32_t(HighPriestVenoxisEnum::ENTRY), PlayerbotAIConfig::instance().sightDistance, true);
    };

    [[nodiscard]] static std::vector<Unit*> FindRazzashiCobras(Player& bot) noexcept
    {
        if (ZulGurubFacade::IsInInstance(bot) == false)
        {
            return {};
        }

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

    [[nodiscard]] static bool IsAtSafeDistanceFromVenoxis(Player& bot) noexcept
    {
        const Unit* const venoxis = FindActiveBoss(bot);

        if (venoxis == nullptr)
        {
            return true;
        }

        const float distanceToVenoxis = bot.GetExactDist2d(venoxis->GetPositionX(), venoxis->GetPositionY());

        return distanceToVenoxis > float(HighPriestVenoxisEnum::PHASE_1_SAFE_DISTANCE);
    };

    [[nodiscard]] static bool IsInPhase1(Player& bot) noexcept
    {
        const Unit* const unit = HighPriestVenoxisFacade::FindActiveBoss(bot);

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

private:
    HighPriestVenoxisFacade() = delete;
    ~HighPriestVenoxisFacade() = delete;

    HighPriestVenoxisFacade(const HighPriestVenoxisFacade&) = delete;
    HighPriestVenoxisFacade& operator=(const HighPriestVenoxisFacade&) = delete;

    HighPriestVenoxisFacade(HighPriestVenoxisFacade&&) = delete;
    HighPriestVenoxisFacade& operator=(HighPriestVenoxisFacade&&) = delete;
};
