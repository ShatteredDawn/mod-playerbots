/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ExternalEventHelper.h"

#include "AiObjectContext.h"
#include "ChatHelper.h"
#include "Trigger.h"

bool ExternalEventHelper::ParseChatCommand(const std::string command, Player& owner)
{
    const bool commandHandled = this->HandleCommand(command, "", owner);

    if (commandHandled)
    {
        return true;
    }

    size_t i = std::string::npos;

    while (true)
    {
        size_t found = command.rfind(" ", i);

        if (found == std::string::npos || found == 0)
        {
            break;
        }

        const std::string name = command.substr(0, found);
        const std::string param = command.substr(found + 1);

        i = found - 1;

        const bool handled = this->HandleCommand(name, param, owner);

        if (handled)
        {
            return true;
        }
    }

    if (!ChatHelper::parseableItem(command))
    {
        return false;
    }

    this->HandleCommand("c", command, owner);
    this->HandleCommand("t", command, owner);

    return true;
}

void ExternalEventHelper::HandlePacket(std::map<uint16, std::string>& handlers, WorldPacket const& packet,
                                       Player* owner)
{
    const uint16_t opcode = packet.GetOpcode();
    const std::string name = handlers.at(opcode);

    if (name.empty())
    {
        return;
    }

    Trigger* const trigger = aiObjectContext->GetTrigger(name);

    if (trigger == nullptr)
    {
        return;
    }

    WorldPacket p(packet);

    trigger->ExternalEvent(p, owner);
}

bool ExternalEventHelper::HandleCommand(const std::string name, const std::string param, Player& owner)
{
    Trigger* const trigger = aiObjectContext->GetTrigger(name);

    if (trigger == nullptr)
    {
        return false;
    }

    trigger->ExternalEvent(param, &owner);

    return true;
}
