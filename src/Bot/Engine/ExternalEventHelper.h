/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#pragma once

#include <map>
#include <string>
#include <cstdint>

class AiObjectContext;
class Player;
class WorldPacket;

class ExternalEventHelper
{
public:
    ExternalEventHelper(AiObjectContext* aiObjectContext) : aiObjectContext(aiObjectContext) {}

    bool ParseChatCommand(const std::string command, Player& owner);
    void HandlePacket(std::map<uint16_t, std::string>& handlers, WorldPacket const& packet, Player* owner = nullptr);
    bool HandleCommand(const std::string name, const std::string param, Player& owner);

private:
    AiObjectContext* aiObjectContext;
};
