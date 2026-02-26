#pragma once

#include "Unit.h"
#include "Player.h"
#include "Spell.h"

#include "GridNotifiers.h"
// Necessary due to a poor implementation by AC.
#include "GridNotifiersImpl.h"

#include "Cell.h"
#include "CellImpl.h"

#include "../definition/enum/GrilekTheWandererEnum.h"
#include <unordered_map>


class GrilekTheWandererAssistant
{
public:
    GrilekTheWandererAssistant() = default;
    ~GrilekTheWandererAssistant() = default;

    GrilekTheWandererAssistant(const GrilekTheWandererAssistant&) = default;
    GrilekTheWandererAssistant& operator=(const GrilekTheWandererAssistant&) = default;

    GrilekTheWandererAssistant(GrilekTheWandererAssistant&&) = default;
    GrilekTheWandererAssistant& operator=(GrilekTheWandererAssistant&&) = default;

    [[nodiscard]] bool isInCombatWithGrilekTheWanderer(Player& bot) const noexcept
    {
        const Unit* const grilekTheWanderer = this->findActiveBoss(bot);

        if (grilekTheWanderer == nullptr)
        {
            return false;
        }

        return bot.IsInCombatWith(grilekTheWanderer) || grilekTheWanderer->IsInCombatWith(&bot);
    }

    [[nodiscard]] Creature* findActiveBoss(Player& bot) const noexcept
    {
        if (!bot.IsInCombat())
        {
            return nullptr;
        }

        Map* map = bot.GetMap();

        if (map == nullptr)
        {
            return nullptr;
        }

        const std::unordered_multimap<ObjectGuid::LowType, Creature*> store = map->GetCreatureBySpawnIdStore();
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator it = store.find(uint32_t(GrilekTheWandererEnum::ENTRY));
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator end = store.end();

        if (it == end)
        {
            return nullptr;
        }

        Creature* grilekTheWanderer = it->second;

        if (grilekTheWanderer == nullptr)
        {
            return nullptr;
        }

        if (!grilekTheWanderer->IsAlive())
        {
            return nullptr;
        }

         if (grilekTheWanderer->GetEntry() != uint32_t(GrilekTheWandererEnum::ENTRY))
        {
            return nullptr;
        }

        return grilekTheWanderer;
    };

    [[nodiscard]] bool isAtSafeDistanceFromGrilekTheWanderer(Player& bot) const noexcept
    {
        const Unit* const grilekTheWanderer = this->findActiveBoss(bot);

        if (grilekTheWanderer == nullptr)
        {
            return true;
        }

        const float distanceToGrilekTheWanderer = bot.GetExactDist2d(grilekTheWanderer->GetPositionX(), grilekTheWanderer->GetPositionY());

        return distanceToGrilekTheWanderer > float(GrilekTheWandererEnum::AVATAR_SAFE_DISTANCE);
    };
};
