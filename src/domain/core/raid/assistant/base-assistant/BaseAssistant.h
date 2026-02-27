#pragma once

#include "Creature.h"
#include "Player.h"

class BaseAssistant
{
public:
    BaseAssistant() = default;
    virtual ~BaseAssistant() = default;


protected:

    [[nodiscard]] Creature* findActiveEnemyByDBGUID(Player& player, uint32_t dbGuid, uint32_t entry) const
    {
        Map* const map = player.GetMap();

        if (map == nullptr)
        {
            return nullptr;
        }

        const std::unordered_multimap<ObjectGuid::LowType, Creature*> store = map->GetCreatureBySpawnIdStore();
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator it = store.find(dbGuid);
        const std::unordered_multimap<ObjectGuid::LowType, Creature*>::const_iterator end = store.end();

        if (it == end)
        {
            return nullptr;
        }

        Creature* enemy = it->second;

        if (enemy == nullptr)
        {
            return nullptr;
        }

        if (!enemy->IsAlive())
        {
            return nullptr;
        }

        if (enemy->GetEntry() != entry)
        {
            return nullptr;
        }

        return enemy;
    }
};
