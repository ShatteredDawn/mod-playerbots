#pragma once

#include "Group.h"
#include "PlayerbotAI.h"
#include "Player.h"
#include "Spell.h"

#include "PlayerbotMgr.h"
#include "./memory/RaidTankAssistantMemory.h"

class RaidTankAssistant
{
private:
    RaidTankAssistantMemory memory{};

public:
    RaidTankAssistant() = default;
    ~RaidTankAssistant() = default;

    RaidTankAssistant(const RaidTankAssistant&) = default;
    RaidTankAssistant& operator=(const RaidTankAssistant&) = default;

    RaidTankAssistant(RaidTankAssistant&&) = default;
    RaidTankAssistant& operator=(RaidTankAssistant&&) = default;

    void initialiseTanks(Player& player) noexcept
    {
        this->memory.clear();

        const std::vector<Player*> tanks = this->getTanksByItemLevel(player);

        for (Player* tank : tanks)
        {
            this->memory.add(tank);
        }
    }

    [[nodiscard]] const RaidTankAssistantMemory& getMemory() const noexcept
    {
        return this->memory;
    }

protected:
    [[nodiscard]] const std::vector<Player*> getTanksByItemLevel(Player& player) const noexcept
    {
        const Group* const group = player.GetGroup();

        if (group == nullptr)
        {
            return { &player };
        }

        const GroupReference* member = group->GetFirstMember();

        if (member == nullptr)
        {
            return { &player };
        }

        std::vector<Player*> tanks{};

        while (member != nullptr)
        {
            Player* const player = member->GetSource();

            if (player == nullptr)
            {
                member = member->next();

                continue;
            }

            if (PlayerbotAI::IsTank(player) == true)
            {
                tanks.emplace_back(player);

                member = member->next();

                continue;
            }

            member = member->next();
        }

        std::sort(tanks.begin(), tanks.end(), [](Player* a, Player* b) {
            if (a == nullptr)
            {
                return false;
            }

            if (b == nullptr)
            {
                return true;
            }

            const uint32_t aItemLevel = a->GetAverageItemLevel();
            const uint32_t bItemLevel = b->GetAverageItemLevel();

            return aItemLevel > bItemLevel;
        });

        return tanks;
    }

};
