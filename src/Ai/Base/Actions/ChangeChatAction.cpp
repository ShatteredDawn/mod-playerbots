/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ChangeChatAction.h"

#include "AiObjectContext.h"
#include "Event.h"
#include "ChatHelper.h"
#include "PlayerbotAI.h"

bool ChangeChatAction::Execute(Event event)
{
    const std::string text = event.getParam();
    const ChatMsg parsed = ChatHelper::parseChat(text);

    std::ostringstream out{};

    if (parsed == CHAT_MSG_SYSTEM)
    {
        out << "Current chat is " << ChatHelper::FormatChat(*this->context->GetValue<ChatMsg>("chat"));
        return this->botAI->TellMaster(out);
    }

    this->context->GetValue<ChatMsg>("chat")->Set(parsed);

    out << "Chat set to " << ChatHelper::FormatChat(parsed);
    return this->botAI->TellMaster(out);

};
