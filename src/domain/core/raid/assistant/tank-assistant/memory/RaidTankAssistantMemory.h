#pragma once

#include <set>

#include "Player.h"

class RaidTankAssistantMemory
{
private:
    std::set<Player*> tanks{};

public:
    RaidTankAssistantMemory() = default;
    ~RaidTankAssistantMemory() = default;

    void clear() noexcept
    {
        this->tanks.clear();
    }

    void add(Player* player) noexcept
    {
        if (player == nullptr)
        {
            return;
        }

        this->tanks.insert(player);
    }

    void remove(Player* player) noexcept
    {
        if (player == nullptr)
        {
            return;
        }

        this->tanks.erase(player);
    }

    [[nodiscard]] const std::set<Player*>& getTanks() const noexcept
    {
        return this->tanks;
    }

    [[nodiscard]] Player* getMainTank() const noexcept
    {
        if (this->tanks.empty())
        {
            return nullptr;
        }

        return *this->tanks.begin();
    }

    [[nodiscard]] Player* getSecondTank() const noexcept
    {
        if (this->tanks.size() < 2)
        {
            return nullptr;
        }

        std::set<Player*>::iterator it = this->tanks.begin();

        ++it;

        return *it;
    }
};
