#pragma once

#include "DruidActions.h"
#include "HunterActions.h"
#include "MageActions.h"
#include "PaladinActions.h"
#include "PriestActions.h"
#include "RogueActions.h"
#include "Spell.h"
#include "AiObjectContext.h"
#include "AttackAction.h"
#include "PlayerbotAI.h"

#include "WarlockActions.h"

#include "domain/core/raid/leader/RaidLeaderRegistry.h"

class HakkarTheSoulflayerPullSonOfHakkarAction : public AttackAction
{
public:
    HakkarTheSoulflayerPullSonOfHakkarAction(
        PlayerbotAI* botAI,
        const std::string name = "hakkar the soulflayer pull son of hakkar"
    ) : AttackAction(botAI, name) {}

    bool Execute(Event) override
    {
        if (this->bot == nullptr)
        {
            return false;
        }

        const uint32_t instanceId = this->bot->GetInstanceId();
        RaidLeaderRegistry& raidRegistry = RaidLeaderRegistry::GetInstance();
        const ZulGurubRaidLeader& raidLeader = raidRegistry.getOrBind<ZulGurubRaidLeader>(instanceId, MAP_ZUL_GURUB);
        const SonOfHakkarAssistant& sonOfHakkarAssistant = raidLeader.getSonOfHakkarAssistant();
        const SonOfHakkarMemory& sonOfHakkarMemory = sonOfHakkarAssistant.getMemory();

        Creature* const sonOfHakkar = sonOfHakkarMemory.getActiveSonOfHakkar();

        if (sonOfHakkar == nullptr)
        {
            return false;
        }

        const bool meleeBot = PlayerbotAI::IsMelee(this->bot);

        this->bot->SetTarget(sonOfHakkar->GetGUID());

        if (meleeBot)
        {
            return this->Attack(sonOfHakkar);
        }

        const uint8_t botClassInfo = this->bot->getClass();

        if (botClassInfo == CLASS_PRIEST)
        {
            const SpellInfo* const shadowWordPainSpellInfo = SpellMgr::instance()->GetSpellInfo(589);

            this->bot->CastSpell(sonOfHakkar, shadowWordPainSpellInfo);
        }

        if (botClassInfo == CLASS_WARLOCK)
        {
            const SpellInfo* const corruptionSpellInfo = SpellMgr::instance()->GetSpellInfo(172);

            this->bot->CastSpell(sonOfHakkar, corruptionSpellInfo);
        }

        if (botClassInfo == CLASS_MAGE)
        {
            const SpellInfo* const fireBlastRank1SpellInfo = SpellMgr::instance()->GetSpellInfo(2136);

            this->bot->CastSpell(sonOfHakkar, fireBlastRank1SpellInfo);
        }

        if (botClassInfo == CLASS_HUNTER)
        {
            const SpellInfo* const serpentStingRank1SpellInfo = SpellMgr::instance()->GetSpellInfo(1978);

            this->bot->CastSpell(sonOfHakkar, serpentStingRank1SpellInfo);
        }

        if (botClassInfo == CLASS_DRUID)
        {
            const SpellInfo* const moonfireRank1SpellInfo = SpellMgr::instance()->GetSpellInfo(8921);

            this->bot->CastSpell(sonOfHakkar, moonfireRank1SpellInfo);
        }

        if (botClassInfo == CLASS_PALADIN)
        {
            const SpellInfo* const handOfReckoningSpellInfo = SpellMgr::instance()->GetSpellInfo(62124);

            this->bot->CastSpell(sonOfHakkar, handOfReckoningSpellInfo);
        }

        return true;
    }
};
