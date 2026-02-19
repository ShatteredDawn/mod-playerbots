#pragma once

#include "Player.h"

class ZulGurubFacade
{
public:

    [[nodiscard]] static bool IsInInstance(Player& player) noexcept
    {
        return player.GetMapId() == MAP_ZUL_GURUB;
    }

private:
    ZulGurubFacade() = delete;
    ~ZulGurubFacade() = delete;

    ZulGurubFacade(ZulGurubFacade const&) = delete;
    ZulGurubFacade& operator=(ZulGurubFacade const&) = delete;

    ZulGurubFacade(ZulGurubFacade&&) = delete;
    ZulGurubFacade& operator=(ZulGurubFacade&&) = delete;
};
