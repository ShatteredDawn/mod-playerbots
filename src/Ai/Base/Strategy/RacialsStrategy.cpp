/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "RacialsStrategy.h"
#include "GenericSpellActions.h"

namespace
{
    constexpr uint32 SPELL_ARCANE_TORRENT_ENERGY = 25046;
    constexpr uint32 SPELL_ARCANE_TORRENT_MANA = 28730;
    constexpr uint32 SPELL_ARCANE_TORRENT_RUNIC_POWER = 50613;
    constexpr uint32 SPELL_WAR_STOMP = 20549;
    constexpr uint32 SPELL_BERSERKING = 26297;
    constexpr uint32 SPELL_EVERY_MAN_FOR_HIMSELF = 59752;
    constexpr uint32 SPELL_WILL_OF_THE_FORSAKEN = 7744;
    constexpr uint32 SPELL_STONEFORM = 20594;
    constexpr uint32 SPELL_ESCAPE_ARTIST = 20589;
}

RacialsStrategy::RacialsStrategy(PlayerbotAI* botAI) : Strategy(botAI)
{
    // No custom ActionNodeFactory needed
}

void RacialsStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    Player* bot = botAI->GetBot();

    if (bot->HasSpell(SPELL_ARCANE_TORRENT_MANA))
    {
        triggers.push_back(
            new TriggerNode(
                "low mana",
                {
                    CreateNextAction<CastArcaneTorrentAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_ARCANE_TORRENT_ENERGY))
    {
        triggers.push_back(
            new TriggerNode(
                "low energy",
                {
                    CreateNextAction<CastArcaneTorrentAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_ARCANE_TORRENT_RUNIC_POWER))
    {
        // No low runic power trigger exists; this trigger should be modified if one is added
        triggers.push_back(
            new TriggerNode(
                "generic boost",
                {
                    CreateNextAction<CastArcaneTorrentAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_WAR_STOMP))
    {
        triggers.push_back(
            new TriggerNode(
                "medium aoe",
                {
                    CreateNextAction<CastWarStompAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_BERSERKING))
    {
        triggers.push_back(
            new TriggerNode(
                "generic boost",
                {
                    CreateNextAction<CastWarStompAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_EVERY_MAN_FOR_HIMSELF))
    {
        triggers.push_back(
            new TriggerNode(
                "loss of control",
                {
                    CreateNextAction<CastEveryManForHimselfAction>(ACTION_EMERGENCY + 1.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_WILL_OF_THE_FORSAKEN))
    {
        triggers.push_back(
            new TriggerNode(
                "fear charm sleep",
                {
                    CreateNextAction<CastWillOfTheForsakenAction>(ACTION_EMERGENCY + 1.0f)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_STONEFORM))
    {
        triggers.push_back(
            new TriggerNode(
                "poison disease bleed",
                {
                    CreateNextAction<CastStoneformAction>(ACTION_DISPEL)
                }
            )
        );
    }

    if (bot->HasSpell(SPELL_ESCAPE_ARTIST))
    {
        triggers.push_back(
            new TriggerNode(
                "movement impaired",
                {
                    CreateNextAction<CastEscapeArtistAction>(ACTION_EMERGENCY + 1.0f)
                }
            )
        );
    }

    if (botAI->HasSpell("blood fury"))
    {
        triggers.push_back(
            new TriggerNode(
                "generic boost",
                {
                    CreateNextAction<CastBloodFuryAction>(ACTION_NORMAL + 5.0f)
                }
            )
        );
    }

    if (botAI->HasSpell("gift of the naaru"))
    {
        // Currently targets self only
        triggers.push_back(
            new TriggerNode(
                "medium health",
                {
                    CreateNextAction<CastGiftOfTheNaaruAction>(ACTION_LIGHT_HEAL + 5.0f)
                }
            )
        );
    }

    if (botAI->HasSpell("lifeblood"))
    {
        triggers.push_back(
            new TriggerNode(
                "medium health",
                {
                    CreateNextAction<CastLifeBloodAction>(ACTION_LIGHT_HEAL + 5.0f)
                }
            )
        );
    }

    triggers.push_back(
        new TriggerNode(
            "generic boost",
            {
                CreateNextAction<UseTrinketAction>(ACTION_NORMAL + 4.0f)
            }
        )
);
}
