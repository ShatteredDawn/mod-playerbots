/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include <charconv>

#include "ChatHelper.h"

#include "AiFactory.h"
#include "Common.h"
#include "ItemTemplate.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "PlayerbotAI.h"
#include "SpellInfo.h"

std::map<std::string, uint32> ChatHelper::consumableSubClasses{
    { "potion", ITEM_SUBCLASS_POTION },
    { "elixir", ITEM_SUBCLASS_ELIXIR },
    { "flask", ITEM_SUBCLASS_FLASK },
    { "scroll", ITEM_SUBCLASS_SCROLL },
    { "food", ITEM_SUBCLASS_FOOD },
    { "bandage", ITEM_SUBCLASS_BANDAGE },
    { "enchant", ITEM_SUBCLASS_CONSUMABLE_OTHER },
};
std::map<std::string, uint32> ChatHelper::itemQualities{
    { "poor", ITEM_QUALITY_POOR },
    { "gray", ITEM_QUALITY_POOR },
    { "normal", ITEM_QUALITY_NORMAL },
    { "white", ITEM_QUALITY_NORMAL },
    { "uncommon", ITEM_QUALITY_UNCOMMON },
    { "green", ITEM_QUALITY_UNCOMMON },
    { "rare", ITEM_QUALITY_RARE },
    { "blue", ITEM_QUALITY_RARE },
    { "epic", ITEM_QUALITY_EPIC },
    { "violet", ITEM_QUALITY_EPIC },
    { "legendary", ITEM_QUALITY_LEGENDARY },
    { "yellow", ITEM_QUALITY_LEGENDARY },
};
std::map<std::string, uint32> ChatHelper::projectileSubClasses{
    { "arrows", ITEM_SUBCLASS_ARROW },
    { "bullets", ITEM_SUBCLASS_BULLET },
};
std::map<std::string, uint32> ChatHelper::slots{
    { "head", EQUIPMENT_SLOT_HEAD },
    { "neck", EQUIPMENT_SLOT_NECK },
    { "shoulder", EQUIPMENT_SLOT_SHOULDERS },
    { "shirt", EQUIPMENT_SLOT_BODY },
    { "chest", EQUIPMENT_SLOT_CHEST },
    { "waist", EQUIPMENT_SLOT_WAIST },
    { "legs", EQUIPMENT_SLOT_LEGS },
    { "feet", EQUIPMENT_SLOT_FEET },
    { "wrist", EQUIPMENT_SLOT_WRISTS },
    { "hands", EQUIPMENT_SLOT_HANDS },
    { "finger 1", EQUIPMENT_SLOT_FINGER1 },
    { "finger 2", EQUIPMENT_SLOT_FINGER2 },
    { "trinket 1", EQUIPMENT_SLOT_TRINKET1 },
    { "trinket 2", EQUIPMENT_SLOT_TRINKET2 },
    { "back", EQUIPMENT_SLOT_BACK },
    { "main hand", EQUIPMENT_SLOT_MAINHAND },
    { "off hand", EQUIPMENT_SLOT_OFFHAND },
    { "ranged", EQUIPMENT_SLOT_RANGED },
    { "tabard", EQUIPMENT_SLOT_TABARD },
};
std::map<std::string, uint32> ChatHelper::skills{
    { "first aid", SKILL_FIRST_AID },
    { "fishing", SKILL_FISHING },
    { "cooking", SKILL_COOKING },
    { "alchemy", SKILL_ALCHEMY },
    { "enchanting", SKILL_ENCHANTING },
    { "engineering", SKILL_ENGINEERING },
    { "leatherworking", SKILL_LEATHERWORKING },
    { "blacksmithing", SKILL_BLACKSMITHING },
    { "tailoring", SKILL_TAILORING },
    { "herbalism", SKILL_HERBALISM },
    { "mining", SKILL_MINING },
    { "skinning", SKILL_SKINNING },
    { "jewelcrafting", SKILL_JEWELCRAFTING },
};
std::map<std::string, ChatMsg> ChatHelper::chats{
    { "party", CHAT_MSG_PARTY },
    { "p", CHAT_MSG_PARTY },
    { "guild", CHAT_MSG_GUILD },
    { "g", CHAT_MSG_GUILD },
    { "raid", CHAT_MSG_RAID },
    { "r", CHAT_MSG_RAID },
    { "whisper", CHAT_MSG_WHISPER },
    { "w", CHAT_MSG_WHISPER },
};
std::map<uint8, std::string> ChatHelper::classes{
    { CLASS_DRUID, "druid" },
    { CLASS_HUNTER, "hunter" },
    { CLASS_MAGE, "mage" },
    { CLASS_PALADIN, "paladin" },
    { CLASS_PRIEST, "priest" },
    { CLASS_ROGUE, "rogue" },
    { CLASS_SHAMAN, "shaman" },
    { CLASS_WARLOCK, "warlock" },
    { CLASS_WARRIOR, "warrior" },
    { CLASS_DEATH_KNIGHT, "dk" },
};
std::map<uint8, std::string> ChatHelper::races{
    { RACE_DWARF, "Dwarf" },
    { RACE_GNOME, "Gnome" },
    { RACE_HUMAN, "Human" },
    { RACE_NIGHTELF, "Night Elf" },
    { RACE_ORC, "Orc" },
    { RACE_TAUREN, "Tauren" },
    { RACE_TROLL, "Troll" },
    { RACE_UNDEAD_PLAYER, "Undead" },
    { RACE_BLOODELF, "Blood Elf" },
    { RACE_DRAENEI, "Draenei" },
};
std::map<uint8, std::map<uint8, std::string> > ChatHelper::specs{
    {
        CLASS_DRUID,
        {
            { DRUID_TAB_BALANCE, "balance" },
            { DRUID_TAB_FERAL, "feral combat" },
            { DRUID_TAB_RESTORATION, "restoration" },
        }
    },
    {
        CLASS_HUNTER,
        {
            { HUNTER_TAB_BEAST_MASTERY, "beast mastery" },
            { HUNTER_TAB_MARKSMANSHIP, "marksmanship" },
            { HUNTER_TAB_SURVIVAL, "survival" },
        }
    },
    {
        CLASS_MAGE,
        {
            { MAGE_TAB_ARCANE, "arcane" },
            { MAGE_TAB_FIRE, "fire" },
            { MAGE_TAB_FROST, "frost" },
        }
    },
    {
        CLASS_PALADIN,
        {
            { PALADIN_TAB_HOLY, "holy" },
            { PALADIN_TAB_PROTECTION, "protection" },
            { PALADIN_TAB_RETRIBUTION, "retribution" },
        }
    },
    {
        CLASS_PRIEST,
        {
            { PRIEST_TAB_DISCIPLINE, "discipline" },
            { PRIEST_TAB_HOLY, "holy" },
            { PRIEST_TAB_SHADOW, "shadow" },
        }
    },
    {
        CLASS_ROGUE,
        {
            { ROGUE_TAB_ASSASSINATION, "assasination" },
            { ROGUE_TAB_COMBAT, "combat" },
            { ROGUE_TAB_SUBTLETY, "subtlety" },
        }
    },
    {
        CLASS_SHAMAN,
        {
            { SHAMAN_TAB_ELEMENTAL, "elemental" },
            { SHAMAN_TAB_ENHANCEMENT, "enhancement" },
            { SHAMAN_TAB_RESTORATION, "restoration" },
        }
    },
    {
        CLASS_WARLOCK,
        {
            { WARLOCK_TAB_AFFLICTION, "affliction" },
            { WARLOCK_TAB_DEMONOLOGY, "demonology" },
            { WARLOCK_TAB_DESTRUCTION, "destruction" },
        }
    },
    {
        CLASS_WARRIOR,
        {
            { WARRIOR_TAB_ARMS, "arms" },
            { WARRIOR_TAB_FURY, "fury" },
            { WARRIOR_TAB_PROTECTION, "protection" },
        }
    },
    {
        CLASS_DEATH_KNIGHT,
        {
            { DEATH_KNIGHT_TAB_BLOOD, "blood" },
            { DEATH_KNIGHT_TAB_FROST, "frost" },
            { DEATH_KNIGHT_TAB_UNHOLY, "unholy" },
        }
    }
};

template <class T>
static bool substrContainsInMap(std::string const searchTerm, std::map<std::string, T> searchIn)
{
    for (typename std::map<std::string, T>::iterator i = searchIn.begin(); i != searchIn.end(); ++i)
    {
        std::string const term = i->first;
        if (term.size() > 1 && searchTerm.find(term) != std::string::npos)
            return true;
    }

    return false;
}

std::string const ChatHelper::formatMoney(uint32 copper)
{
    std::ostringstream out;
    if (!copper)
    {
        out << "0";
        return out.str();
    }

    uint32 gold = uint32(copper / 10000);
    copper -= (gold * 10000);
    uint32 silver = uint32(copper / 100);
    copper -= (silver * 100);

    bool space = false;
    if (gold > 0)
    {
        out << gold << "g";
        space = true;
    }

    if (silver > 0 && gold < 50)
    {
        if (space)
            out << " ";

        out << silver << "s";
        space = true;
    }

    if (copper > 0 && gold < 10)
    {
        if (space)
            out << " ";

        out << copper << "c";
    }

    return out.str();
}

std::string ChatHelper::parseValue(const std::string& type, const std::string& text)
{
    std::string retString;

    std::string pattern = "Hvalue:" + type + ":";

    int pos = text.find(pattern, 0);
    if (pos == -1)
        return retString;

    pos += pattern.size();

    int endPos = text.find('|', pos);
    if (endPos == -1)
        return retString;

    retString = text.substr(pos, endPos - pos);
    return retString;
}

uint32 ChatHelper::parseMoney(std::string const text)
{
    // if user specified money in ##g##s##c format
    std::string acum = "";
    uint32 copper = 0;
    for (uint8 i = 0; i < text.length(); i++)
    {
        if (text[i] == 'g')
        {
            copper += (atol(acum.c_str()) * 100 * 100);
            acum = "";
        }
        else if (text[i] == 'c')
        {
            copper += atol(acum.c_str());
            acum = "";
        }
        else if (text[i] == 's')
        {
            copper += (atol(acum.c_str()) * 100);
            acum = "";
        }
        else if (text[i] == ' ')
            break;
        else if (text[i] >= 48 && text[i] <= 57)
            acum += text[i];
        else
        {
            copper = 0;
            break;
        }
    }
    return copper;
}

ItemIds ChatHelper::parseItems(std::string const text)
{
    ItemIds itemIds;

    uint8 pos = 0;
    while (true)
    {
        auto i = text.find("Hitem:", pos);
        if (i == std::string::npos)
            break;

        pos = i + 6;
        auto endPos = text.find(':', pos);
        if (endPos == std::string::npos)
            break;

        std::string const idC = text.substr(pos, endPos - pos);
        auto id = atol(idC.c_str());
        pos = endPos;
        if (id)
            itemIds.insert(id);
    }

    return itemIds;
}

ItemWithRandomProperty ChatHelper::parseItemWithRandomProperty(std::string const text)
{
    ItemWithRandomProperty res;

    size_t itemStart = text.find("Hitem:");
    if (itemStart == std::string::npos)
        return res;

    itemStart += 6;
    if (itemStart >= text.length())
        return res;

    size_t colonPos = text.find(':', itemStart);
    if (colonPos == std::string::npos)
        return res;

    std::string itemIdStr = text.substr(itemStart, colonPos - itemStart);
    res.itemId = atoi(itemIdStr.c_str());

    std::vector<std::string> params;
    size_t currentPos = colonPos + 1;

    while (currentPos < text.length()) {
        size_t nextColon = text.find(':', currentPos);
        if (nextColon == std::string::npos)
        {
            size_t hTag = text.find("|h", currentPos);
            if (hTag != std::string::npos)
            {
                params.push_back(text.substr(currentPos, hTag - currentPos));
            }
            break;
        }

        params.push_back(text.substr(currentPos, nextColon - currentPos));
        currentPos = nextColon + 1;
    }

    if (params.size() >= 6)
    {
        res.randomPropertyId = atoi(params[5].c_str());
    }

    return res;
}

std::string const ChatHelper::FormatQuest(Quest const* quest)
{
    if (!quest)
    {
        return "Invalid quest";
    }

    std::ostringstream out;
    QuestLocale const* locale = sObjectMgr->GetQuestLocale(quest->GetQuestId());
    std::string questTitle;

    if (locale && locale->Title.size() > sWorld->GetDefaultDbcLocale())
        questTitle = locale->Title[sWorld->GetDefaultDbcLocale()];

    if (questTitle.empty())
        questTitle = quest->GetTitle();

    out << "|cFFFFFF00|Hquest:" << quest->GetQuestId() << ':' << quest->GetQuestLevel() << "|h[" << questTitle << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatGameobject(GameObject* go)
{
    std::ostringstream out;
    out << "|cFFFFFF00|Hfound:" << go->GetGUID().GetRawValue() << ":" << go->GetEntry() << ":"
        << "|h[" << go->GetNameForLocaleIdx(sWorld->GetDefaultDbcLocale()) << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatWorldobject(WorldObject* wo)
{
    std::ostringstream out;
    out << "|cFFFFFF00|Hfound:" << wo->GetGUID().GetRawValue() << ":" << wo->GetEntry() << ":"
        << "|h[";
    out << (wo->ToGameObject() ? ((GameObject*)wo)->GetNameForLocaleIdx(sWorld->GetDefaultDbcLocale())
                               : wo->GetNameForLocaleIdx(sWorld->GetDefaultDbcLocale()))
        << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatWorldEntry(int32 entry)
{
    CreatureTemplate const* cInfo = nullptr;
    GameObjectTemplate const* gInfo = nullptr;

    if (entry > 0)
        cInfo = sObjectMgr->GetCreatureTemplate(entry);
    else
        gInfo = sObjectMgr->GetGameObjectTemplate(entry * -1);

    std::ostringstream out;
    out << "|cFFFFFF00|Hentry:" << abs(entry) << ":"
        << "|h[";

    if (entry < 0 && gInfo)
        out << gInfo->name;
    else if (entry > 0 && cInfo)
        out << cInfo->Name;
    else
        out << "unknown";

    out << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatSpell(SpellInfo const* spellInfo)
{
    std::ostringstream out;
    std::string spellName = spellInfo->SpellName[sWorld->GetDefaultDbcLocale()] ?
        spellInfo->SpellName[sWorld->GetDefaultDbcLocale()] : spellInfo->SpellName[LOCALE_enUS];
    out << "|cffffffff|Hspell:" << spellInfo->Id << "|h[" << spellName << "]|h|r";
    return out.str();
}

std::string const ChatHelper::FormatItem(ItemTemplate const* proto, uint32 count, uint32 total)
{
    char color[32];
    snprintf(color, sizeof(color), "%x", ItemQualityColors[proto->Quality]);

    std::string itemName;
    const ItemLocale* locale = sObjectMgr->GetItemLocale(proto->ItemId);

    if (locale && locale->Name.size() > sWorld->GetDefaultDbcLocale())
        itemName = locale->Name[sWorld->GetDefaultDbcLocale()];

    if (itemName.empty())
        itemName = proto->Name1;

    std::ostringstream out;
    out << "|c" << color << "|Hitem:" << proto->ItemId << ":0:0:0:0:0:0:0"
        << "|h[" << itemName << "]|h|r";

    if (count > 1)
        out << "x" << count;

    if (total > 0)
        out << " (" << total << ")";

    return out.str();
}

std::string const ChatHelper::FormatQItem(uint32 itemId)
{
    char color[32];
    snprintf(color, sizeof(color), "%x", ItemQualityColors[0]);

    std::ostringstream out;
    out << "|c" << color << "|Hitem:" << itemId << ":0:0:0:0:0:0:0"
        << "|h[item"
        << "]|h|r";

    return out.str();
}

ChatMsg ChatHelper::parseChat(std::string const text)
{
    if (chats.find(text) != chats.end())
        return chats[text];

    return CHAT_MSG_SYSTEM;
}

std::string const ChatHelper::FormatChat(ChatMsg chat)
{
    switch (chat)
    {
        case CHAT_MSG_GUILD:
            return "guild";
        case CHAT_MSG_PARTY:
            return "party";
        case CHAT_MSG_WHISPER:
            return "whisper";
        case CHAT_MSG_RAID:
            return "raid";
        default:
            break;
    }

    return "unknown";
}

GuidVector ChatHelper::parseGameobjects(std::string const text)
{
    GuidVector gos;
    //    Link format
    //    |cFFFFFF00|Hfound:" << guid << ':'  << entry << ':'  <<  "|h[" << gInfo->name << "]|h|r";
    //    |cFFFFFF00|Hfound:9582:1731|h[Copper Vein]|h|r

    uint8 pos = 0;
    while (true)
    {
        // extract GO guid
        auto i = text.find("Hfound:", pos);  // base H = 11
        if (i == std::string::npos)          // break if error
            break;

        pos = i + 7;                        // start of window in text 11 + 7 = 18
        auto endPos = text.find(':', pos);  // end of window in text 22
        if (endPos == std::string::npos)    // break if error
            break;

        std::istringstream stream(text.substr(pos, endPos - pos));
        uint64 guid;
        stream >> guid;

        // extract GO entry
        pos = endPos + 1;
        endPos = text.find(':', pos);     // end of window in text
        if (endPos == std::string::npos)  // break if error
            break;

        std::string const entryC = text.substr(pos, endPos - pos);  // get std::string const within window i.e entry
        //uint32 entry = atol(entryC.c_str());                        // convert ascii to float

        ObjectGuid lootCurrent = ObjectGuid(guid);

        if (guid)
            gos.push_back(lootCurrent);
    }

    return gos;
}

std::string const ChatHelper::FormatQuestObjective(std::string const name, uint32 available, uint32 required)
{
    std::ostringstream out;
    out << "|cFFFFFFFF" << name << (available >= required ? "|c0000FF00: " : "|c00FF0000: ") << available << "/"
        << required << "|r";

    return out.str();
}

uint32 ChatHelper::parseItemQuality(std::string const text)
{
    if (itemQualities.find(text) == itemQualities.end())
        return MAX_ITEM_QUALITY;

    return itemQualities[text];
}

bool ChatHelper::parseItemClass(std::string const text, uint32* itemClass, uint32* itemSubClass)
{
    if (text == "questitem")
    {
        *itemClass = ITEM_CLASS_QUEST;
        *itemSubClass = ITEM_SUBCLASS_QUEST;
        return true;
    }

    if (consumableSubClasses.find(text) != consumableSubClasses.end())
    {
        *itemClass = ITEM_CLASS_CONSUMABLE;
        *itemSubClass = consumableSubClasses[text];
        return true;
    }

    if (projectileSubClasses.find(text) != projectileSubClasses.end())
    {
        *itemClass = ITEM_CLASS_PROJECTILE;
        *itemSubClass = projectileSubClasses[text];
        return true;
    }

    return false;
}

uint32 ChatHelper::parseSlot(std::string const text)
{
    if (slots.find(text) != slots.end())
        return slots[text];

    return EQUIPMENT_SLOT_END;
}

bool ChatHelper::parseableItem(std::string const text)
{
    return text.find("|Hitem:") != std::string::npos || text == "questitem" || text == "ammo" ||
           substrContainsInMap<uint32>(text, consumableSubClasses) ||
           substrContainsInMap<uint32>(text, itemQualities) ||
           substrContainsInMap<uint32>(text, slots) || substrContainsInMap<ChatMsg>(text, chats) ||
           substrContainsInMap<uint32>(text, skills) || parseMoney(text) > 0;
}

std::string const ChatHelper::FormatClass(Player* player, int8 spec)
{
    uint8 cls = player->getClass();

    std::ostringstream out;
    out << specs[cls][spec] << " (";

    std::map<uint8, uint32> tabs = AiFactory::GetPlayerSpecTabs(player);
    uint32 c0 = tabs[0];
    uint32 c1 = tabs[1];
    uint32 c2 = tabs[2];

    out << (c0 ? "|h|cff00ff00" : "") << c0 << "|h|cffffffff/";
    out << (c1 ? "|h|cff00ff00" : "") << c1 << "|h|cffffffff/";
    out << (c2 ? "|h|cff00ff00" : "") << c2 << "|h|cffffffff";

    out << ")|r " << classes[cls];
    return out.str();
}

std::string const ChatHelper::FormatClass(uint8 cls) { return classes[cls]; }

std::string const ChatHelper::FormatRace(uint8 race) { return races[race]; }

uint32_t ChatHelper::ParseSkill(std::string const text)
{
    if (skills.find(text) != skills.end())
        return skills[text];

    return SKILL_NONE;
}

std::string const ChatHelper::FormatSkill(uint32 skill)
{
    for (std::map<std::string, uint32>::iterator i = skills.begin(); i != skills.end(); ++i)
    {
        if (i->second == skill)
            return i->first;
    }

    return "";
}

std::string const ChatHelper::FormatBoolean(bool flag) { return flag ? "|cff00ff00ON|r" : "|cffffff00OFF|r"; }

void ChatHelper::EraseAllSubStr(std::string& mainStr, std::string const toErase)
{
    size_t pos = std::string::npos;

    // Search for the substring in std::string const in a loop untill nothing is found
    while ((pos = mainStr.find(toErase)) != std::string::npos)
    {
        // If found then erase it from std::string
        mainStr.erase(pos, toErase.length());
    }
}

std::set<uint32> extractGeneric(std::string_view text, std::string_view prefix)
{
    std::set<uint32_t> ids;
    std::string_view text_view = text;

    size_t pos = 0;
    while ((pos = text_view.find(prefix, pos)) != std::string::npos)
    {
        // skip "Hquest:/Hitem:"
        pos += prefix.size();

        // extract everything after "Hquest:/Hitem:"
        size_t end_pos = text_view.find_first_not_of("0123456789", pos);
        std::string_view number_str = text_view.substr(pos, end_pos - pos);

        uint32 number = 0;

        auto [ptr, ec] = std::from_chars(number_str.data(), number_str.data() + number_str.size(), number);

        if (ec == std::errc())
        {
            ids.insert(number);
        }
        pos = end_pos;
    }

    return ids;
}

std::set<uint32> ChatHelper::ExtractAllQuestIds(const std::string& text)
{
    return extractGeneric(text, "Hquest:");
}

std::set<uint32> ChatHelper::ExtractAllItemIds(const std::string& text)
{
    return extractGeneric(text, "Hitem:");
}
