/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "AiFactory.h"
#include "SayAction.h"

#include <string>

#include "Event.h"
#include "PlayerbotTextMgr.h"
#include "Playerbots.h"

static const std::unordered_set<std::string> noReplyMsgs = {
    "join",
    "leave",
    "follow",
    "attack",
    "pull",
    "flee",
    "reset",
    "reset ai",
    "all ?",
    "talents",
    "talents list",
    "talents auto",
    "talk",
    "stay",
    "stats",
    "who",
    "items",
    "leave",
    "join",
    "repair",
    "summon",
    "nc ?",
    "co ?",
    "de ?",
    "dead ?",
    "follow",
    "los",
    "guard",
    "do accept invitation",
    "stats",
    "react ?",
    "reset strats",
    "home",
};
static const std::unordered_set<std::string> noReplyMsgParts = {
    "+", "-", "@", "follow target", "focus heal", "cast ", "accept [", "e [", "destroy [", "go zone"};
static const std::unordered_set<std::string> noReplyMsgStarts = {"e ", "accept ", "cast ", "destroy "};

SayAction::SayAction(PlayerbotAI* botAI) : Action(botAI, "say"), Qualified() {}

bool SayAction::Execute(Event)
{
    std::string text = "";
    std::map<std::string, std::string> placeholders;
    Unit* target = AI_VALUE(Unit*, "tank target");
    if (!target)
        target = AI_VALUE(Unit*, "current target");

    // set replace strings
    if (target)
        placeholders["<target>"] = target->GetName();
    placeholders["<randomfaction>"] = IsAlliance(bot->getRace()) ? "Alliance" : "Horde";
    if (qualifier == "low ammo" || qualifier == "no ammo")
    {
        if (Item* const pItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED))
        {
            switch (pItem->GetTemplate()->SubClass)
            {
                case ITEM_SUBCLASS_WEAPON_GUN:
                    placeholders["<ammo>"] = "bullets";
                    break;
                case ITEM_SUBCLASS_WEAPON_BOW:
                case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    placeholders["<ammo>"] = "arrows";
                    break;
            }
        }
    }

    if (bot->GetMap())
    {
        if (AreaTableEntry const* zone = sAreaTableStore.LookupEntry(bot->GetMap()->GetZoneId(bot->GetPhaseMask(), bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ())))
            placeholders["<subzone>"] = zone->area_name[sWorld->GetDefaultDbcLocale()];
    }

    // set delay before next say
    uint32 nextTime = time(nullptr) + urand(1, 30);
    botAI->GetAiObjectContext()->GetValue<time_t>("last said", qualifier)->Set(nextTime);

    Group* group = bot->GetGroup();
    if (group)
    {
        std::vector<Player*> members;
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* member = ref->GetSource();
            PlayerbotAI* memberAi = GET_PLAYERBOT_AI(member);
            if (memberAi)
                members.push_back(member);
        }

        uint32 count = members.size();
        if (count > 1)
        {
            for (uint32 i = 0; i < count * 5; i++)
            {
                int i1 = urand(0, count - 1);
                int i2 = urand(0, count - 1);

                Player* item = members[i1];
                members[i1] = members[i2];
                members[i2] = item;
            }
        }

        int index = 0;
        for (auto& member : members)
        {
            PlayerbotAI* memberAi = GET_PLAYERBOT_AI(member);
            if (memberAi)
                memberAi->GetAiObjectContext()
                    ->GetValue<time_t>("last said", qualifier)
                    ->Set(nextTime + (20 * ++index) + urand(1, 15));
        }
    }

    // load text based on chance
    if (!PlayerbotTextMgr::instance().GetBotText(qualifier, text, placeholders))
        return false;

    if (text.find("/y ") == 0)
        bot->Yell(text.substr(3), (bot->GetTeamId() == TEAM_ALLIANCE ? LANG_COMMON : LANG_ORCISH));
    else
        bot->Say(text, (bot->GetTeamId() == TEAM_ALLIANCE ? LANG_COMMON : LANG_ORCISH));

    return true;
}

bool SayAction::isUseful()
{
    if (!botAI->allowActivity())
        return false;

    if (botAI->HasStrategy("silent", BotState::BOT_STATE_NON_COMBAT))
        return false;

    time_t lastSaid = AI_VALUE2(time_t, "last said", qualifier);
    return (time(nullptr) - lastSaid) > 30;
}

// @TODO: This is not a real action.
// This is actually a hack that is called directly during each update tick.
// This should be moved to a chat service attached to a bot.
void ChatReplyAction::ChatReplyDo(Player& bot, const uint32 type, uint32 guid1, std::string& msg, const std::string& chanName, std::string& name)
{
    // if we're just commanding bots around, don't respond...
    // first one is for exact word matches
    if (noReplyMsgs.contains(msg))
    {
        return;
    }

    // second one is for partial matches like + or - where we change strats
    for (const std::string& noReplyMsg : noReplyMsgParts)
    {
        if (msg.find(noReplyMsg) != std::string::npos)
        {
            return;
        }
    }

    for (const std::string& noReplyMsg : noReplyMsgStarts)
    {
        // Check if the start matches
        if (msg.starts_with(noReplyMsg))
        {
            return;
        }
    }

    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return;
    }

    const ChatChannelSource chatChannelSource = botAI->GetChatChannelSource(&bot, type, chanName);

    if (
        (
            msg.starts_with("LFG")
            || msg.starts_with("LFM")
        )
        && ChatReplyAction::HandleLFGQuestsReply(bot, chatChannelSource, msg, name)
    )
    {
        return;
    }

    if (
        msg.starts_with("WTB")
        && ChatReplyAction::HandleWTBItemsReply(bot, chatChannelSource, msg, name)
    )
    {
        return;
    }

    const std::set<uint32_t> itemIds = ChatHelper::ExtractAllItemIds(msg);
    const std::set<uint32_t> questIds = ChatHelper::ExtractAllQuestIds(msg);

    //toxic links
    if (
        msg.starts_with(PlayerbotAIConfig::instance().toxicLinksPrefix)
        && (
            itemIds.empty() == false
            || questIds.empty() == false
        )
    )
    {
        ChatReplyAction::HandleToxicLinksReply(bot, chatChannelSource, msg, name);

        return;
    }

    //thunderfury
    if (itemIds.count(19019) != 0)
    {
        ChatReplyAction::HandleThunderfuryReply(bot, chatChannelSource);

        return;
    }

    std::string messageRepy = ChatReplyAction::GenerateReplyMessage(bot, msg, guid1, name);

    ChatReplyAction::SendGeneralResponse(bot, chatChannelSource, messageRepy, name);
}

bool ChatReplyAction::HandleThunderfuryReply(Player& bot, ChatChannelSource chatChannelSource)
{
    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return false;
    }

    const ItemTemplate* const thunderfury = ObjectMgr::instance()->GetItemTemplate(19019);

    if (thunderfury == nullptr)
    {
        return false;
    }

    const std::map<std::string, std::string> placeholders{
        { "%thunderfury_link", ChatHelper::FormatItem(thunderfury) }
    };

    const std::string responseMessage = PlayerbotTextMgr::instance().GetBotText("thunderfury_spam", placeholders);

    if (chatChannelSource == ChatChannelSource::SRC_WORLD)
    {
        botAI->SayToWorld(responseMessage);
    }

    if (chatChannelSource == ChatChannelSource::SRC_GENERAL)
    {
        botAI->SayToChannel(responseMessage, ChatChannelId::GENERAL);
    }

    Value<time_t>* const lastSaidValue = botAI->GetAiObjectContext()->GetValue<time_t>("last said", "chat");

    if (lastSaidValue == nullptr)
    {
        return true;
    }

    const time_t now = std::time(nullptr);
    const uint8_t randomDelay = urand(5, 60);

    lastSaidValue->Set(now + randomDelay);

    return true;
}

bool ChatReplyAction::HandleToxicLinksReply(Player& bot, ChatChannelSource chatChannelSource, const std::string&, const std::string&)
{
    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return false;
    }

    std::vector<uint32> incompleteQuests{};

    for (uint16_t slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
    {
        const uint32_t questId = bot.GetQuestSlotQuestId(slot);

        if (questId == 0)
        {
            continue;
        }

        const QuestStatus status = bot.GetQuestStatus(questId);

        if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_NONE)
        {
            incompleteQuests.push_back(questId);
        }
    }

    //items
    std::vector<Item*> botItems = botAI->GetInventoryAndEquippedItems();

    std::map<std::string, std::string> placeholders{
        { "%random_inventory_item_link", PlayerbotTextMgr::instance().GetBotText("string_empty_link") },
        { "%prefix", PlayerbotAIConfig::instance().toxicLinksPrefix },
        { "%my_role", ChatHelper::FormatClass(&bot, AiFactory::GetPlayerSpecTab(&bot)) },
        { "%area_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%zone_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%my_class", ChatHelper::FormatClass(bot.getClass()) },
        { "%my_race", ChatHelper::FormatRace(bot.getRace()) },
        { "%my_level", std::to_string(bot.GetLevel()) }
    };

    if (!botItems.empty())
    {
        placeholders.at("%random_inventory_item_link") = ChatHelper::FormatItem(botItems[rand() % botItems.size()]->GetTemplate());
    }

    placeholders.at("%random_taken_quest_or_item_link") = placeholders.at("%random_inventory_item_link");

    if (!incompleteQuests.empty())
    {
        const Quest* const quest = sObjectMgr->GetQuestTemplate(incompleteQuests[rand() % incompleteQuests.size()]);

        placeholders.at("%random_taken_quest_or_item_link") = ChatHelper::FormatQuest(quest);
    }

    const AreaTableEntry* const current_area = botAI->GetCurrentArea();

    if (current_area != nullptr)
    {
        placeholders.at("%area_name") = botAI->GetLocalizedAreaName(current_area);
    }

    const AreaTableEntry* const current_zone = botAI->GetCurrentZone();

    if (current_zone != nullptr)
    {
        placeholders.at("%zone_name") = botAI->GetLocalizedAreaName(current_zone);
    }

    const std::string responseMessage = PlayerbotTextMgr::instance().GetBotText("suggest_toxic_links", placeholders);

    if (chatChannelSource == ChatChannelSource::SRC_WORLD)
    {
        botAI->SayToWorld(responseMessage);
    }

    if (chatChannelSource == ChatChannelSource::SRC_GENERAL)
    {
        botAI->SayToChannel(responseMessage, ChatChannelId::GENERAL);
    }

    if (chatChannelSource == ChatChannelSource::SRC_GUILD)
    {
        botAI->SayToGuild(responseMessage);
    }

    const time_t now = std::time(nullptr);
    const uint8_t randomDelay = urand(5, 60);
    Value<time_t>* const lastSaidValue = botAI->GetAiObjectContext()->GetValue<time_t>("last said", "chat");

    if (lastSaidValue == nullptr)
    {
        return true;
    }

    lastSaidValue->Set(now + randomDelay);

    return true;
}

bool ChatReplyAction::HandleWTBItemsReply(Player& bot, ChatChannelSource chatChannelSource, const std::string& msg, const std::string& name)
{
    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return false;
    }

    const std::set<uint32_t> messageItemIds = ChatHelper::ExtractAllItemIds(msg);

    if (messageItemIds.empty())
    {
        return false;
    }

    std::set<uint32_t> matchingItemIds;

    for (const uint32_t messageItemId : messageItemIds)
    {
        if (botAI->HasItemInInventory(messageItemId))
        {
            matchingItemIds.insert(messageItemId);
        }
    }

    if (matchingItemIds.empty())
    {
        return true;
    }

    std::map<std::string, std::string> placeholders{
        { "%other_name", name },
        { "%area_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%zone_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%my_class", ChatHelper::FormatClass(bot.getClass()) },
        { "%my_race", ChatHelper::FormatRace(bot.getRace()) },
        { "%my_level", std::to_string(bot.GetLevel()) },
        { "%my_role", ChatHelper::FormatClass(&bot, AiFactory::GetPlayerSpecTab(&bot)) },
        { "%formatted_item_links", "" }
    };

    const AreaTableEntry* const current_area = botAI->GetCurrentArea();

    if (current_area != nullptr)
    {
        placeholders.at("%area_name") = botAI->GetLocalizedAreaName(current_area);
    }

    const AreaTableEntry* const current_zone = botAI->GetCurrentZone();

    if (current_zone != nullptr)
    {
        placeholders.at("%zone_name") = botAI->GetLocalizedAreaName(current_zone);
    }

    std::string& formattedLinks = placeholders.at("%formatted_item_links");

    for (const uint32_t matchingItemId : matchingItemIds)
    {
        const ItemTemplate* const proto = ObjectMgr::instance()->GetItemTemplate(matchingItemId);

        if (proto == nullptr)
        {
            continue;
        }

        formattedLinks += ChatHelper::FormatItem(proto, botAI->GetInventoryItemsCountWithId(matchingItemId));
        formattedLinks += " ";
    }

    const time_t now = std::time(nullptr);
    const uint8_t randomDelay = urand(5, 60);
    Value<time_t>* const lastSaidValue = botAI->GetAiObjectContext()->GetValue<time_t>("last said", "chat");

    if (lastSaidValue == nullptr)
    {
        return true;
    }

    if (
        chatChannelSource != ChatChannelSource::SRC_WORLD
        && chatChannelSource != ChatChannelSource::SRC_GENERAL
        && chatChannelSource != ChatChannelSource::SRC_TRADE
    )
    {
        lastSaidValue->Set(now + randomDelay);

        return true;
    }

    const std::string whisperMessage = PlayerbotTextMgr::instance().GetBotText("response_wtb_items_whisper", placeholders);

    const bool sayToChannel = urand(0, 1) == 1;

    if (!sayToChannel)
    {
        botAI->Whisper(whisperMessage, name);
        lastSaidValue->Set(now + randomDelay);

        return true;
    }

    const std::string channelMessage = PlayerbotTextMgr::instance().GetBotText("response_wtb_items_channel", placeholders);

    if (chatChannelSource == ChatChannelSource::SRC_WORLD)
    {
        botAI->SayToWorld(channelMessage);
    }

    if (chatChannelSource == ChatChannelSource::SRC_GENERAL)
    {
        botAI->SayToChannel(channelMessage, ChatChannelId::GENERAL);
    }

    if (chatChannelSource == ChatChannelSource::SRC_TRADE)
    {
        botAI->SayToChannel(channelMessage, ChatChannelId::TRADE);
    }

    lastSaidValue->Set(now + randomDelay);

    return true;
}

// @TODO: This should be move to a dedicated chat service attached to the bot, not a hacky static method in an action.
/**
 * Attempts to respond to LFG/LFM quest messages by matching linked quests
 * against the bot's current quest log.
 *
 * Parses quest links in the incoming message, intersects them with the bot's
 * active quests, and, if any match, replies either in the source channel
 * (World/General) or via whisper depending on the channel and RNG rules.
 * For LookingForGroup it only whispers. When a response is sent, updates the
 * bot's "last said" chat timer to throttle further replies.
 *
 * @param bot The bot player instance to evaluate and respond as.
 * @param chatChannelSource The channel source of the incoming message.
 * @param msg The incoming chat message text.
 * @param name The sender's character name (used for whispers/placeholders).
 * @return true if the message was handled (including when no quest matched),
 *         false if it could not be processed (e.g., no AI, no quests parsed).
 */
bool ChatReplyAction::HandleLFGQuestsReply(Player& bot, ChatChannelSource chatChannelSource, const std::string& msg, const std::string& name)
{
    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return false;
    }

    const std::set<uint32_t> messageQuestIds = ChatHelper::ExtractAllQuestIds(msg);

    if (messageQuestIds.empty())
    {
        return false;
    }

    const std::set<uint32_t> botQuestIds = botAI->GetAllCurrentQuestIds();
    std::set<uint32_t> matchingQuestIds;

    for (const uint32_t botQuestId : botQuestIds)
    {
        if (messageQuestIds.count(botQuestId) != 0)
        {
            matchingQuestIds.insert(botQuestId);
        }
    }

    if (matchingQuestIds.empty())
    {
        return true;
    }

    const AreaTableEntry* const current_area = botAI->GetCurrentArea();
    const AreaTableEntry* const current_zone = botAI->GetCurrentZone();

    std::map<std::string, std::string> placeholders{
        { "%other_name", name },
        { "%quest_links", "" },
        { "%area_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%zone_name", PlayerbotTextMgr::instance().GetBotText("string_unknown_area") },
        { "%my_class", ChatHelper::FormatClass(bot.getClass()) },
        { "%my_race", ChatHelper::FormatRace(bot.getRace()) },
        { "%my_level", std::to_string(bot.GetLevel()) },
        { "%my_role", ChatHelper::FormatClass(&bot, AiFactory::GetPlayerSpecTab(&bot)) },
    };

    if (current_area != nullptr)
    {
        placeholders.at("%area_name") = botAI->GetLocalizedAreaName(current_area);
    }

    if (current_zone != nullptr)
    {
        placeholders.at("%zone_name") = botAI->GetLocalizedAreaName(current_zone);
    }

    std::string& formattedQuestLinks = placeholders.at("%quest_links");

    for (const uint32_t matchingQuestId : matchingQuestIds)
    {
        Quest const* quest = ObjectMgr::instance()->GetQuestTemplate(matchingQuestId);

        if (quest == nullptr)
        {
            continue;
        }

        formattedQuestLinks += ChatHelper::FormatQuest(quest);
    }

    const time_t now = std::time(nullptr);
    const uint8_t randomDelay = urand(5, 60);
    Value<time_t>* const lastSaidValue = botAI->GetAiObjectContext()->GetValue<time_t>("last said", "chat");

    if (lastSaidValue == nullptr)
    {
        return true;
    }

    if (
        chatChannelSource != ChatChannelSource::SRC_WORLD
        && chatChannelSource != ChatChannelSource::SRC_GENERAL
        && chatChannelSource != ChatChannelSource::SRC_LOOKING_FOR_GROUP
    )
    {
        lastSaidValue->Set(now + randomDelay);

        return true;
    }

    const std::string whisperResponse = PlayerbotTextMgr::instance().GetBotText("response_lfg_quests_whisper", placeholders);
    const bool sayToChannel = urand(0, 1) == 1;

    if (!sayToChannel || chatChannelSource == ChatChannelSource::SRC_LOOKING_FOR_GROUP)
    {
        botAI->Whisper(whisperResponse, name);
        lastSaidValue->Set(now + randomDelay);

        return true;
    }

    const std::string channelResponse = PlayerbotTextMgr::instance().GetBotText("response_lfg_quests_channel", placeholders);

    if (chatChannelSource == ChatChannelSource::SRC_WORLD)
    {
        botAI->SayToWorld(channelResponse);
    }

    if (chatChannelSource == ChatChannelSource::SRC_GENERAL)
    {
        botAI->SayToChannel(channelResponse, ChatChannelId::GENERAL);
    }

    lastSaidValue->Set(now + randomDelay);

    return true;
}

bool ChatReplyAction::SendGeneralResponse(Player& bot, ChatChannelSource chatChannelSource, std::string& responseMessage, std::string& name)
{
    PlayerbotAI* const botAI = PlayerbotsMgr::instance().GetPlayerbotAI(&bot);

    if (botAI == nullptr)
    {
        return false;
    }

    switch (chatChannelSource)
    {
        case ChatChannelSource::SRC_WORLD:
        {
            botAI->SayToWorld(responseMessage);

            break;
        }
        case ChatChannelSource::SRC_GENERAL:
        {
            //may reply to the same channel 80% or whisper
            if (urand(0, 100) < 80)
            {
                botAI->SayToChannel(responseMessage, ChatChannelId::GENERAL);

                break;
            }

            botAI->Whisper(responseMessage, name);

            break;
        }
        case ChatChannelSource::SRC_TRADE:
        {
            break;
        }
        case ChatChannelSource::SRC_LOCAL_DEFENSE:
        {
            botAI->SayToChannel(responseMessage, ChatChannelId::LOCAL_DEFENSE);

            break;
        }
        case ChatChannelSource::SRC_WORLD_DEFENSE:
        {
            break;
        }
        case ChatChannelSource::SRC_LOOKING_FOR_GROUP:
        {
            break;
        }
        case ChatChannelSource::SRC_GUILD_RECRUITMENT:
        {
            break;
        }
        case ChatChannelSource::SRC_WHISPER:
        {
            botAI->Whisper(responseMessage, name);

            break;
        }
        case ChatChannelSource::SRC_SAY:
        {
            botAI->Say(responseMessage);

            break;
        }
        case ChatChannelSource::SRC_YELL:
        {
            botAI->Yell(responseMessage);

            break;
        }
        case ChatChannelSource::SRC_GUILD:
        {
            botAI->SayToGuild(responseMessage);

            break;
        }
        default:
            break;
    }

    Value<time_t>* const lastSaidValue = botAI->GetAiObjectContext()->GetValue<time_t>("last said", "chat");

    if (lastSaidValue == nullptr)
    {
        return true;
    }

    lastSaidValue->Set(time(0) + urand(5, 25));

    return true;
}

template <std::size_t S>
static bool containsToken(
    const std::array<std::string_view, S>& values,
    const std::string& token
) noexcept
{
    for (std::size_t i = 0; i < S; ++i)
    {
        if (token == values.at(i))
        {
            return true;
        }
    }

    return false;
}

QuestionCategoryEnum ChatReplyAction::DetermineQuestionCategory(const std::string& fullMessage, const std::string& word) noexcept
{
    if (word.find("what") != std::string::npos)
    {
        return QuestionCategoryEnum::WHAT;
    }

    if (word.find("who") != std::string::npos)
    {
        return QuestionCategoryEnum::WHO;
    }

    if (word == "when")
    {
        return QuestionCategoryEnum::WHEN;
    }

    if (word == "where")
    {
        return QuestionCategoryEnum::WHERE;
    }

    if (word == "why")
    {
        return QuestionCategoryEnum::WHY;
    }

    if (fullMessage.find("?") != std::string::npos)
    {
        return QuestionCategoryEnum::GENERAL;
    }

    return QuestionCategoryEnum::NONE;
}

static constexpr uint8_t MAX_RESPONSE_SIZE = 255;

std::string ChatReplyAction::PostProcessResponseMessage(const std::string& message, const ChatReplyType replyType, const std::string& name) noexcept
{
    std::string response = message;

    if (response.empty())
    {
        response = PlayerbotTextMgr::instance().GetBotText(replyType, name);
    }

    if (response.size() > MAX_RESPONSE_SIZE)
    {
        response.resize(MAX_RESPONSE_SIZE);
    }

    return response;
}

std::array<std::string, MAX_PARSED_WORDS> ChatReplyAction::ExtractWords(const std::string& message) noexcept
{
    std::array<std::string, MAX_PARSED_WORDS> words{};

    uint8_t iterations = 0;
    std::string segment;
    std::stringstream text{message};

    while (std::getline(text, segment, ' ') && iterations < MAX_PARSED_WORDS)
    {
        words.at(iterations) = segment;

        ++iterations;
    }

    return words;
}

static constexpr std::array<std::string_view, 4> whatQuestionAnswersTemplates = {
    "i dont know what",
    "i dont know %s",
    "who cares",
    "afraid that was before i was around or paying attention",
};

static constexpr std::array<std::string_view, 5> whoQuestionAnswersTemplates = {
    "nobody",
    "we all do",
    "perhaps its you, %s",
    "dunno %s",
    "is it me?",
};

static constexpr std::array<std::string_view, 7> whenQuestionAnswersTemplates = {
    "soon perhaps %s",
    "probably later",
    "never",
    "what do i look like, a psychic?",
    "a few minutes, maybe an hour ... years?",
    "when? good question %s",
    "dunno %s",
};

static constexpr std::array<std::string_view, 7> whereQuestionAnswersTemplates = {
    "really want me to answer that?",
    "on the map?",
    "who cares",
    "afk?",
    "none of your buisiness where",
    "yeah, where?",
    "dunno %s",
};

static constexpr std::array<std::string_view, 7> whyQuestionAnswersTemplates = {
    "dunno %s",
    "why? just because %s",
    "why is the sky blue?",
    "dont ask me %s, im just a bot",
    "your asking the wrong person",
    "who knows?",
    "dunno %s",
};

static constexpr std::array<std::string_view, 4> pastTenseQuestionAnswersTemplates = {
    "its true, %verb_pos_1 %verb_pos %verb_pos_2 %verb_pos_3 %verb_pos_4 %verb_pos_4",
    "ya %s but thats in the past",
    "nah, but %verb_pos_1 will %verb_pos_3 again though %s",
    "afraid that was before i was around or paying attention",
};

static constexpr std::array<std::string_view, 7> presentTenseQuestionAnswersTemplates = {
    "its true, %verb_pos_1 %verb_pos %verb_pos_2 %verb_pos_3 %verb_pos_4 %verb_pos_5",
    "ya %s thats true",
    "maybe %verb_pos_1 %verb_pos %verb_pos_2 %verb_pos_3 %verb_pos_4 %verb_pos_5",
    "dunno %s",
    "i dont think so %s",
    "yes",
    "no",
};

static constexpr std::array<std::string_view, 9> futureTenseQuestionAnswersTemplates = {
    "dunno %s",
    "beats me %s",
    "how should i know %s",
    "dont ask me %s, im just a bot",
    "your asking the wrong person",
    "what do i look like, a psychic?",
    "sure %s",
    "i dont think so %s",
    "maybe",
};

static constexpr std::array<std::string_view, 3> pastTenseAnswersTemplates = {
    "yeah %s, the key word being %verb_pos %verb_pos_1",
    "ya %s but thats in the past",
    "%verb_pos_1_1 will %verb_pos_1 again though %s",
};

static constexpr std::array<std::string_view, 3> presentTenseAnswersTemplates = {
    "%s, what do you mean %verb_pos_1?",
    "%s, what is a %verb_pos_1?",
    "yeah i know %verb_pos_1_1 is a %verb_pos_1",
};

static constexpr std::array<std::string_view, 3> futureTenseAnswersTemplates = {
    "are you sure thats going to happen %s?",
    "%s, what will happen %s?",
    "are you saying %verb_pos_1_1 will %verb_pos_1 %verb_pos_2 %s?",
};

std::size_t ChatReplyAction::ClampSize(
    const std::size_t value,
    const std::size_t lowerBound,
    const std::size_t upperBound
) noexcept
{
    if (value > upperBound)
    {
        return upperBound;
    }

    if (value < lowerBound)
    {
        return lowerBound;
    }

    return value;
}

std::array<PlaceholderTemplateStruct, 8u> ChatReplyAction::GeneratePlaceholders(
    const std::string& name,
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
    const SentenceTypeStruct& sentenceType
) noexcept
{
    std::array<PlaceholderTemplateStruct, 8u> placeholders = {
        PlaceholderTemplateStruct{
            .placeholder = "%s",
            .replacement = name
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_1",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_2",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_3",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_4",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_5",
            .replacement = ""
        },
        PlaceholderTemplateStruct{
            .placeholder = "%verb_pos_1_1",
            .replacement = ""
        },
    };

    if (sentenceType.verbTense == VerbTenseEnum::UNKNOWN)
    {
        return placeholders;
    }

    const std::size_t arrayUpperBound = extractedWords.size() - 1u;
    const std::size_t clampedVerbPosition = ChatReplyAction::ClampSize(sentenceType.verbPosition, 0u, arrayUpperBound);
    const std::size_t verbPosition1 = ChatReplyAction::ClampSize(clampedVerbPosition + 1u, 0u, arrayUpperBound);
    const std::size_t verbPosition2 = ChatReplyAction::ClampSize(clampedVerbPosition + 2u, 0u, arrayUpperBound);
    const std::size_t verbPosition3 = ChatReplyAction::ClampSize(clampedVerbPosition + 3u, 0u, arrayUpperBound);
    const std::size_t verbPosition4 = ChatReplyAction::ClampSize(clampedVerbPosition + 4u, 0u, arrayUpperBound);
    const std::size_t verbPosition5 = ChatReplyAction::ClampSize(clampedVerbPosition + 5u, 0u, arrayUpperBound);

    std::size_t variableVerbPosition = sentenceType.verbPosition + 1u;

    if (sentenceType.verbPosition != 0u)
    {
        variableVerbPosition = sentenceType.verbPosition - 1u;
    }

    variableVerbPosition = ChatReplyAction::ClampSize(variableVerbPosition, 0u, arrayUpperBound);

    placeholders.at(1u).replacement = extractedWords.at(clampedVerbPosition);
    placeholders.at(2u).replacement = extractedWords.at(verbPosition1);
    placeholders.at(3u).replacement = extractedWords.at(verbPosition2);
    placeholders.at(4u).replacement = extractedWords.at(verbPosition3);
    placeholders.at(5u).replacement = extractedWords.at(verbPosition4);
    placeholders.at(6u).replacement = extractedWords.at(verbPosition5);
    placeholders.at(7u).replacement = extractedWords.at(variableVerbPosition);

    return placeholders;
}

std::string ChatReplyAction::GenerateResponseFromTemplate(
    const std::string_view& templateString,
    const std::string& name,
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
    const SentenceTypeStruct& sentenceType
) noexcept
{
    const std::array<PlaceholderTemplateStruct, 8u> placeholdersTemplate = ChatReplyAction::GeneratePlaceholders(
        name,
        extractedWords,
        sentenceType
    );

    std::string response{templateString};

    for (const PlaceholderTemplateStruct& placeholderTemplate : placeholdersTemplate)
    {
        if (placeholderTemplate.placeholder.empty() == true)
        {
            continue;
        }

        std::size_t position = response.find(placeholderTemplate.placeholder);

        while (position != std::string::npos)
        {
            response.replace(position, placeholderTemplate.placeholder.size(), placeholderTemplate.replacement);
            position = response.find(placeholderTemplate.placeholder, position + placeholderTemplate.replacement.size());
        }
    }

    return response;
}

template <std::size_t S>
std::string_view pickRandomStringFromArray(const std::array<std::string_view, S> array) noexcept
{
    assert(S > 0);
    assert(array.size() > S);

    const std::size_t randomIndex = urand(0, S - 1);

    return array.at(randomIndex);
}

std::string ChatReplyAction::GenerateRandomQuestionResponse(
    const std::string& name,
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
    const SentenceTypeStruct& sentenceType
) noexcept
{
    if (sentenceType.type != SentenceTypeEnum::QUESTION)
    {
        return "";
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::NONE)
    {
        LOG_ERROR("playerbots", "ChatReplyAction: Attempted to generate random question response without a question category.");

        return "";
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::WHAT)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<whatQuestionAnswersTemplates.size()>(whatQuestionAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::WHO)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<whoQuestionAnswersTemplates.size()>(whoQuestionAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::WHEN)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<whenQuestionAnswersTemplates.size()>(whenQuestionAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::WHERE)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<whereQuestionAnswersTemplates.size()>(whereQuestionAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::WHY)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<whyQuestionAnswersTemplates.size()>(whyQuestionAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.questionCategory == QuestionCategoryEnum::GENERAL)
    {
        if (sentenceType.verbTense == VerbTenseEnum::PAST)
        {
            const std::string_view responseTemplate = pickRandomStringFromArray<pastTenseQuestionAnswersTemplates.size()>(pastTenseQuestionAnswersTemplates);

            return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
        }

        if (sentenceType.verbTense == VerbTenseEnum::PRESENT)
        {
            const std::string_view responseTemplate = pickRandomStringFromArray<presentTenseQuestionAnswersTemplates.size()>(presentTenseQuestionAnswersTemplates);

            return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
        }

        if (sentenceType.verbTense == VerbTenseEnum::FUTURE)
        {
            const std::string_view responseTemplate = pickRandomStringFromArray<futureTenseQuestionAnswersTemplates.size()>(futureTenseQuestionAnswersTemplates);

            return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
        }
    }

    return "";
}

std::string ChatReplyAction::GenerateRandomVerbBasedResponse(
    const std::string& name,
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
    const SentenceTypeStruct& sentenceType
) noexcept
{
    if (sentenceType.verbTense == VerbTenseEnum::UNKNOWN)
    {
        LOG_ERROR("playerbots", "ChatReplyAction: Attempted to generate random verb based response without a verb tense.");

        return "";
    }

    if (sentenceType.verbTense == VerbTenseEnum::PAST)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<pastTenseAnswersTemplates.size()>(pastTenseAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.verbTense == VerbTenseEnum::PRESENT)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<presentTenseAnswersTemplates.size()>(presentTenseAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    if (sentenceType.verbTense == VerbTenseEnum::FUTURE)
    {
        const std::string_view responseTemplate = pickRandomStringFromArray<futureTenseAnswersTemplates.size()>(futureTenseAnswersTemplates);

        return ChatReplyAction::GenerateResponseFromTemplate(responseTemplate, name, extractedWords, sentenceType);
    }

    return "";
}

std::string ChatReplyAction::GenerateRandomResponse(
    const std::string& name,
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
    const SentenceTypeStruct& sentenceType
) noexcept
{
    if (sentenceType.type == SentenceTypeEnum::QUESTION)
    {
        return ChatReplyAction::GenerateRandomQuestionResponse(name, extractedWords, sentenceType);
    }

    if (sentenceType.verbTense != VerbTenseEnum::UNKNOWN)
    {
        return ChatReplyAction::GenerateRandomVerbBasedResponse(name, extractedWords, sentenceType);
    }

    return "";
}

SentenceTypeStruct ChatReplyAction::DetermineSentenceType(
    const std::string& message,
    const std::array<std::string,MAX_PARSED_WORDS>& extractedWords,
    const uint32_t senderLowGUID,
    const std::string& botName
) noexcept
{
    static constexpr std::array<std::string_view, 3> verbPresent = {
        "am",
        "are",
        "is"
    };

    static constexpr std::array<std::string_view, 2> verbPast = {
        "was",
        "were"
    };

    static constexpr std::array<std::string_view, 1> verbFuture = {
        "will"
    };

    static constexpr std::array<std::string_view, 4> greetingWord = {
        "hi",
        "hey",
        "hello",
        "wazzup"
    };

    static constexpr std::array<std::string_view, 2> grudgeWords = {
        "shut",
        "noob"
    };

    static constexpr std::size_t ITERATIONS_UPPER_THRESHOLD = 8;

    SentenceTypeStruct sentenceType{
        .type = SentenceTypeEnum::UNKNOWN,
        .questionCategory = ChatReplyAction::DetermineQuestionCategory(message, extractedWords.at(0)),
        .verbPosition = 0,
        .verbTense = VerbTenseEnum::UNKNOWN
    };

    if (sentenceType.questionCategory != QuestionCategoryEnum::NONE)
    {
        sentenceType.type = SentenceTypeEnum::QUESTION;
    }

    const Player* const sender = ObjectAccessor::FindPlayer(ObjectGuid(HighGuid::Player, senderLowGUID));

    if (sender != nullptr && sender->isGMChat())
    {
        sentenceType.type = SentenceTypeEnum::GM_BLAME;

        return sentenceType;
    }

    const std::size_t iterationsLimit = std::min(ITERATIONS_UPPER_THRESHOLD, extractedWords.size());
    const bool botIsMentioned = message.find(botName) != std::string::npos;

    for (std::size_t i = 0; i < iterationsLimit; ++i)
    {
        const std::string& currentWord = extractedWords.at(i);

        if (currentWord.empty())
        {
            break;
        }

        if (containsToken<greetingWord.size()>(greetingWord, currentWord))
        {
            sentenceType.type = SentenceTypeEnum::GREETING;

            return sentenceType;
        }

        if (containsToken<verbPresent.size()>(verbPresent, currentWord))
        {
            sentenceType.type = SentenceTypeEnum::STATEMENT;
            sentenceType.verbPosition = i;
            sentenceType.verbTense = VerbTenseEnum::PRESENT;

            if (sentenceType.verbPosition == 0)
            {
                sentenceType.type = SentenceTypeEnum::QUESTION;
                sentenceType.questionCategory = QuestionCategoryEnum::GENERAL;
            }

            return sentenceType;
        }

        if (containsToken<verbFuture.size()>(verbFuture, currentWord))
        {
            sentenceType.type = SentenceTypeEnum::STATEMENT;
            sentenceType.verbPosition = i;
            sentenceType.verbTense = VerbTenseEnum::FUTURE;

            return sentenceType;
        }

        if (containsToken<verbPast.size()>(verbPast, currentWord))
        {
            sentenceType.type = SentenceTypeEnum::STATEMENT;
            sentenceType.verbPosition = i;
            sentenceType.verbTense = VerbTenseEnum::PAST;

            return sentenceType;
        }

        if (containsToken<grudgeWords.size()>(grudgeWords, currentWord))
        {
            if (!botIsMentioned)
            {
                continue;
            }

            sentenceType.type = SentenceTypeEnum::GRUDGE_INDUCING;

            return sentenceType;
        }
    }

    return sentenceType;
}

std::string ChatReplyAction::GenerateReplyMessage(Player& bot, std::string& incomingMessage, uint32_t& guid1, std::string& name)
{
    const std::array<std::string, MAX_PARSED_WORDS> extractedWords = ChatReplyAction::ExtractWords(incomingMessage);
    const SentenceTypeStruct sentenceType = ChatReplyAction::DetermineSentenceType(incomingMessage, extractedWords, guid1, bot.GetName());

    if (sentenceType.type == SentenceTypeEnum::GM_BLAME)
    {
        return ChatReplyAction::PostProcessResponseMessage("", REPLY_ADMIN_ABUSE, name);
    }

    if (sentenceType.type == SentenceTypeEnum::GRUDGE_INDUCING)
    {
        return ChatReplyAction::PostProcessResponseMessage("", REPLY_GRUDGE, name);
    }

    if (sentenceType.type == SentenceTypeEnum::GREETING)
    {
        return ChatReplyAction::PostProcessResponseMessage("", REPLY_HELLO, name);
    }

    if (sentenceType.type == SentenceTypeEnum::QUESTION || sentenceType.type == SentenceTypeEnum::STATEMENT)
    {
        const std::string randomResponse = ChatReplyAction::GenerateRandomResponse(name, extractedWords, sentenceType);

        if (!randomResponse.empty())
        {
            return ChatReplyAction::PostProcessResponseMessage(randomResponse, REPLY_NOT_UNDERSTAND, name);
        }
    }

    if (incomingMessage.find(bot.GetName()) != std::string::npos)
    {
        return ChatReplyAction::PostProcessResponseMessage("", REPLY_NAME, name);
    }

    return ChatReplyAction::PostProcessResponseMessage("", REPLY_NOT_UNDERSTAND, name);
}
