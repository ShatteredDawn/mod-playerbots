/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "WarriorTriggers.h"

#include "Playerbots.h"

namespace
{
constexpr uint32 SPELL_VIGILANCE = 50720;
constexpr uint32 SPELL_SHATTERING_THROW = 64382;
constexpr uint32 SPELL_DIVINE_SHIELD = 642;
constexpr uint32 SPELL_ICE_BLOCK = 45438;
constexpr uint32 SPELL_BLESSING_OF_PROTECTION = 41450;
constexpr uint32 SPELL_COMMANDING_PRESENCE_RANKS[] = { 12318, 12857, 12858, 12860, 12861 };
}

bool BloodrageBuffTrigger::IsActive()
{
    return AI_VALUE2(uint8, "health", "self target") >= sPlayerbotAIConfig.mediumHealth &&
           AI_VALUE2(uint8, "rage", "self target") < 20;
}

bool VigilanceTrigger::IsActive()
{
    if (!bot->HasSpell(SPELL_VIGILANCE))
    {
        return false;
    }

    Group* group = bot->GetGroup();
    if (!group)
    {
        return false;
    }

    Player* currentVigilanceTarget = nullptr;
    Player* mainTank = nullptr;
    Player* assistTank1 = nullptr;
    Player* assistTank2 = nullptr;
    Player* highestGearScorePlayer = nullptr;
    uint32 highestGearScore = 0;

    // Iterate once through the group to gather all necessary information
    for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
    {
        Player* member = ref->GetSource();
        if (!member || member == bot || !member->IsAlive())
            continue;

        // Check if member has Vigilance applied by the bot
        if (!currentVigilanceTarget && botAI->HasAura("vigilance", member, false, true))
        {
            currentVigilanceTarget = member;
        }

        // Identify Main Tank
        if (!mainTank && botAI->IsMainTank(member))
        {
            mainTank = member;
        }

        // Identify Assist Tanks
        if (assistTank1 == nullptr && botAI->IsAssistTankOfIndex(member, 0))
        {
            assistTank1 = member;
        }
        else if (assistTank2 == nullptr && botAI->IsAssistTankOfIndex(member, 1))
        {
            assistTank2 = member;
        }

        // Determine Highest Gear Score
        uint32 gearScore = botAI->GetEquipGearScore(member/*, false, false*/);
        if (gearScore > highestGearScore)
        {
            highestGearScore = gearScore;
            highestGearScorePlayer = member;
        }
    }

    // Determine the highest-priority target
    Player* highestPriorityTarget = mainTank ? mainTank :
                                      (assistTank1 ? assistTank1 :
                                      (assistTank2 ? assistTank2 : highestGearScorePlayer));

    // Trigger if no Vigilance is active or the current target is not the highest-priority target
    if (!currentVigilanceTarget || currentVigilanceTarget != highestPriorityTarget)
    {
        return true;
    }

    return false; // No need to reassign Vigilance
}

bool ShatteringThrowTrigger::IsActive()
{
    // Spell cooldown check
    if (!bot->HasSpell(SPELL_SHATTERING_THROW))
    {
        return false;
    }

    // Spell cooldown check
    if (bot->HasSpellCooldown(SPELL_SHATTERING_THROW))
    {
        return false;
    }

    GuidVector enemies = AI_VALUE(GuidVector, "possible targets");

    for (ObjectGuid const& guid : enemies)
    {
        Unit* enemy = botAI->GetUnit(guid);
        if (!enemy || !enemy->IsAlive() || enemy->IsFriendlyTo(bot))
            continue;

        // Check if the enemy is within 25 yards and has the specific auras
        if (bot->IsWithinDistInMap(enemy, 25.0f) &&
            (enemy->HasAura(SPELL_DIVINE_SHIELD) ||
             enemy->HasAura(SPELL_ICE_BLOCK) ||
             enemy->HasAura(SPELL_BLESSING_OF_PROTECTION)))
        {
            return true;
        }
    }

    return false;
}

bool BattleShoutTrigger::IsActive()
{
    if (!BuffTrigger::IsActive())
        return false;

    uint32 battleShoutSpellId = AI_VALUE2(uint32, "spell id", "battle shout");
    if (!battleShoutSpellId)
        return false;

    SpellInfo const* bsInfo = sSpellMgr->GetSpellInfo(battleShoutSpellId);
    if (!bsInfo)
        return false;

    int32 bsApValue = 0;
    for (uint8 eff = 0; eff < MAX_SPELL_EFFECTS; ++eff)
    {
        if (bsInfo->Effects[eff].ApplyAuraName == SPELL_AURA_MOD_ATTACK_POWER)
        {
            bsApValue = bsInfo->Effects[eff].BasePoints + 1;
            break;
        }
    }
    if (!bsApValue)
        return false;

    static const float commandingPresenceBonus[]   = {
        0.05f, 0.10f, 0.15f, 0.20f, 0.25f };

    float cpBonus = 0.0f;
    for (int rank = 4; rank >= 0; --rank)
    {
        if (bot->HasAura(SPELL_COMMANDING_PRESENCE_RANKS[rank]))
        {
            cpBonus = commandingPresenceBonus[rank];
            break;
        }
    }
    int32 effectiveBsAp = int32(bsApValue * (1.0f + cpBonus));

    static const char* blessingNames[] = {
        "blessing of might", "greater blessing of might", nullptr
    };
    for (int i = 0; blessingNames[i] != nullptr; ++i)
    {
        Aura* bom = botAI->GetAura(blessingNames[i], bot);
        if (!bom)
            continue;

        SpellInfo const* bomInfo = bom->GetSpellInfo();
        if (!bomInfo)
            continue;

        for (uint8 eff = 0; eff < MAX_SPELL_EFFECTS; ++eff)
        {
            if (bomInfo->Effects[eff].ApplyAuraName == SPELL_AURA_MOD_ATTACK_POWER)
            {
                int32 bomApValue = bomInfo->Effects[eff].BasePoints + 1;
                if (bomApValue >= effectiveBsAp)
                    return false;
                break;
            }
        }
    }

    return true;
}
