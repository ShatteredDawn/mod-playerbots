/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_SAYACTION_H
#define _PLAYERBOT_SAYACTION_H

#include "Action.h"
#include "PlayerbotAI.h"
#include "NamedObjectContext.h"

class PlayerbotAI;
class SayAction : public Action, public Qualified
{
public:
    SayAction(PlayerbotAI* botAI);

    bool Execute(Event event) override;
    bool isUseful() override;
    std::string const getName() override { return "say::" + qualifier; }

private:
    static std::map<std::string, std::vector<std::string>> stringTable;
    static std::map<std::string, uint32> probabilityTable;
};

enum class QuestionCategoryEnum
{
    NONE = 0,
    GENERAL = 1,
    WHAT = 2,
    WHO = 3,
    WHEN = 4,
    WHERE = 5,
    WHY = 6
};

static constexpr std::size_t MAX_PARSED_WORDS = 15;

struct GenerateResponseFromTemplateParametersStruct
{
    const std::string& templateString;
    const std::string& name;
    const std::array<std::string, MAX_PARSED_WORDS>& extractedWords;
    const bool hasValidVerbPosition;
    const std::size_t verbPosition;
};

struct PlaceholderTemplateStruct
{
    std::string_view placeholder;
    std::string replacement;
};

enum class SentenceTypeEnum
{
    UNKNOWN,
    QUESTION,
    STATEMENT,
    GRUDGE_INDUCING,
    GM_BLAME,
    GREETING
};

enum class VerbTenseEnum
{
    UNKNOWN = 0,
    PAST = 1,
    PRESENT = 2,
    FUTURE = 3
};

struct SentenceTypeStruct
{
    SentenceTypeEnum type;
    QuestionCategoryEnum questionCategory = QuestionCategoryEnum::NONE;
    std::size_t verbPosition = 0;
    VerbTenseEnum verbTense = VerbTenseEnum::UNKNOWN;
};

class ChatReplyAction : public Action
{
public:
    ChatReplyAction(PlayerbotAI* ai) : Action(ai, "chat message") {}
    virtual bool Execute(Event) { return true; }
    bool isUseful() { return true; }

    [[nodiscard]] static std::size_t ClampSize(
        const std::size_t value,
        const std::size_t lowerBound,
        const std::size_t upperBound
    ) noexcept;
    [[nodiscard]] static std::array<PlaceholderTemplateStruct, 8u> GeneratePlaceholders(
        const std::string& name,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const SentenceTypeStruct& sentenceType
    ) noexcept;
    [[nodiscard]] static std::string GenerateRandomQuestionResponse(
        const std::string& name,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const SentenceTypeStruct& sentenceType
    ) noexcept;
    [[nodiscard]] static std::string GenerateRandomVerbBasedResponse(
        const std::string& name,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const SentenceTypeStruct& sentenceType
    ) noexcept;
    [[nodiscard]] static std::string GenerateRandomResponse(
        const std::string& name,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const SentenceTypeStruct& sentenceType
    ) noexcept;
    [[nodiscard]] static std::string GenerateResponseFromTemplate(
        const std::string_view& templateString,
        const std::string& name,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const SentenceTypeStruct& sentenceType
    ) noexcept;
    [[nodiscard]] static SentenceTypeStruct DetermineSentenceType(
        const std::string& message,
        const std::array<std::string, MAX_PARSED_WORDS>& extractedWords,
        const uint32_t senderLowGUID,
        const std::string& botName
    ) noexcept;
    [[nodiscard]] static QuestionCategoryEnum DetermineQuestionCategory(const std::string& fullMessage, const std::string& word) noexcept;
    [[nodiscard]] static std::string PostProcessResponseMessage(const std::string& message, const ChatReplyType replyType, const std::string& name) noexcept;
    [[nodiscard]] static std::array<std::string, MAX_PARSED_WORDS> ExtractWords(const std::string& message) noexcept;
    static void ChatReplyDo(Player& bot, const uint32 type, uint32 guid1, std::string& msg, const std::string& chanName, std::string& name);
    static bool HandleThunderfuryReply(Player& bot, ChatChannelSource chatChannelSource);
    static bool HandleToxicLinksReply(Player& bot, ChatChannelSource chatChannelSource, const std::string& msg, const std::string& name);
    static bool HandleWTBItemsReply(Player& bot, ChatChannelSource chatChannelSource, const std::string& msg, const std::string& name);
    static bool HandleLFGQuestsReply(Player& bot, ChatChannelSource chatChannelSource, const std::string& msg, const std::string& name);
    static bool SendGeneralResponse(Player& bot, ChatChannelSource chatChannelSource, std::string& responseMessage, std::string& name);
    static std::string GenerateReplyMessage(Player& bot, std::string& incomingMessage, uint32& guid1, std::string& name);
};

#endif
