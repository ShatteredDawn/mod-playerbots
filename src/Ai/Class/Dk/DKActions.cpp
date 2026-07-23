/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "DKActions.h"

#include "CreateNextAction.h"
#include "GenericSpellActions.h"
#include "Playerbots.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

std::vector<NextAction> CastDeathchillAction::getPrerequisites()
{
    std::vector<NextAction> prerequisites = CastBuffSpellAction::getPrerequisites();

    prerequisites.emplace_back(CreateNextAction<CastFrostPresenceAction>(1.0f));

    return prerequisites;
}

std::vector<NextAction> CastUnholyMeleeSpellAction::getPrerequisites()
{
    std::vector<NextAction> prerequisites = CastMeleeSpellAction::getPrerequisites();

    prerequisites.emplace_back(CreateNextAction<CastUnholyPresenceAction>(1.0f));

    return prerequisites;
}

std::vector<NextAction> CastFrostMeleeSpellAction::getPrerequisites()
{
    std::vector<NextAction> prerequisites = CastMeleeSpellAction::getPrerequisites();

    prerequisites.emplace_back(CreateNextAction<CastFrostPresenceAction>(1.0f));

    return prerequisites;
}

std::vector<NextAction> CastBloodMeleeSpellAction::getPrerequisites()
{
    std::vector<NextAction> prerequisites = CastMeleeSpellAction::getPrerequisites();
    prerequisites.emplace_back(CreateNextAction<CastBloodPresenceAction>(1.0f));

    return prerequisites;
}

bool CastRaiseDeadAction::Execute(Event event)
{
    const bool result = CastBuffSpellAction::Execute(event);

    if (!result)
        return false;

    const uint32_t spellId = AI_VALUE2(uint32_t, "spell id", spell);

    bot->AddSpellCooldown(spellId, 0, 3 * 60 * 1000);

    return true;
}

Unit* CastHysteriaAction::GetTarget()
{
    Group* const group = this->bot->GetGroup();

    if (group == nullptr)
    {
        // @TODO: Use an enum instead of a magic number
        if (this->bot->HasAura(49016))
        {
            return nullptr;
        }

        return this->bot;
    }

    for (const GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
    {
        Player* const member = ref->GetSource();

        if (member == nullptr || !member->IsAlive())
        {
            continue;
        }

        if (member->GetMap() != this->bot->GetMap() || this->bot->GetDistance(member) > PlayerbotAIConfig::instance().spellDistance)
        {
            continue;
        }

        // @TODO: Use an enum instead of a magic number
        // Skip if already has hysteria
        if (member->HasAura(49016))
        {
            continue;
        }

        // Priority 1: Melee DPS
        if (PlayerbotAI::IsMelee(member) && PlayerbotAI::IsDps(member))
        {
            return member;
        }

        // Priority 2: Ranged DPS (physical, not casters)
        if (PlayerbotAI::IsRanged(member) && PlayerbotAI::IsDps(member) && !PlayerbotAI::IsCaster(member))
        {
            return member;
        }

        // Priority 3: Tank
        if (PlayerbotAI::IsTank(member))
        {
            return member;
        }
    }

    // @TODO: Use an enum instead of a magic number
    // Fallback to self if no hysteria
    if (!this->bot->HasAura(49016))
    {
        return this->bot;
    }

    return nullptr;
}
